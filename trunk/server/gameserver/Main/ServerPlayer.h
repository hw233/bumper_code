//////////////////////////////////////////////////////////////////////
//文件名称：ServerPlayer.h
//功能描述：服务器玩家指的是另外的服务器端程序在当前程序内的数据表现
//版本说明：ServerPlayer API
//			
//编写作者：2017.08.21 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#ifndef __SERVERPLAYER_H__
#define __SERVERPLAYER_H__

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include "Type.h"
#include "Player.h"
#include "Config.h"


//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//class
//////////////////////////////////////////////////////////////////////
//服务器玩家指的是另外的服务器端程序在当前程序内的数据表现
class ServerPlayer : public Player
{
public :
	ServerPlayer();
	virtual ~ServerPlayer();
	BOOL			Init();
	//VOID			FreeOwn();


public:
	//数据接收
	virtual BOOL ProcessInput();
	//数据发送
	virtual BOOL ProcessOutput();
	//消息处理
	virtual BOOL ProcessCommand(bool Option = true);
	//清除
	virtual VOID CleanUp();
	//逻辑接口：心跳
	virtual BOOL HeartBeat(DWORD dwTime=0);
	//kick time修正
	//virtual VOID ResetKick();
	//向此服务器端程序发送消息
	//此接口不支持线程间同步，因此只能有当前线程来调用执行
	virtual BOOL SendPacket(CNetMsg* pMsg);
	//判断当前服务器端玩家是否有效
	virtual BOOL IsValid();
	//玩家断开连接
	virtual VOID Disconnect();
	
	//通用接口
	virtual BOOL IsServerPlayer()		{	return TRUE;	}	
	virtual BOOL IsClientPlayer()		{	return FALSE;	}			
	virtual BOOL IsGamePlayer()			{	return FALSE;	}

	//预留后期使用...
public:
	_SERVER_DATA* GetServerData()					{	return m_pServerData;	}
	VOID SetServerData(_SERVER_DATA* pServerData)	{	assert(pServerData);	m_pServerData = pServerData;	}

	//设置当前服务器端玩家的状态
	void			SetPlayerStatus(UINT status)	{ m_Status = status; }

private :
	//服务器端状态
	UINT			m_Status;
	//指针的内容是在g_Config中的数据
	_SERVER_DATA*	m_pServerData;
};



#endif