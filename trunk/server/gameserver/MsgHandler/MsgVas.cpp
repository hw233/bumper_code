#include "MsgVas.h"

#include "../Base/logger.h"
#include "../Main/ServerManager.h"
#include "MsgUserInfo.h"
#include "../Main/Battle/BattleCfg.h"


//////////////////////////////////////////////////// CMsgWalk ////////////////////////////////////////////////////
//构造函数
CMsgVas::CMsgVas() 
{ 
}

//析构函数
CMsgVas::~CMsgVas() 
{ 	
}

//读取数据
BOOL CMsgVas::Read(SocketInputStream& iStream, UINT nLen)
{
	CNetMsg::Read(iStream, nLen);
	return TRUE;
}

BOOL CMsgVas::Write(SocketOutputStream& oStream) const
{
	return TRUE;
}
//创建消息
BOOL CMsgVas::CreateMsg(INT nAction/*=0*/, INT nType, INT nItemId, INT nItemType, INT nItemCount)
{
	if (m_pMsg)
	{
		cJSON_AddNumberToObject(m_pMsg, "action", nAction);						//atcion :_VAS_BUY
		////买道具成功---战斗BUFF
		if (nAction == _VAS_BUY_SUC)
		{
			cJSON_AddNumberToObject(m_pMsg, "type", nType);							//type:_VAS_BUY_TYPE_GOLD
			cJSON_AddNumberToObject(m_pMsg, "item_id", nItemId);					//道具的id : 0
			cJSON_AddNumberToObject(m_pMsg, "item_type", nItemType);				//1101110 int 物品itmetype
			cJSON_AddNumberToObject(m_pMsg, "item_count", nItemCount);				//1 int 数量 : 10
		}
		//买道具失败---战斗BUFF
		else if (nAction == _VAS_BUY_FAIL)
		{
			
		}	
		//商城买道具成功---商城
		else if (_VAS_SHOP_BUY_SUC)
		{
			cJSON_AddNumberToObject(m_pMsg, "item_type", nType);				//1101110 int 物品itmetype
			cJSON_AddNumberToObject(m_pMsg, "lift_time", nItemId);				
			cJSON_AddNumberToObject(m_pMsg, "view", nItemType);
		}	
		////商城买道具失败---商城			
		else if (_VAS_SHOP_BUY_FAIL)
		{
			cJSON_AddNumberToObject(m_pMsg, "item_type", nType);				//1101110 int 物品itmetype
		}

		m_pData = cJSON_Print(m_pMsg);
		string strMsg(m_pData);
		size_t nLen = strMsg.size();
		LOGDEBUG("[CMsgVas::CreateMsg]:\n m_pData=[%s] nLen=[%d]\n", m_pData, nLen);
	}
	return TRUE;
}

//发送json数据
BOOL CMsgVas::SendMsg(CNetMsg* pMsg)
{
	m_pPlayer->SendMsgToClient(pMsg);
	return TRUE;	
}

//处理前端发送过来的消息
UINT CMsgVas::Process(VOID *pInfo)
{	
	LOGDEBUG("[CMsgVas::Process] begin");
	DEBUG_TRY
	//设置需要回包的玩家
	m_pPlayer = static_cast<Player*>(pInfo);
	CHECKF(m_pPlayer);	

	//处理前端发过来的消息
	m_pMsg = cJSON_Parse(this->m_pData);
	cJSON* pJsonMsg = m_pMsg;	
	if(pJsonMsg)
	{
		cJSON* pAction = cJSON_GetObjectItem(pJsonMsg, "action");							//_VAS_BUY			
		CHECKF(pAction);
		LOGDEBUG("[CMsgVas::Process] PlayerId=[%d] pAction=[%d] pPlayer=[%p]", m_pPlayer->GetUid(), pAction->valueint, m_pPlayer);

		switch(pAction->valueint)
		{
			///////////////////////////////////////////////////////////////////////////
			///买道具-------战斗BUFF
			case _VAS_BUY:
			{
				cJSON* pType = cJSON_GetObjectItem(pJsonMsg, "type");								//购买类型
				cJSON* pItemId = cJSON_GetObjectItem(pJsonMsg, "item_id");							//11 int 物品id
				cJSON* pItemType = cJSON_GetObjectItem(pJsonMsg, "item_type");						//110110 int 物品itmetype
				cJSON* pItemCount = cJSON_GetObjectItem(pJsonMsg, "item_count");					//道具数量
				CHECKF(pItemId && pItemType && pItemCount && pType);
				//金币买道具
				if (pType->valueint == _VAS_BUY_TYPE_GOLD)
				{
					//检查金币是否充足
					INT nGoldSum = 5000;//m_pPlayer->GetGoldSum();
					INT nCfgGoldCount = 500;							//需要的金币数量500
					if (nGoldSum > nCfgGoldCount)	//金币够，购买成功
					{
						INT nLeftGoldSum = nGoldSum - nCfgGoldCount;
						m_pPlayer->SetGoldSum(nLeftGoldSum);
						//发奖励
						//m_pPlayer->SetAddGold(1);					//战斗结束后奖励的金币，金币加成200%
						m_pPlayer->SetAddScore(3000);				//积分基础加:3000
						//chushihuawanjiashuju
						//m_pPlayer->SetFightKillSum(0);
						//m_pPlayer->SetFightScore(1000 + m_pPlayer->GetAddScore());
						//INT nCfgLevel = m_pPlayer->GetFightCfgLevel(m_pPlayer->GetFightScore());
						//m_pPlayer->SetFightLevel(nCfgLevel);

						//同步前端数据包
						CMsgVas msg1;
						if(msg1.CreateMsg(_VAS_BUY_SUC, _VAS_BUY_TYPE_GOLD, pItemId->valueint, pItemType->valueint, pItemCount->valueint))
						{
							m_pPlayer->SendMsgToClient(&msg1);
						}

						//同步玩家属性
						CMsgUserInfo msg2;
						if(msg2.CreateMsg(m_pPlayer))
						{
							SendMsg(&msg2);
						}
					}
					else	//金币不够,购买失败
					{
						//同步前端数据包
						CMsgVas msg;
						if(msg.CreateMsg(_VAS_BUY_FAIL, 0, 0, 0, 0))
						{
							m_pPlayer->SendMsgToClient(&msg);
						}
					}
				}
				//视频买道具：************有前端自己来判定，服务器此处没有做有效性检查*********
				else if(pType->valueint == _VAS_BUY_TYPE_VIWE)
				{
						//发奖励
						m_pPlayer->SetAddGold(BattleCfg()->GetBccAwardStartViewRatio());				//金币加成300%
						//m_pPlayer->SetAddScore(3000);			//积分基础加:3000	

						//同步前端数据包
						CMsgVas msg1;
						if(msg1.CreateMsg(_VAS_BUY_SUC, _VAS_BUY_TYPE_VIWE, pItemId->valueint, pItemType->valueint, pItemCount->valueint))
						{
							m_pPlayer->SendMsgToClient(&msg1);
						}

						//同步玩家属性
						CMsgUserInfo msg2;
						if(msg2.CreateMsg(m_pPlayer))
						{
							SendMsg(&msg2);
						}
				}
				else
				{
					CMsgVas msg1;
					if(msg1.CreateMsg(_VAS_BUY_FAIL, _VAS_BUY_TYPE_VIWE))
					{
						m_pPlayer->SendMsgToClient(&msg1);
					}
				}
			}
			break;	
			///////////////////////////////////////////////////////////////////////////
			/////商城买道具-------商城
			case _VAS_SHOP_BUY:
			{
				cJSON* pJsItemType = cJSON_GetObjectItem(pJsonMsg, "item_type");						//110110 int 物品itmetype
				CHECKF(pJsItemType);

				UINT nItemType = pJsItemType->valueint;
				if (nItemType > 0)
				{
					//查询奖励到具是否配置
				    CItemType* pItemType = NULL;
				    if (QueryItemTypeManager())
				    {
				        pItemType = QueryItemTypeManager()->QueryItemType(nItemType);
				        if(!pItemType)
				        {
				        	CMsgVas msg;
				        	IF_OK(msg.CreateMsg(_VAS_SHOP_BUY_FAIL, nItemType))
				        	{
				        		SendMsg(&msg);
				        	}		

				            LOGERROR("[CMsgVas::Process] QueryItemType Is Error 数据库item_type表中未查询到该道具的配置 nItemType=[%d]", nItemType);
				            return PACKET_EXE_CONTINUE;
				        }
				    }
				    else //QueryItemTypeManager==NULL
				    {
				    	CMsgVas msg;
			        	IF_OK(msg.CreateMsg(_VAS_SHOP_BUY_FAIL, nItemType))
			        	{
			        		SendMsg(&msg);
			        	}
				        LOGERROR("[CMsgVas::Process] QueryItemTypeManager==NULL");
				        return PACKET_EXE_CONTINUE;
				    }

				    //发放奖励
					CItemDataManager* pItemDataManager = m_pPlayer->QueryItemDataManager();
					if(!pItemDataManager)
					{
						CMsgVas msg;
			        	IF_OK(msg.CreateMsg(_VAS_SHOP_BUY_FAIL, nItemType))
			        	{
			        		SendMsg(&msg);
			        	}
				        LOGERROR("[CMsgVas::Process] pItemDataManager == NULL");
				        return PACKET_EXE_CONTINUE;
					}

				    /*
				    xx12345	7位制	
					前2位获取类型	后6位价格	
					xx	123456	
					1	123456	邀请获得
					2	123456	段位获得
					3	123456	金币获得-----不够的时候，可以看视频获得
					4	123456	视频获得
					5	123456	碎片获得-----不够的时候，可以看视频获得
					6	123456	试玩获得
					7	123456	活动获得
					*************类型 1 2 4 6的存在被刷道具的漏洞在，暂时不管了，策划人员就这么设计的，说没事。一群没做过大型MMO端游的孩子**************
					*/

				    //获取价格 : price
				    UINT nPriceAmount = pItemType->GetVasGold();
				    //获取所需道具类型 : itemtype
				    UINT nSpeedItemType = pItemType->GetVasItemType();//3038888
				    UINT nGetType = 0;
				    if (nPriceAmount > 0)		//配置了价格才可以进行外部购买
				    {
				    	nGetType = nPriceAmount/1000000;
				    	if (nGetType == 1)
				    	{
				    		MYSQL_ROW row;
				    		string sql = "SELECT parent_id FROM g_bumper_master WHERE parent_id=" + std::to_string(m_pPlayer->GetUid());
                            MYSQL_RES* result = Database()->Query(sql);
                            if(!(row = mysql_fetch_row(result)))
                            {
                                CMsgVas msg;
					         	IF_OK(msg.CreateMsg(_VAS_SHOP_BUY_FAIL, nItemType))
					         	{
					         		SendMsg(&msg);
					         	}
					         	
                                return PACKET_EXE_CONTINUE;
                            }   
                            mysql_free_result(result);
    						result = NULL;                         
				    	}
				    	else if (nGetType == 2)
				    	{
							INT value = 0;
							INT nId = 100030/10%10;
							string sql = "SELECT condition_id, value FROM g_bumper_cars WHERE id=" + std::to_string(nId);
							MYSQL_RES* result = Database()->Query(sql);
							MYSQL_ROW row;
							enum
							{
								_CONDITION_ID   = 0,
								_VALUE      = 1,
							};
							while(row = mysql_fetch_row(result))
							{
								value = SafeAtoi(row[_VALUE]);
							}
							mysql_free_result(result);
    						result = NULL; 

    						if(m_pPlayer->GetGrandingId() < value) 
                            {
								CMsgVas msg;
					         	IF_OK(msg.CreateMsg(_VAS_SHOP_BUY_FAIL, nItemType))
					         	{
					         		SendMsg(&msg);
					         	}

							 	LOGERROR("[CMsgVas::Process] AwardItem Is Error nItemType=[%d]", nItemType);
							 	return PACKET_EXE_CONTINUE;
                            }
				    	}

				    	//发放奖励
				     	if (nGetType == 1 || nGetType == 2 || nGetType == 4 || nGetType == 6)
				     	{
				     		//发放奖励
							if(!pItemDataManager->AwardItem(nItemType))
							{
							 	CMsgVas msg;
					         	IF_OK(msg.CreateMsg(_VAS_SHOP_BUY_FAIL, nItemType))
					         	{
					         		SendMsg(&msg);
					         	}

							 	LOGERROR("[CMsgVas::Process] AwardItem Is Error nItemType=[%d]", nItemType);
							 	return PACKET_EXE_CONTINUE;
							}
				     	}
				  		else if (nGetType == 3 || nGetType == 5)
				    	{
				    		cJSON* pView = cJSON_GetObjectItem(pJsonMsg, "view");						//110110 int 物品itmetype
							if(pView)
							{
								UINT nView = pView->valueint;
								if (nView == 1)
								{
									nItemType = nItemType/10*10+1;
									//发放奖励
									CItemData* pItemData = pItemDataManager->AwardItem(nItemType);
									if(!pItemData)
									{
									 	CMsgVas msg;
							         	IF_OK(msg.CreateMsg(_VAS_SHOP_BUY_FAIL, nItemType))
							         	{
							         		SendMsg(&msg);
							         	}

									 	LOGERROR("[CMsgVas::Process] AwardItem Is Error nItemType=[%d]", nItemType);
									 	return PACKET_EXE_CONTINUE;
									}
									else
									{
										CMsgVas msg;
							         	IF_OK(msg.CreateMsg(_VAS_SHOP_BUY_SUC, nItemType, pItemData->GetLifeTime(), nView))
							         	{
							         		SendMsg(&msg);
							         	}

									 	LOGDEBUG("[CMsgVas::Process] AwardItem Is Suc nItemType=[%d] LifeTime=[%d]", nItemType,pItemData->GetLifeTime());
									 	return PACKET_EXE_CONTINUE;
									}
								}
							}

							nPriceAmount = nPriceAmount%1000000;			//获得需要花费的道具数量
							UINT nHaveAmount = 0;							//已经拥有的数量

							CItemData* pItemData = pItemDataManager->QueryItemTypeData(nSpeedItemType);
							if (pItemData)
							{
								nHaveAmount = pItemData->GetAmount();
								if (nHaveAmount < nPriceAmount) //道具不足
								{
									CMsgVas msg;
						        	IF_OK(msg.CreateMsg(_VAS_SHOP_BUY_FAIL, nItemType))
						        	{
						        		SendMsg(&msg);
						        	}

									LOGERROR("[CMsgVas::Process] Is Error Not Found The ItemData nSpeedItemType=[%d] nPriceAmount=[%d] nHaveAmount=[%d] ", nSpeedItemType,nPriceAmount,nHaveAmount);
									return PACKET_EXE_CONTINUE;
								}
								else	//进行扣道具
								{
									if(pItemDataManager->DelItem(pItemData->GetId(), nPriceAmount))
									{
										//发放奖励
										if(!pItemDataManager->AwardItem(nItemType))
										{
											CMsgVas msg;
								        	IF_OK(msg.CreateMsg(_VAS_SHOP_BUY_FAIL, nItemType))
								        	{
								        		SendMsg(&msg);
								        	}

											LOGERROR("[CMsgVas::Process] AwardItem Is Error nItemType=[%d]", nItemType);
											return PACKET_EXE_CONTINUE;
										}
									}
									else //DelItem
									{
										CMsgVas msg;
							        	IF_OK(msg.CreateMsg(_VAS_SHOP_BUY_FAIL, nItemType))
							        	{
							        		SendMsg(&msg);
							        	}

										LOGERROR("[CMsgVas::Process] DelItem Is Error id=[%d] nPriceAmount=[%d]", pItemData->GetId(), nPriceAmount);
										return PACKET_EXE_CONTINUE;	
									}
								}
							}
							else //pItemData==NULL
							{
								CMsgVas msg;
					        	IF_OK(msg.CreateMsg(_VAS_SHOP_BUY_FAIL, nItemType))
					        	{
					        		SendMsg(&msg);
					        	}

								LOGERROR("[CMsgVas::Process] Is Error Not Found The ItemData nSpeedItemType=[%d]", nSpeedItemType);
								return PACKET_EXE_CONTINUE;
							}
				    	}
				    	else //nGetType Is Error
				    	{
				    		CMsgVas msg;
				        	IF_OK(msg.CreateMsg(_VAS_SHOP_BUY_FAIL, nItemType))
				        	{
				        		SendMsg(&msg);
				        	}
					        LOGERROR("[CMsgVas::Process] nPriceAmount==0 nItemType=[%d]", nItemType);
					        return PACKET_EXE_CONTINUE;
				    	}

				    	CMsgVas msg;
			        	IF_OK(msg.CreateMsg(_VAS_SHOP_BUY_SUC, nItemType))
			        	{
			        		SendMsg(&msg);
			        	}

						LOGERROR("[CMsgVas::Process] AwardItem Is Error nItemType=[%d]", nItemType);
						return PACKET_EXE_CONTINUE;
				    }
				    else //nPriceAmount == 0
				    {
				    	CMsgVas msg;
			        	IF_OK(msg.CreateMsg(_VAS_SHOP_BUY_FAIL, nItemType))
			        	{
			        		SendMsg(&msg);
			        	}
				        LOGERROR("[CMsgVas::Process] nPriceAmount==0 nItemType=[%d]", nItemType);
				        return PACKET_EXE_CONTINUE;
				    }	
				}
				else  //nItemType == 0
				{
					CMsgVas msg;
		        	IF_OK(msg.CreateMsg(_VAS_SHOP_BUY_FAIL, nItemType))
		        	{
		        		SendMsg(&msg);
		        	}
			        LOGERROR("[CMsgVas::Process] nPriceAmount==0 nItemType=[%d]", nItemType);
				        return PACKET_EXE_CONTINUE;
				}

			}
			break;	
		}	 
	}
	
	return PACKET_EXE_CONTINUE;
	DEBUG_CATCH0("[CMsgVas::Process] Is Catch...");
}