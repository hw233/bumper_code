#include "AllMsg.h"

#include "../Base/logger.h"
#include "../Main/ServerManager.h"
#include "../Main/TaskDetail/TaskDetailCfg.h"
#include "../Main/TaskDetail/TaskDetailData.h"
#include "../Main/TaskDetail/TaskDetailCfgManager.h"
#include "../Main/Item/ItemAwardManager.h"
//////////////////////////////////////////////////// CMsgTest ////////////////////////////////////////////////////
//构造函数
CMsgTest::CMsgTest() 
{ 
}

//析构函数
CMsgTest::~CMsgTest() 
{ 	
}

//读取数据
BOOL CMsgTest::Read(SocketInputStream& iStream, UINT nLen)
{
	CNetMsg::Read(iStream, nLen);
	return TRUE;
}

BOOL CMsgTest::Write(SocketOutputStream& oStream) const
{
	return TRUE;
}

BOOL CMsgTest::CreateMsg(UINT nAction, Player* pPlayer/*=NULL*/, CTaskDetailData* pData/*=NULL*/, UINT nType/*=0*/)
{
	return TRUE;
}

//发送json数据
BOOL CMsgTest::SendMsg(CNetMsg* pMsg)
{
	char* sJson = cJSON_Print(pMsg->m_pMsg);
	m_pPlayer->SendMsgToClient(pMsg);
	free(sJson);
	return TRUE;	
}

//处理前端发送过来的消息
UINT CMsgTest::Process(VOID *pInfo)
{	
	LOGDEBUG("[CMsgTest::Process] begin");
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

	    LOGDEBUG("[CMsgTest::Process] PlayerId=[%d] pAction=[%d] pPlayer=[%p]", m_pPlayer->GetUid(), pAction->valueint, m_pPlayer);

	    CTaskDetailManager* pTaskDetailManager = m_pPlayer->QueryTaskDetailManager();
	    CHECKF(pTaskDetailManager);
	    //switch(pAction->valueint)
		{
		    ////////////////////////////////////////////////////////////////////////////////////////////////////////
		    //获取任务列表
			//case _TASK_DETAIL_LIST:
			{
				//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成10次击杀<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
				if (!m_pPlayer->TriggerDayTaskDetail(DAY_TASK_DETAIL_TYPE_3))
				{
					LOGERROR("[CMsgBattle::Process] TriggerDayTaskDetail Is Error");
				}
				if (!m_pPlayer->TriggerDayTaskDetail(DAY_TASK_DETAIL_TYPE_6))
				{
					LOGERROR("[CMsgBattle::Process] TriggerDayTaskDetail Is Error");
				}
				if (!m_pPlayer->TriggerDayTaskDetail(DAY_TASK_DETAIL_TYPE_7))
				{
					LOGERROR("[CMsgBattle::Process] TriggerDayTaskDetail Is Error");
				}
				if (!m_pPlayer->TriggerDayTaskDetail(DAY_TASK_DETAIL_TYPE_8))
				{
					LOGERROR("[CMsgBattle::Process] TriggerDayTaskDetail Is Error");
				}
				if (!m_pPlayer->TriggerDayTaskDetail(DAY_TASK_DETAIL_TYPE_9))
				{
					LOGERROR("[CMsgBattle::Process] TriggerDayTaskDetail Is Error");
				}
				if (!m_pPlayer->TriggerDayTaskDetail(DAY_TASK_DETAIL_TYPE_10))
				{
					LOGERROR("[CMsgBattle::Process] TriggerDayTaskDetail Is Error");
				}
				
				//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成10次击杀<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
				//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成1次双杀<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
				if (!m_pPlayer->QueryMultiKillTimer().IsActive())
				{
					m_pPlayer->QueryMultiKillTimer().Startup(8);
					m_pPlayer->SetMultiKillCount(m_pPlayer->GetMultiKillCount() + 1);

					LOGERROR("[CMsgBattle::Process] TriggerDayTaskDetail Is Error");
				}
				else
				{
					m_pPlayer->SetMultiKillCount(m_pPlayer->GetMultiKillCount() + 1);
					if (m_pPlayer->GetMultiKillCount() == 2)
					{
						if (!m_pPlayer->TriggerDayTaskDetail(DAY_TASK_DETAIL_TYPE_4))
						{
							LOGERROR("[CMsgBattle::Process] TriggerDayTaskDetail Is Error");
						}
					}
					if (m_pPlayer->GetMultiKillCount() == 3)
					{
						if (!m_pPlayer->TriggerDayTaskDetail(DAY_TASK_DETAIL_TYPE_5))
						{
							LOGERROR("[CMsgBattle::Process] TriggerDayTaskDetail Is Error");
						}
					}
				}
				//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成1次双杀<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
				LOGERROR("[CMsgTest::Process] Is Suc");
				return PACKET_EXE_CONTINUE;
			}
			
		}
	}

	return PACKET_EXE_CONTINUE;
	DEBUG_CATCH0("[CMsgTest::Process] Is Catch...");
}