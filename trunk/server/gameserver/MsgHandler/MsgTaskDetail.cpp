#include "AllMsg.h"

#include "../Base/logger.h"
#include "../Main/ServerManager.h"
#include "../Main/TaskDetail/TaskDetailCfg.h"
#include "../Main/TaskDetail/TaskDetailData.h"
#include "../Main/TaskDetail/TaskDetailCfgManager.h"
#include "../Main/Item/ItemAwardManager.h"
//////////////////////////////////////////////////// CMsgTaskDetail ////////////////////////////////////////////////////
//构造函数
CMsgTaskDetail::CMsgTaskDetail() 
{ 
}

//析构函数
CMsgTaskDetail::~CMsgTaskDetail() 
{ 	
}

//读取数据
BOOL CMsgTaskDetail::Read(SocketInputStream& iStream, UINT nLen)
{
	CNetMsg::Read(iStream, nLen);
	return TRUE;
}

BOOL CMsgTaskDetail::Write(SocketOutputStream& oStream) const
{
	return TRUE;
}

BOOL CMsgTaskDetail::CreateMsg(UINT nAction, Player* pPlayer/*=NULL*/, CTaskDetailData* pData/*=NULL*/, UINT nType/*=0*/)
{
	if (m_pMsg)
	{
		cJSON_AddNumberToObject(m_pMsg, "action", nAction);		
		//添加任务
		if (nAction == _TASK_DETAIL_ADD)
		{
			CHECKF(pPlayer);
			CHECKF(pData);

			cJSON_AddNumberToObject(m_pMsg, "id", pData->GetId());
			cJSON_AddNumberToObject(m_pMsg, "task_id", pData->GetTaskId());
			cJSON_AddNumberToObject(m_pMsg, "complete_num", pData->GetCompleteNum());
			UINT nStatus = 0;	//0 : 未领取 1 : 第一个奖励已经领取 2 : 第二个奖励已经领取
			UINT nMask1 = 0;
			UINT nMask2 = 0;
			//获取任务id->>>>>再通过根据任务id读取配置文件信息
			CTaskDetailCfgManager* pTaskDetailCfgManager = QueryTaskDetailCfgManager();
			CHECKF(pTaskDetailCfgManager);

			UINT nTaskId = pData->GetTaskId();
			CTaskDetailCfg* pTaskDetailCfg = pTaskDetailCfgManager->QueryTaskDetailCfgInfo(nTaskId);
			if (pTaskDetailCfg)
			{
				nMask1 = pTaskDetailCfg->GetMask1();
				nMask2 = pTaskDetailCfg->GetMask2();
				if (pPlayer)
				{
					if (nTaskId/1000 == PERMANENT_TASK_DETAIL_TYPE)	//永久任务
					{
						if(pPlayer->ChkTaskMask(nMask1))
						{
							nStatus = 1;
						}
					}
					else if (nTaskId/1000 == DAY_TASK_DETAIL_TYPE) // 日常任务-领奖2此
					{
						if(pPlayer->ChkDayTaskMask(nMask1))
						{
							nStatus = 1;
						}
						if (pPlayer->ChkDayTaskMask(nMask2))
						{
							nStatus = 2;
						}
					}
					else if (nTaskId/1000 == DAY_TASK_DETAIL_TRY_GAME_TYPE)	//日常任务类型-试玩任务类型-领奖一次
					{
						if(pPlayer->ChkDayTaskMask(nMask1))
						{
							nStatus = 1;
						}
					}
				}
				
				cJSON_AddNumberToObject(m_pMsg, "status", nStatus);
			}
			else
			{
				LOGERROR("[CMsgTaskDetail::CreateMsg] Is Error pTaskDetailCfg=NULL id=[%d] nTaskId=[%d]", pData->GetId(), nTaskId);
				cJSON_AddNumberToObject(m_pMsg, "status", nStatus);
			}			
		}
		//更新某个任务
		else if (nAction == _TASK_DETAIL_UPDATE)
		{
			CHECKF(pPlayer);
			CHECKF(pData);

			cJSON_AddNumberToObject(m_pMsg, "id", pData->GetId());
			cJSON_AddNumberToObject(m_pMsg, "complete_num", pData->GetCompleteNum());
			UINT nStatus = 0;	//0 : 未领取 1 : 第一个奖励已经领取 2 : 第二个奖励已经领取
			UINT nMask1 = 0;
			UINT nMask2 = 0;
			//获取任务id->>>>>再通过根据任务id读取配置文件信息
			CTaskDetailCfgManager* pTaskDetailCfgManager = QueryTaskDetailCfgManager();
			CHECKF(pTaskDetailCfgManager);

			UINT nTaskId = pData->GetTaskId();
			CTaskDetailCfg* pTaskDetailCfg = pTaskDetailCfgManager->QueryTaskDetailCfgInfo(nTaskId);
			if (pTaskDetailCfg)
			{
				nMask1 = pTaskDetailCfg->GetMask1();
				nMask2 = pTaskDetailCfg->GetMask2();
				if (pPlayer)
				{
					if (nTaskId/1000 == PERMANENT_TASK_DETAIL_TYPE)	//永久任务
					{
						if(pPlayer->ChkTaskMask(nMask1))
						{
							nStatus = 1;
						}
					}
					else if (nTaskId/1000 == DAY_TASK_DETAIL_TYPE) // 日常任务-领奖2此
					{
						if(pPlayer->ChkDayTaskMask(nMask1))
						{
							nStatus = 1;
						}
						if (pPlayer->ChkDayTaskMask(nMask2))
						{
							nStatus = 2;
						}
					}
					else if (nTaskId/1000 == DAY_TASK_DETAIL_TRY_GAME_TYPE)	//日常任务类型-试玩任务类型-领奖一次
					{
						if(pPlayer->ChkDayTaskMask(nMask1))
						{
							nStatus = 1;
						}
					}
				}
				
				cJSON_AddNumberToObject(m_pMsg, "status", nStatus);
			}
			else
			{
				LOGERROR("[CMsgTaskDetail::CreateMsg] Is Error pTaskDetailCfg=NULL id=[%d] nTaskId=[%d]", pData->GetId(), nTaskId);
				cJSON_AddNumberToObject(m_pMsg, "status", nStatus);
			}	
		}
		//获取任务列表失败 	 		       S--------------->C
		else if (nAction == _TASK_DETAIL_LIST_FAIL)
		{
			/* code */
		}
		//领取奖励成功 		               S--------------->C
		else if (nAction == _TASK_DETAIL_AWARD_SUC)
		{
			CHECKF(pPlayer);
			CHECKF(pData);

			cJSON_AddNumberToObject(m_pMsg, "id", pData->GetId());
			cJSON_AddNumberToObject(m_pMsg, "type", nType);					//int 0：首次领奖 1 : 再次领奖
			UINT nStatus = 0;	//0 : 未领取 1 : 第一个奖励已经领取 2 : 第二个奖励已经领取
			UINT nMask1 = 0;
			UINT nMask2 = 0;
			//获取任务id->>>>>再通过根据任务id读取配置文件信息
			CTaskDetailCfgManager* pTaskDetailCfgManager = QueryTaskDetailCfgManager();
			CHECKF(pTaskDetailCfgManager);

			UINT nTaskId = pData->GetTaskId();
			CTaskDetailCfg* pTaskDetailCfg = pTaskDetailCfgManager->QueryTaskDetailCfgInfo(nTaskId);
			if (pTaskDetailCfg)
			{
				nMask1 = pTaskDetailCfg->GetMask1();
				nMask2 = pTaskDetailCfg->GetMask2();
				if (pPlayer)
				{
					if (nTaskId/1000 == PERMANENT_TASK_DETAIL_TYPE)	//永久任务
					{
						if(pPlayer->ChkTaskMask(nMask1))
						{
							nStatus = 1;
						}
					}
					else if (nTaskId/1000 == DAY_TASK_DETAIL_TYPE) // 日常任务-领奖2此
					{
						if(pPlayer->ChkDayTaskMask(nMask1))
						{
							nStatus = 1;
						}
						if (pPlayer->ChkDayTaskMask(nMask2))
						{
							nStatus = 2;
						}
					}
					else if (nTaskId/1000 == DAY_TASK_DETAIL_TRY_GAME_TYPE)	//日常任务类型-试玩任务类型-领奖一次
					{
						if(pPlayer->ChkDayTaskMask(nMask1))
						{
							nStatus = 1;
						}
					}
				}
				
				cJSON_AddNumberToObject(m_pMsg, "status", nStatus);
			}
			else
			{
				LOGERROR("[CMsgTaskDetail::CreateMsg] Is Error pTaskDetailCfg=NULL id=[%d] nTaskId=[%d]", pData->GetId(), nTaskId);
				cJSON_AddNumberToObject(m_pMsg, "status", nStatus);
			}
		}
		//获取任务列表成功 	 		       S--------------->C
		else if (nAction == _TASK_DETAIL_LIST_SUC)
		{
			/* code */
		}
		//删除任务 		                   S--------------->C
		else if (nAction == _TASK_DETAIL_DEL)
		{
			cJSON_AddNumberToObject(m_pMsg, "task_id", pData->GetId());
		}
		

		m_pData = cJSON_Print(m_pMsg);
		string strMsg(m_pData);
		size_t nLen = strMsg.size();
		LOGDEBUG("[CMsgTaskDetail::CreateMsg]:\n m_pData=[%s] nLen=[%d]\n", m_pData, nLen);
	}
	return TRUE;
}

//发送json数据
BOOL CMsgTaskDetail::SendMsg(CNetMsg* pMsg)
{
	char* sJson = cJSON_Print(pMsg->m_pMsg);
	m_pPlayer->SendMsgToClient(pMsg);
	free(sJson);
	return TRUE;	
}

//处理前端发送过来的消息
UINT CMsgTaskDetail::Process(VOID *pInfo)
{	
	LOGDEBUG("[CMsgTaskDetail::Process] begin");
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

	    LOGDEBUG("[CMsgTaskDetail::Process] PlayerId=[%d] pAction=[%d] pPlayer=[%p]", m_pPlayer->GetUid(), pAction->valueint, m_pPlayer);

	    CTaskDetailManager* pTaskDetailManager = m_pPlayer->QueryTaskDetailManager();
	    CHECKF(pTaskDetailManager);
	    switch(pAction->valueint)
		{
		    ////////////////////////////////////////////////////////////////////////////////////////////////////////
		    //获取任务列表
			case _TASK_DETAIL_LIST:
			{
				if(!pTaskDetailManager->GetTaskDetailListInfo())
				{
					CMsgTaskDetail msg;
					IF_OK(msg.CreateMsg(_TASK_DETAIL_LIST_FAIL))
					{
						SendMsg(&msg);
					}

					LOGERROR("[CMsgTaskDetail::Process] Is Error");
					return PACKET_EXE_CONTINUE;
				}
				else
				{
					CMsgTaskDetail msg;
					IF_OK(msg.CreateMsg(_TASK_DETAIL_LIST_SUC))
					{
						SendMsg(&msg);
					}
				}

				LOGERROR("[CMsgTaskDetail::Process] Is Suc");
				return PACKET_EXE_CONTINUE;
			}
			break;
			////////////////////////////////////////////////////////////////////////////////////////////////////////
			//领取奖励
			case _TASK_DETAIL_AWARD:
			{
				cJSON* pId = cJSON_GetObjectItem(pJsonMsg, "id"); 					// id : 任务ID
				cJSON* pType = cJSON_GetObjectItem(pJsonMsg, "type");				// type : 0 : 首次领奖 1 : 再次领奖
		    	CHECKF(pId);

		    	UINT nId = pId->valueint;
		    	UINT nType = 0;
		    	if (pType)
		    	{
		    		nType = pType->valueint;
		    	}
		    	
		    	//通过任务iD查询配置表 获得mask 更新掩码位
				UINT nMaskIdx = 0;
				if (pTaskDetailManager)
				{
					//查玩家身上是否存在此任务
					CTaskDetailData* pData = pTaskDetailManager->QueryTaskDetailDataById(nId);
					if (pData)
					{
						//获取任务id->>>>>再通过根据任务id读取配置文件信息
						CTaskDetailCfgManager* pTaskDetailCfgManager = QueryTaskDetailCfgManager();
						CHECKF(pTaskDetailCfgManager);

						UINT nTaskId = pData->GetTaskId();
						CTaskDetailCfg* pTaskDetailCfg = pTaskDetailCfgManager->QueryTaskDetailCfgInfo(nTaskId);
						if (pTaskDetailCfg)
						{
							UINT nCompleteNum = pData->GetCompleteNum();
							UINT nCompleteNumCfg = pTaskDetailCfg->GetCompleteNum();

							LOGDEBUG("[CMsgTaskDetail::Process] nId=[%d] nType=[%d] nTaskId=[%d]", nId, nType, nTaskId);
							
							//需要的目标已经达成
							if(nCompleteNum >= nCompleteNumCfg)
							{
								//相关掩码位
								if (nTaskId / 1000 == PERMANENT_TASK_DETAIL_TYPE)	//永久任务-一次性-单次奖励
								{
									nMaskIdx = pTaskDetailCfg->GetMask1();
									//检查掩码位
									if (m_pPlayer->ChkTaskMask(nMaskIdx))	//领奖已经领过
									{
										CMsgTaskDetail msg;
										IF_OK(msg.CreateMsg(_TASK_DETAIL_AWARD_FAIL))
										{
											SendMsg(&msg);
										}

										LOGERROR("[CMsgTaskDetail::Process] ChkDayTaskMask Is Hava Award nId=[%d] nType=[%d] nTaskId=[%d] nMaskIdx=[%d]", nId, nType, nTaskId, nMaskIdx);
										return PACKET_EXE_CONTINUE;
									}
									
									//设置掩码位
									if(!m_pPlayer->AddTaskMask(nMaskIdx))	//领奖已经领过
									{
										CMsgTaskDetail msg;
										IF_OK(msg.CreateMsg(_TASK_DETAIL_AWARD_FAIL))
										{
											SendMsg(&msg);
										}

										LOGERROR("[CMsgTaskDetail::Process] AddTaskMask Is Hava Award nId=[%d] nType=[%d] nTaskId=[%d] nMaskIdx=[%d]", nId, nType, nTaskId, nMaskIdx);
										return PACKET_EXE_CONTINUE;
									}
								}
								else if (nTaskId / 1000 == DAY_TASK_DETAIL_TYPE) //日常任务-[日常任务]--2次奖励
								{
									if (nType == 0)
									{
										nMaskIdx = pTaskDetailCfg->GetMask1();
									}
									else if (nType == 1)
									{
										nMaskIdx = pTaskDetailCfg->GetMask2();
									}
									else
									{
										CMsgTaskDetail msg;
										IF_OK(msg.CreateMsg(_TASK_DETAIL_AWARD_FAIL))
										{
											SendMsg(&msg);
										}

										LOGERROR("[CMsgTaskDetail::Process] nType Is Error nId=[%d] nType=[%d] nTaskId=[%d] nMaskIdx=[%d]", nId, nType, nTaskId, nMaskIdx);
										return PACKET_EXE_ERROR;
									}

									//检查掩码位
									if (m_pPlayer->ChkDayTaskMask(nMaskIdx))	//领奖已经领过
									{
										CMsgTaskDetail msg;
										IF_OK(msg.CreateMsg(_TASK_DETAIL_AWARD_FAIL))
										{
											SendMsg(&msg);
										}

										LOGERROR("[CMsgTaskDetail::Process] ChkDayTaskMask Is Hava Award nId=[%d] nType=[%d] nTaskId=[%d] nMaskIdx=[%d]", nId, nType, nTaskId, nMaskIdx);
										return PACKET_EXE_CONTINUE;
									}

									//设置掩码位
									if(!m_pPlayer->AddDayTaskMask(nMaskIdx))	//领奖已经领过
									{
										CMsgTaskDetail msg;
										IF_OK(msg.CreateMsg(_TASK_DETAIL_AWARD_FAIL))
										{
											SendMsg(&msg);
										}

										LOGERROR("[CMsgTaskDetail::Process] AddDayTaskMask Is Hava Award nId=[%d] nType=[%d] nTaskId=[%d] nMaskIdx=[%d]", nId, nType, nTaskId, nMaskIdx);
										return PACKET_EXE_CONTINUE;
									}
								}
								else if (nTaskId / 1000 == DAY_TASK_DETAIL_TRY_GAME_TYPE) //日常任务-[试玩任务]-单次奖励
								{
									nMaskIdx = pTaskDetailCfg->GetMask1();

									//检查掩码位
									if (m_pPlayer->ChkDayTaskMask(nMaskIdx))	//领奖已经领过
									{
										CMsgTaskDetail msg;
										IF_OK(msg.CreateMsg(_TASK_DETAIL_AWARD_FAIL))
										{
											SendMsg(&msg);
										}

										LOGERROR("[CMsgTaskDetail::Process] ChkDayTaskMask Is Hava Award nId=[%d] nType=[%d] nTaskId=[%d] nMaskIdx=[%d]", nId, nType, nTaskId, nMaskIdx);
										return PACKET_EXE_CONTINUE;
									}

									//设置掩码位
									if(!m_pPlayer->AddDayTaskMask(nMaskIdx))	//领奖已经领过
									{
										CMsgTaskDetail msg;
										IF_OK(msg.CreateMsg(_TASK_DETAIL_AWARD_FAIL))
										{
											SendMsg(&msg);
										}

										LOGERROR("[CMsgTaskDetail::Process] AddDayTaskMask Is Hava Award nId=[%d] nType=[%d] nTaskId=[%d] nMaskIdx=[%d]", nId, nType, nTaskId, nMaskIdx);
										return PACKET_EXE_CONTINUE;
									}
								}
								else
								{
									CMsgTaskDetail msg;
									IF_OK(msg.CreateMsg(_TASK_DETAIL_AWARD_FAIL))
									{
										SendMsg(&msg);
									}

									LOGERROR("[CMsgTaskDetail::Process] ChkDayTaskMask Is Hava Award nId=[%d] nType=[%d] nTaskId=[%d] nMaskIdx=[%d]", nId, nType, nTaskId, nMaskIdx);
									return PACKET_EXE_CONTINUE;
								}

								//发放奖励
								UINT nAwardId = pTaskDetailCfg->GetTaskData();
								CItemAwardManager* pItemAwardManager = QueryItemAwardManager();
								if (pItemAwardManager)
								{
									//奖励发放成功
									if(pItemAwardManager->AwardItemToPlayer(m_pPlayer, nAwardId))
									{
										//发放完奖励后，删除永久任务
										if (nTaskId / 1000 == PERMANENT_TASK_DETAIL_TYPE)	//永久任务-一次性-单次奖励
										{
											//永久任务清楚---不再下发任务
											pTaskDetailManager->DelTaskDetailData(nId);
										}										

										//同步前端领取成功通知
										CMsgTaskDetail msg;
										IF_OK(msg.CreateMsg(_TASK_DETAIL_AWARD_SUC, m_pPlayer, pData, nType))
										{
											SendMsg(&msg);
										}

										LOGERROR("[CMsgTaskDetail::Process] AwardItemToPlayer Is Suc nId=[%d] nType=[%d] nTaskId=[%d] nMaskIdx=[%d]", nId, nType, nTaskId, nMaskIdx);
										return PACKET_EXE_CONTINUE;
									}
									else
									{
										CMsgTaskDetail msg;
										IF_OK(msg.CreateMsg(_TASK_DETAIL_AWARD_FAIL))
										{
											SendMsg(&msg);
										}	

										LOGERROR("[CMsgTaskDetail::Process] AwardItemToPlayer Is Error nId=[%d] nType=[%d] nTaskId=[%d] nCompleteNum=[%d] nCompleteNumCfg=[%d]", nId, nType, nTaskId, nCompleteNum, nCompleteNumCfg);
										return PACKET_EXE_CONTINUE;
									}
								}
								else
								{
									CMsgTaskDetail msg;
									IF_OK(msg.CreateMsg(_TASK_DETAIL_AWARD_FAIL))
									{
										SendMsg(&msg);
									}	

									LOGERROR("[CMsgTaskDetail::Process] Is Error pItemAwardManager == NULL nId=[%d] nType=[%d] nTaskId=[%d] nCompleteNum=[%d] nCompleteNumCfg=[%d]", nId, nType, nTaskId, nCompleteNum, nCompleteNumCfg);
									return PACKET_EXE_CONTINUE;
								}
							}
							else	//if(nCompleteNum >= nCompleteNumCfg)
							{
								CMsgTaskDetail msg;
								IF_OK(msg.CreateMsg(_TASK_DETAIL_AWARD_FAIL))
								{
									SendMsg(&msg);
								}	

								LOGERROR("[CMsgTaskDetail::Process] Is Not Finish nId=[%d] nType=[%d] nTaskId=[%d] nCompleteNum=[%d] nCompleteNumCfg=[%d]", nId, nType, nTaskId, nCompleteNum, nCompleteNumCfg);
								return PACKET_EXE_CONTINUE;
							}
						}
						else	//pTaskDetailCfg == NULL
						{
							CMsgTaskDetail msg;
							IF_OK(msg.CreateMsg(_TASK_DETAIL_AWARD_FAIL))
							{
								SendMsg(&msg);
							}	

							LOGERROR("[CMsgTaskDetail::Process] IsNot Found Task Info Config pTaskDetailCfg==NULL nId=[%d] nType=[%d] nTaskId=[%d]", nId, nType, nTaskId);
							return PACKET_EXE_CONTINUE;
						}
					}
					else	//pData == NULL
					{
						CMsgTaskDetail msg;
						IF_OK(msg.CreateMsg(_TASK_DETAIL_AWARD_FAIL))
						{
							SendMsg(&msg);
						}	

						LOGERROR("[CMsgTaskDetail::Process] IsNot Found TaskDetailData Is TaskDetailManager pData==NULL nId=[%d] nType=[%d]", nId, nType);
						return PACKET_EXE_CONTINUE;
					}
				}
				else	//pTaskDetailManager == NULL
				{
					CMsgTaskDetail msg;
					IF_OK(msg.CreateMsg(_TASK_DETAIL_AWARD_FAIL))
					{
						SendMsg(&msg);
					}	

					LOGERROR("[CMsgTaskDetail::Process] Is Error pTaskDetailManager==NULL nId=[%d] nType=[%d]", nId, nType);
					return PACKET_EXE_CONTINUE;
				}
			}
			break;
			////////////////////////////////////////////////////////////////////////////////////////////////////////
		    //试玩任务上报
			case _TASK_DETAIL_TRY_GAME:
			{
				cJSON* pId = cJSON_GetObjectItem(pJsonMsg, "id");
		    	CHECKF(pId);

		    	UINT nId = pId->valueint;
				if (pTaskDetailManager)
				{
					//查玩家身上是否存在此任务
					CTaskDetailData* pData = pTaskDetailManager->QueryTaskDetailDataById(nId);
					if (pData)
					{
						//获取任务id->>>>>再通过根据任务id读取配置文件信息
						CTaskDetailCfgManager* pTaskDetailCfgManager = QueryTaskDetailCfgManager();
						CHECKF(pTaskDetailCfgManager);

						UINT nTaskId = pData->GetTaskId();
						CTaskDetailCfg* pTaskDetailCfg = pTaskDetailCfgManager->QueryTaskDetailCfgInfo(nTaskId);
						if (pTaskDetailCfg)
						{
							UINT nCompleteNum = pData->GetCompleteNum();
							UINT nCompleteNumCfg = pTaskDetailCfg->GetCompleteNum();

							LOGDEBUG("[CMsgTaskDetail::Process] nId=[%d] nTaskId=[%d]", nId, nTaskId);

							//玩家数据小于配置数据的时候进行更新
							if (nCompleteNum < nCompleteNumCfg)
							{
								//更新到数据库
								pData->SetCompleteNum(nCompleteNum + 1, TRUE);

								//更新任务数据
								CMsgTaskDetail msg;
								IF_OK(msg.CreateMsg(_TASK_DETAIL_UPDATE, m_pPlayer, pData, pData->GetCompleteNum()))
								{
									SendMsg(&msg);
								}

								//如果处于完成临界点则触发其他任务计数
								if (nCompleteNum + 1 == nCompleteNumCfg)
								{
									//************注意这里统一任务 完成多次的话 都计数**************************************************************//
									//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成3次试玩任务<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
									if (!m_pPlayer->TriggerDayTaskDetail(DAY_TASK_DETAIL_TYPE_2))
									{
										LOGERROR("[CMsgTaskDetail::Process] TriggerDayTaskDetail Is Error");
									}
									//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成3次试玩任务<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
								}

								LOGERROR("[CMsgTaskDetail::Process] UpDate CompleteNum Is Suc nId=[%d] TaskId=[%d]", nId, nTaskId);
								return PACKET_EXE_CONTINUE;
							}
							else	//nCompleteNum >= nCompleteNumCfg
							{
								CMsgTaskDetail msg;
								IF_OK(msg.CreateMsg(_TASK_DETAIL_TRY_GAME_FAIL))
								{
									SendMsg(&msg);
								}

								LOGERROR("[CMsgTaskDetail::Process] nCompleteNum >= nCompleteNumCfg Is Error nId=[%d] TaskId=[%d] nCompleteNum=[%d] nCompleteNumCfg=[%d]", nId, nTaskId, nCompleteNum, nCompleteNumCfg);
								return PACKET_EXE_CONTINUE;
							}
						}
						else	//pTaskDetailCfg == NULL
						{
							CMsgTaskDetail msg;
							IF_OK(msg.CreateMsg(_TASK_DETAIL_AWARD_FAIL))
							{
								SendMsg(&msg);
							}	

							LOGERROR("[CMsgTaskDetail::Process] IsNot Found Task Info Config pTaskDetailCfg==NULL nId=[%d] nTaskId=[%d]", nId, nTaskId);
							return PACKET_EXE_CONTINUE;
						}
					}
					else	//pData == NULL
					{
						CMsgTaskDetail msg;
						IF_OK(msg.CreateMsg(_TASK_DETAIL_AWARD_FAIL))
						{
							SendMsg(&msg);
						}	

						LOGERROR("[CMsgTaskDetail::Process] IsNot Found TaskDetailData Is TaskDetailManagerp pData==NULL nId=[%d]", nId);
						return PACKET_EXE_CONTINUE;
					}
				}
				else	//pTaskDetailManager == NULL
				{
					CMsgTaskDetail msg;
					IF_OK(msg.CreateMsg(_TASK_DETAIL_AWARD_FAIL))
					{
						SendMsg(&msg);
					}	

					LOGERROR("[CMsgTaskDetail::Process] Is Error pTaskDetailManager==NULL nId=[%d] ", nId);
					return PACKET_EXE_CONTINUE;
				}

				LOGERROR("[CMsgTaskDetail::Process] Is Suc");
				return PACKET_EXE_CONTINUE;
			}
			break;
			default:
			{
				LOGDEBUG("[CMsgSingUp::Process] Is Error pAction=[%d] pPlayer=[%p]", pAction->valueint, m_pPlayer);
			}
			break;
		}
	}

	return PACKET_EXE_ERROR;
	DEBUG_CATCH0("[CMsgTaskDetail::Process] Is Catch...");
}