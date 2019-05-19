//////////////////////////////////////////////////////////////////////
//文件名称：CMsgTick.cpp
//功能描述：
//版本说明：CMsgTick API
//			
//编写作者：2019.01.18 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0 心跳包-[ping\]
//
//////////////////////////////////////////////////////////////////////
#include "AllMsg.h"

//////////////////////////////////////////////////// CMsgTick ////////////////////////////////////////////////////
//构造函数
CMsgTick::CMsgTick() 
{ 
}

//析构函数
CMsgTick::~CMsgTick() 
{ 	
}

//读取数据
BOOL CMsgTick::Read(SocketInputStream& iStream, UINT nLen)
{
	CNetMsg::Read(iStream, nLen);
	return TRUE;
}

BOOL CMsgTick::Write(SocketOutputStream& oStream) const
{
	return TRUE;
}

BOOL CMsgTick::CreateMsg(UINT nAction, UINT nRetStatus)
{
	//tick包
	if (m_pMsg)
	{
		cJSON_AddNumberToObject(m_pMsg, "action", nAction);
		//大乱斗创建道具
		if (nAction == _TICK_ACTION_PING)
		{
			cJSON_AddNumberToObject(m_pMsg, "sequence", nRetStatus);			
		}

		m_pData = cJSON_Print(m_pMsg);
		string strMsg(m_pData);
	    size_t nLen = strMsg.size();
		LOGDEBUG("[CMsgTick::CreateMsg]:\n m_pData=[%s] nLen=[%d]\n", m_pData, nLen);
	}	

	return TRUE;
}

//发送json数据
BOOL CMsgTick::SendMsg(CNetMsg* pMsg)
{
	char* sJson = cJSON_Print(pMsg->m_pMsg);
	m_pPlayer->SendMsgToClient(pMsg);
	free(sJson);
	return TRUE;	
}

//处理前端发送过来的消息
UINT CMsgTick::Process(VOID *pInfo)
{	
	LOGDEBUG("[CMsgTick::Process] begin");
	DEBUG_TRY
	//设置需要回包的玩家
	m_pPlayer = static_cast<Player*>(pInfo);
	CHECKF(m_pPlayer);	
	
	//处理前端发过来的消息
	m_pMsg = cJSON_Parse(this->m_pData);
	cJSON* pJsonMsg = m_pMsg;

	if(pJsonMsg)
	{
		// 获取任务的action
		cJSON* pAction = cJSON_GetObjectItem(pJsonMsg, "action");
	    CHECKF(pAction);

	    INT llRecvMillSecTime = GetMillSecTime()%1000000;
	    LOGDEBUG("[CMsgTick::Process] PlayerId=[%d] pAction=[%d] pPlayer=[%p]", m_pPlayer->GetUid(), pAction->valueint, m_pPlayer);
		switch(pAction->valueint)
		{
			//////////////////////////////////////////////////////////////////////////////////////////
			///
			case _TICK_ACTION_PING:
			{
				cJSON* pSequence = cJSON_GetObjectItem(pJsonMsg, "sequence");
				CHECKF(pSequence);
				
				CMsgTick msg;
				IF_OK(msg.CreateMsg(_TICK_ACTION_PING, pSequence->valueint))
				{
					SendMsg(&msg);
				}
				INT llSendMillSecTime = GetMillSecTime()%1000000;
	    		LOGDEBUG("[CMsgTick::Process] tcp rtt r_time=[%d] s_time=[%d] tm=[%d]", llRecvMillSecTime, llSendMillSecTime, llSendMillSecTime-llRecvMillSecTime);
			}
			break;
			default:
			{
				LOGDEBUG("[CMsgTick::Process] Is Error pAction=[%d] pPlayer=[%p]", pAction->valueint, m_pPlayer);
			}
			break;
		}
	}

	return PACKET_EXE_CONTINUE;
	DEBUG_CATCH0("[CMsgTick::Process] Is Catch...");
}