//////////////////////////////////////////////////////////////////////
//文件名称：ClientPlayer.cpp
//功能描述：服务器内部网络连接
//版本说明：ClientPlayer API
//			
//编写作者：2017.09.05 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include "ClientPlayer.h"

#include "../Protocol/cJSON.h"
#include "../Protocol/Command.h"

//////////////////////////////////////////////////////////////////////
//
// func description	:	constructor
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
ClientPlayer::ClientPlayer()
{
	DEBUG_TRY	
    m_Status = 0;
    m_pServerData = NULL;
	DEBUG_CATCH("ClientPlayer::ClientPlayer")
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	destructor
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
ClientPlayer::~ClientPlayer()
{
	
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	Init
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ClientPlayer::Init()
{	
	DEBUG_TRY	
    SetDisconnect(FALSE);
    ResetKick();  
	DEBUG_CATCHF("ClientPlayer::Init")
	return TRUE;
	
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	数据接收
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ClientPlayer::ProcessInput()
{
	DEBUG_TRY	
	return Player::ProcessInput();
	DEBUG_CATCHF("ClientPlayer::ProcessInput")
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	数据发送
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ClientPlayer::ProcessOutput()
{
	DEBUG_TRY	
	return Player::ProcessOutput();
	DEBUG_CATCHF("ClientPlayer::ProcessOutput")
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	消息处理
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ClientPlayer::ProcessCommand(bool Option /*= true*/)
{
	DEBUG_TRY	
	return Player::ProcessCommand(Option);
	DEBUG_CATCHF("ClientPlayer::ProcessCommand")
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	清除
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
VOID ClientPlayer::CleanUp()
{
	DEBUG_TRY
	Player::CleanUp();
	m_pServerData = NULL;
	DEBUG_CATCH("ClientPlayer::CleanUp")	
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	逻辑接口：心跳
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		BOOL: TRUE \ FALSE
BOOL ClientPlayer::HeartBeat(DWORD dwTime)
{
	DEBUG_TRY
	return TRUE;
	DEBUG_CATCHF("ClientPlayer::HeartBeat")
	
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	向此服务器端程序发送消息
//						此接口不支持线程间同步，因此只能有当前线程来调用执行
//
//-----input------
// Parameters
//		pPacket 		:	包
//------output------
// Return 
//		无
BOOL ClientPlayer::SendPacket(CNetMsg* pMsg)
{
	DEBUG_TRY	
	return Player::SendPacket(pMsg);
	DEBUG_CATCHF("ClientPlayer::SendPacket")
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	判断当前服务器端玩家是否有效
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ClientPlayer::IsValid()
{
	DEBUG_TRY	
	return Player::IsValid();
	DEBUG_CATCHF("ClientPlayer::IsValid")
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	判断当前服务器端玩家是否有效
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
VOID ClientPlayer::Disconnect()
{
	DEBUG_TRY	
	return Player::Disconnect();
	DEBUG_CATCH("ClientPlayer::Disconnect")
}
