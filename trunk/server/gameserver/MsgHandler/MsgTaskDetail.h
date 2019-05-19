//////////////////////////////////////////////////////////////////////
//文件名称：CMsgTaskDetail.h
//功能描述：
//版本说明：CMsgTaskDetail API
//			
//编写作者：2018.10.15 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////


#ifndef __MSG_TASK_DETAIL_H__
#define __MSG_TASK_DETAIL_H__


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

#include "../Main/TaskDetail/TaskDetailData.h"

///////////////////////////////////////////////////////////

//action请求
enum _TASK_DETAIL_ACTION
{
	_TASK_DETAIL_LIST						= 1,		//获取任务列表                     C--------------->S
	_TASK_DETAIL_ADD						= 2,		//添加任务 		                   S--------------->C
	_TASK_DETAIL_LIST_SUC					= 3,		//获取任务列表成功 	 		       S--------------->C
	_TASK_DETAIL_LIST_FAIL					= 4,		//获取任务列表失败 	 		       S--------------->C
	_TASK_DETAIL_UPDATE						= 5,		//更新任务 		                   S--------------->C
	_TASK_DETAIL_AWARD						= 6,		//领取奖励                   	   C--------------->S
	_TASK_DETAIL_AWARD_SUC					= 7,		//领取奖励成功 		               S--------------->C
	_TASK_DETAIL_AWARD_FAIL					= 8,		//领取奖励失败 		               S--------------->C
	_TASK_DETAIL_TRY_GAME					= 9,		//试玩任务上报                     C--------------->S
	_TASK_DETAIL_TRY_GAME_FAIL				= 10,		//试玩任务上报失败 		           S--------------->C
	_TASK_DETAIL_DEL						= 11,		//删除任务 		                   S--------------->C

};

//////////////////////////////////////////////////// CMsgTaskDetail ////////////////////////////////////////////////////
//商场包
class CMsgTaskDetail : public CNetMsg
{
public:
	CMsgTaskDetail();
	virtual  ~CMsgTaskDetail();
	virtual BOOL		Read(SocketInputStream& iStream, UINT nLen);
	virtual BOOL		Write(SocketOutputStream& oStream) const;
	virtual	PacketID_t	GetPacketID() const { return _MSG_TASKDETAIL; };	
	virtual	UINT		GetPacketSize() const { return m_nDataLen; };
	virtual BOOL        CreateMsg(UINT nAction, Player* pPlayer=NULL, CTaskDetailData* pData=NULL, UINT nType=0);
	virtual	BOOL 		SendMsg(CNetMsg* pMsg);
	virtual UINT 		Process(VOID *pInfo);
	 	
};

class CMsgTaskDetailFactory : public PacketFactory
{
public:
	CNetMsg* 	CreatePacket() { return new CMsgTaskDetail; };
	PacketID_t 	GetPacketID() const { return _MSG_TASKDETAIL; };
	UINT 		GetPacketMaxSize() const { return m_nPacketSize; };	
	VOID 		SetPacketSize(UINT size) { m_nPacketSize = size; };

private:
	UINT m_nPacketSize;	
};


#endif // __MSG_TASK_DETAIL_H__