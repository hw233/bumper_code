//////////////////////////////////////////////////////////////////////
//文件名称：ServerManager.cpp
//功能描述：
//版本说明：ServerManager API
//			
//编写作者：2017.08.21 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include "../Base/Log.h"
#include "../Base/logger.h"
#include "../Base/TimeManager.h"
#include "../Common/Config.h"
#include "../Common/IniFile.h"
#include "PacketFactoryManager.h"

#include "ServerManager.h"
#include "PlayerPool.h"

#include "../Protocol/Command.h"
#include "../Protocol/cJSON.h"
#include "../WebSocket/WebSocketHandler.h"
#include "BattleManager.h"
#include "Battle.h"
#include "BattleTeam.h"
#include "BattleConfusionSingle.h"
#include "../MsgHandler/MsgSynRoomInfo.h"
#include "../MsgHandler/MsgWalk.h"
#include "../MsgHandler/MsgFriendInfo.h"
#include "./Battle/BattleCfg.h"


#if defined(__LINUX__)
extern INT errno;
#endif

//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////
#define ACCEPT_ONESTEP 50

//全局变量
ServerManager* g_pServerManager = NULL;

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
ServerManager::ServerManager()
{	
#ifdef _SELECT_ 
        FD_ZERO(&m_ReadFDs[SELECT_BAK]);
        FD_ZERO(&m_WriteFDs[SELECT_BAK]);
        FD_ZERO(&m_ExceptFDs[SELECT_BAK]);

        m_Timeout[SELECT_BAK].tv_sec = 0;
        m_Timeout[SELECT_BAK].tv_usec = 0;
#endif        
        m_MinFD = m_MaxFD = INVALID_SOCKET;

        m_iFDSize = 0;

        SetActive(TRUE);

        m_pServerSocket = NULL;
        //add by huyf 2018.01.24:支持I/O多路复用poll
        //m_SocketID = INVALID_SOCKET;
        //add end:支持I/O多路复用poll
        m_ThreadID = 0;
        for (UINT i = 0; i < OVER_MAX_SERVER; ++i)
        {
                m_aServerHash[i] = INVALID_ID;
        }
        
        //add by huyf 2018.01.24:支持I/O多路复用poll
#ifdef _POLL_        
        for (UINT i=0; i<=CLIENT_MAX_SIZE; ++i)   
        {
            m_ClientSet[i].fd = INVALID_ID;
            m_ClientSet[i].events = 0;        
        }
        m_MaxIndex =  0;  //client[i]最大不空闲位置的下标  
        m_nEffectivePollFds = 0;// 有消息通知的fd数量
#endif   
        //add end:支持I/O多路复用poll
        
        //add by huyf 2018.01.24:支持I/O多路复用epoll
#ifdef _EPOLL_  
        m_nEpollFd = 0;
        m_nEffectiveEpollFds = 0;         // 有消息通知的fd数量
        for (UINT i=0; i<=CLIENT_MAX_SIZE; ++i)
        {   
            m_EpollEvents[i].data.fd = INVALID_ID; 
            m_EpollEvents[i].events = 0; 
            m_EffectiveEpollEvents[i].data.fd = INVALID_ID; 
            m_EffectiveEpollEvents[i].events = 0;      
        } 
        m_MaxEpollIndex = 0;
#endif
        //add end:支持I/O多路复用epoll
        m_tOnTimerInterval.Startup(5); 
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
ServerManager::~ServerManager()
{	
	SAFE_DELETE(m_pServerSocket);
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
BOOL ServerManager::Init()
{	
//#ifdef _EPOLL_
//        //error: jump to label ‘XXXX’ [-fpermissive]:所有变量初始化都放在函数开头，要么所有变量就不让它初始化
//        const int MAX_EVENTS = CLIENT_MAX_SIZE;
    UINT    nAddr = 0;
//#endif

    //创建ServerSocket对象
    m_pServerSocket = new ServerSocket();
    CHECKF(m_pServerSocket);
    
    // create socket
    BOOL ret = m_pServerSocket->Create();
    if (!ret)
    {
        LOGERROR("[ServerManager::Init] m_pServerSocket creat is Error ret=[%d]", ret);
        goto INIT_FALSE;
    }
    LOGMSG("[ServerManager::Init] m_pServerSocket creat is success");
    
    //设置关闭模式
    ret = m_pServerSocket->SetLinger(1);
    if (!ret)
    {
        LOGERROR("[ServerManager::Init] m_pServerSocket setLinger is Error ret=[%d]", ret);
        goto INIT_FALSE;
    }
    LOGMSG("[ServerManager::Init] m_pServerSocket setLinger is success");

    //设置地址重用模式
    ret = m_pServerSocket->SetReuseAddr(TRUE);
    if (!ret)
    {
        LOGERROR("[ServerManager::Init] m_pServerSocket setReuseAddr is Error ret=[%d]", ret);
        goto INIT_FALSE;
    }
    LOGMSG("[ServerManager::Init] m_pServerSocket setReuseAddr is success");

    //设置端口重用模式
    ret = m_pServerSocket->SetReusePort(TRUE);
    if (!ret)
    {
        LOGERROR("[ServerManager::Init] m_pServerSocket setReusePort is Error ret=[%d]", ret);
        goto INIT_FALSE;
    }
    LOGMSG("[ServerManager::Init] m_pServerSocket setReusePort is success");

    //设置非阻塞模式
    ret = m_pServerSocket->SetNonBlocking(TRUE);
    if (!ret)
    {
        LOGERROR("[ServerManager::Init] m_pServerSocket setNonBlocking is Error ret=[%d]", ret);
        goto INIT_FALSE;
    }
    LOGMSG("[ServerManager::Init] m_pServerSocket setNonBlocking is success");
    
    /*
    //SO_SNDBUF
    ret = setSendBufferSize(g_config.ONLINE_SND_SIZE);
    if (!ret)
    {
        LOGERROR("[ServerManager::Init] socket setSendBufferSize is Error ret=[%d]", ret);
        return FALSE;
    }
    LOGMSG("[ServerManager::Init] m_pServerSocket setSendBufferSize is success SO_SNDBUF=[%u]", g_config.ONLINE_SND_SIZE);
    */
    //!alter end:转移到后面设置。bind之前
    
    // bind address to socket
    //add by huyf 2018.04.03:添加监听地址可配置
    // 域名->IP地址
    nAddr = 0;//inet_addr(g_config.ONLINE_SOCK_LISTEN_ADDR);            // 必须为 UINT, 以便与in_addr兼容
    if(nAddr == INADDR_NONE)
    {
        //如果监听地址没有配置，那么默认监听所有端口
        nAddr = 0;
        //      hostent * hp;
        //      hp = gethostbyname(CONFIG.ONLINE_SOCK_LISTEN_ADDR);
        //      if(hp == 0)
        //          nAddr = 0;
        //      else
        //          nAddr = *(UINT*)(hp->h_addr_list[0]);   // 或 h_addr
    }
	//add end:添加监听地址可配置
    ret = m_pServerSocket->Bind(g_config.ONLINE_PORT, nAddr);
    if (!ret)
    {
        LOGERROR("[ServerManager::Init] m_pServerSocket Bind Is Error ret=[%d] errno[%d] discription[%s]", ret, errno, strerror(errno));
        goto INIT_FALSE;
    }
    //LOGMSG("[ServerManager::Init] m_pServerSocket bind is success Addr=[%s] port=[%d]", g_config.ONLINE_SOCK_LISTEN_ADDR, g_config.ONLINE_PORT); 

    // set listening queue size
    ret = m_pServerSocket->Listen();
    if (!ret)
    {
        LOGERROR("[ServerManager::Init] m_pServerSocket listen Is Error ret=[%d]", ret);
        goto INIT_FALSE;
    }
    LOGMSG("[ServerManager::Init] m_pServerSocket listen Is Success");
    
    //add by huyf 2018.01.24:支持I/O多路复用poll
    //获取socket_id
    //m_SocketID = m_pServerSocket->GetSocketID();
    //CHECKF(m_SocketID != INVALID_SOCKET);
    //add end:支持I/O多路复用poll
    
    //获取到线程id
    m_ThreadID = MyGetCurrentThreadID();

    //add by huyf 2018.01.24:支持I/O多路复用poll
    //更新maxFD 和minFD
    m_MinFD = m_MaxFD = m_pServerSocket->GetSocketID();
    //add end:支持I/O多路复用poll
    
    for (UINT i = 0; i < OVER_MAX_SERVER; ++i)
    {
        m_aServerHash[i] = INVALID_ID;
    }
      
    //add by huyf 2018.01.24:支持I/O多路复用poll
#ifdef _SELECT_
    //将m_SocketID值 和m_ReadFDs、m_ExceptFDs绑定
    FD_SET(m_pServerSocket->GetSocketID(), &m_ReadFDs[SELECT_BAK]);
    FD_SET(m_pServerSocket->GetSocketID(), &m_ExceptFDs[SELECT_BAK]);

    m_Timeout[SELECT_BAK].tv_sec = 0;
    m_Timeout[SELECT_BAK].tv_usec = 0; 
#endif
        
#ifdef _POLL_     
    m_ClientSet[0].fd = m_pServerSocket->GetSocketID();   
    m_ClientSet[0].events = POLLIN | POLLOUT | POLLERR;  
    m_pServerSocket->GetSocket()->SetClientPollFd(&m_ClientSet[0]);
    m_MaxIndex = 0;       
#endif	  
        //add end:支持I/O多路复用poll
        
#ifdef _EPOLL_
    // 创建一个epoll句柄          
    m_nEpollFd = m_pServerSocket->EpollCreate(CLIENT_MAX_SIZE/*MAX_EVENTS*/);  
    if(m_nEpollFd == INVALID_ID)  
    {  
        LOGERROR("[ServerManager::Init] EpollCreate failed nEpollFd=[%d]", INVALID_ID);
        goto INIT_FALSE;
    }  
    LOGMSG("[ServerManager::Init] EpollCreate success nEpollFd=[%d]", m_nEpollFd);        
    /*
    EPOLLIN:1
    EPOLLOUT:4
    EPOLLRDHUP:8192
    EPOLLPRI:2
    EPOLLERR:8
    EPOLLHUP:16
    EPOLLET:-2147483648
    EPOLLONESHOT:1073741824 
    */
    m_EpollEvents[0].data.fd = m_pServerSocket->GetSocketID();       // 设置与要处理的事件相关的文件描述符
    //m_EpollEvents[0].events = EPOLLIN | EPOLLOUT | EPOLLERR;              // 设置要处理的事件类型
    m_EpollEvents[0].events = EPOLLIN|EPOLLOUT|EPOLLERR|EPOLLHUP|EPOLLRDHUP;             // 设置要处理的事件类型
    LOGMSG("[ServerManager::Init] ServerSocketID=[%d] events=[%d]", m_EpollEvents[0].data.fd, m_EpollEvents[0].events);

    // 向epoll注册server_sockfd监听事件  
    //asmlinkage long sys_epoll_ctl(int epfd,int op,int fd,struct epoll_event __user *event)----->case EPOLL_CTL_ADD:  //增加监听一个fd-------->epds.events |= EPOLLERR | POLLHUP;     //默认包含POLLERR和POLLHUP事件
    //重复添加（在ep的红黑树中查找已经存在这个fd）。error = -EEXIST;
    if(m_pServerSocket->EpollCtl(m_nEpollFd, EPOLL_CTL_ADD, m_pServerSocket->GetSocketID(), &m_EpollEvents[0]) == -1)  
    {  
        LOGERROR("epollCtl::server_sockfd register failed...");  
        goto INIT_FALSE;
    }  
    LOGMSG("[ServerManager::Init] EpollCtl Is Success...");
#endif  
    return TRUE;
        
INIT_FALSE:
    m_pServerSocket->Close();
    LOGERROR("[ServerManager::Init()] epollCtl::server_sockfd register failed");  
    return FALSE;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	网络监听接口
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
#ifdef _SELECT_
INT ServerManager::Select()
{
	DEBUG_TRY
    INT nReadyNum = INVALID_ID;
    m_Timeout[SELECT_USE].tv_sec = m_Timeout[SELECT_BAK].tv_sec;
    m_Timeout[SELECT_USE].tv_usec = m_Timeout[SELECT_BAK].tv_usec;

    m_ReadFDs[SELECT_USE] = m_ReadFDs[SELECT_BAK];
    m_WriteFDs[SELECT_USE] = m_WriteFDs[SELECT_BAK];
    m_ExceptFDs[SELECT_USE] = m_ExceptFDs[SELECT_BAK];

    nReadyNum = SocketAPI::select_ex((int)m_MaxFD+1, &m_ReadFDs[SELECT_USE], &m_WriteFDs[SELECT_USE], &m_ExceptFDs[SELECT_USE], &m_Timeout[SELECT_USE]);
    if (nReadyNum == INVALID_ID)
    {
        LOGERROR("[ServerManager::Select] Error");
    }
    else if(nReadyNum == 0)
    {
        //...
        //LOGERROR("[ServerManager::select_ex] Timeout...nReadyNum == 0"); 
    }
    else //if(nReadyNum > 0)
    {
        //...
    }
    return nReadyNum;
    DEBUG_CATCH0("ServerManager::Select")
}
#endif

//////////////////////////////////////////////////////////////////////
//
// func description	:	网络监听接口poll
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
//add by huyf 2018.01.24:支持I/O多路复用poll
#ifdef _POLL_
INT ServerManager::Poll()
{
	DEBUG_TRY
    INT nReadyNum = INVALID_ID;
    nReadyNum = SocketAPI::poll_ex(m_ClientSet, m_MaxIndex + 1 , 0);//0：非阻塞启动
    if (nReadyNum == INVALID_ID)
    {
        LOGERROR("[ServerManager::poll_ex] Error");            
    }
    else if(nReadyNum == 0)
    {
        //...
        //LOGERROR("[ServerManager::poll_ex] Timeout...nReadyNum == 0"); 
    }
    else //if(nReadyNum > 0)
    {
        //...
    }
    return nReadyNum;	  
    
	DEBUG_CATCH0("ServerManager::Poll")
}
#endif
//add end:支持I/O多路复用poll

//////////////////////////////////////////////////////////////////////
//
// func description	:	网络监听接口epoll
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
//add by huyf 2018.01.24:支持I/O多路复用epoll
#ifdef _EPOLL_  
INT ServerManager::EPoll()
{
    DEBUG_TRY
    INT nReadyNum = INVALID_ID;
    nReadyNum = SocketAPI::epoll_wait_ex(m_nEpollFd, m_EffectiveEpollEvents, m_MaxEpollIndex+1, 0);//0：非阻塞启动
    if (nReadyNum == INVALID_ID)
    {
        LOGERROR("[ServerManager::epoll_wait_ex] Error");            
    }
    else if(nReadyNum == 0)
    {
        //...
        //LOGERROR("[ServerManager::epoll_wait_ex] Timeout...nReadyNum == 0"); 
    }
    else //if(nReadyNum > 0)
    {
        //...
    }
    return nReadyNum;
    DEBUG_CATCH0("ServerManager::EPoll")
}
#endif
//add end:支持I/O多路复用epoll
//////////////////////////////////////////////////////////////////////
//
// func description	:	新连接接收处理
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ServerManager::AcceptNewConnection()
{
	DEBUG_TRY   
    bool bFind = false;      
    if(IsAccept(m_pServerSocket->GetSocket()))
    {
        //LOGDEBUG("------------ServerManager::AcceptNewConnection-----------\n");
        int nStep = 0;
        BOOL ret = FALSE;
        INT nErr = 0;
        //int fd = INVALID_SOCKET;

        //从玩家池中抓取一块player
        ServerPlayer* client = g_pPlayerPool->NewPlayer();
        //!alter by huyf 2018.02.07:达到最大在线人数的时候直接返回，不再accept新的socket连接
        if(!client)
        {
            return FALSE;
        } 
        //!alter end:达到最大在线人数的时候直接返回，不再accept新的socket连接
        Socket* pSocket = client->GetSocket();
        CHECKF(pSocket); 

        //清理下
        //client->CleanUp();
        
        //接受客户端接入Socket句柄
        INT nAcceptSocket = m_pServerSocket->Accept(pSocket);
        if(nAcceptSocket == INVALID_SOCKET)
        {
            nStep = 15;
            goto EXCEPTION;
        }
        else if(nAcceptSocket == 0)
        {
            nStep = 16;
            goto ACCEPT_CONTINUE;
        } 

        //add by huyf 2018.02.06:判断是否超过接收前端的最大上限
        if(IsFullConnect())
        {
#ifdef _SELECT_
            LOGERROR("[ServerManager::AcceptNewConnection] Is Error:已经超出能够检测的网络句柄最大数(Too Many Clients) m_iFDSize=[%d] CLIENT_MAX_SIZE=[%d]", m_iFDSize, CLIENT_MAX_SIZE);         //说明连接的客户端已经达到上限  
#elif _POLL_
            LOGERROR("[ServerManager::AcceptNewConnection] Is Error:已经超出能够检测的网络句柄最大数(Too Many Clients) m_MaxIndex=[%d] CLIENT_MAX_SIZE=[%d]", m_MaxIndex, CLIENT_MAX_SIZE);         //说明连接的客户端已经达到上限  
#elif _EPOLL_  
            LOGERROR("[ServerManager::AcceptNewConnection] Is Error:已经超出能够检测的网络句柄最大数(Too Many Clients) m_MaxEpollIndex=[%d] CLIENT_MAX_SIZE=[%d]", m_MaxEpollIndex, CLIENT_MAX_SIZE);         //说明连接的客户端已经达到上限  
#endif
            nStep = 150;
            goto EXCEPTION;
        }
        //add end:判断是否超过接收前端的最大上限

        //把客户端的socket设置为非阻塞方式
        ret = pSocket->setNonBlocking(TRUE);
        if(!ret)
        {
            nStep = 17;
            goto EXCEPTION;
        }
        nErr = pSocket->getSocketError();
        if(nErr)
        {
            nStep = 18;
            LOGERROR("[ServerManager::AcceptNewConnection] nAcceptSocket=[%d] nStep[%d], Error[%u]",nAcceptSocket, nStep, nErr);
            goto EXCEPTION;
        }

        if(!pSocket->setLinger(0))
        {
            nStep = 19;
            goto EXCEPTION;
        }

        //初始化client
        client->Init();
        
        //设置当前客户端连接的状态
        client->SetPlayerStatus(PS_WORLD_CONNECT);

        //add by huyf 2018.10.19:添加websocket的对接
        if (pSocket->GetWebSocketHandler())
        {
            pSocket->GetWebSocketHandler()->SetSocket(nAcceptSocket);
            pSocket->GetWebSocketHandler()->SetBuff(client->m_pSocketInputStream->m_Buffer);
        }
        //add end:添加websocket的对接

        nStep = 80;		
        ret = AddPlayer(client);
        //char szBuff[1024];
        //memset(szBuff, 0, 1024);
        //SafeCopy(szBuff, (const char*)&(client->QueryData()->GetInfo()), sizeof(PlayerInfoStruct));
        //LOGDEBUG("[ServerManager::AcceptNewConnection] client info...m_UserData=[%s]", szBuff);
        
        if(!ret)
        {
            nStep = 20;
            goto EXCEPTION;
        }

        nStep += 100;   

#ifdef _SELECT_
        LOGDEBUG("[ServerManager::AcceptNewConnection] Is OK... Ip=[%s] port=[%d] nAcceptSocket=[%d] Amount=[%d]",pSocket->m_Host, pSocket->m_Port, nAcceptSocket, GetPlayerAmount());
#endif
#ifdef _POLL_
       //add by huyf 2018.01.24:支持I/O多路复用poll
       //如果是监听套接口发生了可读事件
       for (int i=1; i<=CLIENT_MAX_SIZE; ++i)   
       {   
            //LOGDEBUG("[ServerManager::AcceptNewConnection] m_ClientSet[%d].fd=[%d] ServerSocketID=[%d]", i, m_ClientSet[i].fd, m_pServerSocket->GetSocket()->getSocketID()); 
           //查找空闲节点
            if (m_ClientSet[i].fd < 0)   
            {   
                //找到进行设置
                m_ClientSet[i].fd = nAcceptSocket; 
                LOGDEBUG("[ServerManager::AcceptNewConnection] m_ClientSet[%d].fd=[-1===>%d]", i, nAcceptSocket);
                m_ClientSet[i].events = POLLIN | POLLOUT | POLLERR;
                pSocket->SetClientPollFd(&m_ClientSet[i]);
                LOGDEBUG("[ServerManager::AcceptNewConnection] Is OK... Index=[%d] nAcceptSocket=[%d] events=[%d] pSocket->getSocketID=[%d]", i, m_ClientSet[i].fd, m_ClientSet[i].events, pSocket->getSocketID());  

                //更新最大节点
                if (i > m_MaxIndex)
                {
                    LOGDEBUG("[ServerManager::AcceptNewConnection] Is OK... [Index > m_MaxIndex] Index=[%d] m_MaxIndex=[%d===>%d]", i, m_MaxIndex, i);
                    m_MaxIndex = i;
                }
                LOGDEBUG("[ServerManager::AcceptNewConnection] Is OK... TotalOL=[%d] Ip=[%s] Port=[%d] nAcceptSocket=[%d] CurIndex=[%d] m_MaxEpollIndex=[%d] CLIENT_MAX_SIZE=[%d] events=[%d]", 
                    GetPlayerAmount(), pSocket->m_Host, pSocket->m_Port, nAcceptSocket, i, m_MaxIndex, CLIENT_MAX_SIZE, m_ClientSet[i].events); 
                bFind = true;
                break; 
            }
            //LOGDEBUG("[ServerManager::AcceptNewConnection] m_ClientSet[%d].fd=[%d > 0] CLIENT_MAX_SIZE=[%d]", i, m_ClientSet[i].fd, CLIENT_MAX_SIZE);   
        }
        if(!bFind)
        {
            LOGERROR("[ServerManager::AcceptNewConnection] No Found All Fds Of m_ClientSet > 0 And CLIENT_MAX_SIZE=[%d]", CLIENT_MAX_SIZE);  
        }
        //add end:支持I/O多路复用poll
#endif         
            
#ifdef _EPOLL_  
       //add by huyf 2018.01.24:支持I/O多路复用poll
       //如果是监听套接口发生了可读事件
       for (int i=1; i<=CLIENT_MAX_SIZE; ++i)   
       {   
            //LOGDEBUG("[ServerManager::AcceptNewConnection] m_EpollEvents[%d].data.fd=[%d] ServerSocketID=[%d]", i, m_EpollEvents[i].data.fd, m_pServerSocket->GetSocket()->getSocketID());  
           //查找空闲节点
            if (m_EpollEvents[i].data.fd < 0)   
            {   
                //找到进行设置
                m_EpollEvents[i].data.fd = nAcceptSocket; 
                LOGDEBUG("[ServerManager::AcceptNewConnection] m_EpollEvents[%d].data.fd=[-1===>%d]", i, nAcceptSocket);
                m_EpollEvents[i].events = EPOLLIN | EPOLLOUT | EPOLLERR;
                if(m_pServerSocket->EpollCtl(m_nEpollFd, EPOLL_CTL_ADD, nAcceptSocket, &m_EpollEvents[i]) == -1)  
                {  
                    LOGERROR("[ServerManager::AcceptNewConnection] EpollCtl:nAcceptSocket register failed");  
                    goto EXCEPTION_ADN_DEL_PALYER;
                } 
                pSocket->SetClientEPollEvent(&m_EpollEvents[i]);                    
                
                LOGDEBUG("[ServerManager::AcceptNewConnection] Is OK... Index=[%d] nAcceptSocket=[%d] events=[%d] EPOLL_CTL_ADD ===> m_nEpollFd=[%d]", i, m_EpollEvents[i].data.fd, m_EpollEvents[i].events, m_nEpollFd);  
                //更新最大节点
                if (i > m_MaxEpollIndex)
                {
                    LOGDEBUG("[ServerManager::AcceptNewConnection] Is OK... [Index > m_MaxEpollIndex] Index=[%d] m_MaxEpollIndex=[%d===>%d]", i, m_MaxEpollIndex, i);
                    m_MaxEpollIndex = i;                         
                }
                LOGDEBUG("[ServerManager::AcceptNewConnection] Is OK... TotalOL=[%d] Ip=[%s] Port=[%d] nAcceptSocket=[%d] CurIndex=[%d] m_MaxEpollIndex=[%d] CLIENT_MAX_SIZE=[%d] events=[%d]", 
                    GetPlayerAmount(), pSocket->m_Host, pSocket->m_Port, nAcceptSocket, i, m_MaxEpollIndex, CLIENT_MAX_SIZE, m_EpollEvents[i].events);  
                bFind = true;
                break; 
            }
            //LOGDEBUG("[ServerManager::AcceptNewConnection] m_EpollEvents[%d].data.fd=[%d > 0] CLIENT_MAX_SIZE=[%d]", i, m_EpollEvents[i].data.fd, CLIENT_MAX_SIZE);   
        }
        if(!bFind)
        {
            LOGERROR("[ServerManager::AcceptNewConnection] No Found All Fds Of m_EpollEvents > 0 And CLIENT_MAX_SIZE=[%d]", CLIENT_MAX_SIZE); 
        }
        //add end:支持I/O多路复用poll
#endif
        LOGDEBUG("[ServerManager::AcceptNewConnection] Is OK...");
        return TRUE;            

EXCEPTION:
        LOGERROR("[ServerManager::AcceptNewConnection] ERROR nStep[%d]", nStep);
        g_pPlayerPool->DelPlayer(client->GetPlayerID());
        client->CleanUp();
        return FALSE;

EXCEPTION_ADN_DEL_PALYER:
        LOGDEBUG("[ServerManager::AcceptNewConnection] ERROR nStep[%d]", nStep);
        g_pPlayerPool->DelPlayer(client->GetPlayerID());
        client->CleanUp();
        LOGDEBUG("[ServerManager::AcceptNewConnection] g_pPlayerPool->DelPlayer");            
        return FALSE;
            
ACCEPT_CONTINUE:
        LOGWARNING("[ServerManager::AcceptNewConnection] is timeout socket I/O model is noblock again accept");
        return TRUE;
    }
    return TRUE;
	DEBUG_CATCHF("ServerManager::AcceptNewConnection")
}
//////////////////////////////////////////////////////////////////////
//
// func description :   判断连接是否已满
//
//-----input------
// Parameters
//      无
//------output------
// Return 
//      无
BOOL ServerManager::IsFullConnect()
{
#ifdef _SELECT_ 
    return (BOOL)(m_iFDSize >= _SETSIZE_);
#endif
#ifdef _POLL_
    return (BOOL)(m_MaxIndex >= CLIENT_MAX_SIZE);
#endif
#ifdef _EPOLL_
    return (BOOL)(m_MaxEpollIndex >= CLIENT_MAX_SIZE);
#endif
}
//////////////////////////////////////////////////////////////////////
//
// func description	:	将Player数据加入系统中
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ServerManager::AddPlayer(Player* pPlayer)
{
	DEBUG_TRY			
    //!alter by huyf 2018.02.06:统一调整        
    //if(m_iFDSize >= _SETSIZE_)
    //{//已经超出能够检测的网络句柄最大数；
    //        LOGERROR("[ServerManager::AddPlayer] Is Error:已经超出能够检测的网络句柄最大数,m_iFDSize=[%d] _SETSIZE_=[%d]", m_iFDSize, _SETSIZE_);
    //
    //        return FALSE;
    //} 
    //!alter end:统一调整

    //将玩家添加到playerManager
    BOOL ret = PlayerManager::AddPlayer(pPlayer);
    if(!ret)
    {
            LOGERROR("[ServerManager::AddPlayer] PlayerManager::AddPlayer is Error");
            return FALSE;
    }
    LOGDEBUG("[ServerManager::AddPlayer] Is Ok... TotalOnline=[%d] Ip=[%s] Port=[%u] SocketId=[%u] AccountID=[%d] PlayerId=[%d]", GetPlayerAmount(), pPlayer->GetSocket()->m_Host,pPlayer->GetSocket()->m_Port,pPlayer->GetSocket()->getSocketID(), pPlayer->GetAccountID(),pPlayer->GetPlayerID());

    SOCKET fd = pPlayer->GetSocket()->getSocketID();
    CHECKF(fd != INVALID_SOCKET);

    m_MinFD = min(fd , m_MinFD);
    m_MaxFD = max(fd , m_MaxFD);
#ifdef _SELECT_ 
    //将fd和FDs进行绑定
    FD_SET(fd , &m_ReadFDs[SELECT_BAK]);
    FD_SET(fd , &m_WriteFDs[SELECT_BAK]);
    FD_SET(fd , &m_ExceptFDs[SELECT_BAK]);
#endif
    //增加size
    m_iFDSize++;	
	DEBUG_CATCH("ServerManager::AddPlayer")
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	将拥有fd句柄的玩家数据从当前系统中清除
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ServerManager::DelPlayer(Player* pPlayer)
{
	DEBUG_TRY	        
    bool bFind = false;
    ServerPlayer* pServerPlayer = (ServerPlayer*)pPlayer;
    CHECKF(pServerPlayer);
    Socket* pSocket = pServerPlayer->GetSocket();
    CHECKF(pSocket);

    SOCKET fd = pSocket->getSocketID();

    CHECKF(m_MinFD != INVALID_SOCKET);
    CHECKF(m_MaxFD != INVALID_SOCKET);
    if(fd == INVALID_SOCKET) 
    {
        return FALSE;
    }

    if(fd == m_MinFD) 
    {
        SOCKET s = m_MaxFD;
        UINT nPlayerCount = GetPlayerAmount();
        for(UINT i = 0; i < nPlayerCount; i++)
        {
            if(m_nPlayerIDS[i] == INVALID_ID)
                continue;

            ServerPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_nPlayerIDS[i]);				
            if(pPlayer == NULL)
                continue;

            SOCKET temp = pPlayer->GetSocket()->getSocketID();
            if(temp == fd)
                continue;
            if(temp == INVALID_SOCKET)
                continue;

            if(temp < s)
            {
                s = temp;
            }
        }

        if(m_MinFD == m_MaxFD)
        {
             m_MinFD = m_MaxFD = INVALID_SOCKET;
        }
        else
        {
            //add by huyf 2018.01.24:支持I/O多路复用poll
            if(s > m_pServerSocket->GetSocketID())
            {
                m_MinFD = m_pServerSocket->GetSocketID();
            }
            //add end:支持I/O多路复用poll
            else
            {
                m_MinFD = s;
            }
        }
    } 
    else if(fd == m_MaxFD) 
    {
        SOCKET s = m_MinFD;
        UINT nPlayerCount = GetPlayerAmount();
        for(UINT i=0; i<nPlayerCount; i++)
        {
            if(m_nPlayerIDS[i] == INVALID_ID)
                continue;

            ServerPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_nPlayerIDS[i]);
            if(pPlayer == NULL)
                continue;

            SOCKET temp = pPlayer->GetSocket()->getSocketID();
            if(temp == fd)
                continue;
            if(temp == INVALID_SOCKET)
                continue;

            if(temp > s)
            {
                s = temp;
            }
        }

        if(m_MaxFD == m_MinFD)
        {
            m_MinFD = m_MaxFD = INVALID_SOCKET;
        }
        else
        {
            //add by huyf 2018.01.24:支持I/O多路复用poll                        
            if(s < m_pServerSocket->GetSocketID())
            {
                m_MaxFD = m_pServerSocket->GetSocketID();
            }
            //add end:支持I/O多路复用poll                       
            else
            {
                m_MaxFD = s;
            }
        }
    }
    
    //add by huyf 2018.01.24:支持I/O多路复用poll
#ifdef _SELECT_ 
    FD_CLR(fd , &m_ReadFDs[SELECT_BAK]);
    FD_CLR(fd , &m_ReadFDs[SELECT_USE]);
    FD_CLR(fd , &m_WriteFDs[SELECT_BAK]);
    FD_CLR(fd , &m_WriteFDs[SELECT_USE]);
    FD_CLR(fd , &m_ExceptFDs[SELECT_BAK]);
    FD_CLR(fd , &m_ExceptFDs[SELECT_USE]);
#endif 

    int index = 1;
    Player* pMaxPlayer = NULL;
    Socket* pMaxSocket = NULL;
#ifdef _POLL_    
    struct pollfd* pfd = pSocket->GetClientPollFd();
    CHECKF(pfd);        
    pMaxPlayer = GetPlayerBySocket(m_ClientSet[m_MaxIndex].fd);
    if(pMaxPlayer)
    {
        pMaxSocket = pMaxPlayer->GetSocket();
        CHECKF(pMaxSocket);
        for(; index<=CLIENT_MAX_SIZE; ++index)
        {
            if(pfd->fd == m_ClientSet[index].fd)
            {       
                LOGDEBUG("[ServerManager::DelPlayer] m_ClientSet[%d].fd=[%d===>%d] m_ClientSet[%d].fd=[%d===>%d]", index, m_ClientSet[index].fd, m_ClientSet[m_MaxIndex].fd,m_MaxIndex,m_ClientSet[m_MaxIndex].fd,INVALID_SOCKET);             
                //数据替换
                m_ClientSet[index].fd = m_ClientSet[m_MaxIndex].fd;
                m_ClientSet[index].events = m_ClientSet[m_MaxIndex].events;
                m_ClientSet[m_MaxIndex].fd = INVALID_SOCKET;
                m_ClientSet[m_MaxIndex].events = 0;

                //更新最后一个节点的ClientFd的指针，更新为删除的节点地方
                pSocket->SetClientPollFd(&m_ClientSet[m_MaxIndex]);
                pMaxSocket->SetClientPollFd(&m_ClientSet[index]);

                bFind = true;
                LOGDEBUG("[ServerManager::DelPlayer] Is Found m_MaxIndex=[%d] m_ClientSet[%d].fd=[%d]", m_MaxIndex, m_MaxIndex, INVALID_SOCKET);
                break;
            }
            LOGDEBUG("[ServerManager::DelPlayer] Index=[%d] CLIENT_MAX_SIZE=[%d] pfd->fd=[%d] m_ClientSet[index].fd=[%d]", index, CLIENT_MAX_SIZE, pfd->fd, m_ClientSet[index].fd);
        }
        //尾部节点-1
        m_MaxIndex--;
        LOGDEBUG("[ServerManager::DelPlayer] m_MaxIndex=[%d]", m_MaxIndex);
    }
    if(!bFind)
    {
           LOGERROR("[ServerManager::DelPlayer] Not Fount GetPlayerBySocket() CLIENT_MAX_SIZE=[%d]", CLIENT_MAX_SIZE); 
    }            
#endif
    //add end:支持I/O多路服用select、poll、epoll
        
    //add by huyf 2018.01.24:支持I/O多路复用epoll
#ifdef _EPOLL_
    //备注:可以直接取pSocket中的fd    
    struct epoll_event* pEpollEvent = pSocket->GetClientEPollEvent();
    LOGDEBUG("[ServerManager::DelPlayer] del_pPlayer.fd=[%d] del_pEpollEvent.data.fd=[%d]", fd, pEpollEvent->data.fd);
    CHECKF(pEpollEvent);
    pMaxPlayer = GetPlayerBySocket(m_EpollEvents[m_MaxEpollIndex].data.fd);
    LOGDEBUG("[ServerManager::DelPlayer] m_EpollEvents[m_MaxEpollIndex=%d].data.fd=[%d]", m_MaxEpollIndex, m_EpollEvents[m_MaxEpollIndex].data.fd);
    if(pMaxPlayer)
    {
        pMaxSocket = pMaxPlayer->GetSocket();
        CHECKF(pMaxSocket);
        for(; index<=CLIENT_MAX_SIZE; ++index)
        {
            LOGDEBUG("[ServerManager::DelPlayer] pEpollEvent->data.fd=[%d] m_EpollEvents[%d].data.fd=[%d]", pEpollEvent->data.fd, index,m_EpollEvents[index].data.fd);
            if(pEpollEvent->data.fd == m_EpollEvents[index].data.fd)
            {
                LOGDEBUG("[ServerManager::DelPlayer] m_EpollEvents[%d].data.fd=[%d===>%d] m_EpollEvents[%d].data.fd=[%d===>%d]", index, m_EpollEvents[index].data.fd, m_EpollEvents[m_MaxEpollIndex].data.fd,m_MaxEpollIndex,m_EpollEvents[m_MaxEpollIndex].data.fd,INVALID_SOCKET);

                //数据替换
                m_EpollEvents[index].data.fd = m_EpollEvents[m_MaxEpollIndex].data.fd;
                m_EpollEvents[index].events = m_EpollEvents[m_MaxEpollIndex].events;
                m_EpollEvents[m_MaxEpollIndex].data.fd = INVALID_SOCKET;
                m_EpollEvents[m_MaxEpollIndex].events = 0;

                //更新最后一个节点的ClientFd的指针，更新为删除的节点地方
                pSocket->SetClientEPollEvent(&m_EpollEvents[m_MaxEpollIndex]);
                pMaxSocket->SetClientEPollEvent(&m_EpollEvents[index]);                    

                bFind = true;
                LOGDEBUG("[ServerManager::DelPlayer] Is Found m_MaxEpollIndex=[%d] m_EpollEvents[%d].data.fd=[%d]", m_MaxEpollIndex, m_MaxEpollIndex, INVALID_SOCKET);

                break;
            }
        }
        //尾部节点-1
        m_MaxEpollIndex--;
        LOGDEBUG("[ServerManager::DelPlayer] m_MaxEpollIndex=[%d]", m_MaxEpollIndex);
    }
    if(!bFind)
    {
        LOGERROR("[ServerManager::DelPlayer] Not Fount GetPlayerBySocket() CLIENT_MAX_SIZE=[%d]", CLIENT_MAX_SIZE); 
    }
        
#endif
    //add end:支持I/O多路复用epoll

    m_iFDSize--;
    CHECKF(m_iFDSize >= 0); 
    PlayerManager::RemovePlayer(pServerPlayer->GetPlayerID()); 

#ifdef _POLL_         
    LOGDEBUG("[ServerManager::DelPlayer] Total Online=[%d] m_MaxIndex=[%d] Ip=[%s] Port=[%u] SocketId=[%u] AccountId=[%u] PlayerId=[%u]", GetPlayerAmount(),m_MaxIndex,pServerPlayer->GetSocket()->m_Host,pServerPlayer->GetSocket()->m_Port,pServerPlayer->GetSocket()->getSocketID(),pServerPlayer->GetAccountID(),pServerPlayer->GetPlayerID());
#else
    #ifdef _EPOLL_
        LOGDEBUG("[ServerManager::DelPlayer] Total Online=[%d] m_MaxEpollIndex=[%d] Ip=[%s] Port=[%u] SocketId=[%u] AccountId=[%u] PlayerId=[%u]", GetPlayerAmount(),m_MaxEpollIndex,pServerPlayer->GetSocket()->m_Host,pServerPlayer->GetSocket()->m_Port,pServerPlayer->GetSocket()->getSocketID(),pServerPlayer->GetAccountID(),pServerPlayer->GetPlayerID());
    #else
        LOGDEBUG("[ServerManager::DelPlayer] Total Online=[%d] Ip=[%s] Port=[%u] SocketId=[%u] AccountId=[%u] PlayerId=[%u]", GetPlayerAmount(),pServerPlayer->GetSocket()->m_Host,pServerPlayer->GetSocket()->m_Port,pServerPlayer->GetSocket()->getSocketID(),pServerPlayer->GetAccountID(),pServerPlayer->GetPlayerID());
    #endif
#endif

// 注意：
// 在删除事件   epoll_ctl(kdpfd, EPOLL_CTL_DEL, events[n].data.fd,&ev);之前 调用close()关闭socket描述符，
// 就会导致删除事件时epoll_ctl()返回值小于0，并且提示epoll failed: Bad file descriptor(9)
#ifdef _EPOLL_
    //struct epoll_event ev;
    //ev.events = EPOLLIN | EPOLLOUT | EPOLLERR;
    //ev.data.fd = fd;
    m_pServerSocket->EpollCtl(m_nEpollFd, EPOLL_CTL_DEL, fd, NULL/*&ev*/);
    LOGDEBUG("[ServerManager::DelPlayer] From m_nEpollFd=[%d] Del fd=[%d] EPOLL_CTL_ADD ===> EPOLL_CTL_DEL", m_nEpollFd, fd);
#endif    

    //清理掉网络数据,cleanup会重置m_PID和ManagerID = INVALID_ID
    pServerPlayer->SetDisconnect(TRUE);         
    g_pPlayerPool->DelPlayer(pServerPlayer->GetPlayerID());
    //清理玩家数据和网络数据
    pServerPlayer->CleanUp();
    
    DEBUG_CATCH("ServerManager::DelPlayer")
    return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	出现异常后将Player信息清除，并将系统中的信息也清除
//						断开玩家的连接
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ServerManager::RemovePlayer(Player* pPlayer)
{
    BOOL ret = FALSE;
    CHECKF(pPlayer);
        
	DEBUG_TRY
    //第一步：清除PlayerManager中的信息
    LOGDEBUG("[ServerManager::RemovePlayer] accountID[%d], robotID[%s]", pPlayer->GetAccountID(), pPlayer->GetRobot()->getRobotId());
    
    //从大乱斗队伍中删除
    CBattleManager* pBm = g_pBattleManager;
    CHECKF(pBm);
    CBattle* pBattle = pBm->GetBattle();
    CHECKF(pBattle);
    //大乱斗-单人队列中删除
    CBattleConfusionSingle* pBcs = pPlayer->GetBcs();
    if (pBcs)
    {
        pBattle->DelBcs(pPlayer);
        pPlayer->ClearBcs();
    }
    //大乱斗-队伍队列中删除
    CBattleTeam* pBt = pPlayer->GetBct();
    if (pBt)
    {
        if(pBt->DelMember(pPlayer))
        {
            LOGDEBUG("[ServerManager::RemovePlayer]-2");
            pPlayer->ClearBct();

            //玩家下线后，清空仇人保持的id
            Player* pBeKiller = pPlayer->GetBeKiller();
            if (pBeKiller)
            {
                pBeKiller->SetEnemyId(0, FALSE);
            }

            //如果战斗已经开始则，删除BUFF效果
            if((pBt->GetTeamStatus() != _TEAM_STATUS_QIUCK_MATCH && pBt->GetTeamStatus() != _TEAM_STATUS_CREATE) 
                || pBt->GetTimeStamp() + BattleCfg()->GetBccMaxTime() < time(NULL))
            {
                pPlayer->SetAddGold(0, FALSE);                     //战斗结束后奖励的金币，金币加成200%
                pPlayer->SetAddScore(0, FALSE);                    //积分基础加:3000
            }

            //广播离开队伍信息给玩家
            CMsgSynRoomInfo msg;
            IF_OK(msg.CreateMsg(pPlayer, _SYNROOMINFO_ACTION_DEL_MEMBER, pBt->GetTeamId(), pBt->GetFightTeamColour()))
            {
                pBt->BroadcastTeamMsg(&msg);
            }
            
            //!alter by huyf 2018.11.01:此处0个人应该不会触发
            //如果队伍成员人数=1 && 战斗状态，则从Battle中删除此队伍，直接上面对一个人的时候进行处理
            if ((pBt->GetTeamStatus() == _TEAM_STATUS_CREATE || pBt->GetTeamStatus() == _TEAM_STATUS_QIUCK_MATCH) && pBt->GetMemberAmount() == 0)
            {
                pBattle->DelTeamFromBct(pBt);
            }
            //战斗未开始
            else if ((pBt->GetTeamStatus() == _TEAM_STATUS_CREATE || pBt->GetTeamStatus() == _TEAM_STATUS_QIUCK_MATCH) && pBt->GetMemberAmount() > 0)
            {
                //如果队伍人数不为0
                //如果是队长则进行队长id变更广播
                if(pBt->GetTeamLeaderID() == pPlayer->GetUid())
                {
                    Player* pNewLeader = pBt->GetMemberByIndex(0);
                    if (pNewLeader)
                    {
                        pBt->SetTeamLeaderID(pNewLeader->GetUid());
                        CMsgBattle msg;
                        IF_OK(msg.CreateMsg(_BATTLE_ACTION_CHANGE_TEAM_LEADER_SUC, 0, pBt->GetTeamId(), pNewLeader->GetUid()))
                        {
                            pBt->BroadcastTeamMsg(&msg);
                        }
                    }
                    else
                    {
                        LOGERROR("[ServerManager::RemovePlayer] _BATTLE_ACTION_LEAVE_TEAM Is Error GetMemberByIndex[0]==NULL MemberAmount=[%d]",pBt->GetMemberAmount());
                    }                   
                } 
            }
            //战斗开始
            else if (pBt->GetTeamStatus() == _TEAM_STATUS_START && pBt->GetMemberAmount() == 1)
            {
                //发放奖励
                pBt->Award();
                
                //发送战斗结束排行榜数据包
                pBt->BroadcastConfusionEndMsgToMember();

                //清理队伍成员
                pBt->ClearMember();

                //删除队伍，释放内存
                pBattle->DelTeamFromBct(pBt);
            }
            //!alter end:此处0个人应该不会触发
        }
              
    }
    //红蓝对战-队伍队列中删除
    pBt = pPlayer->GetBrbt();
    if (pBt)
    {
        CBattleTeam* pEnemyBrbt = pBt->GetEnemyTeam();

        if(pBt->DelMember(pPlayer))
        {
            LOGDEBUG("[ServerManager::RemovePlayer] DelMember ");
            pPlayer->ClearBct();

            //广播离开队伍信息给玩家
            CMsgSynRoomInfo msg;
            IF_OK(msg.CreateMsg(pPlayer, _SYNROOMINFO_ACTION_DEL_MEMBER, pBt->GetTeamId(), pBt->GetFightTeamColour()))
            {
                pBt->BroadcastTeamMsg(&msg);

                //找出对战队伍指针
                if(pEnemyBrbt)
                {
                     pEnemyBrbt->BroadcastTeamMsg(&msg);
                }
            }
            
            LOGDEBUG("[ServerManager::RemovePlayer] m_pBrbt.size=[%d]\n",  pBt->GetMemberAmount());

            //如果队伍成员人数=0，则从Battle中删除此队伍-[_TEAM_STATUS_CREATE, _TEAM_STATUS_QIUCK_MATCH]
            if ((pBt->GetTeamStatus() == _TEAM_STATUS_CREATE || pBt->GetTeamStatus() == _TEAM_STATUS_QIUCK_MATCH) && pBt->GetMemberAmount() == 0)
            {
                pBattle->DelTeamFromBrbt(pBt);
            }
            //战斗未开始
            else if ((pBt->GetTeamStatus() == _TEAM_STATUS_CREATE || pBt->GetTeamStatus() == _TEAM_STATUS_QIUCK_MATCH) && pBt->GetMemberAmount() > 0)
            {
                //如果队伍人数不为0
                //如果是队长则进行队长id变更广播
                if(pBt->GetTeamLeaderID() == pPlayer->GetUid())
                {
                    Player* pNewLeader = pBt->GetMemberByIndex(0);
                    if (pNewLeader)
                    {
                        pBt->SetTeamLeaderID(pNewLeader->GetUid());
                        CMsgBattle msg;
                        IF_OK(msg.CreateMsg(_BATTLE_ACTION_CHANGE_TEAM_LEADER_SUC, 0, pBt->GetTeamId(), pNewLeader->GetUid()))
                        {
                            pBt->BroadcastTeamMsg(&msg);
                        }
                    }
                    else
                    {
                        LOGERROR("[ServerManager::RemovePlayer] _BATTLE_ACTION_LEAVE_TEAM Is Error GetMemberByIndex[0]==NULL MemberAmount=[%d]",pBt->GetMemberAmount());
                    }                   
                } 
            }
            //在此处处理，防止出现在battle::Lopp()中出现2个队伍成员都为0的情况。
            else if (pBt->GetTeamStatus() == _TEAM_STATUS_START)
            {
                //如果是战斗状态中退出，则检查战斗是否完成
                if(pBt->IsFightTeamFinsh())
                {
                    //发放奖励
                    pBt->AwardFightTeam();
                    //发送战斗结束排行榜数据包
                    pBt->BroadcastFightTeamEndMsgToMember();
                    //清理队伍成员
                    pBt->ClearMember();

                    //释放内存
                    pBattle->DelTeamFromBrbt(pBt);

                    //找出对战队伍指针
                    if(pEnemyBrbt)
                    {
                        //发放奖励
                        pEnemyBrbt->AwardFightTeam();
                        //发送战斗结束排行榜数据包
                        pEnemyBrbt->BroadcastFightTeamEndMsgToMember();
                        //清理队伍成员
                        pEnemyBrbt->ClearMember();

                        //从队伍管理器中删除2只队伍
                        pBattle->DelTeamFromBrbt(pEnemyBrbt);
                    }
                }
            } 
                
        }         
    }

    //处理玩家下线数据，保持玩家数据到数据库
    //add by huyf 2018.11.08:当玩家刚建立连接得时候，还没有走到login包得时候，userdata数据还未填充，此时不用保存玩家数据
    if (pPlayer->GetGameProcess() == _GAME_PROCESS_LOGIN)
    {        
        pPlayer->SetDeadStatus(_STATUS_ALIVE);
        pPlayer->SetGameStep(_GAME_STEP_NONE);
        pPlayer->QueryCollideSet().clear();

        //更新好友列表为下线状态
        CMsgFriendInfo msg;
        if(msg.CreateMsg(_FRIEND_INFO_OFFLINE, pPlayer, _FRIEND_STATUS_OFFLINE))
        {
            pPlayer->QueryFriendManager().BroadcastFriendSMsg(&msg, pPlayer);
        }

        //保存更新好友表数据
        pPlayer->QueryFriendManager().SaveInfo();     

        //add by huyf 2018.11.24:添加好友系统
        pPlayer->SetGameStatus(_FRIEND_STATUS_OFFLINE);   

        //限时免费领奖-玩家下线保存计时器倒计时
        if(pPlayer->QueryLimitTimeInterval().IsActive())
        {
            UINT nRemainTime = pPlayer->QueryLimitTimeInterval().GetRemain();
            pPlayer->SetShowRemainTs(nRemainTime);
        }

        //在线领奖-玩家下线保存计时器倒计时
        if(pPlayer->QueryOnLineInterval().IsActive())
        {
            UINT nRemainTime = pPlayer->QueryOnLineInterval().GetRemain();
            pPlayer->SetOnLineRemainTs(nRemainTime);
        }

        //注意******全部数据保存必须放到最后调用*********
        //玩家下线保存玩家数据到数据库
        pPlayer->SaveInfo();
    }
    //add end:当玩家刚建立连接得时候，还没有走到login包得时候，userdata数据还未填充，此时不用保存玩家数据
    
    //清空玩家BUFF商城相关数据
    //this->SetAddGold(0);                   //战斗结束后奖励的金币，金币加成200%
    //this->SetAddScore(0);                  //积分基础加:3000

    //add by huyf 2018.01.26:添加异常判断
    if(pPlayer->GetAccountID() != 0 || pPlayer->GetRobot()->getRobotIdStr().length() > 0)
    {
        /*
        cJSON* pLogoutReq = cJSON_CreateObject();
        if (pLogoutReq)
        {
            cJSON_AddNumberToObject(pLogoutReq, "uid", pPlayer->GetAccountID());
            cJSON_AddStringToObject(pLogoutReq, "appid", "3.00001");
            //add by huyf 2018.01.23:add seq element
            std::stringstream ss;
            ss << pPlayer->GetAccountID();
            std::string str = ss.str();
            cJSON_AddStringToObject(pLogoutReq, "seq", str.c_str());
            //add end:add seq element

            LOGDEBUG("[ServerManager::RemovePlayer] SendMsgToServer-ACC=[ACC_LOGOUT_REQ=%d]", ACC_LOGOUT_REQ);
            char* sJson = cJSON_Print(pLogoutReq);
            g_pClientManager->SendMsgToServer(ACC_SERVER, sJson, ACC_LOGOUT_REQ);
            free(sJson);
        }
        cJSON_Delete(pLogoutReq);

        cJSON* pLogout = cJSON_CreateObject();
        if (pLogout)
        {
            cJSON_AddStringToObject(pLogout, "robotID", pPlayer->GetRobot()->getRobotId());
            //add by huyf 2018.03.23:向PW发送玩家下线通知
            //PW需要账号信息
            cJSON_AddNumberToObject(pLogout, "accountID", pPlayer->GetAccountID());
            //add end:向PW发送玩家下线通知

            LOGDEBUG("[ServerManager::RemovePlayer] SendMsgToServer-MTS=[GS2MTS_ROBOT_OFFLINE=%d]", GS2MTS_ROBOT_OFFLINE);
            char* sJson = cJSON_Print(pLogout);
            g_pClientManager->SendMsgToServer(MTS_SERVER, sJson, GS2MTS_ROBOT_OFFLINE);

            //add by huyf 2018.03.23:向PW发送玩家下线通知
            LOGDEBUG("[ServerManager::RemovePlayer] SendMsgToServer-PW=[GS2PW_ROBOT_OFFLINE=%d]", GS2PW_ROBOT_OFFLINE);
            g_pClientManager->SendMsgToServer(PW_SERVER, sJson, GS2PW_ROBOT_OFFLINE);
            //add end:向PW发送玩家下线通知

            free(sJson);
        }
        cJSON_Delete(pLogout);
        */
       
       //根据账号清理玩家容器中的指针
        g_pPlayerPool->DelPlayerByAccout(pPlayer->GetAccountID());
    }
    //add end:添加异常判断        
    
    ret = DelPlayer(pPlayer);

    //第二步：清除PlayerPool中的信息，注意此步骤必须放在最后，
    //当调用此操作后，当前Player就有可能会被马上分配给新接入玩家
    //((ServerPlayer*)pPlayer)->FreeOwn();

    //第三步：处理此服务器上玩家的状态
    //if(bUseShareMem)
    //{//开了共享内存的服务器只处理正常ShutDown
    //	g_pOnlineUser->ChangeUserStatus(ServerID ,US_WAIT_SHUTDOWN);
    //}
    //else
    //	g_pOnlineUser->RemoveUser(ServerID);

    //第四步: 修改m_aServerHash 保存的PlayerID 为INVALID_ID
    //ASSERT(ServerID >= 0 && ServerID < OVER_MAX_SERVER);
    //m_aServerHash[ServerID] = INVALID_ID;

	DEBUG_CATCH("ServerManager::RemovePlayer")
	return ret;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	清理所有玩家
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
void ServerManager::RemoveAllPlayer()
{
	DEBUG_TRY	
    LOGDEBUG("[ServerManager::RemoveAllPlayer]");
    UINT nPlayerCount = GetPlayerAmount();
    for(UINT i=0; i<nPlayerCount; i++)
    {
        if(m_nPlayerIDS[0] == INVALID_ID)
                break;

        Player* pPlayer = g_pPlayerPool->GetPlayer(m_nPlayerIDS[i]);
        CHECK(pPlayer);

        RemovePlayer(pPlayer);
    }
	DEBUG_CATCH("ServerManager::RemoveAllPlayer")
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
BOOL ServerManager::ProcessInputs()
{
	DEBUG_TRY	
    BOOL ret = FALSE;

    if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
    { 
        LOGERROR("[ServerManager::ProcessInputs] m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET");
        return FALSE;
    } 
    //add by huyf 2018.01.24:支持I/O多路复用poll 
    //新连接接入：
    AcceptNewConnection(); 
    //add end:支持I/O多路复用poll        
    
    //数据读取
    UINT nPlayerCount = GetPlayerAmount();
    for(UINT i=0; i<nPlayerCount; i++)
    {	
        //注意：*****
        /*RemovePlayer执行后会导致m_nPlayerAmount-- 
        更新删除位置和最后位置的playerid.
        可以在本次读取数据或者在下次轮训的时候读取数据******/
        if(m_nPlayerIDS[i]==INVALID_ID)
        {
            continue;
        }

        ServerPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_nPlayerIDS[i]);
        CHECKF(pPlayer);
        Socket* pSocket = pPlayer->GetSocket();
        CHECKF(pSocket);

        SOCKET s = pSocket->getSocketID();
        //add by huyf 2018.01.24:支持I/O多路服用select、poll、epoll 
        if(s == m_pServerSocket->GetSocketID() || s == INVALID_SOCKET)
        {
            LOGERROR("[ServerManager::ProcessInputs] -2 ");
            continue;
        }
        //add end:支持I/O多路服用select、poll、epoll 
        
        //add by huyf 2018.01.24:支持I/O多路复用poll
        //if(FD_ISSET(s, &m_ReadFDs[SELECT_USE]))
        if(IsReady(pSocket))
        //add end:支持I/O多路复用poll                   
        {
            if(pSocket->isSockError())
            {
                //连接出现错误---对端在未进入游戏便alt+F4,刚建立三次握手，则对端断开连接，上层会获取到socket的ERROR标识。
                LOGERROR("[ServerManager::ProcessOutputs] pSocket->isSockError() is error i=[%d] AccountID=[%d] PlayerID=[%d] robotId=[%s] sock=[%d] Ip=[%s] Port=[%d] nPlayerCount=[%d] err=[%d] res=[%s]", 
                    i,pPlayer->GetAccountID(),pPlayer->GetPlayerID(),pPlayer->GetRobot()->getRobotId(),pSocket->getSocketID(), pSocket->m_Host,pSocket->m_Port, nPlayerCount, errno, strerror(errno));
                RemovePlayer(pPlayer);
            }
            else
            {
                //连接正常
                ret = pPlayer->ProcessInput();
                if(!ret)
                {
                    LOGERROR("[ServerManager::ProcessOutputs] RemovePlayer i=[%d] AccountID=[%d] PlayerID=[%d] robotId=[%s] sock=[%d] Ip=[%s] Port=[%d] nPlayerCount=[%d] err=[%d] res=[%s]", 
                        i,pPlayer->GetAccountID(),pPlayer->GetPlayerID(),pPlayer->GetRobot()->getRobotId(),pSocket->getSocketID(), pSocket->m_Host,pSocket->m_Port, nPlayerCount, errno, strerror(errno));
                    RemovePlayer(pPlayer);
                }                               
            }
        }
    }
	DEBUG_CATCHF("ServerManager::ProcessInputs")
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
BOOL ServerManager::ProcessOutputs()
{
	DEBUG_TRY	
		
    BOOL ret = FALSE;
    if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
    { 
        LOGERROR("[ServerManager::ProcessOutputs] m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET");
        return FALSE;
    }

    //数据发送
    UINT nPlayerCount = GetPlayerAmount();
    for(UINT i=0; i<nPlayerCount; i++)
    {
        if(m_nPlayerIDS[i]==INVALID_ID)
        {
            continue;
        }

        ServerPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_nPlayerIDS[i]);
        CHECKF(pPlayer);
        Socket* pSocket = pPlayer->GetSocket();
        CHECKF(pSocket);

        SOCKET s = pSocket->getSocketID();
        //add by huyf 2018.01.24:支持I/O多路复用poll 
        if(s == m_pServerSocket->GetSocketID() || s == INVALID_SOCKET)
            continue;
        //add end:支持I/O多路复用poll                
            
        //add by huyf 2018.01.24:支持I/O多路复用poll
        //if(FD_ISSET(s, &m_WriteFDs[SELECT_USE]))
        if(IsWrite(pSocket))
        //add end:支持I/O多路复用poll
        {				
            if(pSocket->isSockError())
            {
                //连接出现错误
                LOGERROR("[ServerManager::ProcessOutputs] pSocket->isSockError() is error i=[%d] AccountID=[%d] PlayerID=[%d] robotId=[%s] sock=[%d] Ip=[%s] Port=[%d] nPlayerCount=[%d] err=[%d] res=[%s]", 
                    i,pPlayer->GetAccountID(),pPlayer->GetPlayerID(),pPlayer->GetRobot()->getRobotId(),pSocket->getSocketID(), pSocket->m_Host,pSocket->m_Port, nPlayerCount, errno, strerror(errno));
                RemovePlayer(pPlayer);
            }
            else
            {						
                //数据发送
                ret = pPlayer->ProcessOutput();
                if(!ret)
                {
                    LOGERROR("[ServerManager::ProcessOutputs] RemovePlayer i=[%d] AccountID=[%d] PlayerID=[%d] robotId=[%s] sock=[%d] Ip=[%s] Port=[%d] nPlayerCount=[%d] err=[%d] res=[%s]", 
                        i,pPlayer->GetAccountID(),pPlayer->GetPlayerID(),pPlayer->GetRobot()->getRobotId(),pSocket->getSocketID(), pSocket->m_Host,pSocket->m_Port, nPlayerCount, errno, strerror(errno));
                    RemovePlayer(pPlayer);
                }
            }
        }
    }

	DEBUG_CATCHF("ServerManager::ProcessOutputs")
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
BOOL ServerManager::ProcessExceptions()
{
	DEBUG_TRY	
    if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
    { 
        LOGDEBUG("[ServerManager::ProcessExceptions] m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET");
        return TRUE;
    }

    UINT nPlayerCount = GetPlayerAmount();
    for(UINT i=0; i<nPlayerCount; i++)
    {
        if(m_nPlayerIDS[i]==INVALID_ID)
            continue;

        //某个玩家断开网络连接
        ServerPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_nPlayerIDS[i]);
        ASSERT(pPlayer);
        Socket* pSocket = pPlayer->GetSocket();
        CHECKF(pSocket);

        SOCKET s = pSocket->getSocketID();
        //add by huyf 2018.01.24:支持I/O多路复用poll 
        if(s == m_pServerSocket->GetSocketID() || s == INVALID_SOCKET)
                continue;
        //add end:支持I/O多路复用poll  
        
        //add by huyf 2018.01.24:支持I/O多路复用poll
        //if(FD_ISSET(s, &m_ExceptFDs[SELECT_USE]))
        if(IsException(pSocket))
        {
            LOGERROR("[ServerManager::ProcessExceptions] is IsException i=[%d] AccountID=[%d] PlayerID=[%d] robotId=[%s] sock=[%d] Ip=[%s] Port=[%d] nPlayerCount=[%d] err=[%d] res=[%s]", 
                i,pPlayer->GetAccountID(),pPlayer->GetPlayerID(),pPlayer->GetRobot()->getRobotId(),pSocket->getSocketID(), pSocket->m_Host,pSocket->m_Port, nPlayerCount, errno, strerror(errno));
            RemovePlayer(pPlayer);
        }     
        //add end:支持I/O多路复用poll
    }
    return TRUE;
	DEBUG_CATCHF("ServerManager::ProcessExceptions")
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
BOOL ServerManager::ProcessCommands()
{   
    BOOL ret;
	DEBUG_TRY
    if (m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET) // no player exist
    { 
        LOGDEBUG("[ServerManager::ProcessCommands] m_MinFD == INVALID_SOCKET && m_MaxFD == INVALID_SOCKET");
        return TRUE;
    }

    UINT nPlayerCount = GetPlayerAmount();
    for(UINT i = 0; i < nPlayerCount; i++)
    {
        if(m_nPlayerIDS[i] == INVALID_ID)
                continue;

        ServerPlayer* pPlayer = g_pPlayerPool->GetPlayer(m_nPlayerIDS[i]);
        CHECKF(pPlayer);
        Socket* pSocket = pPlayer->GetSocket();
        CHECKF(pSocket);

        SOCKET s = pSocket->getSocketID();
        //add by huyf 2018.01.24:支持I/O多路复用poll 
        if(s == m_pServerSocket->GetSocketID() || s == INVALID_SOCKET)
                continue;
        //add end:支持I/O多路复用poll  

        if(pSocket->isSockError())
        {
            //连接出现错误
            LOGERROR("[ServerManager::ProcessCommands] pSocket->isSockError() is error i=[%d] AccountID=[%d] PlayerID=[%d] robotId=[%s] sock=[%d] Ip=[%s] Port=[%d] nPlayerCount=[%d] err=[%d] res=[%s]", 
                i,pPlayer->GetAccountID(),pPlayer->GetPlayerID(),pPlayer->GetRobot()->getRobotId(),pSocket->getSocketID(), pSocket->m_Host,pSocket->m_Port, nPlayerCount, errno, strerror(errno));
            RemovePlayer(pPlayer);
        }
        else
        {
            //连接正常
            //处理玩家相关数据
            ret = pPlayer->ProcessCommand(FALSE);
            if(!ret)
            {
                LOGERROR("[ServerManager::ProcessCommand] RemovePlayer i=[%d] AccountID=[%d] PlayerID=[%d] robotId=[%s] sock=[%d] Ip=[%s] Port=[%d] nPlayerCount=[%d] err=[%d] res=[%s]", 
                    i,pPlayer->GetAccountID(),pPlayer->GetPlayerID(),pPlayer->GetRobot()->getRobotId(),pSocket->getSocketID(), pSocket->m_Host,pSocket->m_Port, nPlayerCount, errno, strerror(errno));
                //如果逻辑操作返回失败，则需要断开当前连接
                ret = RemovePlayer(pPlayer);
            }

            //add by huyf 2018.11.02:添加服务器收到前端消息的时间戳,暂时不做缓存帧
            //时序图:前端逻辑帧（33ms : 30HZ/1秒） 服务器逻辑帧（66ms : 15HZ/1秒），暂时先不做帧缓存试试。
            else
            {
                ///////////////////////////////////////////////////////////////////////
                ///;广播同步帧缓存66ms 0:不缓存 1:缓存
                if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE == 0)
                {
                    //LOGDEBUG("[------------] Syn CMsgWalk(_WALK_ACTION_CONTINUE_PREVIOUS_CMD) pSocket->GetSynMsgStatus()=[%d]", pSocket->GetSynMsgStatus());
                    if (pSocket->GetSynMsgStatus() == _SYN_MSG_PREVIOUS)
                    {
                        long long llCurMillSecTime = GetMillSecTime();
                        long long mllRecvMsgTimeStamp = pSocket->GetRecvMsgTimeStamp();
                        //LOGDEBUG("[------------] Syn CMsgWalk(_WALK_ACTION_CONTINUE_PREVIOUS_CMD) To Client=[%d]\tPlayerID=[%d]\tGetMillSecTime=[%lld]\tmllRecvMsgTimeStamp=[%lld]\tTimeStamp=[%lld]\n", 
                        //            pSocket->getSocketID(), pPlayer->GetUid(), llCurMillSecTime, mllRecvMsgTimeStamp, llCurMillSecTime-mllRecvMsgTimeStamp);
                        if (mllRecvMsgTimeStamp != 0 && mllRecvMsgTimeStamp + g_config.ONLINE_BROADCAST_FRAME_MSG_INTERVAL_TIME <= llCurMillSecTime)
                        {
                            static int snIndex = 0;
                            snIndex++;

                            CBattleTeam* pBt = NULL;
                            CMsgWalk msg;
                            if (msg.CreateMsg(_WALK_ACTION_CONTINUE_PREVIOUS_CMD, (INT)pPlayer->GetUid(),0,0,0,0,0,0, llCurMillSecTime))
                            {
                                pBt = pPlayer->GetBct();
                                if (pBt)
                                {
                                    //大乱斗队伍广播
                                    pBt->BroadcastTeamMsg(&msg);
                                }
                                pBt = pPlayer->GetBrbt();
                                if (pBt)
                                {
                                    //红蓝对战队伍广播
                                    pBt->BroadcastTeamMsg(&msg);
                                    if (pBt->GetEnemyTeam())
                                    {
                                        //敌方战伍广播
                                        pBt->GetEnemyTeam()->BroadcastTeamMsg(&msg);
                                    }
                                }

                                //更新时间
                                pSocket->SetRecvMsgTimeStamp(llCurMillSecTime);
                                
                                LOGDEBUG("[ServerManager::ProcessCommand] Syn CMsgWalk(_WALK_ACTION_CONTINUE_PREVIOUS_CMD) To Client=[%d]\tPlayerID=[%d]\tGetMillSecTime=[%lld]\tmllRecvMsgTimeStamp=[%lld]\tTimeStamp=[%lld] snIndex=[%d]\n", 
                                    pSocket->getSocketID(), pPlayer->GetUid(), llCurMillSecTime, mllRecvMsgTimeStamp, llCurMillSecTime-mllRecvMsgTimeStamp,snIndex);
                            }
                        }
                    }
                    else //if (pSocket->GetSynMsgStatus() == _SYN_MSG_COLLIDE)
                    {
                        long long llCurMillSecTime = GetMillSecTime();
                        long long mllCollideSynMsgTimeStamp = pSocket->GetCollideSynMsgTimeStamp();
                        if (mllCollideSynMsgTimeStamp != 0 && mllCollideSynMsgTimeStamp + g_config.ONLINE_BROADCAST_COLLIDE_MSG_INTERVAL_TIME <= llCurMillSecTime)
                        {
                            CBattleTeam* pBt = NULL;
                            CMsgWalk msg;
                            if (msg.CreateMsg(_WALK_ACTION_CONTINUE_COLLIDE_CMD, (INT)pPlayer->GetUid(),0,0,0,0,0,0,llCurMillSecTime))
                            {
                                pBt = pPlayer->GetBct();
                                if (pBt)
                                {
                                    //大乱斗队伍广播
                                    pBt->BroadcastTeamMsg(&msg);
                                }
                                pBt = pPlayer->GetBrbt();
                                if (pBt)
                                {
                                    //红蓝对战队伍广播
                                    pBt->BroadcastTeamMsg(&msg);
                                    if (pBt->GetEnemyTeam())
                                    {
                                        //敌方战伍广播
                                        pBt->GetEnemyTeam()->BroadcastTeamMsg(&msg);
                                    }
                                }

                                //更新时间
                                pSocket->SetCollideSynMsgTimeStamp(llCurMillSecTime);

                                //更新次数
                                INT nHaveCollideSynMsgCount = pSocket->GetCollideSynMsgCount();
                                pSocket->SetCollideSynMsgCount(++nHaveCollideSynMsgCount);
                                if (nHaveCollideSynMsgCount >= g_config.ONLINE_BROADCAST_COLLIDE_SYN_MSG_COUNT)
                                {
                                    pSocket->SetSynMsgStatus(_SYN_MSG_PREVIOUS);
                                    //更新时间
                                    pSocket->SetRecvMsgTimeStamp(llCurMillSecTime);
                                }
                                
                                LOGDEBUG("[ServerManager::ProcessCommand] Syn CMsgWalk(_WALK_ACTION_CONTINUE_COLLIDE_CMD) To Client=[%d]\tPlayerID=[%d]\tGetMillSecTime=[%lld]\tmllCollideSynMsgTimeStamp=[%lld]\tTimeStamp=[%lld]\n", 
                                    pSocket->getSocketID(), pPlayer->GetUid(), llCurMillSecTime, mllCollideSynMsgTimeStamp, llCurMillSecTime-mllCollideSynMsgTimeStamp);
                            }
                        }
                    }
                    
                }
            }
            //add end:添加服务器收到前端消息的时间戳,暂时不做缓存帧
        }
    }
	DEBUG_CATCHF("ServerManager::ProcessCommands")
	return ret;
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
BOOL ServerManager::HeartBeat(DWORD dwTime/*=0*/)
{
    DEBUG_TRY
    BOOL ret;
    DWORD dwTime = 0;

    UINT nPlayerCount = GetPlayerAmount();
    for(UINT i=0; i<nPlayerCount; i++)
    {
        if(m_nPlayerIDS[i] == INVALID_ID)   
            continue;

        Player* pPlayer = g_pPlayerPool->GetPlayer(m_nPlayerIDS[i]);
        CHECKF(pPlayer);
        Socket* pSocket = pPlayer->GetSocket();
        CHECKF(pSocket);

        if(pSocket->isSockError())
        {
            //连接出现错误
            LOGERROR("[ServerManager::HeartBeat] pSocket->isSockError() is error i=[%d] AccountID=[%d] PlayerID=[%d] robotId=[%s] sock=[%d] Ip=[%s] Port=[%d] nPlayerCount=[%d] err=[%d] res=[%s]", 
                i,pPlayer->GetAccountID(),pPlayer->GetPlayerID(),pPlayer->GetRobot()->getRobotId(),pSocket->getSocketID(), pSocket->m_Host,pSocket->m_Port, nPlayerCount, errno, strerror(errno));
            RemovePlayer(pPlayer);
        }
        else
        {
            //ret = pPlayer->HeartBeat(dwTime);
            //if(!ret)
            if(0)
            {
                LOGERROR("[ServerManager::HeartBeat] RemovePlayer i=[%d] AccountID=[%d] PlayerID=[%d] robotId=[%s] sock=[%d] Ip=[%s] Port=[%d] nPlayerCount=[%d] err=[%d] res=[%s]", 
                i,pPlayer->GetAccountID(),pPlayer->GetPlayerID(),pPlayer->GetRobot()->getRobotId(),pSocket->getSocketID(), pSocket->m_Host,pSocket->m_Port, nPlayerCount, errno, strerror(errno));
                //如果逻辑操作返回失败，则需要断开当前连接
                ret = RemovePlayer(pPlayer);
            }
            //!alter by huyf 2017.12.28:removePlayer index 0 only bug
            // return ret;
            //!alter end:removePlayer index 0 only bug
        }
    }
	DEBUG_CATCH("ServerManager::HeartBeat")

	return FALSE;
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
BOOL ServerManager::Loop()
{
	{
		BOOL ret = FALSE;
		//UINT uTime = g_pTimeManager->CurrentTime();
                
#ifdef _SELECT_ 
		DEBUG_TRY
        INT nReadyNum = -1;
        nReadyNum = Select();
        if(nReadyNum == -1)
        {
            //出错程序退出
            LOGERROR("Select Is Error...nReadyNum=[-1]");
            return FALSE;
        }
        else if(nReadyNum == 0)
        {
            return TRUE;
        }
        else
        {
            //...有消息需要进一步处理
        }                
        DEBUG_CATCHF("Select")
#endif                        
#ifdef _POLL_                  
        DEBUG_TRY
        m_nEffectivePollFds = Poll();
        if(m_nEffectivePollFds == -1)
        {
            //出错程序退出
            LOGERROR("Poll Is Error...m_nEffectivePollFds=[-1]");
            return FALSE;
        }
        else if(m_nEffectivePollFds == 0)
        {
            return TRUE;
        }
        else
        {
            //...有消息需要进一步处理
        }                
        DEBUG_CATCHF("Poll")                        
#endif
#ifdef _EPOLL_
        DEBUG_TRY
        m_nEffectiveEpollFds = EPoll();
        if(m_nEffectiveEpollFds == -1)
        {
            //出错程序退出
            LOGERROR("EPoll Is Error...m_nEffectiveEpollFds=[-1]");
            return FALSE;
        }
        else if(m_nEffectiveEpollFds == 0)
        {
            return TRUE;
        }
        else
        {
            //...有消息需要进一步处理
        }                
        DEBUG_CATCHF("EPoll")          
#endif
        DEBUG_TRY
        ret = ProcessExceptions();
        DEBUG_CATCHF("ProcessExceptions")
        
        DEBUG_TRY
        ret = ProcessInputs();
		DEBUG_CATCHF("ProcessInputs")

		DEBUG_TRY
        ret = ProcessCommands();
        DEBUG_CATCHF("ProcessCommands")

		DEBUG_TRY
        ret = ProcessOutputs();
        DEBUG_CATCHF("ProcessOutputs")

		DEBUG_TRY
        ret = HeartBeat();
        DEBUG_CATCHF("HeartBeat")

        DEBUG_TRY
        ret = OnTimer();
        DEBUG_CATCHF("OnTimer")

        //add by huyf 2018.10.09:碰碰车二期
        //处理房间的事情
        
        //add end:碰碰车二期

        return ret;
	}

}

//////////////////////////////////////////////////////////////////////
//
// func description	:	通过ServerID取得ServerPlayer
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
ServerPlayer* ServerManager::GetServerPlayer(ID_t ServerID)
{
	DEBUG_TRY	
    CHECKF(ServerID >= 0 && ServerID < OVER_MAX_SERVER);
    PlayerID_t pid = m_aServerHash[ServerID];

    if(pid == INVALID_ID)
        return NULL;

    ServerPlayer* pServerPlayer = g_pPlayerPool->GetPlayer(pid);
    CHECKF(pServerPlayer);
	return pServerPlayer;
    DEBUG_CATCH0("ServerManager::BroadCast")
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	向所有服务器端程序广播消息
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
void ServerManager::BroadCast(CNetMsg* pMsg)
{
	DEBUG_TRY	
    UINT nPlayerCount = GetPlayerAmount();
    for(UINT i=0; i<nPlayerCount; i++)
    {
        if(m_nPlayerIDS[i] == INVALID_ID)
            continue;

        Player* pPlayer = g_pPlayerPool->GetPlayer(m_nPlayerIDS[i]);
        IF_OK(pPlayer)
        {
            continue;
        }

        pPlayer->SendPacket(pMsg);
    }
	DEBUG_CATCH("ServerManager::BroadCast")
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	只向Server广播
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
void ServerManager::BroadCastServer(CNetMsg* pMsg)
{
	DEBUG_TRY	
    UINT nPlayerCount = GetPlayerAmount();
    for(UINT i=0; i<nPlayerCount; i++)
    {
        if(m_nPlayerIDS[i] == INVALID_ID)
            continue;

        ServerPlayer* pPlayer = (ServerPlayer*)g_pPlayerPool->GetPlayer(m_nPlayerIDS[i]);
        IF_OK(pPlayer)
        {
            continue;
        }
        if(pPlayer->GetServerData()->m_Type == SERVER_GAME)
            pPlayer->SendPacket(pMsg);
    }

	DEBUG_CATCH("ServerManager::BroadCastServer")
        return;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	只向Login广播
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
void ServerManager::BroadCastLogin(CNetMsg* pMsg)
{
}

//add by huyf 2018.01.24:支持I/O多路复用poll
//////////////////////////////////////////////////////////////////////
//
// func description :   接受新连接判断
//
//-----input------
// Parameters
//      无
//------output------
// Return 
//      无
BOOL ServerManager::IsAccept(Socket* pSocket)
{
    DEBUG_TRY
    CHECKF(pSocket);
#ifdef _SELECT_     
    SOCKET sock = pSocket->getSocketID();
    if(sock != INVALID_SOCKET)
    {
        if(FD_ISSET(sock, &m_ReadFDs[SELECT_USE]))
            return TRUE;
    }
#endif    
#ifdef _POLL_ 
    struct pollfd* pfd = pSocket->GetClientPollFd();
    CHECKF(pfd);
    if(pfd->fd != INVALID_SOCKET)
    {
        if (pfd->revents & POLLIN)
            return TRUE;
    }                     
#endif   
#ifdef _EPOLL_
    // 客户端有新的连接请求  
    SOCKET sock = pSocket->getSocketID();
    if(sock != INVALID_SOCKET)
    {
        for(int i=0; i<m_nEffectiveEpollFds; ++i)
        {
            if(m_EffectiveEpollEvents[i].data.fd > 0 && m_EffectiveEpollEvents[i].data.fd == sock && m_EffectiveEpollEvents[i].events & EPOLLIN)  
            {  
                return TRUE;
            }
        }
    }
    // 客户端有数据发送过来
#endif
    return FALSE;
    DEBUG_CATCHF("ServerManager::IsAccept");
}
//////////////////////////////////////////////////////////////////////
//
// func description :   SOCKET 可写
//
//-----input------
// Parameters
//      无
//------output------
// Return 
//      无
// 
BOOL ServerManager::IsReady(Socket* pSocket)
{
    DEBUG_TRY
    CHECKF(pSocket);
#ifdef _SELECT_  
    SOCKET sock = pSocket->getSocketID();
    if(sock != INVALID_SOCKET)
    {
        if(FD_ISSET(sock, &m_ReadFDs[SELECT_USE]))
            return TRUE;
    }
#endif    
#ifdef _POLL_ 
    struct pollfd* pfd = pSocket->GetClientPollFd();
    CHECKF(pfd);
    if(pfd->fd != INVALID_SOCKET)
    {
        if (pfd->revents & POLLIN)
            return TRUE;
    }                     
#endif   
#ifdef _EPOLL_ 
    SOCKET sock = pSocket->getSocketID();
    if(sock != INVALID_SOCKET)
    {
        for(int i=0; i<m_nEffectiveEpollFds; ++i)
        {            
            if(m_EffectiveEpollEvents[i].data.fd > 0 && m_EffectiveEpollEvents[i].data.fd == sock && m_EffectiveEpollEvents[i].events & EPOLLIN)
            {   
                return TRUE;
            }
        }
    }
#endif
    return FALSE;
    DEBUG_CATCHF("ServerManager::IsReady");
}
//////////////////////////////////////////////////////////////////////
//
// func description :   SOCKET 可读
//
//-----input------
// Parameters
//      无
//------output------
// Return 
//      无
// 
BOOL ServerManager::IsWrite(Socket* pSocket)
{
    DEBUG_TRY
    CHECKF(pSocket);
#ifdef _SELECT_     
    SOCKET sock = pSocket->getSocketID();
   if(sock != INVALID_SOCKET)
    {
        if(FD_ISSET(sock, &m_WriteFDs[SELECT_USE]))
            return TRUE;
    }
    return FALSE;    
#endif    
#ifdef _POLL_ 
    struct pollfd* pfd = pSocket->GetClientPollFd();
    CHECKF(pfd);
    if(pfd->fd != INVALID_SOCKET)
    {
        if (pfd->revents & POLLOUT)
            return TRUE;
    }
    return FALSE;       
#endif    
#ifdef _EPOLL_ 
    SOCKET sock = pSocket->getSocketID();
    if(sock != INVALID_SOCKET)
    {
        for(int i=0; i<m_nEffectiveEpollFds; ++i)
        {
            if(m_EffectiveEpollEvents[i].data.fd > 0 && m_EffectiveEpollEvents[i].data.fd == sock && m_EffectiveEpollEvents[i].events & EPOLLOUT)  
            {  
                return TRUE;
            }
        }
    }
#endif    

    return FALSE;
    DEBUG_CATCHF("ServerManager::IsWrite");
}
//////////////////////////////////////////////////////////////////////
//
// func description :   SOCKET 异常
//
//-----input------
// Parameters
//      无
//------output------
// Return 
//      无
//  注意：
//  针对 _EPOLL_ 模型：遇到EPOLLERR/EPOLLHUP事件时,有两种做法:
//      1.不调用error异常流程,而是跟EPOLLIN一样调用读取流程,让读取流程去确认/处理实际的错误.
//      2.通过getsockopt SO_ERROR获取具体的错误码,并过滤掉非Fatal错误.
// 
BOOL ServerManager::IsException(Socket* pSocket)
{
DEBUG_TRY
    CHECKF(pSocket);
#ifdef _SELECT_     
    SOCKET sock = pSocket->getSocketID();
    if(sock != INVALID_SOCKET)
    {
        if(FD_ISSET(sock, &m_ExceptFDs[SELECT_USE]))
            return TRUE;
    }
    return FALSE;
#endif    
#ifdef _POLL_ 
    struct pollfd* pfd = pSocket->GetClientPollFd();
    CHECKF(pfd);
    if(pfd->fd != INVALID_SOCKET)
    {
        if (pfd->revents & POLLERR)
            return TRUE;
    }
    return FALSE;
#endif 
#ifdef _EPOLL_ 
    //!alter by huyf 2018.04.18:先放开
    // 前端异常端口会直接走EPOLLIN然后有-1和错误码返回进行处理
    // 用EPOLLIN去读取流程,让它去处理异常
    SOCKET sock = pSocket->getSocketID();
    if(sock != INVALID_SOCKET)
    {
        for(int i=0; i<m_nEffectiveEpollFds; ++i)
        {    
            //!alter by huyf 2018.07.26:添加EAGAIN EINTR判断
            //在使用2.6.17之后版本内核的服务器系统中，对端连接断开触发的epoll事件会包含 EPOLLIN | EPOLLRDHUP，即0x2001。有了这个事件，对端断开连接的异常就可以在底层进行处理了，不用再移交到上层
            //注意，在使用2.6.17之前版本内核的系统中，sys/epoll.h的EPOLL_EVENTS枚举类型中是没有EPOLLRDHUP事件的，所以带EPOLLRDHUP的程序无法编译通过
            //if(m_EffectiveEpollEvents[i].data.fd > 0 && m_EffectiveEpollEvents[i].data.fd == sock && m_EffectiveEpollEvents[i].events & EPOLLERR)            
            //if(m_EffectiveEpollEvents[i].data.fd > 0 && m_EffectiveEpollEvents[i].data.fd == sock && m_EffectiveEpollEvents[i].events & (EPOLLERR | EPOLLRDHUP))//注意***此处不能放EPOLLIN否则建立链接的时候就会删除角色断开链接
            if(m_EffectiveEpollEvents[i].data.fd > 0 
                && m_EffectiveEpollEvents[i].data.fd == sock 
                && ((m_EffectiveEpollEvents[i].events & EPOLLERR || m_EffectiveEpollEvents[i].events & EPOLLHUP) && !(m_EffectiveEpollEvents[i].events & EPOLLIN)))
            {  
                //if (errno != EAGAIN && errno != EINTR)
                {
                    LOGERROR("对端已经断开连接...sock=[%d] events=[%d] errno=[%d] res=[%s]", sock, m_EffectiveEpollEvents[i].events, errno, strerror(errno));
                    return TRUE;
                }                
            }
            //!alter end:添加EAGAIN EINTR判断
        }
    }
    // /*
    // //错误退出  
    // if ((eventList[n].events & EPOLLERR) ||  
    //     (eventList[n].events & EPOLLHUP) ||  
    //     !(eventList[n].events & EPOLLIN)) --------建立捂手后有消息过来就会直接判断为异常，干掉socket连接 
    // {  
    //   printf ( "epoll error\n");  
    //   close (eventList[n].data.fd);  
    //   return -1;  
    // }
    // */
    //!alter end:先放开
#endif
    return FALSE;
    DEBUG_CATCHF("ServerManager::IsException");
}
//add end:支持I/O多路复用poll

//OnTimer 接口
BOOL ServerManager::OnTimer()
{
    DEBUG_TRY
    BOOL ret = TRUE;
    //!alter by huyf 2019.01.07:暂时不做时钟检测
    //if (m_tOnTimerInterval.ToNextTime())
    //!alter end:暂时不做时钟检测
    {
       UINT nPlayerCount = GetPlayerAmount();
        for(UINT i=0; i<nPlayerCount; i++)
        {
            if(m_nPlayerIDS[i] == INVALID_ID)   
                continue;

            Player* pPlayer = g_pPlayerPool->GetPlayer(m_nPlayerIDS[i]);
            CHECKF(pPlayer);
            Socket* pSocket = pPlayer->GetSocket();
            CHECKF(pSocket);

            if(pSocket->isSockError())
            {
                //连接出现错误
                LOGERROR("[ServerManager::OnTimer] pSocket->isSockError() is error i=[%d] AccountID=[%d] PlayerID=[%d] robotId=[%s] sock=[%d] Ip=[%s] Port=[%d] nPlayerCount=[%d] err=[%d] res=[%s]", 
                    i,pPlayer->GetAccountID(),pPlayer->GetPlayerID(),pPlayer->GetRobot()->getRobotId(),pSocket->getSocketID(), pSocket->m_Host,pSocket->m_Port, nPlayerCount, errno, strerror(errno));
                RemovePlayer(pPlayer);
            }
            else
            {
                ret = pPlayer->OnTimer();
                if(!ret)
                {
                    LOGERROR("[ServerManager::OnTimer] RemovePlayer i=[%d] AccountID=[%d] PlayerID=[%d] robotId=[%s] sock=[%d] Ip=[%s] Port=[%d] nPlayerCount=[%d] err=[%d] res=[%s]", 
                        i,pPlayer->GetAccountID(),pPlayer->GetPlayerID(),pPlayer->GetRobot()->getRobotId(),pSocket->getSocketID(), pSocket->m_Host,pSocket->m_Port, nPlayerCount, errno, strerror(errno));
                    //如果逻辑操作返回失败，则需要断开当前连接
                    ret = RemovePlayer(pPlayer);
                }
            }
        }
    }

    return ret;
    
    DEBUG_CATCHF("OnTimer")
}