//////////////////////////////////////////////////////////////////////
//文件名称：CBattle.h
//功能描述：
//版本说明：CBattle API
//			
//编写作者：2018.10.10 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#ifndef __BATTLE_H__
#define __BATTLE_H__

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
#include "BattleConfusionSingle.h"
#include "BattleTeam.h"
#include <vector>
using namespace std;
#include "Player.h"

typedef std::vector< CBattleConfusionSingle* > VT_BCS;
typedef std::vector< CBattleTeam* > VT_BC_TEAM;			//大乱斗队伍列表
typedef std::vector< CBattleTeam* > VT_RB_TEAM;						//红蓝战队队伍列表
//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
//class：大乱斗模式
//////////////////////////////////////////////////////////////////////
class CBattle
{
public:
	CBattle();
	virtual ~CBattle();

public:
	//获取大乱斗对象指针
	VT_BCS& GetBcsSet() 	{	return m_vtBcs;	}
	VT_BC_TEAM& GetBctSet() 	{	return m_vtBcTeam;	}
	VT_BC_TEAM& GetBrbtSet() 	{	return m_vtRbTeam;	}

	//LOOP 接口
	BOOL Loop();

	//单人
	//添加到单人列表--大乱斗-单人队列
	BOOL 			AddBcs(Player* pPlayer);
	//从单人列表中删除--大乱斗-单人队列
	BOOL 			DelBcs(Player* pPlayer);
	//获取单人列表的长度
	UINT 			GetBcsAmount();
	//根据索引获取队伍成员
	Player* 		GetPlayerByIndex(UINT nIndex);

	//创建队伍列表
	UINT 			CreateTeam(Player* pPlayer, INT nModelType);
	//根据索引获取队伍指针
	CBattleTeam* 	GetTeamById(INT nModelType, INT nTeamid);
	//添加到队伍列表
	BOOL 			AddTeamToBct(Player* pPlayer, INT nTeamid);
	//根据队伍id获取队伍信息
	CBattleTeam* 	GetTeamInfoById(INT nTeamid);
	//从队伍列表中删除--大乱斗--队伍队列
	BOOL 			DelTeamFromBct(INT nTeamid);
	//从队伍列表中删除--大乱斗-队伍队列
	BOOL 			DelTeamFromBct(CBattleTeam* pBt);
	//从队伍列表中删除--红蓝战队
	BOOL 			DelTeamFromBrbt(INT nTeamid);
	//从队伍列表中删除--红蓝战队
	BOOL 			DelTeamFromBrbt(CBattleTeam* pBt);
	//获取红蓝对战队伍队列的长度
	UINT  			GetRbTeamAmount();
	//获取大乱斗队伍队列的长度
	UINT  			GetBcTeamAmount();
	//红蓝战队开始匹配接口
	BOOL 			StartQuickMatch(INT nModelType, INT nTeamid);
	//获取队列头部队伍
	CBattleTeam* 	GetFrontPrepareFightTeam(INT nModelType);
	//开始匹配enemy队伍
	CBattleTeam* 	MathcEnemyFightTeam(CBattleTeam* pbt);
	//邀请好友加入特定队伍
	BOOL 			InviteMemberJoinTeam(INT nModelType, Player* pMember, INT nTeamid, UINT& nAmount);
	//匹配单人队列人员到组队列表
	BOOL 			MatchMemberToTeam(INT nModelType, INT nTeamid, UINT& nAmount);
	//搜索满足空闲人数的队伍
	CBattleTeam* 	QuerySatisfyConditionTeam(INT nModelType, UINT nOwnerTeamId, UINT nAmount);
	//整合A-B队伍
	BOOL 			MergeTeamS(INT nModelType, CBattleTeam* pOwnerbt, CBattleTeam* pOtherbt);


private:
	///////////////////////////////////////////////////////////////
	///大乱斗模式
	VT_BCS m_vtBcs;		//单人列表
	VT_BC_TEAM m_vtBcTeam;		//组队列表

	///////////////////////////////////////////////////////////////
	///红蓝战队模式
	VT_RB_TEAM m_vtRbTeam;		//组队列表

};
#endif //__BATTLEMANAGER_H__