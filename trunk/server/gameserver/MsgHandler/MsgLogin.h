//////////////////////////////////////////////////////////////////////
//文件名称：CMsgLogin.h
//功能描述：
//版本说明：CMsgLogin API
//			
//编写作者：2018.10.10 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#ifndef __MSG_LOGIN_H__
#define __MSG_LOGIN_H__

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


//////////////////////////////////////////////////// CMsgLogin ////////////////////////////////////////////////////
//
enum LOGIN_RESULT
{
	FAIL = 0,				//成功
	SUCCESS = 1,			//失败
};

class CMsgLogin : public CNetMsg
{
public:
	CMsgLogin();
	virtual  ~CMsgLogin();
	virtual BOOL		Read(SocketInputStream& iStream, UINT nLen);
	virtual BOOL		Write(SocketOutputStream& oStream) const;
	virtual	PacketID_t	GetPacketID() const { return _MSG_LOGIN; };	
	virtual	UINT		GetPacketSize() const { return m_nDataLen; };
	virtual	BOOL 		CreateMsg(UINT nRetStatus, UINT nAction);
	virtual	BOOL 		SendMsg(CNetMsg* pMsg);
	virtual UINT 		Process(VOID *pInfo);
	 	
};

class CMsgLoginFactory : public PacketFactory
{
public:
	CNetMsg* 	CreatePacket() { return new CMsgLogin; };
	PacketID_t 	GetPacketID() const { return _MSG_LOGIN; };
	UINT 		GetPacketMaxSize() const { return m_nPacketSize; };	
	VOID 		SetPacketSize(UINT size) { m_nPacketSize = size; };

private:
	UINT m_nPacketSize;	
};


#endif // __MSG_LOGIN_HANDLER_H__