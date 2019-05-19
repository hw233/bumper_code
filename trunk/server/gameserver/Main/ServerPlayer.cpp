//////////////////////////////////////////////////////////////////////
//文件名称：ServerPlayer.cpp
//功能描述：服务器玩家指的是另外的服务器端程序在当前程序内的数据表现
//版本说明：ServerPlayer API
//			
//编写作者：2017.08.21 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include "ServerPlayer.h"

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
ServerPlayer::ServerPlayer()
{
        m_Status = 0;
        m_pServerData = NULL;
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
ServerPlayer::~ServerPlayer()
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
BOOL ServerPlayer::Init()
{
        SetDisconnect(FALSE);
        ResetKick();
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
BOOL ServerPlayer::ProcessInput()
{	
	return Player::ProcessInput();
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
BOOL ServerPlayer::ProcessOutput()
{
        //printf("------------ServerPlayer::ProcessOutput-----------\n");
        return Player::ProcessOutput();
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
BOOL ServerPlayer::ProcessCommand(bool Option /*= true*/)
{
        //printf("------------ServerPlayer::ProcessCommands-----------\n");
        return Player::ProcessCommand(Option);
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
VOID ServerPlayer::CleanUp()
{
        Player::CleanUp();
        m_pServerData = NULL;
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
BOOL ServerPlayer::HeartBeat(DWORD dwTime/*=0*/)
{
        return Player::HeartBeat(dwTime);
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	向此服务器端程序发送消息
//						此接口不支持线程间同步，因此只能有当前线程来调用执行
//
//-----input------
// Parameters
//		pMsg 		:	包
//------output------
// Return 
//		无
BOOL ServerPlayer::SendPacket(CNetMsg* pMsg)
{
        return Player::SendPacket(pMsg);
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
BOOL ServerPlayer::IsValid()
{
        return Player::IsValid();
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
VOID ServerPlayer::Disconnect()
{
        return Player::Disconnect();
}
