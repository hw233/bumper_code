//////////////////////////////////////////////////////////////////////
//文件名称：ClientManager.cpp
//功能描述：
//版本说明：ClientManager API
//			
//编写作者：2017.09.05 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include <ctime>
#include <chrono>

#include "../Base/Log.h"
#include "../Base/logger.h"
#include "../Base/TimeManager.h"
#include "PacketFactoryManager.h"
#include "../Common/IniFile.h"

#include "ClientManager.h"
#include "ClientPool.h"

#include "../Protocol/cJSON.h"
#include "../Protocol/Command.h"
#include <time.h>
using namespace std;
using namespace chrono;

//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////
#define ACCEPT_ONESTEP 50

//全局变量
ClientManager* g_pClientManager = NULL;

//////////////////////////////////////////////////////////////////////
//class
//////////////////////////////////////////////////////////////////////

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
ClientManager::ClientManager()
{
	DEBUG_TRY	
    m_ThreadID = 0;
    SetActive(TRUE);

    //!alter by huyf 2018.04.23:添加连接开关可配置
    if (IsNeedConnectPW())
    {
        m_pClientPlayerToPW = NULL;
        SetPWActive(FALSE);
        m_nTimeToPW = 0;
        m_nReconnectIntervalTimeToPW = time(NULL);
    }
	
    if (IsNeedConnectMTS())
    {
        m_pClientPlayerToMTS = NULL;
        SetMTSActive(FALSE);
        m_nTimeToMTS = 0;
        m_nReconnectIntervalTimeToMTS = time(NULL);
    }    	

    if (IsNeedConnectACC())
    {
        m_pClientPlayerToACC = NULL;
        SetACCActive(FALSE);
        m_nTimeToACC = 0;
        m_nReconnectIntervalTimeToACC = time(NULL);
    }
    //!alter end:添加连接开关可配置
    
    //this->Init();
    DEBUG_CATCH("ClientManager Constructor")
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
ClientManager::~ClientManager()
{
	DEBUG_TRY
    //!alter by huyf 2018.04.23:添加连接开关可配置
    if (IsNeedConnectPW())
    {
        SAFE_DELETE(m_pClientPlayerToPW);
    }
    
    if (IsNeedConnectMTS())
    {
        SAFE_DELETE(m_pClientPlayerToMTS);
    }       

    if (IsNeedConnectACC())
    {
        SAFE_DELETE(m_pClientPlayerToACC);
    }
    //!alter end:添加连接开关可配置    	
    DEBUG_CATCH("ClientManager Deconstructor")
	
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	CleanConnectToACC
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
void ClientManager::CleanConnectToACC()
{
	DEBUG_TRY
    //!alter by huyf 2018.04.23:添加连接开关可配置
    if (IsNeedConnectACC())
    {
	   SAFE_DELETE(m_pClientPlayerToACC);
    }
    //!alter end:添加连接开关可配置  
	DEBUG_CATCH("CleanConnectToACC")	
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	CleanConnectToMTS
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
void ClientManager::CleanConnectToMTS()
{
	DEBUG_TRY
    //!alter by huyf 2018.04.23:添加连接开关可配置	
    if (IsNeedConnectMTS())
    {
	   SAFE_DELETE(m_pClientPlayerToMTS);	
    }	
    //!alter end:添加连接开关可配置  
	DEBUG_CATCH("CleanConnectToMTS")
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	CleanConnectToPW
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无

void ClientManager::CleanConnectToPW()
{
    DEBUG_TRY   
    //!alter by huyf 2018.04.23:添加连接开关可配置
    if (IsNeedConnectPW())
    {
	   SAFE_DELETE(m_pClientPlayerToPW);
    }
    //!alter end:添加连接开关可配置  
 	DEBUG_CATCH("CleanConnectToPW")
}


//////////////////////////////////////////////////////////////////////
//
// func description	:	模块初始化接口InitReconnectToAcc
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ClientManager::InitReconnectToACC()
{
    DEBUG_TRY	
    //////////////////////////////////////////////////////////////////
    //-------------------------------ACC--------------------------------
    //////////////////////////////////////////////////////////////////
    //!alter by huyf 2018.04.23:添加连接开关可配置
    if (IsNeedConnectACC())
    {
        //创建m_pClientPlayerToMTS对象
        m_pClientPlayerToACC = new ClientPlayer();
        CHECKF(m_pClientPlayerToACC);
        m_pClientPlayerToACC->SetPlayerID(ACC_SERVER_NUM);

        Socket* socket = m_pClientPlayerToACC->GetSocket();
        CHECKF(socket);

        // create socket
        BOOL ret = socket->create();
        if (!ret)
        {
            LOGERROR("[ClientManager::InitReconnectToACC] ACC socket create is error");
            return FALSE;
        }

        //设置socket属性
        SetSocketOpt(socket, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, 0, 0);

        //add by huyf 2018.03.14:可以绑定特定端口去连接服务器
        if (g_config.ONLINE_BIND_PORT_CONNECT_ACC > 0)
        {
            ret = socket->bind(g_config.ONLINE_BIND_PORT_CONNECT_ACC);
            if (!ret)
            {
                LOGERROR("[ClientManager::InitReconnectToACC] ACC socket bind is error bind port=[%d]", g_config.ONLINE_BIND_PORT_CONNECT_ACC);
                return FALSE;
            }
            LOGDEBUG("[ClientManager::InitReconnectToACC] ACC socket bind is Ok... ONLINE_BIND_PORT_CONNECT_ACC=[%d]", g_config.ONLINE_BIND_PORT_CONNECT_ACC);
        }  
        //add end:可以绑定特定端口去连接服务器 

        //!alter by huyf 2018.04.23:释放连接之后，重新建立连接时候重置时间戳
        m_nReconnectIntervalTimeToACC = time(NULL);    
        //!alter end:释放连接之后，重新建立连接时候重置时间戳

    }
    //!alter end:添加连接开关可配置
    DEBUG_CATCHF("InitReconnectToACC")
    return TRUE;
	
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	模块初始化接口InitReconnectToMTS
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ClientManager::InitReconnectToMTS()
{
	DEBUG_TRY    
    //////////////////////////////////////////////////////////////////
    //-------------------------------MTS--------------------------------
    //////////////////////////////////////////////////////////////////
    //!alter by huyf 2018.04.23:添加连接开关可配置   
    if (IsNeedConnectMTS())
    {
        //创建m_pClientPlayerToMTS对象
        m_pClientPlayerToMTS = new ClientPlayer();
        CHECKF(m_pClientPlayerToMTS);
        m_pClientPlayerToMTS->SetPlayerID(MTS_SERVER_NUM);

        Socket* socket = m_pClientPlayerToMTS->GetSocket();
        CHECKF(socket);

        // create socket
        BOOL ret = socket->create();
        if (!ret)
        {
            LOGERROR("[ClientManager::InitReconnectToMTS] MTS socket create is error");
            return FALSE;
        }

        //设置socket属性
        SetSocketOpt(socket, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, 0, 0);

        //add by huyf 2018.03.14:可以绑定特定端口去连接服务器
        if (g_config.ONLINE_BIND_PORT_CONNECT_MTS > 0)	
        {
           ret = socket->bind(g_config.ONLINE_BIND_PORT_CONNECT_MTS);
            if (!ret)
            {
                LOGERROR("[ClientManager::InitReconnectToMTS] MTS socket bind is error bind port=[%d]", g_config.ONLINE_BIND_PORT_CONNECT_MTS);
                return FALSE;
            }
            LOGDEBUG("[ClientManager::InitReconnectToMTS] MTS socket bind is Ok... ONLINE_BIND_PORT_CONNECT_MTS=[%d]", g_config.ONLINE_BIND_PORT_CONNECT_MTS); 
        }    
        //add end:可以绑定特定端口去连接服务器 

        //!alter by huyf 2018.04.23:释放连接之后，重新建立连接时候重置时间戳    
        m_nReconnectIntervalTimeToMTS = time(NULL);    
       //!alter end:释放连接之后，重新建立连接时候重置时间戳
    }
    //!alter end:添加连接开关可配置
	DEBUG_CATCHF("ClientManager::InitReconnectToMTS")
	return TRUE;
}



//////////////////////////////////////////////////////////////////////
//
// func description	:	模块初始化接口InitReconnectToPW
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无

BOOL ClientManager::InitReconnectToPW()
{
	DEBUG_TRY	
    //////////////////////////////////////////////////////////////////
    //-------------------------------PW--------------------------------
    //////////////////////////////////////////////////////////////////
    //!alter by huyf 2018.04.23:添加连接开关可配置   
    if (IsNeedConnectPW())
    {
        //创建m_pClientPlayerToPW对象
        m_pClientPlayerToPW = new ClientPlayer();
        CHECKF(m_pClientPlayerToPW);
        m_pClientPlayerToPW->SetPlayerID(PW_SERVER_NUM);

        Socket* socket = m_pClientPlayerToPW->GetSocket();
        CHECKF(socket);

        // create socket
        BOOL ret = socket->create();
        if (!ret)
        {
            LOGERROR("[ClientManager::InitReconnectToPW] PW socket create is error");
            return FALSE;
        }	

        //设置socket属性
        SetSocketOpt(socket, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, 0, 0);

        //add by huyf 2018.03.14:可以绑定特定端口去连接服务器 
        if (g_config.ONLINE_BIND_PORT_CONNECT_PW > 0)  
        {
            ret = socket->bind(g_config.ONLINE_BIND_PORT_CONNECT_PW);
            if (!ret)
            {
                LOGERROR("[ClientManager::InitReconnectToPW] PW socket bind is error bind port=[%d]", g_config.ONLINE_BIND_PORT_CONNECT_PW);
                return FALSE;
            }
            LOGDEBUG("[ClientManager::InitReconnectToPW] PW socket bind is Ok... ONLINE_BIND_PORT_CONNECT_PW=[%d]", g_config.ONLINE_BIND_PORT_CONNECT_PW);
        }
        //add end:可以绑定特定端口去连接服务器 
    	LOGDEBUG("[ClientManager::InitReconnectToPW] PlayerID=[%d]", m_pClientPlayerToMTS->GetPlayerID());

       //!alter by huyf 2018.04.23:释放连接之后，重新建立连接时候重置时间戳
        m_nReconnectIntervalTimeToPW = time(NULL);
        //!alter end:释放连接之后，重新建立连接时候重置时间戳
    }
    //!alter end:添加连接开关可配置
	DEBUG_CATCHF("InitReconnectToPW")
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
//
// func description	:	模块初始化接口
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ClientManager::Init()
{
	DEBUG_TRY	
    BOOL ret = FALSE;
    Socket* socket = NULL;

    //////////////////////////////////////////////////////////////////
    //-------------------------------PW--------------------------------
    //////////////////////////////////////////////////////////////////
    //!alter by huyf 2018.04.23:添加连接开关可配置   
    if (IsNeedConnectPW())
    {
        //创建m_pClientPlayerToPW对象
        m_pClientPlayerToPW = new ClientPlayer();
        CHECKF(m_pClientPlayerToPW);
        m_pClientPlayerToPW->SetPlayerID(PW_SERVER_NUM);

        socket = m_pClientPlayerToPW->GetSocket();
        CHECKF(socket);

        // create socket
        ret = socket->create();
        if (!ret)
        {
            LOGERROR("[ClientManager::Init] PW Socket create Is Error");
            return FALSE;
        }   

        //设置socket属性
        SetSocketOpt(socket, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, 0, 0);

        //add by huyf 2018.03.14:可以绑定特定端口去连接服务器  
        if (g_config.ONLINE_BIND_PORT_CONNECT_PW > 0)  
        {
            ret = socket->bind(g_config.ONLINE_BIND_PORT_CONNECT_PW);
            if (!ret)
            {
                LOGERROR("[ClientManager::Init] MTS Socket Bind Is Error Bind Port=[%d]", g_config.ONLINE_BIND_PORT_CONNECT_PW);
                return FALSE;
            }
            LOGDEBUG("[ClientManager::Init] MTS Socket Bind Is Ok... ONLINE_BIND_PORT_CONNECT_PW=[%d]", g_config.ONLINE_BIND_PORT_CONNECT_PW);
        }
        //add end:可以绑定特定端口去连接服务器
    }

    //////////////////////////////////////////////////////////////////
    //-------------------------------MTS-------------------------------
    //////////////////////////////////////////////////////////////////
    //!alter by huyf 2018.04.23:添加连接开关可配置   
    if (IsNeedConnectMTS())
    {
        //创建m_pClientPlayerToMTS对象
        m_pClientPlayerToMTS = new ClientPlayer();
        CHECKF(m_pClientPlayerToMTS);
        m_pClientPlayerToMTS->SetPlayerID(MTS_SERVER_NUM);

        socket = m_pClientPlayerToMTS->GetSocket();
        CHECKF(socket);

        // create socket
        ret = socket->create();
        if (!ret)
        {
            LOGERROR("[ClientManager::Init] MTS Socket create Is Error");
            return FALSE;
        }	

        //设置socket属性
        SetSocketOpt(socket, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, 0, 0);

        //add by huyf 2018.03.14:可以绑定特定端口去连接服务器 
        if (g_config.ONLINE_BIND_PORT_CONNECT_MTS > 0)  
        {
            ret = socket->bind(g_config.ONLINE_BIND_PORT_CONNECT_MTS);
            if (!ret)
            {
                LOGERROR("[ClientManager::Init] MTS Socket Bind Is Error Bind Port=[%d]", g_config.ONLINE_BIND_PORT_CONNECT_MTS);
                return FALSE;
            }
            LOGDEBUG("[ClientManager::Init] MTS Socket Bind Is Ok... ONLINE_BIND_PORT_CONNECT_MTS=[%d]", g_config.ONLINE_BIND_PORT_CONNECT_MTS);
        }
        //add end:可以绑定特定端口去连接服务器
    }

    //////////////////////////////////////////////////////////////////
    //-------------------------------ACC-------------------------------
    //////////////////////////////////////////////////////////////////
    //!alter by huyf 2018.04.23:添加连接开关可配置   
    if (IsNeedConnectACC())
    {
        //创建ServerSocket对象
        m_pClientPlayerToACC = new ClientPlayer();
        CHECKF(m_pClientPlayerToACC);
        m_pClientPlayerToACC->SetPlayerID(ACC_SERVER_NUM);

        socket = m_pClientPlayerToACC->GetSocket();
        CHECKF(socket);

        // create socket
        ret = socket->create();
        if (!ret)
        {
            LOGERROR("[ClientManager::Init] ACC Socket create Is error");
            return FALSE;
        }

        //设置socket属性
        SetSocketOpt(socket, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, 0, 0);

        //add by huyf 2018.03.14:可以绑定特定端口去连接服务器 
        if (g_config.ONLINE_BIND_PORT_CONNECT_MTS > 0)  
        {
            ret = socket->bind(g_config.ONLINE_BIND_PORT_CONNECT_ACC);
            if (!ret)
            {
                LOGERROR("[ClientManager::Init] ACC Socket Bind Is Error Bind Port=[%d]", g_config.ONLINE_BIND_PORT_CONNECT_ACC);
                return FALSE;
            }
            LOGDEBUG("[ClientManager::Init] ACC Socket Bind Is Ok... ONLINE_BIND_PORT_CONNECT_ACC=[%d]", g_config.ONLINE_BIND_PORT_CONNECT_ACC);
        }
        //add end:可以绑定特定端口去连接服务器	
    }
    //!alter end:添加连接开关可配置

    //获取到线程id
    m_ThreadID = MyGetCurrentThreadID();

    //创建连接
    this->Connect();

    this->ServerRegToACC();
    this->ServerRegToMTS();
    this->ServerRegToPW();

    //!alter by huyf 2018.04.23:添加连接开关可配置   
    if (IsNeedConnectACC())
    {
        m_pClientPlayerToACC->UpdateNowTime();
    }
    //!alter end:添加连接开关可配置

    //!alter by huyf 2018.04.23:添加连接开关可配置   
    if (IsNeedConnectMTS())
    {
        m_pClientPlayerToMTS->UpdateNowTime();
    }
    //!alter end:添加连接开关可配置

    //!alter by huyf 2018.04.23:添加连接开关可配置   
    if (IsNeedConnectPW())
    {
        m_pClientPlayerToPW->UpdateNowTime();
    }
    //!alter end:添加连接开关可配置
    
    //m_nTimeToACC = system_clock::to_time_t(system_clock::now());
    //m_nTimeToMTS = system_clock::to_time_t(system_clock::now());

	DEBUG_CATCHF("ClientManager::Init")
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
// func description :   SetOpt
//
//-----input------
// Parameters
//      Socket*     :pSocket
//------output------
// Return 
//      无
// 注意：   在bind之前一定要设置好ReuseAddr和ReusePort
//         在connect之前设置好阻塞模式，在连接开始的时候确立是否立即返回
//         在connect之前设置好接收缓冲区，因为在syn建立连接诶三次握手中进行同步
//         在lisent之前设置好socket属性，因为在accept返回的socket继承自lisent的属性
BOOL ClientManager::SetSocketOpt(Socket* pSocket, UINT bLinger/*=TRUE*/, BOOL bReuseAddr/*=TRUE*/, BOOL bReusePort/*=TRUE*/, BOOL bNoDelay/*=TRUE*/, BOOL bKeepAlive/*=TRUE*/, BOOL bNonBlocking/*=TRUE*/, BOOL bDontLinger/*=TRUE*/,UINT unSendBuffSize/*=0*/, UINT unRecvBuffSize/*=0*/)
{
    CHECKF(pSocket);
    //设置关闭模式
    BOOL iRet = pSocket->setSocketOpt(bLinger, bReuseAddr, bReusePort, bNoDelay, bKeepAlive, bNonBlocking, bDontLinger, unSendBuffSize, unRecvBuffSize);
    if (!iRet)
    {
        LOGERROR("[ClientManager::SetSocketOpt] socket SetSocketOpt is Error iRet=[%d]", iRet);
        return FALSE;
    }
    return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	ReconnectToACC
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ClientManager::ReconnectToACC()
{
	DEBUG_TRY
    //!alter by huyf 2018.04.23:添加连接开关可配置   
    if (IsNeedConnectACC())
    {
        //add by huyf 2017.12.18:30s reconnect to acc
         DWORD curTm = time(NULL);
        if(curTm < m_nReconnectIntervalTimeToACC)
        {
            return FALSE;
        }
        m_nReconnectIntervalTimeToACC = curTm + g_config.ONLINE_RECONNECT_INTERVAL_TIME_TO_ACC/*_RECONNECT_INTERVAL_TIME_TO_ACC*/;
        //add end:30s reconnect to acc
        
        //////////////////////////////////////////////////////////////////
        //-------------------------------ACC--------------------------------
        //////////////////////////////////////////////////////////////////
        Socket* socket = m_pClientPlayerToACC->GetSocket();
        CHECKF(socket);

        //LOGDEBUG("[ClientManager::Reconnect] Reconnect To ACC-SERVER [%s:%d]", g_config.ACC_IP, g_config.ACC_PORT);
        BOOL iRet = socket->connect(g_config.ACC_IP,g_config.ACC_PORT);
        if(!iRet)
        {
            LOGERROR("[ClientManager::Reconnect] Reconnect To ACC-SERVER Is ERROR IP=[%s]\tPORT=[%d]-----------\n", g_config.ACC_IP, g_config.ACC_PORT);
            SetACCActive(FALSE);
            return FALSE;
        }
        else
        {
            LOGDEBUG("[ClientManager::Reconnect] Reconnect To ACC-SERVER Is SUC... IP=[%s]\tPORT=[%d]-----------\n", g_config.ACC_IP, g_config.ACC_PORT);
            SetACCActive(TRUE);
            //设置socket属性
            //SetSocketOpt(socket, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, 0, 0);  
        }
    }
    //!alter end:添加连接开关可配置
	DEBUG_CATCHF("ClientManager::ReconnectToACC")
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
//
// func description	:	ReconnectToPW
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无

BOOL ClientManager::ReconnectToPW()
{
	DEBUG_TRY
    //!alter by huyf 2018.04.23:添加连接开关可配置   
    if (IsNeedConnectPW())
    {
        //add by huyf 2017.12.18:30s reconnect to mts
        DWORD curTm = time(NULL);
        if(curTm < m_nReconnectIntervalTimeToPW)
        {
            return FALSE;
        
        }
        m_nReconnectIntervalTimeToPW = curTm + g_config.ONLINE_RECONNECT_INTERVAL_TIME_TO_PW/*_RECONNECT_INTERVAL_TIME_TO_PW*/;//30S reconnect to mts
        //add end:30s reconnect to mts
        //////////////////////////////////////////////////////////////////
        //-------------------------------PW--------------------------------
        //////////////////////////////////////////////////////////////////
        Socket* socket = m_pClientPlayerToPW->GetSocket();
        CHECKF(socket);
        //LOGDEBUG("[ClientManager::ReconnectToPW] Reconnect To PW-SERVER [%s:%d]", g_config.PW_IP, g_config.PW_PORT);       
        BOOL iRet = socket->connect(g_config.PW_IP,g_config.PW_PORT);
        if(!iRet)
        {
            LOGERROR("[ClientManager::ReconnectToPW] Reconnect To PW-SERVER Is ERROR:IP=[%s]\tPORT=[%d]-----------\n", g_config.PW_IP, g_config.PW_PORT);
            SetPWActive(FALSE);
            return FALSE;
        }
        else
        {
            LOGDEBUG("[ClientManager::ReconnectToPW] Reconnect To PW-SERVER Is SUC:IP=[%s]\tPORT=[%d]-----------\n", g_config.PW_IP, g_config.PW_PORT);
            SetPWActive(TRUE);
            //设置socket属性
            //SetSocketOpt(socket, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, 0, 0); 
        }
    }
    //!alter end:添加连接开关可配置
	DEBUG_CATCHF("ReconnectToPW Is Catch...")
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
//
// func description	:	ReconnectToMTS
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ClientManager::ReconnectToMTS()
{
	
    DEBUG_TRY
    //!alter by huyf 2018.04.23:添加连接开关可配置   
    if (IsNeedConnectMTS())
    {
        //add by huyf 2017.12.18:30s reconnect to mts
        DWORD curTm = time(NULL);
        if(curTm < m_nReconnectIntervalTimeToMTS)
        {
            return FALSE;
        
        }
        m_nReconnectIntervalTimeToMTS = curTm + g_config.ONLINE_RECONNECT_INTERVAL_TIME_TO_MTS/*_RECONNECT_INTERVAL_TIME_TO_MTS*/;//30S reconnect to mts
        //add end:30s reconnect to mts
        //////////////////////////////////////////////////////////////////
        //-------------------------------MTS--------------------------------
        //////////////////////////////////////////////////////////////////
        Socket* socket = m_pClientPlayerToMTS->GetSocket();
        CHECKF(socket);

        //LOGDEBUG("[ClientManager::ReconnectToMTS] Reconnect To MTS-SERVER [%s:%d]", g_config.MTS_IP, g_config.MTS_PORT);
        BOOL iRet = socket->connect(g_config.MTS_IP,g_config.MTS_PORT);
        if(!iRet)
        {
            LOGERROR("[ClientManager::ReconnectToMTS] Reconnect To MTS-SERVER Is ERROR IP=[%s]\tPORT=[%d]-----------\n", g_config.MTS_IP, g_config.MTS_PORT);
            SetMTSActive(FALSE);
            return FALSE;
        }
        else
        {
            LOGDEBUG("[ClientManager::ReconnectToMTS] Reconnect To MTS-SERVER Is SUC... IP=[%s]\tPORT=[%d]-----------\n", g_config.MTS_IP, g_config.MTS_PORT);
            SetMTSActive(TRUE);
            //设置socket属性
            //SetSocketOpt(socket, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, 0, 0);  
        }
    }
    //!alter end:添加连接开关可配置
	DEBUG_CATCHF("ClientManager::ReconnectToMTS")
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
//
// func description	:	连接网络地址
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ClientManager::Connect()
{
	DEBUG_TRY	
    BOOL iRet = FALSE;
    Socket* socket = NULL;

    //////////////////////////////////////////////////////////////////
    //-------------------------------PW--------------------------------
    //////////////////////////////////////////////////////////////////
    //!alter by huyf 2018.04.23:添加连接开关可配置   
    if (IsNeedConnectPW())
    {
        socket = m_pClientPlayerToPW->GetSocket();
        CHECKF(socket);

        //LOGDEBUG("[ClientManager::Connect] Connect To PW-SERVER [%s:%d]", g_config.PW_IP, g_config.PW_PORT);
        iRet = socket->connect(g_config.PW_IP,g_config.PW_PORT);
        if(!iRet)
        {
            LOGERROR("[ClientManager::Connect] Connect To PW-SERVER Is ERROR IP=[%s]\tPORT=[%d]\tErrno[%d]\tDiscription[%s]-----------\n", g_config.PW_IP, g_config.PW_PORT, errno, strerror(errno));
            SetPWActive(FALSE);
        }
        else
        {
            LOGDEBUG("[ClientManager::Connect] Connect To PW-SERVER Is SUC... IP=[%s]\tPORT=[%d]-----------\n", g_config.PW_IP, g_config.PW_PORT);
            SetPWActive(TRUE);

            //设置socket属性
            //SetSocketOpt(socket, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, 0, 0);       
        }
    }

    //////////////////////////////////////////////////////////////////
    //-------------------------------MTS--------------------------------
    //////////////////////////////////////////////////////////////////
    //!alter by huyf 2018.04.23:添加连接开关可配置   
    if (IsNeedConnectMTS())
    {
        socket = m_pClientPlayerToMTS->GetSocket();
        CHECKF(socket);

        //LOGDEBUG("[ClientManager::Connect] Connect To MTS-SERVER [%s:%d]", g_config.MTS_IP, g_config.MTS_PORT);
        iRet = socket->connect(g_config.MTS_IP,g_config.MTS_PORT);
        if(!iRet)
        {
            LOGERROR("[ClientManager::Connect] Connect To MTS-SERVER Is ERROR IP=[%s]\tPORT=[%d]\tErrno[%d]\tDiscription[%s]-----------\n", g_config.MTS_IP, g_config.MTS_PORT, errno, strerror(errno));
            SetMTSActive(FALSE);
        }
        else
        {
            LOGDEBUG("[ClientManager::Connect] Connect To MTS-SERVER Is SUC... IP=[%s]\tPORT=[%d]-----------\n", g_config.MTS_IP, g_config.MTS_PORT);
            SetMTSActive(TRUE);

            //设置socket属性
            //SetSocketOpt(socket, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, 0, 0);       
        }
    }
    //////////////////////////////////////////////////////////////////
    //-------------------------------ACC--------------------------------
    //////////////////////////////////////////////////////////////////
    //!alter by huyf 2018.04.23:添加连接开关可配置   
    if (IsNeedConnectACC())
    {
        socket = m_pClientPlayerToACC->GetSocket();
        CHECKF(socket);

        //LOGDEBUG("[ClientManager::Connect] Connect To ACC-SERVER [%s:%d]", g_config.ACC_IP, g_config.ACC_PORT);
        iRet = socket->connect(g_config.ACC_IP,g_config.ACC_PORT);
        if(!iRet)
        {
            LOGERROR("[ClientManager::Connect] Connect To ACC-SERVER Is ERROR IP=[%s]\tPORT=[%d]\tErrno[%d]\tDiscription[%s]-----------\n", g_config.ACC_IP, g_config.ACC_PORT, errno, strerror(errno));
            SetACCActive(FALSE);
        }
        else
        {
            LOGDEBUG("[ClientManager::Connect] Connect To ACC-SERVER Is SUC... IP=[%s]\tPORT=[%d]-----------\n", g_config.ACC_IP, g_config.ACC_PORT);
            SetACCActive(TRUE);
            //设置socket属性
            //SetSocketOpt(socket, FALSE, FALSE, FALSE, FALSE, FALSE, TRUE, FALSE, 0, 0);  
        }
    }
    //!alter end:添加连接开关可配置
    DEBUG_CATCHF("ClientManager::Connect")
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	Server Reg To ACC
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ClientManager::ServerRegToACC()
{
	DEBUG_TRY
    //!alter by huyf 2018.04.23:添加连接开关可配置   
    if (IsNeedConnectACC())
    {
        //add by huyf 2018.06.05:增加激活判断
        if (IsACCActive())
        {
            CHECKF(m_pClientPlayerToACC);
            Socket* socket = m_pClientPlayerToACC->GetSocket();
            CHECKF(socket);

            BOOL ret = socket->isValid();
            if(ret)
            {
                cJSON* pServerReg = cJSON_CreateObject();
                if (pServerReg)
                {
                    cJSON_AddNumberToObject(pServerReg, "serverRegType", SERVER_TYPE_OL);
                    //add by huyf 2018.01.26:添加online注册索引
                    //CIniFile ini("../Config/OnLineConfig.ini", "OnLineServer");
                    int nRegistIndex = g_config.ONLINE_REGIST_INDEX/*ini.GetInt("REGIST_INDEX")*/;
                    cJSON_AddNumberToObject(pServerReg, "serverRegID", nRegistIndex);
                    //add end:添加online注册索引

                    LOGDEBUG("[ClientManager::ServerRegToACC] SendMsgToServer=[%d]", COMMON_SERVER_REG_REQ);
                    char* pJson = cJSON_Print(pServerReg);
                    CHECKF(pJson);
                    std::string sJson(pJson);
                    g_pClientManager->SendMsgToServer(ACC_SERVER, sJson, COMMON_SERVER_REG_REQ);
                    //add by huyf 2018.06.15:内存泄漏
                    free(pJson);
                    //add end:内存泄漏
                }
                cJSON_Delete(pServerReg);
                return TRUE;
            }                
        }
        //add end:增加激活判断
    }
    //!alter end:添加连接开关可配置
    DEBUG_CATCHF("ClientManager::ServerRegToACC")
    return FALSE;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	Server Reg To MTS
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ClientManager::ServerRegToMTS()
{
	DEBUG_TRY	
    //!alter by huyf 2018.04.23:添加连接开关可配置   
    if (IsNeedConnectMTS())
    {
        //add by huyf 2018.06.05:增加激活判断
        if (IsMTSActive())
        {
            CHECKF(m_pClientPlayerToMTS);
            Socket* socket = m_pClientPlayerToMTS->GetSocket();
            CHECKF(socket);

            BOOL ret = socket->isValid();
            if(ret)
            {
                cJSON* pServerReg = cJSON_CreateObject();
                if (pServerReg)
                {
                    cJSON_AddNumberToObject(pServerReg, "serverRegType", SERVER_TYPE_OL);
                    //add by huyf 2018.01.26:添加online注册索引
                    //CIniFile ini("../Config/OnLineConfig.ini", "OnLineServer");
                    int nRegistIndex = g_config.ONLINE_REGIST_INDEX/*ini.GetInt("REGIST_INDEX")*/;
                    cJSON_AddNumberToObject(pServerReg, "serverRegID", nRegistIndex);
                    //add end:添加online注册索引

                    LOGDEBUG("[ClientManager::ServerRegToMTS] SendMsgToServer=[%d]", COMMON_SERVER_REG_REQ);
                    char* pJson = cJSON_Print(pServerReg);
                    CHECKF(pJson);
                    std::string sJson(pJson);
                    g_pClientManager->SendMsgToServer(MTS_SERVER, sJson, COMMON_SERVER_REG_REQ);
                    //add by huyf 2018.06.15:内存泄漏
                    free(pJson);
                    //add end:内存泄漏
                }
                cJSON_Delete(pServerReg);
                return TRUE;
            }
        }
        //add end:增加激活判断
    }
    //!alter end:添加连接开关可配置
	DEBUG_CATCHF("ClientManager::ServerRegToMTS")
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//
// func description :   Server Reg To PW
//
//-----input------
// Parameters
//      无
//------output------
// Return 
//      无
BOOL ClientManager::ServerRegToPW()
{
    DEBUG_TRY   
    //!alter by huyf 2018.04.23:添加连接开关可配置   
    if (IsNeedConnectPW())
    {
        //add by huyf 2018.06.05:增加激活判断
        if (IsPWActive())
        {
            CHECKF(m_pClientPlayerToPW);
            Socket* socket = m_pClientPlayerToPW->GetSocket();
            CHECKF(socket);

            BOOL ret = socket->isValid();
            if(ret)
            {
                cJSON* pServerReg = cJSON_CreateObject();
                if (pServerReg)
                {
                    cJSON_AddNumberToObject(pServerReg, "serverRegType", SERVER_TYPE_OL);
                    //add by huyf 2018.01.26:添加online注册索引
                    //CIniFile ini("../Config/OnLineConfig.ini", "OnLineServer");
                    int nRegistIndex = g_config.ONLINE_REGIST_INDEX/*ini.GetInt("REGIST_INDEX")*/;
                    cJSON_AddNumberToObject(pServerReg, "serverRegID", nRegistIndex);
                    //add end:添加online注册索引

                    LOGDEBUG("[ClientManager::ServerRegToPW] SendMsgToServer-PW=[COMMON_SERVER_REG_REQ=%d] serverRegType=[%d] serverRegID=[%d]", COMMON_SERVER_REG_REQ, SERVER_TYPE_OL, nRegistIndex);
                   
                    char* pJson = cJSON_Print(pServerReg);
                    CHECKF(pJson);
                    std::string sJson(pJson);
                    g_pClientManager->SendMsgToServer(PW_SERVER, sJson, COMMON_SERVER_REG_REQ);
                    //add by huyf 2018.06.15:内存泄漏
                    free(pJson);
                    //add end:内存泄漏
                    

                }
                cJSON_Delete(pServerReg);
                return TRUE;
            }
        }
        //add end:增加激活判断
    }
    //!alter end:添加连接开关可配置
    DEBUG_CATCHF("ClientManager::ServerRegToPW")
    return FALSE;
}


//////////////////////////////////////////////////////////////////////
//
// func description	:	数据接收接口
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ClientManager::ProcessInputs()
{
	DEBUG_TRY	
    BOOL ret = FALSE;
    Socket* socket = NULL;   			

    //////////////////////////////////////////////////////////////////
    //-------------------------------MTS--------------------------------
    //////////////////////////////////////////////////////////////////
    //!alter by huyf 2018.04.23:添加连接开关可配置   
    if (IsNeedConnectMTS())
    {
        if(m_pClientPlayerToMTS)
        {
            if(this->IsMTSActive())
            {
                socket = m_pClientPlayerToMTS->GetSocket();
                CHECKF(socket);

                ret = socket->isValid();
                if(ret)
                {
                    //连接正常
                    DEBUG_TRY
                    ret = m_pClientPlayerToMTS->ProcessInput();
                    if(!ret)
                    {
                        LOGERROR("[ClientManager::ProcessInputs] From MTS-SERVER Send Data Is ERROR: ret=[%d] Connect Is RST", ret);
                        this->SetMTSActive(FALSE);
                        m_pClientPlayerToMTS->CleanUp();
                        this->CleanConnectToMTS();
                    }
                    DEBUG_CATCH("m_pClientPlayerToMTS->ProcessInput()")
                }
                else
                {
                    //...
                    LOGERROR("[ClientManager::ProcessInputs] mts socket Isnot Valid: ret=[%d]", ret);
                }
            }
            else//Reconnect To MTS
            {
                if(this->ReconnectToMTS())
                {
                    this->ServerRegToMTS();	
                    m_pClientPlayerToMTS->UpdateNowTime();
                }
            }			
        }
        else//ReconnectInit
        {
            this->InitReconnectToMTS();
        }
    }

    //////////////////////////////////////////////////////////////////
    //-------------------------------ACC--------------------------------
    //////////////////////////////////////////////////////////////////
    if (IsNeedConnectACC())
    {
        if(m_pClientPlayerToACC)
        {
            if(this->IsACCActive())
            {
                socket = m_pClientPlayerToACC->GetSocket();
                CHECKF(socket);

                ret = socket->isValid();
                if(ret)
                {
                    //连接正常
                    DEBUG_TRY
                    ret = m_pClientPlayerToACC->ProcessInput();
                    if(!ret)
                    {
                        LOGERROR("[ClientManager::ProcessInputs] From ACC-SERVER Send Data Is ERROR: ret=[%d] Connect Is RST", ret);
                        this->SetACCActive(FALSE);
                        m_pClientPlayerToACC->CleanUp();
                        this->CleanConnectToACC();
                    }
                    DEBUG_CATCH("m_pClientPlayerToACC->ProcessInput()")
                }
                else
                {
                    LOGERROR("[ClientManager::ProcessInputs] acc socket Isnot Valid: ret=[%d]", ret);
                }
            }
            else//Reconnect To ACC
            {
                if(this->ReconnectToACC())
                {
                    this->ServerRegToACC();	
                    m_pClientPlayerToACC->UpdateNowTime();
                }
            }			
        }
        else//ReconnectInit
        {
            this->InitReconnectToACC();
        }
    }

    //////////////////////////////////////////////////////////////////
    //-------------------------------PW--------------------------------
    ////////////////////////////////////////////////////////////////// 
    if (IsNeedConnectPW())
    {
        if(m_pClientPlayerToPW)
        {
            if(this->IsPWActive())
            {
                socket = m_pClientPlayerToPW->GetSocket();
                CHECKF(socket);

                ret = socket->isValid();
                if(ret)
                {
                    //连接正常
                    DEBUG_TRY
                    ret = m_pClientPlayerToPW->ProcessInput();
                    if(!ret)
                    {
                        LOGERROR("[ClientManager::ProcessInputs] From PW-SERVER Send Data Is ERROR: ret=[%d] Connect Is RST", ret);
                        this->SetPWActive(FALSE);
                        m_pClientPlayerToPW->CleanUp();
                        this->CleanConnectToPW();
                    }
                    DEBUG_CATCH("m_pClientPlayerToPW->ProcessInput()")
                }
                else
                {
                    LOGERROR("[ClientManager::ProcessInputs] acc socket Isnot Valid: ret=[%d]", ret);
                }
            }
            else//Reconnect To PW
            {
                if(this->ReconnectToPW())
                {
                    this->ServerRegToPW(); 
                    m_pClientPlayerToPW->UpdateNowTime();
                }
            }           
        }
        else//ReconnectInit
        {
            this->InitReconnectToPW();
        }
    }
    //!alter end:添加连接开关可配置
	DEBUG_CATCHF("ClientManager:ProcessInputs")
    return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	数据发送接口
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ClientManager::ProcessOutputs()
{
	DEBUG_TRY			
    BOOL ret = FALSE;
    Socket* socket = NULL;       		

    //////////////////////////////////////////////////////////////////
    //-------------------------------MTS--------------------------------
    //////////////////////////////////////////////////////////////////
    //!alter by huyf 2018.04.23:添加连接开关可配置   
    if (IsNeedConnectMTS())
    {
        if(m_pClientPlayerToMTS)
        {		
            if (this->IsMTSActive())
            {
                socket = m_pClientPlayerToMTS->GetSocket();
                if (socket)
                {

                    ret = socket->isValid();
                    if(ret)
                    {
                        //连接正常
                        DEBUG_TRY
                        ret = m_pClientPlayerToMTS->ProcessOutput();
                        if(!ret)
                        {
                            LOGERROR("[ClientManager::ProcessOutputs] From MTS-SERVER Send Data Is ERROR: ret=[%d] Connect Is RST", ret);
                            this->SetMTSActive(FALSE);
                            m_pClientPlayerToMTS->CleanUp();
                            this->CleanConnectToMTS();
                        }
                        DEBUG_CATCH("m_pClientPlayerToMTS->ProcessOutput()")		
                    }
                }
                else
                {
                    LOGERROR("[ClientManager::ProcessOutputs] mts socket Isnot Valid: ret=[%d]", ret);
                }
            }
            else//Reconnect To MTS
            {
                if(this->ReconnectToMTS())
                {
                    this->ServerRegToMTS();	
                    m_pClientPlayerToMTS->UpdateNowTime();
                }
            }			
        }
        else//ReconnectInit
        {
            this->InitReconnectToMTS();
        }
    }
    //////////////////////////////////////////////////////////////////
    //-------------------------------ACC--------------------------------
    ////////////////////////////////////////////////////////////////// 
    if (IsNeedConnectACC())
    {
        if(m_pClientPlayerToACC)
        {
            if(this->IsACCActive())
            {
                socket = m_pClientPlayerToACC->GetSocket();
                CHECKF(socket);

                ret = socket->isValid();
                if(ret)
                {
                    //连接正常
                    DEBUG_TRY                                
                    ret = m_pClientPlayerToACC->ProcessOutput();
                    if(!ret)
                    {
                            LOGERROR("[ClientManager::ProcessOutputs] From ACC-SERVER Send Data Is ERROR: ret=[%d] Connect Is RST", ret);
                            this->SetACCActive(FALSE);
                            m_pClientPlayerToACC->CleanUp();
                            this->CleanConnectToACC();
                    }                                
                    DEBUG_CATCH("m_pClientPlayerToACC->ProcessOutput()")		
                }
                else
                {
                    LOGERROR("[ClientManager::ProcessOutputs] acc socket Isnot Valid: ret=[%d]", ret);
                }
            }
            else//Reconnect To ACC
            {
                if(this->ReconnectToACC())
                {
                    this->ServerRegToACC();
                    m_pClientPlayerToACC->UpdateNowTime();	
                }
            }			
        }
        else//ReconnectInit
        {
                this->InitReconnectToACC();
        }	
    }

    //////////////////////////////////////////////////////////////////
    //-------------------------------PW--------------------------------
    //////////////////////////////////////////////////////////////////
    if (IsNeedConnectPW())
    {
        if(m_pClientPlayerToPW)
        {
            if(this->IsPWActive())
            {
                socket = m_pClientPlayerToPW->GetSocket();
                CHECKF(socket);

                ret = socket->isValid();
                if(ret)
                {
                    //连接正常
                    DEBUG_TRY                                
                    ret = m_pClientPlayerToPW->ProcessOutput();
                    if(!ret)
                    {
                            LOGERROR("[ClientManager::ProcessOutputs] From PW-SERVER Send Data Is ERROR: ret=[%d] Connect Is RST", ret);
                            this->SetPWActive(FALSE);
                            m_pClientPlayerToPW->CleanUp();
                            this->CleanConnectToPW();
                    }                                
                    DEBUG_CATCH("m_pClientPlayerToPW->ProcessOutput()")        
                }
                else
                {
                    LOGERROR("[ClientManager::ProcessOutputs] acc socket Isnot Valid: ret=[%d]", ret);
                }
            }
            else//Reconnect To PW
            {
                if(this->ReconnectToPW())
                {
                    this->ServerRegToPW();
                    m_pClientPlayerToPW->UpdateNowTime();  
                }
            }           
        }
        else//ReconnectInit
        {
                this->InitReconnectToPW();
        }     
    }  	
    //!alter end:添加连接开关可配置
	DEBUG_CATCHF("ClientManager::ProcessOutputs")
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	异常处理处理
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ClientManager::ProcessExceptions()
{
	DEBUG_TRY	
    /*BOOL ret = FALSE;
    CHECKF(m_pClientPlayerToPW);
    Socket* socket = m_pClientPlayerToPW->GetSocket();
    CHECKF(socket);

    ret = socket->isValid();
    if(!ret)
            //关闭网络连接
            m_pClientPlayerToPW->GetSocket()->close();
    */
	DEBUG_CATCH("ClientManager::ProcessExceptions")
	return TRUE;
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
BOOL ClientManager::ProcessCommands()
{
	DEBUG_TRY
    BOOL ret = FALSE;
    Socket* socket = NULL;

    //////////////////////////////////////////////////////////////////
    //-------------------------------MTS--------------------------------
    //////////////////////////////////////////////////////////////////
    //!alter by huyf 2018.04.23:添加连接开关可配置   
    if (IsNeedConnectMTS())
    {
        if(m_pClientPlayerToMTS)
        {
            if(this->IsMTSActive())
            {

                socket = m_pClientPlayerToMTS->GetSocket();
                CHECKF(socket);

                ret = socket->isValid();
                if(ret)
                {
                    //连接正常
                    DEBUG_TRY
                    ret = m_pClientPlayerToMTS->ProcessCommand();
                    if(!ret)
                    {
                        LOGERROR("From MTS-SERVER Receive Data Is ERROR: ret=[%d]-----------\n", ret);
                        this->SetMTSActive(FALSE);
                        m_pClientPlayerToMTS->CleanUp();
                        this->CleanConnectToMTS();
                    }
                    DEBUG_CATCH("m_pClientPlayerToMTS->ProcessCommand()")	
                }
                else
                {	
                    LOGERROR("[ClientManager::ProcessCommands] mts socket Isnot Valid: ret=[%d]", ret);
                }
            }
            else//Reconnect To MTS
            {
                if(this->ReconnectToMTS())
                {
                    this->ServerRegToMTS();
                    m_pClientPlayerToMTS->UpdateNowTime();	
                }
            }			
        }
        else//ReconnectInit
        {
            this->InitReconnectToMTS();
        }
    }

    //////////////////////////////////////////////////////////////////
    //-------------------------------ACC--------------------------------
    //////////////////////////////////////////////////////////////////
    if (IsNeedConnectACC())
    {
        if(m_pClientPlayerToACC)
        {		
            if(this->IsACCActive())
            {
                socket = m_pClientPlayerToACC->GetSocket();
                CHECKF(socket);

                ret = socket->isValid();
                if(ret)
                {
                    //连接正常
                    DEBUG_TRY
                    ret = m_pClientPlayerToACC->ProcessCommand();
                    if(!ret)
                    {
                        LOGERROR("From Acc-SERVER Receive Data Is ERROR: ret=[%d]-----------\n", ret);
                        this->SetACCActive(FALSE);
                        m_pClientPlayerToACC->CleanUp();
                        this->CleanConnectToACC();
                    }
                    DEBUG_CATCH("m_pClientPlayerToACC->ProcessCommand()")	
                }
                else
                {	
                    LOGERROR("[ClientManager::ProcessCommands] acc socket Isnot Valid: ret=[%d]", ret);
                }
            }
            else//Reconnect To ACC
            {
                if(this->ReconnectToACC())
                {
                    this->ServerRegToACC();	
                    m_pClientPlayerToACC->UpdateNowTime();
                }
            }			
        }
        else//ReconnectInit
        {
            this->InitReconnectToACC();
        }
    }

    //////////////////////////////////////////////////////////////////
    //-------------------------------PW--------------------------------
    //////////////////////////////////////////////////////////////////   
    if (IsNeedConnectPW())
    {
        if(m_pClientPlayerToPW)
        {       
            if(this->IsPWActive())
            {
                socket = m_pClientPlayerToPW->GetSocket();
                CHECKF(socket);

                ret = socket->isValid();
                if(ret)
                {
                    //连接正常
                    DEBUG_TRY
                    ret = m_pClientPlayerToPW->ProcessCommand();
                    if(!ret)
                    {
                        LOGERROR("From PW-SERVER Receive Data Is ERROR: ret=[%d]-----------\n", ret);
                        this->SetPWActive(FALSE);
                        m_pClientPlayerToPW->CleanUp();
                        this->CleanConnectToPW();
                    }
                    DEBUG_CATCH("m_pClientPlayerToPW->ProcessCommand()")   
                }
                else
                {   
                    LOGERROR("[ClientManager::ProcessCommands] acc socket Isnot Valid: ret=[%d]", ret);
                }
            }
            else//Reconnect To PW
            {
                if(this->ReconnectToPW())
                {
                    this->ServerRegToPW(); 
                    m_pClientPlayerToPW->UpdateNowTime();
                }
            }           
        }
        else//ReconnectInit
        {
            this->InitReconnectToPW();
        }
    }
    //!alter end:添加连接开关可配置
	DEBUG_CATCH("ClientManager::ProcessCommands")
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	逻辑接口:心跳
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ClientManager::HeartBeat(DWORD dwTime/*=0*/)
{
    return TRUE;
     
	DEBUG_TRY
    BOOL ret;
    Socket* socket = NULL;

    cJSON* pHeartBeat = cJSON_CreateObject();
    if (pHeartBeat)
    {
            cJSON_AddNumberToObject(pHeartBeat, "nowTime", 0);
    }
    char* pJson = cJSON_Print(pHeartBeat);
    CHECKF(pJson);
    std::string sJson(pJson);
    //add by huyf 2018.06.15:内存泄漏
    free(pJson);
    //add end:内存泄漏
    cJSON_Delete(pHeartBeat);

    //////////////////////////////////////////////////////////////////
    //-------------------------------MTS--------------------------------
    //////////////////////////////////////////////////////////////////
    //!alter by huyf 2018.04.23:添加连接开关可配置   
    if (IsNeedConnectMTS())
    {
        if(m_pClientPlayerToMTS)
        {
            if(this->IsMTSActive())
            {
                socket = m_pClientPlayerToMTS->GetSocket();
                CHECKF(socket);

                ret = socket->isValid();
                if(ret)
                {
                    //连接正常
                    DEBUG_TRY	
                    if (m_pClientPlayerToMTS->CheckTimeEx())
                    {	
                        ret = SendMsgToServer(MTS_SERVER, sJson, COMMON_HEART_BEAT);
                        if (!ret)
                        {
                            LOGERROR("[ClientManager::HeartBeat - Error] From MTS-SERVER Receive Data ret=[%d]", ret);
                            this->SetMTSActive(FALSE);
                            m_pClientPlayerToMTS->CleanUp();
                            this->CleanConnectToMTS();
                        }
                        //更新时间
                        m_pClientPlayerToMTS->UpdateNowTime();
                    }
                    DEBUG_CATCH("SendMsgToServer-MTS")	
                }
                else
                {	
                    LOGERROR("[ClientManager::HeartBeat] mts socket Isnot Valid: ret=[%d]", ret);
                }
            }
            else//Reconnect To MTS
            {
                if(this->ReconnectToMTS())
                {
                    this->ServerRegToMTS();	
                    m_pClientPlayerToMTS->UpdateNowTime();
                }			
            }			
        }
        else//ReconnectInit
        {
            this->InitReconnectToMTS();
        }
    }
    //////////////////////////////////////////////////////////////////
    //-------------------------------ACC--------------------------------
    //////////////////////////////////////////////////////////////////  
    if (IsNeedConnectACC())
    {
        if(m_pClientPlayerToACC)
        {
            if(this->IsACCActive())
            {
                socket = m_pClientPlayerToACC->GetSocket();
                CHECKF(socket);

                ret = socket->isValid();
                if(ret)
                {
                    //连接正常
                    DEBUG_TRY
                    //printf("[ClientManager::HeartBeat] sec[%lu]\t\n", (durationACC.count() * seconds::period::num / seconds::period::den));
                    if (m_pClientPlayerToACC->CheckTimeEx())
                    {
                        ret = SendMsgToServer(ACC_SERVER, sJson, COMMON_HEART_BEAT);
                        if (!ret)
                        {
                            LOGERROR("[ClientManager::HeartBeat - Error] From ACC-SERVER Receive Data ret=[%d]", ret);
                            this->SetACCActive(FALSE);
                            m_pClientPlayerToACC->CleanUp();
                            this->CleanConnectToACC();
                        }
                        //更新时间
                        m_pClientPlayerToACC->UpdateNowTime();
                    }
                    DEBUG_CATCH("SendMsgToServer-ACC")	
                }
                else
                {
                    LOGERROR("[ClientManager::HeartBeat] acc socket Isnot Valid: ret=[%d]", ret);
                }
            }
            else//Reconnect To ACC
            {
                if(this->ReconnectToACC())
                {
                    this->ServerRegToACC();	
                    m_pClientPlayerToACC->UpdateNowTime();
                }
            }			
        }
        else//ReconnectInit
        {
            this->InitReconnectToACC();
        }
    }
    //////////////////////////////////////////////////////////////////
    //-------------------------------PW--------------------------------
    //////////////////////////////////////////////////////////////////
    if (IsNeedConnectPW())
    {
        if(m_pClientPlayerToPW)
        {
            if(this->IsPWActive())
            {
                socket = m_pClientPlayerToPW->GetSocket();
                CHECKF(socket);

                ret = socket->isValid();
                if(ret)
                {
                    //连接正常
                    DEBUG_TRY
                    if (m_pClientPlayerToPW->CheckTimeEx())
                    {
                        ret = SendMsgToServer(PW_SERVER, sJson, COMMON_HEART_BEAT);
                        if (!ret)
                        {
                            LOGERROR("[ClientManager::HeartBeat - Error] From PW-SERVER Receive Data ret=[%d]", ret);
                            this->SetPWActive(FALSE);
                            m_pClientPlayerToPW->CleanUp();
                            this->CleanConnectToPW();
                        }
                        //更新时间
                        m_pClientPlayerToPW->UpdateNowTime();
                    }
                    DEBUG_CATCH("SendMsgToServer-PW")  
                }
                else
                {
                    LOGERROR("[ClientManager::HeartBeat] acc socket Isnot Valid: ret=[%d]", ret);
                }
            }
            else//Reconnect To PW
            {
                if(this->ReconnectToPW())
                {
                    this->ServerRegToPW(); 
                    m_pClientPlayerToPW->UpdateNowTime();
                }
            }           
        }
        else//ReconnectInit
        {
            this->InitReconnectToPW();
        }
    }
    //!alter end:添加连接开关可配置
    
    DEBUG_CATCHF("ClientManager::HeartBeat")
    return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	SendMsgToPW 接口
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ClientManager::SendMsgToServer(INT sendTo, const std::string& sJson, UInt32 cmd)
{   
    DEBUG_TRY
    if (sJson == "")
    {
        LOGERROR("[ClientManager::SendMsgToServer] sJson == NULL!");
    }

    ProtocolHead pHead(cmd, 0, 0);
    pHead.pkgLen = sizeof(ProtocolHead) + sJson.size() - 4;
    UINT msgLen = sizeof(ProtocolHead) + sJson.size();
    char* msg = (char*)malloc(msgLen);

    //add by huyf 2017.10.17
    CHECKF(msg);			
	//add end

    memset(msg, 0, msgLen);
    memcpy(msg, (void*)&pHead, sizeof(ProtocolHead));
    memcpy(msg + sizeof(ProtocolHead), sJson.data(), sJson.size());

	switch(sendTo)
	{

	case PW_SERVER:
        {
            //!alter by huyf 2018.06.05:增加激活判断
            //!alter by huyf 2018.04.23:添加连接开关可配置   
            if (IsNeedConnectPW() && IsPWActive())
            {
                CHECKF(m_pClientPlayerToPW);
                CHECKF(m_pClientPlayerToPW->m_pSocketOutputStream);
                m_pClientPlayerToPW->m_pSocketOutputStream->Write(msg, msgLen);
                m_pClientPlayerToPW->m_pSocketOutputStream->Flush();
                //LOGDEBUG("[ClientManager::SendMsgToServerPW] Ip=[%s] Port=[%u] MsgID=[%d] MsgLen=[%u]", m_pClientPlayerToPW->GetSocket()->m_Host,m_pClientPlayerToPW->GetSocket()->m_Port,cmd, msgLen);
            }
            //!alter end:添加连接开关可配置
            //!alter end:增加激活判断
        }		
		break;
	case MTS_SERVER:
        {
            //!alter by huyf 2018.06.05:增加激活判断
            //!alter by huyf 2018.04.23:添加连接开关可配置   
            if (IsNeedConnectMTS() && IsMTSActive())
            {
                CHECKF(m_pClientPlayerToMTS);
                CHECKF(m_pClientPlayerToMTS->m_pSocketOutputStream);
                m_pClientPlayerToMTS->m_pSocketOutputStream->Write(msg, msgLen);
                m_pClientPlayerToMTS->m_pSocketOutputStream->Flush();
                //LOGDEBUG("[ClientManager::SendMsgToServerMTS] Ip=[%s] Port=[%u] MsgID=[%d] MsgLen=[%u]", m_pClientPlayerToMTS->GetSocket()->m_Host,m_pClientPlayerToMTS->GetSocket()->m_Port,cmd, msgLen);        
            }
            //!alter end:添加连接开关可配置
            //!alter end:增加激活判断
        }
        break;		
	case ACC_SERVER:
        {
            //!alter by huyf 2018.06.05:增加激活判断
            //!alter by huyf 2018.04.23:添加连接开关可配置   
            if (IsNeedConnectACC() && IsACCActive())
            {
                CHECKF(m_pClientPlayerToACC);
                CHECKF(m_pClientPlayerToACC->m_pSocketOutputStream);
                m_pClientPlayerToACC->m_pSocketOutputStream->Write(msg, msgLen);
                m_pClientPlayerToACC->m_pSocketOutputStream->Flush();
                //LOGDEBUG("[ClientManager::SendMsgToServerACC] Ip=[%s] Port=[%u] MsgID=[%d] MsgLen=[%u]", m_pClientPlayerToMTS->GetSocket()->m_Host,m_pClientPlayerToMTS->GetSocket()->m_Port, cmd, msgLen);
            }
            //!alter end:添加连接开关可配置
            //!alter end:增加激活判断
        }
        break;
	default:
		LOGERROR("[ClientManager::SendMsgToServer - Error] SendTo is Error cmd=[%u]", cmd);
		break;
	}

	SAFE_FREE(msg);
    DEBUG_CATCH("ClientManager::SendMsgToServer")
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
//
// func description	:	LOOP 接口
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ClientManager::Loop()
{
	//while(IsActive())
	{
		//printf("<<<<<<<<<<<<<<<<<<<<ClientManager Loop Star>>>>>>>>>>>>>>>>>\n");
		BOOL ret = FALSE;
		//UINT uTime = g_pTimeManager->CurrentTime();

		DEBUG_TRY
        //printf("------------3-2 ProcessExceptions-----------\n");
        ret = ProcessExceptions();
        //CHECK(ret);
        DEBUG_CATCHF("ProcessExceptions")
        
        DEBUG_TRY
        //printf("------------3-3 ProcessInputs-----------\n");
        ret = ProcessInputs();
        //CHECK(ret);		
		DEBUG_CATCHF("ProcessInputs")

		DEBUG_TRY
        //printf("------------3-5 ProcessCommands-----------\n");
        ret = ProcessCommands();
        //CHECK(ret);
		DEBUG_CATCHF("ProcessCommands")

		DEBUG_TRY
        //printf("------------3-4 ProcessOutputs-----------\n");
        ret = ProcessOutputs();
        //CHECK(ret);
		DEBUG_CATCHF("ProcessOutputs")

		DEBUG_TRY
        //printf("------------3-6 HeartBeat-----------\n");
        ret = HeartBeat();
        //CHECK(ret);
		DEBUG_CATCHF("HeartBeat")
                
        return ret;
		//printf("<<<<<<<<<<<<<<<<<<<<ClientManager Loop End>>>>>>>>>>>>>>>>>\n\n");
		

	}
}
