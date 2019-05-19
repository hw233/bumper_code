#include "WebSocketRequest.h"
#include <string.h>
#include <stdio.h>
#include "../Common/Common.h"


WebsocketRequest::WebsocketRequest():
		m_ucFin(0),
		m_ucOpcode(0),
		m_ucMask(0),
		m_u64PayloadLength(0),
        m_nPos(0)
{
	memset(m_ucMaskingKeySet, 0, sizeof(m_ucMaskingKeySet));
	memset(m_szPayload, 0, sizeof(m_szPayload));
}

WebsocketRequest::~WebsocketRequest()
{

}

int WebsocketRequest::FetchWebsocketInfo(char* pMsg)
{
	m_nPos = 0;
	FetchFin(pMsg, m_nPos);
	FetchOpcode(pMsg, m_nPos);
	FetchMask(pMsg, m_nPos);
	FetchMaskingKey(pMsg, m_nPos);
	FetchMaskingKey(pMsg, m_nPos);
	return FetchPayload(pMsg, m_nPos);
}

void WebsocketRequest::PrintWebProtocol()
{
	return;
	LOGDEBUG("WEBSOCKET PROTOCOL\n"
				"FIN: %d\n"
				"OPCODE: %d\n"
				"MASK: %d\n"
				"PAYLOADLEN: %d\n"
				"PAYLOAD: %s",
				m_ucFin, m_ucOpcode, m_ucMask, m_u64PayloadLength, m_szPayload);

	//reset();
}

void WebsocketRequest::Reset(){
	m_ucFin = 0;
	m_ucOpcode = 0;
	m_ucMask = 0;
	memset(m_ucMaskingKeySet, 0, sizeof(m_ucMaskingKeySet));
	m_u64PayloadLength = 0;
	memset(m_szPayload, 0, sizeof(m_szPayload));
    m_nPos=0;
}

int WebsocketRequest::FetchFin(char *pMsg, int &nPos)
{
	m_ucFin = (unsigned char)pMsg[nPos] >> 7;
	return 0;
}

int WebsocketRequest::FetchOpcode(char *pMsg, int &nPos)
{
	m_ucOpcode = pMsg[nPos] & 0x0f;
	nPos++;
	return 0;
}

int WebsocketRequest::FetchMask(char *pMsg, int &nPos)
{

	m_ucMask = (unsigned char)pMsg[nPos] >> 7;
	return 0;
}

int WebsocketRequest::FetchMaskingKey(char *pMsg, int &nPos)
{
	memset(m_ucMaskingKeySet, 0, sizeof(m_ucMaskingKeySet));
	if(m_ucMask != 1)
		return 0;
	for(int i = 0; i < 4; i++)
		m_ucMaskingKeySet[i] = pMsg[nPos + i];
	nPos += 4;
	return 0;
}

int WebsocketRequest::FetchPayloadLength(char *pMsg, int &nPos)
{
	m_u64PayloadLength = pMsg[nPos] & 0x7f;
	nPos++;
	if(m_u64PayloadLength == 126)
	{
		uint16_t length = 0;
		memcpy(&length, pMsg + nPos, 2);
		nPos += 2;
		m_u64PayloadLength = ntohs(length);
	}
	else if(m_u64PayloadLength == 127)
	{
		uint32_t length = 0;
		memcpy(&length, pMsg + nPos, 4);
		nPos += 4;
		m_u64PayloadLength = ntohl(length);
	}
	return 0;
}

int WebsocketRequest::FetchPayload(char *pMsg, int &nPos)
{
	memset(m_szPayload, 0, MSG_MAX_SIZE);
	//add by huyf 2018.12.01:添加异常处理
	if (m_u64PayloadLength > MSG_MAX_SIZE)
	{
		LOGERROR("数据长度超过单包长度设置 MSG_MAX_SIZE=[%d] m_u64PayloadLength=[%d] nPos=[%d] pMsg=[%s]", MSG_MAX_SIZE, m_u64PayloadLength, nPos, pMsg);
		return -1;
	}
	//add end:添加异常处理
	if(m_ucMask != 1)
	{
		memcpy(m_szPayload, pMsg + nPos, m_u64PayloadLength);
	}
	else 
	{
		for(uint i = 0; i < m_u64PayloadLength; i++)
		{
			int j = i % 4;
			m_szPayload[i] = pMsg[nPos + i] ^ m_ucMaskingKeySet[j];
		}
	}
//	nPos += m_u64PayloadLength;
	return 0;
}

std::string WebsocketRequest::GetMsg()
{
    return m_szPayload;
}

uint64_t WebsocketRequest::GetMsgLength()
{
    return m_u64PayloadLength;
}

uint8_t WebsocketRequest::GetMsgOpcode()
{
    return m_ucOpcode;
}
