////////////////////////////////////////////////////////////////
//文件名称：SocketAPI.h
//功能描述：原始套接字socket接口封装，对网络socket进行一个简单的封装操作，针对不同的系统实现相同的接口调用，
//			无其他原因请不要随便修改！！！！
//版本说明：SocketAPI API
//			
//编写作者：2017.08.09 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//定义头文件只会被其他文件包含一次
////////////////////////////////////////////////////////////////
//#if __MSC_VER > 1000
//#pragma once
//#endif

#ifndef __SOCKET_API_H__
#define __SOCKET_API_H__

////////////////////////////////////////////////////////////////
//提示一些报错信息，暂定此文件只能使用在LINUX系统上
////////////////////////////////////////////////////////////////
//#if !defined(__LINUX__)
//#error ERROR: Only In Linux Targets Supported!
//#endif

////////////////////////////////////////////////////////////////
//定义字节对其
////////////////////////////////////////////////////////////////
#ifdef __MSC_VER
#pragma pack(push, 4)
#endif

////////////////////////////////////////////////////////////////
//定义C++可以读取c文件，下面的代码按照c的语言解析来编译
////////////////////////////////////////////////////////////////
/*#ifndef __cpluscplus  
extern "C"  //C++  
{  
#endif */

////////////////////////////////////////////////////////////////
//当前系统包含的头文件引用
////////////////////////////////////////////////////////////////
#include "../Common/BaseType.h"
#include "../Common/Common.h"
/*
#if defined(__WINDOWS__)
	#include <WinSock.h>
#elif defined(__LINUX__)
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <fcntl.h>
#endif
*/



//////////////////////////////////////////////////////////////////////
//
// Platform Independent Socket API Collection (exception based)
//
//////////////////////////////////////////////////////////////////////
namespace SocketAPI 
{


	//
	// exception version of socket ()
	//
	SOCKET socket_ex(INT family, INT type, INT protocol);

	//
	// exception version of getsocketopt ()
	//
	BOOL getsocketopt_ex(SOCKET s, INT level, INT optname, VOID* optval, INT* optlen);

	//
	// exception version of getsocketopt2 ()
	//
	//UINT getsocketopt_ex2(SOCKET s, INT level, INT optname, VOID* optval, INT* optlen);
	
	
	//
	// exception version of setsocketopt ()
	//
	BOOL setsocketopt_ex(SOCKET s, INT level, INT optname, const VOID* optval, UINT optlen);

	//
	// exception version of bind ()
	//
	BOOL bind_ex(SOCKET s, const struct sockaddr* myaddr, UINT addrlen);

	//
	// exception version of connect ()
	//
	BOOL connect_ex(SOCKET s, const struct sockaddr* servaddr, UINT addrlen);

	//
	// exception version of reconnect_ex ()
	//
	BOOL reconnect_ex(SOCKET s, const struct sockaddr* servaddr, UINT addrlen);

	//
	// exception version of listen ()
	//
	BOOL listen_ex(SOCKET s, INT backlog = MAX_BACKLOG);

	//
	// exception version of listen ()
	//
	BOOL listen_ex2(SOCKET s, INT backlog);

	//
	// exception version of accept ()
	//
	SOCKET accept_ex(SOCKET s, struct sockaddr* cliaddr, UINT* addrlen);

	//
	// TCP exception version of send ()
	//
	INT send_ex(SOCKET s, const VOID* buff, UINT nbytes, INT flags);

	//
	// TCP exception version of recv ()
	//
	INT recv_ex(SOCKET s, VOID* buff, UINT nbytes, INT flags);

	//
	// UDP exception version of send ()
	//
	INT sendto_ex(SOCKET s, const VOID* buff, UINT nbytes, INT flags, const struct sockaddr* to, INT addrlen);

	//
	// UPD exception version of recv ()
	//
	INT recvfrom_ex(SOCKET s, VOID* buff, UINT nbytes, INT flags, struct sockaddr* from, INT* addrlen);

	//
	// check if socket is nonblocking mode
	//
	BOOL getsocketnonblocking_ex(SOCKET s);

	//
	// make socket nonblocking mode
	//
	BOOL setsocketnonblocking_ex(SOCKET s, BOOL on);

	//
	// get amount of data in socket input buffer
	//
	UINT availablesocket_ex(SOCKET s);

	//
	// exception version of shutdown()
	//
	BOOL shutdown_ex(SOCKET s, INT howto);

	//
	// exception version of select()
	//
	INT select_ex(INT maxfdp1, fd_set* readset, fd_set* writeset, fd_set* exceptset, struct timeval* timeout);
        
	//add by huyf 2018.01.24:支持I/O多路复用poll
	//
	// exception version of poll_ex()
	//
	INT poll_ex(struct pollfd *fds, nfds_t nfds, int timeout); 
	//add end:支持I/O多路复用poll
        
    //add by huyf 2018.01.30:支持I/O多路复用epoll
    //
	// exception version of epoll_wait_ex()
	//
	INT epoll_wait_ex(int epfd, struct epoll_event * events, int maxevents, int timeout);
     //add end:支持I/O多路复用epoll
        
	//
	// exception version of ioctlsocket()
	//
	// *CAUTION*
	//
	// in UNIX, ioctl() used instead
	//
	BOOL ioctlsocket_ex(SOCKET s, LONG cmd, ULONG* argp);

	//
	// exception version of fork ()
	//
	pid_t fork_ex(void);

	//
	// exception version of close ()
	//
	BOOL closesocket_ex(SOCKET s);
}





#endif