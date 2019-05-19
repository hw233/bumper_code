#include "../Base/logger.h"
#include "../Main/ServerManager.h"
#include "AllMsg.h"


//////////////////////////////////////////////////// CMsgItem ////////////////////////////////////////////////////
//构造函数
CMsgItem::CMsgItem() 
{ 
}

//析构函数
CMsgItem::~CMsgItem() 
{ 	
}

//读取数据
BOOL CMsgItem::Read(SocketInputStream& iStream, UINT nLen)
{
	CNetMsg::Read(iStream, nLen);
	return TRUE;
}

BOOL CMsgItem::Write(SocketOutputStream& oStream) const
{
	return TRUE;
}
//创建消息
BOOL CMsgItem::CreateMsg(UINT nAction, UINT nItemId/*=0*/, UINT nItemType/*=0*/, UINT nSorce/*=0*/, INT nPointX/*=0*/, INT nPointY/*=0*/, INT nTargetId/*=0*/, INT nTag/*=0*/)
{
	//战斗积分同步//复活包
	if (m_pMsg)
	{
		cJSON_AddNumberToObject(m_pMsg, "action", nAction);
		//大乱斗创建道具
		if (nAction == _ACT_ITEM_CREATE_RANDOM_ITEM)
		{
			cJSON_AddNumberToObject(m_pMsg, "item_id", nItemId);
			cJSON_AddNumberToObject(m_pMsg, "item_type", nItemType);
			cJSON_AddNumberToObject(m_pMsg, "score", nSorce);
			cJSON_AddNumberToObject(m_pMsg, "point_x", nPointX);
			cJSON_AddNumberToObject(m_pMsg, "point_y", nPointY);
			cJSON_AddNumberToObject(m_pMsg, "target_id", nTargetId);	
			cJSON_AddNumberToObject(m_pMsg, "tag", nTag);				
		}
		//删除道具	C->S S-C
		if (nAction == _ACT_ITEM_PICKUP_RANDOM_ITEM)
		{
			cJSON_AddNumberToObject(m_pMsg, "item_id", nItemId);
			cJSON_AddNumberToObject(m_pMsg, "item_type", nItemType);
			cJSON_AddNumberToObject(m_pMsg, "score", nSorce);
			cJSON_AddNumberToObject(m_pMsg, "point_x", nPointX);
			cJSON_AddNumberToObject(m_pMsg, "point_y", nPointY);
			cJSON_AddNumberToObject(m_pMsg, "target_id", nTargetId);	
			cJSON_AddNumberToObject(m_pMsg, "tag", nTag);				
		}
		//添加道具
		else if (nAction == _ACT_ITEM_ADD)
		{
			cJSON_AddNumberToObject(m_pMsg, "item_id", nItemId);
			cJSON_AddNumberToObject(m_pMsg, "item_type", nItemType);
			cJSON_AddNumberToObject(m_pMsg, "type", nSorce);
			cJSON_AddNumberToObject(m_pMsg, "lift_time", nPointX);
			cJSON_AddNumberToObject(m_pMsg, "amount", nPointY);
		}
		//删除道具--服务器主动删除道具
		else if (nAction == _ACT_ITEM_DEL)
		{
			cJSON_AddNumberToObject(m_pMsg, "item_id", nItemId);
		}
		//删除道具失败
		else if (nAction == _ACT_ITEM_DEL_FAIL)
		{
			//cJSON_AddNumberToObject(m_pMsg, "item_id", nItemId);
		}
		

		m_pData = cJSON_Print(m_pMsg);
		string strMsg(m_pData);
	    size_t nLen = strMsg.size();
		LOGDEBUG("[CMsgItem::CreateMsg]:\n m_pData=[%s] nLen=[%d]\n", m_pData, nLen);
	}	

	return TRUE;
}

//创建消息
BOOL CMsgItem::CreateMsg(UINT nAction, CItemData* pItem)
{
	CHECKF(pItem);

	if (m_pMsg)
	{
		cJSON_AddNumberToObject(m_pMsg, "action", nAction);
		//添加道具
		if (nAction == _ACT_ITEM_ADD)
		{
			cJSON_AddNumberToObject(m_pMsg, "item_id", pItem->GetId());
			cJSON_AddNumberToObject(m_pMsg, "item_type", pItem->GetItemType());			
			cJSON_AddNumberToObject(m_pMsg, "type", pItem->GetType());
			cJSON_AddNumberToObject(m_pMsg, "lift_time", pItem->GetLifeTime());
			cJSON_AddNumberToObject(m_pMsg, "amount", pItem->GetAmount());
		}
		//删除道具
		else if (nAction == _ACT_ITEM_DEL)
		{
			cJSON_AddNumberToObject(m_pMsg, "item_id", pItem->GetId());
		}

		m_pData = cJSON_Print(m_pMsg);
		string strMsg(m_pData);
	    INT nLen = strMsg.size();
		LOGDEBUG("[CMsgItem::CreateMsg]:\n m_pData=[%s] nLen=[%d]\n", m_pData, nLen);
	}	

	return TRUE;
}

//发送json数据
BOOL CMsgItem::SendMsg(CNetMsg* pMsg)
{
	m_pPlayer->SendMsgToClient(pMsg);
	return TRUE;	
}

//处理前端发送过来的login消息
UINT CMsgItem::Process(VOID *pInfo)
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
			//LOGDEBUG("[CMsgItem::Process] PlayerId=[%d] pAction=[%d] pPlayer=[%p]", m_pPlayer->GetUid(), pAction->valueint, m_pPlayer);
			switch(pAction->valueint)
			{
				//////////////////////////////////////////////////////////////////////////////////////////
				///生成随机道具
				case _ACT_ITEM_CREATE_RANDOM_ITEM_POS:
				{
					CBattleTeam* pBt = m_pPlayer->GetBct();
					if (pBt)
					{
						cJSON* pPointX = cJSON_GetObjectItem(pJsonMsg, "point_x");
						cJSON* pPointY = cJSON_GetObjectItem(pJsonMsg, "point_y");
						cJSON* pTag = cJSON_GetObjectItem(pJsonMsg, "tag");
						CHECKF(pPointX && pPointY && pTag);
						LOGDEBUG("[CMsgItem::Process] _ACT_ITEM_CREATE_RANDOM_ITEM_POS pTag=[%d]", pTag->valueint);

						//生成对战道具并广播给队伍成员
						ItemInfo iteminfo;
						if(pBt->CreateFightItem(iteminfo))
						{
							CMsgItem msg;
							if(msg.CreateMsg(_ACT_ITEM_CREATE_RANDOM_ITEM, iteminfo.nId, iteminfo.nItemType, iteminfo.nSorce, pPointX->valueint, pPointY->valueint, 0, pTag->valueint))
							{
								pBt->BroadcastTeamMsg(&msg);
							}	
						}					
					}					
				}
				break;
				//////////////////////////////////////////////////////////////////////////////////////////
				///拾取随机道具
				case _ACT_ITEM_PICKUP_RANDOM_ITEM:
				{
					CBattleTeam* pBt = m_pPlayer->GetBct();
					if (pBt)
					{
						cJSON* pItemId = cJSON_GetObjectItem(pJsonMsg, "item_id");						
						CHECKF(pItemId);

						//拾取对战道具并广播给队伍成员
						if(m_pPlayer->PickUpFightItem(pItemId->valueint))
						{
							//同步拾取信息包
							CMsgItem msg1;
							if(msg1.CreateMsg(_ACT_ITEM_PICKUP_RANDOM_ITEM, pItemId->valueint, 0, 0, 0, 0, m_pPlayer->GetUid(), 0))
							{
								pBt->BroadcastTeamMsg(&msg1);
							}

							//队伍广播积分信息包
							//更新被击杀者数据：击杀数、击杀积分、击杀等级
							UINT nFightKillSum = m_pPlayer->GetFightKillSum();
							UINT nFightScore = m_pPlayer->GetFightScore();
							UINT nFightLevel = m_pPlayer->GetFightLevel();

							//LOGDEBUG("[CMsgItem::Process] nFightKillSum=[%d] nFightScore=[%d] nFightLevel=[%d]", nFightKillSum, nFightScore, nFightLevel);

							//更新霸主玩家
							pBt->UpdateKingPlayer();
							INT nKingID = pBt->GetKingPlayer()->GetUid();						

							///////////////////////////////////////////////////////////////////////
				            ///;广播同步帧缓存66ms 0:不缓存 1:缓存
				            if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 1)
				            {
								//插入服务器消息队列缓存
								pBt->InsertFrameMsgCache(_BATTLE_MODEL_CONFUSION, _WALK_ACTION_CONFUSION_CAR_LEV_CHANGE, m_pPlayer->GetUid(), nFightLevel, 0, 0, 0, 0, 0, 0, 0, 0, 0);
							}
							//else
							{	
								//广播积分相关数据
								CMsgBattle msg2;
								if (msg2.CreateMsg(_BATTLE_ACTION_BATTLE_SYN_SCORE, _BATTLE_MODEL_CONFUSION, 0, m_pPlayer->GetUid(), 
									0, 0, 0, nFightKillSum, nFightScore, nFightLevel, nKingID, 0, "", ""))
								{
									pBt->BroadcastTeamMsg(&msg2);
								}
							}
						}
					}
				}
				break;
				///////////////////////////////////////////////////////////////////////////
				// 删除道具
				case _ACT_ITEM_DEL:
				{
					cJSON* pItemId = cJSON_GetObjectItem(pJsonMsg, "item_id");					//传递道具唯一id		
					CHECKF(pItemId);

					UINT nItemId = pItemId->valueint;
					if (nItemId > 0)		//继续试用流程
					{
						//玩家道具管理器
						CItemDataManager* pItemDataManager = m_pPlayer->QueryItemDataManager();
						if (pItemDataManager)
						{
							//删除的是使用中的车辆
							UINT nNewId = 0;
							UINT nUseCarID =  m_pPlayer->GetUseCar();
							if (nUseCarID == nItemId)
							{
								CItemData* pItemData = pItemDataManager->QueryItemTypeData(100010);
								if (pItemData)
								{
									nNewId = pItemData->GetId();
									m_pPlayer->SetUseCar(nNewId);
								}
								else
								{
									nNewId = 0;
								}

								//同步当前使用中的默认车辆信息
			                    CMsgBumperTransfer msg;
			                    map<string, string> mss;
			                    IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_USE_CAR_SUC, mss, nNewId))
			                    {
			                        SendMsg(&msg);
			                    }
							}

		                    //删除的是使用中的拖尾
							UINT nUseTailID =  m_pPlayer->GetTail();
							if (nUseTailID == nItemId)
							{
								CItemData* pItemData = pItemDataManager->QueryItemTypeData(110010);

								if (pItemData)
								{
									nNewId = pItemData->GetId();
									m_pPlayer->SetTail(nNewId);
								}
								else
								{
									nNewId = 0;
								}
								
								//同步当前使用中的默认车辆信息
			                    CMsgBumperTransfer msg;
			                    map<string, string> mss;
			                    IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_TAIL_SUC, mss, nNewId))
			                    {
			                        SendMsg(&msg);
			                    }
							}

							//查找到该道具
							if(pItemDataManager->DelItem(nItemId))					//删除成功
							{
								//同步前端信息
								// CMsgItem msg;
								// IF_OK(msg.CreateMsg(_ACT_ITEM_DEL))
								// {
								// 	SendMsg(&msg);
								// }
							}
							else				//删除失败
							{
								//同步前端信息
								CMsgItem msg;
								IF_OK(msg.CreateMsg(_ACT_ITEM_DEL_FAIL))
								{
									SendMsg(&msg);
								}
							}
						}
						else	//pItemDataManager == NULL
						{
							//同步前端消息
							CMsgItem msg;
							IF_OK(msg.CreateMsg(_ACT_ITEM_DEL_FAIL))
							{
								SendMsg(&msg);
							}

							LOGDEBUG("[MsgLogin::Process] pItemDataManager == NULL id=[%d]", nItemId);
							return PACKET_EXE_CONTINUE;
						}
					}
					else	//nItemId == 0
					{
						//同步前端消息
						CMsgItem msg;
						IF_OK(msg.CreateMsg(_ACT_ITEM_DEL_FAIL))
						{
							SendMsg(&msg);
						}

						LOGDEBUG("[MsgLogin::Process] Param Value Is Error Continue Try id=[%d]", nItemId);
						return PACKET_EXE_CONTINUE;
					}
				}
				break;
				
				default:
				{
					LOGDEBUG("[CMsgItem::Process] Is Error pAction=[%d] pPlayer=[%p]", pAction->valueint, m_pPlayer);
				}
				break;
			}
		}
	}

	return PACKET_EXE_CONTINUE;
	DEBUG_CATCH0("[CMsgItem::Process] Is Catch...");
}