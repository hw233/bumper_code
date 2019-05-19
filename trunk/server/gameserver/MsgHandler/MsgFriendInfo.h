//////////////////////////////////////////////////////////////////////
//文件名称：CMsgFriendInfo.h
//功能描述：
//版本说明：CMsgFriendInfo API
//			
//编写作者：2018.11.22 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#ifndef __MSG_FRIEND_INFO_H__
#define __MSG_FRIEND_INFO_H__

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


//////////////////////////////////////////////////// CMsgFriendInfo ////////////////////////////////////////////////////
//action请求
enum _FRIEND_INFO_ACTION
{
	_FRIEND_INFO_ADD					= 1,		//添加好友
	_FRIEND_INFO_DEL					= 2,		//删除请求
	_FRIEND_INFO_UPDATE					= 3,		//更新好友数据
	_FRIEND_INFO_ONLINE					= 4,		//在线
	_FRIEND_INFO_OFFLINE				= 5,		//离线
	_FRIEND_INFO_GETLIST				= 6,		//好友列表信息
};

enum _FRIEND_INFO_ATTRIB
{
	_FRIEND_INFO_ATTRIB_GAMESTATUS		= 1,		//游戏状态 0：下线 1：上线 2：正在游戏中
};




class CMsgFriendInfo : public CNetMsg
{
public:
	CMsgFriendInfo();
	virtual  ~CMsgFriendInfo();
	virtual BOOL		Read(SocketInputStream& iStream, UINT nLen);
	virtual BOOL		Write(SocketOutputStream& oStream) const;
	virtual	PacketID_t	GetPacketID() const { return _MSG_FRIEND_INFO; };	
	virtual	UINT		GetPacketSize() const { return m_nDataLen; };
	virtual	BOOL 		CreateMsg(UINT nRetStatus, UINT nAction);
	//创建消息
	virtual BOOL 		CreateMsg(UINT nAction);
	virtual	BOOL 		CreateMsg(UINT nAction, Player* pPlayer, UINT nStatus, UINT nData=0);
	virtual	BOOL 		CreateMsg(UINT nAction, CFriendData* pFriendData, UINT nStatus, UINT nIndex);
	//追加消息
	virtual BOOL 		AppendMsg(UINT nAction, CFriendData* pFriendData, UINT nStatus, UINT nIndex);
	virtual	BOOL 		SendMsg(CNetMsg* pMsg);
	virtual UINT 		Process(VOID *pInfo);
	 	
};

class CMsgFriendInfoFactory : public PacketFactory
{
public:
	CNetMsg* 	CreatePacket() { return new CMsgFriendInfo; };
	PacketID_t 	GetPacketID() const { return _MSG_FRIEND_INFO; };
	UINT 		GetPacketMaxSize() const { return m_nPacketSize; };	
	VOID 		SetPacketSize(UINT size) { m_nPacketSize = size; };

private:
	UINT m_nPacketSize;	
};


#endif // __MSG_FRIEND_H__