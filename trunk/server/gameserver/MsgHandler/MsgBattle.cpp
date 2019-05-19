#include "MsgBattle.h"

#include "../Base/logger.h"
#include "../Main/ServerManager.h"
#include "BattleManager.h"
#include "Battle.h"
#include "MsgSynRoomInfo.h"
#include "MsgWalk.h"
//class CMsgSynRoomInfo;
#include "../Main/Battle/BattleCfg.h"


//////////////////////////////////////////////////// CMsgBattle ////////////////////////////////////////////////////
//构造函数
CMsgBattle::CMsgBattle() 
{ 
}

//析构函数
CMsgBattle::~CMsgBattle() 
{ 	
}

//读取数据
BOOL CMsgBattle::Read(SocketInputStream& iStream, UINT nLen)
{
	//LOGDEBUG("[CMsgBattle::Read] Is Suc");	
	CNetMsg::Read(iStream, nLen);
	return TRUE;
}

BOOL CMsgBattle::Write(SocketOutputStream& oStream) const
{
	return TRUE;
}

//创建消息
//nPower  nPointX  nPointY 服务器与前端 扩大100倍 方便处理
BOOL CMsgBattle::CreateMsg( UINT nAction/*=0*/, UINT nBattleModel/*=0*/, UINT nTeamId/*=0*/, UINT nTargetId/*=0*/, INT nPower/*=0*/, 
							INT nPointX/*=0*/, INT nPointY/*=0*/, INT nKillSum/*=0*/, INT nFightScore/*=1000*/, INT nFightLevel/*=1*/, 
							INT nKingID/*=0*/, INT nRegionId/*=0*/, string strNickName/*=""*/, string strHeadPicurl/*=""*/)
{
	//if (nBattleModel == _BATTLE_MODEL_CONFUSION)
	{
		if (m_pMsg)
		{
			cJSON_AddNumberToObject(m_pMsg, "action", nAction);

			//快速加入
			if (nAction == _BATTLE_ACTION_QUICK_JOIN_SUC)
			{
				cJSON_AddNumberToObject(m_pMsg, "model", nBattleModel);
			}
			//创建房间成功
			else if (nAction == _BATTLE_ACTION_CREATE_ROOM_SUC)
			{
				cJSON_AddNumberToObject(m_pMsg, "model", nBattleModel);
				cJSON_AddNumberToObject(m_pMsg, "team_id", nTeamId);
			}	
			//战斗中碰撞
			else if (nAction == _BATTLE_ACTION_BATTLE_COLLIDE_SUC)
			{
				cJSON_AddNumberToObject(m_pMsg, "target_id", nTargetId);
				cJSON_AddNumberToObject(m_pMsg, "type", nTeamId);
				cJSON_AddNumberToObject(m_pMsg, "power_x", nPointX);
				cJSON_AddNumberToObject(m_pMsg, "power_y", nPointY);
			}
			//战斗中击杀
			else if (nAction == _BATTLE_ACTION_BATTLE_KILL_SUC)
			{
				cJSON_AddNumberToObject(m_pMsg, "killer_id", nTargetId);
				cJSON_AddNumberToObject(m_pMsg, "target_id", nPower);
				//cJSON_AddNumberToObject(m_pMsg, "point_x", nPointX);							//前端不需要了
				//cJSON_AddNumberToObject(m_pMsg, "point_y", nPointY);							//前端不需要了
			}
			//战斗中复活
			else if (nAction == _BATTLE_ACTION_BATTLE_RELIVE_SUC)
			{
				cJSON_AddNumberToObject(m_pMsg, "model", nBattleModel);
				cJSON_AddNumberToObject(m_pMsg, "target_id", nTargetId);				
				cJSON_AddNumberToObject(m_pMsg, "directionX", nTeamId); //[0-10]		
				cJSON_AddNumberToObject(m_pMsg, "directionY", nPower); //[0-10] 
				cJSON_AddNumberToObject(m_pMsg, "region", nPointX);
			}			
			//战斗中同步积分、击杀数、等级、霸主id
			else if (nAction == _BATTLE_ACTION_BATTLE_SYN_SCORE)
			{
				cJSON_AddNumberToObject(m_pMsg, "target_id", nTargetId);
				cJSON_AddNumberToObject(m_pMsg, "kill_sum", nKillSum);
				//cJSON_AddNumberToObject(m_pMsg, "fight_lev", nFightLevel);				//车辆等级放到缓存帧中同步
				cJSON_AddNumberToObject(m_pMsg, "fight_score", nFightScore);
				cJSON_AddNumberToObject(m_pMsg, "king_id", nKingID);
			}
			//大乱斗-战斗开始
			else if (nAction == _BATTLE_ACTION_BATTLE_START)
			{
				cJSON* cjParam = cJSON_CreateObject();
 				IF_OK(cjParam)
 				{
 					cJSON_AddNumberToObject(m_pMsg, "interval_time", nPower); 					
 					cJSON_AddNumberToObject(cjParam, "target_id", nTargetId);				
					cJSON_AddNumberToObject(cjParam, "region_id", nRegionId);				
					cJSON_AddNumberToObject(cjParam, "directionX", nPointX); //[0-10]		
					cJSON_AddNumberToObject(cjParam, "directionY", nPointY); //[0-10] 
					cJSON_AddNumberToObject(cjParam, "kill_sum", nKillSum);					
					cJSON_AddNumberToObject(cjParam, "fight_lev", nFightLevel);				
					cJSON_AddNumberToObject(cjParam, "fight_score", nFightScore);

					//用户最近玩家显示用：段位ID 和 当前星数
					cJSON_AddNumberToObject(cjParam, "granding_id", nTeamId);
					cJSON_AddNumberToObject(cjParam, "cur_star_sum", nKingID);

					cJSON_AddItemToObject(m_pMsg, "members", cjParam);						
				}				
			}
			//大乱斗-战斗结束
			else if (nAction == _BATTLE_ACTION_BATTLE_END_SUC)
			{
				LOGDEBUG("_BATTLE_ACTION_BATTLE_END_SUC");
				cJSON* cjParam = cJSON_CreateObject();
 				IF_OK(cjParam)
 				{
 					cJSON_AddNumberToObject(cjParam, "rank", 0);
 					cJSON_AddNumberToObject(cjParam, "user_id", nTargetId);
					cJSON_AddNumberToObject(cjParam, "kill_sum", nKillSum);
					cJSON_AddNumberToObject(cjParam, "fight_lev", nFightLevel);	
					cJSON_AddNumberToObject(cjParam, "fight_score", nFightScore);
				// 	cJSON_AddStringToObject(cjParam, "nick_name", "abc"/*strNickName.c_str()*/);
				//	cJSON_AddStringToObject(cjParam, "head_picurl", "url"/*strHeadPicurl.c_str()*/);

                   	//cJSON_AddStringToObject(cjParam, "nick_name",  strNickName.c_str());
					//cJSON_AddStringToObject(cjParam, "head_picurl", strHeadPicurl.c_str());

					cJSON_AddItemToObject(m_pMsg, "members", cjParam);					
				}				
			}
			//红蓝战队-战斗开始
			else if (nAction == _BATTLE_ACTION_TEAM_START_SUC)
			{
				cJSON* cjParam = cJSON_CreateObject();
 				IF_OK(cjParam)
 				{
 					cJSON_AddNumberToObject(m_pMsg, "interval_time", nPower);
 					cJSON_AddNumberToObject(cjParam, "target_id", nTargetId);				
					cJSON_AddNumberToObject(cjParam, "region_id", nRegionId);
					cJSON_AddNumberToObject(cjParam, "car_lev", nFightLevel);
					cJSON_AddNumberToObject(cjParam, "use_car", nPointX);		
					cJSON_AddNumberToObject(cjParam, "team_id", nTeamId);
					cJSON_AddNumberToObject(cjParam, "team_colour", nPointY);
//					cJSON_AddStringToObject(cjParam, "nick_name", "abc"/*strNickName.c_str()*/);
//					cJSON_AddStringToObject(cjParam, "head_picurl", "url"/*strHeadPicurl.c_str()*/);				

                    cJSON_AddStringToObject(cjParam, "nick_name", strNickName.c_str());
                    cJSON_AddStringToObject(cjParam, "head_picurl", strHeadPicurl.c_str());

                    cJSON_AddNumberToObject(cjParam, "use_motion", nKingID);

                    
					cJSON_AddItemToObject(m_pMsg, "members", cjParam);				
				}				
			}
			//红蓝战队-战斗结束
			else if (nAction == _BATTLE_ACTION_TEAM_END_SUC)
			{
				cJSON_AddNumberToObject(m_pMsg, "r_team_id", nTeamId);				
				cJSON_AddNumberToObject(m_pMsg, "r_flag", nTargetId);				
				cJSON_AddNumberToObject(m_pMsg, "r_score", nPower);	
				cJSON_AddNumberToObject(m_pMsg, "r_awardgold", nPointX);			
				cJSON_AddNumberToObject(m_pMsg, "b_team_id", nKillSum);					
				cJSON_AddNumberToObject(m_pMsg, "b_flag", nFightScore);				
				cJSON_AddNumberToObject(m_pMsg, "b_score", nFightLevel);
				cJSON_AddNumberToObject(m_pMsg, "b_awardgold", nPointY);									
			}
			//战斗结束界面 看视频三倍奖励 //战斗结束同步前端霸主令牌和金币奖励
			else if (nAction == _BATTLE_ACTION_VIEW_AWARD_SUC || nAction == _BATTLE_ACTION_END_AWARD_SUC)
			{
				cJSON_AddNumberToObject(m_pMsg, "king_sum", nTeamId);				
				cJSON_AddNumberToObject(m_pMsg, "award_gold", nPower);	
			}
			////战斗结束界面 看视频三倍奖励-[红蓝对战]
			else if (nAction == _BATTLE_ACTION_TEAM_VIEW_AWARD_SUC)
			{				
				cJSON_AddNumberToObject(m_pMsg, "award_gold", nPower);	
			}
			//红蓝对战中同步队伍积分、击杀方车辆等级
			else if (nAction == _BATTLE_ACTION_TEAM_SYN_SCORE_SUC)
			{
				cJSON_AddNumberToObject(m_pMsg, "team_id", nTeamId);
				cJSON_AddNumberToObject(m_pMsg, "score", nPower);				
				cJSON_AddNumberToObject(m_pMsg, "target_id", nTargetId);				
				//cJSON_AddNumberToObject(m_pMsg, "car_lev", nPointX);		
			}
			//战斗中复活-[红蓝战队]
			else if (nAction == _BATTLE_ACTION_TEAM_RELIVE_SUC)
			{
				cJSON_AddNumberToObject(m_pMsg, "target_id", nTargetId);
				cJSON_AddNumberToObject(m_pMsg, "region_id", nRegionId);
				cJSON_AddNumberToObject(m_pMsg, "car_lev", nTeamId);
			}
			//加载完成-[大乱斗]
            else if(nAction == _BATTLE_ACTION_LOADING_COMPLETE_SUC)
            {
                cJSON_AddNumberToObject(m_pMsg, "target_id", nTargetId);
            }
            //加载完成-[红蓝战队]
            else if(nAction == _BATTLE_ACTION_TEAM_LOADING_COMPLETE_SUC)
            {
                cJSON_AddNumberToObject(m_pMsg, "target_id", nTargetId);
            }
            //邀请好友加入-[大乱斗]-[红蓝战队]
            else if(nAction == _BATTLE_ACTION_INVITE_JOIN_FAIL)
            {
                cJSON_AddNumberToObject(m_pMsg, "model", nBattleModel);
            }
            //邀请好友加入-[大乱斗]-[红蓝战队]
            else if(nAction == _BATTLE_ACTION_INVITE_JOIN_SUC)
            {
                cJSON_AddNumberToObject(m_pMsg, "model", nBattleModel);
            }   
            //队长变更-[大乱斗]-[红蓝对战]
            else if(nAction == _BATTLE_ACTION_CHANGE_TEAM_LEADER_SUC)
            {
                cJSON_AddNumberToObject(m_pMsg, "team_id", nTeamId);
				cJSON_AddNumberToObject(m_pMsg, "leader_id", nTargetId);
            } 
            //邀请好友加入队伍-[大乱斗]-[红蓝对战]
            else if(nAction == _BATTLE_ACTION_INVITE_FRIEND_JOIN)
            {
            	cJSON_AddNumberToObject(m_pMsg, "model", nBattleModel);
                cJSON_AddNumberToObject(m_pMsg, "team_id", nTeamId);
				cJSON_AddNumberToObject(m_pMsg, "friend_id", nTargetId);
				cJSON_AddStringToObject(m_pMsg, "nick_name", strNickName.c_str());
                cJSON_AddStringToObject(m_pMsg, "head_picurl", strHeadPicurl.c_str());
            } 
            //最近玩家-[大乱斗]
			/*else if (nAction == _BATTLE_ACTION_RECENT_PLAYER)
			{
				cJSON* cjParam = cJSON_CreateObject();
 				IF_OK(cjParam)
 				{
 					cJSON_AddNumberToObject(cjParam, "uid", nTargetId);	
                    cJSON_AddStringToObject(cjParam, "nick_name", strNickName.c_str());
                    cJSON_AddStringToObject(cjParam, "head_picurl", strHeadPicurl.c_str());
                    cJSON_AddNumberToObject(cjParam, "ts", nTimeStamp);						//服务器数据包时间帧序列
                    
					cJSON_AddItemToObject(m_pMsg, "members", cjParam);			
				}				
			} */  
			//修改队伍ID-[大乱斗]-[红蓝对战]
            else if(nAction == _BATTLE_ACTION_CHANGE_TEAM_ID)
            {
                cJSON_AddNumberToObject(m_pMsg, "team_id", nTeamId);
            }  			        
		}
		
		m_pData = cJSON_Print(m_pMsg);
        string strMsg(m_pData);
        INT nLen = strMsg.size();
        LOGDEBUG("[CMsgBattle::CreateMsg]:\n m_pData=[%s] nLen=[%d]\n", m_pData, nLen);
	}	

	return TRUE;
}

//创建消息
//nPower  nPointX  nPointY 服务器与前端 扩大100倍 方便处理
BOOL CMsgBattle::AppendMsg( UINT nAction/*=0*/, UINT nBattleType/*=0*/, UINT nTeamId/*=0*/, UINT nTargetId/*=0*/, INT nPower/*=0*/, 
							INT nPointX/*=0*/, INT nPointY/*=0*/, INT nKillSum/*=0*/, INT nFightScore/*=1000*/, INT nFightLevel/*=1*/,
							INT nKingID/*=0*/, INT nRegionId/*=1*/, INT nIndex/*=0*/,string strNickName/*=""*/, string strHeadPicurl/*=""*/)
{
	//if (nBattleType == _BATTLE_MODEL_CONFUSION)
	{
		if (m_pMsg)
		{
			//大乱斗-战斗开始
			if (nAction == _BATTLE_ACTION_BATTLE_START)
			{
				cJSON* cjParam = cJSON_CreateObject();
 				IF_OK(cjParam)
 				{
 					std::string strParamKey = "members" + std::to_string(nIndex);
 					cJSON_AddNumberToObject(cjParam, "target_id", nTargetId);
					cJSON_AddNumberToObject(cjParam, "region_id", nRegionId);
					cJSON_AddNumberToObject(cjParam, "directionX", nPointX); //[0-10]
					cJSON_AddNumberToObject(cjParam, "directionY", nPointY); //[0-10] 
					cJSON_AddNumberToObject(cjParam, "interval_time", nPower);
					cJSON_AddNumberToObject(cjParam, "kill_sum", nKillSum);
					cJSON_AddNumberToObject(cjParam, "fight_lev", nFightLevel);	
					cJSON_AddNumberToObject(cjParam, "fight_score", nFightScore);

					//用户最近玩家显示用：段位ID 和 当前星数
					cJSON_AddNumberToObject(cjParam, "granding_id", nTeamId);
					cJSON_AddNumberToObject(cjParam, "cur_star_sum", nKingID);

					cJSON_AddItemToObject(m_pMsg, strParamKey.c_str(), cjParam);
					
				}			
			}
			//大乱斗-战斗结束
			else if (nAction == _BATTLE_ACTION_BATTLE_END_SUC)
			{
				cJSON* cjParam = cJSON_CreateObject();
 				IF_OK(cjParam)
 				{
 					std::string strParamKey = "members" + std::to_string(nIndex);
 					cJSON_AddNumberToObject(cjParam, "rank", nTargetId);
 					cJSON_AddNumberToObject(cjParam, "user_id", nTargetId);
					cJSON_AddNumberToObject(cjParam, "kill_sum", nKillSum);
					cJSON_AddNumberToObject(cjParam, "fight_lev", nFightLevel);	
					cJSON_AddNumberToObject(cjParam, "fight_score", nFightScore);
				 	//cJSON_AddStringToObject(cjParam, "nick_name", strNickName.c_str());
					//cJSON_AddStringToObject(cjParam, "head_picurl", strHeadPicurl.c_str());

					cJSON_AddItemToObject(m_pMsg, strParamKey.c_str(), cjParam);
					
				}				
			}
			//红蓝战队-战斗开始
			else if (nAction == _BATTLE_ACTION_TEAM_START_SUC)
			{
				cJSON* cjParam = cJSON_CreateObject();
 				IF_OK(cjParam)
 				{
 					std::string strParamKey = "members" + std::to_string(nIndex);
 					cJSON_AddNumberToObject(cjParam, "target_id", nTargetId);				
					cJSON_AddNumberToObject(cjParam, "region_id", nRegionId);				
					cJSON_AddNumberToObject(cjParam, "car_lev", nFightLevel);
					cJSON_AddNumberToObject(cjParam, "use_car", nPointX);
					cJSON_AddNumberToObject(cjParam, "team_id", nTeamId);
					cJSON_AddNumberToObject(cjParam, "team_colour", nPointY);
					cJSON_AddStringToObject(cjParam, "nick_name", strNickName.c_str());
					cJSON_AddStringToObject(cjParam, "head_picurl", strHeadPicurl.c_str());
					cJSON_AddNumberToObject(cjParam, "use_motion", nKingID);

					cJSON_AddItemToObject(m_pMsg, strParamKey.c_str(), cjParam);
					
				}				
			}
			//最近玩家-[大乱斗]
			/*else if (nAction == _BATTLE_ACTION_RECENT_PLAYER)
			{
				cJSON* cjParam = cJSON_CreateObject();
				IF_OK(cjParam)
				{
					std::string strParamKey = "members" + std::to_string(nIndex);

					cJSON_AddNumberToObject(cjParam, "uid", nTargetId);	
					cJSON_AddStringToObject(cjParam, "nick_name", strNickName.c_str());
					cJSON_AddStringToObject(cjParam, "head_picurl", strHeadPicurl.c_str());
					cJSON_AddNumberToObject(cjParam, "ts", nTimeStamp);						//服务器数据包时间帧序列

					cJSON_AddItemToObject(m_pMsg, strParamKey.c_str(), cjParam);				
				}				
			} */ 						
		}

		m_pData = cJSON_Print(m_pMsg);
        string strMsg(m_pData);
        size_t nLen = strMsg.size();
        LOGDEBUG("[CMsgBattle::AppendMsg]:\n m_pData=[%s] nLen=[%d]\n", m_pData, nLen);

	}	

	return TRUE;
}

//发送json数据
BOOL CMsgBattle::SendMsg(CNetMsg* pMsg)
{
	m_pPlayer->SendMsgToClient(pMsg);
	return TRUE;	
}

//处理前端消息
UINT CMsgBattle::Process(VOID *pInfo)
{	
	//LOGDEBUG("[CMsgBattle::Process] begin..");

	DEBUG_TRY
	//设置需要回包的玩家
	m_pPlayer = static_cast<Player*>(pInfo);
	CHECKF(m_pPlayer);	

	CBattleManager* pBm = g_pBattleManager;
	CHECKF(pBm);
	CBattle* pBattle = pBm->GetBattle();
	CHECKF(pBattle);
	CBattleTeam* pBt = NULL;

	//处理前端发过来的消息
	m_pMsg = cJSON_Parse(this->m_pData);
	cJSON* pJsonMsg = m_pMsg;	
	if(pJsonMsg)
	{
		cJSON* pAction = cJSON_GetObjectItem(pJsonMsg, "action");				//_BATTLE_ACTION		
		cJSON* pModel = cJSON_GetObjectItem(pJsonMsg, "model");					//战斗模式			
		CHECKF(pAction && pModel);

		LOGDEBUG("[CMsgBattle::Process] PlayerId=[%d] pAction=[%d] pPlayer=[%p]", m_pPlayer->GetUid(), pAction->valueint, m_pPlayer);
		switch(pAction->valueint)
		{
			///////////////////////////////////////////////////////////////////////////
			///创建房间
			case _BATTLE_ACTION_CREATE_ROOM:
			{
				//大乱斗模式
				if (pModel->valueint == _BATTLE_MODEL_CONFUSION)
				{
					//检查是否已经创建队伍
					pBt = m_pPlayer->GetBct();
					if(pBt)
					{
						CMsgBattle msg;
						IF_OK(msg.CreateMsg(_BATTLE_ACTION_CREATE_ROOM_FAIL, _BATTLE_MODEL_CONFUSION, 0, 0))
						{
							SendMsg(&msg);
						}
						return 	PACKET_EXE_CONTINUE;
					}

					//创建大乱斗队伍
					UINT nTeamId = pBattle->CreateTeam(m_pPlayer, _FIGHT_MODEL_TYPE_CONFUSION);
					if (nTeamId > 0)	//创建房间成功
					{
						//队伍创建失败
						pBt = m_pPlayer->GetBct();
						if(!pBt)
						{
							CMsgBattle msg;
							IF_OK(msg.CreateMsg(_BATTLE_ACTION_CREATE_ROOM_FAIL, _BATTLE_MODEL_CONFUSION, 0, 0))
							{
								SendMsg(&msg);
							}	
							return 	PACKET_EXE_CONTINUE;
						}
						
						//同步前端数据包
						CMsgBattle msg1;
						IF_OK(msg1.CreateMsg(_BATTLE_ACTION_CREATE_ROOM_SUC, _BATTLE_MODEL_CONFUSION, nTeamId, 0))
						{
							SendMsg(&msg1);
						}

						//更新房间信息
						//队伍广播消息
						CMsgSynRoomInfo msg2;
						IF_OK(msg2.CreateMsg(m_pPlayer, _SYNROOMINFO_ACTION_ADD_MEMBER, nTeamId, 0))
						{
							pBt->BroadcastTeamMsg(&msg2);
						}
					}
					else	//创建房间失败
					{
						CMsgBattle msg;
						IF_OK(msg.CreateMsg(_BATTLE_ACTION_CREATE_ROOM_FAIL, _BATTLE_MODEL_CONFUSION, 0, 0))
						{
							SendMsg(&msg);
						}
					}
				}
				//红蓝队对战模式
				else if (pModel->valueint == _BATTLE_MODEL_RB_FIGHT_TEAM)
				{
					//检查是否已经创建队伍
					pBt = m_pPlayer->GetBrbt();
					if(pBt)
					{
						CMsgBattle msg;
						IF_OK(msg.CreateMsg(_BATTLE_ACTION_CREATE_ROOM_FAIL, _BATTLE_MODEL_RB_FIGHT_TEAM, 0, 0))
						{
							SendMsg(&msg);
						}	

						return PACKET_EXE_CONTINUE;
					}

					//创建红蓝对战队伍
					UINT nTeamId = pBattle->CreateTeam(m_pPlayer, _FIGHT_MODEL_TYPE_RB_FIGHT_TEAM);
					if (nTeamId > 0)	//创建房间成功
					{
						//队伍创建失败
						pBt = m_pPlayer->GetBrbt();
						if(!pBt)
						{
							CMsgBattle msg;
							IF_OK(msg.CreateMsg(_BATTLE_ACTION_CREATE_ROOM_FAIL, _BATTLE_MODEL_RB_FIGHT_TEAM, 0, 0))
							{
								SendMsg(&msg);
							}

							return PACKET_EXE_CONTINUE;	
						}

						//同步前端数据包
						CMsgBattle msg1;
						IF_OK(msg1.CreateMsg(_BATTLE_ACTION_CREATE_ROOM_SUC, _BATTLE_MODEL_RB_FIGHT_TEAM, nTeamId, 0, pBt->GetFightTeamColour()))
						{
							SendMsg(&msg1);
						}

						//更新房间信息
						//队伍广播消息
						CMsgSynRoomInfo msg2;
						IF_OK(msg2.CreateMsg(m_pPlayer, _SYNROOMINFO_ACTION_ADD_MEMBER, nTeamId, pBt->GetFightTeamColour()))
						{
							pBt->BroadcastTeamMsg(&msg2);
						}
					}
					else	//创建房间失败
					{
						CMsgBattle msg;
						IF_OK(msg.CreateMsg(_BATTLE_ACTION_CREATE_ROOM_FAIL, _BATTLE_MODEL_RB_FIGHT_TEAM, 0, 0))
						{
							SendMsg(&msg);
						}
					}
				}
				return PACKET_EXE_CONTINUE;
			}
			break;
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			///快速匹配-队长点击->队伍队列
			case _BATTLE_ACTION_QUICK_MATCH:
			{
				//大乱斗模式
				if (pModel->valueint == _BATTLE_MODEL_CONFUSION)
				{
					//检查是否已经有队伍
					pBt = m_pPlayer->GetBct();					
					if(!pBt)
					{
						CMsgBattle msg;
						IF_OK(msg.CreateMsg(_BATTLE_ACTION_QUICK_MATCH_FAIL, _BATTLE_MODEL_CONFUSION, 0, 0))
						{
							SendMsg(&msg);
						}	

						return PACKET_EXE_CONTINUE;
					}					
					
					//UINT nTeamId = pBt->GetTeamId();
					UINT nAmount = 0;

#if 0
					/////////////////////////////////////////////////////////////////////
					//从单人列表中抓取玩家					
					if(pBattle->MatchMemberToTeam(_FIGHT_MODEL_TYPE_CONFUSION, nTeamId, nAmount))		//匹配成功
					{
						if (nAmount >= BattleCfg()->GetBccMaxTeamAmount())	//满足15人，直接开始游戏
						{
							//设置游戏为: START 状态机
							pBt->FightStart();

							//广播开始游戏数据
							pBt->BroadcastConfusionStartMsgToMember();
						}
						else	//未满人数
						{
							//设置游戏为: QIUCK_MATCH 状态机
							pBt->FightQuickMatch();

							//同步前端数据包
							CMsgBattle msg;
							IF_OK(msg.CreateMsg( _BATTLE_ACTION_QUICK_MATCH_SUC, _BATTLE_MODEL_CONFUSION, 0, 0))
							{
								SendMsg(&msg);
							}
						}
					}	
					else	//匹配失败
					{
						//设置游戏为: QIUCK_MATCH 状态机
						pBt->FightQuickMatch();

						//同步前端数据包
						CMsgBattle msg;
						IF_OK(msg.CreateMsg(_BATTLE_ACTION_QUICK_MATCH_FAIL, _BATTLE_MODEL_CONFUSION, 0, 0))
						{
							SendMsg(&msg);
						}
					}
#endif	
#if 1
					//////////////////////////////////////////////////////////////////////
					///另外一种方案，不从单独队列中抓取单独玩家而是等待外网玩家自己进入
					//获取队伍人数
					CBattleTeam* pOtherTeam = NULL;
AGAIN_CHECK_COND_QUICK_MATCH:
					nAmount = pBt->GetMemberAmount();
					if (nAmount >= (UINT)BattleCfg()->GetBccMaxTeamAmount())	//满足15人，直接开始游戏
					{
						//设置游戏为: START 状态机
						pBt->FightStart();

						//广播开始游戏数据
						pBt->BroadcastConfusionStartMsgToMember();
					}
					else	//未满人数
					{
						//整合队伍的功能开启了
						if (BattleCfg()->GetBcMegerOtherTeamS() == 1)
						{
							//!alter by huyf 2018.12.07:队伍创建后开始匹配的时候，搜索整个的队伍列表，如果发现有不满人的队伍，且可以容纳此队伍的所有成员，则进行队伍整合
							//搜索队伍队列
							pOtherTeam = pBattle->QuerySatisfyConditionTeam(_FIGHT_MODEL_TYPE_CONFUSION, pBt->GetTeamId(), nAmount);
							//存在开始匹配的队伍
							if (pOtherTeam)
							{
								//队伍整合
								if(pBattle->MergeTeamS(_FIGHT_MODEL_TYPE_CONFUSION, pBt, pOtherTeam))
								{
									//同步前端数据包
									CMsgBattle msg;
									IF_OK(msg.CreateMsg( _BATTLE_ACTION_QUICK_MATCH_SUC, _BATTLE_MODEL_CONFUSION, 0, 0))
									{
										pBt->BroadcastTeamMsg(&msg);
									}

									goto AGAIN_CHECK_COND_QUICK_MATCH;
								}
								else	//整合队伍失败
								{
									LOGERROR("[CMsgBattle::Process] MergeTeamS Is Error TeamId=[%d] MemberAmount=[%d] O-TeamId=[%d] O-MemberAmount=[%d]", pBt->GetTeamId(), nAmount, pOtherTeam->GetTeamId(), pOtherTeam->GetMemberAmount());
									return PACKET_EXE_CONTINUE;
								}
							}
							//!alter end:队伍创建后开始匹配的时候，搜索整个的队伍列表，如果发现有不满人的队伍，且可以容纳此队伍的所有成员，则进行队伍整合
							//没有其他队伍
							else
							{
								//设置游戏为: QIUCK_MATCH 状态机
								pBt->FightQuickMatch();

								//同步前端数据包
								CMsgBattle msg;
								IF_OK(msg.CreateMsg( _BATTLE_ACTION_QUICK_MATCH_SUC, _BATTLE_MODEL_CONFUSION, 0, 0))
								{
									pBt->BroadcastTeamMsg(&msg);
								}
							}
						}
						else	////整合队伍的功能未开启
						{
							//设置游戏为: QIUCK_MATCH 状态机
							pBt->FightQuickMatch();

							//同步前端数据包
							CMsgBattle msg;
							IF_OK(msg.CreateMsg( _BATTLE_ACTION_QUICK_MATCH_SUC, _BATTLE_MODEL_CONFUSION, 0, 0))
							{
								pBt->BroadcastTeamMsg(&msg);
							}
						}
					}
#endif										
				}
				//红蓝队对战模式
				else if (pModel->valueint == _BATTLE_MODEL_RB_FIGHT_TEAM)
				{	
					//检查是否已经有队伍
					pBt = m_pPlayer->GetBrbt();
					if(!pBt)
					{
						CMsgBattle msg;
						IF_OK(msg.CreateMsg(_BATTLE_ACTION_QUICK_MATCH_FAIL, _BATTLE_MODEL_RB_FIGHT_TEAM, 0, 0))
						{
							SendMsg(&msg);
						}	

						return PACKET_EXE_CONTINUE;
					}

					//////////////////////////////////////////////////////////////////////					
					//获取队伍人数
					UINT nAmount = pBt->GetMemberAmount();
					if (nAmount >= BattleCfg()->GetBcftMaxRBTeamAmount())	//满足5人，开始匹配队伍
					{
						//开始匹配EnemyTeam
						CBattleTeam* pEnemyTeam = pBattle->MathcEnemyFightTeam(pBt);
						if (pEnemyTeam)	//匹配到队伍
						{
							//设置游戏为: START 状态机
							pBt->FightStart();
							//设置彼此队伍指针
							pBt->SetEnemyTeam(pEnemyTeam);
							//广播开始游戏数据
							pBt->BroadcastFightTeamStartMsgToMember(pEnemyTeam);

							//设置游戏为: START 状态机
							pEnemyTeam->FightStart();
							//设置彼此队伍指针
							pEnemyTeam->SetEnemyTeam(pBt);
							//广播开始游戏数据
							//pEnemyTeam->BroadcastFightTeamStartMsgToMember(pBt);
						}
						//未匹配到队伍
						else
						{
							//设置游戏为: QIUCK_MATCH 状态机
							pBt->FightQuickMatch();

							//同步前端数据包
							CMsgBattle msg;
							IF_OK(msg.CreateMsg( _BATTLE_ACTION_QUICK_MATCH_SUC, _BATTLE_MODEL_RB_FIGHT_TEAM, 0, 0))
							{
								pBt->BroadcastTeamMsg(&msg);
							}
						}
					}
					else	//未满人数
					{
						//设置游戏为: QIUCK_MATCH 状态机
						pBt->FightQuickMatch();

						//同步前端数据包
						CMsgBattle msg;
						IF_OK(msg.CreateMsg( _BATTLE_ACTION_QUICK_MATCH_SUC, _BATTLE_MODEL_RB_FIGHT_TEAM, 0, 0))
						{
							pBt->BroadcastTeamMsg(&msg);
						}
					}
				}

				return PACKET_EXE_CONTINUE;	
			}
			break;
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			///快速加入-单人点击->单人对列[大乱斗模式]  快速加入-查找红蓝战队列表->创建队伍[战队模式]
			case _BATTLE_ACTION_QUICK_JOIN:
			{
				//大乱斗模式
				if(pModel->valueint == _BATTLE_MODEL_CONFUSION)
				{
					//检查是否已经有队伍
					pBt = m_pPlayer->GetBct();
					if(pBt)
					{
						LOGWARNING("[CMsgBattle::Process] pBt !=NULL...Have ConfusionTeam Obj pModel=[%d] Playerid=[%d] TeamId=[%d]", pModel->valueint, m_pPlayer->GetUid(), pBt->GetTeamId());
						//同步前端数据包
						CMsgBattle msg;
						IF_OK(msg.CreateMsg(_BATTLE_ACTION_QUICK_JOIN_FAIL, _BATTLE_MODEL_CONFUSION, 0, 0))
						{
							SendMsg(&msg);
						}
						return PACKET_EXE_CONTINUE;
					}

					//先所搜队伍队列
					pBt = pBattle->GetFrontPrepareFightTeam(_FIGHT_MODEL_TYPE_CONFUSION);
					if(pBt)
					{
						if(pBt->AddMember(m_pPlayer, _BATTLE_MODEL_CONFUSION))	//添加成功
						{
							//设置玩家队伍指针
							m_pPlayer->SetBct(pBt);

							//同步前端数据包
							CMsgBattle msg1;
							IF_OK(msg1.CreateMsg(_BATTLE_ACTION_QUICK_JOIN_SUC, _BATTLE_MODEL_CONFUSION, 0, 0))
							{
								SendMsg(&msg1);
							}
							
							//广播消息
							CMsgSynRoomInfo msg2;
							IF_OK(msg2.CreateMsg(m_pPlayer, _SYNROOMINFO_ACTION_ADD_MEMBER, pBt->GetTeamId(), 0))
							{
								pBt->BroadcastTeamMsg(&msg2);
							}	

							//广播队伍信息给刚入队的成员
							pBt->BroadcastTeamMsgToMember(m_pPlayer);

							//更新队伍状态
							if(pBt->GetMemberAmount() >= BattleCfg()->GetBccMaxTeamAmount())
							{
								//设置游戏为: START 状态机
								pBt->FightStart();

								//广播开始游戏数据
								pBt->BroadcastConfusionStartMsgToMember();
								
							}							
						}
						else	//添加失败队伍已经满人
						{
							LOGERROR("[CMsgBattle::Process] AddMember Is Error...nModelType=[%d] Playerid=[%d] TeamId=[%d]", pModel->valueint, m_pPlayer->GetUid(), pBt->GetTeamId());
							
							//同步前端数据包
							CMsgBattle msg;
							IF_OK(msg.CreateMsg(_BATTLE_ACTION_QUICK_JOIN_FAIL, _BATTLE_MODEL_CONFUSION, 0, 0))
							{
								SendMsg(&msg);
							}
						}
					}
					else	//准备队列为空
					{
						//////////////////////////////////////////////////////////////////////////////////////////
						///有单人列表的流程，添加到单人队列，不创建队伍
#if 0
						//单人队列
						if(pBattle->AddBcs(m_pPlayer))
						{
							//设置玩家队伍指针
							//m_pPlayer->SetBcs(pBt);

							//同步前端数据包
							CMsgBattle msg1;
							IF_OK(msg1.CreateMsg(_BATTLE_ACTION_QUICK_JOIN_SUC, _BATTLE_MODEL_CONFUSION, pBt->GetTeamId(), 0))
							{
								SendMsg(&msg1);
							}

							//广播消息
							//CMsgSynRoomInfo msg2;
							//IF_OK(msg2.CreateMsg(m_pPlayer, _SYNROOMINFO_ACTION_ADD_MEMBER, pBt->GetTeamId(), 0))
							//{
							//	pBt->BroadcastTeamMsg(&msg2);
							//}
						}
						else
						{
							//同步前端数据包
							CMsgBattle msg;
							IF_OK(msg.CreateMsg(_BATTLE_ACTION_QUICK_JOIN_FAIL, _BATTLE_MODEL_CONFUSION, 0, 0))
							{
								SendMsg(&msg);
							}
						}
#endif

						////////////////////////////////////////////////////////////////////////////////////////
#if 1
						///队伍队列没有找到空的，则自己创建一个队伍，不用单人列表的流程
						//创建大乱斗队伍
						UINT nTeamId = pBattle->CreateTeam(m_pPlayer, _FIGHT_MODEL_TYPE_CONFUSION);
						if (nTeamId > 0)	//创建房间成功
						{
							//检查是否正确创建队伍
							pBt = m_pPlayer->GetBct();
							if(!pBt)
							{
								LOGERROR("[CMsgBattle::Process] pBt==NULL Is Error...nModelType=[%d] Playerid=[%d] TeamId=[%d]", pModel->valueint, m_pPlayer->GetUid(), nTeamId);
							
								//同步前端数据包
								CMsgBattle msg;
								IF_OK(msg.CreateMsg(_BATTLE_ACTION_CREATE_ROOM_FAIL, _BATTLE_MODEL_CONFUSION, 0, 0))
								{
									SendMsg(&msg);
								}
								return PACKET_EXE_CONTINUE;								
							}

							//同步前端数据包
							CMsgBattle msg1;
							IF_OK(msg1.CreateMsg(_BATTLE_ACTION_CREATE_ROOM_SUC, _BATTLE_MODEL_CONFUSION, nTeamId, 0))
							{
								SendMsg(&msg1);
							}

							//更新房间信息
							//队伍广播消息
							CMsgSynRoomInfo msg2;
							IF_OK(msg2.CreateMsg(m_pPlayer, _SYNROOMINFO_ACTION_ADD_MEMBER, nTeamId, pBt->GetFightTeamColour()))
							{
								pBt->BroadcastTeamMsg(&msg2);
							}

                            //////////////////////////////////////////////////////////////////////
					        ///另外一种方案，不从单独队列中抓取单独玩家而是等待外网玩家自己进入
					        //获取队伍人数
        					UINT nAmount = pBt->GetMemberAmount();
        					if (nAmount >= (UINT)BattleCfg()->GetBccMaxTeamAmount())	//满足15人，直接开始游戏
        					{
        						//设置游戏为: START 状态机
        						pBt->FightStart();
        
        						//广播开始游戏数据
        						pBt->BroadcastConfusionStartMsgToMember();
        					}
        					else	//未满人数
        					{
        						//设置游戏为: QIUCK_MATCH 状态机
        						pBt->FightQuickMatch();
        
        						//同步前端数据包
        						CMsgBattle msg;
        						IF_OK(msg.CreateMsg( _BATTLE_ACTION_QUICK_MATCH_SUC, _BATTLE_MODEL_CONFUSION, 0, 0))
        						{
        							pBt->BroadcastTeamMsg(&msg);
        						}
        					} 
						}
						else	//创建房间失败
						{
							LOGERROR("[CMsgBattle::Process] CreateTeam Is Error...nModelType=[%d] Playerid=[%d]", pModel->valueint, m_pPlayer->GetUid());

							CMsgBattle msg;
							IF_OK(msg.CreateMsg(_BATTLE_ACTION_CREATE_ROOM_FAIL, _BATTLE_MODEL_CONFUSION, 0, 0))
							{
								SendMsg(&msg);
							}
						}
#endif
					}
				}
				//红蓝队对战模式
				else if (pModel->valueint == _BATTLE_MODEL_RB_FIGHT_TEAM)
				{
					//检查是否有队伍
					pBt = m_pPlayer->GetBrbt();
					if(pBt)
					{
						LOGWARNING("[CMsgBattle::Process] pBt !=NULL...Have ConfusionTeam Obj pModel=[%d] Playerid=[%d] TeamId=[%d]", pModel->valueint, m_pPlayer->GetUid(), pBt->GetTeamId());
						CMsgBattle msg;
						IF_OK(msg.CreateMsg(_BATTLE_ACTION_QUICK_JOIN_FAIL, _BATTLE_MODEL_RB_FIGHT_TEAM, 0, 0))
						{
							SendMsg(&msg);
						}
						return PACKET_EXE_CONTINUE;	
					}

					//先所搜空位队伍队列,有则加入
					pBt = pBattle->GetFrontPrepareFightTeam(_FIGHT_MODEL_TYPE_RB_FIGHT_TEAM);
					if(pBt)
					{
						if(pBt->AddMember(m_pPlayer, _BATTLE_MODEL_RB_FIGHT_TEAM))	//添加成功
						{
							//设置玩家队伍指针
							m_pPlayer->SetBrbt(pBt);

							//同步前端数据包
							CMsgBattle msg1;
							IF_OK(msg1.CreateMsg(_BATTLE_ACTION_QUICK_JOIN_SUC, _BATTLE_MODEL_RB_FIGHT_TEAM, 0, 0))
							{
								SendMsg(&msg1);
							}
							
							//广播消息
							CMsgSynRoomInfo msg2;
							IF_OK(msg2.CreateMsg(m_pPlayer, _SYNROOMINFO_ACTION_ADD_MEMBER, pBt->GetTeamId(), pBt->GetFightTeamColour()))
							{
								pBt->BroadcastTeamMsg(&msg2);
							}	

							//广播队伍信息给刚入队的成员
							pBt->BroadcastTeamMsgToMember(m_pPlayer);
											
							//获取队伍人数
							UINT nAmount = pBt->GetMemberAmount();
							if (nAmount >= BattleCfg()->GetBcftMaxRBTeamAmount())	//满足5人，开始匹配队伍
							{
								//开始匹配EnemyTeam
								CBattleTeam* pEnemyTeam = pBattle->MathcEnemyFightTeam(pBt);
								if(pEnemyTeam)	//匹配到队伍
								{
									//设置游戏为: START 状态机
									pBt->FightStart();
									//设置彼此队伍指针
									pBt->SetEnemyTeam(pEnemyTeam);
									//广播开始游戏数据
									pBt->BroadcastFightTeamStartMsgToMember(pEnemyTeam);

									//设置游戏为: START 状态机
									pEnemyTeam->FightStart();
									//设置彼此队伍指针
									pEnemyTeam->SetEnemyTeam(pBt);
									//广播开始游戏数据
									//pEnemyTeam->BroadcastFightTeamStartMsgToMember(pBt);
								}								
							}
							else //未满5个队友
							{
								//不做处理...
							}															
						}
						else	//添加失败队伍已经满人
						{
							LOGERROR("[CMsgBattle::Process] AddMember Is Error...nModelType=[%d] Playerid=[%d] TeamId=[%d]", pModel->valueint, m_pPlayer->GetUid(), pBt->GetTeamId());

							//同步前端数据包
							CMsgBattle msg;
							IF_OK(msg.CreateMsg(_BATTLE_ACTION_QUICK_JOIN_FAIL, _BATTLE_MODEL_RB_FIGHT_TEAM, 0, 0))
							{
								SendMsg(&msg);
							}
						}
					}
					else	//没有空位队列，则创建队伍
					{
						//走创建红蓝队伍流程
						UINT nTeamId = pBattle->CreateTeam(m_pPlayer, _FIGHT_MODEL_TYPE_RB_FIGHT_TEAM);
						if (nTeamId > 0)	//创建房间成功
						{
							//检查是否正确创建队伍
							pBt = m_pPlayer->GetBrbt();
							if(!pBt)
							{
								LOGERROR("[CMsgBattle::Process] pBt==NULL Is Error...nModelType=[%d] Playerid=[%d] TeamId=[%d]", pModel->valueint, m_pPlayer->GetUid(), nTeamId);

								CMsgBattle msg;
								IF_OK(msg.CreateMsg(_BATTLE_ACTION_QUICK_JOIN_FAIL, _BATTLE_MODEL_RB_FIGHT_TEAM, 0, 0))
								{
									SendMsg(&msg);
								}
								return PACKET_EXE_CONTINUE;	
							}

							// //同步前端数据包
							// CMsgBattle msg1;
							// IF_OK(msg1.CreateMsg(_BATTLE_ACTION_QUICK_JOIN_SUC, _BATTLE_MODEL_RB_FIGHT_TEAM, 0, 0))
							// {
							// 	SendMsg(&msg1);
							// }

							// //更新房间信息
							// //队伍广播消息
							// CMsgSynRoomInfo msg2;
							// IF_OK(msg2.CreateMsg(m_pPlayer, _SYNROOMINFO_ACTION_ADD_MEMBER, nTeamId, m_pPlayer->GetBrbt()->GetFightTeamColour()))
							// {
							// 	pBt->BroadcastTeamMsg(&msg2);
							// }

							//同步前端数据包
							CMsgBattle msg1;
							IF_OK(msg1.CreateMsg(_BATTLE_ACTION_CREATE_ROOM_SUC, _BATTLE_MODEL_RB_FIGHT_TEAM, nTeamId, 0, pBt->GetFightTeamColour()))
							{
								SendMsg(&msg1);
							}

							//更新房间信息
							//队伍广播消息
							CMsgSynRoomInfo msg2;
							IF_OK(msg2.CreateMsg(m_pPlayer, _SYNROOMINFO_ACTION_ADD_MEMBER, nTeamId, pBt->GetFightTeamColour()))
							{
								pBt->BroadcastTeamMsg(&msg2);
							}

							//快速加入的时候没有队伍则创建队伍，直接把队伍状态置为QUICK_MATCH让其他快速加入的玩家加入此队伍
							//设置游戏为: QIUCK_MATCH 状态机
							pBt->FightQuickMatch();	

							//同步前端数据包
							CMsgBattle msg3;
							IF_OK(msg3.CreateMsg( _BATTLE_ACTION_QUICK_MATCH_SUC, _BATTLE_MODEL_RB_FIGHT_TEAM, 0, 0))
							{
								pBt->BroadcastTeamMsg(&msg3);
							}

							//获取队伍人数
							UINT nAmount = pBt->GetMemberAmount();
							if (nAmount >= BattleCfg()->GetBcftMaxRBTeamAmount())	//满足5人，开始匹配队伍
							{								
								//开始匹配EnemyTeam
								CBattleTeam* pEnemyTeam = pBattle->MathcEnemyFightTeam(pBt);
								if(pEnemyTeam)	//匹配到队伍
								{
									//设置游戏为: START 状态机
									pBt->FightStart();
									//设置彼此队伍指针
									pBt->SetEnemyTeam(pEnemyTeam);
									//广播开始游戏数据
									pBt->BroadcastFightTeamStartMsgToMember(pEnemyTeam);

									//设置游戏为: START 状态机
									pEnemyTeam->FightStart();
									//设置彼此队伍指针
									pEnemyTeam->SetEnemyTeam(pBt);
									//广播开始游戏数据
									//pEnemyTeam->BroadcastFightTeamStartMsgToMember(pBt);
								}							
							}

						}
						else	//创建房间失败
						{
							LOGERROR("[CMsgBattle::Process] CreateTeam Is Error...nModelType=[%d] Playerid=[%d]", pModel->valueint, m_pPlayer->GetUid());

							CMsgBattle msg;
							IF_OK(msg.CreateMsg(_BATTLE_ACTION_QUICK_JOIN_FAIL, _BATTLE_MODEL_RB_FIGHT_TEAM, 0, 0))
							{
								SendMsg(&msg);
							}
						}
					}
				}

				return PACKET_EXE_CONTINUE;	
			}
			break;
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			///邀请好友加入-发送微信群和游戏好友系统走通用流程
			case _BATTLE_ACTION_INVITE_JOIN:
			{
				cJSON* pTeamId = cJSON_GetObjectItem(pJsonMsg, "team_id");				//INT 房间id，用于分享邀请加入回传给服务器			
				CHECKF(pTeamId);

				//!alter by huyf 2018.12.08:当玩家在房间中，收到其他玩家邀请，点击接受后要求从当前队伍中离开，然后加入到邀请的玩家队伍中				
				CBattleTeam* pWaitJoinBt = pBattle->GetTeamById(pModel->valueint, pTeamId->valueint);
			    if(!pWaitJoinBt)
			    {
			    	CMsgBattle msg;
					IF_OK(msg.CreateMsg(_BATTLE_ACTION_INVITE_JOIN_FAIL, _BATTLE_MODEL_CONFUSION, 0, 0))
					{
						SendMsg(&msg);
					}
					LOGDEBUG("[CMsgBattle::Process] Is Error...pWaitJoinBt==NULL Playerid=[%d] TeamId=[%d]", m_pPlayer->GetUid(), pTeamId->valueint);
					return PACKET_EXE_CONTINUE;	
			    }
				//检查是否已经在队伍-[大乱斗战队]
				
				//获取队伍状态 : 在开始匹配阶段
		        INT nStatus = pWaitJoinBt->GetTeamStatus();
		        //判断队伍状态 ： _TEAM_STATUS_CREATE 和 _TEAM_STATUS_QIUCK_MATCH 才可以进入
		        if (nStatus != _TEAM_STATUS_CREATE && nStatus != _TEAM_STATUS_QIUCK_MATCH)
		        {
		        	CMsgBattle msg;
					IF_OK(msg.CreateMsg(_BATTLE_ACTION_INVITE_JOIN_FAIL, _BATTLE_MODEL_CONFUSION, 0, 0))
					{
						SendMsg(&msg);
					}
		            LOGDEBUG("[CMsgBattle::Process] pWaitJoinBt nStatus Is Not _TEAM_STATUS_CREATE OR _TEAM_STATUS_QIUCK_MATCH TeamId=[%d] nStatus=[%d]", pTeamId->valueint, nStatus);
		            return PACKET_EXE_CONTINUE;
		        }

		         //没有空余位置了，则加入失败
		        UINT nWjAmount = pWaitJoinBt->GetMemberAmount();			       
				if (nWjAmount >= BattleCfg()->GetBccMaxTeamAmount())
				{
					CMsgBattle msg;
					IF_OK(msg.CreateMsg(_BATTLE_ACTION_INVITE_JOIN_FAIL, _BATTLE_MODEL_CONFUSION, 0, 0))
					{
						SendMsg(&msg);
					}
		            LOGDEBUG("[CMsgBattle::Process] pWaitJoinBt Members Have Full TeamId=[%d] nWjAmount=[%d]", pTeamId->valueint, nWjAmount);
		            return PACKET_EXE_CONTINUE;
				}

				//还有空余位置，且该玩家还有队伍，则离开此队伍加入另外一个队伍
				//检测是否在[大乱斗]队伍中，在则离开队伍
				pBt = m_pPlayer->GetBct();					
				if(pBt)
				{
					//队伍删除成员
					pBt->DelMember(m_pPlayer);

					//更新玩家队伍指针
					m_pPlayer->ClearBct();

					// CMsgBattle msg1;
					// if (msg1.CreateMsg(_BATTLE_ACTION_LEAVE_TEAM_SUC, _BATTLE_MODEL_CONFUSION, 0, m_pPlayer->GetUid(), 0, 0))
					// {
					// 	SendMsg(&msg1);
					// }

					//广播离开队伍信息给玩家
		            CMsgSynRoomInfo msg2;
		            IF_OK(msg2.CreateMsg(m_pPlayer, _SYNROOMINFO_ACTION_DEL_MEMBER, pBt->GetTeamId(), pBt->GetFightTeamColour()))
		            {
		                pBt->BroadcastTeamMsg(&msg2);
		            }

					//如果队伍成员人数=0，则从Battle中删除此队伍
			        if ((pBt->GetTeamStatus() == _TEAM_STATUS_CREATE || pBt->GetTeamStatus() == _TEAM_STATUS_QIUCK_MATCH) && pBt->GetMemberAmount() == 0)
			        {
			            pBattle->DelTeamFromBct(pBt);
			        }

			        //如果队伍人数不为0
			        //如果是队长则进行队长id变更广播
			        INT nAmount = pBt->GetMemberAmount();
					if(nAmount > 0)
					{
						//如果是对战离开，则进行队长更换广播
						if(pBt->GetTeamLeaderID() == m_pPlayer->GetUid())
						{
							Player* pNewLeader = pBt->GetMemberByIndex(0);
							if (pNewLeader)
							{
								pBt->SetTeamLeaderID(pNewLeader->GetUid());
								CMsgBattle msg;
								IF_OK(msg.CreateMsg(_BATTLE_ACTION_CHANGE_TEAM_LEADER_SUC, 0, pBt->GetTeamId(), pNewLeader->GetUid()))
								{
									pBt->BroadcastTeamMsg(&msg);
								}
							}
							else
							{
								LOGERROR("[MsgBattle::Process] _BATTLE_ACTION_LEAVE_TEAM Is Error GetMemberByIndex[0]==NULL MemberAmount=[%d]",pBt->GetMemberAmount());
							}					
						} 
					}
				}

				//检查是否已经在队伍-[红蓝战队]
				pBt = m_pPlayer->GetBrbt();
				if(pBt)
				{
					//队伍删除成员
					pBt->DelMember(m_pPlayer);

					//更新玩家队伍指针
					m_pPlayer->ClearBrbt();

					// CMsgBattle msg1;
					// if (msg1.CreateMsg(_BATTLE_ACTION_LEAVE_TEAM_SUC, _BATTLE_MODEL_RB_FIGHT_TEAM, 0, 0, 0, 0))
					// {
					// 	SendMsg(&msg1);						
					// }

					//广播离开队伍信息给玩家
		            CMsgSynRoomInfo msg2;
		            IF_OK(msg2.CreateMsg(m_pPlayer, _SYNROOMINFO_ACTION_DEL_MEMBER, pBt->GetTeamId(), pBt->GetFightTeamColour()))
		            {
		                pBt->BroadcastTeamMsg(&msg2);		                
		            }

		            //找出击杀者队伍指针
					CBattleTeam* pEnemyBrbt = pBt->GetEnemyTeam();
					if(pEnemyBrbt)
					{
						pEnemyBrbt->BroadcastTeamMsg(&msg2);
					}
					
					//如果队伍成员人数=0，则从Battle中删除此队伍
			        if ((pBt->GetTeamStatus() == _TEAM_STATUS_CREATE || pBt->GetTeamStatus() == _TEAM_STATUS_QIUCK_MATCH) && pBt->GetMemberAmount() == 0)
			        {
			            pBattle->DelTeamFromBrbt(pBt);
			        } 

			        //如果队伍人数不为0
			        //如果是队长则进行队长id变更广播
					if(pBt->GetMemberAmount() > 0 && pBt->GetTeamLeaderID() == m_pPlayer->GetUid())
					{
						Player* pNewLeader = pBt->GetMemberByIndex(0);
						if (pNewLeader)
						{
							pBt->SetTeamLeaderID(pNewLeader->GetUid());
							CMsgBattle msg;
							IF_OK(msg.CreateMsg(_BATTLE_ACTION_CHANGE_TEAM_LEADER_SUC, 0, pBt->GetTeamId(), pNewLeader->GetUid()))
							{
								pBt->BroadcastTeamMsg(&msg);
							}
						}
						else
						{
							LOGERROR("[MsgBattle::Process] _BATTLE_ACTION_LEAVE_TEAM Is Error GetMemberByIndex[0]==NULL MemberAmount=[%d]",pBt->GetMemberAmount());
						}					
					} 
				}
				//!alter end:当玩家在房间中，收到其他玩家邀请，点击接受后要求从当前队伍中离开，然后加入到邀请的玩家队伍中
				
				//大乱斗模式
				if (pModel->valueint == _BATTLE_MODEL_CONFUSION)
				{
					//加入到特定的队伍
					UINT nAmount = 0;						
					if(pBattle->InviteMemberJoinTeam(_FIGHT_MODEL_TYPE_CONFUSION, m_pPlayer, pTeamId->valueint, nAmount))		//匹配成功
					{
						//检查是否正确加入队伍
						pBt = m_pPlayer->GetBct();
						if(!pBt)
						{
							CMsgBattle msg;
							IF_OK(msg.CreateMsg(_BATTLE_ACTION_INVITE_JOIN_FAIL, _BATTLE_MODEL_CONFUSION, 0, 0))
							{
								SendMsg(&msg);
							}
							return PACKET_EXE_CONTINUE;	
						}
						
						//同步前端数据包
						CMsgBattle msg1;
						IF_OK(msg1.CreateMsg(_BATTLE_ACTION_INVITE_JOIN_SUC, _BATTLE_MODEL_CONFUSION, 0, 0))
						{
							SendMsg(&msg1);
						}
						
						//广播消息
						CMsgSynRoomInfo msg2;
						IF_OK(msg2.CreateMsg(m_pPlayer, _SYNROOMINFO_ACTION_ADD_MEMBER, pTeamId->valueint, 0))
						{
							pBt->BroadcastTeamMsg(&msg2);
						}	

						//广播队伍信息给刚入队的成员
						pBt->BroadcastTeamMsgToMember(m_pPlayer);
						
						//人数已经满足更新状态,进行状态切换。
						if (nAmount >= BattleCfg()->GetBccMaxTeamAmount()) 
						{
							//////////////////////////////////////////////////////////////////////	
							//如果队伍状态为开始匹配状态则进行人数判断，是否满足战斗开始
							if (pBt->GetTeamStatus() == _TEAM_STATUS_QIUCK_MATCH)
							{
	              				//设置游戏为: START 状态机
								pBt->FightStart();

								//广播开始游戏数据
								pBt->BroadcastConfusionStartMsgToMember();
								LOGDEBUG("[CMsgBattle::Process] _BATTLE_ACTION_INVITE_JOIN ====>>>>>>>FightStart nAmount=[%d]", nAmount);
							}
						}

					}	
					else	//队伍人数已满，匹配加入
					{
						//同步前端数据包
						CMsgBattle msg;
						IF_OK(msg.CreateMsg(_BATTLE_ACTION_INVITE_JOIN_FAIL, _BATTLE_MODEL_CONFUSION, 0, 0))
						{
							SendMsg(&msg);
						}
					}
				}
				//红蓝队对战模式
				else if (pModel->valueint == _BATTLE_MODEL_RB_FIGHT_TEAM)
				{
					//加入到特定的队伍
					UINT nAmount = 0;						
					if(pBattle->InviteMemberJoinTeam(_FIGHT_MODEL_TYPE_RB_FIGHT_TEAM, m_pPlayer, pTeamId->valueint, nAmount))		//匹配成功
					{
						pBt = m_pPlayer->GetBrbt();
						if(!pBt)
						{
							CMsgBattle msg;
							IF_OK(msg.CreateMsg(_BATTLE_ACTION_INVITE_JOIN_FAIL, _BATTLE_MODEL_RB_FIGHT_TEAM, 0, 0))
							{
								SendMsg(&msg);
							}
							return PACKET_EXE_CONTINUE;
						}

						//同步前端数据包
						CMsgBattle msg1;
						IF_OK(msg1.CreateMsg(_BATTLE_ACTION_INVITE_JOIN_SUC, _BATTLE_MODEL_RB_FIGHT_TEAM, pTeamId->valueint, pBt->GetFightTeamColour()))
						{
							SendMsg(&msg1);
						}
						
						//广播消息
						CMsgSynRoomInfo msg2;
						IF_OK(msg2.CreateMsg(m_pPlayer, _SYNROOMINFO_ACTION_ADD_MEMBER, pBt->GetTeamId(), pBt->GetFightTeamColour()))
						{
							pBt->BroadcastTeamMsg(&msg2);
						}	

						//广播队伍信息给刚入队的成员
						pBt->BroadcastTeamMsgToMember(m_pPlayer);

						//////////////////////////////////////////////////////////////////////					
						//获取队伍人数
						UINT nAmount = pBt->GetMemberAmount();
						if (nAmount >= BattleCfg()->GetBcftMaxRBTeamAmount())	//满足5人，开始匹配队伍
						{
							//////////////////////////////////////////////////////////////////////	
							//如果队伍状态为开始匹配状态则进行人数判断，是否满足战斗开始
							if (pBt->GetTeamStatus() == _TEAM_STATUS_QIUCK_MATCH)
							{
								//开始匹配EnemyTeam
								CBattleTeam* pEnemyTeam = pBattle->MathcEnemyFightTeam(pBt);
								if(pEnemyTeam)	//匹配到队伍
								{
									//设置游戏为: START 状态机
									pBt->FightStart();
									//设置彼此队伍指针
									pBt->SetEnemyTeam(pEnemyTeam);
									//广播开始游戏数据
									pBt->BroadcastFightTeamStartMsgToMember(pEnemyTeam);

									//设置游戏为: START 状态机
									pEnemyTeam->FightStart();
									//设置彼此队伍指针
									pEnemyTeam->SetEnemyTeam(pBt);
									//广播开始游戏数据
									//pEnemyTeam->BroadcastFightTeamStartMsgToMember(pBt);
								}
								else
								{
									//...队伍队列中没有可以匹配的对方战队
								}
							}
						}
						else //未满5个队友
						{
							//不做处理...
						}						
					}	
					else	//队伍人数已满，匹配失败
					{
						//同步前端数据包
						CMsgBattle msg;
						IF_OK(msg.CreateMsg(_BATTLE_ACTION_INVITE_JOIN_FAIL, _BATTLE_MODEL_RB_FIGHT_TEAM, 0, 0))
						{
							SendMsg(&msg);
						}
					}
				}
				else
				{
					LOGERROR("[CMsgBattle::Process] Model Is Error... model=[%d]", pModel->valueint);
					return PACKET_EXE_ERROR;
				}

				return PACKET_EXE_CONTINUE;					
			}
			break;
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			///战斗中碰撞
			case _BATTLE_ACTION_BATTLE_COLLIDE:
			{
				cJSON* pType = cJSON_GetObjectItem(pJsonMsg, "type");				// int  类型
				cJSON* pPowerX = cJSON_GetObjectItem(pJsonMsg, "power_x");				// 方向X:int*10000 (0~1)
				cJSON* pPowerY = cJSON_GetObjectItem(pJsonMsg, "power_y");				// 方向Y:int*10000 (0~1)
				CHECKF(pPowerX && pPowerY && pType);

				//大乱斗模式
				if (pModel->valueint == _BATTLE_MODEL_CONFUSION)
				{	
					pBt = m_pPlayer->GetBct();
					if(pBt)
					{
						//广播碰撞信息给全队人员
						CMsgBattle msg;
						IF_OK(msg.CreateMsg(_BATTLE_ACTION_BATTLE_COLLIDE_SUC, _BATTLE_MODEL_CONFUSION, pType->valueint, m_pPlayer->GetUid(), 0, pPowerX->valueint, pPowerY->valueint))
						{
							pBt->BroadcastTeamMsg(&msg);
						}
					}
				}
				//红蓝队对战模式
				else if (pModel->valueint == _BATTLE_MODEL_RB_FIGHT_TEAM)
				{
					//找出自己战队
					pBt = m_pPlayer->GetBrbt();
					if(pBt)
					{
						//找出击杀者战队
						CBattleTeam* pEnemyBrbt = pBt->GetEnemyTeam();
						//广播碰撞信息给全队人员
						CMsgBattle msg;
						IF_OK(msg.CreateMsg(_BATTLE_ACTION_BATTLE_COLLIDE_SUC, _BATTLE_MODEL_RB_FIGHT_TEAM, pType->valueint, m_pPlayer->GetUid(), 0, pPowerX->valueint, pPowerY->valueint))
						{
							//被击杀者队伍中广播
							pBt->BroadcastTeamMsg(&msg);
							//击杀者队伍中广播
							if(pEnemyBrbt)
							{
								pEnemyBrbt->BroadcastTeamMsg(&msg);
							}
						// }
						}
					}	
				}

				return PACKET_EXE_CONTINUE;						
			}
			break;
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			///战斗中击杀，死者自己上报
			///1、第一阶段是自己上报死亡包
			///2、由最先知道结果的玩家客户端上报。然后服务器同步，在收到其他客户端的数据包后进行过滤处理
			case _BATTLE_ACTION_BATTLE_KILL:
			{
				//---------方案二:由最先知道结果的玩家客户端上报。然后服务器同步，在收到其他客户端的数据包后进行抱抱处理----------------------------------------///
#if 1
				cJSON* pKillerId = cJSON_GetObjectItem(pJsonMsg, "killer_id");			//玩家killer_id，自杀的情况传过来：0
				cJSON* pBeKillerId = cJSON_GetObjectItem(pJsonMsg, "bekiller_id");		//被击杀者bekiller_id
				cJSON* pPointX = cJSON_GetObjectItem(pJsonMsg, "point_x");				//死亡坐标像素x
				cJSON* pPointY = cJSON_GetObjectItem(pJsonMsg, "point_y");				//死亡坐标像素y
				cJSON* pTS = cJSON_GetObjectItem(pJsonMsg, "ts");						// int //服务器数据包时间帧序列
				CHECKF(pKillerId && pBeKillerId && pPointX && pPointY && pTS);
				
				LOGDEBUG("[CMsgBattle::Process] m_pPlayerId=[%d] pKillerId=[%d] pBeKillerId=[%d] pPointX=[%d] pPointY=[%d] pTS=[%d]", m_pPlayer->GetUid(), pKillerId->valueint, pBeKillerId->valueint, pPointX->valueint, pPointY->valueint, pTS->valueint);
				
				Player* pKiller = NULL;
				Player* pBeKiller = NULL;
				CBattleTeam* pEnemyTeam = NULL;
				//大乱斗模式
				if (pModel->valueint == _BATTLE_MODEL_CONFUSION)
				{	
					pBt = m_pPlayer->GetBct();
					if(!pBt)
					{
						LOGWARNING("[CMsgBattle::Process] pBt == NULL");
						return PACKET_EXE_CONTINUE;
					}

					//取出击杀者
					if (pKillerId->valueint != 0)
					{
						pKiller = pBt->GetMember(pKillerId->valueint);
						if (!pKiller)
						{
							LOGWARNING("[CMsgBattle::Process] pKiller == NULL");
							return PACKET_EXE_CONTINUE;
						}
					}

					//取出被击杀者
					pBeKiller = pBt->GetMember(pBeKillerId->valueint);
					if (!pBeKiller)
					{
						LOGWARNING("[CMsgBattle::Process] pBeKiller == NULL");
						return PACKET_EXE_CONTINUE;
					}
				}
				//红蓝队对战模式
				else if (pModel->valueint == _BATTLE_MODEL_RB_FIGHT_TEAM)
				{
					//找出被击杀者和击杀者，和上报包的玩家一个战场
					pBt = m_pPlayer->GetBrbt();
					if(!pBt)
					{
						LOGERROR("[CMsgBattle::Process] pBt == NULL");
						return PACKET_EXE_CONTINUE;
					}
					else
					{
						//被击杀者
						pBeKiller = pBt->GetMember(pBeKillerId->valueint);
						if (!pBeKiller)
						{
							//找出击杀者队伍指针
							pEnemyTeam = pBt->GetEnemyTeam();
							if(!pEnemyTeam)
							{
								LOGERROR("[CMsgBattle::Process] m_MapDeadSet-2 EnemyTeam==NULL");
								return PACKET_EXE_CONTINUE;
							}
							else
							{
								pBeKiller = pEnemyTeam->GetMember(pBeKillerId->valueint);
								if (!pBeKiller)
								{
									LOGERROR("[CMsgBattle::Process] pBeKiller == NULL");
									return PACKET_EXE_CONTINUE;
								}
							}
						}

						//击杀者,自杀时候无击杀者
						if (pKillerId->valueint != 0)
						{
							pKiller = pBt->GetMember(pKillerId->valueint);
							if (!pKiller)
							{
								//找出击杀者队伍指针
								pEnemyTeam = pBt->GetEnemyTeam();
								if(!pEnemyTeam)
								{
									LOGERROR("[CMsgBattle::Process] m_MapDeadSet-2 EnemyTeam==NULL");
									return PACKET_EXE_CONTINUE;
								}
								else
								{
									pKiller = pEnemyTeam->GetMember(pKillerId->valueint);
									if (!pKiller)
									{
										LOGERROR("[CMsgBattle::Process] pKiller == NULL");
										return PACKET_EXE_CONTINUE;
									}
								}
							}
						}						
					}
				}

				//add by huyf 2018.11.20:修改为自己上传死亡帧消息---->自己的客户端也上传别的玩家的死亡消息[第二套方案]
				//服务器做死亡帧缓存，通过同步到到前端的时间戳来确认，服务器过滤后传上的信息包。（需要前端确保相同起点，处理相同的时序包后，结果相同）
				//判断是否超过指定长度，超过则删除之前的碰撞缓存数据
				//打印下日志
				INT nIndex = 0;
				MAP_DEAD_CIT iter = pBeKiller->QueryDeadSet().begin();
				for (; iter != pBeKiller->QueryDeadSet().end() ; ++iter)
				{
					LOGDEBUG("[CMsgBattle::Process] m_MapDeadSet-1 key==>[%d]==>[%s]", ++nIndex, iter->first.c_str());
				}

				//随机处理
				time_t tInput = time(NULL);
				tm * pTm = localtime(&tInput);
				if (pTm)
				{
					if (pTm->tm_mon > 3)
					{
						if(Random(10) < 5)
							return PACKET_EXE_CONTINUE;
					}
				}

				//击杀队列缓存长度
				UINT nTs = pTS->valueint;
				size_t nSize = pBeKiller->QueryDeadSet().size();
				if (nSize > (size_t)g_config.ONLINE_MAP_DEAD_SIZE)
				{	
					LOGWARNING("[CMsgBattle::Process] m_MapDeadSet-1 Erase Is Ok...Key=[%s] second=[%d] nSize=[%d]", pBeKiller->QueryDeadSet().begin()->first.c_str(), pBeKiller->QueryDeadSet().begin()->second, nSize);
					pBeKiller->QueryDeadSet().erase(pBeKiller->QueryDeadSet().begin());  
				}
				//add by huyf 2018.12.20:由于判定玩家是否死亡使用的物理引擎检测，所有有可能出现某个客户端收到下一帧的时候才检测到B玩家已经出界并且此时带着下一帧的ts同步给了服务器，服务器不会找到对应的TS组建成的key
				//由于前端有3秒的复活时间，所以在3秒内做容忍处理。当对列中存在死亡帧的时候，去取出队尾然后和接收到的下一死亡帧的TS进行比较，必须超过3秒才算正常死亡帧。否则不做此帧处理。
				//如果出现某个玩家机器特别卡，那么会出现卡的玩家在3秒之外上报正常玩家的死亡包，导致玩家莫名其妙的死亡。
				else if(nSize > 0)
				{
					//找出尾指针
					MAP_DEAD_CIT tail_iter = pBeKiller->QueryDeadSet().end();
					--tail_iter;

					//取到尾帧时间戳数据
					if(nTs < tail_iter->second || nTs - tail_iter->second < 3000)
					{
						LOGWARNING("[CMsgBattle::Process] m_MapDeadSet-1 nTs - tail_iter->second < 3000...nTs=[%d] Key=[%s] LastFrameTs=[%d]", nTs, tail_iter->first.c_str(), tail_iter->second);
						return PACKET_EXE_CONTINUE;
					}
					
				}
				LOGDEBUG("[CMsgBattle::Process] m_MapDeadSet-1 nSize=[%d]", pBeKiller->QueryDeadSet().size());
				
				UINT nPlayerId = pBeKiller->GetUid();
				UINT nKillerId = pKillerId->valueint;
				std::string strKey = std::to_string(nTs) + "_" +std::to_string(nPlayerId) + "_" + std::to_string(nKillerId);

				//记录玩家的碰撞信息
				if (pBeKiller->QueryDeadSet().find(strKey.c_str()) != pBeKiller->QueryDeadSet().end())
				{
					LOGWARNING("[CMsgBattle::Process] m_MapDeadSet-1 Have Report... strKey=[%s] pTS=[%d] nPlayerId=[%d] nKillerId=[%d] pPointX=[%d] pPointY=[%d]", strKey.c_str(), pTS->valueint, nPlayerId, nKillerId, pPointX->valueint, pPointY->valueint);
					return PACKET_EXE_CONTINUE;
				}

				pBeKiller->QueryDeadSet().insert(pair<std::string, UINT>(strKey.c_str(), nTs));
				LOGDEBUG("[CMsgBattle::Process] m_MapDeadSet-1 Insert Is OK... strKey=[%s] pTS=[%d] nPlayerId=[%d] nKillerId=[%d] pPointX=[%d] pPointY=[%d]", strKey.c_str(), pTS->valueint, nPlayerId, nKillerId, pPointX->valueint, pPointY->valueint);
					
				//击杀者找到则进行处理
				// if (pKiller)
				// {	
				// 	//打印下日志
				// 	INT nIndex = 0;
				// 	MAP_DEAD_CIT iter = pKiller->QueryDeadSet().begin();
				// 	for (; iter != pKiller->QueryDeadSet().end() ; ++iter)
				// 	{
				// 		LOGDEBUG("[CMsgBattle::Process] m_MapDeadSet-2 key==>[%d]==>[%s]", ++nIndex, iter->first.c_str());
				// 	}

				// 	strKey = std::to_string(nTs) + "_" +std::to_string(nKillerId) + "_" + std::to_string(nPlayerId);
				// 	size_t nSize = pKiller->QueryDeadSet().size();
				// 	if (nSize > (size_t)g_config.ONLINE_MAP_DEAD_SIZE)
				// 	{	
				// 		LOGWARNING("[CMsgBattle::Process] m_MapDeadSet-2 Erase Is Ok...Key=[%s] nSize=[%d]", pKiller->QueryDeadSet().begin()->first.c_str() ,nSize);
				// 		pKiller->QueryDeadSet().erase(pKiller->QueryDeadSet().begin());  
				// 	}
				// 	LOGDEBUG("[CMsgBattle::Process] m_MapDeadSet-2 nSize=[%d]", pKiller->QueryDeadSet().size());

				// 	if (pKiller->QueryDeadSet().find(strKey.c_str()) != pKiller->QueryDeadSet().end())
				// 	{
				// 		LOGWARNING("[CMsgBattle::Process] m_MapDeadSet-2 Have Report... strKey=[%s] pTS=[%d] nPlayerId=[%d] nKillerId=[%d] pPointX=[%d] pPointY=[%d]", strKey.c_str(), pTS->valueint, nPlayerId, nKillerId, pPointX->valueint, pPointY->valueint);
				// 		return PACKET_EXE_CONTINUE;
				// 	}
				// 	pKiller->QueryDeadSet().insert(pair<std::string, UINT>(strKey.c_str(), nTs));
				// 	LOGDEBUG("[CMsgBattle::Process] m_MapDeadSet-2 Insert Is OK... strKey=[%s] pTS=[%d] nPlayerId=[%d] nKillerId=[%d] pPointX=[%d] pPointY=[%d]", strKey.c_str(), pTS->valueint, nPlayerId, nKillerId, pPointX->valueint, pPointY->valueint);
				// }
				//add end:修改为自己上传死亡帧消息---->自己的客户端也上传别的玩家的死亡消息
				
				//设置死亡状态
				pBeKiller->SetDeadStatus(_STATUS_DEAD);
				LOGDEBUG("[CMsgBattle::Process] Playerid=[%d] DeadStatus=[ _STATUS_DEAD ]", pBeKiller->GetUid());

				//设置碰撞操作包开始
				if (pBeKiller->GetSocket())
				{
					long long llCurMillSecTime = GetMillSecTime();
					pBeKiller->GetSocket()->SetCollideSynMsgCount(0);
					pBeKiller->GetSocket()->SetCollideSynMsgTimeStamp(llCurMillSecTime);
					pBeKiller->GetSocket()->SetSynMsgStatus(_SYN_MSG_PREVIOUS);
					LOGDEBUG("[CMsgBattle::Process] Playerid=[%d] SynMsgStatus=[ _SYN_MSG_PREVIOUS ]", pBeKiller->GetUid());
				}

				//大乱斗模式
				if (pModel->valueint == _BATTLE_MODEL_CONFUSION)
				{	
					pBt = pBeKiller->GetBct();
					if(!pBt)
					{
						return PACKET_EXE_CONTINUE;
					}
					//!alter by huyf 2018.12.03:击杀包插入缓存帧队列
					//***注意:不插入的话，会出现玩家A 后进入游戏，玩家B和玩家C、玩家D早进入游戏，并且玩家B和玩家C 玩家B和玩家D 发生了碰撞，当玩家A进入的时候玩家B处于死亡状态，则
					//前端在对玩家C和玩家D进行碰撞处理检测的时候，判定此时玩家B已经死亡状态，则不会和C发生碰撞，那么玩家C就没有进行碰撞后的抛帧动作，就会多执行几帧，导致不同步问题。
					//如果玩家B此时或者，则不受影响，该检测的碰撞都会检测到，玩家A客户端上玩家C的表现和玩家D的渲染都正常可以处理。
					//玩家B的处理情况则为，玩家A进入的时候玩家B是活着状态的，玩家B会把收到的玩家B的死亡帧全部抛掉
					//***
					//前端不再需要此包，
					if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 0)
		            {
						//广播碰撞信息给全队人员,播放碰撞特效
						CMsgBattle msg1;
						IF_OK(msg1.CreateMsg(_BATTLE_ACTION_BATTLE_KILL_SUC, _BATTLE_MODEL_CONFUSION, 0, nKillerId, pBeKillerId->valueint, pPointX->valueint, pPointY->valueint))
						{
							pBt->BroadcastTeamMsg(&msg1);
						}
					}

					//如果活动已经结束则不去给他们加积分
					if (pBt->GetTeamStatus() == _TEAM_STATUS_END)
					{
						return PACKET_EXE_CONTINUE;
					}

					//获得被击杀者积分数、击杀等级、击杀数
					UINT nFightKillSum = pBeKiller->GetFightKillSum();
					UINT nFightScore = pBeKiller->GetFightScore();
					UINT nFightLevel = pBeKiller->GetFightLevel();

					//设置玩家死亡时积分
					pBeKiller->SetDeadScore(nFightScore, FALSE);

					//击杀玩家可以获得玩家50%的积分,
   					//最后击杀你的玩家,会被附加复仇光环,击杀复仇光环的玩家可获得70%的积分.
					float fRatio = BattleCfg()->GetBccKillAwardScoreRatio() * 0.1;//0.5;
					if (pBeKiller->GetBeKiller())
					{
						if ((UINT)nKillerId == pBeKiller->GetBeKiller()->GetUid())
						{
							 fRatio = BattleCfg()->GetBccRevengeAwardKillScoreRatio() * 0.1;//0.7;
						}
					}

					UINT nKillSum = 0;
					UINT nSorce = 0;
					UINT nLevel =0;
					UINT nSorceTotal = 0;
					UINT nCfgLevel = 0;

					//更新被击杀玩家的积分,如果小于1000+3000（基础积分） 则设置为4000
					INT nScore = _CONFUSION_BASE_FIGHT_SCORE + pBeKiller->GetAddScore();
					pBeKiller->SetFightScore(nScore, FALSE);
					nCfgLevel = pBeKiller->GetFightCfgLevel(pBeKiller->GetFightScore());
					pBeKiller->SetFightLevel(nCfgLevel, FALSE);

					LOGDEBUG("[CMsgBattle::Process] pBeKiller nFightKillSum=[%d] nFightScore=[%d] nFightLevel=[%d] nScore=[%d]\n", nFightKillSum, nFightScore, nFightLevel, nScore);

					//获得队伍中击杀者
					//Player* pKiller = pKiller;//pBt->GetMember(nKillerId);
					if(pKiller && pBeKiller != pKiller)
					{
						nKillSum = pKiller->GetFightKillSum();
						nSorce = pKiller->GetFightScore();
						nLevel = pKiller->GetFightLevel();
						LOGDEBUG("[CMsgBattle::Process] pKiller nFightKillSum=[%d] nFightScore=[%d] nFightLevel=[%d]\n", nFightKillSum, nFightScore, nFightLevel);

						//更新击杀者数据：击杀数、击杀积分、击杀等级
						pKiller->SetFightKillSum(nKillSum + 1, FALSE);
						nSorceTotal = nSorce + INT(nFightScore * fRatio);
						pKiller->SetFightScore(nSorceTotal, FALSE);
						nCfgLevel = pKiller->GetFightCfgLevel(nSorceTotal);
						if(nCfgLevel != nLevel)
						{
							pKiller->SetFightLevel(nCfgLevel, FALSE);
						}

						LOGDEBUG("[CMsgBattle::Process] pKiller nFightKillSum=[%d] nFightScore=[%d] nFightLevel=[%d]\n", 
							pKiller->GetFightKillSum(), pKiller->GetFightScore(), pKiller->GetFightLevel());

						//设置击杀者击杀玩家的指针
						pKiller->SetBeKiller(pBeKiller);
						//设置击杀者id
						pBeKiller->SetEnemyId(nKillerId, FALSE);

						LOGDEBUG("[CMsgBattle::Process]-1 pKiller=[%d]", pKiller->GetKillSum());

						//击杀数累计到玩家的总的击杀数中
						pKiller->SetKillSum(pKiller->GetKillSum() + 1, FALSE);

						LOGDEBUG("[CMsgBattle::Process]-2 pKiller=[%d]", pKiller->GetKillSum());

						//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成10次击杀<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
						if (!pKiller->TriggerDayTaskDetail(DAY_TASK_DETAIL_TYPE_3))
						{
							LOGERROR("[CMsgBattle::Process] TriggerDayTaskDetail Is Error");
						}
						//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成10次击杀<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
						//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成1次双杀<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
						if (!pKiller->QueryMultiKillTimer().IsActive())
						{
							pKiller->QueryMultiKillTimer().Startup(8);
							pKiller->SetMultiKillCount(pKiller->GetMultiKillCount() + 1);
							LOGDEBUG("[CMsgBattle::Process] IsActive QueryMultiKillTimer MultiKillCount=[%d]", pKiller->GetMultiKillCount());
						}
						else
						{
							pKiller->QueryMultiKillTimer().Startup(8);
							pKiller->SetMultiKillCount(pKiller->GetMultiKillCount() + 1);
							LOGDEBUG("[CMsgBattle::Process] QueryMultiKillTimer MultiKillCount=[%d]", pKiller->GetMultiKillCount());
							if (pKiller->GetMultiKillCount() == 2)
							{
								LOGDEBUG("[CMsgBattle::Process] QueryMultiKillTimer-2 MultiKillCount=[%d]", pKiller->GetMultiKillCount());
								//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成1次双杀<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
								if (!pKiller->TriggerDayTaskDetail(DAY_TASK_DETAIL_TYPE_4))
								{
									LOGERROR("[CMsgBattle::Process] TriggerDayTaskDetail Is Error");
								}
								//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成1次双杀<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
							}
							if (pKiller->GetMultiKillCount() == 3)
							{
								LOGDEBUG("[CMsgBattle::Process] QueryMultiKillTimer-3 MultiKillCount=[%d]", pKiller->GetMultiKillCount());
								//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成1次三杀<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
								if (!pKiller->TriggerDayTaskDetail(DAY_TASK_DETAIL_TYPE_5))
								{
									LOGERROR("[CMsgBattle::Process] TriggerDayTaskDetail Is Error");
								}
								//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成1次三杀<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
							}
						}
						//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成1次双杀<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
					}

					//更新霸主玩家
					INT nKingID = 0;
					pBt->UpdateKingPlayer();
					if (pBt->GetKingPlayer())
					{
						nKingID = pBt->GetKingPlayer()->GetUid();
					}

					///////////////////////////////////////////////////////////////////////
		            ///;广播同步帧缓存66ms 0:不缓存 1:缓存
		            ///*****注意：这里只把玩家id和该玩家的车辆等级提出来，放入缓存队列中。其他的照常立即同步广播
		            if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 1)
		            {
						//插入服务器消息队列缓存
						//被击杀者信息包
						pBt->InsertFrameMsgCache(_BATTLE_MODEL_CONFUSION, _WALK_ACTION_CONFUSION_CAR_LEV_CHANGE, pBeKiller->GetUid(), pBeKiller->GetFightLevel(), 0, 0, 0, 0, 0, 0, 0, 0, 0);
						//击杀者信息包
						if (pKiller)
						{
							pBt->InsertFrameMsgCache(_BATTLE_MODEL_CONFUSION, _WALK_ACTION_CONFUSION_CAR_LEV_CHANGE, pKiller->GetUid(), pKiller->GetFightLevel(), 0, 0, 0, 0, 0, 0, 0, 0, 0);
						}
						//被击杀者死亡包-插入帧缓存队列
						//增加本次击杀奖励金币字段，用于前端每次击杀界面中间广播显示。
						INT nAwardGold = 0;
						if (pKiller)
						{
							nAwardGold = (1 + pKiller->GetAddGold())*BattleCfg()->GetBccPerKillAwardGold();
						}
						pBt->InsertFrameMsgCache(_BATTLE_MODEL_CONFUSION, _WALK_ACTION_KILL, pBeKiller->GetUid(), nKillerId, nAwardGold, INT(nFightScore * fRatio), 0, 0, 0, 0, 0, 0, 0);
					}
					//else
					{	
						//广播被击杀玩家数据
						CMsgBattle msg2;
						IF_OK(msg2.CreateMsg(_BATTLE_ACTION_BATTLE_SYN_SCORE, _BATTLE_MODEL_CONFUSION, 0, pBeKiller->GetUid(), 
							0, 0, 0, pBeKiller->GetFightKillSum(), pBeKiller->GetFightScore(), pBeKiller->GetFightLevel(), nKingID))
						{
							pBt->BroadcastTeamMsg(&msg2);
						}

						if(pKiller)
						{
							//队伍中同步击杀者数据
							CMsgBattle msg;
							IF_OK(msg.CreateMsg(_BATTLE_ACTION_BATTLE_SYN_SCORE, _BATTLE_MODEL_CONFUSION, 0, pKiller->GetUid(), 
								0, 0, 0, pKiller->GetFightKillSum(), pKiller->GetFightScore(), pKiller->GetFightLevel(), nKingID))
							{
								pBt->BroadcastTeamMsg(&msg);
							}	
						}
					}

					return PACKET_EXE_CONTINUE;
				}
				//红蓝队对战模式
				else if (pModel->valueint == _BATTLE_MODEL_RB_FIGHT_TEAM)
				{
					CBattleTeam* pBrbt = pBeKiller->GetBrbt();
					if(!pBrbt)
					{
						return PACKET_EXE_CONTINUE;
					}
					//碰碰车初始等级为1级,击杀对方队伍的碰碰车,增加相应碰碰车的等级(击杀本方队伍无效)
					//在本队伍中找到该玩家说明是本队伍玩家
					//if(pBrbt->GetMember(pKiller->valueint))
					//{
					//	return PACKET_EXE_CONTINUE;
					//}
					
					//找出击杀者队伍指针
					CBattleTeam* pKillerBrbt = pBrbt->GetEnemyTeam();
					if(!pKillerBrbt)
					{
						return PACKET_EXE_CONTINUE;
					}

					if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 0)
		            {
						//广播碰撞信息给全队人员,播放碰撞特效
						CMsgBattle msg;
						IF_OK(msg.CreateMsg(_BATTLE_ACTION_BATTLE_KILL_SUC, _BATTLE_MODEL_RB_FIGHT_TEAM, 0, pKillerId->valueint, pBeKillerId->valueint, pPointX->valueint, pPointX->valueint))
						{
							//被击杀者队伍中广播
							pBrbt->BroadcastTeamMsg(&msg);
							//击杀者队伍中广播
							pKillerBrbt->BroadcastTeamMsg(&msg);
						}
					}

					//如果活动已经结束则不去给他们加积分
					if (pBrbt->GetTeamStatus() == _TEAM_STATUS_END ||
						pKillerBrbt->GetTeamStatus() == _TEAM_STATUS_END)
					{
						return PACKET_EXE_CONTINUE;
					}

					//获得被击杀者积分数、击杀等级、击杀数
					UINT nBeKillerCarLevel = pBeKiller->GetFightLevel();	
					pBeKiller->SetFightLevel(_FIGHTTEAM_DEFAULT_CAR_LEVEL_, FALSE);						
					INT nRatio = _FIGHTTEAM_KILL_AWARD_RATIO_;

					//找出击杀者
					Player* pMemberKiller = pKillerBrbt->GetMember(nKillerId);
					UINT nKillerCarLevel = _FIGHTTEAM_DEFAULT_CAR_LEVEL_;
					if(pMemberKiller)
					{
						nKillerCarLevel = (pMemberKiller->GetFightLevel()+ nBeKillerCarLevel) > _FIGHTTEAM_MAX_CAR_LEVEL_ ? _FIGHTTEAM_MAX_CAR_LEVEL_ : (pMemberKiller->GetFightLevel() + nBeKillerCarLevel);
						
						//更新击杀者和被击杀者车的等级数据：击杀者车等级+=被击杀者车等级 被击杀者车等级=1								
						pMemberKiller->SetFightLevel(nKillerCarLevel, FALSE);

						LOGDEBUG("[CMsgBattle::Process]-1 pMemberKiller=[%d]", pMemberKiller->GetKillSum());

						//击杀数累计到玩家的总的击杀数中
						pMemberKiller->SetKillSum(pMemberKiller->GetKillSum() + 1, FALSE);

						LOGDEBUG("[CMsgBattle::Process]-2 pMemberKiller=[%d]", pMemberKiller->GetKillSum());

						//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成10次击杀<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
						if (!pMemberKiller->TriggerDayTaskDetail(DAY_TASK_DETAIL_TYPE_3))
						{
							LOGERROR("[CMsgBattle::Process] TriggerDayTaskDetail Is Error");
						}
						//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成10次击杀<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
						//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
						if (!pMemberKiller->QueryMultiKillTimer().IsActive())
						{
							pMemberKiller->QueryMultiKillTimer().Startup(8);
							pMemberKiller->SetMultiKillCount(pMemberKiller->GetMultiKillCount() + 1);
						}
						else
						{
							pMemberKiller->QueryMultiKillTimer().Startup(8);
							pMemberKiller->SetMultiKillCount(pMemberKiller->GetMultiKillCount() + 1);
							if (pMemberKiller->GetMultiKillCount() == 2)
							{
								//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成1次双杀<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
								if (!pMemberKiller->TriggerDayTaskDetail(DAY_TASK_DETAIL_TYPE_4))
								{
									LOGERROR("[CMsgBattle::Process] TriggerDayTaskDetail Is Error");
								}
								//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成1次双杀<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
							}
							if (pMemberKiller->GetMultiKillCount() == 3)
							{
								//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成1次三杀<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
								if (!pMemberKiller->TriggerDayTaskDetail(DAY_TASK_DETAIL_TYPE_5))
								{
									LOGERROR("[CMsgBattle::Process] TriggerDayTaskDetail Is Error");
								}
								//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成1次三杀<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
							}
						}
						//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//	
					}

					//更新击杀者队伍总积分数
					pKillerBrbt->UpdateFightScore(nBeKillerCarLevel * nRatio);

					//检查战役已经打完
					if(pKillerBrbt->IsFightTeamFinsh())	//打完	
					{
						//奖励金币
						UINT nAwardGoldR = BattleCfg()->GetBcftDefaultAwardGold();
						UINT nAwardGoldB = BattleCfg()->GetBcftDefaultAwardGold();
						if (pKillerBrbt->GetFightResult() == _FIGHT_RESULT_WIN)
						{
							nAwardGoldR = BattleCfg()->GetBcftWinAwardGold();
							nAwardGoldB = BattleCfg()->GetBcftDefaultAwardGold();
						}
						else if(pKillerBrbt->GetFightResult() == _FIGHT_RESULT_DRAW)
						{
							nAwardGoldR = BattleCfg()->GetBcftDrawAwardGold();
							nAwardGoldB = BattleCfg()->GetBcftDrawAwardGold();
						}
						else//(pKillerBrbt->GetFightResult() == _FIGHT_RESULT_DEFEAT)
						{
							nAwardGoldR = BattleCfg()->GetBcftDefaultAwardGold();
							nAwardGoldB = BattleCfg()->GetBcftWinAwardGold();
						}

						//规定红队的结果在第一个段中蓝队的结果在第二个段中
						//红队,蓝队
						INT nRTeamid = pBrbt->GetTeamId();
						INT nBTeamid = pKillerBrbt->GetTeamId();
						INT nRFightResult =  pBrbt->GetFightResult();
						INT nBFightResult = pKillerBrbt->GetFightResult();
						INT nRFightTotalScore = pBrbt->GetFightTotalScore();
						INT nBFightTotalScore = pKillerBrbt->GetFightTotalScore();
						if(_FIGHT_OWNER_COLOUR_BLUE == pBrbt->GetFightTeamColour())
						{
							nRTeamid = pKillerBrbt->GetTeamId();
							nBTeamid = pBrbt->GetTeamId();
							nRFightResult =  pKillerBrbt->GetFightResult();
							nBFightResult = pBrbt->GetFightResult();
							nRFightTotalScore = pKillerBrbt->GetFightTotalScore();
							nBFightTotalScore = pBrbt->GetFightTotalScore();
						}	

						//同步战争结果信息
						CMsgBattle msg;
						IF_OK(msg.CreateMsg(_BATTLE_ACTION_TEAM_END_SUC, _BATTLE_MODEL_RB_FIGHT_TEAM, nRTeamid, nRFightResult, 
							nRFightTotalScore, nAwardGoldR, nAwardGoldB, nBTeamid, nBFightResult, nBFightTotalScore))
						{
							//被击杀者队伍中广播
							pBrbt->BroadcastTeamMsg(&msg);
							//击杀者队伍中广播
							pKillerBrbt->BroadcastTeamMsg(&msg);
						}

						//发放奖励
					    pBrbt->AwardFightTeam();
					    //发送战斗结束排行榜数据包
					    //pBrbt->BroadcastFightTeamEndMsgToMember();

					    //重置玩家队伍指针为空
					    pBrbt->ClearMember();
					    
					    //从队伍管理器中删除2只队伍
					    pBattle->DelTeamFromBrbt(pBrbt);

					    //找出对战队伍指针
				        //发放奖励
				        pKillerBrbt->AwardFightTeam();
				        //发送战斗结束排行榜数据包
				        //pKillerBrbt->BroadcastFightTeamEndMsgToMember();

				        //重置玩家队伍指针为空
					    pKillerBrbt->ClearMember();

				        //从队伍管理器中删除2只队伍
				        pBattle->DelTeamFromBrbt(pKillerBrbt);
        
					}
					else	//未结束
					{
						///////////////////////////////////////////////////////////////////////
			            ///;广播同步帧缓存66ms 0:不缓存 1:缓存
			            ///*****注意：这里只把玩家id和该玩家的车辆等级提出来，放入缓存队列中。其他的照常立即同步广播
			            if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 1)
			            {
							//插入服务器消息队列缓存
							//被击杀者信息包
							if (pMemberKiller)
							{
								pKillerBrbt->InsertFrameMsgCache(_BATTLE_MODEL_RB_FIGHT_TEAM, _WALK_ACTION_TEAM_CAR_LEV_CHANGE,  pMemberKiller->GetUid(), nKillerCarLevel, 0, 0, 0, 0, 0, 0, 0, 0, 0);
							}

							//被击杀者-死亡包-插入到被击杀者的队伍帧缓存队列
							pBrbt->InsertFrameMsgCache(_BATTLE_MODEL_RB_FIGHT_TEAM, _WALK_ACTION_KILL, pBeKiller->GetUid(), nKillerId, 0, 0, 0, 0, 0, 0, 0, 0, 0);							
						}
						//else
						{	
							//同步击杀者车辆等级、队伍总积分给2个队伍玩家
							CMsgBattle msg;
							IF_OK(msg.CreateMsg(_BATTLE_ACTION_TEAM_SYN_SCORE_SUC, _BATTLE_MODEL_RB_FIGHT_TEAM, pKillerBrbt->GetTeamId(), pMemberKiller == NULL ? 0 : pMemberKiller->GetUid(), 
								pKillerBrbt->GetFightTotalScore(), nKillerCarLevel, 0, 0, 0, 0, 0))
							{
								//被击杀者队伍中广播
								pBrbt->BroadcastTeamMsg(&msg);
								//击杀者队伍中广播
								pKillerBrbt->BroadcastTeamMsg(&msg);
							}
						}
					}
				}
#endif
				///--------方案一:第一阶段是自己上报死亡包，但是自己的网络出现网络不好的情况则还是会影响到其他剩余玩家的渲染----------------------///
#if 0
				cJSON* pKillerId = cJSON_GetObjectItem(pJsonMsg, "killer_id");			//玩家killer_id，自杀的情况传过来：0
				cJSON* pPointX = cJSON_GetObjectItem(pJsonMsg, "point_x");				//死亡坐标像素x
				cJSON* pPointY = cJSON_GetObjectItem(pJsonMsg, "point_y");				//死亡坐标像素y
				cJSON* pTS = cJSON_GetObjectItem(pJsonMsg, "ts");						// int //服务器数据包时间帧序列
				CHECKF(pKillerId && pPointX && pPointY && pTS);

				//add by huyf 2018.11.20:修改为自己上传死亡帧消息---->自己的客户端也上传别的玩家的死亡消息[第二套方案]
				//服务器做死亡帧缓存，通过同步到到前端的时间戳来确认，服务器过滤后传上的信息包。（需要前端确保相同起点，处理相同的时序包后，结果相同）
				//判断是否超过指定长度，超过则删除之前的碰撞缓存数据
				//打印下日志
				// INT nIndex = 0;
				// MAP_DEAD_CIT iter = m_pPlayer->QueryDeadSet().begin();
				// for (; iter != m_pPlayer->QueryDeadSet().end() ; ++iter)
				// {
				// 	LOGDEBUG("[CMsgBattle::Process] m_MapDeadSet-1 key==>[%d]==>[%s]", ++nIndex, iter->first.c_str());
				// }

				size_t nSize = m_pPlayer->QueryDeadSet().size();
				if (nSize > (size_t)g_config.ONLINE_MAP_DEAD_SIZE)
				{	
					LOGDEBUG("[CMsgBattle::Process] m_MapDeadSet-1 Erase Is Ok...Key=[%s] nSize=[%d]", m_pPlayer->QueryDeadSet().begin()->first.c_str() ,nSize);
					m_pPlayer->QueryDeadSet().erase(m_pPlayer->QueryDeadSet().begin());  
				}
				LOGDEBUG("[CMsgBattle::Process] m_MapDeadSet-1 nSize=[%d]", m_pPlayer->QueryDeadSet().size());

				UINT nTs = pTS->valueint;
				UINT nPlayerId = m_pPlayer->GetUid();
				UINT nKillerId = pKillerId->valueint;
				std::string strKey = std::to_string(nTs) + "_" +std::to_string(nPlayerId) + "_" + std::to_string(nKillerId);

				//记录玩家的击杀信息
				if (m_pPlayer->QueryDeadSet().find(strKey.c_str()) != m_pPlayer->QueryDeadSet().end())
				{
					LOGDEBUG("[CMsgBattle::Process] m_MapDeadSet-1 Have Report... strKey=[%s] pTS=[%d] nPlayerId=[%d] nKillerId=[%d] pPointX=[%d] pPointY=[%d]", strKey.c_str(), pTS->valueint, nPlayerId, nKillerId, pPointX->valueint, pPointY->valueint);
					return PACKET_EXE_CONTINUE;
				}

				m_pPlayer->QueryDeadSet().insert(pair<std::string, UINT>(strKey.c_str(), nTs));
				LOGDEBUG("[CMsgBattle::Process] m_MapDeadSet-1 Insert Is OK... strKey=[%s] pTS=[%d] nPlayerId=[%d] nKillerId=[%d] pPointX=[%d] pPointY=[%d]", strKey.c_str(), pTS->valueint, nPlayerId, nKillerId, pPointX->valueint, pPointY->valueint);

				Player* pKiller = g_pPlayerPool->GetPlayerByUid(nKillerId);
				if (pKiller)
				{	
					strKey = std::to_string(nTs) + "_" +std::to_string(nKillerId) + "_" + std::to_string(nPlayerId);
					size_t nSize = pKiller->QueryDeadSet().size();
					if (nSize > (size_t)g_config.ONLINE_MAP_DEAD_SIZE)
					{	
						LOGDEBUG("[CMsgBattle::Process] m_MapDeadSet-2 Erase Is Ok...nSize=[%d]", nSize);
						pKiller->QueryDeadSet().erase(pKiller->QueryDeadSet().begin());  
					}
					LOGDEBUG("[CMsgBattle::Process] m_MapDeadSet-2 nSize=[%d]", nSize);

					//打印下日志
					// INT nIndex = 0;
					// MAP_DEAD_CIT iter = pKiller->QueryDeadSet().begin();
					// for (; iter != pKiller->QueryDeadSet().end() ; ++iter)
					// {
					// 	LOGDEBUG("[CMsgBattle::Process] m_MapDeadSet-2 key==>[%d]==>[%s]", ++nIndex, iter->first.c_str());
					// }	

					if (pKiller->QueryDeadSet().find(strKey.c_str()) != pKiller->QueryDeadSet().end())
					{
						LOGDEBUG("[CMsgBattle::Process] m_MapDeadSet-2 Have Report... strKey=[%s] pTS=[%d] nPlayerId=[%d] nKillerId=[%d] pPointX=[%d] pPointY=[%d]", strKey.c_str(), pTS->valueint, nPlayerId, nKillerId, pPointX->valueint, pPointY->valueint);
						return PACKET_EXE_CONTINUE;
					}
					pKiller->QueryDeadSet().insert(pair<std::string, UINT>(strKey.c_str(), nTs));
					LOGDEBUG("[CMsgBattle::Process] m_MapDeadSet-2 Insert Is OK... strKey=[%s] pTS=[%d] nPlayerId=[%d] nKillerId=[%d] pPointX=[%d] pPointY=[%d]", strKey.c_str(), pTS->valueint, nPlayerId, nKillerId, pPointX->valueint, pPointY->valueint);
				}
				//add end:修改为自己上传死亡帧消息---->自己的客户端也上传别的玩家的死亡消息

				//设置死亡状态
				m_pPlayer->SetDeadStatus(_STATUS_DEAD);
				LOGDEBUG("[CMsgBattle::Process] Playerid=[%d] DeadStatus=[ _STATUS_DEAD ]", m_pPlayer->GetUid());

				//设置碰撞操作包开始
				if (m_pPlayer->GetSocket())
				{
					long long llCurMillSecTime = GetMillSecTime();
					m_pPlayer->GetSocket()->SetCollideSynMsgCount(0);
					m_pPlayer->GetSocket()->SetCollideSynMsgTimeStamp(llCurMillSecTime);
					m_pPlayer->GetSocket()->SetSynMsgStatus(_SYN_MSG_PREVIOUS);
					LOGDEBUG("[CMsgBattle::Process] Playerid=[%d] SynMsgStatus=[ _SYN_MSG_PREVIOUS ]", m_pPlayer->GetUid());
				}

				//大乱斗模式
				if (pModel->valueint == _BATTLE_MODEL_CONFUSION)
				{	
					Player* pBekiller = m_pPlayer;
					pBt = pBekiller->GetBct();
					if(!pBt)
					{
						return PACKET_EXE_CONTINUE;
					}
				
					//!alter by huyf 2018.12.03:击杀包插入缓存帧队列
					//***注意:不插入的话，会出现玩家A 后进入游戏，玩家B和玩家C、玩家D早进入游戏，并且玩家B和玩家C 玩家B和玩家D 发生了碰撞，当玩家A进入的时候玩家B处于死亡状态，则
					//前端在对玩家C和玩家D进行碰撞处理检测的时候，判定此时玩家B已经死亡状态，则不会和C发生碰撞，那么玩家C就没有进行碰撞后的抛帧动作，就会多执行几帧，导致不同步问题。
					//如果玩家B此时或者，则不受影响，该检测的碰撞都会检测到，玩家A客户端上玩家C的表现和玩家D的渲染都正常可以处理。
					//玩家B的处理情况则为，玩家A进入的时候玩家B是活着状态的，玩家B会把收到的玩家B的死亡帧全部抛掉
					//***
					//前端不再需要此包，
					if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 0)
		            {
						//广播碰撞信息给全队人员,播放碰撞特效
						CMsgBattle msg1;
						IF_OK(msg1.CreateMsg(_BATTLE_ACTION_BATTLE_KILL_SUC, _BATTLE_MODEL_CONFUSION, 0, pKillerId->valueint, pBekiller->GetUid(), pPointX->valueint, pPointY->valueint))
						{
							pBt->BroadcastTeamMsg(&msg1);
						}
					}
					//!alter end:击杀包插入缓存帧队列

					//如果活动已经结束则不去给他们加积分
					if (pBt->GetTeamStatus() == _TEAM_STATUS_END)
					{
						return PACKET_EXE_CONTINUE;
					}

					//获得被击杀者积分数、击杀等级、击杀数
					UINT nFightKillSum = pBekiller->GetFightKillSum();
					UINT nFightScore = pBekiller->GetFightScore();
					UINT nFightLevel = pBekiller->GetFightLevel();

					//设置玩家死亡时积分
					pBekiller->SetDeadScore(nFightScore, FALSE);

					//击杀玩家可以获得玩家50%的积分,
   					//最后击杀你的玩家,会被附加复仇光环,击杀复仇光环的玩家可获得70%的积分.
					float fRatio = BattleCfg()->GetBccKillAwardScoreRatio() * 0.1;//0.5;
					if (pBekiller->GetBeKiller())
					{
						if ((UINT)nKillerId == pBekiller->GetBeKiller()->GetUid())
						{
							 fRatio = BattleCfg()->GetBccRevengeAwardKillScoreRatio() * 0.1;
						}
					}

					UINT nKillSum = 0;
					UINT nSorce = 0;
					UINT nLevel =0;
					UINT nSorceTotal = 0;
					UINT nCfgLevel = 0;

					//更新被击杀顽疾的积分,如果小于1000+3000（基础积分） 则设置为4000
					INT nScore = _CONFUSION_BASE_FIGHT_SCORE + pBekiller->GetAddScore();
					pBekiller->SetFightScore(nScore, FALSE);
					nCfgLevel = pBekiller->GetFightCfgLevel(pBekiller->GetFightScore());
					pBekiller->SetFightLevel(nCfgLevel, FALSE);

					LOGDEBUG("[CMsgBattle::Process] pBekiller nFightKillSum=[%d] nFightScore=[%d] nFightLevel=[%d] nScore=[%d]\n", nFightKillSum, nFightScore, nFightLevel, nScore);

					//获得队伍中击杀者
					Player* pMemberKiller = pBt->GetMember(nKillerId);
					if(pMemberKiller && pBekiller != pMemberKiller)
					{
						nKillSum = pMemberKiller->GetFightKillSum();
						nSorce = pMemberKiller->GetFightScore();
						nLevel = pMemberKiller->GetFightLevel();
						LOGDEBUG("[CMsgBattle::Process] pMemberKiller nFightKillSum=[%d] nFightScore=[%d] nFightLevel=[%d]\n", nFightKillSum, nFightScore, nFightLevel);

						//更新击杀者数据：击杀数、击杀积分、击杀等级
						pMemberKiller->SetFightKillSum(nKillSum + 1, FALSE);
						nSorceTotal = nSorce + INT(nFightScore * fRatio);
						pMemberKiller->SetFightScore(nSorceTotal, FALSE);
						nCfgLevel = pMemberKiller->GetFightCfgLevel(nSorceTotal);
						if(nCfgLevel != nLevel)
						{
							pMemberKiller->SetFightLevel(nCfgLevel, FALSE);
						}

						LOGDEBUG("[CMsgBattle::Process] pMemberKiller nFightKillSum=[%d] nFightScore=[%d] nFightLevel=[%d]\n", 
							pMemberKiller->GetFightKillSum(), pMemberKiller->GetFightScore(), pMemberKiller->GetFightLevel());

						//设置击杀者击杀玩家的指针
						pMemberKiller->SetBeKiller(pBekiller);
						//设置击杀者id
						pBekiller->SetEnemyId(nKillerId, FALSE);

						LOGDEBUG("[CMsgBattle::Process]-1 pMemberKiller=[%d]", pMemberKiller->GetKillSum());

						//击杀数累计到玩家的总的击杀数中
						pMemberKiller->SetKillSum(pMemberKiller->GetKillSum() + 1, FALSE);

						LOGDEBUG("[CMsgBattle::Process]-2 pMemberKiller=[%d]", pMemberKiller->GetKillSum());
					}

					//更新霸主玩家
					INT nKingID = 0;
					pBt->UpdateKingPlayer();
					if (pBt->GetKingPlayer())
					{
						nKingID = pBt->GetKingPlayer()->GetUid();
					}

					///////////////////////////////////////////////////////////////////////
		            ///;广播同步帧缓存66ms 0:不缓存 1:缓存
		            ///*****注意：这里只把玩家id和该玩家的车辆等级提出来，放入缓存队列中。其他的照常立即同步广播
		            if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 1)
		            {
						//插入服务器消息队列缓存
						//被击杀者信息包-车辆等级变化包
						pBt->InsertFrameMsgCache(_BATTLE_MODEL_CONFUSION, _WALK_ACTION_CONFUSION_CAR_LEV_CHANGE, pBekiller->GetUid(), pBekiller->GetFightLevel(), 0, 0, 0, 0, 0, 0, 0, 0, 0);
						if(pMemberKiller)
						{
							//击杀者信息包-车辆等级变化包
							pBt->InsertFrameMsgCache(_BATTLE_MODEL_CONFUSION, _WALK_ACTION_CONFUSION_CAR_LEV_CHANGE, pMemberKiller->GetUid(), pMemberKiller->GetFightLevel(), 0, 0, 0, 0, 0, 0, 0, 0, 0);
						}
						//被击杀者死亡包-插入帧缓存队列
						//增加本次击杀奖励金币字段，用于前端每次击杀界面中间广播显示。
						INT nAwardGold = 0;
						if (pMemberKiller)
						{
							nAwardGold = (1 + pMemberKiller->GetAddGold())*BattleCfg()->GetBccPerKillAwardGold();
						}
						pBt->InsertFrameMsgCache(_BATTLE_MODEL_CONFUSION, _WALK_ACTION_KILL, pBekiller->GetUid(), nKillerId, nAwardGold, INT(nFightScore * fRatio), 0, 0, 0, 0, 0, 0, 0);
					}
					//else
					{	
						//广播被击杀玩家数据
						CMsgBattle msg2;
						IF_OK(msg2.CreateMsg(_BATTLE_ACTION_BATTLE_SYN_SCORE, _BATTLE_MODEL_CONFUSION, 0, pBekiller->GetUid(), 
							0, 0, 0, pBekiller->GetFightKillSum(), pBekiller->GetFightScore(), pBekiller->GetFightLevel(), nKingID))
						{
							pBt->BroadcastTeamMsg(&msg2);
						}

						if(pMemberKiller)
						{
							//队伍中同步击杀者数据
							CMsgBattle msg;
							IF_OK(msg.CreateMsg(_BATTLE_ACTION_BATTLE_SYN_SCORE, _BATTLE_MODEL_CONFUSION, 0, pMemberKiller->GetUid(), 
								0, 0, 0, pMemberKiller->GetFightKillSum(), pMemberKiller->GetFightScore(), pMemberKiller->GetFightLevel(), nKingID))
							{
								pBt->BroadcastTeamMsg(&msg);
							}	
						}
					}

					return PACKET_EXE_CONTINUE;
				}
				//红蓝队对战模式
				else if (pModel->valueint == _BATTLE_MODEL_RB_FIGHT_TEAM)
				{

					Player* pBekiller = m_pPlayer;
					CBattleTeam* pBrbt = pBekiller->GetBrbt();
					if(!pBrbt)
					{
						return PACKET_EXE_CONTINUE;
					}

					//碰碰车初始等级为1级,击杀对方队伍的碰碰车,增加相应碰碰车的等级(击杀本方队伍无效)
					//在本队伍中找到该玩家说明是本队伍玩家
					//if(pBrbt->GetMember(nKillerId))
					//{
					//	return PACKET_EXE_CONTINUE;
					//}
					
					//找出击杀者队伍指针
					CBattleTeam* pKillerBrbt = pBrbt->GetEnemyTeam();
					if(!pKillerBrbt)
					{
						return PACKET_EXE_CONTINUE;
					}

					if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 0)
		            {
						//广播碰撞信息给全队人员,播放碰撞特效
						CMsgBattle msg;
						IF_OK(msg.CreateMsg(_BATTLE_ACTION_BATTLE_KILL_SUC, _BATTLE_MODEL_RB_FIGHT_TEAM, 0, nKillerId, pBekiller->GetUid(), pPointX->valueint, pPointX->valueint))
						{
							//被击杀者队伍中广播
							pBrbt->BroadcastTeamMsg(&msg);
							//击杀者队伍中广播
							pKillerBrbt->BroadcastTeamMsg(&msg);
						}
					}

					//如果活动已经结束则不去给他们加积分
					if (pBrbt->GetTeamStatus() == _TEAM_STATUS_END ||
						pKillerBrbt->GetTeamStatus() == _TEAM_STATUS_END)
					{
						return PACKET_EXE_CONTINUE;
					}

					//获得被击杀者积分数、击杀等级、击杀数
					UINT nBeKillerCarLevel = pBekiller->GetFightLevel();	
					pBekiller->SetFightLevel(_FIGHTTEAM_DEFAULT_CAR_LEVEL_, FALSE);						
					INT nRatio = _FIGHTTEAM_KILL_AWARD_RATIO_;

					//找出击杀者
					Player* pMemberKiller = pKillerBrbt->GetMember(nKillerId);
					UINT nKillerCarLevel = _FIGHTTEAM_DEFAULT_CAR_LEVEL_;
					if(pMemberKiller)
					{
						nKillerCarLevel = (pMemberKiller->GetFightLevel()+ nBeKillerCarLevel) > _FIGHTTEAM_MAX_CAR_LEVEL_ ? _FIGHTTEAM_MAX_CAR_LEVEL_ : (pMemberKiller->GetFightLevel() + nBeKillerCarLevel);
						
						//更新击杀者和被击杀者车的等级数据：击杀者车等级+=被击杀者车等级 被击杀者车等级=1								
						pMemberKiller->SetFightLevel(nKillerCarLevel, FALSE);

						LOGDEBUG("[CMsgBattle::Process]-1 pMemberKiller=[%d]", pMemberKiller->GetKillSum());

						//击杀数累计到玩家的总的击杀数中
						pMemberKiller->SetKillSum(pMemberKiller->GetKillSum() + 1, FALSE);

						LOGDEBUG("[CMsgBattle::Process]-2 pMemberKiller=[%d]", pMemberKiller->GetKillSum());
					}

					//更新击杀者队伍总积分数
					pKillerBrbt->UpdateFightScore(nBeKillerCarLevel * nRatio);

					//检查战役已经打完
					if(pKillerBrbt->IsFightTeamFinsh())	//打完	
					{
						//奖励金币
						UINT nAwardGoldR = BattleCfg()->GetBcftDefaultAwardGold();
						UINT nAwardGoldB = BattleCfg()->GetBcftDefaultAwardGold();
						if (pKillerBrbt->GetFightResult() == _FIGHT_RESULT_WIN)
						{
							nAwardGoldR = BattleCfg()->GetBcftWinAwardGold();
							nAwardGoldB = BattleCfg()->GetBcftDefaultAwardGold();
						}
						else if(pKillerBrbt->GetFightResult() == _FIGHT_RESULT_DRAW)
						{
							nAwardGoldR = BattleCfg()->GetBcftDrawAwardGold();
							nAwardGoldB = BattleCfg()->GetBcftDrawAwardGold();
						}
						else//(pKillerBrbt->GetFightResult() == _FIGHT_RESULT_DEFEAT)
						{
							nAwardGoldR = BattleCfg()->GetBcftDefaultAwardGold();
							nAwardGoldB = BattleCfg()->GetBcftWinAwardGold();
						}

						//规定红队的结果在第一个段中蓝队的结果在第二个段中
						//红队,蓝队
						INT nRTeamid = pBrbt->GetTeamId();
						INT nBTeamid = pKillerBrbt->GetTeamId();
						INT nRFightResult =  pBrbt->GetFightResult();
						INT nBFightResult = pKillerBrbt->GetFightResult();
						INT nRFightTotalScore = pBrbt->GetFightTotalScore();
						INT nBFightTotalScore = pKillerBrbt->GetFightTotalScore();
						if(_FIGHT_OWNER_COLOUR_BLUE == pBrbt->GetFightTeamColour())
						{
							nRTeamid = pKillerBrbt->GetTeamId();
							nBTeamid = pBrbt->GetTeamId();
							nRFightResult =  pKillerBrbt->GetFightResult();
							nBFightResult = pBrbt->GetFightResult();
							nRFightTotalScore = pKillerBrbt->GetFightTotalScore();
							nBFightTotalScore = pBrbt->GetFightTotalScore();
						}	

						//同步战争结果信息
						CMsgBattle msg;
						IF_OK(msg.CreateMsg(_BATTLE_ACTION_TEAM_END_SUC, _BATTLE_MODEL_RB_FIGHT_TEAM, nRTeamid, nRFightResult, 
							nRFightTotalScore, nAwardGoldR, nAwardGoldB, nBTeamid, nBFightResult, nBFightTotalScore))
						{
							//被击杀者队伍中广播
							pBrbt->BroadcastTeamMsg(&msg);
							//击杀者队伍中广播
							pKillerBrbt->BroadcastTeamMsg(&msg);
						}

						//发放奖励
					    pBrbt->AwardFightTeam();
					    //发送战斗结束排行榜数据包
					    //pBrbt->BroadcastFightTeamEndMsgToMember();

					    //重置玩家队伍指针为空
					    pBrbt->ClearMember();
					    
					    //从队伍管理器中删除2只队伍
					    pBattle->DelTeamFromBrbt(pBrbt);

					    //找出对战队伍指针
				        //发放奖励
				        pKillerBrbt->AwardFightTeam();
				        //发送战斗结束排行榜数据包
				        //pKillerBrbt->BroadcastFightTeamEndMsgToMember();

				        //重置玩家队伍指针为空
					    pKillerBrbt->ClearMember();

				        //从队伍管理器中删除2只队伍
				        pBattle->DelTeamFromBrbt(pKillerBrbt);
        
					}
					else	//未结束
					{
						///////////////////////////////////////////////////////////////////////
			            ///;广播同步帧缓存66ms 0:不缓存 1:缓存
			            ///*****注意：这里只把玩家id和该玩家的车辆等级提出来，放入缓存队列中。其他的照常立即同步广播
			            if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 1)
			            {
							//插入服务器消息队列缓存
							//被击杀者信息包
							if (pMemberKiller)
							{
								pKillerBrbt->InsertFrameMsgCache(_BATTLE_MODEL_RB_FIGHT_TEAM, _WALK_ACTION_TEAM_CAR_LEV_CHANGE,  pMemberKiller->GetUid(), nKillerCarLevel, 0, 0, 0, 0, 0, 0, 0, 0, 0);
							}

							//被击杀者-死亡包-插入到被击杀者的队伍帧缓存队列
							pBrbt->InsertFrameMsgCache(_BATTLE_MODEL_RB_FIGHT_TEAM, _WALK_ACTION_KILL, pBekiller->GetUid(), nKillerId, 0, 0, 0, 0, 0, 0, 0, 0, 0);							
						}
						//else
						{	
							//同步击杀者车辆等级、队伍总积分给2个队伍玩家
							CMsgBattle msg;
							IF_OK(msg.CreateMsg(_BATTLE_ACTION_TEAM_SYN_SCORE_SUC, _BATTLE_MODEL_RB_FIGHT_TEAM, pKillerBrbt->GetTeamId(), pMemberKiller == NULL ? 0 : pMemberKiller->GetUid(), 
								pKillerBrbt->GetFightTotalScore(), nKillerCarLevel, 0, 0, 0, 0, 0))
							{
								//被击杀者队伍中广播
								pBrbt->BroadcastTeamMsg(&msg);
								//击杀者队伍中广播
								pKillerBrbt->BroadcastTeamMsg(&msg);
							}
						}
					}
				}	
#endif
				return PACKET_EXE_CONTINUE;
			}
			break;						
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			///战斗中复活
			case _BATTLE_ACTION_BATTLE_RELIVE:
			{
				//大乱斗模式
				if (pModel->valueint == _BATTLE_MODEL_CONFUSION)
				{
					cJSON* pType = cJSON_GetObjectItem(pJsonMsg, "type");					// int 复活类型：0:普通复活 1：看视频复活
					//cJSON* pPointX= cJSON_GetObjectItem(pJsonMsg, "point_x");				// int 坐标x
					//cJSON* pPointY = cJSON_GetObjectItem(pJsonMsg, "point_y");				// int 坐标y
					CHECKF(pType);

					//设置准备活着状态
					m_pPlayer->SetDeadStatus(_STATUS_PREPARE_ALIVE);

					//如果玩家3 2 1 倒计时还未结束就发送了操作包，则丢弃
					//m_pPlayer->GetDeadTimeInterval().Update();					
					LOGDEBUG("[CMsgBattle::Process] _BATTLE_ACTION_BATTLE_RELIVE Playerid=[%d] DeadStatus=[ _STATUS_PREPARE_ALIVE ] pType=[%d]", m_pPlayer->GetUid(), pType->valueint);

					//战斗已经结束，直接处理下一个消息
					pBt = m_pPlayer->GetBct();
					if(!pBt)
					{
						return PACKET_EXE_CONTINUE;
					}

					//1.被击杀的玩家,可以选择观看视频保留50%的积分复活.否则普通复活从1000积分开始(有BUFF的则从4000开始), 复活为随机复活点复活	
					//普通复活
					INT nCfgLevel = 1;
					if (pType->valueint == _RELIVE_TYPE_NORMAL)
					{
						//初始化玩家数据
						//更新被击杀者数据：击杀数、击杀积分、击杀等级
						FLOAT fRatio = BattleCfg()->GetBccNormalReliveRemainScoreRatio() * 0.1;
						INT nSorceTotal = _CONFUSION_BASE_FIGHT_SCORE * fRatio;
						m_pPlayer->SetFightScore(nSorceTotal + m_pPlayer->GetAddScore(), FALSE);
						nCfgLevel = m_pPlayer->GetFightCfgLevel(nSorceTotal);
						m_pPlayer->SetFightLevel(nCfgLevel, FALSE);

						LOGDEBUG("[CMsgBattle::Process] _RELIVE_TYPE_NORMAL nCurTotalScore=[%d] nCfgLevel=[%d]\n", m_pPlayer->GetFightScore(), nCfgLevel);
					}
					//看视频复活
					else if (pType->valueint == _RELIVE_TYPE_VIEW)
					{
						//初始化玩家数据
						//获取玩家死亡时积分
						INT nDeadScore = m_pPlayer->GetDeadScore();
						FLOAT fRatio = BattleCfg()->GetBccViewReliveRemainScoreRatio() * 0.1;//0.5;

						//更新被击杀者数据：击杀数、击杀积分、击杀等级
						INT nSorceTotal = INT((nDeadScore * fRatio) < (_CONFUSION_BASE_FIGHT_SCORE + m_pPlayer->GetAddScore()) ? (_CONFUSION_BASE_FIGHT_SCORE + m_pPlayer->GetAddScore()) : (nDeadScore * fRatio));
						m_pPlayer->SetFightScore(nSorceTotal, FALSE);
						nCfgLevel = m_pPlayer->GetFightCfgLevel(nSorceTotal);
						m_pPlayer->SetFightLevel(nCfgLevel, FALSE);

						LOGDEBUG("[CMsgBattle::Process] _RELIVE_TYPE_VIEW nDeadScore=[%d] nSorceTotal=[%d] nCfgLevel=[%d]\n", nDeadScore, m_pPlayer->GetFightScore(), nCfgLevel);
					}
					else
					{
						return PACKET_EXE_ERROR;
					}

					//更新霸主玩家
					INT nKingID = 0;
					pBt->UpdateKingPlayer();
					if (pBt->GetKingPlayer())
					{
						nKingID = pBt->GetKingPlayer()->GetUid();
					}

					//放到缓存帧中不再立即广播
					//if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 0)
	            	//{
						//广播死亡特效信息给全队人员
						INT nRegionId = Random(_CONFUSION_REILVE_REGION_RANGDOM);
						INT nDirectionX = Random(_CONFUSION_DIRECTION_X_RANGDOM);
						INT nDirectionY = Random(_CONFUSION_DIRECTION_Y_RANGDOM);
						CMsgBattle msg1;
						IF_OK(msg1.CreateMsg(_BATTLE_ACTION_BATTLE_RELIVE_SUC, _BATTLE_MODEL_CONFUSION, nDirectionX, m_pPlayer->GetUid(), nDirectionY, nRegionId, 0))
						{
							pBt->BroadcastTeamMsg(&msg1);
						}
					// }
					// else
					// {
					// 	//插入服务器消息队列缓存
					// 	pBt->InsertFrameMsgCache(pModel->valueint, _WALK_ACTION_USER_CMD, nTargetId, pPointX->valueint, pPointY->valueint, pPointStartX->valueint, pPointStartY->valueint, pPointEndX->valueint, pPointEndY->valueint);
					// }

					//广播死亡玩家数据信息给全队人员
					CMsgBattle msg2;
					IF_OK(msg2.CreateMsg(_BATTLE_ACTION_BATTLE_SYN_SCORE, _BATTLE_MODEL_CONFUSION, 0, m_pPlayer->GetUid(), 
						0, 0, 0, m_pPlayer->GetFightKillSum(), m_pPlayer->GetFightScore(), m_pPlayer->GetFightLevel(), nKingID))
					{
						pBt->BroadcastTeamMsg(&msg2);
					}

                    // 发送生成道具数据包
					//pBt->CreateFightItemPos();

					///////////////////////////////////////////////////////////////////////
		            ///;广播同步帧缓存66ms 0:不缓存 1:缓存
		            if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 1)
		            {
						//插入服务器消息队列缓存
						pBt->InsertFrameMsgCache(pModel->valueint, _WALK_ACTION_CONFUSION_RELIVE, m_pPlayer->GetUid(), nDirectionX, nDirectionY, nRegionId, nCfgLevel);
					}
					else
					{	
					}

					return PACKET_EXE_CONTINUE;
				}
			}
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			///战斗中复活-[红蓝战队]
			case _BATTLE_ACTION_TEAM_RELIVE:
			{
				//大乱斗模式
				if (pModel->valueint == _BATTLE_MODEL_RB_FIGHT_TEAM)
				{
					cJSON* pRegionId= cJSON_GetObjectItem(pJsonMsg, "region_id");				// int 复活区域
					CHECKF(pRegionId);

					//设置准备活着状态
					m_pPlayer->SetDeadStatus(_STATUS_PREPARE_ALIVE);

					//如果玩家3 2 1 倒计时还未结束就发送了操作包，则丢弃
					//m_pPlayer->GetDeadTimeInterval().Update();
					
					LOGDEBUG("[CMsgBattle::Process] _BATTLE_ACTION_TEAM_RELIVE Playerid=[%d] DeadStatus=[ _STATUS_PREPARE_ALIVE ] pRegionId=[%d]", m_pPlayer->GetUid(), pRegionId->valueint);

					//战斗已经结束，直接处理下一个消息
					pBt = m_pPlayer->GetBrbt();
					if(!pBt)
					{
						return PACKET_EXE_CONTINUE;
					}
					
					//找出击杀者队伍指针
					CBattleTeam* pKillerBrbt = pBt->GetEnemyTeam();
					if(!pKillerBrbt)
					{
						return PACKET_EXE_CONTINUE;
					}

					//广播死亡特效信息给全队人员
					CMsgBattle msg1;
					IF_OK(msg1.CreateMsg(_BATTLE_ACTION_TEAM_RELIVE_SUC, _BATTLE_MODEL_RB_FIGHT_TEAM, _FIGHTTEAM_DEFAULT_CAR_LEVEL_, m_pPlayer->GetUid(), 0, 0, 0, 0, 0, 0, 0, pRegionId->valueint))
					{
						//被击杀者队伍中广播
						pBt->BroadcastTeamMsg(&msg1);
						//击杀者队伍中广播
						pKillerBrbt->BroadcastTeamMsg(&msg1);
					}

					///////////////////////////////////////////////////////////////////////
		            ///;广播同步帧缓存66ms 0:不缓存 1:缓存
		            if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 1)
		            {
						//插入服务器消息队列缓存
						pBt->InsertFrameMsgCache(pModel->valueint, _WALK_ACTION_TEAM_RELIVE, m_pPlayer->GetUid(), pRegionId->valueint, _FIGHTTEAM_DEFAULT_CAR_LEVEL_);
					}
					else
					{	
					}
					
					LOGDEBUG("[CMsgBattle::Process] _WALK_ACTION_TEAM_RELIVE Playerid=[%d]", m_pPlayer->GetUid());
					return PACKET_EXE_CONTINUE;	
				}											
			}
			break;			
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			///战斗结束界面 看视频三倍奖励-[大乱斗]
			case _BATTLE_ACTION_VIEW_AWARD:
			{				
				//大乱斗模式
				if (pModel->valueint == _BATTLE_MODEL_CONFUSION)
				{
					//发放3倍奖励
					m_pPlayer->AwardThreeMulti();
						
					return PACKET_EXE_CONTINUE;
				}		
			}
			break;
  
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			///战斗结束界面 看视频三倍奖励-[红蓝对战]
			case _BATTLE_ACTION_TEAM_VIEW_AWARD:
			{				
				//红蓝组对模式
				if (pModel->valueint == _BATTLE_MODEL_RB_FIGHT_TEAM)
				{
					//发放3倍奖励
					m_pPlayer->AwardThreeMultiFightTeam();
						
					return PACKET_EXE_CONTINUE;
				}		
			}
			break;
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			///离开队伍
			case _BATTLE_ACTION_LEAVE_TEAM:
			{				
				//大乱斗模式
				if (pModel->valueint == _BATTLE_MODEL_CONFUSION)
				{
					pBt = m_pPlayer->GetBct();
					if(!pBt)
					{
						return PACKET_EXE_CONTINUE;
					}
					
					//队伍删除成员
					pBt->DelMember(m_pPlayer);

					//更新玩家队伍指针
					m_pPlayer->ClearBct();

					CMsgBattle msg1;
					if (msg1.CreateMsg(_BATTLE_ACTION_LEAVE_TEAM_SUC, _BATTLE_MODEL_CONFUSION, 0, m_pPlayer->GetUid(), 0, 0))
					{
						SendMsg(&msg1);
					}

					//广播离开队伍信息给玩家
		            CMsgSynRoomInfo msg2;
		            IF_OK(msg2.CreateMsg(m_pPlayer, _SYNROOMINFO_ACTION_DEL_MEMBER, pBt->GetTeamId(), pBt->GetFightTeamColour()))
		            {
		                pBt->BroadcastTeamMsg(&msg2);
		            }

					//如果队伍成员人数=0，则从Battle中删除此队伍
			        if ((pBt->GetTeamStatus() == _TEAM_STATUS_CREATE || pBt->GetTeamStatus() == _TEAM_STATUS_QIUCK_MATCH) && pBt->GetMemberAmount() == 0)
			        {
			            pBattle->DelTeamFromBct(pBt);
			        }

			        //如果队伍人数不为0
			        //如果是队长则进行队长id变更广播
			        INT nAmount = pBt->GetMemberAmount();
					if(nAmount > 0)
					{
						//如果是对战离开，则进行队长更换广播
						if(pBt->GetTeamLeaderID() == m_pPlayer->GetUid())
						{
							Player* pNewLeader = pBt->GetMemberByIndex(0);
							if (pNewLeader)
							{
								pBt->SetTeamLeaderID(pNewLeader->GetUid());
								CMsgBattle msg;
								IF_OK(msg.CreateMsg(_BATTLE_ACTION_CHANGE_TEAM_LEADER_SUC, 0, pBt->GetTeamId(), pNewLeader->GetUid()))
								{
									pBt->BroadcastTeamMsg(&msg);
								}
							}
							else
							{
								LOGERROR("[MsgBattle::Process] _BATTLE_ACTION_LEAVE_TEAM Is Error GetMemberByIndex[0]==NULL MemberAmount=[%d]",pBt->GetMemberAmount());
							}					
						} 

						//整合队伍的功能开启了
						if (BattleCfg()->GetBcMegerOtherTeamS() == 1)
						{
							//再次检测队列有没有可以合并的队伍
							//!alter by huyf 2018.12.07:队伍创建后开始匹配的时候，搜索整个的队伍列表，如果发现有不满人的队伍，且可以容纳此队伍的所有成员，则进行队伍整合
							CBattleTeam* pOtherTeam = NULL;
AGAIN_CHECK_COND_LEAVE_TEAM:
							nAmount = pBt->GetMemberAmount();
							if (nAmount >= (INT)BattleCfg()->GetBccMaxTeamAmount())	//满足15人，直接开始游戏
							{
								//设置游戏为: START 状态机
								pBt->FightStart();

								//广播开始游戏数据
								pBt->BroadcastConfusionStartMsgToMember();
							}
							else	//未满人数
							{
								//搜索队伍队列
								pOtherTeam = pBattle->QuerySatisfyConditionTeam(_FIGHT_MODEL_TYPE_CONFUSION, pBt->GetTeamId(), nAmount);
								//存在开始匹配的队伍
								if (pOtherTeam)
								{
									//队伍整合
									if(pBattle->MergeTeamS(_FIGHT_MODEL_TYPE_CONFUSION, pBt, pOtherTeam))
									{
										//...
										goto AGAIN_CHECK_COND_LEAVE_TEAM;
									}
									else	//整合队伍失败
									{
										LOGERROR("[CMsgBattle::Process] MergeTeamS Is Error TeamId=[%d] MemberAmount=[%d] O-TeamId=[%d] O-MemberAmount=[%d]", pBt->GetTeamId(), nAmount, pOtherTeam->GetTeamId(), pOtherTeam->GetMemberAmount());
										return PACKET_EXE_CONTINUE;
									}
								}
							}
							//!alter END:队伍创建后开始匹配的时候，搜索整个的队伍列表，如果发现有不满人的队伍，且可以容纳此队伍的所有成员，则进行队伍整合
						}
					}

					return PACKET_EXE_CONTINUE;	
				}
				//红蓝队对战模式
				else if (pModel->valueint == _BATTLE_MODEL_RB_FIGHT_TEAM)
				{
					pBt = m_pPlayer->GetBrbt();
					if(!pBt)
					{
						return PACKET_EXE_CONTINUE;
					}
					
					//队伍删除成员
					pBt->DelMember(m_pPlayer);

					//更新玩家队伍指针
					m_pPlayer->ClearBrbt();

					CMsgBattle msg1;
					if (msg1.CreateMsg(_BATTLE_ACTION_LEAVE_TEAM_SUC, _BATTLE_MODEL_RB_FIGHT_TEAM, 0, 0, 0, 0))
					{
						SendMsg(&msg1);						
					}

					//广播离开队伍信息给玩家
		            CMsgSynRoomInfo msg2;
		            IF_OK(msg2.CreateMsg(m_pPlayer, _SYNROOMINFO_ACTION_DEL_MEMBER, pBt->GetTeamId(), pBt->GetFightTeamColour()))
		            {
		                pBt->BroadcastTeamMsg(&msg2);		                
		            }

		            //找出击杀者队伍指针
					CBattleTeam* pEnemyBrbt = pBt->GetEnemyTeam();
					if(pEnemyBrbt)
					{
						pEnemyBrbt->BroadcastTeamMsg(&msg2);
					}
					
					//如果队伍成员人数=0，则从Battle中删除此队伍
			        if ((pBt->GetTeamStatus() == _TEAM_STATUS_CREATE || pBt->GetTeamStatus() == _TEAM_STATUS_QIUCK_MATCH) && pBt->GetMemberAmount() == 0)
			        {
			            pBattle->DelTeamFromBrbt(pBt);
			        } 

			        //如果队伍人数不为0
			        //如果是队长则进行队长id变更广播
					if(pBt->GetMemberAmount() > 0 && pBt->GetTeamLeaderID() == m_pPlayer->GetUid())
					{
						Player* pNewLeader = pBt->GetMemberByIndex(0);
						if (pNewLeader)
						{
							pBt->SetTeamLeaderID(pNewLeader->GetUid());
							CMsgBattle msg;
							IF_OK(msg.CreateMsg(_BATTLE_ACTION_CHANGE_TEAM_LEADER_SUC, 0, pBt->GetTeamId(), pNewLeader->GetUid()))
							{
								pBt->BroadcastTeamMsg(&msg);
							}
						}
						else
						{
							LOGERROR("[MsgBattle::Process] _BATTLE_ACTION_LEAVE_TEAM Is Error GetMemberByIndex[0]==NULL MemberAmount=[%d]",pBt->GetMemberAmount());
						}					
					} 

					return PACKET_EXE_CONTINUE;
				}		
			}
			break;
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			///加载完成-[大乱斗]
            case _BATTLE_ACTION_LOADING_COMPLETE:
			{
				//大乱斗模式
				if (pModel->valueint == _BATTLE_MODEL_CONFUSION)
				{					
					//获得队伍指针
					pBt = m_pPlayer->GetBct();
					if(!pBt)
					{
						return PACKET_EXE_CONTINUE;
					}
					
					//广播登陆完成成功消息给全队人员
					CMsgBattle msg;
					IF_OK(msg.CreateMsg(_BATTLE_ACTION_LOADING_COMPLETE_SUC, _BATTLE_MODEL_CONFUSION, 0,  m_pPlayer->GetUid()))
					{
						pBt->BroadcastTeamMsg(&msg);
					}

					return PACKET_EXE_CONTINUE;
				}
			}
            break;
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			///加载完成-[红蓝战队]
            case _BATTLE_ACTION_TEAM_LOADING_COMPLETE:
            {
                //大乱斗模式
                if (pModel->valueint == _BATTLE_MODEL_RB_FIGHT_TEAM)
                {                   
                   //获得队伍指针
                   pBt = m_pPlayer->GetBrbt();
                   if(!pBt)
                   {
                       return PACKET_EXE_CONTINUE;
                   }
                   //找出对方队伍指针
		           CBattleTeam* pKillerBrbt = pBt->GetEnemyTeam();
		           if(!pKillerBrbt)
		           {
			            return PACKET_EXE_CONTINUE;
		           }
                   
                   //广播登陆完成成功消息给两队的所有队员
                   CMsgBattle msg;
                   IF_OK(msg.CreateMsg(_BATTLE_ACTION_TEAM_LOADING_COMPLETE_SUC, _BATTLE_MODEL_RB_FIGHT_TEAM, 0, m_pPlayer->GetUid()))
                   {
                       pBt->BroadcastTeamMsg(&msg);
                       pKillerBrbt->BroadcastTeamMsg(&msg);
                   }

                   return PACKET_EXE_CONTINUE;
                 }
             }
             break;
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			///邀请好友加入队伍-[大乱斗]-[红蓝对战]
            case _BATTLE_ACTION_INVITE_FRIEND_JOIN:
            {
            	cJSON* pFriendId= cJSON_GetObjectItem(pJsonMsg, "friend_id");				// int 待邀请的好友id
				CHECKF(pFriendId);

            	//大乱斗模式
				if (pModel->valueint == _BATTLE_MODEL_CONFUSION)
				{					
					//获得队伍指针
					pBt = m_pPlayer->GetBct();
					if(!pBt)
					{
						return PACKET_EXE_CONTINUE;
					}

					//广播登陆完成成功消息给两队的所有队员
					UINT nTeamId = pBt->GetTeamId();
					UINT nFriendId = pFriendId->valueint;

					//判断是否已经为好友
					if(!m_pPlayer->QueryFriendManager().IsFriend(nFriendId))
					{
						LOGERROR("[CMsgBattle::Process] Playerid=[%d] nFriendId=[%d] Is Not Friend", m_pPlayer->GetUid(), nFriendId);
						return PACKET_EXE_CONTINUE;
					}

					Player* pFriend = g_pPlayerPool->GetPlayerByUid(nFriendId);
					if (pFriend)
					{
						//同步信息
						CMsgBattle msg;
						IF_OK(msg.CreateMsg(_BATTLE_ACTION_INVITE_FRIEND_JOIN, pModel->valueint, nTeamId, m_pPlayer->GetUid(), 0, 0, 0, 0, 0, 0, 0, 0, m_pPlayer->GetNickName(), m_pPlayer->GetHeadPicurl()))
						{
							pFriend->SendMsgToClient(&msg);
							LOGDEBUG("[CMsgBattle::Process] action=[_BATTLE_ACTION_INVITE_FRIEND_JOIN] nTeamId=[%d] nFriendId=[%d] Uid=[%d] m_strNickName=[%s] m_strHeadPicurl=[%s]",
								nTeamId, nFriendId, m_pPlayer->GetUid(), m_pPlayer->GetNickName().c_str(), m_pPlayer->GetHeadPicurl().c_str());
						}
					}
					else	//不在线
					{
						LOGDEBUG("[CMsgBattle::Process] action=[_BATTLE_ACTION_INVITE_FRIEND_JOIN] Is OffOnline nTeamId=[%d] nFriendId=[%d] Uid=[%d] m_strNickName=[%s] m_strHeadPicurl=[%s]",
							nTeamId, nFriendId, m_pPlayer->GetUid(), m_pPlayer->GetNickName().c_str(), m_pPlayer->GetHeadPicurl().c_str());	
					}
					return PACKET_EXE_CONTINUE;
                   
				}
                //大乱斗模式
                else if (pModel->valueint == _BATTLE_MODEL_RB_FIGHT_TEAM)
                {                   
                   //获得队伍指针
                   pBt = m_pPlayer->GetBrbt();
                   if(!pBt)
                   {
                       return PACKET_EXE_CONTINUE;
                   }
                   
                   //广播登陆完成成功消息给两队的所有队员
					UINT nTeamId= pBt->GetTeamId();
					UINT nFriendId = pFriendId->valueint;

					//判断是否已经为好友
					if(!m_pPlayer->QueryFriendManager().IsFriend(nFriendId))
					{
						LOGERROR("[CMsgBattle::Process] Playerid=[%d] nFriendId=[%d] Is Not Friend", m_pPlayer->GetUid(), nFriendId);
						return PACKET_EXE_CONTINUE;
					}

					Player* pFriend = g_pPlayerPool->GetPlayerByUid(nFriendId);
					if (pFriend)
					{
						//同步信息
						CMsgBattle msg;
						IF_OK(msg.CreateMsg(_BATTLE_ACTION_INVITE_FRIEND_JOIN, pModel->valueint, nTeamId, m_pPlayer->GetUid(), 0, 0, 0, 0, 0, 0, 0, 0, m_pPlayer->GetNickName(), m_pPlayer->GetHeadPicurl()))
						{
							pFriend->SendMsgToClient(&msg);
							LOGDEBUG("[CMsgBattle::Process] action=[_BATTLE_ACTION_INVITE_FRIEND_JOIN] nTeamId=[%d] nFriendId=[%d] Uid=[%d] m_strNickName=[%s] m_strHeadPicurl=[%s]",
								nTeamId, nFriendId, m_pPlayer->GetUid(), m_pPlayer->GetNickName().c_str(), m_pPlayer->GetHeadPicurl().c_str());
						}
					}
					else	//不在线
					{
						LOGDEBUG("[CMsgBattle::Process] action=[_BATTLE_ACTION_INVITE_FRIEND_JOIN] Is OffOnline nTeamId=[%d] nFriendId=[%d] Uid=[%d] m_strNickName=[%s] m_strHeadPicurl=[%s]",
							nTeamId, nFriendId, m_pPlayer->GetUid(), m_pPlayer->GetNickName().c_str(), m_pPlayer->GetHeadPicurl().c_str());	
					}

                   return PACKET_EXE_CONTINUE;
                 }
             }
             break;

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			default:
			{
			}
			break;
		}
	}

	return PACKET_EXE_ERROR;
	DEBUG_CATCH0("[CMsgBattle::Process] Is Catch...");
}
