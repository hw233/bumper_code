//////////////////////////////////////////////////////////////////////
//文件名称：CMsgUserInfo.h
//功能描述：
//版本说明：CMsgUserInfo API
//			
//编写作者：2018.10.13 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#ifndef __MSG_USERINFO_H__
#define __MSG_USERINFO_H__

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


//////////////////////////////////////////////////// CMsgUserInfo ////////////////////////////////////////////////////
//
class CMsgUserInfo : public CNetMsg
{
public:
	CMsgUserInfo();
	virtual  ~CMsgUserInfo();
	virtual BOOL		Read(SocketInputStream& iStream, UINT nLen);
	virtual BOOL		Write(SocketOutputStream& oStream) const;
	virtual	PacketID_t	GetPacketID() const { return _MSG_USERINFO; };	
	virtual	UINT		GetPacketSize() const { return m_nDataLen; };
	virtual	BOOL 		CreateMsg(UINT nRet);
	//创建消息
	BOOL 				CreateMsg(Player* player);
	virtual	BOOL 		SendMsg(CNetMsg* pMsg);
	virtual UINT 		Process(VOID *pInfo);
	 	
};

class CMsgUserInfoFactory : public PacketFactory
{
public:
	CNetMsg* 	CreatePacket() { return new CMsgUserInfo; };
	PacketID_t 	GetPacketID() const { return _MSG_USERINFO; };
	UINT 		GetPacketMaxSize() const { return m_nPacketSize; };	
	VOID 		SetPacketSize(UINT size) { m_nPacketSize = size; };

private:
	UINT m_nPacketSize;	
};


#endif // __MSG_USERINFO_H__