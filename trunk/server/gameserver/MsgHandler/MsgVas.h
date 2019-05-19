//////////////////////////////////////////////////////////////////////
//文件名称：CMsgVas.h
//功能描述：
//版本说明：CMsgVas API
//			
//编写作者：2018.10.15 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#ifndef __MSG_VAS_H__
#define __MSG_VAS_H__

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
///子消息类型
enum 
{
	_VAS_BUY			= 1,			//买道具-------战斗BUFF
	_VAS_BUY_SUC		= 2,			//买道具成功---战斗BUFF
	_VAS_BUY_FAIL		= 3,			//买道具失败---战斗BUFF
	_VAS_SHOP_BUY		= 4,			//商城买道具-------商城
	_VAS_SHOP_BUY_SUC	= 5,			//商城买道具成功---商城
	_VAS_SHOP_BUY_FAIL	= 6,			//商城买道具失败---商城
};

///////////////////////////////////////////////////////////
///购买类型
enum 
{
	_VAS_BUY_TYPE_GOLD	= 1,			//金币买道具
	_VAS_BUY_TYPE_VIWE	= 2,			//视频买道具
};

//////////////////////////////////////////////////// CMsgVas ////////////////////////////////////////////////////
//商场包
class CMsgVas : public CNetMsg
{
public:
	CMsgVas();
	virtual  ~CMsgVas();
	virtual BOOL		Read(SocketInputStream& iStream, UINT nLen);
	virtual BOOL		Write(SocketOutputStream& oStream) const;
	virtual	PacketID_t	GetPacketID() const { return _MSG_VAS; };	
	virtual	UINT		GetPacketSize() const { return m_nDataLen; };
	virtual	BOOL 		CreateMsg(INT nAction/*=0*/, INT nModelType=0, INT nItemId=0, INT nItemType=0, INT nItemCount=0);
	virtual	BOOL 		SendMsg(CNetMsg* pMsg);
	virtual UINT 		Process(VOID *pInfo);
	 	
};

class CMsgVasFactory : public PacketFactory
{
public:
	CNetMsg* 	CreatePacket() { return new CMsgVas; };
	PacketID_t 	GetPacketID() const { return _MSG_VAS; };
	UINT 		GetPacketMaxSize() const { return m_nPacketSize; };	
	VOID 		SetPacketSize(UINT size) { m_nPacketSize = size; };

private:
	UINT m_nPacketSize;	
};


#endif // __MSG_LOGIN_HANDLER_H__