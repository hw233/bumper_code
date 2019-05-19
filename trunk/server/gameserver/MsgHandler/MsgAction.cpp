#include "../Base/logger.h"
#include "../Main/ServerManager.h"
#include "../Main/Item/ItemAwardManager.h"
#include "../Main/OnLineAward/OnLineAwardCfgManager.h"
#include "AllMsg.h"

//////////////////////////////////////////////////// CMsgAction ////////////////////////////////////////////////////
//构造函数
CMsgAction::CMsgAction() 
{ 
}

//析构函数
CMsgAction::~CMsgAction() 
{ 	
}

//读取数据
BOOL CMsgAction::Read(SocketInputStream& iStream, UINT nLen)
{
	CNetMsg::Read(iStream, nLen);
	return TRUE;
}

BOOL CMsgAction::Write(SocketOutputStream& oStream) const
{
	return TRUE;
}
//创建消息
BOOL CMsgAction::CreateMsg(UINT nAction, UINT nTargetId/*=0*/, UINT nType/*=0*/, UINT nData1/*=0*/, UINT nData2/*=0*/, UINT nParam1/*=0*/, UINT nParam2/*=0*/)
{
	if (m_pMsg)
	{
		cJSON_AddNumberToObject(m_pMsg, "action", nAction);

		//开启宝箱成功
		if (nAction == _ACTION_OPEN_BOX_SUC)
		{
			cJSON_AddNumberToObject(m_pMsg, "idx", nTargetId);
		}
		//在线领奖成功
		else if (nAction == _ACTION_ONLINE_AWARD_SUC)
		{
			cJSON_AddNumberToObject(m_pMsg, "online_idx", nTargetId);
		}
		//限时领奖成功
		else if (nAction == _ACTION_LIMIT_TIME_AWARD_SUC)
		{
			cJSON_AddNumberToObject(m_pMsg, "show_item_type", nTargetId);
			cJSON_AddNumberToObject(m_pMsg, "show_remain_ts", nType);
			cJSON_AddNumberToObject(m_pMsg, "life_time", nData1);
		}
		//限时领奖-皮肤使用-继续使用成功
		else if (_ACTION_LIMIT_TIME_CONTINUE_TRY_SUC)
		{
			cJSON_AddNumberToObject(m_pMsg, "item_id", nTargetId);
		}
		
		m_pData = cJSON_Print(m_pMsg);
		string strMsg(m_pData);
	    size_t nLen = strMsg.size();
		LOGDEBUG("[CMsgAction::CreateMsg]:\n m_pData=%s nLen=[%d]\n", m_pData, nLen);	
	}

	return TRUE;
}

//发送json数据
BOOL CMsgAction::SendMsg(CNetMsg* pMsg)
{
	m_pPlayer->SendMsgToClient(pMsg);
	return TRUE;	
}

//处理前端发送过来的消息
UINT CMsgAction::Process(VOID *pInfo)
{	
	LOGDEBUG("[CMsgAction::Process] begin");

	DEBUG_TRY
	//设置需要回包的玩家
	m_pPlayer = static_cast<Player*>(pInfo);
	CHECKF(m_pPlayer);

	//处理前端发过来的消息
	m_pMsg = cJSON_Parse(this->m_pData);
	cJSON* pJsonMsg = m_pMsg;	
	if(pJsonMsg)
	{
		cJSON* pAction = cJSON_GetObjectItem(pJsonMsg, "action");				//_BATTLE_ACTION		
		CHECKF(pAction);

		LOGDEBUG("[CMsgAction::Process] PlayerId=[%d] pAction=[%d] pPlayer=[%p]", m_pPlayer->GetUid(), pAction->valueint, m_pPlayer);
		switch(pAction->valueint)
		{
			///////////////////////////////////////////////////////////////////////////
			///大乱斗战斗结束后返回游戏主界面
			case _ACTION_GO_BACK_HOME_SCREEN:
			{
				//清空玩家BUFF商城相关数据
				m_pPlayer->SetAddGold(0);					//战斗结束后奖励的金币，金币加成200%
				m_pPlayer->SetAddScore(0);					//积分基础加:3000

				//清除玩家的上局战斗结果
    			m_pPlayer->SetFightTeamResult(_FIGHT_RESULT_NONE);

				//玩家死亡之后游戏结束，服务器没有收到复活包退出游戏返回主界面，直接设置为活着状态
				m_pPlayer->SetDeadStatus(_STATUS_ALIVE);

				//add by huyf 2018.11.24:添加好友系统
				m_pPlayer->SetGameStatus(_FRIEND_STATUS_ONLINE);
				//add end:添加好友系统

				//同步玩家属性
				CMsgUserInfo msg;
				if(msg.CreateMsg(m_pPlayer))
				{
					SendMsg(&msg);
				}

				//add by huyf 2018.11.24:添加好友系统
				//广播玩家进入战斗状态给自己好友
				CMsgFriendInfo msg2;
				IF_OK(msg2.CreateMsg(_FRIEND_INFO_UPDATE, m_pPlayer, m_pPlayer->GetGameStatus(), _FRIEND_STATUS_ONLINE))
				{
					m_pPlayer->QueryFriendManager().BroadcastFriendSMsg(&msg2);
				}
				//add end:添加好友系统
			}
			break;		
			///////////////////////////////////////////////////////////////////////////
			//开启宝箱
			case _ACTION_OPEN_BOX:
			{
				cJSON* pIdx = cJSON_GetObjectItem(pJsonMsg, "idx");				//宝箱索引		
				CHECKF(pIdx);

				//检查参数有效性
				if (pIdx->valueint < 1 || pIdx->valueint > 4)		//1 : 1天宝箱 2 : 3天宝箱 3 : 5天宝箱 4 : 7天宝箱
				{
					CMsgAction msg;
					IF_OK(msg.CreateMsg(_ACTION_OPEN_BOX_FAIL))
					{
						SendMsg(&msg);
					}
					LOGERROR("[CMsgAction::Process] Param Is Error pIdx=[%d]", pIdx->valueint);
					return PACKET_EXE_CONTINUE;
				}

				//查找脚本和掩码位
				UINT nAwardId = pIdx->valueint + OPEN_BOX_AWARD_ID;								//脚本执行ID
				UINT nMaskIdx = pIdx->valueint + OPEN_BOX_DAY_MASK_OFFSET;						//掩码位idx

				//检查是否天数够
				UINT nSignUpCount = 0;
				for (int i = 0; i <= 6; ++i)
				{
					if(m_pPlayer->ChkWeekTaskMask(i))
					{
						nSignUpCount++;
					}
				}
				if (pIdx->valueint == 1)
				{
					if(nSignUpCount < 1)
					{
						CMsgAction msg;
						IF_OK(msg.CreateMsg(_ACTION_OPEN_BOX_FAIL))
						{
							SendMsg(&msg);
						}

						LOGERROR("[CMsgAction::Process] Is Error nSignUpCount < 1 nSignUpCount=[%d] nIdx=[%d] nMaskIdx=[%d]", nSignUpCount, pIdx->valueint, nMaskIdx);
						return PACKET_EXE_CONTINUE;
					}
				}
				else if (pIdx->valueint == 2)
				{
					if(nSignUpCount < 3)
					{
						CMsgAction msg;
						IF_OK(msg.CreateMsg(_ACTION_OPEN_BOX_FAIL))
						{
							SendMsg(&msg);
						}

						LOGERROR("[CMsgAction::Process] Is Error nSignUpCount < 1 nSignUpCount=[%d] nIdx=[%d] nMaskIdx=[%d]", nSignUpCount, pIdx->valueint, nMaskIdx);
						return PACKET_EXE_CONTINUE;
					}
				}
				else if (pIdx->valueint == 3)
				{
					if(nSignUpCount < 5)
					{
						CMsgAction msg;
						IF_OK(msg.CreateMsg(_ACTION_OPEN_BOX_FAIL))
						{
							SendMsg(&msg);
						}

						LOGERROR("[CMsgAction::Process] Is Error nSignUpCount < 1 nSignUpCount=[%d] nIdx=[%d] nMaskIdx=[%d]", nSignUpCount, pIdx->valueint, nMaskIdx);
						return PACKET_EXE_CONTINUE;
					}
				}
				else //if (pIdx->valueint == 4)
				{
					if(nSignUpCount < 7)
					{
						CMsgAction msg;
						IF_OK(msg.CreateMsg(_ACTION_OPEN_BOX_FAIL))
						{
							SendMsg(&msg);
						}

						LOGERROR("[CMsgAction::Process] Is Error nSignUpCount < 1 nSignUpCount=[%d] nIdx=[%d] nMaskIdx=[%d]", nSignUpCount, pIdx->valueint, nMaskIdx);
						return PACKET_EXE_CONTINUE;
					}
				}

				//检查是否签到过
				if(m_pPlayer->ChkWeekTaskMask(nMaskIdx))
				{
					CMsgAction msg;
					IF_OK(msg.CreateMsg(_ACTION_OPEN_BOX_FAIL))
					{
						SendMsg(&msg);
					}

					LOGERROR("[CMsgAction::Process] ChkWeekTaskMask Is Set nMaskIdx=[%d]", nMaskIdx);
					return PACKET_EXE_CONTINUE;
				}

				//未领取则设置掩码位
				m_pPlayer->AddWeekTaskMask(nMaskIdx);
				LOGDEBUG("[CMsgAction::Process] AddWeekTaskMask Is Ok nMaskIdx=[%d] nAwardId=[%d]", nMaskIdx, nAwardId);

				//发放奖励
				CItemAwardManager* pItemAwardManager = QueryItemAwardManager();
				if (pItemAwardManager)
				{
					//补签成功
					if(pItemAwardManager->AwardItemToPlayer(m_pPlayer, nAwardId))
					{
						CMsgAction msg;
						IF_OK(msg.CreateMsg(_ACTION_OPEN_BOX_SUC, pIdx->valueint))
						{
							SendMsg(&msg);
						}

						LOGERROR("[CMsgAction::Process] AwardItemToPlayer Is Suc nAwardId=[%d] nMaskIdx=[%d]", nAwardId, nMaskIdx);
						return PACKET_EXE_CONTINUE;
					}
					else	//补签失败
					{
						CMsgAction msg;
						IF_OK(msg.CreateMsg(_ACTION_OPEN_BOX_FAIL))
						{
							SendMsg(&msg);
						}

						LOGERROR("[CMsgAction::Process] AwardItemToPlayer Is Error nAwardId=[%d] nMaskIdx=[%d]", nAwardId, nMaskIdx);
						return PACKET_EXE_CONTINUE;
					}					
				}
				else//pItemAwardManager
				{
					CMsgAction msg;
					IF_OK(msg.CreateMsg(_ACTION_OPEN_BOX_FAIL))
					{
						SendMsg(&msg);
					}

					LOGERROR("[CMsgAction::Process] pItemAwardManager == NULL");
					return PACKET_EXE_CONTINUE;
				}
			}
			break;
			///////////////////////////////////////////////////////////////////////////
			//限时领奖
			case _ACTION_LIMIT_TIME_AWARD:
			{
				//cJSON* pView = cJSON_GetObjectItem(pJsonMsg, "view");					//观看视屏领取使用皮肤的时候才会有此字段，普通倒计时领奖不需要带此字段				
				UINT nItemType = m_pPlayer->GetShowItemType();							//奖励的道具itemtype
				// if (pView)		//观看视屏领领奖	
				// {
				// 	if (nItemType == 0)
				// 	{
				// 		CMsgAction msg;
				// 		IF_OK(msg.CreateMsg(_ACTION_LIMIT_TIME_AWARD_FAIL))
				// 		{
				// 			SendMsg(&msg);
				// 		}

				// 		LOGERROR("[CMsgAction::Process] Is Error nItemType=[0]");
				// 		return PACKET_EXE_CONTINUE;
				// 	}

				// 	//发放奖励
				// 	CItemDataManager* pItemDataManager = m_pPlayer->QueryItemDataManager();
				// 	if (pItemDataManager)
				// 	{
				// 		//奖励成功
				// 		if(pItemDataManager->AwardItem(nItemType))
				// 		{
				// 			//同步前端消息
				// 			CMsgAction msg;
				// 			IF_OK(msg.CreateMsg(_ACTION_LIMIT_TIME_AWARD_SUC))
				// 			{
				// 				SendMsg(&msg);
				// 			}

				// 			//更新相应字段，筛选下一个道具
				// 			CItemTypeManager* pItemTypeManager = QueryItemTypeManager();
				// 			if (pItemTypeManager)
				// 			{
				// 				CItemType* pItemType = pItemTypeManager->QueryNoExistItemType(m_pPlayer, ITEM_TYPE_CAR_FACE);
				// 				if (pItemType)	//找到相应的道具
				// 				{
				// 					//查到没有的类型道具
				// 					m_pPlayer->SetShowItemType(pItemType->GetItemType());

				// 					//同步前端属性变化
				// 					CMsgUserAttrib msg;
				// 					IF_OK(msg.CreateMsg(_ACT_USERATTRIB_SHOW_ITEM_TYPE, m_pPlayer->GetShowItemType()))
				// 					{
				// 						SendMsg(&msg);
				// 					}

				// 					//设置倒计时 2小时
				// 					UINT nLifeTime = 3600*2;
				// 					m_pPlayer->SetShowRemainTs(nLifeTime);

				// 					//同步前端属性变化
				// 					CMsgUserAttrib msg2;
				// 					IF_OK(msg2.CreateMsg(_ACT_USERATTRIB_SHOW_REMAIN_Ts, nLifeTime))
				// 					{
				// 						SendMsg(&msg2);
				// 					}

				// 					//启动时钟计时器
				// 					m_pPlayer->QueryLimitTimeInterval().Startup(nLifeTime);
				// 				}
				// 				else	//未找到
				// 				{
				// 					//查到没有的类型道具
				// 					m_pPlayer->SetShowItemType(0);

				// 					//同步前端属性变化
				// 					CMsgUserAttrib msg;
				// 					IF_OK(msg.CreateMsg(_ACT_USERATTRIB_SHOW_ITEM_TYPE, m_pPlayer->GetShowItemType()))
				// 					{
				// 						SendMsg(&msg);
				// 					}

				// 					//设置倒计时0
				// 					m_pPlayer->SetShowRemainTs(0);

				// 					//此处OnTimer中已经同步这里不用再做数据同步
				// 					//...
									
				// 					//启动时钟计时器
				// 					m_pPlayer->QueryLimitTimeInterval().Clear();
				// 				}
				// 			}
				// 			else
				// 			{
				// 				CMsgAction msg;
				// 				IF_OK(msg.CreateMsg(_ACTION_LIMIT_TIME_AWARD_FAIL))
				// 				{
				// 					SendMsg(&msg);
				// 				}
				// 				LOGERROR("[MsgLogin::Process] pItemTypeManager == NULL");
				// 				return PACKET_EXE_CONTINUE;
				// 			}

				// 			LOGERROR("[CMsgAction::Process] AwardItem Is Suc nItemType=[%d]", nItemType);
				// 			return PACKET_EXE_CONTINUE;
							
				// 		}
				// 		else	//奖励失败
				// 		{
				// 			CMsgAction msg;
				// 			IF_OK(msg.CreateMsg(_ACTION_LIMIT_TIME_AWARD_FAIL))
				// 			{
				// 				SendMsg(&msg);
				// 			}

				// 			LOGERROR("[CMsgAction::Process] AwardItem Is Error nItemType=[%d]", nItemType);
				// 			return PACKET_EXE_CONTINUE;
				// 		}
				// 	}
				// 	else//pItemAwardManager
				// 	{
				// 		CMsgAction msg;
				// 		IF_OK(msg.CreateMsg(_ACTION_LIMIT_TIME_AWARD_FAIL))
				// 		{
				// 			SendMsg(&msg);
				// 		}

				// 		LOGERROR("[CMsgAction::Process] pItemAwardManager == NULL");
				// 		return PACKET_EXE_CONTINUE;
				// 	}
				// }
				// else //普通倒计时领奖
				{
					//判断倒计时是否结束
					// if (m_pPlayer->GetShowRemainTs() > 0)
					// {
					// 	CMsgAction msg;
					// 	IF_OK(msg.CreateMsg(_ACTION_LIMIT_TIME_AWARD_FAIL))
					// 	{
					// 		SendMsg(&msg);
					// 	}

					// 	LOGERROR("[CMsgAction::Process] Remain Is Not TimeOver RemainTs=[%d]", m_pPlayer->GetShowRemainTs());
					// 	return PACKET_EXE_CONTINUE;
					// }

					if (nItemType == 0)
					{
						CMsgAction msg;
						IF_OK(msg.CreateMsg(_ACTION_LIMIT_TIME_AWARD_FAIL))
						{
							SendMsg(&msg);
						}

						LOGERROR("[CMsgAction::Process] Is Error nItemType=[0]");
						return PACKET_EXE_CONTINUE;
					}

					//发放奖励
					CItemDataManager* pItemDataManager = m_pPlayer->QueryItemDataManager();
					if (pItemDataManager)
					{
						//奖励成功
						CItemData* pItemData = pItemDataManager->AwardItem(nItemType);
						if(pItemData)
						{
							// //同步前端属性变化
							// CMsgAction msg;
							// IF_OK(msg.CreateMsg(_ACTION_LIMIT_TIME_AWARD_SUC))
							// {
							// 	SendMsg(&msg);
							// }

							//更新相应字段，筛选下一个道具
							CItemTypeManager* pItemTypeManager = QueryItemTypeManager();
							if (pItemTypeManager)
							{
								CItemType* pItemType = pItemTypeManager->QueryNoExistItemType(m_pPlayer, ITEM_TYPE_CAR_FACE);
								if (pItemType)	//找到相应的道具
								{
									//查到没有的类型道具
									m_pPlayer->SetShowItemType(pItemType->GetItemType());

									//同步前端属性变化
									// CMsgUserAttrib msg;
									// IF_OK(msg.CreateMsg(_ACT_USERATTRIB_SHOW_ITEM_TYPE, m_pPlayer->GetShowItemType()))
									// {
									// 	SendMsg(&msg);
									// }

									//设置倒计时 2小时
									UINT nLifeTime = 3600*2;
									m_pPlayer->SetShowRemainTs(nLifeTime);									

									//同步前端属性变化
									// CMsgUserAttrib msg2;
									// IF_OK(msg2.CreateMsg(_ACT_USERATTRIB_SHOW_REMAIN_Ts, nLifeTime))
									// {
									// 	SendMsg(&msg2);
									// }

									//启动时钟计时器
									m_pPlayer->QueryLimitTimeInterval().Startup(nLifeTime);

									//同步前端属性变化
									CMsgAction msg;
									IF_OK(msg.CreateMsg(_ACTION_LIMIT_TIME_AWARD_SUC, pItemType->GetItemType(), nLifeTime, pItemData->GetLifeTime()))
									{
										SendMsg(&msg);
									}
								}
								else	//未找到
								{
									//查到没有的类型道具
									m_pPlayer->SetShowItemType(0);

									//同步前端属性变化
									// CMsgUserAttrib msg;
									// IF_OK(msg.CreateMsg(_ACT_USERATTRIB_SHOW_ITEM_TYPE, m_pPlayer->GetShowItemType()))
									// {
									// 	SendMsg(&msg);
									// }

									//设置倒计时0
									m_pPlayer->SetShowRemainTs(0);

									//此处OnTimer中已经同步这里不用再做数据同步
									//...
									
									//启动时钟计时器
									m_pPlayer->QueryLimitTimeInterval().Clear();

									//同步前端属性变化
									CMsgAction msg;
									IF_OK(msg.CreateMsg(_ACTION_LIMIT_TIME_AWARD_SUC, m_pPlayer->GetShowItemType(), 0, 0))
									{
										SendMsg(&msg);
									}
								}
							}
							else
							{
								CMsgAction msg;
								IF_OK(msg.CreateMsg(_ACTION_LIMIT_TIME_AWARD_FAIL))
								{
									SendMsg(&msg);
								}
								LOGERROR("[MsgLogin::Process] pItemTypeManager == NULL");
								return PACKET_EXE_CONTINUE;
							}

							LOGERROR("[CMsgAction::Process] AwardItem Is Suc nItemType=[%d]", nItemType);
							return PACKET_EXE_CONTINUE;							
						}
						else	//奖励失败
						{
							CMsgAction msg;
							IF_OK(msg.CreateMsg(_ACTION_LIMIT_TIME_AWARD_FAIL))
							{
								SendMsg(&msg);
							}

							LOGERROR("[CMsgAction::Process] AwardItem Is Error nItemType=[%d]", nItemType);
							return PACKET_EXE_CONTINUE;
						}
					}
					else//pItemAwardManager
					{
						CMsgAction msg;
						IF_OK(msg.CreateMsg(_ACTION_LIMIT_TIME_AWARD_FAIL))
						{
							SendMsg(&msg);
						}

						LOGERROR("[CMsgAction::Process] pItemAwardManager == NULL");
						return PACKET_EXE_CONTINUE;
					}
				}
			}
			break;			
			///////////////////////////////////////////////////////////////////////////
			//在线领奖
			case _ACTION_ONLINE_AWARD:
			{			
				UINT nRemainTs = m_pPlayer->QueryOnLineInterval().GetRemain();							//获取在线领奖剩余时间戳（秒）
				UINT nItemType = m_pPlayer->GetOnLineItemType();							//奖励的道具itemtype
				if (nRemainTs > 0)		//倒计时还未结束	
				{
					CMsgAction msg;
					IF_OK(msg.CreateMsg(_ACTION_ONLINE_AWARD_FAIL))
					{
						SendMsg(&msg);
					}

					LOGERROR("[CMsgAction::Process] nRemainTs > 0 nRemainTs=[%d]", nRemainTs);
					return PACKET_EXE_CONTINUE;
				}

				if (nItemType == 0)
				{
					CMsgAction msg;
					IF_OK(msg.CreateMsg(_ACTION_ONLINE_AWARD_FAIL))
					{
						SendMsg(&msg);
					}

					LOGERROR("[CMsgAction::Process] Is Error nItemType=[0]");
					return PACKET_EXE_CONTINUE;
				}

				//发放奖励
				CItemDataManager* pItemDataManager = m_pPlayer->QueryItemDataManager();
				if(pItemDataManager)
				{
					//查看awardid奖励组配置					
					COnLineAwardCfgManager* pOnLineAwardCfgManager = QueryOnLineAwardCfgManager();
					if(!pOnLineAwardCfgManager)
					{
						CMsgAction msg;
						IF_OK(msg.CreateMsg(_ACTION_ONLINE_AWARD_FAIL))
						{
							SendMsg(&msg);
						}

						LOGERROR("[CMsgAction::Process] pItemAwardManager == NULL");
						return PACKET_EXE_CONTINUE;
					}
					UINT nId = m_pPlayer->GetOnLineIdx();
					COnLineAwardCfg* pOnLineAwardCfg = pOnLineAwardCfgManager->QueryOnLineAwardCfg(nId);
					if(!pOnLineAwardCfg)
					{
						CMsgAction msg;
						IF_OK(msg.CreateMsg(_ACTION_ONLINE_AWARD_FAIL))
						{
							SendMsg(&msg);
						}

						LOGERROR("[CMsgAction::Process] pOnLineAwardCfg == NULL nId=[%d]", nId);
						return PACKET_EXE_CONTINUE;
					}

					//读取奖励道相关配置
					UINT nAwardItemType = pOnLineAwardCfg->GetItemType();
					UINT nAwardItemAmount = pOnLineAwardCfg->GetItemAmount();
					//奖励成功
					if(!pItemDataManager->AwardItem(nAwardItemType, nAwardItemAmount))
					{
						CMsgAction msg;
						IF_OK(msg.CreateMsg(_ACTION_ONLINE_AWARD_FAIL))
						{
							SendMsg(&msg);
						}

						LOGERROR("[CMsgAction::Process] AwardItem Is Error nItemType=[%d]", nItemType);
						return PACKET_EXE_CONTINUE;
					}

					//查找到相应的在线奖励awardid配置
					UINT nAwardId = pOnLineAwardCfg->GetAwardId();
					if (nAwardId > 0)		//配置了奖励组，则进行多层次奖励发放
					{
						//发放奖励
						CItemAwardManager* pItemAwardManager = QueryItemAwardManager();
						if (pItemAwardManager)
						{
							if(!pItemAwardManager->AwardItemToPlayer(m_pPlayer, nAwardId))
							{
								CMsgAction msg;
								IF_OK(msg.CreateMsg(_ACTION_ONLINE_AWARD_FAIL))
								{
									SendMsg(&msg);
								}

								LOGERROR("[CMsgAction::Process] AwardItem Is Error nItemType=[%d]", nItemType);
								return PACKET_EXE_CONTINUE;
							}
						}
					}
					else	//未配置奖励组-不用管
					{
						//...
					}

					//奖励发放完毕进行下一轮设置
					//设置下一个阶段奖励索引
					UINT nNextIdx = pOnLineAwardCfg->GetNextId();
					if (nNextIdx > 0)
					{
						m_pPlayer->SetOnLineIdx(nNextIdx);
						//同步前端属性变更包
						//...暂时只做服务器保持，客户端暂时不用
						
						pOnLineAwardCfg = pOnLineAwardCfgManager->QueryOnLineAwardCfg(nNextIdx);
						if(!pOnLineAwardCfg)
						{
							CMsgAction msg;
							IF_OK(msg.CreateMsg(_ACTION_ONLINE_AWARD_FAIL))
							{
								SendMsg(&msg);
							}

							LOGERROR("[CMsgAction::Process] pOnLineAwardCfg == NULL nNextIdx=[%d]", nNextIdx);
							return PACKET_EXE_CONTINUE;
						}

						//设置下一个阶段道具
						UINT nNextItemType = pOnLineAwardCfg->GetItemType();
						m_pPlayer->SetOnLineItemType(nNextItemType);

						//同步前端属性变更包
						CMsgUserAttrib msg;
						IF_OK(msg.CreateMsg(_ACT_USERATTRIB_ONLINE_ITEM_TYPE, nNextItemType))
						{
							SendMsg(&msg);
						}

						//设置下一个阶段时间戳
						UINT nNextIntervalTime = pOnLineAwardCfg->GetIntervalTime();
						m_pPlayer->SetOnLineRemainTs(nNextIntervalTime);

						//启动时钟计时器
						m_pPlayer->QueryOnLineInterval().Startup(nNextIntervalTime);

						//下一个awardid
						UINT nNextAwardId = pOnLineAwardCfg->GetAwardId();

						//同步前端属性变更包
						CMsgUserAttrib msg2;
						IF_OK(msg2.CreateMsg(_ACT_USERATTRIB_ONLINE_REMAIN_Ts, nNextIntervalTime))
						{
							SendMsg(&msg2);
						}

						LOGDEBUG("[CMsgAction::Process] Is Suc... nIdx=[%d] nItemType=[%d] nAwardId=[%d] nNextIdx=[%d] nNextItemType=[%d] nNextIntervalTime=[%d] nNextAwardId=[%d]", 
							nId,nItemType,nAwardId,nNextIdx,nNextItemType,nNextIntervalTime,nNextAwardId);
					}
					else	//nNextIdx == 0
					{
						m_pPlayer->SetOnLineIdx(0);
						m_pPlayer->SetOnLineItemType(0);

						//同步前端属性变更包
						CMsgUserAttrib msg;
						IF_OK(msg.CreateMsg(_ACT_USERATTRIB_ONLINE_ITEM_TYPE, m_pPlayer->GetOnLineItemType()))
						{
							SendMsg(&msg);
						}

						m_pPlayer->SetOnLineRemainTs(0);
						//启动时钟计时器
						m_pPlayer->QueryOnLineInterval().Clear();

						//同步前端属性变更包
						CMsgUserAttrib msg2;
						IF_OK(msg2.CreateMsg(_ACT_USERATTRIB_ONLINE_REMAIN_Ts, m_pPlayer->GetOnLineRemainTs()))
						{
							SendMsg(&msg2);
						}

						LOGDEBUG("[CMsgAction::Process] Is Suc... nIdx=[%d] nItemType=[%d] nAwardId=[%d] nNextIdx=[0] nNextItemType=[0] nNextIntervalTime=[0] nNextAwardId=[0]", 
							nId,nItemType,nAwardId);
					}

					//索引又要通过这个包给前端同步过去，越弄越乱.....
					CMsgAction msg3;
					IF_OK(msg3.CreateMsg(_ACTION_ONLINE_AWARD_SUC, nId))
					{
						SendMsg(&msg3);
					}

					LOGERROR("[CMsgAction::Process] Is Suc... nIdx=[%d] nItemType=[%d] nAwardId=[%d]", nId,nItemType,nAwardId);

					return PACKET_EXE_CONTINUE;
				}
				else//pItemAwardManager
				{
					CMsgAction msg;
					IF_OK(msg.CreateMsg(_ACTION_ONLINE_AWARD_FAIL))
					{
						SendMsg(&msg);
					}

					LOGERROR("[CMsgAction::Process] pItemAwardManager == NULL");
					return PACKET_EXE_CONTINUE;
				}
			}
			break;
			///////////////////////////////////////////////////////////////////////////
			//在线加速
			case _ACTION_ONLINE_QUICK:
			{			
				UINT nRemainTs = m_pPlayer->QueryOnLineInterval().GetRemain();							//获取在线领奖剩余时间戳（秒）
				UINT nItemType = m_pPlayer->GetOnLineItemType();											//奖励的道具itemtype
				// if (nRemainTs <= 0)		//倒计时已经结束
				// {
				// 	CMsgAction msg;
				// 	IF_OK(msg.CreateMsg(_ACTION_ONLINE_QUICK_FAIL))
				// 	{
				// 		SendMsg(&msg);
				// 	}

				// 	LOGERROR("[CMsgAction::Process] nRemainTs == 0");
				// 	return PACKET_EXE_CONTINUE;
				// }

				if (nItemType == 0)
				{
					CMsgAction msg;
					IF_OK(msg.CreateMsg(_ACTION_ONLINE_AWARD_FAIL))
					{
						SendMsg(&msg);
					}

					LOGERROR("[CMsgAction::Process] Is Error nItemType=[0]");
					return PACKET_EXE_CONTINUE;
				}

				//减少时间戳
				UINT nDecIntervalTs = 30*60;
				//计时器剩余时间大于30分钟
				if (nRemainTs > nDecIntervalTs)
				{
					m_pPlayer->SetOnLineRemainTs(nRemainTs-nDecIntervalTs);
					//时钟计时器时间周期减少
					m_pPlayer->QueryOnLineInterval().DecInterval(nDecIntervalTs);

					LOGDEBUG("[CMsgAction::Process] Is Suc... nRemainTs=[%d] nItemType=[%d] nDecIntervalTs=[%d] AfterRemainTs=[%d]", nRemainTs,nItemType,nDecIntervalTs,nRemainTs-nDecIntervalTs);
				}
				else		//小于30分钟
				{
					m_pPlayer->SetOnLineRemainTs(0);
					//时钟计时器时间停止计时
					m_pPlayer->QueryOnLineInterval().Clear();

					LOGDEBUG("[CMsgAction::Process] Is Suc... nRemainTs=[%d] nItemType=[%d] nDecIntervalTs=[%d] AfterRemainTs=[0]", nRemainTs,nItemType,nDecIntervalTs);
				}

				//同步前端属性变更包
				CMsgUserAttrib msg2;
				IF_OK(msg2.CreateMsg(_ACT_USERATTRIB_ONLINE_REMAIN_Ts, m_pPlayer->GetOnLineRemainTs()))
				{
					SendMsg(&msg2);
				}

				CMsgAction msg3;
				IF_OK(msg3.CreateMsg(_ACTION_ONLINE_QUICK_SUC))
				{
					SendMsg(&msg3);
				}

				return PACKET_EXE_CONTINUE;
			}
			break;
			///////////////////////////////////////////////////////////////////////////
			//限时领奖-皮肤使用-继续使用
			case _ACTION_LIMIT_TIME_CONTINUE_TRY:
			{
				cJSON* pItemId = cJSON_GetObjectItem(pJsonMsg, "item_id");					//继续试用的时候传递道具唯一id		
				CHECKF(pItemId);

				UINT nItemId = pItemId->valueint;
				if (nItemId > 0)		//继续试用流程
				{
					//玩家道具管理器
					CItemDataManager* pItemDataManager = m_pPlayer->QueryItemDataManager();
					if (pItemDataManager)
					{
						//查找到该道具
						CItemData* pItemData = pItemDataManager->QueryItemData(nItemId);
						if(pItemData)
						{
							//不做有效限时道具检测了
							//...
							
							CItemTypeManager* pItemTypeManager = QueryItemTypeManager();
							if (pItemTypeManager)
							{
								CItemType* pItemType = pItemTypeManager->QueryItemType(pItemData->GetItemType());
								if (pItemType)	//找到相应的道具
								{
									//获取该道具时限
									UINT nRemainTs = pItemType->GetLifeTime();
									if (nRemainTs == 0)
									{
										//同步前端消息
										CMsgAction msg;
										IF_OK(msg.CreateMsg(_ACTION_LIMIT_TIME_CONTINUE_TRY_FAIL))
										{
											SendMsg(&msg);
										}

										LOGDEBUG("[MsgLogin::Process] nRemainTs == 0 ItemType=[%d]", pItemData->GetItemType());
										return PACKET_EXE_CONTINUE;
									}

									//更新道具时限
									pItemData->SetLifeTime(nRemainTs);
									//同步前端道具时限属性
									CMsgItemAttrib msg;
									IF_OK(msg.CreateMsg(_ITEMATTRIB_LIFE_TIME, nItemId, nRemainTs))
									{
										SendMsg(&msg);
									}

									//启动计时器
									pItemData->QueryLeftTimeOut().Startup(nRemainTs);

									//同步前端消息
									CMsgAction msg2;
									IF_OK(msg2.CreateMsg(_ACTION_LIMIT_TIME_CONTINUE_TRY_SUC, nItemId))
									{
										SendMsg(&msg2);
									}

									LOGDEBUG("[MsgLogin::Process] Is Suc continue try id=[%d]", nItemId);
									return PACKET_EXE_CONTINUE;
								}
								else	//pItemType == NULL
								{
									//同步前端消息
									CMsgAction msg;
									IF_OK(msg.CreateMsg(_ACTION_LIMIT_TIME_CONTINUE_TRY_FAIL))
									{
										SendMsg(&msg);
									}

									LOGDEBUG("[MsgLogin::Process] Not Found This ItemType Config pItemType == NULL ItemType=[%d]", pItemData->GetItemType());
									return PACKET_EXE_CONTINUE;
								}
							}
							else	//pItemTypeManager == NULL
							{
								//同步前端消息
								CMsgAction msg;
								IF_OK(msg.CreateMsg(_ACTION_LIMIT_TIME_CONTINUE_TRY_FAIL))
								{
									SendMsg(&msg);
								}

								LOGDEBUG("[MsgLogin::Process] pItemTypeManager == NULL id=[%d]", nItemId);
								return PACKET_EXE_CONTINUE;
							}
						}
						else	//pItemData == NULL
						{
							//同步前端消息
							CMsgAction msg;
							IF_OK(msg.CreateMsg(_ACTION_LIMIT_TIME_CONTINUE_TRY_FAIL))
							{
								SendMsg(&msg);
							}

							LOGDEBUG("[MsgLogin::Process] Not Found This ItemData pItemData == NULL id=[%d]", nItemId);
							return PACKET_EXE_CONTINUE;
						}
					}
					else
					{
						//同步前端消息
						CMsgAction msg;
						IF_OK(msg.CreateMsg(_ACTION_LIMIT_TIME_CONTINUE_TRY_FAIL))
						{
							SendMsg(&msg);
						}

						LOGDEBUG("[MsgLogin::Process] pItemDataManager == NULL id=[%d]", nItemId);
						return PACKET_EXE_CONTINUE;
					}
				}
				else
				{
					//同步前端消息
					CMsgAction msg;
					IF_OK(msg.CreateMsg(_ACTION_LIMIT_TIME_CONTINUE_TRY_FAIL))
					{
						SendMsg(&msg);
					}

					LOGDEBUG("[MsgLogin::Process] Param Value Is Error Continue Try id=[%d]", nItemId);
					return PACKET_EXE_CONTINUE;
				}
			}
			break;			

			///////////////////////////////////////////////////////////////////////////
			///添加到小程序
			case _ACTION_ADD_APP_LIST:
			{
				//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 添加到微信小程序<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
				if (!m_pPlayer->TriggerDayTaskDetail(PERMANENT_TASK_DETAIL_TYPE_1))
				{
					LOGERROR("[CMsgSingUp::Process] TriggerDayTaskDetail Is Error");
					CMsgAction msg;
					IF_OK(msg.CreateMsg(_ACTION_ADD_APP_LIST_FAIL))
					{
						SendMsg(&msg);
					}
					return PACKET_EXE_CONTINUE;
				}
				//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 添加到微信小程序<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//

				CMsgAction msg;
				IF_OK(msg.CreateMsg(_ACTION_ADD_APP_LIST_SUC))
				{
					SendMsg(&msg);
				}
				return PACKET_EXE_CONTINUE;
			}
			break;			
		}
	}

	return PACKET_EXE_CONTINUE;
	DEBUG_CATCH0("[CMsgAction::Process] Is Catch...");
}