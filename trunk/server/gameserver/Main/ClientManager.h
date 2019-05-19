//////////////////////////////////////////////////////////////////////
//文件名称：ClientManager.h
//功能描述：
//版本说明：ClientManager API
//			
//编写作者：2017.09.05 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#ifndef __CLIENTMANAGER_H__
#define __CLIENTMANAGER_H__

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include "../Common/Type.h"
#include "../Common/BaseType.h"
#include "../Common/Thread.h"
//!alter by huyf 2018.04.23:添加连接开关可配置 
#include "../Common/Config.h"
//!alter end:添加连接开关可配置 
#include "../NetBase/ClientSocket.h"


#include "PlayerManager.h"


//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////

class ClientPlayer;
//////////////////////////////////////////////////////////////////////
//class
//////////////////////////////////////////////////////////////////////
class ClientManager : public PlayerManager
{
public:
	ClientManager();
	virtual ~ClientManager();

public:
	//模块初始化接口
	BOOL 	Init();
	//网络监听接口
	BOOL    Connect();
	BOOL 	ReconnectToPW();
	BOOL 	ReconnectToMTS();
	BOOL 	ReconnectToACC();
	//Server Reg 
	BOOL 	ServerRegToACC();
	BOOL 	ServerRegToMTS();
	BOOL 	ServerRegToPW();

	//数据接收接口
	BOOL    ProcessInputs();
	//数据发送接口
	BOOL    ProcessOutputs();
	//异常处理处理
	BOOL    ProcessExceptions();
	//消息处理
	BOOL    ProcessCommands();
	//逻辑接口
	BOOL    HeartBeat(DWORD dwTime=0);

	//Send Msg To PW/MTS/Acc
	BOOL	SendMsgToServer(INT sendTo, const std::string& sJson, UInt32 cmd);

	//LOOP 接口
	BOOL	Loop();	

	//
	void 	CleanConnectToACC();
	BOOL 	InitReconnectToACC();
	void 	CleanConnectToMTS();
	BOOL 	InitReconnectToMTS();
	void 	CleanConnectToPW();
	BOOL 	InitReconnectToPW();

	BOOL				IsActive()						{ 	return m_bActive; 			} 
	void				SetActive(BOOL bActive)			{ 	m_bActive = bActive; 		}
	BOOL				IsPWActive()					{ 	return m_bPWActive; 		} 
	void				SetPWActive(BOOL bActive)		{ 	m_bPWActive = bActive; 		}
	BOOL				IsMTSActive()					{ 	return m_bMTSActive; 		} 
	void				SetMTSActive(BOOL bActive)		{ 	m_bMTSActive = bActive; 	}
	BOOL				IsACCActive()					{ 	return m_bACCActive; 		} 
	void				SetACCActive(BOOL bActive)		{ 	m_bACCActive = bActive; 	}
	BOOL 				SetSocketOpt(Socket* pSocket, UINT bLinger=TRUE, BOOL bReuseAddr=TRUE, BOOL bReusePort=TRUE, BOOL bNoDelay=TRUE, BOOL bKeepAlive=TRUE, BOOL bNonBlocking=TRUE, BOOL bDontLinger=TRUE,UINT unSendBuffSize=16*1024, UINT unRecvBuffSize=32*1024);
	//!alter by huyf 2018.04.23:添加连接开关可配置 
	BOOL                IsNeedConnectPW()				{	return g_config.PW_CONNECT_OPEN != 0 ? TRUE : FALSE;	}
	BOOL                IsNeedConnectMTS()				{	return g_config.MTS_CONNECT_OPEN != 0 ? TRUE : FALSE;	}
	BOOL                IsNeedConnectACC()				{	return g_config.ACC_CONNECT_OPEN != 0 ? TRUE : FALSE;	}
	//!alter end:添加连接开关可配置 
public:
	TID					m_ThreadID;
	BOOL 				m_bActive;

private:
	//PW数据相关
	ClientPlayer*		m_pClientPlayerToPW;
	BOOL 				m_bPWActive;
	DWORD 				m_nTimeToPW;
	DWORD 				m_nReconnectIntervalTimeToPW;			//add by huyf 2017.12.17:add reconnect interval time to MTS Server

	//MTS数据相关
	ClientPlayer*		m_pClientPlayerToMTS;
	BOOL 				m_bMTSActive;
	DWORD 				m_nTimeToMTS;
	DWORD 				m_nReconnectIntervalTimeToMTS;			//add by huyf 2017.12.17:add reconnect interval time to MTS Server

	//Acc数据相关
	ClientPlayer*		m_pClientPlayerToACC;
	BOOL 				m_bACCActive;
	DWORD 				m_nTimeToACC;
	DWORD 				m_nReconnectIntervalTimeToACC;	//add by huyf 2017.12.17:add reconnect interval time to ACC Server
};

//全局变量
extern ClientManager* g_pClientManager;

#endif// __CLIENTMANAGER_H__
