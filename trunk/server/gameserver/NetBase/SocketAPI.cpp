////////////////////////////////////////////////////////////////
//文件名称：SocketAPI.cpp
//功能描述：原始套接字socket接口封装，对网络socket进行一个简单的封装操作，针对不同的系统实现相同的接口调用，
//          无其他原因请不要随便修改！！！！
//版本说明：SocketAPI API
//          
//编写作者：2017.08.09 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
// include files
////////////////////////////////////////////////////////////////

#include "FileAPI.h"
#include "SocketAPI.h"

#ifndef __LINUX__
	#define __LINUX__
#endif

#if defined (__WINDOWS__)
#include <ws2tcpip.h>			// for socklen_t
#elif defined(__LINUX__)
#include <sys/types.h>			// for accept(), FD_SETDIZE 256
#include <sys/socket.h>			// for socklen_t
#include <sys/time.h>
#include <arpa/inet.h>			// for inet_xxx()
#include <netinet/in.h>			// for all of INADDR_ const value
#include <errno.h>				// for errno

#include <unistd.h>				// for socklen_t
#include <sys/select.h>			// for select()
#include <netinet/tcp.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#endif
/*
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
*/

////////////////////////////////////////////////////////////////
// external variable
////////////////////////////////////////////////////////////////
#if defined(__LINUX__)
extern INT errno;
#endif

CHAR Error[ERROR_SIZE];

//////////////////////////////////////////////////////////////////////
//
// func description	:	创建套接字，指定要用的通信协议类型
//
//-----input------
// Parameters
//     	family 	: 	指明协议族。
//			  		取值范围：
//						AF_INET(internet socket IPv4协议), 
//						AF_INET6(internet socket IPv6协议), 
//						AF_LOCAL(Unix域协议),
//						AF_ROUTE(路由套接字),
//						AF_KEY(密钥套接字),
//	   	types 	: 	指明套接字类型。
//					取值范围：SOCK_STREAM(TCP), SOCK_DGRAM(UDP), ...
//		
//     	protocol ：	协议类型。一般设置为0
//					取值范围：IPPROTO_TCP,IPPROTO_UDP,...	
//------output------
// Return 
//     SOCKET 	:	成功时候返回一个小于socket descriptor(sockfd)
SOCKET SocketAPI::socket_ex(INT family, INT type, INT protocol)
{
	DEBUG_TRY	
    SOCKET s =::socket(family, type, protocol);
    if(s == INVALID_SOCKET)
    {
#if defined(__LINUX__)
        switch(errno)
        {
            case EPROTONOSUPPORT:
            case EMFILE: 
            case ENFILE: 
            case EACCES: 
            case ENOBUFS: 
            default: 
                {
                    break;
                }
        }//end of switch
#elif defined(__WINDOWS__)
        INT iErr = WSAGetLastError();
        switch (iErr) 
        {
        case WSANOTINITIALISED: 
            strncpy(Error, "WSANOTINITIALISED", ERROR_SIZE);
            break;
        case WSAENETDOWN: 
            strncpy(Error, "WSAENETDOWN", ERROR_SIZE);
            break;
        case WSAEAFNOSUPPORT: 
            strncpy(Error, "WSAEAFNOSUPPORT", ERROR_SIZE);
            break;
        case WSAEINPROGRESS: 
            strncpy(Error, "WSAEINPROGRESS", ERROR_SIZE);
            break;
        case WSAEMFILE: 
            strncpy(Error, "WSAEMFILE", ERROR_SIZE);
            break;
        case WSAENOBUFS: 
            strncpy(Error, "WSAENOBUFS", ERROR_SIZE);
            break;
        case WSAEPROTONOSUPPORT: 
            strncpy(Error, "WSAEPROTONOSUPPORT", ERROR_SIZE);
            break;
        case WSAEPROTOTYPE: 
            strncpy(Error, "WSAEPROTOTYPE", ERROR_SIZE);
            break;
        case WSAESOCKTNOSUPPORT: 
            strncpy(Error, "WSAESOCKTNOSUPPORT", ERROR_SIZE);
            break;
        default: 
            {
                strncpy(Error, "UNKNOWN", ERROR_SIZE);
                break;
            }
        }//end of switch
#endif
    }

    return s;	
	DEBUG_CATCH0("SocketAPI::socket_ex");
	

	
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	把一个本地协议赋予给一个套接字
//
//-----input------
// Parameters
//     	s 	 		: 	由socket函数返回的套接字描述符
//	   	myaddr 		:   指向特定于协议的地址结构的指针，指定端口号为0，则选择一个临时端口
//     	addrlen		：  该地址结构的长度
//------output------
// Return 
//     BOOL 	:	成功：0 失败：-1
BOOL SocketAPI::bind_ex(SOCKET s, const struct sockaddr* myaddr, UINT addrlen)
{
	DEBUG_TRY	
    if(bind(s, myaddr, addrlen) == -1)
    {
#if defined(__LINUX__)
        switch(errno)
        {
            case EADDRINUSE:
            case EINVAL: 
            case EACCES: 
            case ENOTSOCK: 
            case EBADF: 
            case EROFS: 
            case EFAULT: 
            case ENAMETOOLONG: 
            case ENOENT: 
            case ENOMEM: 
            case ENOTDIR: 
            case ELOOP: 
            default:
                {
                    break;
                }
        }
#elif defined(__WINDOWS__)
        INT iErr = WSAGetLastError()
        switch (iErr) 
        {
            case WSANOTINITIALISED: 
            {
                strncpy(Error, "WSAESOCKTNOSUPPORT", ERROR_SIZE);
                break;
            }

            case WSAENETDOWN: 
            {
                strncpy(Error, "WSAENETDOWN", ERROR_SIZE);
                break;
            }
            case WSAEADDRINUSE: 
            {
                strncpy(Error, "WSAEADDRINUSE", ERROR_SIZE);
                break;
            }
            case WSAEADDRNOTAVAIL: 
            {
                strncpy(Error, "WSAEADDRNOTAVAIL", ERROR_SIZE);
                break;
            }
            case WSAEFAULT: 
            {
                strncpy(Error, "WSAEFAULT", ERROR_SIZE);
                break;
            }
            case WSAEINPROGRESS: 
            {
                strncpy(Error, "WSAEINPROGRESS", ERROR_SIZE);
                break;
            }
            case WSAEINVAL: 
            {
                strncpy(Error, "WSAEINVAL", ERROR_SIZE);
                break;
            }
            case WSAENOBUFS: 
            {
                strncpy(Error, "WSAENOBUFS", ERROR_SIZE);
                break;
            }
            case WSAENOTSOCK: 
            {
                strncpy(Error, "WSAENOTSOCK", ERROR_SIZE);
                break;
            }
            default:
            {
                strncpy(Error, "UNKNOWN", ERROR_SIZE);
                break;
            }
        }//end of switch
#endif
        return FALSE;
    }
    return TRUE;	
	DEBUG_CATCHF("SocketAPI::bind_ex");
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	建立与TCP服务器的连接
//
//-----input------
// Parameters
//     	s 	 		: 	由socket函数返回的套接字描述符
//	   	servaddr 	:   指向套接字地址结构的指针
//     	addrlen		：  结构的长度
//------output------
// Return 
//     BOOL 		:	成功返回0，若出错则返回-1
//     不要把错误(EINTR/EINPROGRESS/EAGAIN)当成Fatal
//
BOOL SocketAPI::connect_ex(SOCKET s, const struct sockaddr* servaddr, UINT addrlen)
{
	DEBUG_TRY
    if (connect(s, servaddr, addrlen) == -1)
    {
        //LOGDEBUG("[SocketAPI::connect_ex] Error errno[%d] discription[%s]", errno, strerror(errno));
#if defined(__LINUX__)	
        switch(errno)
        {
            case EALREADY:      //#define EALREADY    114 /* Operation already in progress */
            case EINPROGRESS:   //#define EINPROGRESS 115 /* Operation now in progress */
            case EINTR:         //#define EINTR        4  /* Interrupted system call */
            case EAGAIN:        //#define EAGAIN      11  /* Try again */          
            {
                    //!alter by huyf 2018.06.05:修改非阻塞connet处理流程
                    //建立connect连接，此时socket设置为非阻塞，connect调用后，无论连接是否建立立即返回-1，同时将errno（包含errno.h就可以直接使用）设置为EINPROGRESS, 
                    //表示此时tcp三次握手仍就进行，如果errno不是EINPROGRESS，则说明连接错误，程序结束。
                    return reconnect_ex(s, servaddr, addrlen) == 0 ? TRUE : FALSE;
                    //return TRUE;
                    //!alter end:修改非阻塞connet处理流程
            }            
            //!alter by huyf 2018.06.05:修改非阻塞connet处理流程
            //增加已经连接的处理，此处可以直接返回告之
            case EISCONN:   //#define EISCONN     106 /* Transport endpoint is already connected */
            {
                return TRUE;
            }
            //!alter end:修改非阻塞connet处理流程
            case ECONNREFUSED:
            case ETIMEDOUT:
            case ENETUNREACH:
            case EADDRINUSE:
            case EBADF:
            case EFAULT:
            case ENOTSOCK:
            default:
            {
                //LOGERROR("[SocketAPI::connect_ex] Is Error errno[%d] discription[%s]", errno, strerror(errno));
                break;
            }
        }//end of switch
#elif defined(__WINDOWS__)
        INT iErr = WSAGetLastError();
        switch(iErr)
        {
        case WSANOTINITIALISED: 
            {
                strncpy(Error, "WSANOTINITIALISED", ERROR_SIZE);
                break;
            }

        case WSAENETDOWN:
            { 
                strncpy(Error, "WSAENETDOWN", ERROR_SIZE);
                break;
            }
        case WSAEADDRINUSE: 
            { 
                strncpy(Error, "WSAEADDRINUSE", ERROR_SIZE);
                break;
            }
        case WSAEINTR: 
            { 
                strncpy(Error, "WSAEINTR", ERROR_SIZE);
                break;
            }
        case WSAEINPROGRESS: 
            { 
                strncpy(Error, "WSAEINPROGRESS", ERROR_SIZE);
                break;
            }
        case WSAEALREADY: 
            { 
                strncpy(Error, "WSAEALREADY", ERROR_SIZE);
                break;
            }
        case WSAEADDRNOTAVAIL: 
            { 
                strncpy(Error, "WSAEADDRNOTAVAIL", ERROR_SIZE);
                break;
            }
        case WSAEAFNOSUPPORT: 
            { 
                strncpy(Error, "WSAEAFNOSUPPORT", ERROR_SIZE);
                break;
            }
        case WSAECONNREFUSED: 
            { 
                strncpy(Error, "WSAECONNREFUSED", ERROR_SIZE);
                break;
            }
        case WSAEFAULT: 
            { 
                strncpy(Error, "WSAEFAULT", ERROR_SIZE);
                break;
            }
        case WSAEINVAL: 
            { 
                strncpy(Error, "WSAEINVAL", ERROR_SIZE);
                break;
            }
        case WSAEISCONN: 
            { 
                strncpy(Error, "WSAEISCONN", ERROR_SIZE);
                break;
            }
        case WSAENETUNREACH: 
            { 
                strncpy(Error, "WSAENETUNREACH", ERROR_SIZE);
                break;
            }
        case WSAENOBUFS: 
            { 
                strncpy(Error, "WSAENOBUFS", ERROR_SIZE);
                break;
            }
        case WSAENOTSOCK: 
            { 
                strncpy(Error, "WSAENOTSOCK", ERROR_SIZE);
                break;
            }
        case WSAETIMEDOUT: 
            { 
                strncpy(Error, "WSAETIMEDOUT", ERROR_SIZE);
                break;
            }
        case WSAEWOULDBLOCK: 
            { 
                strncpy(Error, "WSAEWOULDBLOCK", ERROR_SIZE);
                break;
            }
        default:
            {
                strncpy(Error, "UNKNOWN", ERROR_SIZE);
                break;
            }
        }//end of switch		
#endif
        return FALSE;
    }
    return TRUE;	
	DEBUG_CATCHF("SocketAPI::connect_ex");	
}

//////////////////////////////////////////////////////////////////////
//
// func description :   非阻塞套接字建立连接时未立即完成的检查(tcp三次握手阶段)
//
//-----input------
// Parameters
//      s         :   由socket函数返回的套接字描述符
//------output------
// Return 
//     BOOL         :   成功返回0，若出错则返回-1
//     不要把错误(EINTR/EINPROGRESS/EAGAIN)当成Fatal
//
int SocketAPI::reconnect_ex(SOCKET s, const struct sockaddr* servaddr, UINT addrlen)
{   
    //LOGDEBUG("[SocketAPI::reconnect_ex] Get The Connect Result By Select() Errno=[%d] Discription=[%s]", errno, strerror(errno));    
    //if (errno == EINPROGRESS)    
    //{    
        //int nTimes = 0; 
        int nRet = -1;   
        //while (nTimes++ < 5)    
        {    
            fd_set rfds, wfds;    
            struct timeval tv;
            FD_ZERO(&rfds);    
            FD_ZERO(&wfds);    
            FD_SET(s, &rfds);    
            FD_SET(s, &wfds);    
                
            /* set select() time out */    
            tv.tv_sec = 1;     
            tv.tv_usec = 0;
            /*
            2.源自Berkeley的实现(和Posix.1g)有两条与select和非阻塞IO相关的规则:
            A:当连接建立成功时,套接口描述符变成可写;
            B:当连接出错时,套接口描述符变成既可读又可写;
            注意:当一个套接口出错时,它会被select调用标记为既可读又可写;
            一种更有效的判断方法，经测试验证，在Linux环境下是有效的：
                再次调用connect，相应返回失败，如果错误errno是EISCONN，表示socket连接已经建立，否则认为连接失败。
            */    
            int nSelRet = select(s+1, &rfds, &wfds, NULL, &tv);    
            switch (nSelRet)    
            {    
                case -1:    //出错
                {
                    //LOGERROR("[SocketAPI::reconnect_ex] Select Is Error... nSelRet=[%d] Errno=[%d] Discription=[%s]", nSelRet, errno, strerror(errno)); 
                    nRet = -1; 
                }   
                break;    
                case 0:    //超时
                {
                    //LOGWARNING("[SocketAPI::reconnect_ex] Select Is Time Out... nSelRet=[%d] Errno=[%d] Discription=[%s]", nSelRet, errno, strerror(errno));     
                    nRet = -1; 
                }   
                break;    
                default:    //有数据过来
                {
                    //LOGDEBUG("[SocketAPI::reconnect_ex] nSelRet=[%d] Errno=[%d] Discription=[%s]", nSelRet, errno, strerror(errno));   
                    if (FD_ISSET(s, &rfds) || FD_ISSET(s, &wfds))    //判断可读或者可写
                    {    
                        #if 0 // not useable in linux environment, suggested in <<Unix network programming>>  SO_ERROR no used 
                            int errinfo, errlen;    
                            if (-1 == getsockopt(s, SOL_SOCKET, SO_ERROR, &errinfo, &errlen))    
                            {    
                                nRet = -1;  
                                LOGERROR("getsockopt return -1.\n");  
                                break;    
                            }    
                            else if (0 != errinfo)    
                            {      
                                nRet = -1;   
                                LOGERROR("getsockopt return errinfo = %d.\n", errinfo); 
                                break;    
                            }                                       
                            nRet = 0;  
                            LOGDEBUG("connect ok?\n");     
                        #else    
                            #if 1    
                                connect(s, servaddr, addrlen);      //再次连接来判断套接字状态    
                                if (errno == EISCONN)    
                                {    
                                    //LOGDEBUG("[SocketAPI::reconnect_ex] Reconnect Finished...nSelRet=[%d]", nSelRet);    
                                    nRet = 0;    
                                }    
                                else    
                                {      
                                    //LOGWARNING("[SocketAPI::reconnect_ex] Reconnect Failed...FD_ISSET(s, &rfds)=[%d] FD_ISSET(s, &wfds)=[%d] nSelRet=[%d] Errno=[%d] Discription=[%s]", FD_ISSET(s, &rfds) , FD_ISSET(s, &wfds), nSelRet, errno, strerror(errno));     
                                    nRet = -1;    
                                }    
                            #else    //test
                                char buff[2];    
                                if (read(s, buff, 0) < 0)    
                                {    
                                    LOGERROR("connect failed. errno = %d\n", errno);    
                                    nRet = errno;    
                                }    
                                else    
                                {    
                                    LOGDEBUG("connect finished.\n");    
                                    nRet = 0;    
                                }    
                            #endif    
                        #endif    
                    } 
                }
                break;   
            } 
        }   
    //}  
    return nRet;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	把套接字（s）从CLOSE状态装换到LISTEN状态，设置套接字为被动套接字,指示内核接收向该套接字的连接请求
//
//-----input------
// Parameters
//     	s 	 		: 	唤醒的套接字
//	   	backlog		:	规定了内核应该为相应套接字排队的最大连接个数
//------output------
// Return 
//     BOOL 		:	成功：0 失败：-1
//
BOOL SocketAPI::listen_ex(SOCKET s, INT backlog/*= MAX_BACKLOG*/)
{
	DEBUG_TRY	
    if (listen(s, backlog) == -1)
    {
#if defined(__LINUX__)
        switch(errno)
        {
            case EBADF: 
            case ENOTSOCK:
            case EOPNOTSUPP:
            default:
                {
                    break;
                }
        }
#elif defined(__WINDOWS__)
        INT iErr = WSAGetLastError()
        switch(iErr)
        {
            case WSANOTINITIALISED: 
            {
                strncpy(Error, "WSANOTINITIALISED", ERROR_SIZE);
                break;
            }
            case WSAENETDOWN: 
            {
                strncpy(Error, "WSAENETDOWN", ERROR_SIZE);
                break;
            }
            case WSAEADDRINUSE: 
            {
                strncpy(Error, "WSAEADDRINUSE", ERROR_SIZE);
                break;
            }
            case WSAEINPROGRESS: 
            {
                strncpy(Error, "WSAEINPROGRESS", ERROR_SIZE);
                break;
            }
            case WSAEINVAL: 
            {
                strncpy(Error, "WSAEINVAL", ERROR_SIZE);
                break;
            }
            case WSAEISCONN: 
            {
                strncpy(Error, "WSAEISCONN", ERROR_SIZE);
                break;
            }
            case WSAEMFILE: 
            {
                strncpy(Error, "WSAEMFILE", ERROR_SIZE);
                break;
            }
            case WSAENOBUFS: 
            {
                strncpy(Error, "WSAENOBUFS", ERROR_SIZE);
                break;
            }
            case WSAENOTSOCK: 
            {
                strncpy(Error, "WSAENOTSOCK", ERROR_SIZE);
                break;
            }
            case WSAEOPNOTSUPP: 
            {
                strncpy(Error, "WSAEOPNOTSUPP", ERROR_SIZE);
                break;
            }
            default:
            {
                strncpy(Error, "UNKNOWN", ERROR_SIZE);
                break;
            }
        }
#endif
    return FALSE;
    }

    return TRUE;	
	DEBUG_CATCHF("SocketAPI::listen_ex");
}

/////////////////////////////////////////////////////////////////////
//
// func description	:	把套接字（s）从CLOSE状态装换到LISTEN状态，允许通过环境变量指定backlog值
//
//-----input------
// Parameters
//     	s 	 		: 	唤醒的套接字
//	   	backlog		:	规定了内核应该为相应套接字排队的最大连接个数,允许环境变量LISTEN覆写由调用者指定的值
//------output------
// Return 
//     BOOL 		:	成功：0 失败：-1
//
BOOL SocketAPI::listen_ex2(SOCKET s, INT backlog)
{
	DEBUG_TRY	
    CHAR *ptr;
    if ((ptr = getenv("LISTENQ")) != NULL)
            backlog = atoi(ptr);

    if (listen(s, backlog) == -1)
    {
#if defined(__LINUX__)
        switch(errno)
        {
            case EBADF: 
            case ENOTSOCK:
            case EOPNOTSUPP:
            default:
                {
                    break;
                }
        }
#elif defined(__WINDOWS__)
        INT iErr = WSAGetLastError()
        switch(iErr)
        {
            case WSANOTINITIALISED: 
            {
                strncpy(Error, "WSANOTINITIALISED", ERROR_SIZE);
                break;
            }
            case WSAENETDOWN: 
            {
                strncpy(Error, "WSAENETDOWN", ERROR_SIZE);
                break;
            }
            case WSAEADDRINUSE: 
            {
                strncpy(Error, "WSAEADDRINUSE", ERROR_SIZE);
                break;
            }
            case WSAEINPROGRESS: 
            {
                strncpy(Error, "WSAEINPROGRESS", ERROR_SIZE);
                break;
            }
            case WSAEINVAL: 
            {
                strncpy(Error, "WSAEINVAL", ERROR_SIZE);
                break;
            }
            case WSAEISCONN: 
            {
                strncpy(Error, "WSAEISCONN", ERROR_SIZE);
                break;
            }
            case WSAEMFILE: 
            {
                strncpy(Error, "WSAEMFILE", ERROR_SIZE);
                break;
            }
            case WSAENOBUFS: 
            {
                strncpy(Error, "WSAENOBUFS", ERROR_SIZE);
                break;
            }
            case WSAENOTSOCK: 
            {
                strncpy(Error, "WSAENOTSOCK", ERROR_SIZE);
                break;
            }
            case WSAEOPNOTSUPP: 
            {
                strncpy(Error, "WSAEOPNOTSUPP", ERROR_SIZE);
                break;
            }
            default:
            {
                strncpy(Error, "UNKNOWN", ERROR_SIZE);
                break;
            }
        }
#endif

    return FALSE;
    }

    return TRUE;	
	DEBUG_CATCHF("SocketAPI::listen_ex2");
}

/////////////////////////////////////////////////////////////////////
//
// func description	:	由TCP服务器调用，用于从已完成连接队列头返回下一个已完成连接，如果已经完成连接队列为空，那么进程被投入睡眠（阻塞模式）
//
//-----input------
// Parameters
//     	s 	 		: 	唤醒的套接字
//	   	cliaddr		:	用来返回已连接的对端进程（客户）的协议地址，调用前，我们将由*addrlen所引用的整数值置为由cliaddr所置的套接字地址结构的长度
//		addrlen		:	返回时，该整数值纪委由内核存放在该套接字地址结构内的确切字节数
//------output------
// Return 
//     INT 		:	成功：非负描述符 失败：-1
//  不要把错误(EINTR/ECONNABORTED/EPROTO)当成Fatal
//
SOCKET SocketAPI::accept_ex(SOCKET s, struct sockaddr *cliaddr, UINT* addrlen)
{
	
	UINT len = sizeof(struct sockaddr);
	SOCKET client = accept(s, cliaddr, &len);
	if (client == INVALID_SOCKET)
	{
#if defined(__LINUX__)
		switch (errno) 
		{
            case EINTR://被信号中断，可以继续去循环
            case EAGAIN://表示暂时发送失败，需要重试
                client = 0;
                break;                            
			//case EWOULDBLOCK: 
			case ECONNRESET:
			case ECONNABORTED:
			case EPROTO:
			case EBADF: 
			case ENOTSOCK: 
			case EOPNOTSUPP: 
			case EFAULT: 
			default:
				{
					break;
				}
		}//end of switch
#elif defined(__WINDOWS__)
		INT iErr = WSAGetLastError();
		switch(iErr)
		{
			case WSANOTINITIALISED: 
			{
				strncpy(Error, "WSANOTINITIALISED", ERROR_SIZE);
				break;
			}
			case WSAENETDOWN: 
			{
				strncpy(Error, "WSAENETDOWN", ERROR_SIZE);
				break;
			}
			case WSAEFAULT: 
			{
				strncpy(Error, "WSAEFAULT", ERROR_SIZE);
				break;
			}
			case WSAEINTR: 
			{
				strncpy(Error, "WSAEINTR", ERROR_SIZE);
				break;
			}
			case WSAEINPROGRESS: 
			{
				strncpy(Error, "WSAEINPROGRESS", ERROR_SIZE);
				break;
			}
			case WSAEINVAL: 
			{
				strncpy(Error, "WSAEINVAL", ERROR_SIZE);
				break;
			}
			case WSAEMFILE: 
			{
				strncpy(Error, "WSAEMFILE", ERROR_SIZE);
				break;
			}
			case WSAENOBUFS: 
			{
				strncpy(Error, "WSAENOBUFS", ERROR_SIZE);
				break;
			}
			case WSAENOTSOCK: 
			{
				strncpy(Error, "WSAENOTSOCK", ERROR_SIZE);
				break;
			}
			case WSAEOPNOTSUPP: 
			{
				strncpy(Error, "WSAEOPNOTSUPP", ERROR_SIZE);
				break;
			}
			case WSAEWOULDBLOCK: 
			{
				strncpy(Error, "WSAEWOULDBLOCK", ERROR_SIZE);
				break;
			}
			default:
			{
				strncpy(Error, "UNKNOWN", ERROR_SIZE);
				break;
			}
		}
#endif		
	}
	//LOGDEBUG("[SocketAPI::accept_ex] client[%u]", client);
	return client;
	
}


//////////////////////////////////////////////////////////////////////
// func description	:	指示内核等待多个事件中的任何一个发生后唤醒它，（使用非阻塞select模型）
//
//-----input------
// Parameters
//     	maxfdp1		:	指定待测试的描述符个数，值为待测试的最大描述符+1（0.1.2...~maxfdp1-1均将被测试）
//      readset 	:	让内核测试读的描述符
//      writeset 	:	让内核测试写的描述符
//      exceptset 	:	让内核测试异常条件的描述符
//		timeout		:	它告知内核等待所指定描述符中的任何一个就绪可花多长时间。秒，微秒
//						NULL:永久等下去，直到有描述符准备好IO时，才返回。
//						指定好时间：等待一段固定时间，不超过设置的timeval结构体中的设置的秒和微妙
//						0：不等待直接返回（称为轮询（polling））指向timeval结构，但是秒和毫秒都是0
//------output------
// Return 
//     返回值：若有继续描述符则为其数目，若超时则为0，若出错则为-1
//
INT SocketAPI::select_ex(INT maxfdp1, fd_set* readset, fd_set* writeset, fd_set* exceptset, struct timeval* timeout)
{
	INT nReady = 0;
	nReady = select(maxfdp1, readset, writeset, exceptset, timeout);
	if (nReady == -1)
	{
#if defined(__LINUX__)
        switch (errno) 
        {
            case EINTR://被信号中断，可以继续去循环
            case EAGAIN:
                nReady = 0;
                break;
            default:
                break;

        }//end of switch
#elif  defined(__WINDOWS__) 
		INT iErr = WSAGetLastError()
		switch(iErr)
		{
			case WSANOTINITIALISED: 
			{
				strncpy(Error, "WSANOTINITIALISED", _ESIZE);
				break;
			}
			case WSAEFAULT:
			{
				strncpy(Error, "WSAEFAULT", _ESIZE);
				break;
			}
			case WSAENETDOWN:
			{
				strncpy(Error, "WSAENETDOWN", _ESIZE);
				break;
			}
			case WSAEINVAL:
			{
				strncpy(Error, "WSAEINVAL", _ESIZE);
				break;
			}
			case WSAEINTR:
			{
				strncpy(Error, "WSAEINTR", _ESIZE);
				break;
			}
			case WSAEINPROGRESS:
			{
				strncpy(Error, "WSAEINPROGRESS", _ESIZE);
				break;
			}
			case WSAENOTSOCK:
			{
				strncpy(Error, "WSAENOTSOCK", _ESIZE);
				break;
			}
			default:
			{
				strncpy(Error, "UNKNOWN", _ESIZE);
				break;
			}
		}
#endif
	}

	return nReady;
}

//////////////////////////////////////////////////////////////////////
// func description	:	指示内核等待多个事件中的任何一个发生后唤醒它，（使用非阻塞poll_ex模型）
//
//-----input------
// Parameters
//     	maxfdp1		:	指定待测试的描述符个数，值为待测试的最大描述符+1（0.1.2...~maxfdp1-1均将被测试）
//      readset 	:	让内核测试读的描述符
//      writeset 	:	让内核测试写的描述符
//      exceptset 	:	让内核测试异常条件的描述符
//		timeout		:	timeout : 超时时间 ，等于0表示非阻塞式等待，小于0表示阻塞式等待，大于0表示等待的时间。 
//                      它告知内核等待所指定描述符中的任何一个就绪可花多长时间。秒，微秒
//						NULL:永久等下去，直到有描述符准备好IO时，才返回。
//						指定好时间：等待一段固定时间，不超过设置的timeval结构体中的设置的秒和微妙
//						0：不等待直接返回（称为轮询（polling））指向timeval结构，但是秒和毫秒都是0
//------output------
// Return 
//     返回值：若有继续描述符则为其数目，若超时则为0，若出错则为-1
//      EBADF　　       一个或多个结构体中指定的文件描述符无效。
//      EFAULTfds　　 指针指向的地址超出进程的地址空间。
//      EINTR　　　　  请求的事件之前产生一个信号，调用可以重新发起。
//      EINVALnfds　　参数超出PLIMIT_NOFILE值。
//      ENOMEM　　     可用内存不足，无法完成请求。
//
INT SocketAPI::poll_ex(struct pollfd *fds, nfds_t nfds, int timeout)
{
	INT nReady = 0;
#if defined(__LINUX__)
            nReady = poll(fds, nfds, 0);   //0:no block
	if (nReady == -1)   // 出错
    {   
        switch (errno) 
        {
            case EINTR://被信号中断，可以继续去循环
            case EAGAIN://add by huyf 2018.01.31:添加
                nReady = 0;
                break;
            default:
                break;
        }                   
    }             
#elif  defined(__WINDOWS__) 
#endif
    return nReady;
}

//add by huyf 2018.01.30:支持I/O多路复用epoll
/////////////////////////////////////////////////////////////////////
// func description	:	函数说明：该函数用于轮询I/O事件的发生。（使用非阻塞epoll_wait模型）
//
//-----input------
// Parameters
//          epfd：由epoll_create 生成的epoll专用的文件描述符； 
//          epoll_event：用于回传代处理事件的数组； 
//          maxevents：每次能处理的事件数； 
//          timeout：等待I/O事件发生的超时值,单位 ms； 
//------output------
// Return 
//     返回值：返回发生事件数。
//
INT SocketAPI::epoll_wait_ex(int epfd, struct epoll_event * events, int maxevents, int timeout)
{
    INT nReady = 0;
#if defined(__LINUX__)
    //（注：当对一个非阻塞流的读写发生缓冲区满或缓冲区空，write/read会返回-1，并设置errno=EAGAIN。而epoll只关心缓冲区非满和缓冲区非空事件）。  
    nReady = epoll_wait(epfd, events, maxevents, timeout);   //0:no block
    if (nReady == -1)   // 出错
    {   
        switch (errno) 
        {
            case EINTR://被信号中断，可以继续去循环
            case EAGAIN:
                nReady = 0;
                break;
            default:
                break;
        }                   
    }               
#elif  defined(__WINDOWS__) 
#endif
	return nReady;
}
//add end:支持I/O多路复用epoll
        
        
//////////////////////////////////////////////////////////////////////
//
// func description	:	UNIX中派生新进程的唯一方法，不同的系统可能存在不同的变种，使用的时候请注意！！！咱不提供实现
//
//-----input------
// Parameters
//     	无
//------output------
// Return 
//     在子进程中为0，在父进程中为子进程ID，出错：-1
//
pid_t SocketAPI::fork_ex(void)
{
	return 0;

}

//////////////////////////////////////////////////////////////////////
//
// func description	:	UNIX关闭套接字，并终止TCP连接
//
//-----input------
// Parameters
//     	s 			:	需要关闭的socket
//------output------
// Return 
//     成功：0 失败：-1
//     注意：阻赛和非阻赛 关闭方式有区别，在非阻赛模式下，再调用和返回时间段内，返回WSAEWOULDBLOCK则尝试下次继续关闭
//
BOOL SocketAPI::closesocket_ex(SOCKET s)
{
#if defined(__LINUX__)
	if(close(s) == -1)
	{
		return FALSE;
    }
#elif defined(__WINDOWS__)
	if (closesocket(s) == SOCKET_ERROR) 
	{
		INT iErr = WSAGetLastError();
		switch(iErr) 
		{
			case WSANOTINITIALISED: 
			{
				strncpy(Error, "WSANOTINITIALISED", ERROR_SIZE);
				break;
			}
			case WSAENETDOWN: 
			{
				strncpy(Error, "WSAENETDOWN", ERROR_SIZE);
				break;
			}
			case WSAENOTSOCK: 
			{
				strncpy(Error, "WSAENOTSOCK", ERROR_SIZE);
				break;
			}
			case WSAEINPROGRESS: 
			{
				strncpy(Error, "WSAEINPROGRESS", ERROR_SIZE);
				break;
			}
			case WSAEINTR: 
			{
				strncpy(Error, "WSAEINTR", ERROR_SIZE);
				break;
			}
			case WSAEWOULDBLOCK: 
			{
				strncpy(Error, "WSAEWOULDBLOCK", ERROR_SIZE);
				break;
			}
			default: 
			{
			strncpy(Error, "UNKNOWN", ERROR_SIZE);
			break;
			}
		}//end of switch

		return FALSE;
	}
#endif
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	设置套接字属性
//
//-----input------
// Parameters
//     	s 			:	指向一个打开的套接字描述符(socket descriptor)
//		level		:	指定系统中解释选项的代码或者为通用套接字代码，或者为某个特定协议代码（IPv4、IPv6、TCP、SCTP）(socket option level (SOL_SOCKET, ...))
//		optval		:	一个执行某个变量的指针。*optval的大小由最后一个参数指定(pointer to contain option value)
//		optlen		:	*optval的大小(length of optval)
//------output------
// Return 
//     成功：0 失败：-1
//
BOOL SocketAPI::getsocketopt_ex(SOCKET s, INT level, INT optname, VOID* optval, INT* optlen)
{
#if defined(__LINUX__)
	if(getsockopt(s, level, optname, optval, (socklen_t *)optlen) == -1)
	{
		switch(errno)
		{
			case EBADF: 
			case ENOTSOCK: 
			case ENOPROTOOPT: 
			case EFAULT: 
            case EINVAL:
			default:
				{
					break;
				}
		}//end of switch

		return FALSE;
	}
#elif defined(__WINDOWS__)
	if (getsockopt(s, level, optname, (CHAR*)optval, optlen) == SOCKET_ERROR)
	{
		INT iErr = WSAGetLastError();
		switch (iErr) 
		{
			case WSANOTINITIALISED: 
			{
				strncpy(Error, "WSANOTINITIALISED", ERROR_SIZE);
				break;
			}
			case WSAENETDOWN: 
			{
				strncpy(Error, "WSAENETDOWN", ERROR_SIZE);
				break;
			}
			case WSAEFAULT: 
			{
				strncpy(Error, "WSAEFAULT", ERROR_SIZE);
				break;
			}
			case WSAEINPROGRESS: 
			{
				strncpy(Error, "WSAEINPROGRESS", ERROR_SIZE);
				break;
			}
			case WSAEINVAL: 
			{
				strncpy(Error, "WSAEINVAL", ERROR_SIZE);
				break;
			}
			case WSAENOPROTOOPT: 
			{
				strncpy(Error, "WSAENOPROTOOPT", ERROR_SIZE);
				break;
			}
			case WSAENOTSOCK: 
			{
				strncpy(Error, "WSAENOTSOCK", ERROR_SIZE);
				break;
			}
			default: 
			{
				strncpy(Error, "UNKNOWN", ERROR_SIZE);
				break;
			}
		}//end of switch

		return FALSE;
	}

#endif

	return TRUE;
}
	

//////////////////////////////////////////////////////////////////////
//
// func description	:	设置套接字属性
//
//-----input------
// Parameters
//     	s 			:	指向一个打开的套接字描述符(socket descriptor)
//		level		:	指定系统中解释选项的代码或者为通用套接字代码，或者为某个特定协议代码（IPv4、IPv6、TCP、SCTP）(socket option level (SOL_SOCKET, ...))
//		optval		:	一个执行某个变量的指针。*optval的大小由最后一个参数指定(pointer to contain option value)
//		optlen		:	*optval的大小(length of optval)
//------output------
// Return 
//     成功：0 失败：-1
//
/*UINT SocketAPI::getsockopt_ex2(SOCKET s, INT level, INT optname, VOID * optval, INT* optlen)
{
	MY_TRY
	{
#if defined(__LINUX__)
		if (getsockopt(s, level, optname, optval, optlen) == SOCKET_ERROR) 
		{
			switch (errno) 
			{
			case EBADF: 
				return 1;
			case ENOTSOCK: 
				return 2;
			case ENOPROTOOPT: 
				return 3;
			case EFAULT: 
				return 4;
			default:
				return 5;
			}//end of switch
		}
		return 0;

#elif defined(__WINDOWS__)
		if (getsockopt(s, level, optname, (CHAR*)optval, (int*)optlen) == SOCKET_ERROR) 
		{
			INT iErr = WSAGetLastError();
			switch (iErr) 
			{
				case WSANOTINITIALISED:
				{
					strncpy(Error, "WSANOTINITIALISED", ERROR_SIZE);
					break;
				}
				case WSAENETDOWN:
				{
					strncpy(Error, "WSAENETDOWN", ERROR_SIZE);
					break;
				}
				case WSAEFAULT:
				{
					strncpy(Error, "WSAEFAULT", ERROR_SIZE);
					break;
				}
				case WSAEINPROGRESS:
				{
					strncpy(Error, "WSAEINPROGRESS", ERROR_SIZE);
					break;
				}
				case WSAEINVAL:
				{
					strncpy(Error, "WSAEINVAL", ERROR_SIZE);
					break;
				}
				case WSAENOPROTOOPT:
				{
					strncpy(Error, "WSAENOPROTOOPT", ERROR_SIZE);
					break;
				}
				case WSAENOTSOCK:
				{
					strncpy(Error, "WSAENOTSOCK", ERROR_SIZE);
					break;
				}
				default: 
				{
					strncpy(Error, "UNKNOWN", ERROR_SIZE);
					break;
				}
			}
		}
#endif

		return 0;
	}
	MY_CATCH
	{

	}
}
*/
//////////////////////////////////////////////////////////////////////
//
// func description	:	设置套接字属性
//
//-----input------
// Parameters
//     	s 			:	指向一个打开的套接字描述符(socket descriptor)
//		level		:	指定系统中解释选项的代码或者为通用套接字代码，或者为某个特定协议代码(socket option level (SOL_SOCKET, ...))
//		optval		:	一个执行某个变量的指针。*optval的大小由最后一个参数指定(pointer to contain option value)
//		optlen		:	*optval的大小(length of optval)
//------output------
// Return 
//     成功：0 失败：-1
//
BOOL SocketAPI::setsocketopt_ex(SOCKET s, INT level, INT optname, const VOID* optval, UINT optlen)
{
#if defined(__LINUX__)
		if(setsockopt(s, level, optname, optval, optlen) == -1)
		{
			switch (errno) 
			{
				case EBADF: 
				case ENOTSOCK: 
				case ENOPROTOOPT: 
				case EFAULT: 
				default:
					{
						break;
					}
			}//end of switch

			return FALSE;
		}
#elif defined(__WINDOWS__)
		if (setsockopt(s, level, optname, (CHAR*)optval, optlen) == SOCKET_ERROR)
		{
			INT iErr = WSAGetLastError();
			switch (iErr) 
			{
				case WSANOTINITIALISED: 
				{
					strncpy(Error, "WSANOTINITIALISED", ERROR_SIZE);
					break;
				}
				case WSAENETDOWN: 
				{
					strncpy(Error, "WSAENETDOWN", ERROR_SIZE);
					break;
				}
				case WSAEFAULT: 
				{
					strncpy(Error, "WSAEFAULT", ERROR_SIZE);
					break;
				}
				case WSAEINPROGRESS: 
				{
					strncpy(Error, "WSAEINPROGRESS", ERROR_SIZE);
					break;
				}
				case WSAEINVAL: 
				{
					strncpy(Error, "WSAEINVAL", ERROR_SIZE);
					break;
				}
				case WSAENETRESET: 
				{
					strncpy(Error, "WSAENETRESET", ERROR_SIZE);
					break;
				}
				case WSAENOPROTOOPT: 
				{
					strncpy(Error, "WSAENOPROTOOPT", ERROR_SIZE);
					break;
				}
				case WSAENOTCONN: 
				{
					strncpy(Error, "WSAENOTCONN", ERROR_SIZE);
					break;
				}
				case WSAENOTSOCK: 
				{
					strncpy(Error, "WSAENOTSOCK", ERROR_SIZE);
					break;
				}
				default:
				{
					strncpy(Error, "UNKNOWN", ERROR_SIZE);
					break;
				}
			}//end of switch

			return FALSE;
		}

#endif
		return TRUE;
}

//////////////////////////////////////////////////////////////////////
//--------------------------------TCP---------------------------------
// func description	:	向socket的缓存中发送数据
//
//-----input------
// Parameters
//     	s 			:	指向一个打开的套接字描述符(socket descriptor)，
//						是你希望给发送数据的套接口文件描述符。它可以是你通过socket()系统调用返回的，也可以是通过accept()系统调用得到的。
//		buff		:	是指向你希望发送的数据的指针，存放要发送数据的缓冲区（send buffer）
//		nbytes		:	是数据的字节长度（send data length）
//		flags		:	send flag (MSG_OOB、MSG_DONTROUTE、MSG_DONTWAIT、MSG_WAITALL)
//------output------
// Return 
//     返回值：成功返回写入的字节数，出错返回-1并设置errno，socket的缓存区已满，则返回-1,同时设置errno为EAGAIN.
//             (send/sendto)不要把错误(EINTR/EAGAIN/EWOULDBLOCK)当成Fatal.
//              EINTR 4 - ("Interrupted system call")"The receive was interrupted by delivery of a signal：发送/接收处理过程被中断打断//    
//              EAGAIN 11-("Try again")
//              EWOULDBLOCK 11-("Resource temporarily unavailable")
//              "The socket is marked nonblocking and the receive operation would block, or a receive timeout had been set and the timeout expired before data was received. 
//              POSIX.1-2001 allows either error to be returned for this case,and does not require these constants to have the same value, so a portable application should check for both possibilities."
INT SocketAPI::send_ex(SOCKET s, const VOID* buff, UINT nbytes, INT flags)
{
	INT nSendBytes = -1;
		
#if defined(__LINUX__)
		nSendBytes = send(s, buff, nbytes, flags);
#elif defined(__WINDOWS__)
		nSendBytes = send(s, (const CHAR*)buff, nbytes, flags);
#endif
        //alter by huyf 2018.03.15:修改
        //return nSendBytes;
        //alter end:修改

		if (nSendBytes == -1)
		{
#if defined(__LINUX__)
			switch(errno)
			{
				//case EWOULDBLOCK://表示暂时发送失败，需要重试
				case EAGAIN://表示暂时发送失败，需要重试
				case EINTR: //表示暂时发送失败，需要重试
					return nSendBytes;//SOCKET_ERROR_WOULDBLOCK;

				//VVVVVVVVVVV连接异常，需要关闭VVVVVVVVVVVV
				case ECONNRESET:
				case EPIPE:
				case EBADF: 
				case ENOTSOCK: 
				case EFAULT: 
				case EMSGSIZE: 
				case ENOBUFS: 
					return nSendBytes;

				default: 
					{
						break;
					}
			}//end of switch		
#elif defined(__WINDOWS__)
			INT iErr = WSAGetLastError();
			switch(iErr)
			{
				case WSANOTINITIALISED: 
				{
					strncpy(Error, "WSANOTINITIALISED", ERROR_SIZE);
					break;
				}
				case WSAENETDOWN: 
				{
					strncpy(Error, "WSAENETDOWN", ERROR_SIZE);
					break;
				}
				case WSAEACCES: 
				{
					strncpy(Error, "WSAEACCES", ERROR_SIZE);
					break;
				}
				case WSAEINTR: 
				{
					strncpy(Error, "WSAEINTR", ERROR_SIZE);
					break;
				}
				case WSAEINPROGRESS: 
				{
					strncpy(Error, "WSAEINPROGRESS", ERROR_SIZE);
					break;
				}
				case WSAEFAULT: 
				{
					strncpy(Error, "WSAEFAULT", ERROR_SIZE);
					break;
				}
				case WSAENETRESET: 
				{
					strncpy(Error, "WSAENETRESET", ERROR_SIZE);
					break;
				}
				case WSAENOBUFS: 
				{
					strncpy(Error, "WSAENOBUFS", ERROR_SIZE);
					break;
				}
				case WSAENOTCONN: 
				{
					strncpy(Error, "WSAENOTCONN", ERROR_SIZE);
					break;
				}
				case WSAENOTSOCK: 
				{
					strncpy(Error, "WSAENOTSOCK", ERROR_SIZE);
					break;
				}
				case WSAEOPNOTSUPP: 
				{
					strncpy(Error, "WSAEOPNOTSUPP", ERROR_SIZE);
					break;
				}
				case WSAESHUTDOWN: 
				{
					strncpy(Error, "WSAESHUTDOWN", ERROR_SIZE);
					break;
				}
				case WSAEWOULDBLOCK: 
					//strncpy(Error, "WSAEWOULDBLOCK", ERROR_SIZE);				
					//break;
					return SOCKET_ERROR_WOULDBLOCK;
				case WSAEMSGSIZE: 
				{
					strncpy(Error, "WSAEMSGSIZE", ERROR_SIZE);
					break;
				}
				case WSAEHOSTUNREACH: 
				{
					strncpy(Error, "WSAEHOSTUNREACH", ERROR_SIZE);
					break;
				}
				case WSAEINVAL: 
				{
					strncpy(Error, "WSAEINVAL", ERROR_SIZE);
					break;
				}
				case WSAECONNABORTED: 
				{
					strncpy(Error, "WSAECONNABORTED", ERROR_SIZE);
					break;
				}
				case WSAECONNRESET: 
				{
					strncpy(Error, "WSAECONNRESET", ERROR_SIZE);
					break;
				}
				case WSAETIMEDOUT: 
				{
					strncpy(Error, "WSAETIMEDOUT", ERROR_SIZE);
					break;
				}
				default:
				{
				strncpy(Error, "UNKNOWN", ERROR_SIZE);
				break;
				}
			}//end of switch
#endif
		}
		//else if(nSendBytes == 0)//socket的缓存区已满，则返回0//windowns
		//{
		//	//...
		//	//closesocket_ex(s);
		//}

		return nSendBytes;
}

//////////////////////////////////////////////////////////////////////
//--------------------------------TCP---------------------------------
// func description	:	从socket的缓存区中接收数据到用户缓存区
//
//-----input------
// Parameters
//     	s 			:	接收端套接字描述符(socket descriptor)
//		buff		:	用来存放recv函数接收到的数据的缓冲区（recv buffer）
//		nbytes		:	指明buff的长度（recv data length）
//		flags		:	recv flag (MSG_OOB、MSG_DONTROUTE、MSG_DONTWAIT、MSG_WAITALL)
//------output------
// Return 
//     返回值：成功返回读取的字节数，出错返回-1并设置errno，无数据可以读取，则返回0
//	   阻塞与非阻塞recv返回值没有区分，都是 <0 出错 =0 连接关闭 >0 接收到数据大小
//
INT SocketAPI::recv_ex(SOCKET s, VOID* buff, UINT nbytes, INT flags)
{
	INT nRecvBytes = -1;
		
#if defined(__LINUX__)
		nRecvBytes = recv(s, buff, nbytes, flags);
#elif defined(__WINDOWS__)
		nRecvBytes = recv(s, (const CHAR*)buff, nbytes, flags);
#endif
        //!alter by huyf 2018.03.15:修改
        //return nRecvBytes;
        //!alter end:修改


		if (nRecvBytes == -1)//<0 出错
		{
#if defined(__LINUX__)
			switch(errno)
			{
                //case EWOULDBLOCK://认为连接是正常的，继续接收
				case EAGAIN:
				case EINTR: 
                    {
                        
                        return nRecvBytes;//SOCKET_ERROR_WOULDBLOCK
                    }
				case ECONNRESET:
				case EPIPE:
				case EBADF: 
				case ENOTCONN: 
				case ENOTSOCK: 				
				case EFAULT: 
					return nRecvBytes;//-1

				default: 
					{
						break;
					}
			}//end of switch
#elif defined(__WINDOWS__)/*
			INT iErr = WSAGetLastError();
			switch(iErr)
			{
				case WSANOTINITIALISED: 
					strncpy(Error, "WSANOTINITIALISED", ERROR_SIZE);
					break;
				case WSAENETDOWN: 
					strncpy(Error, "WSAENETDOWN", ERROR_SIZE);
					break;
				case WSAEFAULT: 
					strncpy(Error, "WSAEFAULT", ERROR_SIZE);
					break;
				case WSAENOTCONN: 
					strncpy(Error, "WSAENOTCONN", ERROR_SIZE);
					break;
				case WSAEINTR: 
					strncpy(Error, "WSAEINTR", ERROR_SIZE);
					break;
				case WSAEINPROGRESS: 
					strncpy(Error, "WSAEINPROGRESS", ERROR_SIZE);
					break;
				case WSAENETRESET: 
					strncpy(Error, "WSAENETRESET", ERROR_SIZE);
					break;
				case WSAENOTSOCK: 
					strncpy(Error, "WSAENOTSOCK", ERROR_SIZE);
					break;
				case WSAEOPNOTSUPP: 
					strncpy(Error, "WSAEOPNOTSUPP", ERROR_SIZE);
					break;
				case WSAESHUTDOWN: 
					strncpy(Error, "WSAESHUTDOWN", ERROR_SIZE);
					break;
				case WSAEWOULDBLOCK: 
					//strncpy(Error, "WSAEWOULDBLOCK", ERROR_SIZE);				
					//break;
					return SOCKET_ERROR_WOULDBLOCK;				
				case WSAEMSGSIZE: 
					strncpy(Error, "WSAEMSGSIZE", ERROR_SIZE);
					break;
				case WSAEINVAL: 
					strncpy(Error, "WSAEINVAL", ERROR_SIZE);
					break;
				case WSAECONNABORTED: 
					strncpy(Error, "WSAECONNABORTED", ERROR_SIZE);
					break;
				case WSAETIMEDOUT: 
					strncpy(Error, "WSAETIMEDOUT", ERROR_SIZE);
					break;
				case WSAECONNRESET: 
					strncpy(Error, "WSAECONNRESET", ERROR_SIZE);
					break;
				default:
					{
						strncpy(Error, "UNKNOWN", ERROR_SIZE);
						break;
					};
			};//end of switch*/
#endif
		}
		else if (nRecvBytes == 0)//=0 连接关闭
		{
			/* code */
			//closesocket_ex(s);
            LOGERROR("[recv] 链接对方主动关闭 ret=[%d] errno[%d] discription[%s]",nRecvBytes, errno, strerror(errno));
                        
		}

		return nRecvBytes;
}

//////////////////////////////////////////////////////////////////////
//--------------------------------UDP---------------------------------
// func description	:	向socket的缓存中发送数据
//
//-----input------
// Parameters
//     	s 			:	指向一个打开的套接字描述符(socket descriptor)
//		buff		:	用户发送缓存区指针（send buffer）
//		nbytes		:	请求写入的字节数（send data length）
//		flags		:	send flag (MSG_OOB、MSG_DONTROUTE、MSG_DONTWAIT、MSG_WAITALL)
//		to			:   指向一个含有数据包接受者的协议地址的套接字地址结构
//		addrlen 	:   to结构的大小
//------output------
// Return 
//     返回值：成功返回写入的字节数，出错返回-1并设置errno，socket的缓存区已满，则返回0
//
INT SocketAPI::sendto_ex(SOCKET s, const VOID* buff, UINT nbytes, INT flags, const struct sockaddr* to, INT addrlen )
{
	INT nSendBytes = -1;

		
#if defined(__LINUX__)
		nSendBytes = sendto(s, buff, nbytes, flags, to, addrlen);
#elif defined(__WINDOWS__)
		nSendBytes = sendto(s, (const CHAR*)buff, nbytes, flags, to, addrlen);
#endif

		if (nSendBytes == -1)
		{
#if defined(__LINUX__)
			switch(errno)
			{
				//case EWOULDBLOCK: 
				case EAGAIN:
                return SOCKET_ERROR_WOULDBLOCK;
				case ECONNRESET:
				case EPIPE:
				case EBADF: 
				case ENOTSOCK: 
				case EFAULT: 
				case EMSGSIZE: 
				case ENOBUFS: 
				default: 
					{
						break;
					}
			}//end of switch		
#elif defined(__WINDOWS__)
			
#endif
		}
		else if(nSendBytes == 0)//socket的缓存区已满，则返回0
		{
			//...
		}

		return nSendBytes;
}

//////////////////////////////////////////////////////////////////////
//--------------------------------UDP---------------------------------
// func description	:	从socket的缓存中读取数据
//
//-----input------
// Parameters
//     	s 			:	指向一个打开的套接字描述符(socket descriptor)
//		buff		:	用户发送缓存区指针（send buffer）
//		nbytes		:	请求写入的字节数（send data length）
//		flags		:	send flag (MSG_OOB、MSG_DONTROUTE、MSG_DONTWAIT、MSG_WAITALL)
//		from		:   指向一个将由该函数在返回时填写数据报发送者的协议地址的套接字结构，
//		addrlen 	:   该套接字地址结构中填写的字节数则放在此处的整数中返回给调用者
//------output------
// Return 
//     返回值：成功返回写入的字节数，出错返回-1并设置errno，socket的缓存区已满，则返回0
//
INT SocketAPI::recvfrom_ex(SOCKET s, VOID* buff, UINT nbytes, INT flags, struct sockaddr* from, INT* addrlen)
{
	INT nRecvBytes = -1;

		
#if defined(__LINUX__)
		nRecvBytes = 0;//recvfrom(s, buff, nbytes, flags, from, addrlen);
#elif defined(__WINDOWS__)
		nRecvBytes = 0;//recvfrom(s, (const CHAR*)buff, nbytes, flags, from, addrlen);
#endif
		if (nRecvBytes == -1)
		{
#if defined(__LINUX__)
			switch(errno)
			{
				//case EWOULDBLOCK:
				case EAGAIN: 
					return SOCKET_ERROR_WOULDBLOCK;
				case ECONNRESET:
				case EPIPE:
				case EBADF: 
				case ENOTCONN: 
				case ENOTSOCK: 
				case EINTR: 
				case EFAULT: 
				default: 
					{
						break;
					}
			}//end of switch
#elif defined(__WINDOWS__)
			
#endif
		}
		else if (nRecvBytes == 0)
		{
			/* code */
		}

		return nRecvBytes;
}

//////////////////////////////////////////////////////////////////////
// func description	:	check if this socket is nonblocking mode
//
//-----input------
// Parameters
//     	s 			:	指向一个打开的套接字描述符(socket descriptor)
//------output------
// Return 
//     返回值：TRUE if nonblocking, FALSE if blocking,出错为-1
//
BOOL SocketAPI::getsocketnonblocking_ex(SOCKET s)
{

#if defined(__LINUX__)
    INT flags = FileAPI::fcntl_ex(s, F_GETFL, 0);
    if (flags < 0)
    {
        return FALSE;
    }

    return flags | O_NONBLOCK;

#elif  defined(__WINDOWS__) 
	return FALSE;
#endif

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// func description	:	make this socket blocking/nonblocking
//
//-----input------
// Parameters
//     	s 			:	指向一个打开的套接字描述符(socket descriptor)
//      on 			:	TRUE if nonblocking, FALSE if blocking
//------output------
// Return 
//     返回值：TRUE if nonblocking, FALSE if blocking,出错为-1
//
BOOL SocketAPI::setsocketnonblocking_ex(SOCKET s, BOOL on)
{

#if defined(__LINUX__)
	INT flags = FileAPI::fcntl_ex(s, F_GETFL, 0);
	if (on)
		// make nonblocking fd
		flags |= O_NONBLOCK;
	else
		// make blocking fd
		flags &= ~O_NONBLOCK;

	FileAPI::fcntl_ex(s , F_SETFL , flags );
	return TRUE;
#elif  defined(__WINDOWS__) 
	ULONG argp = (on == TRUE ) ? 1: 0;
	return ioctlsocket_ex(s, FIONBIO, &argp);
#endif

	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// func description	:	get amount of data in socket input buffer
//
//-----input------
// Parameters
//     	s		:	socket descriptor
//------output------
// Return 
//     返回值：amount of data in socket input buffer
//
BOOL SocketAPI::ioctlsocket_ex(SOCKET s, LONG cmd, ULONG* argp)
{

#if defined(__LINUX__)
#elif  defined(__WINDOWS__) 
		/*
		if(ioctlsocket(s, FIONREAD, &argp) == SOCKET_ERROR)
		{
			INT iErr = WSAGetLastError();
			switch (iErr) 
			{
			case WSANOTINITIALISED: 
				strncpy(Error, "WSANOTINITIALISED", ERROR_SIZE):
				break;
			case WSAENETDOWN: 
				strncpy(Error, "WSAENETDOWN", ERROR_SIZE):
				break;
			case WSAEINPROGRESS: 
				strncpy(Error, "WSAEINPROGRESS", ERROR_SIZE):
				break;
			case WSAENOTSOCK: 
				strncpy(Error, "WSAENOTSOCK", ERROR_SIZE):
				break;
			case WSAEFAULT: 
				strncpy(Error, "WSAEFAULT", ERROR_SIZE):
				break;
			default:
				{
				strncpy(Error, "UNKNOWN", ERROR_SIZE):
				break;
				};
			};

			return FALSE;
		}
		return TRUE;*/

#endif
		return FALSE;
}

//////////////////////////////////////////////////////////////////////
// func description	:	get amount of data in socket input buffer
//
//-----input------
// Parameters
//     	s		:	socket descriptor
//------output------
// Return 
//     返回值：amount of data in socket input buffer
//
UINT SocketAPI::availablesocket_ex(SOCKET s)
{

#if defined(__LINUX__)
	return FileAPI::availablefile_ex(s);
#elif  defined(__WINDOWS__) 
	ULONG argp = 0;
	ioctlsocket_ex(s, FIONREAD, &argp);
	return argp;
#endif
	return 0;
}

//////////////////////////////////////////////////////////////////////
// func description	:	shutdown all or part of connection of socket
//
//-----input------
// Parameters
//     	s		:	socket descriptor
//		how 	: 	how to close (all, send, receive)
//					SHUT_RD:关闭连接的读这一半，套接字不在有数据接收，而且套接字接收缓存区中的现有数据都被丢弃
//					SHUT_WR:关闭连接的写这一半，（TCP：半关闭）当前留在套接字发送缓存区中的数据将被发送掉，进程不能再对这样的套接字调用任何写函数。
//							后跟TCP的正常连接终止序列
//					SHUT_RDWR:连接的读写半部都关闭。至于close操作取决于SO_LINGER套接字选项参数。
//------output------
// Return
//     成功：0 出错：-1
//
BOOL SocketAPI::shutdown_ex(SOCKET s, INT howto)
{
	if (shutdown(s, howto) < 0) 
	{
#if defined(__LINUX__)
		switch(errno) 
		{
			case EBADF: 
			case ENOTSOCK: 
			case ENOTCONN: 
			default: 
				{
					break;
				}
		}

#elif defined(__WINDOWS__)
		/*
		INT iErr = WSAGetLastError();
		switch(iErr)
		{
		case WSANOTINITIALISED: 
			strncpy(Error, "WSANOTINITIALISED", ERROR_SIZE);
			break;
		case WSAENETDOWN:
			strncpy(Error, "WSAENETDOWN", ERROR_SIZE);
			break;
		case WSAEINVAL: 
			strncpy(Error, "WSAEINVAL", ERROR_SIZE);
			break;
		case WSAEINPROGRESS: 
			strncpy(Error, "WSAEINPROGRESS", ERROR_SIZE);
			break;
		case WSAENOTCONN: 
			strncpy(Error, "WSAENOTCONN", ERROR_SIZE);
			break;
		case WSAENOTSOCK: 
			strncpy(Error, "WSAENOTSOCK", ERROR_SIZE);
			break;
		default:
			{
				strncpy(Error, "UNKNOWN", ERROR_SIZE);
				break;
			}
		}*/
#endif

		return FALSE;
	}

	return TRUE;
}
