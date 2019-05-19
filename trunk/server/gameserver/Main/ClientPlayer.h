//////////////////////////////////////////////////////////////////////
//文件名称：ClientPlayer.h
//功能描述：前端网络对接接口（基类）
//版本说明：socket API
//			
//编写作者：2017.09.05 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#ifndef __CLIENTPLAYER_H__
#define __CLIENTPLAYER_H__

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include "../Common/Type.h"
#include "../Common/Config.h"

#include "Player.h"

//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////

//用于连接某些服务器的前端连接

/*
* 内部server_1|------>内部server_2
*             |------>内部server_3
*			  |***	
*/

class ClientPlayer : public Player
{
public:
	ClientPlayer();
	virtual ~ClientPlayer();
	BOOL			Init();

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
	virtual BOOL HeartBeat(DWORD dwTime = 0);
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
	virtual BOOL IsServerPlayer()		{	return FALSE;	}
	virtual BOOL IsClientPlayer()		{	return TRUE;	}				
	virtual BOOL IsGamePlayer()			{	return FALSE;	}
	

	//预留后期使用...
public:
	_CLIENT_DATA* GetServerData()					{	return m_pServerData;	}
	VOID SetServerData(_CLIENT_DATA* pServerData)	{	assert(pServerData);	m_pServerData = pServerData;	}

	//设置当前服务器端玩家的状态
	void			SetPlayerStatus(UINT status)	{ m_Status = status; }

private :
	//服务器端状态
	UINT			m_Status;
	//指针的内容是在g_Config中的数据
	_CLIENT_DATA*	m_pServerData;


};

#endif