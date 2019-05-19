//////////////////////////////////////////////////////////////////////
//文件名称：CBattleTeam.cpp
//功能描述：
//版本说明：CBattleTeam API
//			
//编写作者：2018.10.11 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include "../Common/Type.h"
#include "../Common/BaseType.h"
#include "../Common/Thread.h"
//!alter by huyf 2018.04.23:添加连接开关可配置 
#include "../Common/Config.h"
//!alter end:添加连接开关可配置 
#include "../NetBase/ClientSocket.h"

#include "BattleTeam.h"
#include "Player.h"
//#include "../MsgHandler/MsgBattle.h"
//#include "../MsgHandler/MsgSynRoomInfo.h"
//#include "../MsgHandler/MsgAction.h"
//#include "../MsgHandler/MsgUserInfo.h"
//#include "../MsgHandler/MsgItem.h"
#include "../MsgHandler/AllMsg.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <functional>
#include <vector>
#include <string>
using namespace std;
#include "Battle/BattleCfg.h"

//初始化静态成员变量
//UINT CBattleTeam::m_id = 0;
static int _FIGHT_CREATE_RANDOM_ITEM_ID = 1;	//生成道具的唯一id
static int _MAX_TEAM_ID	= 1;						//队伍ID


//对积分进行排序
bool less_(const BattleConfusionTeamTopInfo & obj1, const BattleConfusionTeamTopInfo & obj2) 
{
    return obj1.nSorce > obj2.nSorce;
}

//////////////////////////////////////////////////////////////////////
//class
//构造函数
CBattleTeam::CBattleTeam()
{
	Init();
}

//析构函数
CBattleTeam::~CBattleTeam()
{
	Destroy();
}	

//是否已经战斗结束
BOOL CBattleTeam::IsFightConfusionFinsh()
{
	time_t tmCur = time(NULL);
	//!alter by huyf 2019.01.11:游戏中其他玩家主动退出的时候，没有判断队伍人数是是否剩余1人的情况，现阶段剩余1人则结束游戏
	//!alter by huyf 2018.11.01:此处不会出现0、1人的情况，因为再异常和玩家主动退出的已经进行了人数判定
	//战斗时间耗尽 大乱斗的在战人数为1个人的时候，则判定为对战结束,0个人单线程不会触发
	if((m_nTimeStamp != 0 && (m_nTimeStamp + BattleCfg()->GetBccMaxTime()) < tmCur) ||
		(GetMemberAmount() == 1 || GetMemberAmount() == 0))
	{
		m_nTeamStatus = _TEAM_STATUS_END;
		return TRUE;
	}
	//!alter end:此处不会出现0、1人的情况，因为再异常和玩家主动退出的已经进行了人数判定
	//!alter end:游戏中其他玩家主动退出的时候，没有判断队伍人数是是否剩余1人的情况，现阶段剩余1人则结束游戏
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//队伍销毁,慎用
VOID CBattleTeam::Destroy()
{
	m_id = 0;
	m_vtTeamPlayerS.clear();
	m_nTimeStamp = time(NULL);	
	m_mapTeamNetMsg.clear();
	m_llRecvMsgTimeStamp = 0;
}

//////////////////////////////////////////////////////////////////////
//初始化init
VOID CBattleTeam::Init()
{
	m_id = _MAX_TEAM_ID++;
	m_vtTeamPlayerS.clear();
	m_nTimeStamp = 0;
	SetTeamStatus(_TEAM_STATUS_CREATE);
	m_vItemList.clear();
	m_pKingPlayer = NULL;
	m_nKingSorce = 0;
	m_mapTeamNetMsg.clear();
	m_llRecvMsgTimeStamp = 0;

	m_nFightTeamColour = _FIGHT_OWNER_COLOUR_NONE;
	m_nFightTeamKillSum = 0;
	m_nFightTotalScore = 0;
	m_nFightResult = 0;
	m_pEnemyTeam = NULL;
	m_nQuickMatchTimeStamp = 0;
	m_tFightTimeInterval.Clear();
	LOGERROR("[CBattleTeam::Init] ★★★【m_id = %d】***", m_id);
}

//开始战斗更新队伍状态
BOOL CBattleTeam::FightStart()
{
	SetTeamStatus(_TEAM_STATUS_START);
	m_tFightTimeInterval.SetInterval(BattleCfg()->GetBccRandomCreateItemIntervalSecs());
	m_tFightTimeInterval.Update();
	m_nTimeStamp = time(NULL);
	return TRUE;
}

//准备状态
BOOL CBattleTeam::FightQuickMatch()
{
	SetTeamStatus(_TEAM_STATUS_QIUCK_MATCH);
	m_nQuickMatchTimeStamp = time(NULL);
	return TRUE;
}

//是否快速匹配超时
BOOL CBattleTeam::IsQuickMatchTimeout()
{
	if (m_nQuickMatchTimeStamp != 0 && m_nQuickMatchTimeStamp + BattleCfg()->GetBccMaxQuickMatchWaitTime() < time(NULL))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CBattleTeam::ToNextTime()
{
	if(m_tFightTimeInterval.IsActive())
	{
		if(m_tFightTimeInterval.IsTimeOut())
			return TRUE;
	}
	return FALSE;
}

//添加队伍成员
BOOL CBattleTeam::AddMember(Player* pPlayer, INT nModelType)
{
	CHECKF(pPlayer);
	if (nModelType == _BATTLE_MODEL_CONFUSION)
	{
		if(GetMemberAmount() < BattleCfg()->GetBccMaxTeamAmount())
		{
			m_vtTeamPlayerS.push_back(pPlayer);
			LOGDEBUG("[CBattleTeam::AddMember] TeamId=[%d] Size=[%d]-1\n", m_id, GetMemberAmount());
			return TRUE;
		}
	}
	else //if (nModelType == _BATTLE_MODEL_RB_FIGHT_TEAM)
	{
		if(GetMemberAmount() < _MAX_RB_TEAMAMOUNT)
		{
			//设置玩家的车辆等级为1级
        	pPlayer->SetFightLevel(1);

			m_vtTeamPlayerS.push_back(pPlayer);
			LOGDEBUG("[CBattleTeam::AddMember] TeamId=[%d] Size=[%d]-2\n", m_id, GetMemberAmount());
			return TRUE;
		}
	}
	return FALSE;	
}
//清理队伍中所有队员
BOOL CBattleTeam::ClearMember()
{
	for (auto iter = m_vtTeamPlayerS.begin(); iter != m_vtTeamPlayerS.end(); iter++)
    {	
    	LOGDEBUG("[CBattleTeam::ClearMember] pPlayerId=[%d] DeadScore=[_STATUS_ALIVE] GameStep=[_GAME_STEP_NONE]", (*iter)->GetUid());
    	//清空队伍的队伍指针
    	(*iter)->ClearBct();
    	//清空队伍的队伍指针
    	(*iter)->ClearBrbt();

    	//设置活着的状态
    	(*iter)->SetDeadStatus(_STATUS_ALIVE);
    	//初始化玩家游戏step
    	(*iter)->SetGameStep(_GAME_STEP_NONE);
    }

    //清空队列
	m_vtTeamPlayerS.clear();

	//初始值
	//SetEnemyTeam(NULL);

	return TRUE;
}
//根据索引获取队伍成员
Player* CBattleTeam::GetMemberByIndex(size_t nIndex)
{
	if (nIndex >= m_vtTeamPlayerS.size() || nIndex < 0)
		return NULL;

	return m_vtTeamPlayerS[nIndex];
}

//根据玩家id获取队伍成员
Player* CBattleTeam::GetMember(UINT nPlayerId)
{
	for (auto iter = m_vtTeamPlayerS.begin(); iter != m_vtTeamPlayerS.end(); iter++)
    {	
    	//找到对应玩家
    	if((*iter)->GetUid() ==  nPlayerId)
    	{
    		return (*iter);
    	}
    }
    return NULL;
}

//删除队伍成员
BOOL CBattleTeam::DelMember(Player* pPlayer)
{
	//TEAMPLAYERSET::iterator iter = find(m_vtTeamPlayerS.begin(), m_vtTeamPlayerS.end(), _player);
	//if(iter != m_vtTeamPlayerS.end())
	//	m_vtTeamPlayerS.erase(iter);

	for (auto iter = m_vtTeamPlayerS.begin(); iter != m_vtTeamPlayerS.end(); iter++)
    {	
    	//找到对应玩家
    	if((*iter) == pPlayer)
    	{
    		m_vtTeamPlayerS.erase(iter);
    		LOGDEBUG("[CBattleTeam::DelMember] TeamId=[%d] Size=[%d]\n", m_id, GetMemberAmount());
    		return TRUE;
    	}
    }
    return FALSE;
}

//////////////////////////////////////////////////////////////////////
//获取队伍成员数量
UINT CBattleTeam::GetMemberAmount()
{    
	return ((UINT)BattleCfg()->GetBccMaxTeamAmount() > (m_vtTeamPlayerS.size()) ? m_vtTeamPlayerS.size() : (UINT)BattleCfg()->GetBccMaxTeamAmount());
}

//////////////////////////////////////////////////////////////////////
//是否为队伍成员
BOOL CBattleTeam::IsTeamMember(Player* pPlayer)
{
	size_t nSize = GetMemberAmount();
	for(size_t i=0; i<nSize; i++)
	{
		Player* pMember = GetMemberByIndex(i); 
		if(pMember && pMember == pPlayer)
			return TRUE;
	}

	return FALSE;
}
//////////////////////////////////////////////////////////////////////
//是否有活着的队伍成员
BOOL CBattleTeam::IsAliveMember()
{
	size_t nSize = GetMemberAmount();
	for(size_t i=0; i<nSize; i++)
	{
		Player* pMember = GetMemberByIndex(i); 
		if(pMember && pMember->GetDeadStatus() == _STATUS_ALIVE)
			return TRUE;
	}

	return FALSE;
}
//////////////////////////////////////////////////////////////////////
//更新霸主玩家指针
VOID CBattleTeam::UpdateKingPlayer()
{	 
	m_nKingSorce = 0;
	for (auto iter = m_vtTeamPlayerS.begin(); iter != m_vtTeamPlayerS.end(); iter++)
    {	
    	//找到对应玩家
    	if((*iter)->GetFightScore() > (UInt32)m_nKingSorce)
    	{
    		m_nKingSorce = (*iter)->GetFightScore();
    		m_pKingPlayer = (*iter);
    		LOGDEBUG("[CBattleTeam::UpdateKingPlayer] m_nKingSorce=[%d] m_pKingPlayerID=[%d]\n",m_nKingSorce, m_pKingPlayer->GetUid()); 
    		continue;   		
    	}
    }
}

//////////////////////////////////////////////////////////////////////
//广播消息给队伍成员
VOID CBattleTeam::BroadcastTeamMsg(CNetMsg* pMsg, Player* pSender/*=NULL*/)
{
	if(!pMsg)
		return;

	size_t nSize = GetMemberAmount();
	for(size_t i=0; i<nSize; i++)
	{
		Player* pMember	= this->GetMemberByIndex(i);
		if(pMember)
		{
			if (pMember != pSender)
				pMember->SendMsgToClient(pMsg);
		}		
	}
}

//////////////////////////////////////////////////////////////////////
//整合A-B队伍
BOOL CBattleTeam::MergeOtherTeam(INT nModelType, CBattleTeam* pOtherTeam)
{
	CHECKF(pOtherTeam);

	//大乱斗模式
	if (nModelType == _FIGHT_MODEL_TYPE_CONFUSION)
	{
		//把pOwnerbt队伍成员 广播添加到 pOtherTeam队伍成员
		size_t nSize = GetMemberAmount();
		for(size_t i=0; i<nSize; i++)
		{
			Player* pMember	= this->GetMemberByIndex(i);
			if(pMember)
			{
				//队伍广播消息
				CMsgSynRoomInfo msg;
				IF_OK(msg.CreateMsg(pMember, _SYNROOMINFO_ACTION_ADD_MEMBER, this->m_id, this->m_nFightTeamColour))
				{
					pOtherTeam->BroadcastTeamMsg(&msg);
				}
			}		
		}

		//更新pOtherTeam队伍的队长ID
		CMsgBattle msg;
		msg.CreateMsg(_BATTLE_ACTION_CHANGE_TEAM_LEADER_SUC, 0, this->m_id, this->m_nTeamLeaderID);
		
		//把pOtherTeam队伍成员 广播添加到 pOwnerbt队伍成员
		//pOwnerbt 队伍ID进行修改广播
		CMsgBattle msg2;
		IF_OK(msg2.CreateMsg(_BATTLE_ACTION_CHANGE_TEAM_ID, nModelType, this->m_id))
	    {
			size_t nSize = pOtherTeam->GetMemberAmount();
			for(size_t i=0; i<nSize; i++)
			{
				Player* pMember	= pOtherTeam->GetMemberByIndex(i);
				if(pMember)
				{
					//通知pOtherTeam队伍的所有成员-队伍ID修改
					pMember->SendMsgToClient(&msg2);
						

					//把pOtherTeam队伍的所有成员添加到pOwnerbt队伍中
					if(this->AddMember(pMember, _BATTLE_MODEL_CONFUSION))
					{
						//队伍广播消息
						CMsgSynRoomInfo msg3;
						IF_OK(msg3.CreateMsg(pMember, _SYNROOMINFO_ACTION_ADD_MEMBER, this->m_id, this->m_nFightTeamColour))
						{
							this->BroadcastTeamMsg(&msg3);
						}

						//更新pOtherTeam队伍成员的反向队列指针
	        			pMember->SetBct(this);
	        			//设置活着的状态
				    	pMember->SetDeadStatus(_STATUS_ALIVE);
				    	//初始化玩家游戏step
				    	pMember->SetGameStep(_GAME_STEP_NONE);

	        			//更新pOtherTeam队伍的队长ID
						pMember->SendMsgToClient(&msg);
					}
				}	
			}	
		}

		//清空pOtherTeam队伍成员
		pOtherTeam->m_vtTeamPlayerS.clear();

		return TRUE;
	}
	//红蓝对战模式
	if (nModelType == _FIGHT_MODEL_TYPE_RB_FIGHT_TEAM)
	{
		return FALSE;
	}

	return FALSE;
}

//广播66ms网络数据帧包
VOID CBattleTeam::BroadcastConfusionFrameMsg()
{
	CMsgWalk msg;
	BOOL bNeedBroadcastMsg = FALSE;
	long long llCurMillSecTime = GetMillSecTime();		//获取当前时间戳
	size_t nSize = GetMemberAmount();
	LOGDEBUG("[CBattleTeam::BroadcastConfusionFrameMsg] nSize=[%d]", nSize);
	for(size_t i=0; i<nSize; i++)
	{
		Player* pMember	= this->GetMemberByIndex(i);
		LOGDEBUG("[CBattleTeam::BroadcastConfusionFrameMsg] Playerid=[%d] DeadStatus=[%d] GameStep=[%d]", pMember->GetUid(), pMember->GetDeadStatus(), pMember->GetGameStep());
		if(pMember && (pMember->GetDeadStatus() == _STATUS_ALIVE || pMember->GetDeadStatus() == _STATUS_PREPARE_ALIVE || pMember->GetDeadStatus() == _STATUS_DEAD) && pMember->GetGameStep() == _GAME_STEP_LOADING_COMPLETE)
		{
			if (pMember->GetDeadStatus() == _STATUS_ALIVE)
			{
				bNeedBroadcastMsg = TRUE;
			}

			//遍历66ms内该队员上传上来的信息包
			INT nHaveSynMsgInterval = 10;		//由于前端问题，导致一帧中可能发送多个包过来,最多10个包
			INT nHaveSynMsgIndex = 1;		//由于前端问题，导致一帧中可能发送多个包过来
			BOOL bHaveSynMsg = FALSE;
			Range range = m_mapTeamNetMsg.equal_range(pMember->GetUid());
			while(range.first != range.second)//有收到该玩家的操作包
			{
				if (pMember->GetDeadStatus() == _STATUS_PREPARE_ALIVE)	
				{
					if(range.first->second.nAction == _WALK_ACTION_CONFUSION_RELIVE || range.first->second.nAction == _WALK_ACTION_COLLIDE_CMD)
					{
						LOGDEBUG("[CBattleTeam::BroadcastConfusionFrameMsg] _WALK_ACTION_CONFUSION_RELIVE||_WALK_ACTION_COLLIDE_CMD nAction=[%d] nTargetId=[%d] nPointX=[%d] nPointY=[%d] nPointStartX=[%d] nPointStartY=[%d] nPointEndX=[%d] nPointEndY=[%d] i=[%d] nBeCollideId=[%d] nTType=[%d] nTPowerX=[%d] nTPowerY=[%d]", 
							range.first->second.nAction, range.first->second.nTargetId, range.first->second.nPointX, range.first->second.nPointY, range.first->second.nPointStartX, 
							range.first->second.nPointStartY, range.first->second.nPointEndX, range.first->second.nPointEndY, i*nHaveSynMsgInterval+nHaveSynMsgIndex,
							range.first->second.nBeCollideId, range.first->second.nTType, range.first->second.nTPowerX, range.first->second.nTPowerY, llCurMillSecTime%100000000);
						bNeedBroadcastMsg = TRUE;
					}
					else
					{
						//LOGDEBUG("[CBattleTeam::BroadcastConfusionFrameMsg] continue nAction=[%d] nTargetId=[%d] nPointX=[%d] nPointY=[%d] nPointStartX=[%d] nPointStartY=[%d] nPointEndX=[%d] nPointEndY=[%d] i=[%d] nBeCollideId=[%d] nTType=[%d] nTPowerX=[%d] nTPowerY=[%d] ", 
						//	range.first->second.nAction, range.first->second.nTargetId, range.first->second.nPointX, range.first->second.nPointY, range.first->second.nPointStartX, 
						//	range.first->second.nPointStartY, range.first->second.nPointEndX, range.first->second.nPointEndY, i*nHaveSynMsgInterval+nHaveSynMsgIndex,
						//	range.first->second.nBeCollideId, range.first->second.nTType, range.first->second.nTPowerX, range.first->second.nTPowerY, llCurMillSecTime%100000000);
						++range.first;
						continue;
					}
				}
				//击杀后死亡帧处理，需要进行同步
				else if (pMember->GetDeadStatus() == _STATUS_DEAD)
				{
					if(range.first->second.nAction == _WALK_ACTION_CONFUSION_CAR_LEV_CHANGE || range.first->second.nAction == _WALK_ACTION_KILL)
					{
						LOGDEBUG("[CBattleTeam::BroadcastConfusionFrameMsg] _WALK_ACTION_CONFUSION_CAR_LEV_CHANGE||_WALK_ACTION_KILL nAction=[%d] nTargetId=[%d] nPointX=[%d] nPointY=[%d] nPointStartX=[%d] nPointStartY=[%d] nPointEndX=[%d] nPointEndY=[%d] i=[%d] nBeCollideId=[%d] nTType=[%d] nTPowerX=[%d] nTPowerY=[%d]", 
							range.first->second.nAction, range.first->second.nTargetId, range.first->second.nPointX, range.first->second.nPointY, range.first->second.nPointStartX, 
							range.first->second.nPointStartY, range.first->second.nPointEndX, range.first->second.nPointEndY, i*nHaveSynMsgInterval+nHaveSynMsgIndex,
							range.first->second.nBeCollideId, range.first->second.nTType, range.first->second.nTPowerX, range.first->second.nTPowerY, llCurMillSecTime%100000000);
						bNeedBroadcastMsg = TRUE;
					}
					else
					{
						//LOGDEBUG("[CBattleTeam::BroadcastConfusionFrameMsg] continue nAction=[%d] nTargetId=[%d] nPointX=[%d] nPointY=[%d] nPointStartX=[%d] nPointStartY=[%d] nPointEndX=[%d] nPointEndY=[%d] i=[%d] nBeCollideId=[%d] nTType=[%d] nTPowerX=[%d] nTPowerY=[%d] ", 
						//	range.first->second.nAction, range.first->second.nTargetId, range.first->second.nPointX, range.first->second.nPointY, range.first->second.nPointStartX, 
						//	range.first->second.nPointStartY, range.first->second.nPointEndX, range.first->second.nPointEndY, i*nHaveSynMsgInterval+nHaveSynMsgIndex,
						//	range.first->second.nBeCollideId, range.first->second.nTType, range.first->second.nTPowerX, range.first->second.nTPowerY, llCurMillSecTime%100000000);
						++range.first;
						continue;
					}
				}
				bHaveSynMsg = TRUE;
				msg.CreateFrameMsg(range.first->second.nAction, range.first->second.nTargetId, range.first->second.nPointX, range.first->second.nPointY, range.first->second.nPointStartX, 
					range.first->second.nPointStartY, range.first->second.nPointEndX, range.first->second.nPointEndY, i*nHaveSynMsgInterval+nHaveSynMsgIndex,
					range.first->second.nBeCollideId, range.first->second.nTType, range.first->second.nTPowerX, range.first->second.nTPowerY, llCurMillSecTime%100000000);				
				
				LOGDEBUG("[CBattleTeam::BroadcastConfusionFrameMsg] nAction=[%d] nTargetId=[%d] nPointX=[%d] nPointY=[%d] nPointStartX=[%d] nPointStartY=[%d] nPointEndX=[%d] nPointEndY=[%d] i=[%d] nBeCollideId=[%d] nTType=[%d] nTPowerX=[%d] nTPowerY=[%d]", 
					range.first->second.nAction, range.first->second.nTargetId, range.first->second.nPointX, range.first->second.nPointY, range.first->second.nPointStartX, 
					range.first->second.nPointStartY, range.first->second.nPointEndX, range.first->second.nPointEndY, i*nHaveSynMsgInterval+nHaveSynMsgIndex, 
					range.first->second.nBeCollideId, range.first->second.nTType, range.first->second.nTPowerX, range.first->second.nTPowerY, llCurMillSecTime%100000000);

				nHaveSynMsgIndex++;
				++range.first;
			}

			//未收到该玩家的操作包
			if (!bHaveSynMsg && pMember->GetDeadStatus() == _STATUS_ALIVE)
			{				
				//处于碰撞状态
				Socket* pSocket = pMember->GetSocket();
				if (pSocket &&  pSocket->GetSynMsgStatus() == _SYN_MSG_COLLIDE)
                {
                    //更新次数
                    INT nHaveCollideSynMsgCount = pSocket->GetCollideSynMsgCount();
                    pSocket->SetCollideSynMsgCount(++nHaveCollideSynMsgCount);                
                    if (nHaveCollideSynMsgCount >= g_config.ONLINE_BROADCAST_COLLIDE_SYN_MSG_COUNT)
       				{
                        pSocket->SetSynMsgStatus(_SYN_MSG_PREVIOUS);
                    }
                    msg.CreateFrameMsg(_WALK_ACTION_CONTINUE_COLLIDE_CMD, pMember->GetUid(), 0, 0, 0, 0, 0, 0, i*nHaveSynMsgInterval+nHaveSynMsgIndex, 0, 0, 0, 0, llCurMillSecTime%100000000); 
                    LOGDEBUG("[CBattleTeam::BroadcastConfusionFrameMsg] cmd=[_WALK_ACTION_CONTINUE_COLLIDE_CMD] nHaveCollideSynMsgCount=[%d] SynMsgStatus=[%d]", nHaveCollideSynMsgCount, pSocket->GetSynMsgStatus());                     
                }
				else //if (pSocket &&  pSocket->GetSynMsgStatus() == _SYN_MSG_PREVIOUS)
                {
                	msg.CreateFrameMsg(_WALK_ACTION_CONTINUE_PREVIOUS_CMD, pMember->GetUid(), 0, 0, 0, 0, 0, 0, i*nHaveSynMsgInterval+nHaveSynMsgIndex, 0, 0, 0, 0, llCurMillSecTime%100000000);
                	LOGDEBUG("[CBattleTeam::BroadcastConfusionFrameMsg] cmd=[_WALK_ACTION_CONTINUE_PREVIOUS_CMD] SynMsgStatus=[%d]", pSocket->GetSynMsgStatus());
                }
                nHaveSynMsgIndex++;
			}
		}
	}

	char* sJson = cJSON_Print(msg.m_pMsg);
	string strMsg(sJson);
    INT nLen = strMsg.size();    
    long long mllRecvMsgTimeStamp = GetRecvMsgTimeStamp();
	LOGDEBUG("[CBattleTeam::BroadcastConfusionFrameMsg] TeamId=[%d] llCurMillSecTime=[%lld] mllLastRecvMsgTimeStamp=[%lld] TimeStamp=[%lld] m_mapTeamNetMsg.size=[%d]\t\nmsg.m_pMsg=[%s] nLen=[%d]",
		m_id, llCurMillSecTime, mllRecvMsgTimeStamp, llCurMillSecTime-mllRecvMsgTimeStamp, m_mapTeamNetMsg.size(), sJson, nLen);
	free(sJson);

	//队伍有成员活着进行广播
	if (bNeedBroadcastMsg)
	{
		//广播开始战争消息
		for(size_t i=0; i<nSize; i++)
		{
			Player* pMember	= this->GetMemberByIndex(i);
			if(pMember)
			{
				LOGDEBUG("[CBattleTeam::BroadcastConfusionFrameMsg] TeamId=[%d] playerid=[%d]", m_id, pMember->GetUid());
				pMember->SendMsgToClient(&msg);
			}
		}
	}
	
	//更新时间戳
	//long long llCurMillSecTime = GetMillSecTime();
	SetRecvMsgTimeStamp(llCurMillSecTime);

	//清空帧缓存区
	m_mapTeamNetMsg.clear();
}

//广播66ms网络数据帧包-[红蓝战队]
VOID CBattleTeam::BroadcastTeamFrameMsg()
{
	if (!m_pEnemyTeam)
	{
		return;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////
	//处理我方队友位置等信息
    CMsgWalk msg;
    BOOL bNeedBroadcastMsg = FALSE;
    long long llCurMillSecTime = GetMillSecTime();
	size_t nSize = GetMemberAmount();	
	//LOGDEBUG("[CBattleTeam::BroadcastTeamFrameMsg] ------------------begin------------------------------------");
	LOGDEBUG("[CBattleTeam::BroadcastTeamFrameMsg] nSize=[%d]", nSize);
	for(size_t i=0; i<nSize; i++)
	{
		Player* pMember	= this->GetMemberByIndex(i);
		//LOGDEBUG("[CBattleTeam::BroadcastTeamFrameMsg] Playerid=[%d] DeadStatus=[%d] GameStep=[%d]", pMember->GetUid(), pMember->GetDeadStatus(), pMember->GetGameStep());
		if(pMember && (pMember->GetDeadStatus() == _STATUS_ALIVE || pMember->GetDeadStatus() == _STATUS_PREPARE_ALIVE || pMember->GetDeadStatus() == _STATUS_DEAD) && pMember->GetGameStep() == _GAME_STEP_LOADING_COMPLETE)
		{
			if (pMember->GetDeadStatus() == _STATUS_ALIVE)
			{
				bNeedBroadcastMsg = TRUE;
			}

			INT nHaveSynMsgInterval = 10;		//由于前端问题，导致一帧中可能发送多个包过来,最多10个包
			INT nHaveSynMsgIndex = 1;			//由于前端问题，导致一帧中可能发送多个包过来
			//遍历66ms内该队员上传上来的信息包
			BOOL bHaveSynMsg = FALSE;
			Range range = m_mapTeamNetMsg.equal_range(pMember->GetUid());
			while(range.first != range.second)//有收到该玩家的操作包
			{
				//==============================================================================================================================================//
				//																																			    //
				//									同步帧操作处理部分关键技术点梳理																	 	    //
				//																																			    //
				//==============================================================================================================================================//
				//																																			    //
				//☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆【一】☆☆☆☆☆☆☆☆☆【死亡复活解析】 ☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆//
				//☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆插帧 msgwalk _WALK_ACTION_CONFUSION_RELIVE 和 _WALK_ACTION_TEAM_RELIVE 帧☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆//
				//==============================================================================================================================================//
				//1、死亡(客户端立即上报)----->服务器 																										    //	
				//2、服务器立即标记玩家死亡状态(_STATUS_DEAD)																								    //
				//3、广播死亡信息包给与反馈和同步其他玩家 																									    //
				//4、前端收到死亡ACK后，立即发送复活包(msgbattle->_BATTLE_ACTION_TEAM_RELIVE) 																    //
				//5、服务器收到复活包后，立即标志该玩家为准备复活状态(_STATUS_PREPARE_ALIVE),并插入复活帧到帧缓存队列，带下一帧同步所有前端 				    //
				//6、***此时可能出现碰撞包插帧 和 死亡包（不插帧）、复活帧在同一帧内收到并处理完。，需要做特殊处理 											    //
				//7、continue 此状态下，不需要同步其他空帧(321倒计时的时候)否则前端会在321倒计时的时候车辆继续前进 											    //
				//==============================================================================================================================================//
				//																																			    //
				//==============================================================================================================================================//
				//☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆【二】☆☆☆☆☆☆☆☆☆【碰撞解析】 ☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆//
				//☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆插帧 msgwalk _WALK_ACTION_COLLIDE_CMD 和 _WALK_ACTION_CONTINUE_COLLIDE_CMD☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆//
				//==============================================================================================================================================//
				//1、当玩家A执行第N帧的时候与玩家B发生碰撞，玩家A立即上报碰撞信息，信息中附带玩家B的反弹信息													//			
				//2、服务器收到玩家A上报的碰撞信息后 																											//
				//3、第一步进行数据解析，然后构建玩家碰撞信息，key=ts(服务器帧标记)+玩家A_id+玩家B_id，相应的插入到玩家A的碰撞缓存队列 							//
				//4、组建玩家B的碰撞信息数据，key=ts(服务器帧标记)+玩家B_id+玩家A_id，相应的插入到玩家b的碰撞缓存队列 进行分包和插帧处理 						//
				//5、把相关信息插入到对应的碰撞玩家的缓存队列后。等到下一帧的发送广播给前端 																	//
				//6、***此时如果收到玩家B上报碰撞信息(玩家B碰撞了玩家A)，此时服务器构造玩家B的碰撞信息，key==ts(服务器帧标记)+玩家B_id+玩家A_id 				//
				//7、然后去缓存队列中查找key是否已经存在 																										//
				//8、如果存在则表示已经上报，如果不存在则表示第一次发生，可以进行广播信息于所有客户端， 														//
				//9、***此时玩家B可能上传自己的一些操作帧，服务器暂不做过滤 																					//
				//10、前端收到服务器同步过去的碰撞反弹帧后，过滤掉后续的其他非碰撞持续包，即碰撞空帧包。 														//
				//==============================================================================================================================================//
				//																																				//
				//==============================================================================================================================================//
				//☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆【三】☆☆☆☆☆☆☆☆☆【车辆模型大小发生改变解析】 ☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆//
				//☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆msgwalk _WALK_ACTION_CONFUSION_CAR_LEV_CHANGE 和 _WALK_ACTION_TEAM_CAR_LEV_CHANGE 帧☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆☆//
				//==============================================================================================================================================//
				//1、当玩家A击杀玩家B的时候，玩家A的车辆的大小发生变化时，此时需要同步所有客户端玩家A的车辆大小发生变化 										//
				//2、这样才能够保证当每个客户端处理完击杀后，接着立刻处理玩家车辆大小变化的数据包 																//
				//3、保持所有客户端的玩家A的车辆同帧变化，才可以保证每个客户端检测碰撞的时候保持同步。															//
				//==============================================================================================================================================//
				//																																				//
				//==============================================================================================================================================//
				//======================【四】======================【玩家A延迟进入游戏玩家B此时处于死亡或者死亡多次又复活状态解析】============================//
				//======================插帧 msgwalk _WALK_ACTION_COLLIDE_CMD 和 _WALK_ACTION_CONTINUE_COLLIDE_CMD==============================================//
				//==============================================================================================================================================//
				//1、当出现玩家A延迟进入游戏，玩家B和玩家C、玩家D很早就已经进入游戏，并且玩家B和玩家C、玩家B和玩家D已经发生了碰撞或者发生了击杀 				//
				//2、当玩家A进入游戏的时候玩家B处于死亡状态（或者死亡了几次但是又复活了）， 																	//
				//3、前端在对玩家C和玩家D进行碰撞处理检测的时候，判定此时玩家B已经死亡状态，则不会和玩家C发生碰撞 												//
				//4、那么玩家C就没有进行碰撞后的抛帧动作，就会多执行几个多余的帧，导致不同步问题。现在都不再抛帧处理，直接播放所有帧的动画。 					//
				//==============================================================================================================================================//
				//																																				//
				//==============================================================================================================================================//
				//======================【五】======================【浮点数->定点数处理】======================================================================//
				//======================前端自行处理============================================================================================================//
				//==============================================================================================================================================//
				//																																				//
				//==============================================================================================================================================//
				//
				if (pMember->GetDeadStatus() == _STATUS_PREPARE_ALIVE)	
				{
					if(range.first->second.nAction == _WALK_ACTION_TEAM_RELIVE || range.first->second.nAction == _WALK_ACTION_COLLIDE_CMD)
					{
						LOGDEBUG("[CBattleTeam::BroadcastTeamFrameMsg] _WALK_ACTION_TEAM_RELIVE||_WALK_ACTION_COLLIDE_CMD nAction=[%d] nTargetId=[%d] nPointX=[%d] nPointY=[%d] nPointStartX=[%d] nPointStartY=[%d] nPointEndX=[%d] nPointEndY=[%d] i=[%d] nBeCollideId=[%d] nTType=[%d] nTPowerX=[%d] nTPowerY=[%d] ", 
							range.first->second.nAction, range.first->second.nTargetId, range.first->second.nPointX, range.first->second.nPointY, range.first->second.nPointStartX, 
							range.first->second.nPointStartY, range.first->second.nPointEndX, range.first->second.nPointEndY, i*nHaveSynMsgInterval+nHaveSynMsgIndex,
							range.first->second.nBeCollideId, range.first->second.nTType, range.first->second.nTPowerX, range.first->second.nTPowerY, llCurMillSecTime%100000000);
						bNeedBroadcastMsg = TRUE;
					}
					else
					{
						//LOGDEBUG("[CBattleTeam::BroadcastTeamFrameMsg] continue nAction=[%d] nTargetId=[%d] nPointX=[%d] nPointY=[%d] nPointStartX=[%d] nPointStartY=[%d] nPointEndX=[%d] nPointEndY=[%d] i=[%d] nBeCollideId=[%d] nTType=[%d] nTPowerX=[%d] nTPowerY=[%d] ", 
						//	range.first->second.nAction, range.first->second.nTargetId, range.first->second.nPointX, range.first->second.nPointY, range.first->second.nPointStartX, 
						//	range.first->second.nPointStartY, range.first->second.nPointEndX, range.first->second.nPointEndY, i*nHaveSynMsgInterval+nHaveSynMsgIndex,
						//	range.first->second.nBeCollideId, range.first->second.nTType, range.first->second.nTPowerX, range.first->second.nTPowerY, llCurMillSecTime%100000000);
						++range.first;
						continue;
					}
				}
				//击杀后死亡帧处理，需要进行同步
				else if (pMember->GetDeadStatus() == _STATUS_DEAD)
				{
					if(range.first->second.nAction == _WALK_ACTION_TEAM_CAR_LEV_CHANGE || range.first->second.nAction == _WALK_ACTION_KILL)
					{
						LOGDEBUG("[CBattleTeam::BroadcastTeamFrameMsg] _WALK_ACTION_TEAM_CAR_LEV_CHANGE||_WALK_ACTION_KILL nAction=[%d] nTargetId=[%d] nPointX=[%d] nPointY=[%d] nPointStartX=[%d] nPointStartY=[%d] nPointEndX=[%d] nPointEndY=[%d] i=[%d] nBeCollideId=[%d] nTType=[%d] nTPowerX=[%d] nTPowerY=[%d] ", 
							range.first->second.nAction, range.first->second.nTargetId, range.first->second.nPointX, range.first->second.nPointY, range.first->second.nPointStartX, 
							range.first->second.nPointStartY, range.first->second.nPointEndX, range.first->second.nPointEndY, i*nHaveSynMsgInterval+nHaveSynMsgIndex,
							range.first->second.nBeCollideId, range.first->second.nTType, range.first->second.nTPowerX, range.first->second.nTPowerY, llCurMillSecTime%100000000);
						bNeedBroadcastMsg = TRUE;
					}
					else
					{
						//LOGDEBUG("[CBattleTeam::BroadcastTeamFrameMsg] continue nAction=[%d] nTargetId=[%d] nPointX=[%d] nPointY=[%d] nPointStartX=[%d] nPointStartY=[%d] nPointEndX=[%d] nPointEndY=[%d] i=[%d] nBeCollideId=[%d] nTType=[%d] nTPowerX=[%d] nTPowerY=[%d] ", 
						//	range.first->second.nAction, range.first->second.nTargetId, range.first->second.nPointX, range.first->second.nPointY, range.first->second.nPointStartX, 
						//	range.first->second.nPointStartY, range.first->second.nPointEndX, range.first->second.nPointEndY, i*nHaveSynMsgInterval+nHaveSynMsgIndex,
						//	range.first->second.nBeCollideId, range.first->second.nTType, range.first->second.nTPowerX, range.first->second.nTPowerY, llCurMillSecTime%100000000);
						++range.first;
						continue;
					}
				}
				
				bHaveSynMsg = TRUE;
				msg.CreateFrameMsg(range.first->second.nAction, range.first->second.nTargetId, range.first->second.nPointX, range.first->second.nPointY, range.first->second.nPointStartX, 
					range.first->second.nPointStartY, range.first->second.nPointEndX, range.first->second.nPointEndY, i*nHaveSynMsgInterval+nHaveSynMsgIndex,
					range.first->second.nBeCollideId, range.first->second.nTType, range.first->second.nTPowerX, range.first->second.nTPowerY, llCurMillSecTime%100000000);				

				LOGDEBUG("[CBattleTeam::BroadcastTeamFrameMsg] nAction=[%d] nTargetId=[%d] nPointX=[%d] nPointY=[%d] nPointStartX=[%d] nPointStartY=[%d] nPointEndX=[%d] nPointEndY=[%d] i=[%d] nBeCollideId=[%d] nTType=[%d] nTPowerX=[%d] nTPowerY=[%d] ", 
					range.first->second.nAction, range.first->second.nTargetId, range.first->second.nPointX, range.first->second.nPointY, range.first->second.nPointStartX, 
					range.first->second.nPointStartY, range.first->second.nPointEndX, range.first->second.nPointEndY, i*nHaveSynMsgInterval+nHaveSynMsgIndex,
					range.first->second.nBeCollideId, range.first->second.nTType, range.first->second.nTPowerX, range.first->second.nTPowerY, llCurMillSecTime%100000000);

				nHaveSynMsgIndex++;
				++range.first;
			}

			//未收到该玩家的操作包
			if (!bHaveSynMsg && pMember->GetDeadStatus() == _STATUS_ALIVE)
			{	
				//处于碰撞状态
				Socket* pSocket = pMember->GetSocket();
				if (pSocket &&  pSocket->GetSynMsgStatus() == _SYN_MSG_COLLIDE)
                {
                    //更新次数
                    INT nHaveCollideSynMsgCount = pSocket->GetCollideSynMsgCount();
                    pSocket->SetCollideSynMsgCount(++nHaveCollideSynMsgCount);                
                    if (nHaveCollideSynMsgCount >= g_config.ONLINE_BROADCAST_COLLIDE_SYN_MSG_COUNT)
       				{
                        pSocket->SetSynMsgStatus(_SYN_MSG_PREVIOUS);
                    }
                    msg.CreateFrameMsg(_WALK_ACTION_CONTINUE_COLLIDE_CMD, pMember->GetUid(), 0, 0, 0, 0, 0, 0, i*nHaveSynMsgInterval+nHaveSynMsgIndex, 0, 0, 0, 0, llCurMillSecTime%100000000);
                    LOGDEBUG("[CBattleTeam::BroadcastTeamFrameMsg] cmd=[_WALK_ACTION_CONTINUE_COLLIDE_CMD] nTargetId=[%d] nHaveCollideSynMsgCount=[%d] SynMsgStatus=[%d]", pMember->GetUid(), nHaveCollideSynMsgCount, pSocket->GetSynMsgStatus());
                }
				else //if (pSocket &&  pSocket->GetSynMsgStatus() == _SYN_MSG_PREVIOUS)
                {
                	msg.CreateFrameMsg(_WALK_ACTION_CONTINUE_PREVIOUS_CMD, pMember->GetUid(), 0, 0, 0, 0, 0, 0, i*nHaveSynMsgInterval+nHaveSynMsgIndex, 0, 0, 0, 0, llCurMillSecTime%100000000);
                	LOGDEBUG("[CBattleTeam::BroadcastTeamFrameMsg] nTargetId=[%d] cmd=[_WALK_ACTION_CONTINUE_PREVIOUS_CMD] SynMsgStatus=[%d]", pMember->GetUid(), pSocket->GetSynMsgStatus());
                }
                nHaveSynMsgIndex++;
			}
		}
	}

	size_t nEtSize = m_pEnemyTeam->GetMemberAmount();
	LOGDEBUG("[CBattleTeam::BroadcastTeamFrameMsg] nEtSize=[%d]", nEtSize);
	for(size_t i=0; i<nEtSize; i++)
	{
		Player* pMember	= m_pEnemyTeam->GetMemberByIndex(i);
		//LOGDEBUG("[CBattleTeam::BroadcastTeamFrameMsg] Playerid=[%d] DeadStatus=[%d] GameStep=[%d]", pMember->GetUid(), pMember->GetDeadStatus(), pMember->GetGameStep());
		if(pMember && (pMember->GetDeadStatus() == _STATUS_ALIVE || pMember->GetDeadStatus() == _STATUS_PREPARE_ALIVE || pMember->GetDeadStatus() == _STATUS_DEAD) && pMember->GetGameStep() == _GAME_STEP_LOADING_COMPLETE)
		{
			if (pMember->GetDeadStatus() == _STATUS_ALIVE)
			{
				bNeedBroadcastMsg = TRUE;
			}
			
			INT nHaveSynMsgInterval = 10;		//由于前端问题，导致一帧中可能发送多个包过来,最多10个包
			INT nHaveSynMsgIndex = 1;		//由于前端问题，导致一帧中可能发送多个包过来
			//遍历66ms内该队员上传上来的信息包
			BOOL bHaveSynMsg = FALSE;
			Range range = m_pEnemyTeam->m_mapTeamNetMsg.equal_range(pMember->GetUid());
			while(range.first != range.second)//有收到该玩家的操作包
			{
				if (pMember->GetDeadStatus() == _STATUS_PREPARE_ALIVE)
				{
					if(range.first->second.nAction == _WALK_ACTION_TEAM_RELIVE || range.first->second.nAction == _WALK_ACTION_COLLIDE_CMD)
					{
						bNeedBroadcastMsg = TRUE;
						LOGDEBUG("[CBattleTeam::BroadcastTeamFrameMsg] _WALK_ACTION_TEAM_RELIVE||_WALK_ACTION_COLLIDE_CMD nAction=[%d] nTargetId=[%d] nPointX=[%d] nPointY=[%d] nPointStartX=[%d] nPointStartY=[%d] nPointEndX=[%d] nPointEndY=[%d] i=[%d] nBeCollideId=[%d] nTType=[%d] nTPowerX=[%d] nTPowerY=[%d] ", 
							range.first->second.nAction, range.first->second.nTargetId, range.first->second.nPointX, range.first->second.nPointY, range.first->second.nPointStartX, 
							range.first->second.nPointStartY, range.first->second.nPointEndX, range.first->second.nPointEndY, (i+nSize)*nHaveSynMsgInterval+nHaveSynMsgIndex, 
							range.first->second.nBeCollideId, range.first->second.nTType, range.first->second.nTPowerX, range.first->second.nTPowerY, llCurMillSecTime%100000000);
					}
					else
					{
						LOGDEBUG("[CBattleTeam::BroadcastTeamFrameMsg] continue nAction=[%d] nTargetId=[%d] nPointX=[%d] nPointY=[%d] nPointStartX=[%d] nPointStartY=[%d] nPointEndX=[%d] nPointEndY=[%d] i=[%d] nBeCollideId=[%d] nTType=[%d] nTPowerX=[%d] nTPowerY=[%d] ", 
							range.first->second.nAction, range.first->second.nTargetId, range.first->second.nPointX, range.first->second.nPointY, range.first->second.nPointStartX, 
							range.first->second.nPointStartY, range.first->second.nPointEndX, range.first->second.nPointEndY, (i+nSize)*nHaveSynMsgInterval+nHaveSynMsgIndex, 
							range.first->second.nBeCollideId, range.first->second.nTType, range.first->second.nTPowerX, range.first->second.nTPowerY, llCurMillSecTime%100000000);
						++range.first;
						continue;
					}
				}
				//击杀后死亡帧处理，需要进行同步
				else if (pMember->GetDeadStatus() == _STATUS_DEAD)
				{
					if(range.first->second.nAction == _WALK_ACTION_TEAM_CAR_LEV_CHANGE || range.first->second.nAction == _WALK_ACTION_KILL)
					{
						bNeedBroadcastMsg = TRUE;
						LOGDEBUG("[CBattleTeam::BroadcastTeamFrameMsg] _WALK_ACTION_TEAM_CAR_LEV_CHANGE||_WALK_ACTION_KILL nAction=[%d] nTargetId=[%d] nPointX=[%d] nPointY=[%d] nPointStartX=[%d] nPointStartY=[%d] nPointEndX=[%d] nPointEndY=[%d] i=[%d] nBeCollideId=[%d] nTType=[%d] nTPowerX=[%d] nTPowerY=[%d] ", 
							range.first->second.nAction, range.first->second.nTargetId, range.first->second.nPointX, range.first->second.nPointY, range.first->second.nPointStartX, 
							range.first->second.nPointStartY, range.first->second.nPointEndX, range.first->second.nPointEndY, (i+nSize)*nHaveSynMsgInterval+nHaveSynMsgIndex, 
							range.first->second.nBeCollideId, range.first->second.nTType, range.first->second.nTPowerX, range.first->second.nTPowerY, llCurMillSecTime%100000000);
					}
					else
					{
						LOGDEBUG("[CBattleTeam::BroadcastTeamFrameMsg] continue nAction=[%d] nTargetId=[%d] nPointX=[%d] nPointY=[%d] nPointStartX=[%d] nPointStartY=[%d] nPointEndX=[%d] nPointEndY=[%d] i=[%d] nBeCollideId=[%d] nTType=[%d] nTPowerX=[%d] nTPowerY=[%d] ", 
							range.first->second.nAction, range.first->second.nTargetId, range.first->second.nPointX, range.first->second.nPointY, range.first->second.nPointStartX, 
							range.first->second.nPointStartY, range.first->second.nPointEndX, range.first->second.nPointEndY, (i+nSize)*nHaveSynMsgInterval+nHaveSynMsgIndex, 
							range.first->second.nBeCollideId, range.first->second.nTType, range.first->second.nTPowerX, range.first->second.nTPowerY, llCurMillSecTime%100000000);
						++range.first;
						continue;
					}
				}

				bHaveSynMsg = TRUE;
				msg.CreateFrameMsg(range.first->second.nAction, range.first->second.nTargetId, range.first->second.nPointX, range.first->second.nPointY, range.first->second.nPointStartX, 
					range.first->second.nPointStartY, range.first->second.nPointEndX, range.first->second.nPointEndY, (i+nSize)*nHaveSynMsgInterval+nHaveSynMsgIndex,
					range.first->second.nBeCollideId, range.first->second.nTType, range.first->second.nTPowerX, range.first->second.nTPowerY, llCurMillSecTime%100000000);				

				LOGDEBUG("[CBattleTeam::BroadcastTeamFrameMsg] nAction=[%d] nTargetId=[%d] nPointX=[%d] nPointY=[%d] nPointStartX=[%d] nPointStartY=[%d] nPointEndX=[%d] nPointEndY=[%d] i=[%d] nBeCollideId=[%d] nTType=[%d] nTPowerX=[%d] nTPowerY=[%d] ", 
					range.first->second.nAction, range.first->second.nTargetId, range.first->second.nPointX, range.first->second.nPointY, range.first->second.nPointStartX, 
					range.first->second.nPointStartY, range.first->second.nPointEndX, range.first->second.nPointEndY, (i+nSize)*nHaveSynMsgInterval+nHaveSynMsgIndex, 
					range.first->second.nBeCollideId, range.first->second.nTType, range.first->second.nTPowerX, range.first->second.nTPowerY, llCurMillSecTime%100000000);		

				nHaveSynMsgIndex++;
				++range.first;
			}

			//未收到该玩家的操作包
			if (!bHaveSynMsg && pMember->GetDeadStatus() == _STATUS_ALIVE)
			{
				//处于碰撞状态
				Socket* pSocket = pMember->GetSocket();
				if (pSocket &&  pSocket->GetSynMsgStatus() == _SYN_MSG_COLLIDE)
                {
                    //更新次数
                    INT nHaveCollideSynMsgCount = pSocket->GetCollideSynMsgCount();
                    pSocket->SetCollideSynMsgCount(++nHaveCollideSynMsgCount);                
                    if (nHaveCollideSynMsgCount >= g_config.ONLINE_BROADCAST_COLLIDE_SYN_MSG_COUNT)
       				{
                        pSocket->SetSynMsgStatus(_SYN_MSG_PREVIOUS);
                    }
                    msg.CreateFrameMsg(_WALK_ACTION_CONTINUE_COLLIDE_CMD, pMember->GetUid(), 0, 0, 0, 0, 0, 0, (i+nSize)*nHaveSynMsgInterval+nHaveSynMsgIndex, 0, 0, 0, 0, llCurMillSecTime%100000000);
                    LOGDEBUG("[CBattleTeam::BroadcastTeamFrameMsg]-m_pEnemyTeam cmd=[_WALK_ACTION_CONTINUE_COLLIDE_CMD] nTargetId=[%d] nHaveCollideSynMsgCount=[%d] SynMsgStatus=[%d]\n", pMember->GetUid(), nHaveCollideSynMsgCount, pSocket->GetSynMsgStatus());                                                                
                }
				else //if (pSocket &&  pSocket->GetSynMsgStatus() == _SYN_MSG_PREVIOUS)
                {
                	msg.CreateFrameMsg(_WALK_ACTION_CONTINUE_PREVIOUS_CMD, pMember->GetUid(), 0, 0, 0, 0, 0, 0, (i+nSize)*nHaveSynMsgInterval+nHaveSynMsgIndex, 0, 0, 0, 0, llCurMillSecTime%100000000);
                	LOGDEBUG("[CBattleTeam::BroadcastTeamFrameMsg]-m_pEnemyTeam nTargetId=[%d] cmd=[_WALK_ACTION_CONTINUE_PREVIOUS_CMD] SynMsgStatus=[%d]\n", pMember->GetUid(), pSocket->GetSynMsgStatus());
                }
                nHaveSynMsgIndex++;
			}
		}
	}

	char* sJson = cJSON_Print(msg.m_pMsg);
	string strMsg(sJson);
    INT nLen = strMsg.size();
    
    long long mllRecvMsgTimeStamp = GetRecvMsgTimeStamp();
	LOGDEBUG("[CBattleTeam::BroadcastTeamFrameMsg] TeamId=[%d] llCurMillSecTime=[%lld] mllRecvMsgTimeStamp=[%lld] TimeStamp=[%lld] m_mapTeamNetMsg.size=[%d] m_pEnemyTeam->m_mapTeamNetMsg.size=[%d]\nmsg.m_pMsg=[%s] nLen=[%d]\n",
		m_id, llCurMillSecTime, mllRecvMsgTimeStamp, llCurMillSecTime-mllRecvMsgTimeStamp, m_mapTeamNetMsg.size(), m_pEnemyTeam->m_mapTeamNetMsg.size(), sJson, nLen);
	free(sJson);

	//队伍有成员活着进行广播
	if (bNeedBroadcastMsg)
	{
		//广播给我方战队-开始战争消息
		for(size_t i=0; i<nSize; i++)
		{
			Player* pMember	= this->GetMemberByIndex(i);
			if(pMember)
			{
				LOGDEBUG("[CBattleTeam::BroadcastTeamFrameMsg] SendMsgToClient = [%d]", i);
				pMember->SendMsgToClient(&msg);
			}
		}
	
		for(size_t i=0; i<nEtSize; i++)
		{
			Player* pMember	= m_pEnemyTeam->GetMemberByIndex(i);
			if(pMember)
			{
				LOGDEBUG("[CBattleTeam::BroadcastTeamFrameMsg] m_pEnemyTeam-SendMsgToClient = [%d]", i);
				pMember->SendMsgToClient(&msg);
			}
		}
	}
	//更新时间戳
	SetRecvMsgTimeStamp(llCurMillSecTime);
	m_pEnemyTeam->SetRecvMsgTimeStamp(llCurMillSecTime);
	//清空帧缓存区
	m_mapTeamNetMsg.clear();
	m_pEnemyTeam->m_mapTeamNetMsg.clear();

	//LOGDEBUG("[CBattleTeam::BroadcastTeamFrameMsg] ------------------end------------------------------------\n");
}
//////////////////////////////////////////////////////////////////////
//广播队伍成员信息给某位队友，一条一条广播
VOID CBattleTeam::BroadcastTeamMsgToMember(Player* pSender/*=NULL*/)
{	
	size_t nSize = GetMemberAmount();
	for(size_t i=0; i<nSize; i++)
	{
		Player* pMember	= this->GetMemberByIndex(i);
		if(pMember)
		{
			//广播队伍信息给刚入队的成员
			CMsgSynRoomInfo msg;			
			IF_OK(msg.CreateMsg(pMember, _SYNROOMINFO_ACTION_ADD_MEMBER, m_id, m_nFightTeamColour))
			{
				if (pMember!= pSender)
				{
					pSender->SendMsgToClient(&msg);
				}
			}			
		}		
	}
}

//////////////////////////////////////////////////////////////////////
//广播战斗开始玩家数据,生成区域、击杀、积分等信息
//此处随机按顺序生成10-15个点，前端配置的时候随机配置
VOID CBattleTeam::BroadcastConfusionStartMsgToMember()
{
	CMsgBattle msg;	
    INT nRegionId = Random(_CONFUSION_REILVE_REGION_RANGDOM);
	size_t nSize = GetMemberAmount();
	//add by huyf 2018.11.24:添加最近玩家
	//CMsgBattle msg2;
	//time_t tm = time(NULL);	
	//CRecentData* pRecentData = new CRecentData(nSize);
	//if (!pRecentData)
	//{
	//	return;
	//}	
	//add end:添加最近玩家
	for(size_t i=0; i<nSize; i++)
	{
		Player* pMember	= this->GetMemberByIndex(i);
		if(pMember)
		{
			//初始化玩家战斗数据：击杀数、击杀积分、车辆等级
			pMember->SetFightKillSum(0, FALSE);
			pMember->SetFightScore(_CONFUSION_BASE_FIGHT_SCORE + pMember->GetAddScore(), FALSE);
			INT nCfgLevel = pMember->GetFightCfgLevel(pMember->GetFightScore());
			pMember->SetFightLevel(nCfgLevel, FALSE);

			//设置玩家活着状态
			pMember->SetDeadStatus(_STATUS_ALIVE);

			//add by huyf 2018.11.24:添加好友系统
			pMember->SetGameStatus(_FRIEND_STATUS_GAMEING);
			//add end:添加好友系统

			//广播战斗开始包
			if (i==0)
			{
				msg.CreateMsg( _BATTLE_ACTION_BATTLE_START, _BATTLE_MODEL_CONFUSION, pMember->GetGrandingId(), pMember->GetUid(), BattleCfg()->GetBccMaxTime()/*分钟*/, 
					Random(_CONFUSION_DIRECTION_X_RANGDOM), Random(_CONFUSION_DIRECTION_Y_RANGDOM), pMember->GetFightKillSum(), pMember->GetFightScore(), pMember->GetFightLevel(), 
					pMember->GetCurStarSum(), nRegionId++%30, "", "");	
				//add by huyf 2018.11.24:添加最近玩家
				//msg2.CreateMsg( _BATTLE_ACTION_RECENT_PLAYER, _BATTLE_MODEL_CONFUSION, 0, pMember->GetUid(), 0/*分钟*/, 0, 0, 0, 0, 0, 0, tm, pMember->GetNickName(), pMember->GetHeadPicurl());	
				//add end:添加最近玩家
			}
			else
			{
				msg.AppendMsg( _BATTLE_ACTION_BATTLE_START, _BATTLE_MODEL_CONFUSION, pMember->GetGrandingId(), pMember->GetUid(), BattleCfg()->GetBccMaxTime()/*分钟*/, 
					Random(_CONFUSION_DIRECTION_X_RANGDOM), Random(_CONFUSION_DIRECTION_Y_RANGDOM), pMember->GetFightKillSum(), pMember->GetFightScore(), pMember->GetFightLevel(), 
					pMember->GetCurStarSum(), nRegionId++%30, i, "", "");
				//add by huyf 2018.11.24:添加最近玩家
				//msg2.AppendMsg( _BATTLE_ACTION_RECENT_PLAYER, _BATTLE_MODEL_CONFUSION, 0, pMember->GetUid(), 0/*分钟*/, 0, 0, 0, 0, 0, 0, tm, i, pMember->GetNickName(), pMember->GetHeadPicurl());
				//add end:添加最近玩家
			}

			//add by huyf 2018.11.24:添加最近玩家
			//pRecentData->SetUid(pMember->GetUid(), i);
			//pRecentData->SetRecentId(pMember->SetRecentId(), i);
			//pRecentData->SetGrandingId(pMember->SetGrandingId(), i);
			//pRecentData->SetCurStarSum(pMember->SetCurStarSum(), i);
			//pRecentData->SetNickName(pMember->SetNickName(), i);
			//pRecentData->SetHeadPicurl(pMember->SetHeadPicurl(), i);
			//add end:添加最近玩家
		}
	}

	//广播开始战争消息
	for(size_t i=0; i<nSize; i++)
	{
		Player* pMember	= this->GetMemberByIndex(i);
		if(pMember)
		{
			pMember->SendMsgToClient(&msg);
			//add by huyf 2018.11.24:添加最近玩家
			//pMember->SendMsgToClient(&msg2);
			//插入数据库
			//pMember->QueryRecentManager().AddRecent(pRecentData);
			//add end:添加最近玩家	
				
			//add by huyf 2018.11.24:添加好友系统
			//广播玩家进入战斗状态给自己好友
			CMsgFriendInfo msg2;
			IF_OK(msg2.CreateMsg(_FRIEND_INFO_UPDATE, pMember, pMember->GetGameStatus(), _FRIEND_INFO_ATTRIB_GAMESTATUS))
			{
				pMember->QueryFriendManager().BroadcastFriendSMsg(&msg2);
			}
			//add end:添加好友系统
			
			//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成1次大乱斗<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
			if (!pMember->TriggerDayTaskDetail(DAY_TASK_DETAIL_TYPE_6))
			{
				LOGERROR("[CBattleTeam::BroadcastConfusionStartMsgToMember] TriggerDayTaskDetail Is Error");
			}
			//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成1次大乱斗<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
			
			//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 进行3次大乱斗<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
			if (!pMember->TriggerDayTaskDetail(DAY_TASK_DETAIL_TYPE_7))
			{
				LOGERROR("[CBattleTeam::BroadcastConfusionStartMsgToMember] TriggerDayTaskDetail Is Error");
			}
			//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 进行3次大乱斗<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
		}
	}

	//队伍状态更新启动计时器
	FightStart();	
}


//////////////////////////////////////////////////////////////////////
//广播战斗开始玩家数据,生成区域、车辆等级、队伍总积分等信息-zhandui
VOID CBattleTeam::BroadcastFightTeamStartMsgToMember(CBattleTeam* pEnemyTeam)
{	
	if (!pEnemyTeam)
	{
		return;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////
	//处理我方队友位置等信息
    CMsgBattle msg;
    CMsgFriendInfo msg2;
    CMsgFriendInfo msg3;
	INT nRegionId = Random(_FIGHTTEAM_REILVE_REGION_RANGDOM);

	size_t nSize = GetMemberAmount();
	LOGDEBUG("[CBattleTeam::BroadcastFightTeamStartMsgToMember] nSize=[%d]", nSize);
	for(size_t i=0; i<nSize; i++)
	{
		Player* pMember	= this->GetMemberByIndex(i);
		if(pMember)
		{
			//设置玩家活着状态
			pMember->SetDeadStatus(_STATUS_ALIVE);

			//add by huyf 2018.11.24:添加好友系统
			pMember->SetGameStatus(_FRIEND_STATUS_GAMEING);
			//add end:添加好友系统
			
			//!alter by huyf 2019.01.09:车辆改造修改
			CItemDataManager* pItemDataManager = pMember->QueryItemDataManager();
			if (!pItemDataManager)
			{
				LOGERROR("[CBattleTeam::BroadcastFightTeamStartMsgToMember] pItemDataManager=NULL");
				continue;
			}

			//当前使用的车辆itemtype
			UINT nCurCarItemType = 100010;
			CItemData* pItemData = pItemDataManager->QueryItemData(pMember->GetUseCar());
			if (pItemData)
			{
				nCurCarItemType = pItemData->GetItemType();	
			}	

			//当前使用的拖尾itemtype
			UINT nCurTailItemType = 110010;
			pItemData = pItemDataManager->QueryItemData(pMember->GetTail());
			if (pItemData)
			{
				nCurTailItemType = pItemData->GetItemType();	
			}		

			if (i==0)
			{
				msg.CreateMsg(_BATTLE_ACTION_TEAM_START_SUC, _BATTLE_MODEL_RB_FIGHT_TEAM, m_id, pMember->GetUid(), BattleCfg()->GetBcftMaxFightTeamCostTime(), nCurCarItemType/*pMember->GetUseCar()*/, 
					m_nFightTeamColour, 0, 0, _FIGHTTEAM_DEFAULT_CAR_LEVEL_, nCurTailItemType, nRegionId++%_FIGHTTEAM_REILVE_REGION_RANGDOM, pMember->GetNickName().c_str(), pMember->GetHeadPicurl().c_str());	
			}
			else
			{
				msg.AppendMsg(_BATTLE_ACTION_TEAM_START_SUC, _BATTLE_MODEL_RB_FIGHT_TEAM, m_id, pMember->GetUid(), BattleCfg()->GetBcftMaxFightTeamCostTime(), nCurCarItemType/*pMember->GetUseCar()*/, 
					m_nFightTeamColour, 0, 0, _FIGHTTEAM_DEFAULT_CAR_LEVEL_, nCurTailItemType, nRegionId++%_FIGHTTEAM_REILVE_REGION_RANGDOM, i, pMember->GetNickName().c_str(), pMember->GetHeadPicurl().c_str());			
			}
			//!alter end:车辆改造修改

			//add by huyf 2018.11.24:添加好友系统
			//广播玩家进入战斗状态给自己好友			
			msg2.CreateMsg(_FRIEND_INFO_UPDATE, pMember, pMember->GetGameStatus(), _FRIEND_INFO_ATTRIB_GAMESTATUS);			
			//add end:添加好友系统
		}
	}

	size_t nEtSize = pEnemyTeam->GetMemberAmount();
	LOGDEBUG("[CBattleTeam::BroadcastFightTeamStartMsgToMember] nEtSize=[%d]", nEtSize);
	for(size_t i=0; i<nEtSize; i++)
	{
		Player* pMember	= pEnemyTeam->GetMemberByIndex(i);
		if(pMember)
		{
			//设置玩家活着状态
			pMember->SetDeadStatus(_STATUS_ALIVE);

			//add by huyf 2018.11.24:添加好友系统
			pMember->SetGameStatus(_FRIEND_STATUS_GAMEING);
			//add end:添加好友系统
			
			//!alter by huyf 2019.01.09:车辆改造修改
			CItemDataManager* pItemDataManager = pMember->QueryItemDataManager();
			if (!pItemDataManager)
			{
				LOGERROR("[CBattleTeam::BroadcastFightTeamStartMsgToMember] pItemDataManager=NULL");
				continue;
			}

			//当前使用的车辆itemtype
			UINT nCurCarItemType = 100010;
			CItemData* pItemData = pItemDataManager->QueryItemData(pMember->GetUseCar());
			if (pItemData)
			{
				nCurCarItemType = pItemData->GetItemType();	
			}

			//当前使用的拖尾itemtype
			UINT nCurTailItemType = 110010;
			pItemData = pItemDataManager->QueryItemData(pMember->GetTail());
			if (pItemData)
			{
				nCurTailItemType = pItemData->GetItemType();	
			}
			
			msg.AppendMsg(_BATTLE_ACTION_TEAM_START_SUC, _BATTLE_MODEL_RB_FIGHT_TEAM, pEnemyTeam->m_id, pMember->GetUid(), BattleCfg()->GetBcftMaxFightTeamCostTime(), nCurCarItemType/*pMember->GetUseCar()*/, 
				pEnemyTeam->m_nFightTeamColour, 0, 0, _FIGHTTEAM_DEFAULT_CAR_LEVEL_, nCurTailItemType, nRegionId++%_FIGHTTEAM_REILVE_REGION_RANGDOM+_FIGHTTEAM_REILVE_REGION_RANGDOM, i+nSize, pMember->GetNickName().c_str(), pMember->GetHeadPicurl().c_str());
			//!alter end:车辆改造修改

			//add by huyf 2018.11.24:添加好友系统
			//广播玩家进入战斗状态给自己好友			
			msg3.CreateMsg(_FRIEND_INFO_UPDATE, pMember, pMember->GetGameStatus(), _FRIEND_INFO_ATTRIB_GAMESTATUS);			
			//add end:添加好友系统
		}
	}

	//广播给我方战队-开始战争消息
	for(size_t i=0; i<nSize; i++)
	{
		Player* pMember	= this->GetMemberByIndex(i);
		if(pMember)
		{
			//初始化 红蓝对战战斗结果
			pMember->SetFightTeamResult(_FIGHT_RESULT_NONE);

			LOGDEBUG("[CBattleTeam::BroadcastFightTeamStartMsgToMember] SendMsgToClient = [%d]", i);
			pMember->SendMsgToClient(&msg);

			//add by huyf 2018.11.24:添加好友系统
			//广播玩家进入战斗状态给自己好友
			pMember->QueryFriendManager().BroadcastFriendSMsg(&msg2);			
			//add end:添加好友系统
			
			//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成1次组队战<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
			if (!pMember->TriggerDayTaskDetail(DAY_TASK_DETAIL_TYPE_8))
			{
				LOGERROR("[CBattleTeam::BroadcastFightTeamStartMsgToMember] TriggerDayTaskDetail Is Error");
			}
			//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成1次组队战<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
			
			//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成5次组队游戏<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
			if (!pMember->TriggerDayTaskDetail(DAY_TASK_DETAIL_TYPE_9))
			{
				LOGERROR("[CBattleTeam::BroadcastFightTeamStartMsgToMember] TriggerDayTaskDetail Is Error");
			}
			//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成5次组队游戏<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
		}
	}

	//广播给地方战队-开始战争消息
	//nSize = pEnemyTeam->GetMemberAmount();
	
	for(size_t i=0; i<nEtSize; i++)
	{
		Player* pMember	= pEnemyTeam->GetMemberByIndex(i);
		if(pMember)
		{
			//初始化 红蓝对战战斗结果
			pMember->SetFightTeamResult(_FIGHT_RESULT_NONE);

			LOGDEBUG("[CBattleTeam::BroadcastFightTeamStartMsgToMember] pEnemyTeam-SendMsgToClient = [%d]", i);
			pMember->SendMsgToClient(&msg);

			//add by huyf 2018.11.24:添加好友系统
			//广播玩家进入战斗状态给自己好友
			pMember->QueryFriendManager().BroadcastFriendSMsg(&msg3);			
			//add end:添加好友系统
			
			//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成1次组队战<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
			if (!pMember->TriggerDayTaskDetail(DAY_TASK_DETAIL_TYPE_8))
			{
				LOGERROR("[CBattleTeam::BroadcastFightTeamStartMsgToMember] TriggerDayTaskDetail Is Error");
			}
			//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成1次组队战<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
			
			//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成5次组队游戏<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
			if (!pMember->TriggerDayTaskDetail(DAY_TASK_DETAIL_TYPE_9))
			{
				LOGERROR("[CBattleTeam::BroadcastFightTeamStartMsgToMember] TriggerDayTaskDetail Is Error");
			}
			//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 完成5次组队游戏<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
		}
	}
}

//////////////////////////////////////////////////////////////////////
//广播战斗结束玩家数据,生成区域、击杀、积分等信息
VOID CBattleTeam::BroadcastTopListMsgToMember()
{
	//Player* pKingPlayer = NULL;							//统计霸主
	INT nMaxSorce = 0;									//积分
	VT_BATTLECONFUSIONTEAM_TOP_LIST vtBctTopList;		//战斗排行榜计算数据
	BattleConfusionTeamTopInfo bctTopInfo;
	//memset(&bctTopInfo, 0, sizeof(BattleConfusionTeamTopInfo));

	size_t nSize = GetMemberAmount();
	for(size_t i=0; i<nSize; i++)
	{
		Player* pMember	= this->GetMemberByIndex(i);
		if(pMember)
		{
			//获取玩家积分
			INT nMemberKillSum = pMember->GetFightKillSum();
			INT nMemberSorce = pMember->GetFightScore();
			INT nUid = pMember->GetUid();
			std::string strNickName = pMember->GetNickName();
			std::string strHeadPicurl = pMember->GetHeadPicurl();
			
			//筛选出最大的积分霸主
			if (nMaxSorce < nMemberSorce)
			{
				nMaxSorce = nMemberSorce;
				m_pKingPlayer = pMember;
			}

			//构建info
			bctTopInfo.nUid = nUid;
			bctTopInfo.nSorce = nMemberSorce;
			bctTopInfo.nKillSum = nMemberKillSum;
			bctTopInfo.strNickName = strNickName.c_str();
			bctTopInfo.strHeadPicurl = strHeadPicurl.c_str();

			//插入数组
			vtBctTopList.push_back(bctTopInfo);
		}
	}

	//设置霸主标识：100
	if (m_pKingPlayer)
	{
		m_pKingPlayer->SetKingRank(100);

		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 获得1次大乱斗排名第1<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
		if (!m_pKingPlayer->TriggerDayTaskDetail(DAY_TASK_DETAIL_TYPE_10))
		{
			LOGERROR("[CBattleTeam::BroadcastFightTeamStartMsgToMember] TriggerDayTaskDetail Is Error");
		}
		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>触发日常任务 获得1次大乱斗排名第1<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<//
	}

	//排序数组
	sort(vtBctTopList.begin(), vtBctTopList.end(), less_);
	//稳定
	//stable_sort(vtBctTopList.begin(), vtBctTopList.end(),less<student>());
	//向前端发送排行榜数据
	CMsgBattle msg;
	nSize = vtBctTopList.size();
	for(size_t i=0; i<nSize; i++)
	{
		if (i == 0)
		{
			msg.CreateMsg(_BATTLE_ACTION_BATTLE_END_SUC, _BATTLE_MODEL_CONFUSION, 0, vtBctTopList[i].nUid, 0, 0, 0, vtBctTopList[i].nKillSum,
					vtBctTopList[i].nSorce, 0, 0, 0, vtBctTopList[i].strNickName, vtBctTopList[i].strHeadPicurl);				
		}
		else
		{
			msg.AppendMsg(_BATTLE_ACTION_BATTLE_END_SUC, _BATTLE_MODEL_CONFUSION, 0, vtBctTopList[i].nUid, 0, 0, 0, vtBctTopList[i].nKillSum,
					vtBctTopList[i].nSorce, 0, 0, 0, i, vtBctTopList[i].strNickName, vtBctTopList[i].strHeadPicurl);			
		}
	}

	//广播开始战争消息
	for(size_t i=0; i<nSize; i++)
	{
		Player* pMember	= this->GetMemberByIndex(i);
		if(pMember)
		{
			pMember->SendMsgToClient(&msg);
		}
	}
}
//广播战斗结束数据-大乱斗
VOID CBattleTeam::BroadcastConfusionEndMsgToMember()
{
	//广播战斗结果toplist消息
	BroadcastTopListMsgToMember();

	//!alter by huyf 2018.11.26:不用在战斗结束的时候发送，好友更新数据，已经迁移到msgaction中点击返回主界面的时候发送
	// size_t nSize = GetMemberAmount();
	// //广播开始战争消息
	// for(size_t i=0; i<nSize; i++)
	// {
	// 	Player* pMember	= this->GetMemberByIndex(i);
	// 	if(pMember)
	// 	{			
	// 		//设置玩家活着状态
	// 		pMember->SetDeadStatus(_STATUS_ALIVE);

	// 		//add by huyf 2018.11.24:添加好友系统
	// 		pMember->SetGameStatus(_FRIEND_STATUS_ONLINE);
	// 		//add end:添加好友系统
			
	// 		//add by huyf 2018.11.24:添加好友系统
	// 		//广播玩家进入战斗状态给自己好友
	// 		CMsgFriendInfo msg;
	// 		IF_OK(msg.CreateMsg(_FRIEND_INFO_UPDATE, pMember, pMember->GetGameStatus(), _FRIEND_STATUS_ONLINE))
	// 		{
	// 			pMember->QueryFriendManager().BroadcastFriendSMsg(&msg);
	// 		}
	// 		//add end:添加好友系统
	// 	}
	// }
	//!alter end:不用在战斗结束的时候发送，好友更新数据，已经迁移到msgaction中点击返回主界面的时候发送
}
////////////////////////////////////////////////////////////////////////
//广播战斗结束数据-红蓝战队
VOID CBattleTeam::BroadcastFightTeamEndMsgToMember()
{
	if (!m_pEnemyTeam)
	{
		LOGERROR("[CBattleTeam::BroadcastFightTeamEndMsgToMember] Is Error... m_pEnemyTeam=NULL OR have BroadcastTeamMsg...\n");
		return;
	}

	//规定红队的结果在第一个段中蓝队的结果在第二个段中
	//红队
	UINT nRTeamid = m_id;
	UINT nBTeamid = m_pEnemyTeam->m_id;
	UINT nRFightResult = m_nFightResult;												//红方
	UINT nBFightResult = _FIGHT_RESULT_DEFEAT;//m_pEnemyTeam->m_nFightResult;		//蓝方
	UINT nRFightTotalScore = m_nFightTotalScore;
	UINT nBFightTotalScore = m_pEnemyTeam->m_nFightTotalScore;
	//奖励金币
	UINT nAwardGoldR = BattleCfg()->GetBcftWinAwardGold();
	UINT nAwardGoldB = BattleCfg()->GetBcftDefaultAwardGold();
	//红方胜利
	if (m_nFightResult == _FIGHT_RESULT_WIN)
	{
		//蓝方就失败
		nBFightResult = _FIGHT_RESULT_DEFEAT;

		nAwardGoldR = BattleCfg()->GetBcftWinAwardGold();
		nAwardGoldB = BattleCfg()->GetBcftDefaultAwardGold();
	}
	//红方平局
	else if(m_nFightResult == _FIGHT_RESULT_DRAW)
	{
		//蓝方就平局
		nBFightResult = _FIGHT_RESULT_DRAW;

		nAwardGoldR = BattleCfg()->GetBcftDrawAwardGold();
		nAwardGoldB = BattleCfg()->GetBcftDrawAwardGold();
	}
	//红方失败
	else//(m_nFightResult == _FIGHT_RESULT_DEFEAT)
	{
		//蓝方就胜利
		nBFightResult = _FIGHT_RESULT_WIN;

		nAwardGoldR = BattleCfg()->GetBcftDefaultAwardGold();
		nAwardGoldB = BattleCfg()->GetBcftWinAwardGold();
	}

	//蓝队
	if(_FIGHT_OWNER_COLOUR_BLUE == m_nFightTeamColour)
	{
		nRTeamid = m_pEnemyTeam->m_id;
		nBTeamid = m_id;

		nRFightResult = _FIGHT_RESULT_DEFEAT;//m_pEnemyTeam->m_nFightResult;
		nBFightResult = m_nFightResult;

		nRFightTotalScore = m_pEnemyTeam->m_nFightTotalScore;
		nBFightTotalScore = m_nFightTotalScore;

		//蓝方胜利
		if (m_nFightResult == _FIGHT_RESULT_WIN)
		{
			//红方就失败
			nRFightResult = _FIGHT_RESULT_DEFEAT;

			nAwardGoldR = BattleCfg()->GetBcftDefaultAwardGold();
			nAwardGoldB = BattleCfg()->GetBcftWinAwardGold();
		}
		//蓝方平局
		else if(m_nFightResult == _FIGHT_RESULT_DRAW)
		{
			//红方就平局
			nRFightResult = _FIGHT_RESULT_DRAW;

			nAwardGoldR = BattleCfg()->GetBcftDrawAwardGold();
			nAwardGoldB = BattleCfg()->GetBcftDrawAwardGold();
		}
		//蓝方失败
		else//(m_nFightResult == _FIGHT_RESULT_DEFEAT)
		{
			//红方就胜利
			nRFightResult = _FIGHT_RESULT_WIN;

			nAwardGoldR = BattleCfg()->GetBcftWinAwardGold();
			nAwardGoldB = BattleCfg()->GetBcftDefaultAwardGold();
		}
	}	

	CMsgBattle msg;
	msg.CreateMsg(_BATTLE_ACTION_TEAM_END_SUC, _BATTLE_MODEL_RB_FIGHT_TEAM, nRTeamid, nRFightResult, 
		nRFightTotalScore, nAwardGoldR, nAwardGoldB, nBTeamid, nBFightResult, nBFightTotalScore);

	//战斗结果广播给我队伍成员
	size_t nSize = GetMemberAmount();
	for(size_t i=0; i<nSize; i++)
	{
		Player* pMember	= this->GetMemberByIndex(i);
		if(pMember)
		{
			pMember->SendMsgToClient(&msg);	

			//设置玩家活着状态
			pMember->SetDeadStatus(_STATUS_ALIVE);

			//!alter by huyf 2018.11.26:不用在战斗结束的时候发送，好友更新数据，已经迁移到msgaction中点击返回主界面的时候发送
			// //add by huyf 2018.11.24:添加好友系统
			// pMember->SetGameStatus(_FRIEND_STATUS_ONLINE);
			// //add end:添加好友系统
			
			// //add by huyf 2018.11.24:添加好友系统
			// //广播玩家进入战斗状态给自己好友
			// CMsgFriendInfo msg2;
			// IF_OK(msg2.CreateMsg(_FRIEND_INFO_UPDATE, pMember, pMember->GetGameStatus(), _FRIEND_STATUS_ONLINE))
			// {
			// 	pMember->QueryFriendManager().BroadcastFriendSMsg(&msg2);
			// }
			// //add end:添加好友系统
			//!alter end:不用在战斗结束的时候发送，好友更新数据，已经迁移到msgaction中点击返回主界面的时候发送
		}		
	}

	//战斗结果广播给敌方队伍成员
	nSize = m_pEnemyTeam->GetMemberAmount();
	for(size_t i=0; i<nSize; i++)
	{
		Player* pMember	= m_pEnemyTeam->GetMemberByIndex(i);
		if(pMember)
		{
			pMember->SendMsgToClient(&msg);	

			//设置玩家活着状态
			pMember->SetDeadStatus(_STATUS_ALIVE);

			// //!alter by huyf 2018.11.26:不用在战斗结束的时候发送，好友更新数据，已经迁移到msgaction中点击返回主界面的时候发送
			// //add by huyf 2018.11.24:添加好友系统
			// pMember->SetGameStatus(_FRIEND_STATUS_ONLINE);
			// //add end:添加好友系统
			
			// //add by huyf 2018.11.24:添加好友系统
			// //广播玩家进入战斗状态给自己好友
			// CMsgFriendInfo msg2;
			// IF_OK(msg2.CreateMsg(_FRIEND_INFO_UPDATE, pMember, pMember->GetGameStatus(), _FRIEND_STATUS_ONLINE))
			// {
			// 	pMember->QueryFriendManager().BroadcastFriendSMsg(&msg2);
			// }
			// //add end:添加好友系统					
			// //!alter end:不用在战斗结束的时候发送，好友更新数据，已经迁移到msgaction中点击返回主界面的时候发送
		}		
	}

	//敌方战队的指针也设置为空，因为消息已经在上面给广播掉了。
	m_pEnemyTeam->SetEnemyTeam(NULL);
	m_pEnemyTeam = NULL;
}


//发送生成道具数据包给队伍中的每个玩家，由他来生成像素坐标点回传服务器
VOID CBattleTeam::CreateFightItemPos()
{
	size_t nSize = GetMemberAmount();
	for(size_t i=0; i<nSize; i++)
	{
		Player* pMember	= this->GetMemberByIndex(i);
		if(pMember)
		{
			CMsgItem msg;
			if(msg.CreateMsg(_ACT_ITEM_CREATE_RANDOM_ITEM_POS))
			{
				pMember->SendMsgToClient(&msg);
			}
		}	
	}
}

//生成对战道具并广播给队伍成员
BOOL CBattleTeam::CreateFightItem(ItemInfo& iteminfo)
{
	iteminfo.nId = _FIGHT_CREATE_RANDOM_ITEM_ID++;

	//随机生成3中道具
	INT nRandom = Random(_CONFUSION_ITEMTYPE_AMOUNT);//[0,1,2]
	iteminfo.nItemType = _ITEM_TYPE[nRandom];
	iteminfo.nSorce = _ITEM_SORCE[nRandom];

	m_vItemList.push_back(iteminfo);
	LOGDEBUG("[CBattleTeam::CreateFightItem] nRandom=[%d] nId=[%d] nItemType=[%d] nSorce=[%d] m_vItemList.szie=[%u]\n", nRandom, iteminfo.nId, iteminfo.nItemType, iteminfo.nSorce, m_vItemList.size());
	return TRUE;	
}

//成员拾取战斗道具增加积分
BOOL CBattleTeam::PickUpFightItem(Player* pPlayer, INT nItemId)
{
	INT nAddSorce = 0;
	//所搜对应道具
	for (auto iter = m_vItemList.begin(); iter != m_vItemList.end(); iter++)
    {
    	if((*iter).nId == nItemId)
    	{
    		nAddSorce = (*iter).nSorce;
    		LOGDEBUG("[CBattleTeam::PickUpFightItem] nId=[%d] nItemType=[%d] 道具增加积分=[%d]\n", nItemId, (*iter).nItemType, nAddSorce);
    		//从队列中删除战斗道具
    		m_vItemList.erase(iter);
    		break;   
    	}
    }

    //搜索对应玩家
	for (auto iter = m_vtTeamPlayerS.begin(); iter != m_vtTeamPlayerS.end(); iter++)
    {	
    	//增加战斗积分
    	if((*iter) == pPlayer)
    	{
    		pPlayer->SetFightScore(pPlayer->GetFightScore() + nAddSorce, FALSE);
    		UINT nCfgLev = pPlayer->GetFightCfgLevel(pPlayer->GetFightScore());
    		if (nCfgLev != pPlayer->GetFightLevel())
    		{
    			pPlayer->SetFightLevel(nCfgLev, FALSE);
    		}
    		LOGDEBUG("[CBattleTeam::PickUpFightItem] pPlayerID=[%d] GetFightScore=[%d] 道具增加积分=[%d] pPlayer->GetFightCfgLevel()=[%d] nCfgLev=[%d] \n", pPlayer->GetUid(), pPlayer->GetFightScore(), nAddSorce, pPlayer->GetFightLevel(), nCfgLev);

    		return TRUE;
    	}
    }

    return FALSE;
}

//更新队伍总积分数
VOID CBattleTeam::UpdateFightScore(INT nFightScore)
{
	//总积分小于50，进行数值累加
	//if (m_nFightTotalScore < BattleCfg()->GetBcftMaxFightTeamFinishScore())
	{
		m_nFightTotalScore = (m_nFightTotalScore + nFightScore) > (INT)(BattleCfg()->GetBcftMaxFightTeamFinishScore()) ? (INT)(BattleCfg()->GetBcftMaxFightTeamFinishScore()) : (m_nFightTotalScore + nFightScore);
	}
}

//是否战役已经打完
BOOL CBattleTeam::IsFightTeamFinsh()
{	
	//注意****敌方的队伍战斗结果需要开启来。
	//if (m_nTeamStatus != _TEAM_STATUS_START)
	//{
	//	LOGERROR("CBattleTeam::IsFightTeamFinsh() Is Error...m_nTeamStatus=[%d]", m_nTeamStatus);
	//	return FALSE;
	//}

	//如果此时满足50积分，则判定此队伍已经胜利
	if (m_nFightTotalScore >= (INT)(BattleCfg()->GetBcftMaxFightTeamFinishScore()))
	{
		if(!m_pEnemyTeam)
		{
			LOGERROR("[CBattleTeam::IsFightTeamFinsh] Is Error...m_pEnemyTeam=NULL-1\n");
			return TRUE;
		}
		//设置我方状态为胜利
		m_nFightResult = _FIGHT_RESULT_WIN;

		//设置我方 战争为结束状态
		m_nTeamStatus = _TEAM_STATUS_END;

		//设置敌方队伍的状态为失败
		m_pEnemyTeam->m_nFightResult = _FIGHT_RESULT_DEFEAT;

		//!alter by huyf 2019.01.12:这里不能设置敌方战队位完成状态，否则后面队伍的循环判定就不为战斗开始状态了
		//设置敌方队伍 战争为结束状态
		//m_pEnemyTeam->m_nTeamStatus = _TEAM_STATUS_END;
		//!later end:这里不能设置敌方战队位完成状态，否则后面队伍的循环判定就不为战斗开始状态了

		//设置我方战斗结果
		SetFightTeamResultToMember(m_nFightResult);
		
		//设置敌方战斗结果
		m_pEnemyTeam->SetFightTeamResultToMember(m_pEnemyTeam->m_nFightResult);

		return TRUE;
	}

	//时间到也完成
	if (m_nTimeStamp != 0 && m_nTimeStamp + BattleCfg()->GetBcftMaxFightTeamCostTime() < time(NULL))
	{
		if(!m_pEnemyTeam)
		{
			LOGERROR("CBattleTeam::IsFightTeamFinsh() Is Error...m_pEnemyTeam=NULL-2\n");
			return TRUE;
		}
		//胜利
		if (m_nFightTotalScore > m_pEnemyTeam->m_nFightTotalScore)
		{
			//设置我方状态为胜利
			m_nFightResult = _FIGHT_RESULT_WIN;

			//设置我方 战争为结束状态
			m_nTeamStatus = _TEAM_STATUS_END;

			//设置敌方队伍的状态为失败
			m_pEnemyTeam->m_nFightResult = _FIGHT_RESULT_DEFEAT;

			//设置敌方队伍 战争为结束状态
			//m_pEnemyTeam->m_nTeamStatus = _TEAM_STATUS_END;

		}
		//战平
		else if (m_nFightTotalScore == m_pEnemyTeam->m_nFightTotalScore)
		{
			//设置我方状态为胜利
			m_nFightResult = _FIGHT_RESULT_DRAW;

			//设置我方 战争为结束状态
			m_nTeamStatus = _TEAM_STATUS_END;

			//设置敌方队伍的状态为失败
			m_pEnemyTeam->m_nFightResult = _FIGHT_RESULT_DRAW;

			//设置敌方队伍 战争为结束状态
			//m_pEnemyTeam->m_nTeamStatus = _TEAM_STATUS_END;

		}
		//失败
		else 
		{
			//设置我方状态为胜利
			m_nFightResult = _FIGHT_RESULT_DEFEAT;

			//设置我方 战争为结束状态
			m_nTeamStatus = _TEAM_STATUS_END;

			//设置敌方队伍的状态为失败
			m_pEnemyTeam->m_nFightResult = _FIGHT_RESULT_WIN;

			//设置敌方队伍 战争为结束状态
			//m_pEnemyTeam->m_nTeamStatus = _TEAM_STATUS_END;

		}

		//设置我方战斗结果
		SetFightTeamResultToMember(m_nFightResult);

		//设置敌方战斗结果
		m_pEnemyTeam->SetFightTeamResultToMember(m_pEnemyTeam->m_nFightResult);
		
		return TRUE;
	}

	//如果一方退出的战队成员为0，则判定战斗结束，进行结算
	if (GetMemberAmount() == 0)
	{
		if(!m_pEnemyTeam)
		{
			LOGERROR("[CBattleTeam::IsFightTeamFinsh] Is Error...m_pEnemyTeam=NULL-3\n");
			return TRUE;
		}

		//设置我方状态为胜利
		m_nFightResult = _FIGHT_RESULT_DEFEAT;

		//设置我方 战争为结束状态
		m_nTeamStatus = _TEAM_STATUS_END;

		//设置我方战斗结果
		SetFightTeamResultToMember(m_nFightResult);

		//!alter by huyf 2019.01.12:一方队伍全体成员战斗中全部退出则需要设置另一方队伍的战斗结果位胜利
		//设置敌方队伍的状态为失败
		m_pEnemyTeam->m_nFightResult = _FIGHT_RESULT_WIN;

		//设置敌方队伍 战争为结束状态
		//m_pEnemyTeam->m_nTeamStatus = _TEAM_STATUS_END;

		//设置敌方战斗结果
		m_pEnemyTeam->SetFightTeamResultToMember(m_pEnemyTeam->m_nFightResult);
		//!alter end:一方队伍全体成员战斗中全部退出则需要设置另一方队伍的战斗结果位胜利

		return TRUE;
	}

	return FALSE;
}

//设置队伍成员战斗结果
BOOL CBattleTeam::SetFightTeamResultToMember(INT nResult)
{
	//搜索对应玩家
	for (auto iter = m_vtTeamPlayerS.begin(); iter != m_vtTeamPlayerS.end(); iter++)
    {	
    	if(*iter)
    	{
            LOGDEBUG("[CBattleTeam::SetFightTeamResultToMember] nResult=%d\n ", nResult);
    		(*iter)->SetFightTeamResult(nResult);    		
    	}
    }
    return TRUE;
}

//发放奖励
VOID CBattleTeam::Award()
{
	INT nKillSum = 0;
	INT nAwardGold = 0;
	INT nAwardKingSum = 0;

	//搜索对应玩家
	for (auto iter = m_vtTeamPlayerS.begin(); iter != m_vtTeamPlayerS.end(); iter++)
    {	
    	if((*iter))
    	{
    		//奖励金币
    		nKillSum = (*iter)->GetFightKillSum();
    		nAwardGold = ((*iter)->GetAddGold() + 1) * nKillSum * BattleCfg()->GetBccPerKillAwardGold();
    		(*iter)->SetGoldSum((*iter)->GetGoldSum() + nAwardGold);

    		LOGDEBUG("[CBattleTeam::Award] nKillSum=[%d] nAwardGold=[%d] GetAddGold=[%d]\n", nKillSum, nAwardGold, (*iter)->GetAddGold());
    		nAwardKingSum = 0;
    		
    		//霸主额外奖励霸主令牌(一种货币)
		    if ((*iter) == m_pKingPlayer)
		    {
		    	nAwardKingSum = 1;
		    	m_pKingPlayer->SetKingSum(m_pKingPlayer->GetKingSum() + nAwardKingSum);
		    	LOGDEBUG("[CBattleTeam::Award] nKillSum=[%d] nAwardGold=[%d] GetAddGold=[%d] nAwardKingSum=[%d]\n", nKillSum, nAwardGold, (*iter)->GetAddGold(), nAwardKingSum);
		    }

    		//还原战斗初始化数据
    		//(*iter)->SetFightKillSum(0);
    		//(*iter)->SetFightLevel(1);
    		//(*iter)->SetFightScore(1000);

    		//BUFF商城相关
    		//(*iter)->SetAddScore(0);
    		//(*iter)->SetAddGold(0);

    		//同步玩家到前端
    		CMsgUserInfo msg;
    		if(msg.CreateMsg((*iter)))
    		{
    			(*iter)->SendMsgToClient(&msg);
    		}

    		//同步玩家到前端
    		CMsgBattle msg2;
    		if(msg2.CreateMsg(_BATTLE_ACTION_END_AWARD_SUC, _BATTLE_MODEL_CONFUSION, nAwardKingSum, (*iter)->GetUid(), nAwardGold))
    		{
    			(*iter)->SendMsgToClient(&msg2);
    		}
    	}
    }     
}

//给红蓝对战玩家发放奖励
VOID CBattleTeam::AwardFightTeam()
{
	int nAwardGold = 0;

	//奖励金币
	if (m_nFightResult == _FIGHT_RESULT_WIN)
	{
		nAwardGold = BattleCfg()->GetBcftWinAwardGold();
	}
	else if(m_nFightResult == _FIGHT_RESULT_DRAW)
	{
		nAwardGold = BattleCfg()->GetBcftDrawAwardGold();
	}
	else //if(m_nFightResult == _FIGHT_RESULT_DEFEAT)
	{
		nAwardGold = _FIGHTTEAM_DEFEAT_AWARD_GOLD;
	}

	//搜索对应玩家
	for (auto iter = m_vtTeamPlayerS.begin(); iter != m_vtTeamPlayerS.end(); iter++)
    {	
    	if((*iter))
    	{
    		LOGDEBUG("[CBattleTeam::AwardFightTeam] Playerid=[%d] GoldSum=[%d] nAwardGold=[%d]", (*iter)->GetUid(), (*iter)->GetGoldSum(), nAwardGold);
    		(*iter)->SetGoldSum((*iter)->GetGoldSum() + nAwardGold);
    		LOGDEBUG("[CBattleTeam::AwardFightTeam] Playerid=[%d] GoldSum=[%d]", (*iter)->GetUid(), (*iter)->GetGoldSum());

    		//同步玩家到前端
    		CMsgUserInfo msg;
    		if(msg.CreateMsg((*iter)))
    		{
    			(*iter)->SendMsgToClient(&msg);
    		}
    	}
    }  
}
//插入服务器队伍的消息队列缓存
VOID CBattleTeam::InsertFrameMsgCache(INT nModel/*=_BATTLE_MODEL_CONFUSION*/, INT nAction/*=_WALK_ACTION_USER_CMD*/, INT nTargetId/*=0*/, INT nPointX/*=0*/, INT nPointY/*=0*/, INT nPointStartX/*=0 type*/, INT nPointStartY/*=0*/,INT nPointEndX/*=0*/, INT nPointEndY/*=0*/, INT nBeCollideId/*=0*/, INT nTType/*=0*/, INT nTPowerX/*=0*/, INT nTPowerY/*=0*/)
{
	// ////////////////////////////////////////////////////////////////////////
	// //装载到multimap中
	// CBattleTeam* pBt = NULL;
	// Player* pPlayer = g_pPlayerPool->GetPlayerByUid(nTargetId);
	// if (!pPlayer)
	// {
	// 	LOGERROR("[CBattleTeam::InsertFrameMsgCache] Is Error pPlayer == NULL nTargetId=[%d]", nTargetId);
	// 	return;
	// }
	// //大乱斗模式
	// if (nModel == _BATTLE_MODEL_CONFUSION)
	// {
	// 	pBt = pPlayer->GetBct();
	// }
	// //红蓝队对战模式
	// else// if (nModel ==  _BATTLE_MODEL_RB_FIGHT_TEAM)
	// {
	// 	pBt = pPlayer->GetBrbt();
	// }

	// if(pBt == NULL)
	// {
	// 	LOGERROR("[CBattleTeam::InsertFrameMsgCache] Is Error pBt == NULL nTargetId=[%d]", nTargetId);
	// 	return;
	// }
	
	//调试的时候发现，误传了0进来
	if (nTargetId == 0)
	{
		LOGERROR("[CBattleTeam::InsertFrameMsgCache] Is Error nTargetId=[%d]", nTargetId);
	 	return;
	}

	//装载数据结构
	TeamNetMsg tnm;
	tnm.nModel = nModel;
	tnm.nAction = nAction;

	tnm.nTargetId = nTargetId;
	tnm.nPointX = nPointX;
	tnm.nPointY = nPointY;
	tnm.nPointStartX = nPointStartX;
	tnm.nPointStartY = nPointStartY;
	tnm.nPointEndX = nPointEndX;
	tnm.nPointEndY = nPointEndY;

	tnm.nBeCollideId = nBeCollideId;
	tnm.nTType = nTType;
	tnm.nTPowerX = nTPowerX;
	tnm.nTPowerY = nTPowerY;

	//插入对应的队伍消息缓存
	pair<UINT, TeamNetMsg> p(nTargetId, tnm);
	m_mapTeamNetMsg.insert(p);
	
	////////////////////////////////////////////////////////////////////////
	long long llRecvMsgTimeStamp = GetMillSecTime();
	//if (nAction == 7 || nAction == 9)
	{
		LOGDEBUG("[CBattleTeam::InsertFrameMsgCache] PlayerId=[%d] nModel=[%d] llRecvMsgTimeStamp=[%lld] nAction=[%d] nPointX=[%d] nPointY=[%d] nPointStartX=[%d] nPointStartY=[%d] nPointEndX=[%d] nPointEndY=[%d] nBeCollideId=[%d] nTType=[%d] nTPowerX=[%d] nTPowerY=[%d] size=[%d]", 
			nTargetId, nModel, llRecvMsgTimeStamp, nAction, nPointX, nPointY, nPointStartX, nPointStartY, nPointEndX, nPointEndY, nBeCollideId, nTType, nTPowerX, nTPowerY, m_mapTeamNetMsg.size());
	}
	
}