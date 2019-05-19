#include <unistd.h>
#include "WebSocketHandler.h"
#include "Protocol.h"
#include <string>
#include <string.h>
#include "../Common/Common.h"

std::list<int> WebsocketHandler::m_BroadcastList ; 
int  WebsocketHandler::m_ListenFd; 
bool  WebsocketHandler::m_First = true; 

WebsocketHandler::WebsocketHandler():
    m_pBuff(NULL),
    m_Status(WEBSOCKET_UNCONNECT),
    m_HeaderMap(),
    m_ClientFd(INVALID_SOCKET),
    m_pRequest(new WebsocketRequest()),
    m_pRespond(new WebsocketRespond())
{
    if(m_First)
    {
        m_ListenFd = m_ClientFd;
        m_BroadcastList.push_front(m_ListenFd);
        m_First = false;
    }
    else 
    {
        m_BroadcastList.push_front(m_ClientFd);
    }
}
bool WebsocketHandler::SetSocket(int nFd)
{
    if (nFd != INVALID_SOCKET)
    {
        m_ClientFd = nFd;
        return true;
    }

    return false;
}

bool WebsocketHandler::SetBuff(char* pBuff)
{
    if (pBuff)
    {
        m_pBuff = pBuff;
        return true;
    }

    return false;
}

WebsocketHandler::~WebsocketHandler()
{
    SAFE_DELETE(m_pRequest);
    SAFE_DELETE(m_pRespond);
}

int WebsocketHandler::Process()
{
    if(m_Status == WEBSOCKET_UNCONNECT)
    {
        return HandShark();
    }

    m_pRequest->FetchWebsocketInfo(m_pBuff);
    m_pRequest->PrintWebProtocol();

    unsigned char* pOut = NULL;
    size_t         unOutLen;
    std::string strRequestMsg = m_pRequest->GetMsg();
    //size_t   nRequestMsgLen=m_pRequest->GetMsgLength();

    //判断request请求opcode类型
    //当为1的时候 正常通信
    uint8_t ucMsgOpcode = m_pRequest->GetMsgOpcode();
    if(ucMsgOpcode == WEBSOCKET_TEXT_DATA)
    {
        string strReply = "";
        m_pRespond->PackData((const unsigned char*)strReply.c_str() ,strReply.length(), WEBSOCKET_FIN_MSG_END , 
                WEBSOCKET_TEXT_DATA , WEBSOCKET_NEED_NOT_MASK , &pOut, &unOutLen ); 

        SendData((char*)pOut, unOutLen);           //回显
//      SendBroadcastData((char*)pOut); //广播
        free(pOut);
    }
    else if (ucMsgOpcode == WEBSOCKET_CONNECT_CLOSE)
    {
        m_pRequest->PrintWebProtocol();
        DelBroadcastList();
    }

//  m_pRequest->Reset();                  //请求重置
    LOGDEBUG("3,now, in Handler::process, buff: %s, len: %d, pos: %d\n", m_pBuff, m_pRequest->GetMsgLength(), m_pRequest->GetPos());
//  memset(m_pBuff, 0, sizeof(m_pBuff));

    return 0;
}


int WebsocketHandler::HandShark()
{
    char szRequest[1024];
    memset(szRequest, 0, 1024);
    m_Status = WEBSOCKET_HANDSHARKED;
    FetchHttpInfo();
    ParseStr(szRequest);
    //memset(m_pBuff, 0, sizeof(m_pBuff));
    return SendData(szRequest, strlen(szRequest));
}

void WebsocketHandler::ParseStr(char *pRequest)
{  
    strcat(pRequest, "HTTP/1.1 101 Switching Protocols\r\n");
    strcat(pRequest, "Connection: upgrade\r\n");
    strcat(pRequest, "Sec-WebSocket-Accept: ");
    std::string strServerKey = m_HeaderMap["Sec-WebSocket-Key"];
    strServerKey += MAGIC_KEY;

    SHA1 sha;
    unsigned int unMessageDigest[5];
    sha.Reset();
    sha << strServerKey.c_str();

    sha.Result(unMessageDigest);
    for (int i = 0; i < 5; i++) {
        unMessageDigest[i] = htonl(unMessageDigest[i]);
    }
    strServerKey = base64_encode(reinterpret_cast<const unsigned char*>(unMessageDigest),20);
    strServerKey += "\r\n";
    strcat(pRequest, strServerKey.c_str());
    strcat(pRequest, "Upgrade: websocket\r\n\r\n");
}

int WebsocketHandler::FetchHttpInfo()
{
    std::istringstream szBuff(m_pBuff);
    std::string strRequest;

    std::getline(szBuff, strRequest);
    if (strRequest[strRequest.size()-1] == '\r') 
    {
        strRequest.erase(strRequest.end()-1);
    } 
    else 
    {
        return -1;
    }

    std::string strHeader;
    std::string::size_type unEnd;

    while (std::getline(szBuff, strHeader) && strHeader != "\r") 
    {
        if (strHeader[strHeader.size()-1] != '\r') 
        {
            continue; //unEnd
        } 
        else 
        {
            strHeader.erase(strHeader.end()-1);	//remove last char
        }

        unEnd = strHeader.find(": ",0);
        if (unEnd != std::string::npos) 
        {
            std::string strKey = strHeader.substr(0, unEnd);
            std::string strValue = strHeader.substr(unEnd+2);
            m_HeaderMap[strKey] = strValue;
        }
    }

    return 0;
}

int WebsocketHandler::SendData(char *pBuff, size_t unOutLen)
{
    return write(m_ClientFd, pBuff, unOutLen);
}

int WebsocketHandler::SendBroadcastData(char *pBuff)
{
    if(m_BroadcastList.empty())
    {
        printf("无客户端连接!");
        return -1;
    }

    try
    {
        list<int>::iterator iter;
        for(iter=m_BroadcastList.begin() ; iter!=m_BroadcastList.end() ; iter++)
        {
            if( (*iter == m_ClientFd)|| (*iter == m_ListenFd) )
                continue;
            write(*iter, pBuff, strlen(pBuff));
        }
    }
    catch(exception &e)
    {
        cout << e.what() << endl;
        return -1;
    }

    return 0;
}

int WebsocketHandler::DelBroadcastList()
{
    try
    {
        list<int>::iterator iter;
        for(iter=m_BroadcastList.begin() ; iter!=m_BroadcastList.end() ; iter++)
        {
            if( (*iter == m_ClientFd)&&( *iter !=m_ListenFd ))
            {
                iter=m_BroadcastList.erase(iter);
            }
        }
    }
    catch(exception &e)
    {
        cout << e.what() << endl;
        return -1;
    }

    return 0;
}
