//////////////////////////////////////////////////////////////////////
//文件名称：CMsgItemAttrib.h
//功能描述：
//版本说明：CMsgItemAttrib API
//			
//编写作者：2018.10.25 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0 道具属性包
//
//////////////////////////////////////////////////////////////////////
#ifndef _MSG_ITEMATTRIB_H__
#define _MSG_ITEMATTRIB_H__

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

//////////////////////////////////////////////////// CMsgItemAttrib ////////////////////////////////////////////////////
///
enum _ITEMATTRIB 
{
	_ITEMATTRIB_ID 								= 0,					// 道具唯一ID(数据库自增量)
	_ITEMATTRIB_USER_ID 						= 1,					// 用户ID
	_ITEMATTRIB_ITEMTYPE 						= 2,					// 道具类型typeID
	_ITEMATTRIB_TYPE 							= 3,					// 道具类型type
	_ITEMATTRIB_LIFE_TIME 						= 4,					// 试用时间（剩余）
	_ITEMATTRIB_AMOUNT							= 5,					// 叠加数量(当前)
};


typedef struct 
{
	DWORD	ucAttributeType;
	DWORD	dwAttributeData;
}ST_ITEMATTRIB;

class CMsgItemAttrib : public CNetMsg
{
public:
	CMsgItemAttrib();
	virtual  ~CMsgItemAttrib();
	virtual BOOL		Read(SocketInputStream& iStream, UINT nLen);
	virtual BOOL		Write(SocketOutputStream& oStream) const;
	virtual	PacketID_t	GetPacketID() const { return _MSG_ITEM_ATTRIB; };	
	virtual	UINT		GetPacketSize() const { return m_nDataLen; };

    //创建消息
	virtual	BOOL 		CreateMsg(UINT nAction, UINT nItemId, UINT nAttributeData);   
	virtual	BOOL 		SendMsg(CNetMsg* pMsg);
	virtual UINT 		Process(VOID *pInfo);
	 	
};

class CMsgItemAttribFactory : public PacketFactory
{
public:
	CNetMsg* 	CreatePacket() { return new CMsgItemAttrib; };
	PacketID_t 	GetPacketID() const { return _MSG_ITEM_ATTRIB; };
	UINT 		GetPacketMaxSize() const { return m_nPacketSize; };	
	VOID 		SetPacketSize(UINT size) { m_nPacketSize = size; };

private:
	UINT m_nPacketSize;	
};


#endif // _MSG_ITEMATTRIB_H__