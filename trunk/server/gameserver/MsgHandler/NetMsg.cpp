////////////////////////////////////////////////////////////////
//文件名称：ServerSocket.cpp
//功能描述：相关包基类
//文件名称：Packet API
//
//编写作者：2017.08.15 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////
#include "NetMsg.h"
#include "../Common/BaseType.h"
#include "../Main/Player.h"
#include "../NetBase/SocketInputStream.h"
#include "../NetBase/SocketOutputStream.h"
#include "../WebSocket/WebSocketRespond.h"
#include "../WebSocket/WebSocketRequest.h"
#include "../WebSocket/WebSocketHandler.h"


CNetMsg::CNetMsg()
{
	m_Index = 0;
	m_Status = 0;
	m_pData = NULL;
	m_pPlayer = NULL;
	m_nDataLen = 0;
	m_pMsg = cJSON_CreateObject();	
}

CNetMsg::~CNetMsg()
{
	cJSON_Delete(m_pMsg);
	SAFE_FREE(m_pData);
	m_pPlayer = NULL;
}
BOOL CNetMsg::CreateMsg (UINT nAction, UINT nRetStatus)
{
	return TRUE;
}

VOID CNetMsg::Release()
{
	cJSON_Delete(m_pMsg);
	SAFE_FREE(m_pData);
	m_pPlayer = NULL;
}

VOID CNetMsg::Reset()
{
	cJSON_Delete(m_pMsg);
	SAFE_FREE(m_pData);
	m_pPlayer = NULL;

	m_Index = 0;
	m_Status = 0;
	m_pData = NULL;
	m_nDataLen = 0;
	m_pMsg = cJSON_CreateObject();	
}

BOOL CNetMsg::SendMsg ( CNetMsg* pMsg )
{
	return TRUE;
}

BOOL CNetMsg::Write(SocketOutputStream& oStream) const 
{
    if (!m_pPlayer)
    {
        return FALSE;
    }

    if (!m_pPlayer->GetSocket())
    {
        return FALSE;
    }
     
	return m_pPlayer->GetSocket()->Write(m_pPlayer->GetSocketOutputStream(), m_pData, m_nDataLen);
}


BOOL CNetMsg::Read(SocketInputStream& iStream, UINT nLen)
{
    //LOGDEBUG ( "[CNetMsg::Read] nLen=[%d]\n", nLen);
    
    //限定包的最大长度4096    
    if(nLen >= MSG_MAX_SIZE)
	{
		LOGERROR("[CNetMsg::Read] 接受到的数据长度解析出来的包长度错误，超过了单包的最大长度=[%d] nLen=[%d] socketid=[%u] playerid=[%d]-----------", MSG_MAX_SIZE, nLen, m_pPlayer->GetUid(), m_pPlayer->GetSocket()->getSocketID());                        
		return FALSE;
	}

	if(!m_pPlayer)
	{
		LOGERROR("[CNetMsg::Read] m_pPlayer==NULL");                        
		return FALSE;
	}

	if(!m_pPlayer->m_pSocket)
	{
		LOGERROR("[CNetMsg::Read] m_pPlayer->m_pSocket==NULL");                        
		return FALSE;
	}

	if(!m_pPlayer->m_pSocket->m_handler)
	{
		LOGERROR("[CNetMsg::Read] m_pPlayer->m_pSocket->m_handler==NULL");                        
		return FALSE;
	}

	if(!m_pPlayer->m_pSocket->m_handler->GetRequest())
	{
		LOGERROR("[CNetMsg::Read] m_pPlayer->m_pSocket->m_handler->GetRequest()==NULL");                        
		return FALSE;
	}

	char szMsg[MSG_MAX_SIZE];
	memset(szMsg, 0, MSG_MAX_SIZE);
	if(!m_pPlayer->m_pSocketInputStream->Read(szMsg, nLen))
	{
		LOGERROR("缓存中的数据长度不够协议头的长度...socketid=[%u]\t数据还不够，需要继续接收-----------",m_pPlayer->GetSocket()->getSocketID());                        
		return FALSE;
	}
	
	INT nResult = m_pPlayer->m_pSocket->m_handler->GetRequest()->FetchWebsocketInfo(szMsg);
	//!alter by huyf 2018.12.01:添加异常处理
	if (nResult == -1)
	{                       
		return FALSE;
	}
	//!alter end:添加异常处理
	std::string request_msg = m_pPlayer->m_pSocket->m_handler->GetRequest()->GetMsg();
    //size_t request_msg_len= m_pPlayer->m_pSocket->m_handler->GetRequest()->GetMsgLength();
	//uint8_t msg_opcode = m_pPlayer->m_pSocket->m_handler->GetRequest()->GetMsgOpcode();
    
	string strPackageID = request_msg.substr(0,4); 	//1001
	//INT nPackageID = atoi(strPackageID.c_str());

	//LOGERROR("[CNetMsg::Read] npackageID = %d\n", nPackageID);
	string strPackageLen = request_msg.substr(4,4);	//83

	INT nPackageLen =  atoi(strPackageLen.c_str());
	m_nDataLen = nPackageLen;
	//LOGERROR("[CNetMsg::Read] nPackageLen = %d\n", nPackageLen);

	//!alter by huyf 2018.12.06:此处coredump过,增加长度读取 
	//!#0  0x00007f4b5748b418 in _int_free () from /lib64/libc.so.6
	//!#1  0x00007f4b57da7c03 in std::basic_string<char, std::char_traits<char>, std::allocator<char> >::~basic_string() () from /lib64/libstdc++.so.6
	//!#2  0x00000000004b4b23 in CNetMsg::Read (this=0x6ef0150, iStream=..., nLen=202) at /root/GameServer/MsgHandler/NetMsg.cpp:150
	//!#3  0x000000000048fe39 in CMsgLogin::Read (this=0x6ef0150, iStream=..., nLen=202) at /root/GameServer/MsgHandler/MsgLogin.cpp:23
	//!#4  0x000000000048a6f3 in SocketInputStream::ReadPacket (this=0x19ead90, pMsg=0x6ef0150, nLengh=202) at /root/GameServer/NetBase/SocketInputStream.cpp:226
	//!#5  0x000000000047d765 in Player::ProcessCommand (this=0x7f4b58923648, Option=false) at /root/GameServer/Main/Player.cpp:443
	//!#6  0x00000000004598e1 in ServerPlayer::ProcessCommand (this=0x7f4b58923648, Option=false) at /root/GameServer/Main/ServerPlayer.cpp:108
	//!#7  0x0000000000477069 in ServerManager::ProcessCommands (this=0x1980f70) at /root/GameServer/Main/ServerManager.cpp:1555
	//!#8  0x00000000004778c8 in ServerManager::Loop (this=0x1980f70) at /root/GameServer/Main/ServerManager.cpp:1808
	//!#9  0x000000000047a03f in World::Loop (this=0x6f2808 <g_World>) at /root/GameServer/Main/World.cpp:535
	//!#10 0x0000000000479d80 in main (argc=1, argv=0x7ffe7ab278c8) at /root/GameServer/Main/World.cpp:453
	string strPackageJson = request_msg.substr(8, nPackageLen);   //data     
	//LOGERROR("[CNetMsg::Read] strPackageJson=%s size=[%d]\n", strPackageJson.c_str(), strPackageJson.size());
	//!alter end:此处coredump过,增加长度读取

	m_pData = (char*)malloc(nPackageLen + 1);
	memset(m_pData, 0, m_nDataLen + 1);

	memcpy(m_pData, strPackageJson.c_str(), strPackageJson.size());	
	
	//LOGDEBUG ("[CNetMsg::Read] nPackageID=[%d] m_nDataLen=[%d] m_pData=[%s] m_Head=[%d] m_Tail=[%d]\n", 
	//	nPackageID, m_nDataLen, m_pData,m_pPlayer->m_pSocketInputStream->m_Head,m_pPlayer->m_pSocketInputStream->m_Tail);
	return TRUE;
}

UINT CNetMsg::Process ( VOID* pInfo )
{
	// process _msg
	// switch
	// case
	return TRUE;
}