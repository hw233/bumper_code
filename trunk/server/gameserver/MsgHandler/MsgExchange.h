//////////////////////////////////////////////////////////////////////
//文件名称：CMsgExchange.h
//功能描述：
//版本说明：CMsgExchange API
//			
//编写作者：2018.10.15 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#ifndef __MSG_EXCHANG_H__
#define __MSG_EXCHANG_H__

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


//////////////////////////////////////////////////// CMsgExchange ////////////////////////////////////////////////////
//兑奖包
class CMsgExchange : public CNetMsg
{
public:
	CMsgExchange();
	virtual  ~CMsgExchange();
	virtual BOOL		Read(SocketInputStream& iStream, UINT nLen);
	virtual BOOL		Write(SocketOutputStream& oStream) const;
	virtual	PacketID_t	GetPacketID() const { return _MSG_EXCHANGE; };	
	virtual	UINT		GetPacketSize() const { return m_nDataLen; };
	virtual	BOOL 		CreateMsg(UINT nRetStatus, UINT nAction);
	virtual	BOOL 		SendMsg(CNetMsg* pMsg);
	virtual UINT 		Process(VOID *pInfo);
	 	
};

class CMsgExchangeFactory : public PacketFactory
{
public:
	CNetMsg* 	CreatePacket() { return new CMsgExchange; };
	PacketID_t 	GetPacketID() const { return _MSG_EXCHANGE; };
	UINT 		GetPacketMaxSize() const { return m_nPacketSize; };	
	VOID 		SetPacketSize(UINT size) { m_nPacketSize = size; };

private:
	UINT m_nPacketSize;	
};


#endif // __MSG_LOGIN_HANDLER_H__