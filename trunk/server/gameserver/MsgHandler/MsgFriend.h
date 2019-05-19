//////////////////////////////////////////////////////////////////////
//文件名称：CMsgFriend.h
//功能描述：
//版本说明：CMsgFriend API
//			
//编写作者：2018.11.21 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#ifndef __MSG_FRIEND_H__
#define __MSG_FRIEND_H__

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


//////////////////////////////////////////////////// CMsgFriend ////////////////////////////////////////////////////
//action请求
enum _FRIEND_ACTION
{
	_FRIEND_APPLY					= 1,		//申请好友
	_FRIEND_ACCEPT					= 2,		//接受请求
	_FRIEND_REFUSE					= 3,		//拒绝请求
	_FRIEND_GETLIST					= 4,		//获取好友信息
};

class CMsgFriend : public CNetMsg
{
public:
	CMsgFriend();
	virtual  ~CMsgFriend();
	virtual BOOL		Read(SocketInputStream& iStream, UINT nLen);
	virtual BOOL		Write(SocketOutputStream& oStream) const;
	virtual	PacketID_t	GetPacketID() const { return _MSG_FRIEND; };	
	virtual	UINT		GetPacketSize() const { return m_nDataLen; };
	virtual	BOOL 		CreateMsg(UINT nRetStatus, UINT nAction);
	//创建消息
	virtual	BOOL 		CreateMsg(UINT nAction=_FRIEND_APPLY, UINT nUid=0, const string& strNickName="default", const string& strHeadPicurl="default");
	virtual	BOOL 		SendMsg(CNetMsg* pMsg);
	virtual UINT 		Process(VOID *pInfo);
	 	
};

class CMsgFriendFactory : public PacketFactory
{
public:
	CNetMsg* 	CreatePacket() { return new CMsgFriend; };
	PacketID_t 	GetPacketID() const { return _MSG_FRIEND; };
	UINT 		GetPacketMaxSize() const { return m_nPacketSize; };	
	VOID 		SetPacketSize(UINT size) { m_nPacketSize = size; };

private:
	UINT m_nPacketSize;	
};


#endif // __MSG_FRIEND_H__