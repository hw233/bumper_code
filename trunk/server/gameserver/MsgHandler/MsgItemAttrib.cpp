#include "MsgItemAttrib.h"

#include "../Base/logger.h"
#include "../Main/ServerManager.h"


//////////////////////////////////////////////////// CMsgItemAttrib ////////////////////////////////////////////////////
//构造函数
CMsgItemAttrib::CMsgItemAttrib() 
{ 
}

//析构函数
CMsgItemAttrib::~CMsgItemAttrib() 
{ 	
}

//读取数据
BOOL CMsgItemAttrib::Read(SocketInputStream& iStream, UINT nLen)
{
	CNetMsg::Read(iStream, nLen);
	return TRUE;
}

BOOL CMsgItemAttrib::Write(SocketOutputStream& oStream) const
{
	return TRUE;
}

//创建消息
BOOL CMsgItemAttrib::CreateMsg(UINT nAction, UINT nItemId, UINT nAttributeData)
{	
	if (m_pMsg)
	{
        cJSON_AddNumberToObject(m_pMsg, "action", nAction);  
        cJSON_AddNumberToObject(m_pMsg, "item_id", nItemId);      
        cJSON_AddNumberToObject(m_pMsg, "attrib_data", nAttributeData);

		m_pData = cJSON_Print(m_pMsg);
		string strMsg(m_pData);
	    size_t nLen = strMsg.size();
		LOGDEBUG("[CMsgItemAttrib::CreateMsg]:\n sJson=[%s] nLen=[%d]\n", strMsg.c_str(), nLen);
	}	

	return TRUE;
}

//发送json数据
BOOL CMsgItemAttrib::SendMsg(CNetMsg* pMsg)
{
	m_pPlayer->SendMsgToClient(pMsg);
	return TRUE;	
}

//处理前端发送过来的login消息
UINT CMsgItemAttrib::Process(VOID *pInfo)
{	
	//LOGDEBUG("[CMsgItemAttrib::Process] begin");

	DEBUG_TRY
	//设置需要回包的玩家
	m_pPlayer = static_cast<Player*>(pInfo);
	CHECKF(m_pPlayer);	

	//处理前端发过来的消息
	m_pMsg = cJSON_Parse(this->m_pData);
	cJSON* pJsonMsg = m_pMsg;
	if(pJsonMsg)
	{
		cJSON* pAction = cJSON_GetObjectItem(pJsonMsg, "action");		
		if (pAction)
		{
			LOGDEBUG("[CMsgItemAttrib::Process] action=[%d] PlayerID=[%d] pPlayer=[%p]", pAction->valueint, m_pPlayer->GetUid(), m_pPlayer);
			
		}
	}

	return PACKET_EXE_CONTINUE;
	DEBUG_CATCH0("[CMsgItemAttrib::Process] Is Catch...");
}
