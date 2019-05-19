//////////////////////////////////////////////////////////////////////
//文件名称：CMsgSign.h
//功能描述：
//版本说明：CMsgSign API
//			
//编写作者：2018.12.24 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#ifndef _MSG_SIGNUP_H__
#define _MSG_SIGNUP_H__

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
//#include "../Main/Player.h"

#include "../Protocol/Command.h"
#include "../Protocol/cJSON.h"


//////////////////////////////////////////////////// CMsgSingUp ////////////////////////////////////////////////////
//类型
enum _ACT_SIGNUP_
{
	_ACT_SIGNUP_CMD 		= 1,			//签到、补签请求 ****通过里面附带dayindex来标识是否为签到或者补签，签到时不带此参数，补签的时候需要到此参数
	_ACT_SIGNUP_CMD_SUC 	= 2,			//签到、补签成功
	_ACT_SIGNUP_CMD_FAIL 	= 3,			//签到、补签失败
};

class CMsgSingUp : public CNetMsg
{
public:
	CMsgSingUp();
	virtual  ~CMsgSingUp();
	virtual BOOL		Read(SocketInputStream& iStream, UINT nLen);
	virtual BOOL		Write(SocketOutputStream& oStream) const;
	virtual	PacketID_t	GetPacketID() const { return _MSG_SIGNUP; };	
	virtual	UINT		GetPacketSize() const { return m_nDataLen; };	
	//创建消息
	virtual	BOOL 		CreateMsg(UINT nAction, UINT nType=0, UINT nData=0);
	virtual	BOOL 		SendMsg(CNetMsg* pMsg);
	virtual UINT 		Process(VOID *pInfo);
	 	
};

class CMsgSingUpFactory : public PacketFactory
{
public:
	CNetMsg* 	CreatePacket() { return new CMsgSingUp; };
	PacketID_t 	GetPacketID() const { return _MSG_SIGNUP; };
	UINT 		GetPacketMaxSize() const { return m_nPacketSize; };	
	VOID 		SetPacketSize(UINT size) { m_nPacketSize = size; };

private:
	UINT m_nPacketSize;	
};


#endif // _MSG_SIGNUP_H__