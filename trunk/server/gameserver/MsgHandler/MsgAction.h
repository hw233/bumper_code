//////////////////////////////////////////////////////////////////////
//文件名称：CMsgAction.h
//功能描述：
//版本说明：CMsgAction API
//			
//编写作者：2018.10.14 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#ifndef __MSG_ACTION_H__
#define __MSG_ACTION_H__

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


//////////////////////////////////////////////////// CMsgAction ////////////////////////////////////////////////////
//action请求
enum _ACTION
{
	_ACTION_GO_BACK_HOME_SCREEN					= 1,		//大乱斗战斗结束后返回游戏主界面
	_ACTION_GO_BACK_HOME_SCREEN_SUC				= 2,		//大乱斗战斗结束后返回游戏主界面成功
	_ACTION_GO_BACK_HOME_SCREEN_FAIL			= 3,		//大乱斗战斗结束后返回游戏主界面失败
	_ACTION_OPEN_BOX							= 4,		//开启宝箱
	_ACTION_OPEN_BOX_SUC						= 5,		//开启宝箱成功
	_ACTION_OPEN_BOX_FAIL						= 6,		//开启宝箱失败
	_ACTION_LIMIT_TIME_AWARD					= 7, 		//限时领奖
	_ACTION_LIMIT_TIME_AWARD_SUC				= 8,		//限时领奖成功
	_ACTION_LIMIT_TIME_AWARD_FAIL				= 9,		//限时领奖失败
	_ACTION_ONLINE_AWARD						= 10, 		//在线领奖
	_ACTION_ONLINE_AWARD_SUC					= 11,		//在线领奖成功
	_ACTION_ONLINE_AWARD_FAIL					= 12,		//在线领奖失败
	_ACTION_ONLINE_QUICK						= 13, 		//在线加速
	_ACTION_ONLINE_QUICK_SUC					= 14,		//在线加速成功
	_ACTION_ONLINE_QUICK_FAIL					= 15,		//在线加速失败
	_ACTION_LIMIT_TIME_CONTINUE_TRY				= 16, 		//限时领奖-皮肤使用-继续使用
	_ACTION_LIMIT_TIME_CONTINUE_TRY_SUC			= 17,		//限时领奖-皮肤使用-继续使用成功
	_ACTION_LIMIT_TIME_CONTINUE_TRY_FAIL		= 18,		//限时领奖-皮肤使用-继续使用失败
	//!alter by huyf 2019.01.09:服务器自己计算，不用前端上报
	// _ACTION_THREE_KILL							= 19, 		//完成一次三杀 														C----------------->S
	// _ACTION_THREE_KILL_SUC						= 20,		//完成一次三杀成功(暂不发送，需要时在打开屏蔽)						S----------------->C
	// _ACTION_THREE_KILL_FAIL						= 21,		//完成一次三杀失败 													S----------------->C
	// _ACTION_THREE_EXERCISE_WIN					= 22, 		//赢得排位赛胜利 													C----------------->S
	// _ACTION_THREE_EXERCISE_WIN_SUC				= 23,		//赢得排位赛胜利成功(暂不发送，需要时在打开屏蔽)					S----------------->C
	// _ACTION_THREE_EXERCISE_WIN_FAIL				= 24,		//赢得排位赛胜利失败 												S----------------->C
	//!alter end:服务器自己计算，不用前端上报
	_ACTION_ADD_APP_LIST						= 25, 		//添加到小程序														C----------------->S
	_ACTION_ADD_APP_LIST_SUC					= 26,		//添加到小程序成功													S----------------->C
	_ACTION_ADD_APP_LIST_FAIL					= 27,		//添加到小程序失败 													S----------------->C
};

class CMsgAction : public CNetMsg
{
public:
	CMsgAction();
	virtual  ~CMsgAction();
	virtual BOOL		Read(SocketInputStream& iStream, UINT nLen);
	virtual BOOL		Write(SocketOutputStream& oStream) const;
	virtual	PacketID_t	GetPacketID() const { return _MSG_ACTION; };	
	virtual	UINT		GetPacketSize() const { return m_nDataLen; };
	//创建消息
	virtual	BOOL 		CreateMsg(UINT nAction, UINT nTargetId=0, UINT nType=0, UINT nData1=0, UINT nData2=0, UINT nParam1=0, UINT nParam2=0);
	virtual	BOOL 		SendMsg(CNetMsg* pMsg);
	virtual UINT 		Process(VOID *pInfo);
	 	
};

class CMsgActionFactory : public PacketFactory
{
public:
	CNetMsg* 	CreatePacket() { return new CMsgAction; };
	PacketID_t 	GetPacketID() const { return _MSG_ACTION; };
	UINT 		GetPacketMaxSize() const { return m_nPacketSize; };	
	VOID 		SetPacketSize(UINT size) { m_nPacketSize = size; };

private:
	UINT m_nPacketSize;	
};


#endif // __MSG_LOGIN_HANDLER_H__