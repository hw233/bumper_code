//////////////////////////////////////////////////////////////////////
//文件名称：CBattle.cpp
//功能描述：
//版本说明：CBattle API
//			
//编写作者：2018.10.10 yanfeng.hu
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

#include "Battle.h"
#include "Player.h"
#include "../MsgHandler/MsgBattle.h"
#include "../MsgHandler/MsgSynRoomInfo.h"
#include <vector>
using namespace std;
#include "Battle/BattleCfg.h"

//////////////////////////////////////////////////////////////////////
//构造函数
CBattle::CBattle()
{
	m_vtBcs.clear();
	m_vtBcTeam.clear();
}

//析构函数
CBattle::~CBattle()
{
    ///////////////////////////////////////////////////////////////
    ///大乱斗模式
    /////单人列表
	for (auto iter = m_vtBcs.begin(); iter != m_vtBcs.end(); iter++)
    {	
    	SAFE_DELETE(*iter);
    }
	m_vtBcs.clear();

    //组队列表
	for (auto iter = m_vtBcTeam.begin(); iter != m_vtBcTeam.end(); iter++)
    {	
    	SAFE_DELETE(*iter);
    }
    m_vtBcTeam.clear();

    ///////////////////////////////////////////////////////////////
    ///红蓝战队模式
    /////组队列表
    for (auto iter = m_vtRbTeam.begin(); iter != m_vtRbTeam.end(); iter++)
    {   
        SAFE_DELETE(*iter);
    }
    m_vtRbTeam.clear();
}

//LOOP 接口
BOOL CBattle::Loop()
{
    //大乱斗模式
	for (auto iter = m_vtBcTeam.begin(); iter != m_vtBcTeam.end(); )
    {	
        //LOGDEBUG("[CBattle::Loop()] (*iter)->GetTeamStatus()=[%d] (*iter)->IsQuickMatchTimeout()=[%d]\n", (*iter)->GetTeamStatus(),(*iter)->IsQuickMatchTimeout());
        //大乱斗模式
        //15秒超时查看是否队伍人数满足10人，满足则开启战斗，不满足组更新时间戳
        if ((*iter)->GetTeamStatus() == _TEAM_STATUS_QIUCK_MATCH && (*iter)->IsQuickMatchTimeout())
        {
            if ((*iter)->GetMemberAmount() >= BattleCfg()->GetBccEnouthTeamAmount())
            {
                //设置游戏为: START 状态机
                (*iter)->FightStart();

                //广播开始游戏数据
                (*iter)->BroadcastConfusionStartMsgToMember();

                LOGDEBUG("[CBattle::Loop] TeamId=[%d] IsQuickMatchTimeout  status=[START] MemberAmount=[%d]", (*iter)->GetTeamId(), (*iter)->GetMemberAmount());
            }
            else
            {
                //小于10个人,刷新队伍准备时间戳，继续匹配
                (*iter)->SetQuickMatchTimeStamp(time(NULL));
                LOGDEBUG("[CBattle::Loop] TeamId=[%d] SetQuickMatchTimeStamp  status=[_TEAM_STATUS_QIUCK_MATCH] MemberAmount=[%d]", (*iter)->GetTeamId(), (*iter)->GetMemberAmount());
            }
        }
        //已经开战
        else if ((*iter)->GetTeamStatus() == _TEAM_STATUS_START)
        {
        	//战斗结束时间到，则从队列中删除队伍
        	if((*iter)->IsFightConfusionFinsh())
        	{
                //发放奖励
                (*iter)->Award();

                //发送战斗结束排行榜数据包
                (*iter)->BroadcastConfusionEndMsgToMember();
        		
        		//清理队伍成员
        		(*iter)->ClearMember();

        		//释放内存
        		SAFE_RELEASE(*iter);

        		//删除此队伍元素
        		iter = m_vtBcTeam.erase(iter);
        		continue;
        	}
            else // 未结束	
        	{
                //LOGDEBUG("[CBattle::Loop] CreateFightItemPos...IsActive=[%d] ToNextTime=[%d]", (*iter)->GetFightTimeInterval().IsActive(), (*iter)->GetFightTimeInterval().ToNextTime());
        		//检查生成对战道具
        		if ((*iter)->GetFightTimeInterval().IsActive() && (*iter)->GetFightTimeInterval().ToNextTime())
        		{
        			(*iter)->CreateFightItemPos();
                    LOGDEBUG("[CBattle::Loop] CreateFightItemPos IS SUC...");
        		}

                ///////////////////////////////////////////////////////////////////////
                ///战斗还在继续的时候，广播帧同步包;广播同步帧缓存66ms 0:不缓存 1:缓存
                if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 1)
                {
                    long long llCurMillSecTime = GetMillSecTime();
                    long long mllRecvMsgTimeStamp = (*iter)->GetRecvMsgTimeStamp();
                    
                    //始终便宜10毫秒的话记录日志
                    if (llCurMillSecTime - mllRecvMsgTimeStamp > 50 && mllRecvMsgTimeStamp > 0)
                    {
                        LOGWARNING("[CBattle::Loop] llCurMillSecTime=[%lld] mllRecvMsgTimeStamp=[%lld] MsgTimeStampOffset=[%d]", llCurMillSecTime, mllRecvMsgTimeStamp, llCurMillSecTime - mllRecvMsgTimeStamp);
                    }

                    if (mllRecvMsgTimeStamp != 0 && mllRecvMsgTimeStamp + g_config.ONLINE_BROADCAST_FRAME_MSG_INTERVAL_TIME <= llCurMillSecTime)
                    {
                        //敌方66ms内收到的队员操作数据包
                        (*iter)->BroadcastConfusionFrameMsg();
                        //LOGDEBUG("[CBattle::Loop] BroadcastConfusionFrameMsg To TeamMembers TeamId=[%d] llCurMillSecTime=[%lld] mllRecvMsgTimeStamp=[%lld] TimeStamp=[%lld]", 
                        //(*iter)->GetTeamId(), llCurMillSecTime, mllRecvMsgTimeStamp, llCurMillSecTime-mllRecvMsgTimeStamp);                                         
                    }
                }
                ///////////////////////////////////////////////////////////////////////            
        	}
        }

    	iter++;
    }

    //红蓝战队模式
    for (auto iter = m_vtRbTeam.begin(); iter != m_vtRbTeam.end(); )
    {   
        //15秒超时查看是否队伍人数满足10人，满足则开启战斗，不满足组更新时间戳
        if ((*iter)->GetTeamStatus() == _TEAM_STATUS_QIUCK_MATCH && (*iter)->IsQuickMatchTimeout())
        {
            //刷新队伍开始匹配时间戳，继续匹配
            (*iter)->SetQuickMatchTimeStamp(time(NULL));            
        }
        //已经开战
        else if ((*iter)->GetTeamStatus() == _TEAM_STATUS_START)
        {
            //战斗结束时间到，则从队列中删除队伍
            if((*iter)->IsFightTeamFinsh())
            {
                //发放奖励
                (*iter)->AwardFightTeam();
                //发送战斗结束排行榜数据包
                (*iter)->BroadcastFightTeamEndMsgToMember();
                //清理队伍成员
                (*iter)->ClearMember();

                //释放内存
                SAFE_RELEASE(*iter);

                //删除此队伍元素
                iter = m_vtRbTeam.erase(iter);
                continue;
            }
            else    //未完成
            {
                ///////////////////////////////////////////////////////////////////////
                ///战斗还在继续的时候，广播帧同步包,;广播同步帧缓存66ms 0:不缓存 1:缓存
                if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 1)
                {
                    long long llCurMillSecTime = GetMillSecTime();
                    long long mllRecvMsgTimeStamp = (*iter)->GetRecvMsgTimeStamp();
                    if (mllRecvMsgTimeStamp != 0 && mllRecvMsgTimeStamp + g_config.ONLINE_BROADCAST_FRAME_MSG_INTERVAL_TIME <= llCurMillSecTime)
                    {
                        //敌方66ms内收到的队员操作数据包
                        (*iter)->BroadcastTeamFrameMsg();
                        //LOGDEBUG("[CBattle::Loop] BroadcastTeamFrameMsg To TeamMembers TeamId=[%d] llCurMillSecTime=[%lld] mllRecvMsgTimeStamp=[%lld] TimeStamp=[%lld]", 
                        //(*iter)->GetTeamId(), llCurMillSecTime, mllRecvMsgTimeStamp, llCurMillSecTime-mllRecvMsgTimeStamp);                                         
                    }
                }
                ///////////////////////////////////////////////////////////////////////
            }
        }

        iter++;
    }

    return TRUE;
}

///////////////////////////大乱斗模式//////////////////////////////////////////////////
//添加到单人列表
BOOL CBattle::AddBcs(Player* pPlayer)
{
	CHECKF(pPlayer);

	CBattleConfusionSingle* pBcs = new CBattleConfusionSingle(pPlayer);
	CHECKF(pBcs);

    //设置单人队列元素指针
    pPlayer->SetBcs(pBcs);

    //入队
	m_vtBcs.push_back(pBcs);

	return TRUE;
}

//从单人列表中删除
BOOL CBattle::DelBcs(Player* pPlayer)
{
	CHECKF(pPlayer);

    for (auto iter = m_vtBcs.begin(); iter != m_vtBcs.end(); iter++)
    {	
    	//找到对应玩家
    	if((*iter)->GetSignUpPlayer() ==  pPlayer)
    	{
            //清空单人队列元素指针
            pPlayer->ClearBcs();
            //出队
    		m_vtBcs.erase(iter);
    		return TRUE;
    	}
    }
    return FALSE;
}
//获取单人列表的长度
UINT CBattle::GetBcsAmount()
{
	return m_vtBcs.size();
}

//根据索引获取队伍成员
Player* CBattle::GetPlayerByIndex(UINT nIndex)
{
	if (nIndex >= m_vtBcs.size() || nIndex < 0)
		return NULL;

	return m_vtBcs[nIndex]->GetSignUpPlayer();
}
/////////////////////////////////战队模式///////////////////////////////////////
//创建队伍列表
UINT CBattle::CreateTeam(Player* pPlayer, INT nModelType)
{
	CHECKF(pPlayer);

    CBattleTeam* pBt = new CBattleTeam();
    CHECKF(pBt);
    //设置队长id
    pBt->SetTeamLeaderID(pPlayer->GetUid());
    //成员添加到队列
    pBt->AddMember(pPlayer, nModelType);

    //大乱斗模式
    if (nModelType == _FIGHT_MODEL_TYPE_CONFUSION)
    {
        //设置队伍状态
        pBt->SetTeamStatus(_TEAM_STATUS_CREATE);

        //设置反向队列指针
        pPlayer->SetBct(pBt);

        //队伍信息添加到队列
        m_vtBcTeam.push_back(pBt);
    }
    //红蓝战队模式
    else if (nModelType == _FIGHT_MODEL_TYPE_RB_FIGHT_TEAM)
    {
        //初始化队伍数据
        pBt->SetFightTeamColour(_FIGHT_OWNER_COLOUR_NONE);
        pBt->SetFightTotalScore(0);
        //设置队伍状态
        pBt->SetTeamStatus(_TEAM_STATUS_CREATE);

        //设置反向队列指针
        pPlayer->SetBrbt(pBt);

        //添加到红蓝队列
        m_vtRbTeam.push_back(pBt);
    }
    //其他模式还未开放
    else
    {
        //...记录错误日志
        delete pBt;
        pBt = NULL;
        return 0;
    }

	return pBt->GetTeamId();
}

//添加到队伍列表
BOOL CBattle::AddTeamToBct(Player* pPlayer, INT nTeamid)
{
	CHECKF(pPlayer);

	for (auto iter = m_vtBcTeam.begin(); iter != m_vtBcTeam.end(); iter++)
    {	
    	//找到对应玩家
    	if((*iter)->GetTeamId() ==  (UINT)nTeamid)
    	{
    		INT nAmount = (*iter)->GetMemberAmount();
    		if (nAmount < (INT)BattleCfg()->GetBccMaxTeamAmount())
    		{
    			//成员添加到队列
    			(*iter)->AddMember(pPlayer, _FIGHT_MODEL_TYPE_CONFUSION);

    			//设置反向映射
                CBattleTeam* pBt = (*iter);
				pPlayer->SetBct(pBt);

    			return TRUE;
    		}    		
    	}
    }
    return FALSE;
}

//根据索引获取队伍指针
CBattleTeam* CBattle::GetTeamById(INT nModelType, INT nTeamid)
{
    //大乱斗模式
    if (nModelType == _FIGHT_MODEL_TYPE_CONFUSION)
    {
       for (auto iter = m_vtBcTeam.begin(); iter != m_vtBcTeam.end(); iter++)
        {   
            LOGDEBUG("[CBattle::GetTeamById] TeamId=[%d] MatchTeamId=[%d]",(*iter)->GetTeamId(), (UINT)nTeamid);
            //找到对应队伍
            if((*iter)->GetTeamId() ==  (UINT)nTeamid)
            {
                LOGDEBUG("[CBattle::GetTeamById] Is Find");
                return (*iter);
            }
        }
    }
    //红蓝战队模式 
    else //if(nModelType== _FIGHT_MODEL_TYPE_RB_FIGHT_TEAM)
    {
        for (auto iter = m_vtRbTeam.begin(); iter != m_vtRbTeam.end(); iter++)
        {   
            //找到对应队伍
            if((*iter)->GetTeamId() ==  (UINT)nTeamid)
            {
                return (*iter);
            }
        }
    }
	
    return NULL;
}

//匹配单人队列人员到组队列表
BOOL CBattle::MatchMemberToTeam(INT nModelType, INT nTeamid, UINT& nAmount)
{
    CBattleTeam* pBct = GetTeamById(nModelType, nTeamid);
    CHECKF(pBct);

    //大乱斗模式    
    if (nModelType == _FIGHT_MODEL_TYPE_CONFUSION)
    {
        //获取队伍长度
        nAmount = pBct->GetMemberAmount();
        if(nAmount < BattleCfg()->GetBccMaxTeamAmount())
        {
            for (auto iter = m_vtBcs.begin(); iter != m_vtBcs.end(); )
            {
                //从单人列表抓取玩家放入队伍队列
                if(*iter)
                {
                    //成员添加到队列
                    if(pBct->AddMember((*iter)->GetSignUpPlayer(), nModelType))
                    {
                        //设置反向映射,单人列表
                        (*iter)->GetSignUpPlayer()->ClearBcs();

                        //设置反向映射,队伍列表
                        (*iter)->GetSignUpPlayer()->SetBct(pBct);

                        //增加队伍人数
                        nAmount++;

                        //把玩家信息广播给队友
                        CMsgSynRoomInfo msg;
                        if(msg.CreateMsg((*iter)->GetSignUpPlayer(), _SYNROOMINFO_ACTION_ADD_MEMBER, 0, 0))
                        {
                            pBct->BroadcastTeamMsg(&msg);
                        }

                        //释放内存
                        SAFE_RELEASE(*iter);
                        
                        //从单人列表中删除对象，返回删除元素的下一个迭代器指针
                        iter = m_vtBcs.erase(iter);

                        //如果大于等于15则对战开始
                        if (nAmount >= BattleCfg()->GetBccMaxTeamAmount())
                        {
                            return TRUE;
                        } 

                        //继续添加下一个队友
                        continue;                          
                    }
                    else    //队伍添加失败
                    {
                        return FALSE;
                    }   
                } 
                iter++; 
            }   
        }
        else    //人数大于15人
        {
            return FALSE;
        }
    }
    else //if(nModelType== _FIGHT_MODEL_TYPE_RB_FIGHT_TEAM)//红蓝战队模式 
    {
        //更新队伍状态为：快速匹配状态，其他玩家可以申请进入队伍
        //pBct->SetTeamStatus(_TEAM_STATUS_QIUCK_MATCH);
    }
	
	return TRUE;
}

//红蓝战队开始匹配接口
BOOL CBattle::StartQuickMatch(INT nModelType, INT nTeamid)
{
    //红蓝战队模式 
    if(nModelType== _FIGHT_MODEL_TYPE_RB_FIGHT_TEAM)
    {
        CBattleTeam* pBct = GetTeamById(nModelType, nTeamid);
        CHECKF(pBct);
        //更新队伍状态为：快速匹配状态，其他玩家可以申请进入队伍
        pBct->SetTeamStatus(_TEAM_STATUS_QIUCK_MATCH);

        //开始计时
    }
    
    return TRUE;
}

//邀请好友加入特定队伍
BOOL CBattle::InviteMemberJoinTeam(INT nModelType, Player* pMember, INT nTeamid, UINT& nAmount)
{
    CHECKF(pMember);
    CBattleTeam* pBt = GetTeamById(nModelType, nTeamid);
    if(!pBt)
    {
        LOGERROR("[CBattle::InviteMemberJoinTeam] Team Is Not Eixst nModelType=[%d] TeamId=[%d]",nModelType, nTeamid);
        return FALSE;
    }

    //大乱斗模式    
    if (nModelType == _FIGHT_MODEL_TYPE_CONFUSION)
    {
        //获取队伍状态 : 在开始匹配阶段
        INT nStatus = pBt->GetTeamStatus();
        //判断队伍状态 ： _TEAM_STATUS_CREATE 和 _TEAM_STATUS_QIUCK_MATCH 才可以进入
        if (nStatus != _TEAM_STATUS_CREATE && nStatus != _TEAM_STATUS_QIUCK_MATCH)
        {
            LOGERROR("[CBattle::InviteMemberJoinTeam] ***_FIGHT_MODEL_TYPE_CONFUSION*** Team nStatus Is Not _TEAM_STATUS_CREATE OR _TEAM_STATUS_QIUCK_MATCH TeamId=[%d] nStatus=[%d]", nTeamid, nStatus);
            return FALSE;
        }

        //获取队伍长度 : < 15
        nAmount = pBt->GetMemberAmount();                
        if(nAmount < BattleCfg()->GetBccMaxTeamAmount())
        {
            //成员添加到队列
            if(pBt->AddMember(pMember, nModelType))
            {
                //设置反向映射
                pMember->SetBct(pBt);

                //成员数增加
                nAmount++;

                return TRUE;                        
            }        
        }
        else // 房间人员已经满了。(nAmount >= 15)
        {
            LOGERROR("[CBattle::InviteMemberJoinTeam] ***_FIGHT_MODEL_TYPE_CONFUSION*** Team nAmount >= _MAX_TEAMAMOUNT TeamId=[%d] nAmount=[%d]", nTeamid, nAmount);
            return FALSE;
        }
    }
    //红蓝战队模式 
    else if (nModelType == _FIGHT_MODEL_TYPE_RB_FIGHT_TEAM)
    {
        //获取队伍状态 : 在开始匹配阶段
        INT nStatus = pBt->GetTeamStatus();
        //判断队伍状态 ： _TEAM_STATUS_CREATE 和 _TEAM_STATUS_QIUCK_MATCH 才可以进入
        if (nStatus != _TEAM_STATUS_CREATE && nStatus != _TEAM_STATUS_QIUCK_MATCH)
        {
            LOGERROR("[CBattle::InviteMemberJoinTeam] ***_FIGHT_MODEL_TYPE_RB_FIGHT_TEAM*** Team nStatus Is Not _TEAM_STATUS_CREATE OR _TEAM_STATUS_QIUCK_MATCH TeamId=[%d] nStatus=[%d]", nTeamid, nStatus);
            return FALSE;
        }

        //获取队伍长度 : < 5
        nAmount = pBt->GetMemberAmount();   
        if(nAmount < BattleCfg()->GetBcftMaxRBTeamAmount())
        {
            //成员添加到队列
            if(pBt->AddMember(pMember, nModelType))
            {
                //设置反向映射
                pMember->SetBrbt(pBt);

                //成员数增加
                nAmount++;
     
                return TRUE;                        
            }  
        }
        else // 房间人员已经满了。(nAmount >= 15)
        {
            LOGERROR("[CBattle::InviteMemberJoinTeam] ***_FIGHT_MODEL_TYPE_RB_FIGHT_TEAM*** Team nAmount >= _MAX_TEAMAMOUNT TeamId=[%d] nAmount=[%d]", nTeamid, nAmount);
            return FALSE;
        }
    }
	return FALSE;
}

//根据队伍id获取队伍信息
CBattleTeam* CBattle::GetTeamInfoById(INT nTeamid)
{
	for (auto iter = m_vtBcTeam.begin(); iter != m_vtBcTeam.end(); iter++)
    {	
    	//找到对应玩家
    	if((*iter)->GetTeamId() ==  (UINT)nTeamid)
    	{
    		return (*iter);  		
    	}
    }
    return NULL;
}

//从队伍列表中删除
BOOL CBattle::DelTeamFromBct(CBattleTeam* pBt)
{
	CHECKF(pBt);

	for (auto iter = m_vtBcTeam.begin(); iter != m_vtBcTeam.end(); iter++)
    {	
    	//找到对应玩家
    	if((*iter) ==  pBt)
    	{
            //调用对象的析构函数,vector存指针的时候不会自动调用元素的析构函数
            SAFE_RELEASE(pBt);

    		m_vtBcTeam.erase(iter);
    		return TRUE;
    	}
    }
    return FALSE;
}

//从队伍列表中删除
BOOL CBattle::DelTeamFromBct(INT nTeamid)
{
	for (auto iter = m_vtBcTeam.begin(); iter != m_vtBcTeam.end(); iter++)
    {	
    	//找到对应玩家
    	if((*iter)->GetTeamId() ==  (UINT)nTeamid)
    	{
            //调用对象的析构函数,vector存指针的时候不会自动调用元素的析构函数
            SAFE_RELEASE(*iter);

    		m_vtBcTeam.erase(iter); 
    		return TRUE;		
    	}
    }
    return FALSE;
}

//从队伍列表中删除--红蓝战队
BOOL CBattle::DelTeamFromBrbt(CBattleTeam* pBt)
{
    CHECKF(pBt);

    for (auto iter = m_vtRbTeam.begin(); iter != m_vtRbTeam.end(); iter++)
    {   
        //找到对应玩家
        if((*iter) ==  pBt)
        {
            //LOGDEBUG("[CBattle::DelTeamFromBrbt] pBt.size=[%d]\n",  pBt->GetMemberAmount());

            //调用对象的析构函数,vector存指针的时候不会自动调用元素的析构函数
            SAFE_RELEASE(pBt);

            m_vtRbTeam.erase(iter);
            //LOGDEBUG("[CBattle::DelTeamFromBrbt] m_vtRbTeam.size=[%d]\n",  m_vtRbTeam.size());
            return TRUE;
        }
    }
    //LOGDEBUG("[CBattle::DelTeamFromBrbt] m_vtRbTeam.size=[%d]\n",  m_vtRbTeam.size());
    return FALSE;
}

//从队伍列表中删除--红蓝战队
BOOL CBattle::DelTeamFromBrbt(INT nTeamid)
{
    for (auto iter = m_vtRbTeam.begin(); iter != m_vtRbTeam.end(); iter++)
    {   
        //找到对应玩家
        if((*iter)->GetTeamId() ==  (UINT)nTeamid)
        {
            //调用对象的析构函数,vector存指针的时候不会自动调用元素的析构函数
            SAFE_RELEASE(*iter);
            
            m_vtRbTeam.erase(iter); 
            return TRUE;        
        }
    }
    return FALSE;
}

//获取大乱斗队伍队列的长度
UINT  CBattle::GetBcTeamAmount()
{
	return m_vtBcTeam.size();
}
//获取红蓝对战队伍队列的长度
UINT  CBattle::GetRbTeamAmount()
{
    return m_vtRbTeam.size();
}
//获取队列头部队伍
CBattleTeam* CBattle::GetFrontPrepareFightTeam(INT nModelType)
{
    //大乱斗模式
    if (nModelType == _FIGHT_MODEL_TYPE_CONFUSION)
    {
        for (auto iter = m_vtBcTeam.begin(); iter != m_vtBcTeam.end(); iter++)
        {   
            //人数未满，并且队伍状态为 ： 快速匹配阶段的，快速加入玩家才可以进入
            if ((*iter)->GetMemberAmount() < BattleCfg()->GetBccMaxTeamAmount() && (*iter)->GetTeamStatus() == _TEAM_STATUS_QIUCK_MATCH)
            {
                return (*iter);  
            }      
        }
    }
    //红蓝战队模式
    else if (nModelType == _FIGHT_MODEL_TYPE_RB_FIGHT_TEAM)
    {
        for (auto iter = m_vtRbTeam.begin(); iter != m_vtRbTeam.end(); iter++)
        {   
            //人数未满5人 && 处于快速匹配阶段
            if ((*iter)->GetMemberAmount() < BattleCfg()->GetBcftMaxRBTeamAmount() && (*iter)->GetTeamStatus() == _TEAM_STATUS_QIUCK_MATCH)
            {
                //LOGDEBUG("[CBattle::GetFrontPrepareFightTeam] nTeamid=[%d]\n",  (*iter)->GetTeamId());
                return (*iter);  
            }  
        }
    }
    return NULL;
}

//搜索满足空闲人数的队伍
CBattleTeam* CBattle::QuerySatisfyConditionTeam(INT nModelType, UINT nOwnerTeamId, UINT nAmount)
{
    if (nAmount <= 0)
    {
        LOGERROR("[CBattle::QuerySatisfyConditionTeam] nAmount <= 0");
        return NULL;
    }

    //大乱斗模式
    if (nModelType == _FIGHT_MODEL_TYPE_CONFUSION)
    {
        for (auto iter = m_vtBcTeam.begin(); iter != m_vtBcTeam.end(); iter++)
        {   
            //找到其他队伍，自己的队伍人数+其他队伍人数<=开战人数，其他队伍也为匹配状态
            if (nOwnerTeamId != (*iter)->GetTeamId() && (*iter)->GetMemberAmount() + nAmount <= BattleCfg()->GetBccMaxTeamAmount() && (*iter)->GetTeamStatus() == _TEAM_STATUS_QIUCK_MATCH)
            {
                return (*iter);  
            }      
        }
    }
    //红蓝战队模式
    else if (nModelType == _FIGHT_MODEL_TYPE_RB_FIGHT_TEAM)
    {
        return NULL;
    }
    return NULL;
}

//整合A-B队伍
BOOL CBattle::MergeTeamS(INT nModelType, CBattleTeam* pOwnerbt, CBattleTeam* pOtherbt)
{
    CHECKF(pOwnerbt);
    CHECKF(pOtherbt);

    //大乱斗模式
    if (nModelType == _FIGHT_MODEL_TYPE_CONFUSION)
    {
        //把pOtherbt队伍成员广播给pOtherbt队伍成员
        pOwnerbt->MergeOtherTeam(nModelType, pOtherbt);

        //销毁pOtherbt队伍
        DelTeamFromBct(pOtherbt);

        return TRUE;
    }
    //红蓝战队模式
    else if (nModelType == _FIGHT_MODEL_TYPE_RB_FIGHT_TEAM)
    {
        return FALSE;
    }
    return FALSE;

}

//开始匹配enemy队伍，匹配完了之后再去设置队伍所属方
CBattleTeam* CBattle::MathcEnemyFightTeam(CBattleTeam* pbt)
{
    CHECKF(pbt);

    for (auto iter = m_vtRbTeam.begin(); iter != m_vtRbTeam.end(); iter++)
    {   
        //人数满5人 && 处于快速匹配阶段 && 另外一方的队伍
        if ((*iter)->GetMemberAmount() >= BattleCfg()->GetBcftMaxRBTeamAmount() && (*iter)->GetTeamStatus() == _TEAM_STATUS_QIUCK_MATCH && 
            /*(*iter)->GetFightTeamColour() != pbt->GetFightTeamColour() &&*/ (*iter) != pbt)
        {
            //设置红蓝队伍标识
            pbt->SetFightTeamColour(_FIGHT_OWNER_COLOUR_RED);
            (*iter)->SetFightTeamColour(_FIGHT_OWNER_COLOUR_BLUE);

            return (*iter);  
        }  
    }
    return NULL;
}