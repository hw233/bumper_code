////////////////////////////////////////////////////////////////
//文件名称：Socket.h
//功能描述：网络底层socket封装API接口(socket system-call(API) Collection)
//版本说明：socket API
//			
//编写作者：2017.08.11 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//include头文件
////////////////////////////////////////////////////////////////

#include "Socket.h"
#include <string.h>
//!alter by huyf 2018.08.01:支持双栈支持IPV4和IPV6
#include "../Common/Config.h"
//!alter end:支持双栈支持IPV4和IPV6
//add by huyf 2018.11.23:player处的websocket接受数据处理迁移
#include "SocketInputStream.h"
#include "SocketOutputStream.h"
#include "../MsgHandler/NetMsg.h"
//add end:player处的websocket接受数据处理迁移

#if defined (__WINDOWS__)
#elif defined(__LINUX__)
#endif


//////////////////////////////////////////////////////////////////////
//
// func description	:	socket constructor
//
//-----input------
// Parameters
//     	无
//------output------
// Return 
//     无
//
Socket::Socket()
{	
    //socket id
     m_SocketID = INVALID_SOCKET;
     
    //add by huyf 2018.01.24:支持I/O多路复用poll
#ifdef _POLL_
    m_pClientPollFd = NULL;
#endif        
    //add end:支持I/O多路复用poll

    // socket address structure
    memset(&m_SockAddr, 0, scnSOCKADDR_IN);

    //!alter by huyf 2018.08.01:支持双栈支持IPV4和IPV6
    memset(&m_SockAddr6, 0, scnSOCKADDR_IN6);
    //!alter end:支持双栈支持IPV4和IPV6

    // peer host
    memset(m_Host, 0, IP_SIZE);

    // peer port
    m_Port = 0;
#ifdef _EPOLL_
    m_pEpollEvent = NULL;
    m_EpollFd = INVALID_ID;
#endif

    //add by liufei 2018.10.19:添加websocket的对接
    m_handler = new WebsocketHandler();
    m_nFirstHand = 1;
    //add end:添加websocket的对接
    //
    m_llRecvMsgTimeStamp = 0;
    m_llCollideSynMsgTimeStamp = 0;
    m_nCollideSynMsgCount = 0;
    m_nSynMsgStatus = _SYN_MSG_PREVIOUS;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	socket constructor
//
//-----input------
// Parameters
//     	host 			:	IP字符串
//		port 			:   端口号
//------output------
// Return 
//     无
//
Socket::Socket(const CHAR* host, UINT port)
{
    //socket id
     m_SocketID = INVALID_SOCKET;
     
    //add by huyf 2018.01.24:支持I/O多路复用poll
#ifdef _POLL_
    m_pClientPollFd = NULL;
#endif        
    //add end:支持I/O多路复用poll
    //add by huyf 2018.01.24:支持I/O多路复用epoll
#ifdef _EPOLL_
    m_pEpollEvent = NULL;
    m_EpollFd = INVALID_ID;
#endif        
    //add end:支持I/O多路复用epoll
    // socket address structure
    memset(&m_SockAddr, 0, scnSOCKADDR_IN);

    //!alter by huyf 2018.08.01:支持双栈支持IPV4和IPV6
    memset(&m_SockAddr6, 0, scnSOCKADDR_IN6);
    //!alter end:支持双栈支持IPV4和IPV6

    // peer host
    memset(m_Host, 0, IP_SIZE);

    if(host)
    {
        strncpy(m_Host, host, IP_SIZE);
    }

    m_Port = port;

    //add by liufei 2018.10.19:添加websocket的对接
    m_handler = new WebsocketHandler();
    m_nFirstHand =1;
    //add end:添加websocket的对接
    
    m_llRecvMsgTimeStamp = 0;
    m_llCollideSynMsgTimeStamp = 0;
    m_nCollideSynMsgCount = 0;
    m_nSynMsgStatus = _SYN_MSG_PREVIOUS;
  
    //create
    create();
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	socket destructor
//
//-----input------
// Parameters
//     	无
//------output------
// Return 
//     无
//
Socket::~Socket()
{
    //add by liufei 2018.10.19:添加websocket的对接
    SAFE_DELETE(m_handler);
    //add end:添加websocket的对接
    
    //close
    close();              
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	socket close
//
//-----input------
// Parameters
//     	无
//------output------
// Return 
//     无
//
VOID Socket::close() 
{ 
    DEBUG_TRY
    //if(isValid() && !isSockError()) 
    //{
        SocketAPI::closesocket_ex(m_SocketID);			
    //}

    m_SocketID = INVALID_SOCKET;
    
    //add by huyf 2018.01.24:支持I/O多路复用poll
#ifdef  _POLL_
    if(m_pClientPollFd)
    {
        m_pClientPollFd->fd = INVALID_SOCKET;
        m_pClientPollFd = NULL;
    }
#endif        
    //add end:支持I/O多路复用poll
    //add by huyf 2018.01.24:支持I/O多路复用epoll
#ifdef  _EPOLL_
    if(m_pEpollEvent)
    {
        m_pEpollEvent->data.fd = INVALID_SOCKET;
        m_pEpollEvent = NULL;
    }
    if(m_EpollFd != INVALID_ID)
    {
        SocketAPI::closesocket_ex(m_EpollFd); 
        m_EpollFd = INVALID_ID; 
    }
#endif        
    //add end:支持I/O多路复用epoll     
    memset(&m_SockAddr, 0, sizeof(SOCKADDR_IN));
    //!alter by huyf 2018.08.01:支持双栈支持IPV4和IPV6
    memset(&m_SockAddr6, 0, sizeof(SOCKADDR_IN6));
    //!alter end:支持双栈支持IPV4和IPV6
    memset(m_Host, 0, IP_SIZE);
    m_Port = 0;

    //add by liufei 2018.10.19:添加websocket的对接
    m_nFirstHand = 1;
    m_handler->SetStatus(WEBSOCKET_UNCONNECT);
    m_handler->SetSocket(INVALID_SOCKET);
    m_handler->SetBuff(NULL);
    m_llRecvMsgTimeStamp = 0;
    m_llCollideSynMsgTimeStamp = 0;
    m_nCollideSynMsgCount = 0;
    m_nSynMsgStatus = _SYN_MSG_PREVIOUS;
    LOGDEBUG("[Socket::close] m_nFirstHand=[%d]", m_nFirstHand);
    //add end:添加websocket的对接

    DEBUG_CATCH("Socket::Close");
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	socket create
//
//-----input------
// Parameters
//     	无
//------output------
// Return 
//     成功：TRUE 失败:FALSE
//
BOOL Socket::create(int nIPModule /*=0*/)
{
	DEBUG_TRY
    //!alter by huyf 2018.08.01:支持双栈支持IPV4和IPV6
    if (nIPModule) //  IPV6
    {
        //AF_INET ---> AF_INET6
        m_SocketID = SocketAPI::socket_ex(AF_INET6, SOCK_STREAM, 0);  //获得fd
        bzero(&m_SockAddr6, scnSOCKADDR_IN6);
    }
    else //  IPV4
    {
        m_SocketID = SocketAPI::socket_ex(AF_INET, SOCK_STREAM, 0);  //获得fd
        bzero(&m_SockAddr, scnSOCKADDR_IN);
    }
    //!alter end:支持双栈支持IPV4和IPV6
    
    if (isValid())
    {
        //!alter by huyf 2018.03.19:socket缓冲区监控日志
        UINT unRecvBufSize = getReceiveBufferSize();
        UINT unSendBufSize = getSendBufferSize();
        LOGMSG("[Socket::create] m_SocketID=[%d] RECEIVE_BUFFER_SIZE=[%u] SEND_BUFFER_SIZE=[%u]", m_SocketID, unRecvBufSize, unSendBufSize);
        //!alter end:socket缓冲区监控日志
        return TRUE;
    }
    else
    {
        return FALSE;
    }
	DEBUG_CATCHF("Socket::create()")
	
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	connect
//
//-----input------
// Parameters
//     	无
//------output------
// Return 
//     成功：TRUE 失败:FALSE
//
BOOL Socket::connect()
{
	DEBUG_TRY
    bzero(&m_SockAddr, sizeof(m_SockAddr));

    //set struct sockaddr_in params...
    //设置地址家族
    m_SockAddr.sin_family = AF_INET;
    //设置地址
    inet_aton(m_Host, &m_SockAddr.sin_addr);//inet_addr->inet_aton,inet_addr有缺陷
    //设置端口
    m_SockAddr.sin_port = htons(m_Port);

    // try to connect to peer host
    BOOL result = SocketAPI::connect_ex(m_SocketID, (const struct sockaddr*)&m_SockAddr, sizeof(m_SockAddr));

    if (result)
        return TRUE;
    else
        return FALSE;
	DEBUG_CATCHF("Socket::connect")
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	connect
//
//-----input------
// Parameters
//     	host 			:	需要连接的host
//     	port 			:	需要连接的port
//------output------
// Return 
//     成功：TRUE 失败:FALSE
//
BOOL Socket::connect(const CHAR* host, UINT port)
{
	DEBUG_TRY
    //copy host to m_host
    strncpy(m_Host, host, IP_SIZE);
    m_Port = port;

    // try to connect to peer host
    return connect();
	DEBUG_CATCHF("Socket::connect")
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	close previous connection and connect to another server socket
//
//-----input------
// Parameters
//     	host 			:	需要连接的host
//     	port 			:	需要连接的port
//------output------
// Return 
//     成功：TRUE 失败:FALSE
//
BOOL Socket::reconnect(const CHAR* host, UINT port)
{
	DEBUG_TRY
    //close
    close();

    //create
    if(create())
    {
        //copy host to m_host
        strncpy(m_Host, host, IP_SIZE);
        m_Port = port;

        // try to connect to peer host
        return connect();	
    }	
	DEBUG_CATCH("Socket::reconnect")
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	向socket的缓存中发送数据（send data to peer）
//
//-----input------
// Parameters
//		buff		:	用户发送缓存区指针（send buffer）
//		len 		:	请求写入的字节数（send data length）
//		flags		:	send flag (MSG_OOB、MSG_DONTROUTE、MSG_DONTWAIT、MSG_WAITALL)
//------output------
// Return 
//     返回值：成功返回写入的字节数，出错返回-1并设置errno
//
INT Socket::send(const VOID* buf, UINT len, UINT flags/*=0*/)
{
	return SocketAPI::send_ex(m_SocketID, buf, len, flags);	
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	从socket的缓存中接收数据，（receive data from peer）
//
//-----input------
// Parameters
//		buff		:	用来存放recv函数接收到的数据的缓冲区（recv buffer）
//		len 		:	指明buff的长度（recv data length）
//		flags		:	recv flag (MSG_OOB、MSG_DONTROUTE、MSG_DONTWAIT、MSG_WAITALL)
//------output------
// Return 
//     返回值：成功返回读取的字节数，出错返回-1并设置errno，无数据可以读取，则返回0
//
INT Socket::receive(VOID* buf, UINT len, UINT flags/*=0*/)
{
	return SocketAPI::recv_ex(m_SocketID, buf, len, flags);
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	get amount of data in socket input buffer
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//     返回值：成功返回缓存区的字节数，出错返回-1并设置errno，无数据可以读取，则返回0
//
INT Socket::available() const
{
	return SocketAPI::availablesocket_ex(m_SocketID);
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	由TCP服务器调用，用于从已完成连接队列头返回下一个已完成连接，如果已经完成连接队列为空，那么进程被投入睡眠（阻塞模式）
//
//-----input------
// Parameters
//	   	addr		:	用来返回已连接的对端进程（客户）的协议地址，调用前，我们将由*addrlen所引用的整数值置为由cliaddr所置的套接字地址结构的长度
//		len 		:	返回时，该整数值纪委由内核存放在该套接字地址结构内的确切字节数
//------output------
// Return 
//     INT 		:	成功：非负描述符 失败：-1
//
SOCKET Socket::accept(struct sockaddr* addr, UINT* len)
{
	return SocketAPI::accept_ex(m_SocketID, addr, len);
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	把一个本地协议赋予给一个套接字
//
//-----input------
// Parameters
//     	无
//------output------
// Return 
//     成功：TRUE 失败：FALSE
//
// BOOL Socket::bind()
// {
//     DEBUG_TRY
//     //m_SockAddr.sin_family = AF_INET;  //设置地址家族
//     m_SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);//监听“所有地址”
// 	m_SockAddr.sin_port        = htons(m_Port);//设置端口
// 	BOOL result = SocketAPI::bind_ex(m_SocketID, (const struct sockaddr *)&m_SockAddr, sizeof(m_SockAddr));
// 	if (result)
// 	{
// 		return TRUE;
// 	}
// 	else
// 	{
// 		return FALSE;
// 	}
//     DEBUG_CATCH("Socket::bind")
// 	return FALSE;
// }

//////////////////////////////////////////////////////////////////////
//
// func description	:	把一个本地协议赋予给一个套接字
//
//-----input------
// Parameters
//     	port 			:	特定端口
//------output------
// Return 
//     成功：TRUE 失败：FALSE
//
BOOL Socket::bind(UINT port, UINT nAddr/* = 0*/)
{	
    BOOL result = FALSE;
    m_Port = port;

    if (g_config.ONLINE_LISTEN_IPV_MODULE) //  IPV6
    {
        //!alter by huyf 2018.08.01:支持双栈支持IPV4和IPV6  
        LOGMSG("[Socket::bind IPV6] Listen Addr = %d", nAddr);  
        bzero(&m_SockAddr6, sizeof(m_SockAddr6));   
        m_SockAddr6.sin6_family = AF_INET6;    // IPv6   
        m_SockAddr6.sin6_port = htons(m_Port);   // IPv6 
        //地址可以配置
        if (nAddr != 0)  
        {
            //m_SockAddr.sin_addr.s_addr = inet_addr(nAddr); // IPv4  
            //inet_pton(AF_INET6, nAddr, &m_SockAddr6.sin6_addr);  // IPv6  
        }
        else  
        {
            //m_SockAddr.sin_addr.s_addr = INADDR_ANY;  // IPv4  
            m_SockAddr6.sin6_addr = in6addr_any;            // IPv6 in6addr_any 系统预先分配in6addr_any变量并将其初始化为IN6ADDR_ANY_INIT.头文件<netinet/in.h>中含有extern声明
        }   
        result = SocketAPI::bind_ex(m_SocketID, (const struct sockaddr *)&m_SockAddr6, sizeof(m_SockAddr6));   
        //!alter end:支持双栈支持IPV4和IPV6
    }
    else    //  IPV4
    {
         //m_SockAddr.sin_family = AF_INET;  //设置地址家族
        //add by huyf 2018.04.03:添加地址可以配置
        LOGMSG("[Socket::bind IPV4] Listen Addr = %d", nAddr);
        if (nAddr != 0)
        {
            m_SockAddr.sin_addr.s_addr = htonl(nAddr);//监听特定IP
        }
        else
        {
            m_SockAddr.sin_addr.s_addr = htonl(INADDR_ANY);//监听“所有地址”
        }
        //add end:添加地址可以配置

        m_SockAddr.sin_port        = htons(m_Port);//设置端口
        result = SocketAPI::bind_ex(m_SocketID, (const struct sockaddr *)&m_SockAddr, sizeof(m_SockAddr));
    }
	
	return result;	
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	把套接字（s）从CLOSE状态装换到LISENT状态，设置套接字为被动套接字,指示内核接收向该套接字的连接请求
//
//-----input------
// Parameters
//	   	backlog		:	规定了内核应该为相应套接字排队的最大连接个数
//------output------
// Return 
//     成功：TRUE 失败：FALSE
//
BOOL Socket::listen(INT backlog)
{	
    	BOOL result = SocketAPI::listen_ex(m_SocketID, backlog);
    	return result;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	若有数据，获取socket的延迟关闭
//
//-----input------
// Parameters
//	   	无
//------output------
// Return 
//     成功：TRUE 失败：FALSE
/*
struct linger
{
int l_onoff;
int l_linger;
};
l_onoff = 0(l_linger忽略)：优雅关闭
l_onoff ！= 0
	(l_linger=0)： 	在调用closesocket的时候同样会立刻返回，但不会发送未发送完成的数据，而是通过一个REST包强制的关闭socket描述符，也就是强制的退出,不经历TIME_WAIT的过程
	(l_linger！=0)：在调用closesocket的时候不会立刻返回，内核会延迟一段时间，这个时间就由l_linger得值来决定。如果超时时间到达之前，
					发送完未发送的数据(包括FIN包)并得到另一端的确认，closesocket会返回正确，socket描述符优雅性退出。否则，closesocket会直接返回错误值，
					未发送数据丢失，socket描述符被强制性退出。需要注意的时，如果socket描述符被设置为非堵塞型，则closesocket会直接返回值。

*/
//
UINT Socket::getLinger() const
{
	struct linger ling;
	INT len = sizeof(ling);
	BOOL result = SocketAPI::getsocketopt_ex(m_SocketID, SOL_SOCKET, SO_LINGER, (VOID*)&ling, &len);
	if (result)
	{
		return ling.l_linger;
	}
	else
	{
		return FALSE;
	}
}


//////////////////////////////////////////////////////////////////////
//
// func description	:	若有数据，获取socket的延迟关闭
//
//-----input------
// Parameters
//	   	lingertime		:		linger 标识
//------output------
// Return 
//     成功：TRUE 失败：FALSE
//
BOOL Socket::setLinger(UINT lingertime)
{
	struct linger ling;
    ling.l_onoff = lingertime > 0 ? 1 : 0;
    ling.l_linger = lingertime;
    UINT len = sizeof(ling);

    //设置优雅的断开和强制断开
	BOOL result = SocketAPI::setsocketopt_ex(m_SocketID, SOL_SOCKET, SO_LINGER, (const VOID*)&ling, len);
	return result;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	get/set socket's nonblocking status
//
//-----input------
// Parameters
//	   	无
//------output------
// Return 
//     成功：TRUE 失败：FALSE
//
BOOL Socket::isNonBlocking()const 
{ 
	return SocketAPI::getsocketnonblocking_ex(m_SocketID);
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	get/set socket's nonblocking status
//
//-----input------
// Parameters
//	   	on		:		开关
//------output------
// Return 
//     成功：TRUE 失败：FALSE
//
BOOL Socket::setNonBlocking(BOOL on) 
{ 
	return SocketAPI::setsocketnonblocking_ex(m_SocketID, on);
	
}
//////////////////////////////////////////////////////////////////////
//
// func description	:	周期性测试连接是否仍存活
//
//-----input------
// Parameters
//	   	无
//------output------
// Return 
//     成功：TRUE 失败：FALSE
//
BOOL Socket::isKeepAlive() const
{
	INT keepalive = 0;
	INT len = sizeof(keepalive);
	BOOL result = SocketAPI::getsocketopt_ex(m_SocketID, SOL_SOCKET, SO_KEEPALIVE, (VOID*)&keepalive, &len);
	return result;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	周期性测试连接是否仍存活
//
//-----input------
// Parameters
//	   	on 				:	开启或者关闭
//------output------
// Return 
//     成功：TRUE 失败：FALSE
//
BOOL Socket::setKeepAlive(BOOL on/*=TRUE*/)
{
	INT keepalive = on > 0 ? 1 : 0;
	UINT len = sizeof(keepalive);
	//设置优雅的断开和强制断开
	BOOL result = SocketAPI::setsocketopt_ex(m_SocketID, SOL_SOCKET, SO_KEEPALIVE, (const VOID*)&keepalive, len);
	return result;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	允许重用本地地址
//
//-----input------
// Parameters
//	   	无
//------output------
// Return 
//     成功：TRUE 失败：FALSE
//
BOOL Socket::isReuseAddr() const
{
	INT reuse = 0;
	INT len = sizeof(reuse);
	BOOL result = SocketAPI::getsocketopt_ex(m_SocketID, SOL_SOCKET, SO_REUSEADDR, (VOID*)&reuse, &len);
	if (result)
	{
		if (reuse != 0)
		{
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		return FALSE;
	}
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	允许重用本地地址
//
//-----input------
// Parameters
//	   	on 				:	开启或者关闭
//------output------
// Return 
//     成功：TRUE 失败：FALSE
//
BOOL Socket::setReuseAddr(BOOL on/*=TRUE*/)
{
	INT reuse = on > 0 ? 1 : 0;
	UINT len = sizeof(reuse);
	//设置优雅的断开和强制断开
	BOOL result = SocketAPI::setsocketopt_ex(m_SocketID, SOL_SOCKET, SO_REUSEADDR, (const VOID*)&reuse, len);
	return result;    	
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	允许重用本地端口
//
//-----input------
// Parameters
//	   	无
//------output------
// Return 
//     成功：TRUE 失败：FALSE
//
BOOL Socket::isReusePort() const
{
	INT reuse = 0;
	INT len = sizeof(reuse);
	BOOL result = SocketAPI::getsocketopt_ex(m_SocketID, SOL_SOCKET, SO_REUSEPORT, (VOID*)&reuse, &len);
	if (result)
	{
		if (reuse != 0)
		{
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		return FALSE;
	}
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	允许重用本地地址
//
//-----input------
// Parameters
//	   	on 				:	开启或者关闭
//------output------
// Return 
//     成功：TRUE 失败：FALSE
//
BOOL Socket::setReusePort(BOOL on/*=TRUE*/)
{
	INT reuse = on > 0 ? 1 : 0;
	UINT len = sizeof(reuse);
	//设置优雅的断开和强制断开
	BOOL result = SocketAPI::setsocketopt_ex(m_SocketID, SOL_SOCKET, SO_REUSEPORT, (const VOID*)&reuse, len);
	return result;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	接收缓冲区低水位标记
//
//-----input------
// Parameters
//	   	无
//------output------
// Return 
//     INT 标记
//
INT Socket::getRcvLowAt() const
{
	INT rcvlowat = 0;
	INT len = sizeof(rcvlowat);
	BOOL result = SocketAPI::getsocketopt_ex(m_SocketID, SOL_SOCKET, SO_RCVLOWAT, (VOID*)&rcvlowat, &len);
	if (result)
	{
		return rcvlowat;
	}
	else
	{
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	接收缓冲区低水位标记
//
//-----input------
// Parameters
//	   	on 				:	开启或者关闭
//------output------
// Return 
//     成功：TRUE 失败：FALSE
//
BOOL Socket::setRcvLowAt(BOOL on/*=TRUE*/)
{
	INT rcvlowat = on > 0 ? 1 : 0;
	UINT len = sizeof(rcvlowat);
	//设置优雅的断开和强制断开
	BOOL result = SocketAPI::setsocketopt_ex(m_SocketID, SOL_SOCKET, SO_RCVLOWAT, (const VOID*)&rcvlowat, len);
	return result;
}


//////////////////////////////////////////////////////////////////////
//
// func description	:	送缓冲区低水位标记
//
//-----input------
// Parameters
//	   	无
//------output------
// Return 
//     INT 标记
//
INT Socket::getSendLowAt() const
{
	INT sendlowat = 0;
	INT len = sizeof(sendlowat);
	BOOL result = SocketAPI::getsocketopt_ex(m_SocketID, SOL_SOCKET, SO_SNDLOWAT, (VOID*)&sendlowat, &len);
	if (result)
	{
		return sendlowat;
	}
	else
	{
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	送缓冲区低水位标记
//
//-----input------
// Parameters
//	   	on 				:	开启或者关闭
//------output------
// Return 
//     成功：TRUE 失败：FALSE
//
BOOL Socket::setSendLowAt(BOOL on/*=TRUE*/)
{
	INT sendlowat = on > 0 ? 1 : 0;
	UINT len = sizeof(sendlowat);
	//设置优雅的断开和强制断开
	BOOL result = SocketAPI::setsocketopt_ex(m_SocketID, SOL_SOCKET, SO_SNDLOWAT, (const VOID*)&sendlowat, len);
	return result;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	接收超时
//
//-----input------
// Parameters
//	   	无
//------output------
// Return 
//     返回timeval结构的引用
//
BOOL Socket::getRcvTimeO(struct timeval& timeout)
{
	INT len = sizeof(timeout);
	BOOL result = SocketAPI::getsocketopt_ex(m_SocketID, SOL_SOCKET, SO_RCVTIMEO, (VOID*)&timeout, &len);
	return result;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	接收超时
//
//-----input------
// Parameters
//	   	timeout 				:	timeval结构变量
//------output------
// Return 
//     成功：TRUE 失败：FALSE
//
BOOL Socket::setRcvTimeO(struct timeval& timeout)
{
	UINT len = sizeof(timeout);
	//设置优雅的断开和强制断开
	BOOL result = SocketAPI::setsocketopt_ex(m_SocketID, SOL_SOCKET, SO_RCVTIMEO, (const VOID*)&timeout, len);
	return result;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	发送超时
//
//-----input------
// Parameters
//	   	无
//------output------
// Return 
//     返回timeval结构的引用
//
BOOL Socket::getSendTimeO(struct timeval& timeout)
{
	//timeout.tv_sec = 0;//s
	//timeout.tv_usec = 0;//ms
	INT len = sizeof(timeout);
	BOOL result = SocketAPI::getsocketopt_ex(m_SocketID, SOL_SOCKET, SO_SNDTIMEO, (VOID*)&timeout, &len);
	return result;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	发送超时
//
//-----input------
// Parameters
//	   	timeout 				:	timeval结构变量
//------output------
// Return 
//     成功：TRUE 失败：FALSE
//
BOOL Socket::setSendTimeO(struct timeval& timeout)
{
	UINT len = sizeof(timeout);
	//设置优雅的断开和强制断开
	BOOL result = SocketAPI::setsocketopt_ex(m_SocketID, SOL_SOCKET, SO_SNDTIMEO, (const VOID*)&timeout, len);
	return result;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	get receive buffer size
//
//-----input------
// Parameters
//	   	无
//------output------
// Return 
//     缓冲区长度
//
UINT Socket::getReceiveBufferSize() const
{
	UINT size = 0;
	INT len = sizeof(size);
	BOOL result = SocketAPI::getsocketopt_ex(m_SocketID, SOL_SOCKET, SO_RCVBUF, (VOID*)&size, &len);
	if (result)
	{
		return size;
	}
	else
	{
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	set receive buffer size
//
//-----input------
// Parameters
//	   	size 				:	缓冲区大小
//------output------
// Return 
//     成功：TRUE 失败：FALSE
//
BOOL Socket::setReceiveBufferSize(UINT size)
{
	UINT len = sizeof(size);
	//设置优雅的断开和强制断开
	BOOL result = SocketAPI::setsocketopt_ex(m_SocketID, SOL_SOCKET, SO_RCVBUF, (const VOID*)&size, len);
	return result;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	get send buffer size
//
//-----input------
// Parameters
//	   	无
//------output------
// Return 
//     缓冲区长度
//
UINT Socket::getSendBufferSize() const
{
	UINT size = 0;
	INT len = sizeof(size);
	BOOL result = SocketAPI::getsocketopt_ex(m_SocketID, SOL_SOCKET, SO_SNDBUF, (VOID*)&size, &len);
	if (result)
	{
		return size;
	}
	else
	{
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	set send buffer size
//
//-----input------
// Parameters
//	   	size 				:	缓冲区大小
//------output------
// Return 
//     成功：TRUE 失败：FALSE
//
BOOL Socket::setSendBufferSize(UINT size)
{
	UINT len = sizeof(size);
	//设置优雅的断开和强制断开
	BOOL result = SocketAPI::setsocketopt_ex(m_SocketID, SOL_SOCKET, SO_SNDBUF, (const VOID*)&size, len);
	return result;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	获得TCP禁止nagle算法开启标识
//
//-----input------
// Parameters
//	   	无
//------output------
// Return 
//     成功：TRUE 失败：FALSE
//
BOOL Socket::isNoDelay() const
{
	INT nodelay = 0;
	INT len = sizeof(nodelay);
	BOOL result = SocketAPI::getsocketopt_ex(m_SocketID, IPPROTO_TCP, TCP_NODELAY, (VOID*)&nodelay, &len);
	if (result)
	{
		if (nodelay != 0)
		{
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		return FALSE;
	}
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	设置TCP禁止nagle算法开启标识
//
//-----input------
// Parameters
//	   	on 				:	开启或者关闭
//------output------
// Return 
//     成功：TRUE 失败：FALSE
//
BOOL Socket::setNoDelay(BOOL on)
{
	INT nodelay = on > 0 ? 1 : 0;
	UINT len = sizeof(nodelay);
	BOOL result = SocketAPI::setsocketopt_ex(m_SocketID, IPPROTO_TCP, TCP_NODELAY, (const VOID*)&nodelay, len);
	return result;	
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	获取TCP最大分节大小
//
//-----input------
// Parameters
//	   	无
//------output------
// Return 
//     返回mss长度
//
INT Socket::getMaxSeg() const
{
	INT maxseg = 0;
	INT len = sizeof(maxseg);
	BOOL result = SocketAPI::getsocketopt_ex(m_SocketID, IPPROTO_TCP, TCP_MAXSEG, (VOID*)&maxseg, &len);
	if (result)
	{
		if (maxseg > 0)
		{
			return maxseg;
		}
		return 0;
	}
	else
	{
		return 0;
	}
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	设置TCP最大分节大小
//
//-----input------
// Parameters
//	   	size 				:	大小长度（字节）
//------output------
// Return 
//     成功：TRUE 失败：FALSE
//
BOOL Socket::setMaxSeg(INT size)
{
	INT maxseg = size;
	UINT len = sizeof(maxseg);
	BOOL result = SocketAPI::setsocketopt_ex(m_SocketID, IPPROTO_TCP, TCP_MAXSEG, (const VOID*)&maxseg, len);
	return result;
}

//////////////////////////////////////////////////////////////////////
//
// func description :   不经历TIME_WAIT的过程
//
//-----input------
// Parameters
//      on              :   开启或者关闭
//------output------
// Return 
//     成功：TRUE 失败：FALSE
//
BOOL Socket::setDontLinger(BOOL on/*=TRUE*/)
{
    return TRUE;
    //UINT len = sizeof(on);
    //BOOL result = SocketAPI::setsocketopt_ex(m_SocketID, SOL_SOCKET, SO_DONTLINER , (const VOID*)&on, len);
    //return result;
}
   
//////////////////////////////////////////////////////////////////////
//
// func description	:	get socket error
//
//-----input------
// Parameters
//	   	无
//------output------
// Return 
//     返回错误标识
//
INT Socket::isSockError() const
{
	INT error = 0;
    INT len = sizeof(error);
    SocketAPI::getsocketopt_ex(m_SocketID, SOL_SOCKET, SO_ERROR, &error, &len);
    return error;	
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	get socket error
//
//-----input------
// Parameters
//	   	无
//------output------
// Return 
//     返回错误标识
//
UINT Socket::getSocketError()
{
	return isSockError();
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	check if socket is valid
//
//-----input------
// Parameters
//	   	无
//------output------
// Return 
//     返回错误标识
//
BOOL Socket::isValid() const
{
	return m_SocketID != INVALID_SOCKET;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	get socket discreption
//
//-----input------
// Parameters
//	   	无
//------output------
// Return 
//     返回错误标识
//
//
SOCKET Socket::getSocketID()
{
	return m_SocketID;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	get socket discreption
//
//-----input------
// Parameters
//	   	无
//------output------
// Return 
//     返回错误标识
//
//
IP_t Socket::getHostIP() const
{
	return (IP_t)(m_SockAddr.sin_addr.s_addr);
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	get socket discreption
//
//-----input------
// Parameters
//	   	无
//------output------
// Return 
//     返回错误标识
//
//
UINT Socket::getPort() const
{
	return m_Port;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	get sockaddr IPV4
//
//-----input------
// Parameters
//	   	无
//------output------
// Return 
//     返回错误标识
//
//
SOCKADDR_IN& Socket::getSocketAddIn()
{
	return m_SockAddr;
}

//!alter by huyf 2018.08.01:支持双栈支持IPV4和IPV6
//////////////////////////////////////////////////////////////////////
//
// func description :   get sockaddr IPV6
//
//-----input------
// Parameters
//      无
//------output------
// Return 
//     返回错误标识
//
//
SOCKADDR_IN6& Socket::getSocketAddIn6()
{
    return m_SockAddr6;
}
//!alter end:支持双栈支持IPV4和IPV6

//////////////////////////////////////////////////////////////////////
//
// func description	:	该函数生成一个epoll专用的文件描述符
//
//-----input------
// Parameters
//	   	int size: 其中的参数是指定生成描述符的最大范围。
//------output------
// Return 
//     一个epoll专用的文件描述符
//
#ifdef _EPOLL_
int Socket::epollCreate(int size)
{
    // 创建一个epoll句柄  
    m_EpollFd = epoll_create(size);  
    return m_EpollFd;
}
#endif

/////////////////////////////////////////////////////////////////////
//
// func description	:	用于控制某个文件描述符上的事件，可以注册事件、修改事件、删除事件
//                      
//-----input------
// Parameters
//          epfd：由 epoll_create 生成的epoll专用的文件描述符； 
//          op：要进行的操作，可能的取值EPOLL_CTL_ADD 注册、EPOLL_CTL_MOD 修改、EPOLL_CTL_DEL 删除； 
//          fd：关联的文件描述符； 
//          event：指向epoll_event的指针； 
//------output------
// Return 
//          如果调用成功则返回0，不成功则返回-1。
// 

#ifdef _EPOLL_
int Socket::epollCtl(int epfd, int op, int fd, struct epoll_event *event)
{ 
    return epoll_ctl(epfd, op, fd, event);  
}
#endif

/////////////////////////////////////////////////////////////////////
//
// func description	:	该函数用于轮询I/O事件的发生。
//                      
//-----input------
// Parameters
//          epfd：由epoll_create 生成的epoll专用的文件描述符； 
//          epoll_event：用于回传代处理事件的数组； 
//          maxevents：每次能处理的事件数； 
//          timeout：等待I/O事件发生的超时值； 
//------output------
// Return 
//          返回发生事件数。
//   

//#ifdef _EPOLL_
//int Socket::epollWait(int epfd, struct epoll_event * events, int maxevents, int timeout)
//{
//    return epoll_wait(epfd, events, maxevents, timeout); 
//}
//#endif 

/////////////////////////////////////////////////////////////////////
//
// func description :   该函数用于设置socket各项属性
//                      
//-----input------
// Parameters
//          bLinger： 
//------output------
// Return 
//          返回成功失败
//  
BOOL Socket::setSocketOpt(UINT bLinger/*=TRUE*/, BOOL bReuseAddr/*=TRUE*/, BOOL bReusePort/*=TRUE*/, BOOL bNoDelay/*=TRUE*/, BOOL bKeepAlive/*=TRUE*/, BOOL bNonBlocking/*=TRUE*/, BOOL bDontLinger/*=TRUE*/,UINT unSendBuffSize/*=0*/, UINT unRecvBuffSize/*=0*/)
{
    BOOL iRet = FALSE;
    //设置关闭模式   
    if (bLinger)
     {
        iRet = setLinger(bLinger);
        if (!iRet)
        {
            LOGERROR("[Socket::setSocketOpt] socket setLinger is Error iRet=[%d] errno[%d] discription[%s]",iRet, errno, strerror(errno));
            return FALSE;
        }
    }     

    //设置地址重用模式
    if (bReuseAddr)
    {
        iRet = setReuseAddr(bReuseAddr);
        if (!iRet)
        {
            LOGERROR("[Socket::setSocketOpt] socket setReuseAddr is Error iRet=[%d] errno[%d] discription[%s]",iRet, errno, strerror(errno));
            return FALSE;
        }
    }    

    //设置端口重用模式
    if (bReusePort)
    {
        iRet = setReusePort(bReusePort);
        if (!iRet)
        {
            LOGERROR("[Socket::setSocketOpt] socket setReusePort is Error iRet=[%d] errno[%d] discription[%s]",iRet, errno, strerror(errno));
            return FALSE;
        }
    }
    

    //禁止nagle算法
    if (bNoDelay)
    {
       iRet = setNoDelay(bNoDelay);
        if (!iRet)
        {
            LOGERROR("[Socket::setSocketOpt] socket setNoDelay is Error iRet=[%d] errno[%d] discription[%s]",iRet, errno, strerror(errno));
            return FALSE;
        }
    }
    

    //keepalive
    if (bKeepAlive)
    {
       iRet = setKeepAlive(bKeepAlive);
        if (!iRet)
        {
            LOGERROR("[Socket::setSocketOpt] socket setKeepAlive is Error iRet=[%d] errno[%d] discription[%s]",iRet, errno, strerror(errno));
            return FALSE;
        }
    }

    //非阻赛模式提到connect后面,否则会115错误.LINUX socket 在connect的时候发生 Operation now in progress 错误.conect为阻塞模式
    //设置非阻塞模式
    if(bNonBlocking)//true:非阻塞模式 默认阻塞模式
    {
        iRet = setNonBlocking(bNonBlocking);
        if (!iRet)
        {
            LOGERROR("[Socket::setSocketOpt] socket setNonBlocking is Error iRet=[%d] errno[%d] discription[%s]",iRet, errno, strerror(errno));
            return FALSE;
        }
    }
    

    //DontLinger
    /*
    iRet = setDontLinger(bDontLinger);
    if (!iRet)
    {
        LOGERROR("[Socket::setSocketOpt] socket setDontLinger is Error iRet=[%d] errno[%d] discription[%s]",iRet, errno, strerror(errno));
        return FALSE;
    }
    */

    //SO_SNDBUF
    /*
    iRet = setSendBufferSize(unSendBuffSize);
    if (!iRet)
    {
        LOGERROR("[Socket::setSocketOpt] socket setSendBufferSize is Error iRet=[%d] errno[%d] discription[%s]",iRet, errno, strerror(errno));
        return FALSE;
    }
    */

    //SO_RCVBUF****此处一般不设置，由syn对端同步
    /*
    iRet = setReceiveBufferSize(unRecvBuffSize);
    if (!iRet)
    {
        LOGERROR("[Socket::setSocketOpt] socket setReceiveBufferSize is Error iRet=[%d] errno[%d] discription[%s]",iRet, errno, strerror(errno));
        return FALSE;
    }
    */

    return TRUE;
}

//add by huyf 2018.11.23:player出的websocket接受数据处理迁移
/////////////////////////////////////////////////////////////////////
//
// func description :   处理网络消息
//                      
//-----input------
// Parameters
//          bLinger： 
//------output------
// Return 
//          返回成功失败
//  
INT Socket::ReadNetData(SocketInputStream* pSocketInputStream, INT& nPackageID, UINT& nNeedReadTotalLengh)
{
    if (!pSocketInputStream)
    {
        return -2;
    }

    if (!m_handler)
    {
        return -2;
    }

    if (!m_handler->GetRequest())
    {
        return -2;
    }

    UINT nWebSocketBaseHeadLengh = 6;

    ProtocolHead14Byte ph14Byte;
    memset(&ph14Byte, 0, sizeof(ph14Byte));

    ProtocolHead16Byte ph16Byte;
    memset(&ph16Byte, 0, sizeof(ph16Byte));

    //先读取前6个字节，解析出来数据长度
    INT nAddSize = 0;
    char szWebSocketHead[8];
    memset(szWebSocketHead, 0, 8);
    if (!pSocketInputStream->Peek(szWebSocketHead, nWebSocketBaseHeadLengh))
    {
        //break; 
        return -1;    
    }
    else    //读取到websocket头部6个字节，进行解析
    {
        INT opcode_ = szWebSocketHead[0] & 0x0f;
        if (opcode_ == WEBSOCKET_CONNECT_CLOSE)
        {
            LOGERROR("[Player::ProcessCommand]  websocket主动断开连接。");
            m_handler->SetStatus(WEBSOCKET_UNCONNECT);
            //return FALSE;
            return -2;
        }

        INT payload_length_ = szWebSocketHead[1] & 0x7f;////[key_4][key_3][key_2][mask_]111 1111[fin_]111 [opcode_1111]
        if(payload_length_ == 126)//[1111 1111][1111 1111][0111 1111][key_4][key_3][key_2][mask_]111 1111[fin_]111 1111
        {
                                    //{data}{[1111 1111][1111 1111]}{[0111 1110]}{[key_4][key_3][key_2][mask_]111 1111}{[fin_]111 1111}
            nAddSize += 2;
        }
        else if(payload_length_ == 127)
        {        
                           //{data}{[1111 1111][1111 1111][1111 1111][1111 1111]} {[0111 1111]}{[key_4][key_3][key_2][mask_]111 1111}{[fin_]111 1111}
            nAddSize += 4;
        }
    }

    if (nAddSize == 0)
    {
        if(!pSocketInputStream->Peek((CHAR*)&ph14Byte, (UINT)sizeof(ph14Byte)))
        {
            //LOGERROR("缓存中的数据长度不够协议头的长度...socketid=[%u]\t数据还不够，需要继续接收-----------",GetSocket()->getSocketID());                        
            //continue;
            return 0;
        }
        m_handler->GetRequest()->FetchWebsocketInfo((CHAR*)&ph14Byte);
    }
    else if (nAddSize == 2)
    {
        if(!pSocketInputStream->Peek((CHAR*)&ph16Byte, (UINT)sizeof(ph16Byte)))
        {
            //LOGERROR("缓存中的数据长度不够协议头的长度...socketid=[%u]\t数据还不够，需要继续接收-----------",GetSocket()->getSocketID());                        
            //continue;
            return 0;
        }
        m_handler->GetRequest()->FetchWebsocketInfo((CHAR*)&ph16Byte);
    }
    else if (nAddSize == 4)
    {
        m_handler->SetStatus(WEBSOCKET_UNCONNECT);
        //return FALSE;
        return -2;
    }
    else
    {
        LOGERROR("[Player::ProcessCommand]  websocket主动断开连接。nAddSize == undefined");
        m_handler->SetStatus(WEBSOCKET_UNCONNECT);
        //return FALSE;
        return -2;
    }

    std::string request_msg = m_handler->GetRequest()->GetMsg();
    size_t   request_msg_len= m_handler->GetRequest()->GetMsgLength();

    string strPackageID = request_msg.substr(0, 4);   //1001
    //LOGERROR("strPackageId = [ %s ]\n", strPackageID.c_str());
    nPackageID = atoi(strPackageID.c_str());

    string strPackageLen = request_msg.substr(4, 4);   //83
    //LOGERROR("strPackageLen = [ %s ]\n", strPackageLen.c_str());
    //INT nPackageLen = atoi(strPackageLen.c_str());

    nNeedReadTotalLengh = nWebSocketBaseHeadLengh/*基础websocket长度*/ + request_msg_len/*应用层数据长度*/ + nAddSize/*大包websocket额外增加的长度*/;

    //LOGERROR("缓存中的数据长度=[%d] nNeedReadTotalLengh=[%d] ph14Byte=[%d] ph16Byte=[%d] nWebSocketBaseHeadLengh=[%d] nPackageLen=[%d] nAddSize=[%d] request_msg=[%s]- request_msg_len=[%d]",
    //    nHaveDataLen, nNeedReadTotalLengh, sizeof(ph14Byte), sizeof(ph16Byte), nWebSocketBaseHeadLengh, nPackageLen, nAddSize, request_msg, request_msg_len);

    return 1;
}       

/////////////////////////////////////////////////////////////////////
//
// func description :   处理网络写数据到应用层缓存区
//                      
//-----input------
// Parameters
//          SocketOutputStream*： pSocketOutputStream 玩家输出缓存指针 
//          CONST CHAR* ：m_pData 待发送的数据缓存指针
//          UINT : m_nDataLen 待发送的数据长度
//------output------
// Return 
//          返回成功:TRUE 失败:FALSE
// 
BOOL Socket::Write(SocketOutputStream* pSocketOutputStream, const CHAR* pData, UINT nDataLen) 
{
    if (!pSocketOutputStream)
    {
        return FALSE;
    }

    if (!m_handler)
    {
        return FALSE;
    }

    if (!m_handler->GetRespond())
    {
        return FALSE;
    }

    unsigned char* out = NULL;
    size_t out_len = 0;
    m_handler->GetRespond()->PackData((const unsigned char*)pData, nDataLen, WEBSOCKET_FIN_MSG_END, WEBSOCKET_TEXT_DATA, WEBSOCKET_NEED_NOT_MASK, &out, &out_len);

    pSocketOutputStream->Write((const char*)out, out_len);
    pSocketOutputStream->Flush();

    free(out);
    return TRUE;
}

/////////////////////////////////////////////////////////////////////
//
// func description :   处理网络写数据发送给前端
//                      
//-----input------
// Parameters
//          SocketOutputStream*： pSocketOutputStream 玩家输出缓存指针 
//          CNetMsg* : pMsg 待发送的消息
//------output------
// Return 
//          返回成功:TRUE 失败:FALSE
// 
BOOL Socket::SendMsgToClient(SocketOutputStream* pSocketOutputStream, CNetMsg* pMsg)
{
    if (!pSocketOutputStream)
    {
        return FALSE;
    }

    if (!m_handler)
    {
        return FALSE;
    }

    if (!m_handler->GetRespond())
    {
        return FALSE;
    }

    if (!pMsg)
    {
        LOGERROR("[Socket::SendMsgToClient] pMsg == NULL!");
        return FALSE;
    }

    INT nPacketID = pMsg->GetPacketID();
    string strMsg(pMsg->m_pData);
    INT nSize = strMsg.size();

    UINT msgLen = strMsg.size()+8+1;  // adding \0 结尾       
    char* msg = (char*)malloc(msgLen);
    memset(msg, 0, msgLen);

    //LOGDEBUG("msg = [%s] msgLen=[%d]\n ", msg, msgLen);

    string strPacketId = std::to_string(nPacketID);
    string strSize = std::to_string(nSize);
    //检查长度不能超过4位
    if (strSize.size() > 4)
    {
        SAFE_FREE(msg);
        LOGERROR("[Socket::SendMsgToClient] Msg Size Is OverFlow > 4 Size=[%s]", strSize.c_str());
        return FALSE;
    }
    //前端不足4位数补足"0"
    int nHavaZero = 4 - strlen(strSize.c_str());
    string strKongZero = strSize;
    if (nHavaZero > 0)
    {
        strKongZero = string(nHavaZero,'0') + strSize;
    }
    //LOGDEBUG("strPacketId = [%s] strKongZero=[%s]\n ", strPacketId.c_str(), strKongZero.c_str());

    memcpy(msg, (void*)strPacketId.c_str(), 4);
    memcpy(msg + 4, (void*)strKongZero.c_str(), 4);
    memcpy(msg + 8, (void*)strMsg.c_str(), strMsg.size());  
    //LOGDEBUG("msg = [%s] size=[%d]\n ", msg, strlen(msg));

    //Send Packet To Client
    unsigned char* out = NULL;
    size_t out_len = 0;
    m_handler->GetRespond()->PackData((const unsigned char*)msg, strlen(msg), WEBSOCKET_FIN_MSG_END, WEBSOCKET_TEXT_DATA, WEBSOCKET_NEED_NOT_MASK, &out, &out_len);
    
    //!alter by huyf 2018.11.26:做一些发送失败的处理
    if(pSocketOutputStream->Write((const char*)out, out_len) == 0)
    {
        //刷新一次缓存
        pSocketOutputStream->Flush();
        LOGERROR("[Socket::SendMsgToClient] Msg Write Is Error out_len=[%s] ", out_len);

        if (out)
        {
            free(out);
            out = NULL;
        }

        SAFE_FREE(msg);

        return FALSE;
    }
    //!alter end:做一些发送失败的处理

    //刷新一次缓存
    pSocketOutputStream->Flush();

    if (out)
    {
        free(out);
        out = NULL;
    }

    SAFE_FREE(msg);
    return TRUE;
    
}
//add end:player出的websocket接受数据处理迁移