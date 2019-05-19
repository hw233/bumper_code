#include "MsgExchange.h"

#include "../Base/logger.h"
#include "../Main/ServerManager.h"


//////////////////////////////////////////////////// CMsgExchange ////////////////////////////////////////////////////
//构造函数
CMsgExchange::CMsgExchange() 
{ 
}

//析构函数
CMsgExchange::~CMsgExchange() 
{ 	
}

//读取数据
BOOL CMsgExchange::Read(SocketInputStream& iStream, UINT nLen)
{
	CNetMsg::Read(iStream, nLen);
	return TRUE;
}

BOOL CMsgExchange::Write(SocketOutputStream& oStream) const
{
	CNetMsg::Write(oStream);
	return TRUE;
}

//创建消息
BOOL CMsgExchange::CreateMsg(UINT nRetStatus, UINT nAction)
{
	return TRUE;
}

//发送json数据
BOOL CMsgExchange::SendMsg(CNetMsg* pMsg)
{
	m_pPlayer->SendMsgToClient(pMsg);
	return TRUE;	
}

//处理前端发送过来的消息
UINT CMsgExchange::Process(VOID *pInfo)
{		
	return PACKET_EXE_CONTINUE;
}