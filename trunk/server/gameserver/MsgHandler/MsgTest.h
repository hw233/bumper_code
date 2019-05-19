//////////////////////////////////////////////////////////////////////
//文件名称：CMsgTest.h
//功能描述：
//版本说明：CMsgTest API
//			
//编写作者：2019.01.10 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////


#ifndef __MSG_TEST_H__
#define __MSG_TEST_H__


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
enum _TEST_ACTION
{

};

//////////////////////////////////////////////////// CMsgTest ////////////////////////////////////////////////////
//商场包
class CMsgTest : public CNetMsg
{
public:
	CMsgTest();
	virtual  ~CMsgTest();
	virtual BOOL		Read(SocketInputStream& iStream, UINT nLen);
	virtual BOOL		Write(SocketOutputStream& oStream) const;
	virtual	PacketID_t	GetPacketID() const { return _MSG_TEST; };	
	virtual	UINT		GetPacketSize() const { return m_nDataLen; };
	virtual BOOL        CreateMsg(UINT nAction, Player* pPlayer=NULL, CTaskDetailData* pData=NULL, UINT nType=0);
	virtual	BOOL 		SendMsg(CNetMsg* pMsg);
	virtual UINT 		Process(VOID *pInfo);
	 	
};

class CMsgTestFactory : public PacketFactory
{
public:
	CNetMsg* 	CreatePacket() { return new CMsgTest; };
	PacketID_t 	GetPacketID() const { return _MSG_TEST; };
	UINT 		GetPacketMaxSize() const { return m_nPacketSize; };	
	VOID 		SetPacketSize(UINT size) { m_nPacketSize = size; };

private:
	UINT m_nPacketSize;	
};


#endif // __MSG_TEST_H__