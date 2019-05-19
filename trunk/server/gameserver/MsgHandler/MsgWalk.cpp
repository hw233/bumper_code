#include "MsgWalk.h"
#include "MsgUserInfo.h"

#include "../Base/logger.h"
#include "../Main/ServerManager.h"
#include "../NetBase/Socket.h"
#include "MsgAction.h"
#include "MsgBattle.h"
#include <map>

//////////////////////////////////////////////////// CMsgWalk ////////////////////////////////////////////////////
//构造函数
CMsgWalk::CMsgWalk() 
{ 
}

//析构函数
CMsgWalk::~CMsgWalk() 
{ 	
}

//读取数据
BOOL CMsgWalk::Read(SocketInputStream& iStream, UINT nLen)
{
	CNetMsg::Read(iStream, nLen);
	return TRUE;
}

BOOL CMsgWalk::Write(SocketOutputStream& oStream) const
{
	return TRUE;
}
BOOL CMsgWalk::CreateMsg(UINT nRetStatus, UINT nAction)
{
	return TRUE;
}
//创建消息
BOOL CMsgWalk::CreateFrameMsg(INT nAction/*=_WALK_ACTION_USER_CMD*/, INT nTargetId/*=0*/, INT nPointX/*=0*/, INT nPointY/*=0*/, INT nPointStartX/*=0*/, INT nPointStartY/*=0*/,INT nPointEndX/*=0*/, INT nPointEndY/*=0*/, INT nIndex/*=0*/, INT nBeCollideId/*=0*/, INT nTType/*=0*/, INT nTPowerX/*=0*/, INT nTPowerY/*=0*/, INT nTimeStamp/*=0*/  )
{
	if (m_pMsg)
	{
		if (nAction == _WALK_ACTION_USER_CMD)
		{
			cJSON* cjParam = cJSON_CreateObject();
			IF_OK(cjParam)
			{
				std::string strParamKey = "members" + std::to_string(nIndex);					//members 成员list
				cJSON_AddNumberToObject(cjParam, "action", nAction);							//action 玩家id
				cJSON_AddNumberToObject(cjParam, "target_id", nTargetId);						//target_id 玩家id
				//cJSON_AddNumberToObject(cjParam, "point_x", nPointX);							//point_x 车坐标像素x
				//cJSON_AddNumberToObject(cjParam, "point_y", nPointY);							//point_x 车坐标像素y
				cJSON_AddNumberToObject(cjParam, "point_start_x", nPointStartX);				//point_start_x 用户操作起始点坐标像素x
				cJSON_AddNumberToObject(cjParam, "point_start_y", nPointStartY);				//point_start_y 用户操作起始点坐标像素y
				//cJSON_AddNumberToObject(cjParam, "point_end_x", nPointEndX);					//point_end_x 用户操作起终点坐标像素x
				//cJSON_AddNumberToObject(cjParam, "point_end_y", nPointEndY);					//point_end_y 用户操作起终点坐标像素y
				cJSON_AddNumberToObject(cjParam, "ts", nTimeStamp);								//ts 服务器数据包时间帧序列

				cJSON_AddItemToObject(m_pMsg, strParamKey.c_str(), cjParam);						
			}	
		}		
		//空桢继续上一个操作命令，服务器66ms = 前端33ms * 2
		else if(nAction == _WALK_ACTION_CONTINUE_PREVIOUS_CMD)
		{
			cJSON* cjParam = cJSON_CreateObject();
			IF_OK(cjParam)
			{
				std::string strParamKey = "members" + std::to_string(nIndex);					//members 成员list
				cJSON_AddNumberToObject(cjParam, "action", nAction);							//action 当玩家66ms 没有操作数据包过来直接发送继续执行上一个命令的数据包给前端
				cJSON_AddNumberToObject(cjParam, "target_id", nTargetId);						//target_id 玩家id
				cJSON_AddNumberToObject(cjParam, "ts", nTimeStamp);								//ts 服务器数据包时间帧序列

				cJSON_AddItemToObject(m_pMsg, strParamKey.c_str(), cjParam);
			}
			
		}
		//战斗中碰撞-[大乱斗]-[红蓝战队]
		else if(nAction == _WALK_ACTION_COLLIDE_CMD)				
		{
			cJSON* cjParam = cJSON_CreateObject();
			IF_OK(cjParam)
			{
				std::string strParamKey = "members" + std::to_string(nIndex);					//members 成员list
				cJSON_AddNumberToObject(cjParam, "action", nAction);							//action 当玩家66ms 没有操作数据包过来直接发送继续执行上一个命令的数据包给前端
				cJSON_AddNumberToObject(cjParam, "target_id", nTargetId);						//target_id 玩家id
				cJSON_AddNumberToObject(cjParam, "type", nPointStartX);							//type 
				cJSON_AddNumberToObject(cjParam, "power_x", nPointX);							//power_x
				cJSON_AddNumberToObject(cjParam, "power_y", nPointY);							//power_y

				//被撞击者的数据
				//cJSON_AddNumberToObject(cjParam, "becollide_id", nBeCollideId);				// int  被撞击着的ID
				//cJSON_AddNumberToObject(cjParam, "t_type", nTType);							// int  类型
				//cJSON_AddNumberToObject(cjParam, "t_power_x", nTPowerX);						// float 冲量x
				//cJSON_AddNumberToObject(cjParam, "t_power_y", nTPowerY);						// float 冲量y

				cJSON_AddNumberToObject(cjParam, "ts", nTimeStamp);								//ts 服务器数据包时间帧序列

				cJSON_AddItemToObject(m_pMsg, strParamKey.c_str(), cjParam);
			}
		}
		else if(nAction == _WALK_ACTION_CONTINUE_COLLIDE_CMD)				
		{
			cJSON* cjParam = cJSON_CreateObject();
			IF_OK(cjParam)
			{
				std::string strParamKey = "members" + std::to_string(nIndex);
				cJSON_AddNumberToObject(cjParam, "action", nAction);							//action 当玩家66ms 没有操作数据包过来直接发送继续执行上一个命令的数据包给前端
				cJSON_AddNumberToObject(cjParam, "target_id", nTargetId);						//target_id 玩家id
				cJSON_AddNumberToObject(cjParam, "type", nPointStartX);							//type 
				cJSON_AddNumberToObject(cjParam, "power_x", nPointX);							//power_x
				cJSON_AddNumberToObject(cjParam, "power_y", nPointY);							//power_y

				//被撞击者的数据
				//cJSON_AddNumberToObject(cjParam, "becollide_id", nBeCollideId);				// int  被撞击着的ID
				//cJSON_AddNumberToObject(cjParam, "t_type", nTType);							// int  类型
				//cJSON_AddNumberToObject(cjParam, "t_power_x", nTPowerX);						// float 冲量x
				//cJSON_AddNumberToObject(cjParam, "t_power_y", nTPowerY);						// float 冲量y

				cJSON_AddNumberToObject(cjParam, "ts", nTimeStamp);								//ts 服务器数据包时间帧序列

				cJSON_AddItemToObject(m_pMsg, strParamKey.c_str(), cjParam);
			}
		}
		//继续战斗复活-[大乱斗]
		else if(nAction == _WALK_ACTION_CONFUSION_RELIVE)				
		{
			cJSON* cjParam = cJSON_CreateObject();
			IF_OK(cjParam)
			{
				std::string strParamKey = "members" + std::to_string(nIndex);
				cJSON_AddNumberToObject(cjParam, "action", nAction);							//action 当玩家66ms 没有操作数据包过来直接发送继续执行上一个命令的数据包给前端
				cJSON_AddNumberToObject(cjParam, "target_id", nTargetId);						//target_id 玩家id
				cJSON_AddNumberToObject(cjParam, "directionX", nPointX);						//directionX 
				cJSON_AddNumberToObject(cjParam, "directionY", nPointY);						//directionY
				cJSON_AddNumberToObject(cjParam, "region_id", nPointStartX);					//region_id
				cJSON_AddNumberToObject(cjParam, "car_lev", nPointStartY);						//车辆等级

				cJSON_AddNumberToObject(cjParam, "ts", nTimeStamp);								//服务器数据包时间帧序列

				cJSON_AddItemToObject(m_pMsg, strParamKey.c_str(), cjParam);
			}
		}
		//继续战斗复活-[红蓝战队]
		else if(nAction == _WALK_ACTION_TEAM_RELIVE)				
		{
			cJSON* cjParam = cJSON_CreateObject();
			IF_OK(cjParam)
			{
				std::string strParamKey = "members" + std::to_string(nIndex);
				cJSON_AddNumberToObject(cjParam, "action", nAction);							//当玩家66ms 没有操作数据包过来直接发送继续执行上一个命令的数据包给前端
				cJSON_AddNumberToObject(cjParam, "target_id", nTargetId);						//玩家id
				cJSON_AddNumberToObject(cjParam, "region_id", nPointX);							//region_id
				cJSON_AddNumberToObject(cjParam, "car_lev", nPointY);							//car_lev

				cJSON_AddNumberToObject(cjParam, "ts", nTimeStamp);								//服务器数据包时间帧序列

				cJSON_AddItemToObject(m_pMsg, strParamKey.c_str(), cjParam);
			}
		}
		//玩家车辆等级改变-[大乱斗]
		else if (nAction == _WALK_ACTION_CONFUSION_CAR_LEV_CHANGE)
		{
			cJSON* cjParam = cJSON_CreateObject();
			IF_OK(cjParam)
			{
				std::string strParamKey = "members" + std::to_string(nIndex);
				cJSON_AddNumberToObject(cjParam, "action", nAction);							//action
				cJSON_AddNumberToObject(cjParam, "target_id", nTargetId);						//target_id
				cJSON_AddNumberToObject(cjParam, "fight_lev", nPointX);							//fight_lev

				cJSON_AddNumberToObject(cjParam, "ts", nTimeStamp);								//服务器数据包时间帧序列

				cJSON_AddItemToObject(m_pMsg, strParamKey.c_str(), cjParam);
			}
		}
		//玩家车辆等级改变-[红蓝战队]
		else if (nAction == _WALK_ACTION_TEAM_CAR_LEV_CHANGE)
		{				
			cJSON* cjParam = cJSON_CreateObject();
			IF_OK(cjParam)
			{
				std::string strParamKey = "members" + std::to_string(nIndex);
				cJSON_AddNumberToObject(cjParam, "action", nAction);							//action
				cJSON_AddNumberToObject(cjParam, "target_id", nTargetId);						//target_id
				cJSON_AddNumberToObject(cjParam, "car_lev", nPointX);							//car_lev

				cJSON_AddNumberToObject(cjParam, "ts", nTimeStamp);								//服务器数据包时间帧序列

				cJSON_AddItemToObject(m_pMsg, strParamKey.c_str(), cjParam);
			}		
		}
		//击杀-[大乱斗]-[红蓝战队]
		else if (nAction == _WALK_ACTION_KILL)
		{				
			cJSON* cjParam = cJSON_CreateObject();
			IF_OK(cjParam)
			{
				std::string strParamKey = "members" + std::to_string(nIndex);
				cJSON_AddNumberToObject(cjParam, "action", nAction);							//action
				cJSON_AddNumberToObject(cjParam, "target_id", nTargetId);						//target_id
				cJSON_AddNumberToObject(cjParam, "killer_id", nPointX);							//killer_id	
				cJSON_AddNumberToObject(cjParam, "award_gold", nPointY);						//本次击杀奖励给击杀者的数量			
				cJSON_AddNumberToObject(cjParam, "award_score", nPointStartX);					//本次击杀奖励给击杀者的积分

				cJSON_AddNumberToObject(cjParam, "ts", nTimeStamp);								//服务器数据包时间帧序列

				cJSON_AddItemToObject(m_pMsg, strParamKey.c_str(), cjParam);
			}		
		}		

		m_pData = cJSON_Print(m_pMsg);
		string strMsg(m_pData);
    	size_t nLen = strMsg.size();
		//if (nAction == _WALK_ACTION_CONTINUE_COLLIDE_CMD)
		{
			LOGDEBUG("[CMsgWalk::CreateFrameMsg]\n m_pData=[%s] nLen=[%d]\n", m_pData, nLen);
		}						
	}		

	return TRUE;
}

//创建消息
BOOL CMsgWalk::CreateMsg(INT nAction/*=_WALK_ACTION_USER_CMD*/, INT nTargetId/*=0*/, INT nPointX/*=0*/, INT nPointY/*=0*/, INT nPointStartX/*=0*/, INT nPointStartY/*=0*/,INT nPointEndX/*=0*/, INT nPointEndY/*=0*/, long long  llTimeStamp/*=0*/)
{
	if (m_pMsg)
	{
		if (nAction == _WALK_ACTION_USER_CMD)
		{
			cJSON_AddNumberToObject(m_pMsg, "action", nAction);
			cJSON_AddNumberToObject(m_pMsg, "target_id", nTargetId);					//玩家id
			cJSON_AddNumberToObject(m_pMsg, "point_x", nPointX);						//车坐标像素x
			cJSON_AddNumberToObject(m_pMsg, "point_y", nPointY);						//车坐标像素y
			cJSON_AddNumberToObject(m_pMsg, "point_start_x", nPointStartX);				//用户操作起始点坐标像素x
			cJSON_AddNumberToObject(m_pMsg, "point_start_y", nPointStartY);				//用户操作起始点坐标像素y
			cJSON_AddNumberToObject(m_pMsg, "point_end_x", nPointEndX);					//用户操作起终点坐标像素x
			cJSON_AddNumberToObject(m_pMsg, "point_end_y", nPointEndY);					//用户操作起终点坐标像素y
			cJSON_AddNumberToObject(m_pMsg, "time_stamp", INT(llTimeStamp%1000000));	//用户操作起终点坐标像素y
			
		}
		//空桢继续上一个操作命令，服务器66ms = 前端33ms * 2
		else if(nAction == _WALK_ACTION_CONTINUE_PREVIOUS_CMD)
		{
			cJSON_AddNumberToObject(m_pMsg, "action", nAction);							//当玩家66ms 没有操作数据包过来直接发送继续执行上一个命令的数据包给前端
			cJSON_AddNumberToObject(m_pMsg, "target_id", nTargetId);					//玩家id
			cJSON_AddNumberToObject(m_pMsg, "time_stamp", INT(llTimeStamp%1000000));	//用户操作起终点坐标像素y
		}
		//战斗中碰撞-[大乱斗]-[红蓝战队]
		else if(nAction == _WALK_ACTION_COLLIDE_CMD)				
		{
			cJSON_AddNumberToObject(m_pMsg, "action", nAction);							//当玩家66ms 没有操作数据包过来直接发送继续执行上一个命令的数据包给前端
			cJSON_AddNumberToObject(m_pMsg, "target_id", nTargetId);					//玩家id
			cJSON_AddNumberToObject(m_pMsg, "type", nPointStartX);						//type
			cJSON_AddNumberToObject(m_pMsg, "power_x", nPointX);						//power_x
			cJSON_AddNumberToObject(m_pMsg, "power_y", nPointY);						//power_y
			cJSON_AddNumberToObject(m_pMsg, "time_stamp", INT(llTimeStamp%1000000));	//用户操作起终点坐标像素y
		}
		else if(nAction == _WALK_ACTION_CONTINUE_COLLIDE_CMD)				
		{
			cJSON_AddNumberToObject(m_pMsg, "action", nAction);							//当玩家66ms 没有操作数据包过来直接发送继续执行上一个命令的数据包给前端
			cJSON_AddNumberToObject(m_pMsg, "target_id", nTargetId);					//玩家id
			cJSON_AddNumberToObject(m_pMsg, "type", nPointStartX);						//type
			cJSON_AddNumberToObject(m_pMsg, "power_x", nPointX);						//power_x
			cJSON_AddNumberToObject(m_pMsg, "power_y", nPointY);						//power_y
			cJSON_AddNumberToObject(m_pMsg, "time_stamp", INT(llTimeStamp%1000000));	//用户操作起终点坐标像素y
		}


		//m_pData = cJSON_Print(m_pMsg);
		//string strMsg(m_pData);
    	//INT nLen = strMsg.size();
		//LOGDEBUG("[CMsgWalk::CreateMsg]\n m_pData=[%s] nLen=[%d]\n", m_pData, nLen);			
	}		

	return TRUE;
}
//插入服务器消息队列缓存
// VOID CMsgWalk::InsertFrameMsgCache(INT nModel/*=_BATTLE_MODEL_CONFUSION*/, INT nAction/*=_WALK_ACTION_USER_CMD*/, INT nTargetId/*=0*/, INT nPointX/*=0*/, INT nPointY/*=0*/, INT nPointStartX/*=0 type*/, INT nPointStartY/*=0*/,INT nPointEndX/*=0*/, INT nPointEndY/*=0*/, INT nBeCollideId/*=0*/, INT nTType/*=0*/, INT nTPowerX/*=0*/, INT nTPowerY/*=0*/)
// {
// 	////////////////////////////////////////////////////////////////////////
// 	//装载到multimap中
// 	CBattleTeam* pBt = NULL;
// 	Player* pPlayer = g_pPlayerPool->GetPlayerByUid(nTargetId);
// 	if (!pPlayer)
// 	{
// 		LOGERROR("[CMsgWalk::InsertFrameMsgCache] Is Error pPlayer == NULL nTargetId=[%d]", nTargetId);
// 		return;
// 	}
// 	//大乱斗模式
// 	if (nModel == _BATTLE_MODEL_CONFUSION)
// 	{
// 		pBt = pPlayer->GetBct();
// 	}
// 	//红蓝队对战模式
// 	else// if (nModel ==  _BATTLE_MODEL_RB_FIGHT_TEAM)
// 	{
// 		pBt = pPlayer->GetBrbt();
// 	}

// 	if(pBt == NULL)
// 	{
// 		LOGERROR("[CMsgWalk::InsertFrameMsgCache] Is Error pBt == NULL nTargetId=[%d]", nTargetId);
// 		return;
// 	}

// 	//装载数据结构
// 	TeamNetMsg tnm;
// 	tnm.nModel = nTargetId;
// 	tnm.nAction = nAction;

// 	tnm.nTargetId = nTargetId;
// 	tnm.nPointX = nPointX;
// 	tnm.nPointY = nPointY;
// 	tnm.nPointStartX = nPointStartX;
// 	tnm.nPointStartY = nPointStartY;
// 	tnm.nPointEndX = nPointEndX;
// 	tnm.nPointEndY = nPointEndY;

// 	tnm.nBeCollideId = nBeCollideId;
// 	tnm.nTType = nTType;
// 	tnm.nTPowerX = nTPowerX;
// 	tnm.nTPowerY = nTPowerY;

// 	//插入对应的队伍消息缓存
// 	pair<UINT, TeamNetMsg> p(nTargetId, tnm);
// 	pBt->GetTeamNetMsgSet().insert(p);
	
// 	////////////////////////////////////////////////////////////////////////
// 	long long llRecvMsgTimeStamp = GetMillSecTime();
// 	LOGDEBUG("[CMsgWalk::InsertFrameMsgCache] PlayerId=[%d] llRecvMsgTimeStamp=[%lld] nAction=[%d] nPointX=[%d] nPointY=[%d] nPointStartX=[%d] nPointStartY=[%d] nPointEndX=[%d] nPointEndY=[%d] nBeCollideId=[%d] nTType=[%d] nTPowerX=[%d] nTPowerY=[%d] size=[%d]", 
// 		nTargetId, llRecvMsgTimeStamp,nAction,nPointX,nPointY,nPointStartX,nPointStartY,nPointEndX,nPointEndY,nBeCollideId,nTType,nTPowerX,nTPowerY, pBt->GetTeamNetMsgSet().size());
// }
//发送json数据
BOOL CMsgWalk::SendMsg(CNetMsg* pMsg)
{
	m_pPlayer->SendMsgToClient(pMsg);
	return TRUE;	
}

//处理前端发送过来的消息
UINT CMsgWalk::Process(VOID *pInfo)
{	
	//LOGDEBUG("[CMsgWalk::Process] begin");	
	DEBUG_TRY
	//设置需要回包的玩家
	m_pPlayer = static_cast<Player*>(pInfo);
	CHECKF(m_pPlayer);	

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

	//处理前端发过来的消息
	m_pMsg = cJSON_Parse(this->m_pData);
	cJSON* pJsonMsg = m_pMsg;	
	IF_OK(pJsonMsg)
	{
		cJSON* pModel = cJSON_GetObjectItem(pJsonMsg, "model");
		cJSON* pAction = cJSON_GetObjectItem(pJsonMsg, "action");
		CHECKF(pAction && pModel);

		CBattleTeam* pBt = NULL;
		LOGDEBUG("[CMsgWalk::Process] model=[%d] action=[%d] PlayerID=[%d] pPlayer=[%p]", pModel->valueint, pAction->valueint, m_pPlayer->GetUid(), m_pPlayer);
		switch(pAction->valueint)
		{
			///////////////////////////////////////////////////////////////////////////
			///用户操作
			case _WALK_ACTION_USER_CMD:
			{
				//调试的时候发现服务器广播碰撞空帧的时候到第三帧的时候收到了前端执行碰撞逻辑玩家的操作包，此处服务器做下过滤，死亡需要重置
				//后面在仔细检查
				//if (m_pPlayer->GetSocket() && m_pPlayer->GetSocket()->GetSynMsgStatus() == _SYN_MSG_COLLIDE)
				//{
				//	LOGWARNING("[CMsgWalk::Process] status=[_SYN_MSG_COLLIDE] CollideSynMsgCount=[%d] NeedSynCount=[%d]", m_pPlayer->GetSocket()->GetCollideSynMsgCount(), g_config.ONLINE_BROADCAST_COLLIDE_SYN_MSG_COUNT);
				//	return PACKET_EXE_CONTINUE;
				//}

				//大乱斗模式
				if (pModel->valueint == _BATTLE_MODEL_CONFUSION)
				{
					pBt = m_pPlayer->GetBct();
				}
				//红蓝队对战模式
				else if (pModel->valueint ==  _BATTLE_MODEL_RB_FIGHT_TEAM)
				{
					pBt = m_pPlayer->GetBrbt();

					//如果玩家3 2 1 倒计时还未结束就发送了操作包，则丢弃
					// if (m_pPlayer->GetDeadTimeInterval().TimeOver(_STATUS_PREPARE_ALIVE_INTERVAL))
					// {
					// 	//...处理玩家操作包
					// 	m_pPlayer->GetDeadTimeInterval().Clear();
					// }
					// else
					// {
					// 	//...不处理玩家操作包直接返回
					// 	LOGERROR("[CMsgWalk::Process] Not TimeOver Receive Player CMD playerid=[%d]", m_pPlayer->GetUid());
					// 	return PACKET_EXE_CONTINUE;
					// }
				}
				else
				{
					LOGERROR("[CMsgWalk::Process] Is Error-1 pModel==[%d]", pModel->valueint);
					return PACKET_EXE_ERROR;
				}
				
				//处理走路包同步所有队伍成员
				if(pBt)
				{
					//cJSON* pTargetId = cJSON_GetObjectItem(pJsonMsg, "target_id");
					cJSON* pPointX = cJSON_GetObjectItem(pJsonMsg, "point_x");//第一版：位置y
					cJSON* pPointY = cJSON_GetObjectItem(pJsonMsg, "point_y");//第一版：位置y
					cJSON* pPointStartX = cJSON_GetObjectItem(pJsonMsg, "point_start_x");//第二版：方向x（0~1）	
					cJSON* pPointStartY = cJSON_GetObjectItem(pJsonMsg, "point_start_y");//第二版：方向y（0~1）
					cJSON* pPointEndX = cJSON_GetObjectItem(pJsonMsg, "point_end_x");//无用
					cJSON* pPointEndY = cJSON_GetObjectItem(pJsonMsg, "point_end_y");//无用
					CHECKF(pPointX && pPointY && pPointStartX && pPointStartY && pPointEndX && pPointEndY);

					UINT nTargetId = m_pPlayer->GetUid();

					///////////////////////////////////////////////////////////////////////
		            ///;广播同步帧缓存66ms 0:不缓存 1:缓存
		            if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 1)
		            {
						//插入服务器消息队列缓存
						pBt->InsertFrameMsgCache(pModel->valueint, _WALK_ACTION_USER_CMD, nTargetId, pPointX->valueint, pPointY->valueint, pPointStartX->valueint, pPointStartY->valueint, pPointEndX->valueint, pPointEndY->valueint,0,0,0,0);
					}
					else
					{	
						//同步前端数据包
						CMsgWalk msg;
						IF_OK(msg.CreateMsg(_WALK_ACTION_USER_CMD, nTargetId, pPointX->valueint, pPointY->valueint, pPointStartX->valueint, pPointStartY->valueint, pPointEndX->valueint, pPointEndY->valueint))
						{
			                pBt->BroadcastTeamMsg(&msg);
			                
			                CBattleTeam* pEnemyBt = pBt->GetEnemyTeam();
			                if(pEnemyBt)
			                {
			                    pEnemyBt->BroadcastTeamMsg(&msg);
			                }
						}
					}

					///////////////////////////////////////////////////////////////////////
		            ///;广播同步帧缓存66ms 0:不缓存 1:缓存
		            //if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 0)
		            {
						//收到第一个命令包，设置时间戳
		            	long long llCurMillSecTime = GetMillSecTime();
		            	long long mllRecvMsgTimeStamp = pBt->GetRecvMsgTimeStamp();
		            	if (mllRecvMsgTimeStamp == 0)
		            	{
		            		pBt->SetRecvMsgTimeStamp(llCurMillSecTime);
		            	}
					}

					//设置玩家游戏状态
                   m_pPlayer->SetGameStep(_GAME_STEP_LOADING_COMPLETE);
                   LOGDEBUG("[CMsgWalk::Process] GameStep=[_GAME_STEP_LOADING_COMPLETE] PlayerId=[%d]", m_pPlayer->GetUid());

                   //设置活着状态
                   if (m_pPlayer->GetDeadStatus() == _STATUS_PREPARE_ALIVE)
                   {
                   		m_pPlayer->SetDeadStatus(_STATUS_ALIVE);
                   		LOGDEBUG("[CMsgWalk::Process] _WALK_ACTION_USER_CMD Playerid=[%d] DeadStatus=[ _STATUS_ALIVE ]", m_pPlayer->GetUid());
                   }
				}	
				else
				{
					LOGERROR("[CMsgWalk::Process] Is Error-2 pBt==NULL");
				}					
			}
			break;
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			///战斗中碰撞-当操作执行
			case _WALK_ACTION_COLLIDE_CMD:
			{
				///--------------方案三、撞击者A和撞击者B两个玩家发生碰撞事件，由整个队伍中全员都上报A_B事件，此时如果出现都网络不好的情况，也不会影响到其他玩家的渲染
#if 0
				cJSON* pCollideId = cJSON_GetObjectItem(pJsonMsg, "collide_id");			// int  撞击着的ID
				cJSON* pType = cJSON_GetObjectItem(pJsonMsg, "type");						// int  类型
				cJSON* pPowerX = cJSON_GetObjectItem(pJsonMsg, "power_x");					// float 冲量x
				cJSON* pPowerY = cJSON_GetObjectItem(pJsonMsg, "power_y");					// float 冲量y

				//被撞击者的数据
				cJSON* pBeCollideId = cJSON_GetObjectItem(pJsonMsg, "becollide_id");		// int  被撞击着的ID
				cJSON* pTType = cJSON_GetObjectItem(pJsonMsg, "t_type");					// int  类型
				cJSON* pTPowerX = cJSON_GetObjectItem(pJsonMsg, "t_power_x");				// float 冲量x
				cJSON* pTPowerY = cJSON_GetObjectItem(pJsonMsg, "t_power_y");				// float 冲量y
				cJSON* pTS = cJSON_GetObjectItem(pJsonMsg, "ts");							// int //服务器数据包时间帧序列
				CHECKF(pPowerX && pPowerY && pType && pBeCollideId && pTType && pTPowerX && pTPowerY && pTS);

				UINT nCollideId = pCollideId->valueint;
				UINT nBeCollideId = pBeCollideId->valueint;
				UINT nTs = pTS->valueint;
				Player* pCollider = NULL;
				Player* pBeCollider = NULL;
				CBattleTeam* pEnemyBrbt = NULL;

				//大乱斗模式
				if (pModel->valueint == _BATTLE_MODEL_CONFUSION)
				{	
					pBt = m_pPlayer->GetBct();
					if(pBt)
					{
						//找到撞击者
						pCollider = pBt->GetMember(nCollideId);
						if (!pCollider)
						{
							LOGWARNING("[CMsgBattle::Process] pCollider == NULL");
							return PACKET_EXE_CONTINUE;
						}

						//找到被撞击者
						pBeCollider = pBt->GetMember(nBeCollideId);
						if (!pBeCollider)
						{
							LOGWARNING("[CMsgBattle::Process] pBeCollider == NULL");
							return PACKET_EXE_CONTINUE;
						}

						//检测碰撞帧序列，前面已经发生的，但是服务器已经处理了很后面的抛掉
						UINT nColliderCurFrame = pCollider->GetCurCollideFrame();
						if (nColliderCurFrame != 0 && nTs < nColliderCurFrame)
						{
							LOGWARNING("[CMsgBattle::Process] nTs < nColliderCurFrame nTs=[%d] nColliderCurFrame=[%d]", nTs, nColliderCurFrame);
							return PACKET_EXE_CONTINUE;
						}

						UINT nBeColliderCurFrame = pBeCollider->GetCurCollideFrame();
						if (nBeColliderCurFrame != 0 && nTs < nBeColliderCurFrame)
						{
							LOGWARNING("[CMsgBattle::Process] nTs < nBeColliderCurFrame nTs=[%d] nBeColliderCurFrame=[%d]", nTs, nBeColliderCurFrame);
							return PACKET_EXE_CONTINUE;
						}

						//打印碰撞缓存队列中的记录信息
						// INT nIndex = 0;
						// MAP_COLLIDE_CIT iter = pCollider->QueryCollideSet().begin();
						// for (; iter != pCollider->QueryCollideSet().end() ; ++iter)
						// {
						// 	LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_CONFUSION m_MapCollideSet-1 key==>[%d]==>[%s]", ++nIndex, iter->first.c_str());
						// }
						
						//判断是否超过指定长度，超过则删除之前的碰撞缓存数据
						size_t nSize = pCollider->QueryCollideSet().size();
						if (nSize > (size_t)g_config.ONLINE_MAP_COLLIDE_SIZE)
						{	
							LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_CONFUSION m_MapCollideSet-1 Erase Is Ok...Key=[%s] nSize=[%d]", pCollider->QueryCollideSet().begin()->first.c_str() ,nSize);
							pCollider->QueryCollideSet().erase(pCollider->QueryCollideSet().begin());  
						}
						LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_CONFUSION m_MapCollideSet-1 Size=[%d]", nSize);
						
						std::string strKey = std::to_string(nTs) + "_" +std::to_string(nCollideId) + "_" + std::to_string(nBeCollideId);
						//记录玩家的碰撞信息
						if (pCollider->QueryCollideSet().find(strKey.c_str()) != pCollider->QueryCollideSet().end())
						{
							LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_CONFUSION m_MapCollideSet-1 Have Report... strKey=[%s] nTs=[%d] nCollideId=[%d] pPowerX=[%d] pPowerY=[%d] nBeCollideId=[%d] pTType=[%d] pTPowerX=[%d] pTPowerY=[%d]", 
								strKey.c_str(), nTs, nCollideId, pPowerX->valueint, pPowerY->valueint, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);
							return PACKET_EXE_CONTINUE;
						}

						pCollider->QueryCollideSet().insert(pair<std::string, UINT>(strKey.c_str(), 1));
						pCollider->SetCurCollideFrame(nTs);
 						LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_CONFUSION m_MapCollideSet-1 Insert Is OK... strKey=[%s] nTs=[%d] nCollideId=[%d] pPowerX=[%d] pPowerY=[%d] nTargetId=[%d] pTType=[%d] pTPowerX=[%d] pTPowerY=[%d]", 
 							strKey.c_str(), nTs, nCollideId, pPowerX->valueint, pPowerY->valueint, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);

 						//Player* pBeCollider = g_pPlayerPool->GetPlayerByUid(nBeCollideId);
 						if (pBeCollider)
 						{
 							strKey = std::to_string(nTs) + "_" +std::to_string(nBeCollideId) + "_" + std::to_string(nCollideId);
 							if (pBeCollider->QueryCollideSet().find(strKey.c_str()) != pBeCollider->QueryCollideSet().end())
							{
								LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_CONFUSION m_MapCollideSet-2 Have Report... strKey=[%s] nTs=[%d] nCollideId=[%d] pPowerX=[%d] pPowerY=[%d] nBeCollideId=[%d] pTType=[%d] pTPowerX=[%d] pTPowerY=[%d]", 
									strKey.c_str(), nTs, nCollideId, pPowerX->valueint, pPowerY->valueint, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);
								return PACKET_EXE_CONTINUE;
							}
 							pBeCollider->QueryCollideSet().insert(pair<std::string, UINT>(strKey.c_str(), 1));
 							pBeCollider->SetCurCollideFrame(nTs);
 							LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_CONFUSION m_MapCollideSet-2 Insert Is OK... strKey=[%s] nTs=[%d] nCollideId=[%d] pPowerX=[%d] pPowerY=[%d] nBeCollideId=[%d] pTType=[%d] pTPowerX=[%d] pTPowerY=[%d]", 
 								strKey.c_str(), nTs, nCollideId, pPowerX->valueint, pPowerY->valueint, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);
 						} 						

						///////////////////////////////////////////////////////////////////////
			            ///;广播同步帧缓存66ms 0:不缓存 1:缓存
			            if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 1)
			            {
							//插入服务器消息队列缓存-撞击者
							pBt->InsertFrameMsgCache(pModel->valueint, _WALK_ACTION_COLLIDE_CMD, nCollideId, pPowerX->valueint, pPowerY->valueint, pType->valueint, 0, 0, 0, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);
							//插入服务器消息队列缓存-被撞击者
							pBt->InsertFrameMsgCache(pModel->valueint, _WALK_ACTION_COLLIDE_CMD, nBeCollideId, pTPowerX->valueint, pTPowerY->valueint, pTType->valueint, 0, 0, 0, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);
						}
						else
						{	
							//广播碰撞信息给全队人员
							CMsgWalk msg;
							IF_OK(msg.CreateMsg(_WALK_ACTION_COLLIDE_CMD, nCollideId, pPowerX->valueint, pPowerY->valueint, pType->valueint, 0, 0, 0))
							{
								pBt->BroadcastTeamMsg(&msg);
							}
						}

						///////////////////////////////////////////////////////////////////////
			            ///;广播同步帧缓存66ms 0:不缓存 1:缓存
			            //if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 0)
			            {
			            	long long llCurMillSecTime = GetMillSecTime();
							//设置碰撞操作包开始-撞击者启动
							if (pCollider->GetSocket())
							{
								pCollider->GetSocket()->SetCollideSynMsgCount(0);
								pCollider->GetSocket()->SetCollideSynMsgTimeStamp(llCurMillSecTime);
								pCollider->GetSocket()->SetSynMsgStatus(_SYN_MSG_COLLIDE);
							}

							//设置碰撞操作包开始-被撞击者启动
							if (pBeCollider && pBeCollider->GetSocket())
							{
								pBeCollider->GetSocket()->SetCollideSynMsgCount(0);
								pBeCollider->GetSocket()->SetCollideSynMsgTimeStamp(llCurMillSecTime);
								pBeCollider->GetSocket()->SetSynMsgStatus(_SYN_MSG_COLLIDE);
							}

							//收到第一个命令包，设置时间戳
			            	long long mllRecvMsgTimeStamp = pBt->GetRecvMsgTimeStamp();
			            	if (mllRecvMsgTimeStamp == 0)
			            	{
			            		pBt->SetRecvMsgTimeStamp(llCurMillSecTime);
			            	}			            	
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
						//找到撞击者
						pCollider = pBt->GetMember(nCollideId);
						if (!pCollider)
						{
							pEnemyBrbt = pBt->GetEnemyTeam();
							if (!pEnemyBrbt)
							{
								LOGWARNING("[CMsgBattle::Process] pEnemyBrbt == NULL");
								return PACKET_EXE_CONTINUE;								
							}
							else
							{
								pCollider = pEnemyBrbt->GetMember(nCollideId);
								if (!pCollider)
								{
									LOGWARNING("[CMsgBattle::Process] pCollider == NULL");
									return PACKET_EXE_CONTINUE;
								}
							}
						}

						//找到被撞击者
						pBeCollider = pBt->GetMember(nBeCollideId);
						if (!pBeCollider)
						{
							pEnemyBrbt = pBt->GetEnemyTeam();
							if (!pEnemyBrbt)
							{
								LOGWARNING("[CMsgBattle::Process] pEnemyBrbt == NULL");
								return PACKET_EXE_CONTINUE;								
							}
							else
							{
								pBeCollider = pEnemyBrbt->GetMember(nBeCollideId);
								if (!pBeCollider)
								{
									LOGWARNING("[CMsgBattle::Process] pBeCollider == NULL");
									return PACKET_EXE_CONTINUE;
								}
							}
						}

						//检测碰撞帧序列，前面已经发生的，但是服务器已经处理了很后面的抛掉
						UINT nColliderCurFrame = pCollider->GetCurCollideFrame();
						if (nColliderCurFrame != 0 && nTs < nColliderCurFrame)
						{
							LOGWARNING("[CMsgBattle::Process] nTs < nColliderCurFrame nTs=[%d] nColliderCurFrame=[%d]", nTs, nColliderCurFrame);
							return PACKET_EXE_CONTINUE;
						}

						UINT nBeColliderCurFrame = pBeCollider->GetCurCollideFrame();
						if (nBeColliderCurFrame != 0 && nTs < nBeColliderCurFrame)
						{
							LOGWARNING("[CMsgBattle::Process] nTs < nBeColliderCurFrame nTs=[%d] nBeColliderCurFrame=[%d]", nTs, nBeColliderCurFrame);
							return PACKET_EXE_CONTINUE;
						}

						//判断是否超过指定长度，超过则删除之前的碰撞缓存数据
						//打印下日志
						// INT nIndex = 0;
						// MAP_COLLIDE_CIT iter = pCollider->QueryCollideSet().begin();
						// for (; iter != pCollider->QueryCollideSet().end() ; ++iter)
						// {
						// 	LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_RB_FIGHT_TEAM m_MapCollideSet-1 key==>[%d]==>[%s]", ++nIndex, iter->first.c_str());
						// }

						size_t nSize = pCollider->QueryCollideSet().size();
						if (nSize > (size_t)g_config.ONLINE_MAP_COLLIDE_SIZE)
						{	
							LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_RB_FIGHT_TEAM m_MapCollideSet-1 Erase Is Ok...Key=[%s] nSize=[%d]", pCollider->QueryCollideSet().begin()->first.c_str() ,nSize);
							pCollider->QueryCollideSet().erase(pCollider->QueryCollideSet().begin());  
						}
						LOGDEBUG("[CMsgWalk::Process] _BATTLE_MODEL_RB_FIGHT_TEAM m_MapCollideSet-1 Size=[%d]", pCollider->QueryCollideSet().size());

						std::string strKey = std::to_string(nTs) + "_" +std::to_string(nCollideId) + "_" + std::to_string(nBeCollideId);
						//记录玩家的碰撞信息
						if (pCollider->QueryCollideSet().find(strKey.c_str()) != pCollider->QueryCollideSet().end())
						{
							LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_RB_FIGHT_TEAM-m_MapCollideSet-1 Have Report... strKey=[%s] nTs=[%d] nCollideId=[%d] pPowerX=[%d] pPowerY=[%d] nBeCollideId=[%d] pTType=[%d] pTPowerX=[%d] pTPowerY=[%d]", 
								strKey.c_str(), nTs, nCollideId, pPowerX->valueint, pPowerY->valueint, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);
							return PACKET_EXE_CONTINUE;
						}

						pCollider->QueryCollideSet().insert(pair<std::string, UINT>(strKey.c_str(), 1));
						pCollider->SetCurCollideFrame(nTs);
 						LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_RB_FIGHT_TEAM m_MapCollideSet-1 Insert Is OK... strKey=[%s] nTs=[%d] nCollideId=[%d] pPowerX=[%d] pPowerY=[%d] nBeCollideId=[%d] pTType=[%d] pTPowerX=[%d] pTPowerY=[%d]", 
 							strKey.c_str(), nTs, nCollideId, pPowerX->valueint, pPowerY->valueint, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);

 						
 						if (pBeCollider)
 						{
 							strKey = std::to_string(nTs) + "_" +std::to_string(nBeCollideId) + "_" + std::to_string(nCollideId);
 							if (pBeCollider->QueryCollideSet().find(strKey.c_str()) != pBeCollider->QueryCollideSet().end())
							{
								LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_RB_FIGHT_TEAM m_MapCollideSet-2 Have Report... strKey=[%s] nTs=[%d] nCollideId=[%d] pPowerX=[%d] pPowerY=[%d] nBeCollideId=[%d] pTType=[%d] pTPowerX=[%d] pTPowerY=[%d]", 
									strKey.c_str(), nTs, nCollideId, pPowerX->valueint, pPowerY->valueint, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);
								return PACKET_EXE_CONTINUE;
							}
 							pBeCollider->QueryCollideSet().insert(pair<std::string, UINT>(strKey.c_str(), 1));
 							pBeCollider->SetCurCollideFrame(nTs);
 							LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_RB_FIGHT_TEAM m_MapCollideSet-2 Insert Is OK... strKey=[%s] nTs=[%d] nCollideId=[%d] pPowerX=[%d] pPowerY=[%d] nBeCollideId=[%d] pTType=[%d] pTPowerX=[%d] pTPowerY=[%d]", 
 								strKey.c_str(), nTs, nCollideId, pPowerX->valueint, pPowerY->valueint, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);
 						}
						
						//找出击杀者战队
						CBattleTeam* pEnemyBrbt = pBt->GetEnemyTeam();
						if (!pEnemyBrbt)
						{
							LOGERROR("[CMsgWalk::Process] Is Error pEnemyBrbt==NULL");
							return PACKET_EXE_ERROR;
						}

						///////////////////////////////////////////////////////////////////////
			            ///;广播同步帧缓存66ms 0:不缓存 1:缓存
			            if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 1)
			            {
							//插入服务器消息队列缓存-撞击者
							pBt->InsertFrameMsgCache(pModel->valueint, _WALK_ACTION_COLLIDE_CMD, nCollideId, pPowerX->valueint, pPowerY->valueint, pType->valueint, 0, 0, 0, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);
							//插入服务器消息队列缓存-被撞击者
							//同一个战队的，插入到自己的队伍缓存队列
							if (pBeCollider && pBeCollider->GetBrbt() == pCollider->GetBrbt())
							{
								pBt->InsertFrameMsgCache(pModel->valueint, _WALK_ACTION_COLLIDE_CMD, nBeCollideId, pTPowerX->valueint, pTPowerY->valueint, pTType->valueint, 0, 0, 0, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);
							}
							//存在撞击着并且不是同一个队伍（属于敌方战队的玩家撞击）则插入到敌方战队缓存队列
							else if (pBeCollider && pBeCollider->GetBrbt() != pCollider->GetBrbt() && pEnemyBrbt)
							{
								pEnemyBrbt->InsertFrameMsgCache(pModel->valueint, _WALK_ACTION_COLLIDE_CMD, nBeCollideId, pTPowerX->valueint, pTPowerY->valueint, pTType->valueint, 0, 0, 0, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);
							}
						}
						else
						{	
							//找出击杀者战队
							CBattleTeam* pEnemyBrbt = pBt->GetEnemyTeam();
							//广播碰撞信息给全队人员
							CMsgWalk msg;
							IF_OK(msg.CreateMsg(_WALK_ACTION_COLLIDE_CMD, nCollideId, pPowerX->valueint, pPowerY->valueint, pType->valueint, 0, 0, 0))
							{
								//被击杀者队伍中广播
								pBt->BroadcastTeamMsg(&msg);
								//击杀者队伍中广播
								if(pEnemyBrbt)
								{
									pEnemyBrbt->BroadcastTeamMsg(&msg);
								}
							}
						}

						///////////////////////////////////////////////////////////////////////
			            ///;广播同步帧缓存66ms 0:不缓存 1:缓存
			            //if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 0)
			            {
			            	long long llCurMillSecTime = GetMillSecTime();
							//设置碰撞操作包开始-撞击者启动
							if (pCollider->GetSocket())
							{
								pCollider->GetSocket()->SetCollideSynMsgCount(0);
								pCollider->GetSocket()->SetCollideSynMsgTimeStamp(llCurMillSecTime);
								pCollider->GetSocket()->SetSynMsgStatus(_SYN_MSG_COLLIDE);
							}

							//设置碰撞操作包开始-被撞击者启动
							if (pBeCollider && pBeCollider->GetSocket())
							{
								pBeCollider->GetSocket()->SetCollideSynMsgCount(0);
								pBeCollider->GetSocket()->SetCollideSynMsgTimeStamp(llCurMillSecTime);
								pBeCollider->GetSocket()->SetSynMsgStatus(_SYN_MSG_COLLIDE);
							}

							//收到第一个命令包，设置时间戳
			            	long long mllRecvMsgTimeStamp = pBt->GetRecvMsgTimeStamp();
			            	if (mllRecvMsgTimeStamp == 0)
			            	{
			            		pBt->SetRecvMsgTimeStamp(llCurMillSecTime);
			            	}			            	
						}
					}	
				}
#endif
				///--------------方案二、撞击者A和撞击者B两个玩家负责上传碰撞信息，此时如果出现都网络不好的情况，则会影响到其他玩家的渲染
				///--------------当玩家A或者玩家B上报的过程中，玩家C执行了服务器返回的自己的前进操作命令的时候，此时还未收到A_B碰撞移动事件
				///--------------玩家C进行处理自己的移动事件，此时和B发生碰撞，立刻上报C_B的碰撞事件
				///--------------当A_B的事件在上报的路上的时候(由A和B事件发生者上报的)，服务器收到C的碰撞事件，进行广播，当服务器收到A_B的碰撞的事件时，
				///--------------判定服务器的帧数已经过期，则直接抛弃A_B的数据帧。
				///--------------当A_B的事件在下发的路径上的时候，此时服务器收到C_B的事件，则会进行处理，所有客户端正常收到A_B 、C_B事件
				///--------------如果服务器此时再插几个A_B的空帧，则在其他客户端表现上就会出现先执行A_B、几个空帧、然后C_B的事件，会出现不同步
				///--------------优点：处理简单、需要参与广播的成员少、流量少、减少玩家的流量压力
				///--------------缺点：一旦出现A、B网络都不好的时候，会出现不同步的情况。服务器或者前端需要进行特殊的包碰撞帧检测和抛帧处理--------///
#if 1
				cJSON* pType = cJSON_GetObjectItem(pJsonMsg, "type");					// int  类型
				cJSON* pPowerX = cJSON_GetObjectItem(pJsonMsg, "power_x");				// float 冲量x
				cJSON* pPowerY = cJSON_GetObjectItem(pJsonMsg, "power_y");				// float 冲量y

				//被撞击者的数据
				cJSON* pBeCollideId = cJSON_GetObjectItem(pJsonMsg, "becollide_id");		// int  被撞击着的ID
				cJSON* pTType = cJSON_GetObjectItem(pJsonMsg, "t_type");					// int  类型
				cJSON* pTPowerX = cJSON_GetObjectItem(pJsonMsg, "t_power_x");				// float 冲量x
				cJSON* pTPowerY = cJSON_GetObjectItem(pJsonMsg, "t_power_y");				// float 冲量y
				cJSON* pTS = cJSON_GetObjectItem(pJsonMsg, "ts");							// int //服务器数据包时间帧序列
				CHECKF(pPowerX && pPowerY && pType && pBeCollideId && pTType && pTPowerX && pTPowerY && pTS);

				UINT nCollideId = m_pPlayer->GetUid();
				UINT nBeCollideId = pBeCollideId->valueint;
				UINT nTs = pTS->valueint;
				Player* pCollider = m_pPlayer;
				Player* pBeCollider = NULL;
				//CBattleTeam* pEnemyBrbt = NULL;

				//大乱斗模式
				if (pModel->valueint == _BATTLE_MODEL_CONFUSION)
				{	
					pBt = pCollider->GetBct();
					if(pBt)
					{
						// INT nIndex = 0;
						// MAP_COLLIDE_CIT iter = pCollider->QueryCollideSet().begin();
						// for (; iter != pCollider->QueryCollideSet().end() ; ++iter)
						// {
						// 	LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_CONFUSION m_MapCollideSet-1 key==>[%d]==>[%s]", ++nIndex, iter->first.c_str());
						// }
						
						//判断是否超过指定长度，超过则删除之前的碰撞缓存数据
						size_t nSize = pCollider->QueryCollideSet().size();
						if (nSize > (size_t)g_config.ONLINE_MAP_COLLIDE_SIZE)
						{	
							LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_CONFUSION m_MapCollideSet-1 Erase Is Ok...Key=[%s] nSize=[%d]", pCollider->QueryCollideSet().begin()->first.c_str() ,nSize);
							pCollider->QueryCollideSet().erase(pCollider->QueryCollideSet().begin());  
						}
						LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_CONFUSION m_MapCollideSet-1 Size=[%d]", nSize);

						std::string strKey = std::to_string(nTs) + "_" +std::to_string(nCollideId) + "_" + std::to_string(nBeCollideId);
						//记录玩家的碰撞信息
						if (pCollider->QueryCollideSet().find(strKey.c_str()) != pCollider->QueryCollideSet().end())
						{
							LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_CONFUSION m_MapCollideSet-1 Have Report... strKey=[%s] nTs=[%d] nCollideId=[%d] pPowerX=[%d] pPowerY=[%d] nBeCollideId=[%d] pTType=[%d] pTPowerX=[%d] pTPowerY=[%d]", 
								strKey.c_str(), nTs, nCollideId, pPowerX->valueint, pPowerY->valueint, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);
							return PACKET_EXE_CONTINUE;
						}

						pCollider->QueryCollideSet().insert(pair<std::string, UINT>(strKey.c_str(), 1));
 						LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_CONFUSION m_MapCollideSet-1 Insert Is OK... strKey=[%s] nTs=[%d] nCollideId=[%d] pPowerX=[%d] pPowerY=[%d] nBeCollideId=[%d] pTType=[%d] pTPowerX=[%d] pTPowerY=[%d]", 
 							strKey.c_str(), nTs, nCollideId, pPowerX->valueint, pPowerY->valueint, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);

 						pBeCollider = g_pPlayerPool->GetPlayerByUid(nBeCollideId);
 						if (pBeCollider)
 						{
 							strKey = std::to_string(nTs) + "_" +std::to_string(nBeCollideId) + "_" + std::to_string(nCollideId);
 							if (pBeCollider->QueryCollideSet().find(strKey.c_str()) != pBeCollider->QueryCollideSet().end())
							{
								LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_CONFUSION m_MapCollideSet-2 Have Report... strKey=[%s] nTs=[%d] nCollideId=[%d] pPowerX=[%d] pPowerY=[%d] nBeCollideId=[%d] pTType=[%d] pTPowerX=[%d] pTPowerY=[%d]", 
									strKey.c_str(), nTs, nCollideId, pPowerX->valueint, pPowerY->valueint, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);
								return PACKET_EXE_CONTINUE;
							}
 							pBeCollider->QueryCollideSet().insert(pair<std::string, UINT>(strKey.c_str(), 1));
 							LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_CONFUSION m_MapCollideSet-2 Insert Is OK... strKey=[%s] nTs=[%d] nCollideId=[%d] pPowerX=[%d] pPowerY=[%d] nBeCollideId=[%d] pTType=[%d] pTPowerX=[%d] pTPowerY=[%d]", 
 								strKey.c_str(), nTs, nCollideId, pPowerX->valueint, pPowerY->valueint, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);
 						} 						

						///////////////////////////////////////////////////////////////////////
			            ///;广播同步帧缓存66ms 0:不缓存 1:缓存
			            if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 1)
			            {
							//插入服务器消息队列缓存-撞击者
							pBt->InsertFrameMsgCache(pModel->valueint, _WALK_ACTION_COLLIDE_CMD, nCollideId, pPowerX->valueint, pPowerY->valueint, pType->valueint, 0, 0, 0, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);
							//插入服务器消息队列缓存-被撞击者
							pBt->InsertFrameMsgCache(pModel->valueint, _WALK_ACTION_COLLIDE_CMD, nBeCollideId, pTPowerX->valueint, pTPowerY->valueint, pTType->valueint, 0, 0, 0, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);
						}
						else
						{	
							//广播碰撞信息给全队人员
							CMsgWalk msg;
							IF_OK(msg.CreateMsg(_WALK_ACTION_COLLIDE_CMD, nCollideId, pPowerX->valueint, pPowerY->valueint, pType->valueint, 0, 0, 0))
							{
								pBt->BroadcastTeamMsg(&msg);
							}
						}

						///////////////////////////////////////////////////////////////////////
			            ///;广播同步帧缓存66ms 0:不缓存 1:缓存
			            //if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 0)
			            {
			            	long long llCurMillSecTime = GetMillSecTime();
							//设置碰撞操作包开始-撞击者启动
							if (pCollider->GetSocket())
							{
								pCollider->GetSocket()->SetCollideSynMsgCount(0);
								pCollider->GetSocket()->SetCollideSynMsgTimeStamp(llCurMillSecTime);
								pCollider->GetSocket()->SetSynMsgStatus(_SYN_MSG_COLLIDE);
							}

							//设置碰撞操作包开始-被撞击者启动
							if (pBeCollider && pBeCollider->GetSocket())
							{
								pBeCollider->GetSocket()->SetCollideSynMsgCount(0);
								pBeCollider->GetSocket()->SetCollideSynMsgTimeStamp(llCurMillSecTime);
								pBeCollider->GetSocket()->SetSynMsgStatus(_SYN_MSG_COLLIDE);
							}

							//收到第一个命令包，设置时间戳
			            	long long mllRecvMsgTimeStamp = pBt->GetRecvMsgTimeStamp();
			            	if (mllRecvMsgTimeStamp == 0)
			            	{
			            		pBt->SetRecvMsgTimeStamp(llCurMillSecTime);
			            	}			            	
						}
					}
				}
				//红蓝队对战模式
				else if (pModel->valueint == _BATTLE_MODEL_RB_FIGHT_TEAM)
				{
					//找出自己战队
					pBt = pCollider->GetBrbt();
					if(pBt)
					{
						//判断是否超过指定长度，超过则删除之前的碰撞缓存数据
						//打印下日志
						// INT nIndex = 0;
						// MAP_COLLIDE_CIT iter = pCollider->QueryCollideSet().begin();
						// for (; iter != pCollider->QueryCollideSet().end() ; ++iter)
						// {
						// 	LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_RB_FIGHT_TEAM m_MapCollideSet-1 key==>[%d]==>[%s]", ++nIndex, iter->first.c_str());
						// }

						size_t nSize = pCollider->QueryCollideSet().size();
						if (nSize > (size_t)g_config.ONLINE_MAP_COLLIDE_SIZE)
						{	
							LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_RB_FIGHT_TEAM m_MapCollideSet-1 Erase Is Ok...Key=[%s] nSize=[%d]", pCollider->QueryCollideSet().begin()->first.c_str() ,nSize);
							pCollider->QueryCollideSet().erase(pCollider->QueryCollideSet().begin());  
						}
						LOGDEBUG("[CMsgWalk::Process] _BATTLE_MODEL_RB_FIGHT_TEAM m_MapCollideSet-1 Size=[%d]", pCollider->QueryCollideSet().size());

						std::string strKey = std::to_string(nTs) + "_" +std::to_string(nCollideId) + "_" + std::to_string(nBeCollideId);
						//记录玩家的碰撞信息
						if (pCollider->QueryCollideSet().find(strKey.c_str()) != pCollider->QueryCollideSet().end())
						{
							LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_RB_FIGHT_TEAM-m_MapCollideSet-1 Have Report... strKey=[%s] pTS=[%d] nCollideId=[%d] pPowerX=[%d] pPowerY=[%d] nBeCollideId=[%d] pTType=[%d] pTPowerX=[%d] pTPowerY=[%d]", 
								strKey.c_str(), pTS->valueint, nCollideId, pPowerX->valueint, pPowerY->valueint, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);
							return PACKET_EXE_CONTINUE;
						}

						pCollider->QueryCollideSet().insert(pair<std::string, UINT>(strKey.c_str(), 1));
 						LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_RB_FIGHT_TEAM m_MapCollideSet-1 Insert Is OK... strKey=[%s] pTS=[%d] nCollideId=[%d] pPowerX=[%d] pPowerY=[%d] nBeCollideId=[%d] pTType=[%d] pTPowerX=[%d] pTPowerY=[%d]", 
 							strKey.c_str(), pTS->valueint, nCollideId, pPowerX->valueint, pPowerY->valueint, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);

 						pBeCollider = g_pPlayerPool->GetPlayerByUid(nBeCollideId);
 						if (pBeCollider)
 						{
 							strKey = std::to_string(nTs) + "_" +std::to_string(nBeCollideId) + "_" + std::to_string(nCollideId);
 							if (pBeCollider->QueryCollideSet().find(strKey.c_str()) != pBeCollider->QueryCollideSet().end())
							{
								LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_RB_FIGHT_TEAM m_MapCollideSet-2 Have Report... strKey=[%s] pTS=[%d] nCollideId=[%d] pPowerX=[%d] pPowerY=[%d] nBeCollideId=[%d] pTType=[%d] pTPowerX=[%d] pTPowerY=[%d]", 
									strKey.c_str(), pTS->valueint, nCollideId, pPowerX->valueint, pPowerY->valueint, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);
								return PACKET_EXE_CONTINUE;
							}
 							pBeCollider->QueryCollideSet().insert(pair<std::string, UINT>(strKey.c_str(), 1));
 							LOGDEBUG("[CMsgWalk::Process]-_BATTLE_MODEL_RB_FIGHT_TEAM m_MapCollideSet-2 Insert Is OK... strKey=[%s] pTS=[%d] nCollideId=[%d] pPowerX=[%d] pPowerY=[%d] nBeCollideId=[%d] pTType=[%d] pTPowerX=[%d] pTPowerY=[%d]", 
 								strKey.c_str(), pTS->valueint, nCollideId, pPowerX->valueint, pPowerY->valueint, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);
 						}

 						//找出击杀者战队
						CBattleTeam* pEnemyBrbt = pBt->GetEnemyTeam();
						if (!pEnemyBrbt)
						{
							LOGERROR("[CMsgWalk::Process] Is Error pEnemyBrbt==NULL");
							return PACKET_EXE_ERROR;
						}

						///////////////////////////////////////////////////////////////////////
			            ///;广播同步帧缓存66ms 0:不缓存 1:缓存
			            if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 1)
			            {
							//插入服务器消息队列缓存-撞击者
							pBt->InsertFrameMsgCache(pModel->valueint, _WALK_ACTION_COLLIDE_CMD, nCollideId, pPowerX->valueint, pPowerY->valueint, pType->valueint, 0, 0, 0, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);
							//插入服务器消息队列缓存-被撞击者
							//同一个战队的，插入到自己的队伍缓存队列
							if (pBeCollider && pBeCollider->GetBrbt() == pCollider->GetBrbt())
							{
								pBt->InsertFrameMsgCache(pModel->valueint, _WALK_ACTION_COLLIDE_CMD, nBeCollideId, pTPowerX->valueint, pTPowerY->valueint, pTType->valueint, 0, 0, 0, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);
							}
							//存在撞击着并且不是同一个队伍（属于敌方战队的玩家撞击）则插入到敌方战队缓存队列
							else if (pBeCollider && pBeCollider->GetBrbt() != pCollider->GetBrbt() && pEnemyBrbt)
							{
								pEnemyBrbt->InsertFrameMsgCache(pModel->valueint, _WALK_ACTION_COLLIDE_CMD, nBeCollideId, pTPowerX->valueint, pTPowerY->valueint, pTType->valueint, 0, 0, 0, nBeCollideId, pTType->valueint, pTPowerX->valueint, pTPowerY->valueint);
							}							
						}
						else
						{								
							//广播碰撞信息给全队人员
							CMsgWalk msg;
							IF_OK(msg.CreateMsg(_WALK_ACTION_COLLIDE_CMD, nCollideId, pPowerX->valueint, pPowerY->valueint, pType->valueint, 0, 0, 0))
							{
								//被击杀者队伍中广播
								pBt->BroadcastTeamMsg(&msg);
								//击杀者队伍中广播
								if(pEnemyBrbt)
								{
									pEnemyBrbt->BroadcastTeamMsg(&msg);
								}
							}
						}

						///////////////////////////////////////////////////////////////////////
			            ///;广播同步帧缓存66ms 0:不缓存 1:缓存
			            //if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 0)
			            {
			            	long long llCurMillSecTime = GetMillSecTime();
							//设置碰撞操作包开始-撞击者启动
							if (pCollider->GetSocket())
							{
								pCollider->GetSocket()->SetCollideSynMsgCount(0);
								pCollider->GetSocket()->SetCollideSynMsgTimeStamp(llCurMillSecTime);
								pCollider->GetSocket()->SetSynMsgStatus(_SYN_MSG_COLLIDE);
							}

							//设置碰撞操作包开始-被撞击者启动
							if (pBeCollider && pBeCollider->GetSocket())
							{
								pBeCollider->GetSocket()->SetCollideSynMsgCount(0);
								pBeCollider->GetSocket()->SetCollideSynMsgTimeStamp(llCurMillSecTime);
								pBeCollider->GetSocket()->SetSynMsgStatus(_SYN_MSG_COLLIDE);
							}

							//收到第一个命令包，设置时间戳
			            	long long mllRecvMsgTimeStamp = pBt->GetRecvMsgTimeStamp();
			            	if (mllRecvMsgTimeStamp == 0)
			            	{
			            		pBt->SetRecvMsgTimeStamp(llCurMillSecTime);
			            	}			            	
						}
					}	
				}
#endif
				///--------------方案一、撞击者A和撞击者B两个玩家各自负责上传自己的碰撞信息，此时如果出现都网络不好的情况，则会影响到其他玩家的渲染，
				///在其他玩家的渲染上由于事件发生者网络不好或者没有上报导致其他玩家渲染不了新状态---------------///
#if 0
				cJSON* pType = cJSON_GetObjectItem(pJsonMsg, "type");					// int  类型
				cJSON* pPowerX = cJSON_GetObjectItem(pJsonMsg, "power_x");				// float 冲量x
				cJSON* pPowerY = cJSON_GetObjectItem(pJsonMsg, "power_y");				// float 冲量y
				CHECKF(pPowerX && pPowerY && pType);

				UINT nTargetId = m_pPlayer->GetUid();

				//大乱斗模式
				if (pModel->valueint == _BATTLE_MODEL_CONFUSION)
				{	
					pBt = m_pPlayer->GetBct();
					if(pBt)
					{
						///////////////////////////////////////////////////////////////////////
			            ///;广播同步帧缓存66ms 0:不缓存 1:缓存
			            if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 1)
			            {
							//插入服务器消息队列缓存
							pBt->InsertFrameMsgCache(pModel->valueint, _WALK_ACTION_COLLIDE_CMD, nTargetId, pPowerX->valueint, pPowerY->valueint, pType->valueint, 0, 0, 0);
						}
						else
						{	
							//广播碰撞信息给全队人员
							CMsgWalk msg;
							IF_OK(msg.CreateMsg(_WALK_ACTION_COLLIDE_CMD, nTargetId, pPowerX->valueint, pPowerY->valueint, pType->valueint, 0, 0, 0))
							{
								pBt->BroadcastTeamMsg(&msg);
							}
						}

						///////////////////////////////////////////////////////////////////////
			            ///;广播同步帧缓存66ms 0:不缓存 1:缓存
			            //if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 0)
			            {
							//设置碰撞操作包开始
							if (m_pPlayer->GetSocket())
							{
								long long llCurMillSecTime = GetMillSecTime();
								m_pPlayer->GetSocket()->SetCollideSynMsgCount(0);
								m_pPlayer->GetSocket()->SetCollideSynMsgTimeStamp(llCurMillSecTime);
								m_pPlayer->GetSocket()->SetSynMsgStatus(_SYN_MSG_COLLIDE);
							}

							//收到第一个命令包，设置时间戳
			            	long long llCurMillSecTime = GetMillSecTime();
			            	long long mllRecvMsgTimeStamp = pBt->GetRecvMsgTimeStamp();
			            	if (mllRecvMsgTimeStamp == 0)
			            	{
			            		pBt->SetRecvMsgTimeStamp(llCurMillSecTime);
			            	}
			            	
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
						///////////////////////////////////////////////////////////////////////
			            ///;广播同步帧缓存66ms 0:不缓存 1:缓存
			            if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 1)
			            {
							//插入服务器消息队列缓存
							pBt->InsertFrameMsgCache(pModel->valueint, _WALK_ACTION_COLLIDE_CMD, nTargetId, pPowerX->valueint, pPowerY->valueint, pType->valueint, 0, 0, 0);
						}
						else
						{	
							//找出击杀者战队
							CBattleTeam* pEnemyBrbt = pBt->GetEnemyTeam();
							//广播碰撞信息给全队人员
							CMsgWalk msg;
							IF_OK(msg.CreateMsg(_WALK_ACTION_COLLIDE_CMD, nTargetId, pPowerX->valueint, pPowerY->valueint, pType->valueint, 0, 0, 0))
							{
								//被击杀者队伍中广播
								pBt->BroadcastTeamMsg(&msg);
								//击杀者队伍中广播
								if(pEnemyBrbt)
								{
									pEnemyBrbt->BroadcastTeamMsg(&msg);
								}
							}
						}

						///////////////////////////////////////////////////////////////////////
			            ///;广播同步帧缓存66ms 0:不缓存 1:缓存
			            //if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 0)
			            {
							//设置碰撞操作包开始
							if (m_pPlayer->GetSocket())
							{
								long long llCurMillSecTime = GetMillSecTime();
								m_pPlayer->GetSocket()->SetCollideSynMsgCount(0);
								m_pPlayer->GetSocket()->SetCollideSynMsgTimeStamp(llCurMillSecTime);
								m_pPlayer->GetSocket()->SetSynMsgStatus(_SYN_MSG_COLLIDE);
							}

							//收到第一个命令包，设置时间戳
			            	long long llCurMillSecTime = GetMillSecTime();
			            	long long mllRecvMsgTimeStamp = pBt->GetRecvMsgTimeStamp();
			            	if (mllRecvMsgTimeStamp == 0)
			            	{
			            		pBt->SetRecvMsgTimeStamp(llCurMillSecTime);
			            	}			            	
						}
					}	
				}
#endif

				return PACKET_EXE_CONTINUE;	

			}
			break;
		}
	}

	return PACKET_EXE_CONTINUE;
	DEBUG_CATCH0("[CMsgWalk::Process] Is Catch...");
}