//////////////////////////////////////////////////////////////////////
//文件名称：CMsgItem.h
//功能描述：
//版本说明：CMsgItem API
//			
//编写作者：2018.10.14 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#ifndef _MSG_ITEM_H__
#define _MSG_ITEM_H__

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


//////////////////////////////////////////////////// CMsgItem ////////////////////////////////////////////////////
//类型
enum _ACT_ITEM
{
	_ACT_ITEM_CREATE_RANDOM_ITEM_POS 			= 1,		//获取随机点 S-C C-S
	_ACT_ITEM_CREATE_RANDOM_ITEM				= 2,		//生成随机道具	S->C
	_ACT_ITEM_PICKUP_RANDOM_ITEM				= 3,		//删除道具	C->S S-C
	_ACT_ITEM_ADD								= 4,		// 添加道具
	_ACT_ITEM_DEL								= 5,		// 删除道具 1、C------>S(前端发送限时道具不再想试用时由前端发送请求到后端) 
															//			2、S------->C 服务器返回删除道具标识，返回信息中包含了需要清理的道具唯一id（包括服务器主动下发删除道具情况）
	_ACT_ITEM_UPDATE							= 6,		// 更新道具(全量)
	_ACT_ITEM_USE								= 7,		// 使用某个道具
	_ACT_ITEM_DEL_FAIL							= 8,		// 删除道具失败（当前端请求，数据不对的时候服务器返回前端错误提示信息）
};

class CMsgItem : public CNetMsg
{
public:
	CMsgItem();
	virtual  ~CMsgItem();
	virtual BOOL		Read(SocketInputStream& iStream, UINT nLen);
	virtual BOOL		Write(SocketOutputStream& oStream) const;
	virtual	PacketID_t	GetPacketID() const { return _MSG_ITEM; };	
	virtual	UINT		GetPacketSize() const { return m_nDataLen; };	
	//创建消息
	virtual	BOOL 		CreateMsg(UINT nAction, UINT nItemId=0, UINT nItemType=0, UINT nSorce=0, INT nPointX=0, INT nPointY=0, INT nTargetId=0, INT nTag=0);
	//创建消息
	virtual BOOL 		CreateMsg(UINT nAction, CItemData* pItem);
	virtual	BOOL 		SendMsg(CNetMsg* pMsg);
	virtual UINT 		Process(VOID *pInfo);
	 	
};

class CMsgItemFactory : public PacketFactory
{
public:
	CNetMsg* 	CreatePacket() { return new CMsgItem; };
	PacketID_t 	GetPacketID() const { return _MSG_ITEM; };
	UINT 		GetPacketMaxSize() const { return m_nPacketSize; };	
	VOID 		SetPacketSize(UINT size) { m_nPacketSize = size; };

private:
	UINT m_nPacketSize;	
};


#endif // _MSG_ITEM_H__