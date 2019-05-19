//////////////////////////////////////////////////////////////////////
//文件名称：CMsgUserAttrib.h
//功能描述：
//版本说明：CMsgUserAttrib API
//			
//编写作者：2018.12.25 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0 用户属性包
//
//////////////////////////////////////////////////////////////////////
#ifndef __MSG_USERATTRIB_H__
#define __MSG_USERATTRIB_H__

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
//类型
enum _ACT_USERATTRIB_
{
	_ACT_USERATTRIB_HEAD_PICURL				=1, 		//【预留不做更新】
	_ACT_USERATTRIB_NICK_NAME				=2,			//【预留不做更新】
	_ACT_USERATTRIB_GOLD_SUM				=3,
	_ACT_USERATTRIB_KILL_SUM				=4,
	_ACT_USERATTRIB_GRANDING_ID				=5,
	_ACT_USERATTRIB_CUR_STAR_SUM			=6,
	_ACT_USERATTRIB_UNLOCK_CARS				=7,
	_ACT_USERATTRIB_USE_CAR					=8,
	_ACT_USERATTRIB_KING_SUM				=9,
	_ACT_USERATTRIB_GIFT_DAY				=10,
	_ACT_USERATTRIB_GIFT_SUM				=11,
	_ACT_USERATTRIB_STAR_SUM				=12,
	_ACT_USERATTRIB_FIGHT_KILL_SUM			=13,
	_ACT_USERATTRIB_FIGHT_SCORE				=14,
	_ACT_USERATTRIB_ADD_SCORE				=15,
	_ACT_USERATTRIB_ADD_GOLD				=16,
	_ACT_USERATTRIB_RATIO					=17,
	_ACT_USERATTRIB_ENEMY_ID				=18,
	_ACT_USERATTRIB_GET_GIFT				=19,
	_ACT_USERATTRIB_KEEP_STAR				=20,

	_ACT_USERATTRIB_DAYTASKMASK_FIR			=21,			//天任务掩码位1							//前端没有使用，前端使用的status字段控制
	_ACT_USERATTRIB_CURTMWDAY				=22, 			//当前星期[0-6]:星期天-星期六 			

	_ACT_USERATTRIB_SHOW_IDX				=23,			//展示索引[预留后期扩展前期不适用]
	_ACT_USERATTRIB_SHOW_ITEM_TYPE			=24,			//限时皮肤试用奖励道具的ItemType
	_ACT_USERATTRIB_SHOW_REMAIN_Ts			=25,			//展示的道具的剩余时间戳(秒)

	_ACT_USERATTRIB_ONLINE_IDX				=26,			//在线领奖索引[预留后期扩展前期不适用]
	_ACT_USERATTRIB_ONLINE_ITEM_TYPE		=27,			//在线领奖索引展示的道具itemtype
	_ACT_USERATTRIB_ONLINE_REMAIN_Ts		=28,			//在线领奖索引展示的道具的剩余时间戳(秒)

	_ACT_USERATTRIB_TASKMASK				=29,			//终生任务掩码位
	_ACT_USERATTRIB_WEEKTASKMASK_FIR 		=30,			//每周任务掩码位

};

//////////////////////////////////////////////////// CMsgUserAttrib ////////////////////////////////////////////////////
//
class CMsgUserAttrib : public CNetMsg
{
public:
	CMsgUserAttrib();
	virtual  ~CMsgUserAttrib();
	virtual BOOL		Read(SocketInputStream& iStream, UINT nLen);
	virtual BOOL		Write(SocketOutputStream& oStream) const;
	virtual	PacketID_t	GetPacketID() const { return _MSG_USER_ATTRIB; };	
	virtual	UINT		GetPacketSize() const { return m_nDataLen; };
	//virtual BOOL 		CreateMsg(UINT nRetStatus, UINT nAction);
	virtual BOOL 		CreateMsg(UINT nAction, UINT nAttribData);
	//创建消息
	virtual	BOOL 		SendMsg(CNetMsg* pMsg);
	virtual UINT 		Process(VOID *pInfo);
	 	
};

class CMsgUserAttribFactory : public PacketFactory
{
public:
	CNetMsg* 	CreatePacket() { return new CMsgUserAttrib; };
	PacketID_t 	GetPacketID() const { return _MSG_USER_ATTRIB; };
	UINT 		GetPacketMaxSize() const { return m_nPacketSize; };	
	VOID 		SetPacketSize(UINT size) { m_nPacketSize = size; };

private:
	UINT m_nPacketSize;	
};


#endif // __MSG_USERATTRIB_H__