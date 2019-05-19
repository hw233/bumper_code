//////////////////////////////////////////////////////////////////////
//文件名称：CMsgTopList.h
//功能描述：
//版本说明：CMsgTopList API
//			
//编写作者：2018.11.23 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#ifndef __MSG_TOP_LIST_H__
#define __MSG_TOP_LIST_H__

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


//////////////////////////////////////////////////// CMsgTopList ////////////////////////////////////////////////////
//action请求
enum _TOPLIST_ACTION
{
	_TOPLIST_BY_GOLD					= 0,		//金币排行榜数据
	_TOPLIST_BY_SCORE					= 1,		//最高分排行榜数据

};

class CMsgTopList : public CNetMsg
{
public:
	CMsgTopList();
	virtual  ~CMsgTopList();
	virtual BOOL		Read(SocketInputStream& iStream, UINT nLen);
	virtual BOOL		Write(SocketOutputStream& oStream) const;
	virtual	PacketID_t	GetPacketID() const { return _MSG_TOPLIST; };	
	virtual	UINT		GetPacketSize() const { return m_nDataLen; };
	virtual	BOOL 		CreateMsg(UINT nRetStatus, UINT nAction);
	
	//创建消息
	virtual	BOOL 		CreateMsg(UINT nAction, Player* pPlayer, UINT nStatus);
	virtual	BOOL 		CreateMsg(UINT nAction, CFriendData* pFriendData, UINT nStatus, UINT nIndex);
	//追加消息
	virtual BOOL 		AppendMsg(UINT nAction, CFriendData* pFriendData, UINT nStatus, UINT nIndex);
	virtual	BOOL 		SendMsg(CNetMsg* pMsg);
	virtual UINT 		Process(VOID *pInfo);
	 	
};

class CMsgTopListFactory : public PacketFactory
{
public:
	CNetMsg* 	CreatePacket() { return new CMsgTopList; };
	PacketID_t 	GetPacketID() const { return _MSG_TOPLIST; };
	UINT 		GetPacketMaxSize() const { return m_nPacketSize; };	
	VOID 		SetPacketSize(UINT size) { m_nPacketSize = size; };

private:
	UINT m_nPacketSize;	
};


#endif // __MSG_TOP_LIST_H__