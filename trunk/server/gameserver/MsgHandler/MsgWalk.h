//////////////////////////////////////////////////////////////////////
//文件名称：CMsgWalk.h
//功能描述：
//版本说明：CMsgWalk API
//			
//编写作者：2018.10.15 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#ifndef __MSG_WALK_H__
#define __MSG_WALK_H__

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
#include "MsgBattle.h"

#include "../Protocol/Command.h"
#include "../Protocol/cJSON.h"


//////////////////////////////////////////////////// CMsgWalk ////////////////////////////////////////////////////
//类型
enum _WALK_ACTION
{
	_WALK_ACTION_USER_CMD	 				= 1,		//用户操作
	_WALK_ACTION_CONTINUE_PREVIOUS_CMD		= 2,		//空桢继续上一个操作命令，服务器66ms = 前端33ms * 2
	_WALK_ACTION_COLLIDE_CMD				= 3,		//战斗中碰撞-[大乱斗]-[红蓝战队]
	_WALK_ACTION_CONTINUE_COLLIDE_CMD		= 4,		//继续战斗中碰撞-[大乱斗]-[红蓝战队]
	_WALK_ACTION_CONFUSION_RELIVE			= 5,		//继续战斗复活-[大乱斗]
	_WALK_ACTION_TEAM_RELIVE				= 6,		//继续战斗复活-[红蓝战队]
	_WALK_ACTION_CONFUSION_CAR_LEV_CHANGE	= 7,		//玩家车辆等级改变-[大乱斗]
	_WALK_ACTION_TEAM_CAR_LEV_CHANGE		= 8,		//玩家车辆等级改变-[红蓝战队]
	_WALK_ACTION_KILL						= 9,		//击杀-[大乱斗]-[红蓝战队]
};

//////////////////////////////////////////////////// CMsgWalk ////////////////////////////////////////////////////
//走路包
class CMsgWalk : public CNetMsg
{
public:
	CMsgWalk();
	virtual  ~CMsgWalk();
	virtual BOOL		Read(SocketInputStream& iStream, UINT nLen);
	virtual BOOL		Write(SocketOutputStream& oStream) const;
	virtual	PacketID_t	GetPacketID() const { return _MSG_WALK; };	
	virtual	UINT		GetPacketSize() const { return m_nDataLen; };
	virtual	BOOL 		CreateMsg(UINT nRetStatus, UINT nAction);
	virtual	BOOL 		CreateMsg(INT nAction=_WALK_ACTION_USER_CMD, INT nTargetId=0, INT nPointX=0, INT nPointY=0, INT nPointStartX=0, INT nPointStartY=0,INT nPointEndX=0, INT nPointEndY=0, long long llTimeStamp=0);
	virtual BOOL 		CreateFrameMsg(INT nAction=_WALK_ACTION_USER_CMD, INT nTargetId=0, INT nPointX=0, INT nPointY=0, INT nPointStartX=0, INT nPointStartY=0,INT nPointEndX=0, INT nPointEndY=0, INT nIndex=0, 
										INT nBeCollideId=0, INT nTType=0, INT nTPowerX=0, INT nTPowerY=0, INT nTimeStamp=0);
	//!alter by huyf 2018.11.30:迁移到battleteam类中
	//virtual VOID 		InsertFrameMsgCache(INT nModel=_BATTLE_MODEL_CONFUSION, INT nAction=_WALK_ACTION_USER_CMD, INT nTargetId=0, INT nPointX=0, INT nPointY=0, INT nPointStartX=0, INT nPointStartY=0,
	//										INT nPointEndX=0, INT nPointEndY=0,INT nBeCollideId=0, INT nTType=0, INT nTPowerX=0, INT nTPowerY=0);
	//!alter end:迁移到battleteam类中
	virtual	BOOL 		SendMsg(CNetMsg* pMsg);
	virtual UINT 		Process(VOID *pInfo);
	 	
};

class CMsgWalkFactory : public PacketFactory
{
public:
	CNetMsg* 	CreatePacket() { return new CMsgWalk; };
	PacketID_t 	GetPacketID() const { return _MSG_WALK; };
	UINT 		GetPacketMaxSize() const { return m_nPacketSize; };	
	VOID 		SetPacketSize(UINT size) { m_nPacketSize = size; };

private:
	UINT m_nPacketSize;	
};


#endif // __MSG_LOGIN_HANDLER_H__