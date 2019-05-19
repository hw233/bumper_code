#include "../Base/logger.h"
#include "../Main/ServerManager.h"
#include "AllMsg.h"


//////////////////////////////////////////////////// CMsgUserAttrib ////////////////////////////////////////////////////
//构造函数
CMsgUserAttrib::CMsgUserAttrib() 
{ 
}

//析构函数
CMsgUserAttrib::~CMsgUserAttrib() 
{ 	
}

//读取数据
BOOL CMsgUserAttrib::Read(SocketInputStream& iStream, UINT nLen)
{
	CNetMsg::Read(iStream, nLen);
	return TRUE;
}

BOOL CMsgUserAttrib::Write(SocketOutputStream& oStream) const
{
	return TRUE;
}

//创建消息
BOOL CMsgUserAttrib::CreateMsg(UINT nAction, UINT nAttribData/*=0*/)
{
	if (m_pMsg)
	{
		cJSON_AddNumberToObject(m_pMsg, "action", nAction);
		cJSON_AddNumberToObject(m_pMsg, "attrib_data", nAttribData);				
		
		m_pData = cJSON_Print(m_pMsg);
		string strMsg(m_pData);
	    size_t nLen = strMsg.size();
		LOGDEBUG("[CMsgUserAttrib::CreateMsg]:\n m_pData=[%s] nLen=[%d]\n", m_pData, nLen);
	}	

	return TRUE;
}

//发送json数据
BOOL CMsgUserAttrib::SendMsg(CNetMsg* pMsg)
{
	m_pPlayer->SendMsgToClient(pMsg);
	return TRUE;	
}

//处理前端发送过来的login消息
UINT CMsgUserAttrib::Process(VOID *pInfo)
{	
	DEBUG_TRY
	return PACKET_EXE_CONTINUE;
	DEBUG_CATCH0("[CMsgUserAttrib::Process] Is Catch...");
}