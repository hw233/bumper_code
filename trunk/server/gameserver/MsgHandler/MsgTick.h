//////////////////////////////////////////////////////////////////////
//文件名称：CMsgTick.h
//功能描述：
//版本说明：CMsgTick API
//			
//编写作者：2019.01.18 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0 心跳包-[ping\]
//
//////////////////////////////////////////////////////////////////////


#ifndef __MSG_TICK_H__
#define __MSG_TICK_H__
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

///////////////////////////////////////////////////////////

//action请求
enum _TICK_ACTION
{
	_TICK_ACTION_PING = 1,						//ping 		C<-------->S
};

//////////////////////////////////////////////////// CMsgTick ////////////////////////////////////////////////////
//商场包
class CMsgTick : public CNetMsg
{
public:
	CMsgTick();
	virtual  ~CMsgTick();
	virtual BOOL		Read(SocketInputStream& iStream, UINT nLen);
	virtual BOOL		Write(SocketOutputStream& oStream) const;
	virtual	PacketID_t	GetPacketID() const { return _MSG_TICK; };	
	virtual	UINT		GetPacketSize() const { return m_nDataLen; };
	virtual BOOL 		CreateMsg(UINT nAction, UINT nRetStatus);
	virtual	BOOL 		SendMsg(CNetMsg* pMsg);
	virtual UINT 		Process(VOID *pInfo);
	 	
};

class CMsgTickFactory : public PacketFactory
{
public:
	CNetMsg* 	CreatePacket() { return new CMsgTick; };
	PacketID_t 	GetPacketID() const { return _MSG_TICK; };
	UINT 		GetPacketMaxSize() const { return m_nPacketSize; };	
	VOID 		SetPacketSize(UINT size) { m_nPacketSize = size; };

private:
	UINT m_nPacketSize;	
};


#endif // __MSG_TICK_H__