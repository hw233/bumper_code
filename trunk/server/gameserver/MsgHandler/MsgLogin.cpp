#include "MsgLogin.h"
#include "MsgUserInfo.h"

#include "../Base/logger.h"
#include "../Main/ServerManager.h"
#include "MsgFriendInfo.h"
#include "../Main/LimitAward/LimitTimeAwardCfg.h"
#include "../Main/LimitAward/LimitTimeAwardCfgManager.h"
#include "../Main/Item/ItemType.h"
#include "../Main/Item/ItemTypeManager.h"


//////////////////////////////////////////////////// CMsgLogin ////////////////////////////////////////////////////
//构造函数
CMsgLogin::CMsgLogin() 
{ 
}

//析构函数
CMsgLogin::~CMsgLogin() 
{ 	
}

//读取数据
BOOL CMsgLogin::Read(SocketInputStream& iStream, UINT nLen)
{
	CNetMsg::Read(iStream, nLen);
	return TRUE;
}

BOOL CMsgLogin::Write(SocketOutputStream& oStream) const
{
	return TRUE;
}

//创建消息
BOOL CMsgLogin::CreateMsg(UINT nRetStatus, UINT nAction)
{
	if (m_pMsg)
	{
		if (nRetStatus == FAIL)
		{
			cJSON_AddNumberToObject(m_pMsg, "result", nRetStatus);
		}	

		m_pData = cJSON_Print(m_pMsg);
		string strMsg(m_pData);
	    size_t nLen = strMsg.size();
		LOGDEBUG("[CMsgLogin::CreateMsg]:\n m_pData=[%s] nLen=[%d]\n", m_pData, nLen);	
	}	
	
	return TRUE;
}

//发送json数据
BOOL CMsgLogin::SendMsg(CNetMsg* pMsg)
{
	m_pPlayer->SendMsgToClient(pMsg);
	return TRUE;	
}

//处理前端发送过来的login消息
UINT CMsgLogin::Process(VOID *pInfo)
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
		cJSON* pUid = cJSON_GetObjectItem(pJsonMsg, "uid");							//string 用户id
		cJSON* pHeadPicurl = cJSON_GetObjectItem(pJsonMsg, "head_picurl");			//string 头像
		cJSON* pNickName = cJSON_GetObjectItem(pJsonMsg, "nick_name");				//string 昵称	
		
		if (pUid)			
		{
			LOGDEBUG("[CMsgLogin::Process] pUid=[%d] PlayerID=[%d] pPlayer=[%p]", 
				pUid->valueint, m_pPlayer->m_PID, m_pPlayer);
		
			CUserData* pUserData = m_pPlayer->QueryData();
			if (pUserData)
			{
				if (pUid->valueint == 0)
				{
					LOGERROR("[CMsgLogin::Process] pUid->valueint==0 pUid=[%d] PlayerID=[%d] pPlayer=[%p]", pUid->valueint, m_pPlayer->m_PID, m_pPlayer);
					return PACKET_EXE_ERROR;
				}

				//设置玩家游戏进度
				m_pPlayer->SetGameProcess(_GAME_PROCESS_LOGIN);

				//设置玩家ID
				pUserData->SetUid(pUid->valueint);
				//pUserData->SetHeadPicurl(pHeadPicurl->valuestring);
				//pUserData->SetNickName(pNickName->valuestring);
				
				//从数据库读取用户数据
                pUserData->LoadInfo(pUserData->GetUid());

                //↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓↓
                //********注意******其他玩家数据在下面添加		
				
				//玩家登录的时候检查每周任务，不是同一周则进行掩码位重置
				if (m_pPlayer->GetCurWeekNum() != (UInt32)WeekNum()) 
			    {
			        //更新掩码位
			        m_pPlayer->ResetWeekTaskMask(TRUE, FALSE);
			    }

				//!alter by huyf 2019.01.02:优化不再操作数据库,玩家在线过凌晨的话不会自动更新,不修改了就这样吧
                if(pUserData->GetNickName() != "default_initial")
                {
                    string currentDay = getCurrentYearMonDay();
                    if(pUserData->GetLastDay() != currentDay)
                    {
                        //json:   data{ 'last_day': currentDay, 'keep_star': 0, 'get_gift': 0 }
                        // string last_day = currentDay;
                        // int keep_star = 0;
                        // int get_gift = 0;

                        // char szSql[512];
                        // memset(szSql, 0, sizeof(szSql)/sizeof(*szSql));
                        // sprintf(szSql, "UPDATE g_bumper_info SET \`last_day\`=\"%s\", keep_star=%d, get_gift=%d where user_id=%d",currentDay.c_str(), keep_star, get_gift, pUserData->GetUid());
                        // LOGDEBUG("sql statement szSql: %s\n", szSql);
                        // Database()->UpdateRecord(szSql);

                        m_pPlayer->SetLastDay(currentDay);
                        m_pPlayer->SetKeepStar(0);
                        m_pPlayer->SetGetGift(0);
                    }
                 }     
                 //!alter end:优化不再操作数据库,玩家在线过凌晨的话不会自动更新,不修改了就这样吧
                
                //add by huyf 2018.11.24:添加好友系统
                {
					pUserData->SetGameStatus(_FRIEND_STATUS_ONLINE);

	                m_pPlayer->QueryFriendManager().SetOwner(m_pPlayer);
	                //登录时候加载好友数据
	                m_pPlayer->QueryFriendManager().LoadInfo();
			        //更新好友列表为上线状态
			        CMsgFriendInfo msg;       
			        if(msg.CreateMsg(_FRIEND_INFO_ONLINE, m_pPlayer, _FRIEND_STATUS_ONLINE))
			        {
			            m_pPlayer->QueryFriendManager().BroadcastFriendSMsg(&msg, m_pPlayer);
			        }
		        } 
		        //add end:添加好友系统
		        
		        //add by huyf 2018.12.28:添加道具系统
		        CItemDataManager* pItemDataManager = m_pPlayer->QueryItemDataManager();
		        if (pItemDataManager)
		        {
		        	pItemDataManager->SetOwner(m_pPlayer);
		        	pItemDataManager->LoadInfo();

		        	//下发道具信息
		        	pItemDataManager->GetItemDataListInfo();

		        	//!alter by huyf 2019.01.02:设置限时免费相关
					if(m_pPlayer->GetShowItemType() == 0 && m_pPlayer->GetShowRemainTs() == 0)
					{
						CItemTypeManager* pItemTypeManager = QueryItemTypeManager();
						if (pItemTypeManager)
						{
							CItemType* pItemType = pItemTypeManager->QueryNoExistItemType(m_pPlayer, ITEM_TYPE_CAR_FACE);
							if (pItemType)
							{
								//查到没有的类型道具
								m_pPlayer->SetShowItemType(pItemType->GetItemType());
								//设置倒计时 2小时
								UINT nLifeTime = 3600*2;
								m_pPlayer->SetShowRemainTs(nLifeTime);
								//启动时钟计时器
								m_pPlayer->QueryLimitTimeInterval().Startup(nLifeTime);
							}
						}
						else
						{
							LOGERROR("[CMsgLogin::Process] pItemTypeManager == NULL");
						}						
					}
					else if (m_pPlayer->GetShowItemType() > 0 && m_pPlayer->GetShowRemainTs() > 0)
					{
						UINT nLifeTime = m_pPlayer->GetShowRemainTs();
						//启动时钟计时器
						m_pPlayer->QueryLimitTimeInterval().Startup(nLifeTime);
					}
					//!alter end:设置限时免费相关	

		        	//add by huyf 2019.01.07:现存解锁车辆和当前使用车辆扩展改造
			        //*******注意：确保车辆的编号和玩家道具自增量ID出现重叠，item表从1000开始进行自增量计算，玩家现存1~14个车辆编号**********
			        //懒得弄配置了，先这样吧。
			        //金币做成道具形式修改
			        //判断玩家是否转换过 --- 金币----道具
			        if (m_pPlayer->GetIsGoldTransformItem() == 0)				// 0 : 未转换 1 : 已经转换
			        {
				        //当前使用的车辆
				        UINT nItemType = 0;
				        CItemData* pItemData = NULL;
				        UINT nUseCar = m_pPlayer->GetUseCar();
				        if (nUseCar < 1000)
				        {
				        	nItemType = 100000 + nUseCar * 10;
				        	pItemData = pItemDataManager->AwardItem(nItemType);
				        	if (pItemData)
				        	{
				        		m_pPlayer->SetUseCar(pItemData->GetId());
				        	}
				        	else 	//pItemData == NULL
				        	{
				        		LOGERROR("[CMsgLogin::Process] AwardItem Is Error ItemType=[%d]", nItemType);
				        	}				        	
				        }

				        //已经解锁车辆
					    m_pPlayer->RemakeUnlockCar();

				        //设置默认拖尾道具
				        nItemType = 110010;
				        pItemData = pItemDataManager->AwardItem(nItemType);
			        	if (pItemData)
			        	{
			        		m_pPlayer->SetTail(pItemData->GetId());
			        		LOGDEBUG("[CMsgLogin::Process] AwardItem Is Sue ItemType=[%d]", nItemType);
			        	}
			        	else 	//pItemData == NULL
			        	{
			        		LOGERROR("[CMsgLogin::Process] AwardItem Is Error ItemType=[%d]", nItemType);
			        	}

			        	//新增默认车辆
			        	nItemType = 100010;
				        pItemData = pItemDataManager->AwardItem(nItemType);
			        	if (pItemData)
			        	{
			        		LOGDEBUG("[CMsgLogin::Process] AwardItem Is Sue ItemType=[%d]", nItemType);
			        	}
			        	else 	//pItemData == NULL
			        	{
			        		LOGERROR("[CMsgLogin::Process] AwardItem Is Error ItemType=[%d]", nItemType);
			        	}
			        
			        	UINT nGoldAmount = m_pPlayer->GetGoldSum(FALSE);
			        	if (nGoldAmount > 0)
			        	{
			        		//金币130010 碎片140010
			        		UINT nItemType = 130010;
				        	CItemData* pItemData = pItemDataManager->AwardItem(nItemType, nGoldAmount);
				        	if (pItemData)
				        	{
				        		m_pPlayer->SetIsGoldTransformItem(1);
				        	}
				        	else 	//pItemData == NULL
				        	{
				        		LOGERROR("[CMsgLogin::Process] AwardItem Is Error ItemType=[%d]", nItemType);
				        	}
			        	}
			        }
			        //add end:现存解锁车辆和当前使用车辆扩展改造
		        }
		        else
		        {
		        	LOGERROR("[CMsgLogin::Process] pItemDataManager=[NULL]");
		        }
		        //add end::添加道具系统
		        //add by huyf 2018.12.28:添加任务系统
		        CTaskDetailManager* pTaskDetailManager = m_pPlayer->QueryTaskDetailManager();
		        if (pTaskDetailManager)
		        {
		        	pTaskDetailManager->SetOwner(m_pPlayer);
		        	pTaskDetailManager->LoadInfo();
		        }
		        else
		        {
		        	LOGERROR("[CMsgLogin::Process] pTaskDetailManager=[NULL]");
		        }
		        //add end::添加任务系统
		        
		        //add by huyf 2019.01.09:玩家登录的时候检查每日任务，不是同一天则进行掩码位重置
				if (pUserData->GetLastResetTaskTime() != (UInt32)DateStamp())
				{
					m_pPlayer->ResetDayTaskMask(TRUE, FALSE);

					//设置新的-当前星期[0-6]:星期天-星期六
					/*int tm_wday;  星期 – 取值区间为[0,6]，其中0代表星期天，1代表星期一，以此类推 */
					time_t tInput = time(NULL);
					tm * pTm = localtime(&tInput);
					if (pTm)
					{
						m_pPlayer->SetCurTmWday(pTm->tm_wday);
					}

					//!alter by huyf 2019.01.02:在线领奖
					//!不处于同一天的时候，直接更新到第一个index
					m_pPlayer->SetOnLineAward(1, FALSE);
					//!alter end:在线领奖
				}
				else 			//同一天登录则直接启动计时器
				{
					//启动计时器
				    if (m_pPlayer->GetOnLineItemType() > 0 && m_pPlayer->GetOnLineRemainTs() > 0)
				    {
				        UINT nLifeTime = m_pPlayer->GetOnLineRemainTs();
				        //启动时钟计时器
				        m_pPlayer->QueryOnLineInterval().Startup(nLifeTime);
				    }
				}
		        //add end:玩家登录的时候检查每日任务，不是同一天则进行掩码位重置

                if(pHeadPicurl != NULL && pHeadPicurl->valuestring != 0x0)
				{
					pUserData->SetHeadPicurl(pHeadPicurl->valuestring);	
					LOGDEBUG("[CMsgLogin::Process] pUid=[%d] pHeadPicurl=[%s]", pUid->valueint, pHeadPicurl->valuestring);		
				}
				if(pNickName != NULL && pNickName->valuestring != 0x0)
				{
					pUserData->SetNickName(pNickName->valuestring);
					LOGDEBUG("[CMsgLogin::Process] pUid=[%d] pNickName=[%s]", pUid->valueint, pNickName->valuestring);
				}
                
				//向客户端发送玩家数据包
				CMsgUserInfo msg1;
				if (msg1.CreateMsg(m_pPlayer))
				{
					SendMsg(&msg1);
				}
				LOGDEBUG("[CMsgLogin::Process] Is Dealed Finish And SendMsg(CMsgUserInfo) To Client Is Suc");
			}
			else	//pUserData == NULL
			{
				//向客户端发送玩家数据包
				CMsgLogin msg;
				if (msg.CreateMsg(FAIL, 0))
				{
					SendMsg(&msg);
				}
				LOGERROR("[CMsgLogin::Process] pUserData = NULL");
			}			
		}
		else
		{
			//向客户端发送玩家数据包
			CMsgLogin msg;
			if (msg.CreateMsg(FAIL,0))
			{
				SendMsg(&msg);
			}
			LOGERROR("[CMsgLogin::Process] 参数错误");
		}
	}

	return PACKET_EXE_CONTINUE;
	DEBUG_CATCH0("[CMsgLogin::Process] Is Catch...");
}
