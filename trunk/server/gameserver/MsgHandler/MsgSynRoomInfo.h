//////////////////////////////////////////////////////////////////////
//文件名称：CMsgSynRoomInfo.h
//功能描述：
//版本说明：CMsgSynRoomInfo API
//			
//编写作者：2018.10.10 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#ifndef __MSG_SYNROOMINFO_H__
#define __MSG_SYNROOMINFO_H__

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
#include "MsgBattle.h"



//////////////////////////////////////////////////// CMsgSynRoomInfo ////////////////////////////////////////////////////
#if 0
///战斗类型
enum //_BATTLE_MODEL_TYPE
{	
	_BATTLE_MODEL_NONE				= 0,		//xxx模式
	_BATTLE_MODEL_CONFUSION				= 1,		//大乱斗模式
	_BATTLE_MODEL_RB_FIGHT_TEAM				= 2,		//红蓝队对战模式
};
#endif
///战斗之前房间信息同步，战斗数据同步使用CMsgBattle同步
enum _SYNROOMINFO_ACTION
{
	_SYNROOMINFO_ACTION_ADD_MEMBER 		= 1,						//添加成员
	_SYNROOMINFO_ACTION_DEL_MEMBER 		= 2,						//删除成员
};

//同步请求结果
enum _SYNROOMINFO_RESULT
{
	_SYNROOMINFO_RESULT_FAIL 		= 0,						//失败
	_SYNROOMINFO_RESULT_SUCCESS 	= 1,						//成功
};

class CMsgSynRoomInfo : public CNetMsg
{
public:
	CMsgSynRoomInfo();
	virtual  ~CMsgSynRoomInfo();
	virtual BOOL		Read(SocketInputStream& iStream, UINT nLen);
	virtual BOOL		Write(SocketOutputStream& oStream) const;
	virtual	PacketID_t	GetPacketID() const { return _MSG_SYN_ROOM; };	
	virtual	UINT		GetPacketSize() const { return m_nDataLen; };
	virtual	BOOL 		CreateMsg(UINT nRetStatus, UINT nAction);
	virtual	BOOL 		CreateMsg(Player* player, UINT nAction=0, INT nTeamId=0, INT nTeamColour=0);
	virtual	BOOL 		SendMsg(CNetMsg* pMsg);
	virtual UINT 		Process(VOID *pInfo);
	 	
};

class CMsgSynRoomInfoFactory : public PacketFactory
{
public:
	CNetMsg* 	CreatePacket() { return new CMsgSynRoomInfo; };
	PacketID_t 	GetPacketID() const { return _MSG_SYN_ROOM; };
	UINT 		GetPacketMaxSize() const { return m_nPacketSize; };	
	VOID 		SetPacketSize(UINT size) { m_nPacketSize = size; };

private:
	UINT m_nPacketSize;	
};


#endif // __MSG_LOGIN_HANDLER_H__