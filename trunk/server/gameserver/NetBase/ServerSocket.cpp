////////////////////////////////////////////////////////////////
//文件名称：ServerSocket.cpp
//功能描述：服务器端socket
//文件名称：ServerSocket API
//			
//编写作者：2017.08.15 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
// include files
////////////////////////////////////////////////////////////////
#include "ServerSocket.h"
//!alter by huyf 2018.08.01:支持双栈支持IPV4和IPV6
#include "../Common/Config.h"
//!alter end:支持双栈支持IPV4和IPV6
////////////////////////////////////////////////////////////////
//常量
////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//
// func description	:	constructor
//
//-----input------
// Parameters
//	   	port 			: 	端口
//	   	backlog			:	规定了内核应该为相应套接字排队的最大连接个数
//------output------
// Return 
//     出错抛出异常
//
ServerSocket::ServerSocket()
{
    // create socket implementation object
    m_pSocket = new Socket();
    CHECK(m_pSocket);   
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
ServerSocket::~ServerSocket()
{	
    if (m_pSocket != NULL) 
    {
        SAFE_DELETE(m_pSocket);
    }	
}


//////////////////////////////////////////////////////////////////////
//
// func description	:	Create
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ServerSocket::Create()
{
    CHECKF(m_pSocket);
    return m_pSocket->create(g_config.ONLINE_LISTEN_IPV_MODULE);
}


//////////////////////////////////////////////////////////////////////
//
// func description	:	Bind
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		port    : 端口
BOOL ServerSocket::Bind(INT port, UINT nAddr/* = 0*/)
{
    CHECKF(m_pSocket);
    return m_pSocket->bind(port, nAddr);
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	Listen
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		backlog    : 监听队列大小（半连接队列、全连接队列）
BOOL ServerSocket::Listen(UINT backlog /*= MAX_BACKLOG*/)
{
    CHECKF(m_pSocket);
    return m_pSocket->listen(backlog);
}

//add by huyf 2018.01.24:支持I/O多路复用epoll
//////////////////////////////////////////////////////////////////////
//
// func description	:	该函数生成一个epoll专用的文件描述符
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		maxEvents    : 其中的参数是指定生成描述符的最大范围。
#ifdef _EPOLL_  
BOOL ServerSocket::EpollCreate(INT maxEvents)
{
    CHECKF(m_pSocket);
    return m_pSocket->epollCreate(maxEvents);
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
BOOL ServerSocket::EpollCtl(int epfd, int op, int fd, struct epoll_event *event)
{
    CHECKF(m_pSocket);
    return m_pSocket->epollCtl(epfd, op, fd, event);
}
#endif
//add end:支持I/O多路复用epoll

//////////////////////////////////////////////////////////////////////
//
// func description	:	Close
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
VOID ServerSocket::Close()
{	
    if (m_pSocket != NULL) 
    {
        m_pSocket->close();
    }
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	Close
//
//-----input------
// Parameters
//		socket 			:	
//------output------
// Return 
//		INT:  0:下次重试 -1：出错 >0:正确创建socket
/*在Linux中最多可以打开1024个文件描述如，其中还得除去0，1，2。
 * 而服务器端只能accept 返回1020个已连接套接字，因为除了0,1,2之外还有一个监听套接字listenfd，
 * 客户端某一个套接字（不一定是最后一个）虽然已经建立了连接，
 * 在已完成连接队列中，但accept返回时达到最大描述符限制，返回错误，打印提示信息。
 * */
INT ServerSocket::Accept(Socket* socket)
{	
    CHECKF(socket);
    //执行一次关闭
    socket->close();

    //!alter by huyf 2018.08.01:支持双栈支持IPV4和IPV6
    UINT addrlen = 0;
    SOCKET acceptSock = INVALID_SOCKET;
    if (g_config.ONLINE_LISTEN_IPV_MODULE) //  IPV6
    {
        addrlen = sizeof(SOCKADDR_IN6);
        acceptSock = m_pSocket->accept((struct sockaddr *)(&(socket->m_SockAddr6)), &addrlen);
    }
    else //  IPV4
    {
        addrlen = sizeof(SOCKADDR_IN);
        acceptSock = m_pSocket->accept((struct sockaddr *)(&(socket->m_SockAddr)), &addrlen);
    }    
    //!alter end:支持双栈支持IPV4和IPV6

    if(acceptSock == INVALID_SOCKET)
    {
        LOGERROR("接受新的连接失败...Accept is error acceptSock=[-1]");
        return acceptSock;
    }
    if(acceptSock == 0)//EAGAIN
    {
        LOGWARNING("Accept is again acceptSock=[0] EAGAIN");
        return acceptSock;
    }
    
    //!alter by huyf 2018.02.06:放到前面去判断
    //if(acceptSock >= _SETSIZE_)  
    //{  
    //    //MySleep(4);
    //    LOGERROR("[ServerSocket::Accept] Too Many Clients AcceptSock=[%d] _SETSIZE_=[%d]\n", acceptSock, _SETSIZE_);         //说明连接的客户端已经达到上限  
    //    return INVALID_SOCKET;
    //}
    //!alter end:放到前面去判断

    socket->setSocketID(acceptSock);

    //!alter by huyf 2018.03.19:socket缓冲区监控日志
    UINT unRecvBufSize = socket->getReceiveBufferSize();
    UINT unSendBufSize = socket->getSendBufferSize();
    LOGDEBUG("[ServerSocket::Accept] acceptSock=[%d] RECEIVE_BUFFER_SIZE=[%u] SEND_BUFFER_SIZE=[%u]", acceptSock, unRecvBufSize, unSendBufSize);
    //!alter end:socket缓冲区监控日志
    
    //!alter by huyf 2018.08.01:支持双栈支持IPV4和IPV6
    if (g_config.ONLINE_LISTEN_IPV_MODULE) //  IPV6
    {
        int nV4mapped = IN6_IS_ADDR_V4MAPPED(&socket->m_SockAddr6.sin6_addr);
        if(nV4mapped == 0) // IPv6 
        {
            socket->m_Port = htons(socket->m_SockAddr6.sin6_port); 
            LOGDEBUG("[ServerSocket::Accept] IN6_IS_ADDR_V4MAPPED == [0] client = [IPV6]");  
            //inet_ntop(AF_INET6, &their_addr.sin6_addr, buf, sizeof(buf)), // IPv6          
            const char *ptr = inet_ntop(AF_INET6, &socket->m_SockAddr6.sin6_addr, socket->m_Host, IP_SIZE);
            if (ptr)
            {
                LOGDEBUG("[ServerSocket::Accept]-1 Is OK... Client_Socket=[%d]\tIP=[%s]\tPORT=[%u]\tptr=[%s]", acceptSock, socket->m_Host, socket->m_Port, ptr); 
            }
            else
            {
                LOGERROR("[ServerSocket::Accept]-1 Is Error... Client_Socket=[%d]\tIP=[%s]\tPORT=[%u]\tptr=NULL", acceptSock, socket->m_Host, socket->m_Port, ptr); 
            }   
        }
        else
        {
             
            LOGDEBUG("[ServerSocket::Accept] IN6_IS_ADDR_V4MAPPED == [%d] client = [IPV4]", nV4mapped);
            socket->m_Port = htons(socket->m_SockAddr.sin_port);//htons
            strncpy(socket->m_Host, inet_ntoa(socket->m_SockAddr.sin_addr), IP_SIZE-1);
            LOGDEBUG("[ServerSocket::Accept] Is OK... Client_Socket=[%d]\tIP=[%s]\tPORT=[%u]", acceptSock, socket->m_Host, socket->m_Port); 
        }  
    }
    else //  IPV4
    {
        socket->m_Port = htons(socket->m_SockAddr.sin_port);//htons
        strncpy(socket->m_Host, inet_ntoa(socket->m_SockAddr.sin_addr), IP_SIZE-1);
        LOGDEBUG("[ServerSocket::Accept] Is OK... Client_Socket=[%d]\tIP=[%s]\tPORT=[%u]", acceptSock, socket->m_Host, socket->m_Port);
    }
    //!alter end:支持双栈支持IPV4和IPV6  

    return acceptSock;
}

