#include "../Base/logger.h"
#include "../Main/ServerManager.h"
#include "AllMsg.h"
#include "../Main/Item/ItemAward.h"
#include "../Main/Item/ItemAwardManager.h"
#include "../Main/Item/ItemType.h"
#include "../Main/Item/ItemTypeManager.h"
#include "../Main/Item/ItemAwardGroupManager.h"


//////////////////////////////////////////////////// CMsgSingUp ////////////////////////////////////////////////////
//构造函数
CMsgSingUp::CMsgSingUp() 
{ 
}

//析构函数
CMsgSingUp::~CMsgSingUp() 
{ 	
}

//读取数据
BOOL CMsgSingUp::Read(SocketInputStream& iStream, UINT nLen)
{
	CNetMsg::Read(iStream, nLen);
	return TRUE;
}

BOOL CMsgSingUp::Write(SocketOutputStream& oStream) const
{
	return TRUE;
}

//创建消息
BOOL CMsgSingUp::CreateMsg(UINT nAction, UINT nType/*=0*/, UINT nData/*=0*/)
{
	if (m_pMsg)
	{
		cJSON_AddNumberToObject(m_pMsg, "action", nAction);
		if (nAction == _ACT_SIGNUP_CMD_FAIL)
		{				
		}
		else if (nAction == _ACT_SIGNUP_CMD_SUC)
		{	
			cJSON_AddNumberToObject(m_pMsg, "type", nType);	
			cJSON_AddNumberToObject(m_pMsg, "dayindex", nData);			
		}

		m_pData = cJSON_Print(m_pMsg);
		string strMsg(m_pData);
	    size_t nLen = strMsg.size();
		LOGDEBUG("[CMsgSingUp::CreateMsg]:\n m_pData=[%s] nLen=[%d]\n", m_pData, nLen);
	}	

	return TRUE;
}

//发送json数据
BOOL CMsgSingUp::SendMsg(CNetMsg* pMsg)
{
	m_pPlayer->SendMsgToClient(pMsg);
	return TRUE;	
}

//处理前端发送过来的login消息
UINT CMsgSingUp::Process(VOID *pInfo)
{	
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
			LOGDEBUG("[CMsgSingUp::Process] PlayerId=[%d] pAction=[%d] pPlayer=[%p]", m_pPlayer->GetUid(), pAction->valueint, m_pPlayer);
			switch(pAction->valueint)
			{
				//////////////////////////////////////////////////////////////////////////////////////////
				///签到
				case _ACT_SIGNUP_CMD:
				{
					cJSON* pDayIndex = cJSON_GetObjectItem(pJsonMsg, "dayindex");
					if (pDayIndex == 0x0)	//签到
					{
						//获取今日星期几
						/*int tm_wday;  星期 – 取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推 */
						//生成对战道具并广播给队伍成员
						time_t tInput = time(NULL);
						tm * pTm = localtime(&tInput);
						INT nCurTmWday = 0;
						INT nMaskIdx = 0;
						if (pTm)
						{
							nCurTmWday = pTm->tm_wday;
							nMaskIdx = (nCurTmWday + 6) % 7;
							LOGDEBUG("[CMsgSingUp::Process] nCurTmWday=[%d] nMaskIdx=[%d]", nCurTmWday, nMaskIdx);

							//检查是否签到过
							if(m_pPlayer->ChkWeekTaskMask(nMaskIdx))
							{
								CMsgSingUp msg;
								IF_OK(msg.CreateMsg(_ACT_SIGNUP_CMD_FAIL))
								{
									SendMsg(&msg);
								}

								LOGERROR("[CMsgSingUp::Process] ChkWeekTaskMask Is Set nCurTmWday=[%d] nMaskIdx=[%d]", nCurTmWday, nMaskIdx);
								return PACKET_EXE_CONTINUE;
							}

							//签到有单倍奖励和多倍奖励之分
							INT nAwardMulti = 1;
							cJSON* pType = cJSON_GetObjectItem(pJsonMsg, "type");
							if (pType)
							{ 
								if(pType->valueint == 1)
								{
									nAwardMulti = 2;
								}
							}
							// else
							// {
							// 	CMsgSingUp msg;
							// 	IF_OK(msg.CreateMsg(_ACT_SIGNUP_CMD_FAIL))
							// 	{
							// 		SendMsg(&msg);
							// 	}

							// 	LOGERROR("[CMsgSingUp::Process] Param type Is Disappearance nCurTmWday=[%d] nMaskIdx=[%d]", nCurTmWday, nMaskIdx);
							// 	return PACKET_EXE_CONTINUE;
							// }

							//未领取则设置掩码位
							m_pPlayer->AddWeekTaskMask(nMaskIdx);
							LOGDEBUG("[CMsgSingUp::Process] AddWeekTaskMask Is Ok nMaskIdx=[%d] nAwardMulti=[%d]", nMaskIdx, nAwardMulti);

							//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 1	每日签到<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
							if (!m_pPlayer->TriggerDayTaskDetail(DAY_TASK_DETAIL_TYPE_1))
							{
								LOGERROR("[CMsgSingUp::Process] TriggerDayTaskDetail Is Error");
							}
							//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 1	每日签到<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//

							//发放奖励
							CItemAwardManager* pItemAwardManager = QueryItemAwardManager();
							if (pItemAwardManager)
							{
								if(!pItemAwardManager->AwardItemToPlayer(m_pPlayer, SIGN_UP_AWARD_ID+nMaskIdx+1, nAwardMulti))
								{
									CMsgSingUp msg;
									IF_OK(msg.CreateMsg(_ACT_SIGNUP_CMD_FAIL))
									{
										SendMsg(&msg);
									}

									LOGERROR("[CMsgSingUp::Process] AwardItemToPlayer Is Error nCurTmWday=[%d] nMaskIdx=[%d]", nCurTmWday, nMaskIdx);
									return PACKET_EXE_CONTINUE;
								}

								//补签成功
								CMsgSingUp msg;
								IF_OK(msg.CreateMsg(_ACT_SIGNUP_CMD_SUC, pType->valueint, nCurTmWday))
								{
									SendMsg(&msg);
								}
							}
							else//pItemAwardManager
							{
								CMsgSingUp msg;
								IF_OK(msg.CreateMsg(_ACT_SIGNUP_CMD_FAIL))
								{
									SendMsg(&msg);
								}

								LOGERROR("[CMsgSingUp::Process] pItemAwardManager == NULL");
								return PACKET_EXE_CONTINUE;
							}
						}
						else	//pTm
						{
							CMsgSingUp msg;
							IF_OK(msg.CreateMsg(_ACT_SIGNUP_CMD_FAIL))
							{
								SendMsg(&msg);
							}

							LOGERROR("[CMsgSingUp::Process] pTm == NULL");
							return PACKET_EXE_CONTINUE;
						}
					}
					else	//补签
					{
						if (pDayIndex->valueint < 0 || pDayIndex->valueint > 6)	
						{
							CMsgSingUp msg;
							IF_OK(msg.CreateMsg(_ACT_SIGNUP_CMD_FAIL))
							{
								SendMsg(&msg);
							}

							LOGERROR("[CMsgSingUp::Process] pDayIndex->valueint=[%d]", pDayIndex->valueint);
							return PACKET_EXE_CONTINUE;
						}

						//获取今日星期几
						/*int tm_wday;  星期 – 取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推 */
						//生成对战道具并广播给队伍成员
						time_t tInput = time(NULL);
						tm * pTm = localtime(&tInput);
						INT nCurTmWday = 0;
						INT nMaskIdx = 0;					//今天的掩码位
						INT nDayMaskIdx = 0;				//补签上来的天掩码位
						if (pTm)
						{
							nCurTmWday = pTm->tm_wday;
							nMaskIdx = (nCurTmWday + 6) % 7;
							nDayMaskIdx  = (pDayIndex->valueint + 6) % 7;

							//只能补签今天之前的掩码位
							if (nMaskIdx <= nDayMaskIdx)
							{
								CMsgSingUp msg;
								IF_OK(msg.CreateMsg(_ACT_SIGNUP_CMD_FAIL))
								{
									SendMsg(&msg);
								}

								LOGERROR("[CMsgSingUp::Process] Is Error... nMaskIdx <= nDayMaskIdx nMaskIdx=[%d] nDayMaskIdx=[%d]", nMaskIdx, nDayMaskIdx);
								return PACKET_EXE_CONTINUE;
							}

							LOGDEBUG("[CMsgSingUp::Process] pDayIndex->valueint=[%d] nDayMaskIdx=[%d]", pDayIndex->valueint, nDayMaskIdx);

							//检查是否签到过
							if(m_pPlayer->ChkWeekTaskMask(nDayMaskIdx))
							{
								CMsgSingUp msg;
								IF_OK(msg.CreateMsg(_ACT_SIGNUP_CMD_FAIL))
								{
									SendMsg(&msg);
								}

								LOGERROR("[CMsgSingUp::Process] ChkWeekTaskMask Is Set pDayIndex->valueint=[%d] nDayMaskIdx=[%d]", pDayIndex->valueint, nDayMaskIdx);
								return PACKET_EXE_CONTINUE;
							}

							//未领取则设置掩码位
							m_pPlayer->AddWeekTaskMask(nDayMaskIdx);
							LOGDEBUG("[CMsgSingUp::Process] AddDayTaskMask Is Ok nDayMaskIdx=[%d]", nDayMaskIdx);

							//发放奖励
							CItemAwardManager* pItemAwardManager = QueryItemAwardManager();
							if (pItemAwardManager)
							{
								if(!pItemAwardManager->AwardItemToPlayer(m_pPlayer, SIGN_UP_AWARD_ID+nDayMaskIdx+1, 1))
								{
									CMsgSingUp msg;
									IF_OK(msg.CreateMsg(_ACT_SIGNUP_CMD_FAIL))
									{
										SendMsg(&msg);
									}

									LOGERROR("[CMsgSingUp::Process] AwardItemToPlayer Is Error pDayIndex->valueint=[%d] nDayMaskIdx=[%d]", pDayIndex->valueint, nDayMaskIdx);
									return PACKET_EXE_CONTINUE;
								}

								//补签成功
								CMsgSingUp msg;
								IF_OK(msg.CreateMsg(_ACT_SIGNUP_CMD_SUC, 0, pDayIndex->valueint))
								{
									SendMsg(&msg);
								}
								
								return PACKET_EXE_CONTINUE;
							}
							else//pItemAwardManager
							{
								CMsgSingUp msg;
								IF_OK(msg.CreateMsg(_ACT_SIGNUP_CMD_FAIL))
								{
									SendMsg(&msg);
								}

								LOGERROR("[CMsgSingUp::Process] pItemAwardManager == NULL");
								return PACKET_EXE_CONTINUE;
							}
						}
						else	//pTm
						{
							CMsgSingUp msg;
							IF_OK(msg.CreateMsg(_ACT_SIGNUP_CMD_FAIL))
							{
								SendMsg(&msg);
							}

							LOGERROR("[CMsgSingUp::Process] pTm == NULL");
							return PACKET_EXE_CONTINUE;
						}
					}					
				}
				break;
				default:
				{
					LOGDEBUG("[CMsgSingUp::Process] Is Error pAction=[%d] pPlayer=[%p]", pAction->valueint, m_pPlayer);
				}
				break;
			}
		}
	}

	return PACKET_EXE_CONTINUE;
	DEBUG_CATCH0("[CMsgSingUp::Process] Is Catch...");
}