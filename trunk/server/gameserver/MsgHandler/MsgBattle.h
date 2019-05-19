//////////////////////////////////////////////////////////////////////
//文件名称：CMsgBattle.h
//功能描述：
//版本说明：CMsgBattle API
//			
//编写作者：2018.10.13 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#ifndef __MSG_BATTLE_H__
#define __MSG_BATTLE_H__

#include <sstream>

#include "../Common/BaseType.h"
#include "../Common/Common.h"
#include "../Main/PacketFactory.h"
#include "../MsgHandler/NetMsg.h"
#include "../NetBase/SocketInputStream.h"
#include "../NetBase/SocketOutputStream.h"
#include "../Main/ClientManager.h"
#include "../Main/PlayerPool.h"
#include "../Main/ServerPlayer.h"
#include "../Main/Player.h"

#include "../Protocol/Command.h"
#include "../Protocol/cJSON.h"


//////////////////////////////////////////////////// CMsgBattle ////////////////////////////////////////////////////

//战斗包请求
enum //_BATTLE_ACTION
{
	_BATTLE_ACTION_CREATE_ROOM 			= 1,		//创建房间-[大乱斗]-[红蓝战队]
	_BATTLE_ACTION_CREATE_ROOM_SUC 		= 2,		//创建房间-[大乱斗]-[红蓝战队]
	_BATTLE_ACTION_CREATE_ROOM_FAIL 	= 3,		//创建房间-[大乱斗]-[红蓝战队]

	_BATTLE_ACTION_QUICK_MATCH 			= 4,		//快速匹配-[大乱斗]-[红蓝战队]
	_BATTLE_ACTION_QUICK_MATCH_SUC 		= 5,		//快速匹配-[大乱斗]-[红蓝战队]
	_BATTLE_ACTION_QUICK_MATCH_FAIL 	= 6,		//快速匹配-[大乱斗]-[红蓝战队]

	_BATTLE_ACTION_QUICK_JOIN	 		= 7,		//快速加入-[大乱斗]-[红蓝战队]
	_BATTLE_ACTION_QUICK_JOIN_SUC	 	= 8,		//快速加入-[大乱斗]-[红蓝战队]
	_BATTLE_ACTION_QUICK_JOIN_FAIL	 	= 9,		//快速加入-[大乱斗]-[红蓝战队]

	_BATTLE_ACTION_INVITE_JOIN	 		= 10,		//邀请好友加入-[大乱斗]-[红蓝战队]
	_BATTLE_ACTION_INVITE_JOIN_SUC	 	= 11,		//邀请好友加入-[大乱斗]-[红蓝战队]
	_BATTLE_ACTION_INVITE_JOIN_FAIL	 	= 12,		//邀请好友加入-[大乱斗]-[红蓝战队]

	_BATTLE_ACTION_BATTLE_START			= 13,		//战斗开始-[大乱斗]
	_BATTLE_ACTION_BATTLE_START_SUC		= 14,		//战斗开始-[大乱斗]
	_BATTLE_ACTION_BATTLE_START_FAIL	= 15,		//战斗开始-[大乱斗]

	_BATTLE_ACTION_BATTLE_COLLIDE 		= 16,		//战斗中碰撞-[大乱斗]-[红蓝战队]
	_BATTLE_ACTION_BATTLE_COLLIDE_SUC 	= 17,		//战斗中碰撞-[大乱斗]-[红蓝战队]
	_BATTLE_ACTION_BATTLE_COLLIDE_FAIL 	= 18,		//战斗中碰撞-[大乱斗]-[红蓝战队]

	_BATTLE_ACTION_BATTLE_KILL 			= 19,		//战斗中击杀-[大乱斗]-[红蓝战队]
	_BATTLE_ACTION_BATTLE_KILL_SUC 		= 20,		//战斗中击杀-[大乱斗]-[红蓝战队]
	_BATTLE_ACTION_BATTLE_KILL_FAIL 	= 21,		//战斗中击杀-[大乱斗]-[红蓝战队]

	_BATTLE_ACTION_BATTLE_SYN_SCORE		= 22,		//战斗中同步积分-[大乱斗]
	_BATTLE_ACTION_BATTLE_SYN_SCORE_SUC	= 23,		//战斗中同步积分-[大乱斗]
	_BATTLE_ACTION_BATTLE_SYN_SCORE_FAIL= 24,		//战斗中同步积分-[大乱斗]

	_BATTLE_ACTION_BATTLE_RELIVE		= 25, 		//战斗中复活-[大乱斗]-----已经迁移到msgwalk包，现已经无用
	_BATTLE_ACTION_BATTLE_RELIVE_SUC	= 26, 		//战斗中复活-[大乱斗]
	_BATTLE_ACTION_BATTLE_RELIVE_FAIL	= 27, 		//战斗中复活-[大乱斗]

	_BATTLE_ACTION_BATTLE_END			= 28,		//战斗结束-[大乱斗]
	_BATTLE_ACTION_BATTLE_END_SUC		= 29,		//战斗结束-[大乱斗]
	_BATTLE_ACTION_BATTLE_END_FAIL		= 30,		//战斗结束-[大乱斗]

	_BATTLE_ACTION_VIEW_AWARD			= 31,		//战斗结束界面 看视频三倍奖励-[大乱斗]
	_BATTLE_ACTION_VIEW_AWARD_SUC		= 32,		//战斗结束界面 看视频三倍奖励-[大乱斗]
	_BATTLE_ACTION_VIEW_AWARD_FAIL		= 33,		//战斗结束界面 看视频三倍奖励-[大乱斗]

	_BATTLE_ACTION_TEAM_START			= 34,		//红蓝对战中战斗开始-[红蓝战队]
	_BATTLE_ACTION_TEAM_START_SUC		= 35,		//红蓝对战中战斗开始-[红蓝战队]
	_BATTLE_ACTION_TEAM_START_FAIL		= 36,		//红蓝对战中战斗开始-[红蓝战队]

	_BATTLE_ACTION_TEAM_END				= 37,		//红蓝对战中战斗结束-[红蓝战队]
	_BATTLE_ACTION_TEAM_END_SUC			= 38,		//红蓝对战中战斗结束-[红蓝战队]
	_BATTLE_ACTION_TEAM_END_FAIL		= 39,		//红蓝对战中战斗结束-[红蓝战队]

	_BATTLE_ACTION_TEAM_SYN_SCORE		= 40,		//红蓝对战中同步队伍积分、击杀方车辆等级-[红蓝战队]
	_BATTLE_ACTION_TEAM_SYN_SCORE_SUC	= 41,		//红蓝对战中同步队伍积分、击杀方车辆等级-[红蓝战队]
	_BATTLE_ACTION_TEAM_SYN_SCORE_FAIL	= 42,		//红蓝对战中同步队伍积分、击杀方车辆等级-[红蓝战队]

	_BATTLE_ACTION_LEAVE_TEAM			= 43,		//离开队伍-[大乱斗]-[红蓝战队]
	_BATTLE_ACTION_LEAVE_TEAM_SUC		= 44,		//离开队伍-[大乱斗]-[红蓝战队]
	_BATTLE_ACTION_LEAVE_TEAM_FAIL		= 45,		//离开队伍-[大乱斗]-[红蓝战队]
	
	_BATTLE_ACTION_END_AWARD			= 46,		//战斗结束同步前端霸主令牌和金币奖励-[大乱斗]
	_BATTLE_ACTION_END_AWARD_SUC		= 47,		//战斗结束同步前端霸主令牌和金币奖励-[大乱斗]
	_BATTLE_ACTION_END_AWARD_FAIL		= 48,		//战斗结束同步前端霸主令牌和金币奖励-[大乱斗]

	_BATTLE_ACTION_TEAM_RELIVE			= 49, 		//战斗中复活-[红蓝战队]------已经迁移到msgwalk包，现已经无用
	_BATTLE_ACTION_TEAM_RELIVE_SUC		= 50, 		//战斗中复活-[红蓝战队]
	_BATTLE_ACTION_TEAM_RELIVE_FAIL		= 51, 		//战斗中复活-[红蓝战队]

    _BATTLE_ACTION_LOADING_COMPLETE      = 52,       //加载完成-[大乱斗]
    _BATTLE_ACTION_LOADING_COMPLETE_SUC  = 53,       //加载完成成功-[大乱斗]
    _BATTLE_ACTION_LOADING_COMPLETE_FAIL = 54,       //加载完成失败-[大乱斗]

    _BATTLE_ACTION_TEAM_LOADING_COMPLETE        = 55,       //加载完成-[红蓝战队]
    _BATTLE_ACTION_TEAM_LOADING_COMPLETE_SUC    = 56,       //加载完成成功-[红蓝战队]
    _BATTLE_ACTION_TEAM_LOADING_COMPLETE_FAIL   = 57,       //加载完成失败-[红蓝战队]

    _BATTLE_ACTION_TEAM_VIEW_AWARD				= 58,		//战斗结束界面 看视频三倍奖励-[红蓝对战]
    _BATTLE_ACTION_TEAM_VIEW_AWARD_SUC			= 59,		//战斗结束界面 看视频三倍奖励成功-[红蓝对战]
    _BATTLE_ACTION_TEAM_VIEW_AWARD_FAIL			= 60,		//战斗结束界面 看视频三倍奖励失败-[红蓝对战]

    _BATTLE_ACTION_CHANGE_TEAM_LEADER			= 61,		//队长变更-[大乱斗]-[红蓝对战]
    _BATTLE_ACTION_CHANGE_TEAM_LEADER_SUC		= 62,		//队长变更-[大乱斗]-[红蓝对战]
    _BATTLE_ACTION_CHANGE_TEAM_LEADER_FAIL		= 63,		//队长变更-[大乱斗]-[红蓝对战]

    _BATTLE_ACTION_INVITE_FRIEND_JOIN			= 64,		//邀请好友加入队伍-[大乱斗]-[红蓝对战]
    _BATTLE_ACTION_INVITE_FRIEND_JOIN_SUC		= 65,		//邀请好友加入队伍-[大乱斗]-[红蓝对战]
    _BATTLE_ACTION_INVITE_FRIEND_JOIN_FAIL		= 66,		//邀请好友加入队伍-[大乱斗]-[红蓝对战]

    _BATTLE_ACTION_RECENT_PLAYER				= 67,		//最近玩家-[大乱斗]
    _BATTLE_ACTION_RECENT_PLAYER_SUC			= 68,		//最近玩家-[大乱斗]
    _BATTLE_ACTION_RECENT_PLAYER_FAIL			= 69,		//最近玩家-[大乱斗]

    _BATTLE_ACTION_CHANGE_TEAM_ID				= 70,		//修改队伍ID-[大乱斗]-[红蓝对战]
    
};

enum //_BATTLE_RESULT
{
	_BATTLE_RESULT_FAIL = 0,						//失败
	_BATTLE_RESULT_SUCCESS = 1,						//成功
};

class CMsgBattle : public CNetMsg
{
public:
	CMsgBattle();
	virtual  ~CMsgBattle();
	virtual BOOL		Read(SocketInputStream& iStream, UINT nLen);
	virtual BOOL		Write(SocketOutputStream& oStream) const;
	virtual	PacketID_t	GetPacketID() const { return _MSG_BATTLE; };	
	virtual	UINT		GetPacketSize() const { return m_nDataLen; };
//	virtual	BOOL 		CreateMsg(UINT nRetStatus, UINT nAction);
	virtual	BOOL 		CreateMsg(UINT nAction=0, UINT nBattleModel=0, UINT nTeamId=0, UINT nTargetId=0, INT nPower=0, 
							INT nPointX=0, INT nPointY=0, INT nKillSum=0, INT nFightScore=1000, INT nFightLevel=1, 
							INT nKingID=0, INT nRegionId=0, string strNickName="", string strHeadPicurl="");
	//nPower  nPointX  nPointY 服务器与前端 扩大100倍 方便处理
	virtual BOOL 		AppendMsg( UINT nAction=0, UINT nBattleModel=0, UINT nTeamId=0, UINT nTargetId=0, INT nPower=0, 
							INT nPointX=0, INT nPointY=0, INT nKillSum=0, INT nFightScore=1000, INT nFightLevel=1, 
							INT nKingID=0, INT nRegionId=1, INT nIndex=0, string strNickName="", string strHeadPicurl="");
	virtual	BOOL 		SendMsg(CNetMsg* pMsg);
	virtual UINT 		Process(VOID *pInfo);
	 	
};

class CMsgBattleFactory : public PacketFactory
{
public:
	CNetMsg* 	CreatePacket() { return new CMsgBattle; };
	PacketID_t 	GetPacketID() const { return _MSG_BATTLE; };
	UINT 		GetPacketMaxSize() const { return m_nPacketSize; };	
	VOID 		SetPacketSize(UINT size) { m_nPacketSize = size; };

private:
	UINT m_nPacketSize;	
};


#endif // __MSG_LOGIN_HANDLER_H__
