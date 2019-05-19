////////////////////////////////////////////////////////////////
//文件名称：BaseType.h
//功能描述：系统底层类型基础文件，无授权人员禁止修改此文件内容。
//版本说明：windows操作系统需要定义宏：__WINDOWS__
//			linux操作系统需要定义宏：__LINUX__
//编写作者：2017.08.08 yanfeng.hu
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

#ifndef __BASE_TYPE_H__
#define __BASE_TYPE_H__


////////////////////////////////////////////////////////////////
//提示一些报错信息，暂定此文件只能使用在LINUX系统上
////////////////////////////////////////////////////////////////
#if !defined(__LINUX__)
//#error ERROR: Only In Linux Targets Supported!
#endif

//#ifndef __WINDOWS__
//	#define __WINDOWS__
//#endif
#ifndef __LINUX__
	#define __LINUX__
#endif

////////////////////////////////////////////////////////////////
//当前系统包含的头文件引用
////////////////////////////////////////////////////////////////
#if defined(__WINDOWS__)
	#pragma warning (disable : 4786)
	#pragma warning (disable : 4996)
	#include <Windows.h>
	#include "crtdbg.h"
#elif defined(__LINUX__)
	#include <sys/types.h>
	#include <pthread.h>
	#include <execinfo.h>
	#include <signal.h>
	#include <exception>
	#include <setjmp.h>
	#include <sys/epoll.h>

	#include <arpa/inet.h>
#endif

#include <assert.h>		//for assert()

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>       //time、time_t
#include <math.h>
#include <stdarg.h>
using namespace std;
#include <error.h>         //perror
#include <errno.h>         //perror/sys_errlist/sys_nerr/errno
#include <string.h>
#include <netinet/tcp.h>	
//add by huyf 2018.01.24:支持I/O多路复用poll
#include <sys/poll.h>
#include <poll.h>   
#include <unistd.h>   
#include <stdlib.h>   
#include <sys/wait.h>       
#include <sstream>    //使用stringstream需要引入这个头文件  
using namespace std;
#include <sys/resource.h>   //getrlimit/setrlimit/rlimit 
#include <sys/time.h>
//add end:支持I/O多路复用poll 
//add by huyf 2018.01.24:支持I/O多路复用epoll
#include <algorithm>
#include <sys/epoll.h>  //epoll ways file 
#include <vector>
using namespace std;
#define hand_error(msg) do{perror(msg); exit(EXIT_FAILURE);}while(0)
//add end:支持I/O多路复用epoll

//tcp
#if defined(__WINDOWS__)
	#include <WinSock.h>
#elif defined(__LINUX__)
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <fcntl.h>      //block and noblock
#endif

///////////////////////////////////////////////////////////////////////
//基本头文件引用
///////////////////////////////////////////////////////////////////////

//#include "GameUtil.h"
//#include "PacketDefine.h"

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
//标准数据类型重定义
////////////////////////////////////////////////////////////////
//#define VOID	void									//标准空
typedef	void	VOID;									//标准空
typedef	unsigned char	UCHAR;							//标准无符号char
typedef	char	CHAR;									//标准有符号char
typedef	unsigned int	UINT;							//标准无符号int
typedef int 	INT;									//标准有符号int
typedef	unsigned short USHORT;							//标准无符号short
typedef	short 	SHORT;									//标准有符号short
typedef	unsigned long ULONG;							//标准无符号long
typedef	long 	LONG;									//标准有符号long
typedef	float	FLOAT;									//标准无符号float

typedef UCHAR uchar;
typedef USHORT ushort;
typedef UINT uint;
typedef ULONG ulong;
typedef ULONG IP_t;
//typedef USHORT PacketID_t;
typedef UINT PacketID_t;
typedef INT BOOL;
typedef UCHAR BYTE;
typedef const CHAR* LPCTSTR;

typedef uint8_t		UInt8;
typedef uint16_t	UInt16;
typedef uint32_t	UInt32;
typedef uint64_t	UInt64;
typedef UInt32 		UINT32;
typedef uint64_t	_int64;
typedef UInt32 		OBJID;


//IP地址的字符最大长度
//!alter by huyf 2018.08.01:支持双栈支持IPV4和IPV6
#define IP_SIZE			128//24
//#define IP_SIZE_IPV6	128
//!alter end:支持双栈支持IPV4和IPV6
#define KEY_SIZE		64
#define MAX_WORLDCOUNT	256
#define TIMELENTH		23
//#define OVER_MAX_SERVER	256
//add by huyf 2018.11.02:添加服务器收到前端消息的时间戳,暂时不做缓存帧
//修改为读取配置文件:g_config.ONLINE_BROADCAST_FRAME_MSG_INTERVAL_TIME
//时序图:前端逻辑帧（33ms : 30HZ/1秒） 服务器逻辑帧（66ms : 15HZ/1秒），暂时先不做帧缓存试试。
//const int _MSG_INTERVAL_TIME_FPS = 66;		//针对帧同步的操作的MsgWalk
////add by huyf 2018.11.02:添加服务器收到前端消息的时间戳,暂时不做缓存帧

////////////////////////////////////////////////////////////////
//网络信息预定义宏
////////////////////////////////////////////////////////////////

//add by huyf 2018.01.24:支持I/O多路复用poll
//后面转移到ini配置文件
extern int g_IoModelSelect;
extern int g_IoModelPoll;
extern int g_IoModelEPoll;


#ifndef _SELECT_
	//#define _SELECT_  1     
#endif
#ifndef _POLL_
	//#define _POLL_ 1
#endif
#ifndef _EPOLL_
	#define _EPOLL_ 1
#endif
//multiplexing_api:epoll
/*
#ifdef _SELECT_
    #ifndef FD_SETSIZE 
        #define FD_SETSIZE 2
    #endif //FD_SETSIZE 
#endif

#ifdef _POLL_
    #ifndef POLL_SETSIZE 
        #define POLL_SETSIZE 2
    #endif //POLL_SETSIZE 
#endif

#ifdef _EPOLL_
    #ifndef EPOLL_SETSIZE 
        #define EPOLL_SETSIZE 2
    #endif //EPOLL_SETSIZE 
#endif*/

#ifdef _SELECT_
    #ifndef FD_SETSIZE
        #define FD_SETSIZE      1024
    #endif //FD_SETSIZE
    #ifndef _SETSIZE_
        #define _SETSIZE_       FD_SETSIZE
    #endif //_SETSIZE_
#elif _POLL_
    #ifndef POLL_SETSIZE
        #define POLL_SETSIZE    2048
    #endif //POLL_SETSIZE
    #ifndef _SETSIZE_
        #define _SETSIZE_      POLL_SETSIZE
    #endif //_SETSIZE_
#elif _EPOLL_
    #ifndef EPOLL_SETSIZE
        #define EPOLL_SETSIZE   2048
    #endif //EPOLL_SETSIZE
    #ifndef _SETSIZE_
        #define _SETSIZE_       EPOLL_SETSIZE
    #endif //_SETSIZE_
#else
    #ifndef ZERO_SETSIZE
        #define ZERO_SETSIZE    0
    #endif //ZERO_SETSIZE
    #ifndef _SETSIZE_
        #define _SETSIZE_       ZERO_SETSIZE
    #endif //_SETSIZE_ 
#endif


//GamePlayer管理器的上限
#ifndef MAX_PLAYER 
	#define MAX_PLAYER _SETSIZE_//最大在线承载人数
#endif //MAX_PLAYER 

//玩家池上限
#ifndef MAX_POOL_SIZE 
	#define MAX_POOL_SIZE _SETSIZE_
#endif //MAX_POOL_SIZE 

#define CLIENT_MAX_SIZE  _SETSIZE_

//add end:支持I/O多路复用poll

////////////////////////////////////////////////////////////////
//常量
////////////////////////////////////////////////////////////////
//套接字接收缓存区大小
#define RECEIVE_BUFFER_SIZE 32*1024
//套接字发送缓存区大小
#define SEND_BUFFER_SIZE 16*1024

//单个数据包的最大长度
#define MSG_MAX_SIZE 4096

const int MAX_BACKLOG = 512;

//add by huyf 2018.11.23:好友系统
const int MAX_FRIEND_AMOUNT = 100;
//add end:好友系统


////////////////////////////////////////////////////////////////
//网络信息预定义宏
////////////////////////////////////////////////////////////////
#ifndef ERROR_SIZE 
#define ERROR_SIZE 256
#endif //ERROR_SIZE 

#ifdef __LINUX__
	typedef INT SOCKET;
	#define INVALID_SOCKET -1
	//#ifndef SOCKET_ERROR_EX		
	//	#define SOCKET_ERROR_EX 0xFFFF
	//#endif
#endif

static const INT SOCKET_ERROR_WOULDBLOCK_EX = -100;
typedef struct sockaddr SOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;
static const UINT scnSOCKADDR_IN = sizeof(SOCKADDR_IN);
//!alter by huyf 2018.08.01:支持双栈支持IPV4和IPV6
typedef struct sockaddr_in6 SOCKADDR_IN6;
static const UINT scnSOCKADDR_IN6 = sizeof(SOCKADDR_IN6);
//!alter end:支持双栈支持IPV4和IPV6

//////////////////////////////////////////////////////////////////////
// 控制变量
const int	THREADDELAY_MS			= 400;			// 等待内部消息的最大时间
const int	MAPGROUP_ONTIMER_MS		= 500;			// 地图组线程的ONTIMER频率
const int	SOCKETKERNEL_ONTIMER_MS	= 1000;			// 网络线程的ONTIMER频率
const int	WORLDKERNEL_ONTIMER_MS	= 1000;			// 世界线程的ONTIMER频率
const int	TIMER_OFFSET_LIMIT		= 10;			// TIMER时钟误差，大于该值时会自动纠正
const int	SOCKET_PROCESS_INTERVAL	= 40;			// SOCKET轮循的频率
const int	CLOSETHREAD_MILLISECS	= 30000;		// 等待线程关闭的毫秒数

//////////////////////////////////////////////////////////////////////
// 通用常量
const int	_MAX_ACCOUNTSIZE	= 24;  	
const int	IPSTR_SIZE			= 64;
const int 	_MAX_NAMESIZE 		= 32;
const int	_MAX_TITLESIZE 		= 128;
#define 	NAME_SIZE 			28
#define		SYSTEM_NAME 		"SYSTEM"			// 广播消息时代表“系统”的名字串
#define		ALLUSERS_NAME		"ALLUSERS"			// TALK中代表“大家”的名字串
#define		REPLAY_OK_STR		"ANSWER_OK"			// 申请新帐号或LOGIN时返回的成功标志串。

const int	ID_NONE			= 0;

typedef	char			NAMESTR[_MAX_NAMESIZE];
typedef	char			SQLBUF[1024];
typedef	char			MSGBUF[1024];
//typedef	char			MESSAGESTR[MAX_MESSAGESIZE];
//typedef	char			PARAMBUF[MAX_PARAMSIZE];
//typedef	char			TENETSTR[MAX_TENETSIZE];

////////////////////////////////////////////////////////////////
//基本数据宏定义
////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
//add by huyf 2018.02.26:版本号
#define	GAME_TITLE			"OnLineServer-服务器"
#define	LOGFILE_DIR			"syslog"
#define	LOGFILE_FILENAME	"syslog\/ONLINE-SERVER"
//const int _MAX_NAMESIZE = 32;
//const int _MAX_TITLESIZE = 128;
//!alter by huyf 2018.03.27:修改为读取配置文件
//#define REPORT_INTERVAL_TIME 10
//const int _RECONNECT_INTERVAL_TIME_TO_ACC = 30;
//const int _RECONNECT_INTERVAL_TIME_TO_MTS = 30;
//const int _RECONNECT_INTERVAL_TIME_TO_PW = 30;
//!alter end:修改为读取配置文件

//const int	GAME_VERSION	= 108;		// 主版本号，客户端和服务器必须同步才能运行。
// version define
//关于版本号
//!alter by huyf 20091103 : 版本分线支持
//#if defined(_DEBUG)
//	static int g_nServerVersion=0;
	const double _VERSION				=4;
	const double _VERSION_2				=1;
	const double _SERVER_VERSION		=4.20181207;//服务器内部版本号
//#else
//	static int g_nServerVersion=0;
//	const double _VERSION				=1151;
//	const double _VERSION_2				=1;
//	const double _SERVER_VERSION		=1151.20180226;//服务器内部版本号
//#endif

//修改版本号规则
//版本号说明:按以上举例
//千位				封测为1，内测为2，公测为3，正式版本为4
//个，十，百位		重大版本更新换代
//.后面n位			小版本更新
//服务器这边只记录大版本号，同一大版本号的客户端都能登陆

//版本号说明:按以上举例
//.前面的1位	游戏正式推出市场为1，封测为0
//.后面1位		重大版本更新换代
//.后面2.3位	小版本更新
//.后面4位		7为年份
//.后面5.6位	月份
//.后面7.8位	为日期
//客户端只显示0.301，年份开始只是为了我们内部方便查看
//add end:版本号


#define PACK_COMPART "$-$"
#define PACK_COMPART_SIZE strlen(PACK_COMPART)

//无效句柄
#define INVALID_HANDL	-1

//无效id
#define INVALID_ID		-1

//真
#ifndef TRUE
	#define TRUE	1
#endif

//假
#ifndef FALSE
	#define FALSE 	0
#endif

#ifndef INFINITE
	#define INFINITE 	-1
#endif
	//nst INT INFINITE = -1;

//#ifndef LPVOID
//	#define void *LPVOID
//#endif
	

//文件最大路径长度
#ifndef _MAX_PATH
	#define _MAX_PATH	260
#endif

//定义socket send recv变量
#ifndef SIZE_T//size_t--32位系统上是32位无符号整形unsigned int--在64位系统上定义为 unsigned long 
	typedef size_t SIZE_T;
#endif

#ifndef SSIZE_T//ssize_t--ssize_t是signed size_t
	typedef ssize_t SSIZE_T;
#endif

#ifndef SOCKLEN_T//socklen_t--socklen_t是int
	typedef socklen_t SOCKLEN_T;
#endif

#ifndef SOCKET_ERROR_WOULDBLOCK
	#define SOCKET_ERROR_WOULDBLOCK	11
#endif

enum SOCKET_ERROR_CODE
{
	SOCKET_WOULDBLOCK = 1,
	SOCKET_CLOSE = 0,
	SOCKET_ERROR = -1,
};



////////////////////////////////////////////////////////////////
//调试宏预定义
////////////////////////////////////////////////////////////////
#if defined(NODEBUG)
	#define __ENTER_FUNCTION_FOXNET if(1){
	#define __LEAVE_FUNCTION_FOXNET }
#else
	#define __ENTER_FUNCTION_FOXNET if(1){
	#define __LEAVE_FUNCTION_FOXNET }
#endif
//-----------------------------------------------------------//
#if defined(NODEBUG)
	#define MY_TRY try
	#define MY_CATCH catch(...)
#else
	#define MY_TRY try
	#define MY_CATCH catch(...)
#endif
//-----------------------------------------------------------//
#if defined(__WINDOWS__)
	#if defined(NDEBUG)
	  	#define __ENTER_FUNCTION {try{
	  	#define __LEAVE_FUNCTION }catch(...){AssertSpecial(FALSE,__FUNCTION__);}}
	#else
	  #define __ENTER_FUNCTION {try{
	  #define __LEAVE_FUNCTION }catch(...){AssertSpecial(FALSE,__FUNCTION__);}}
 	#endif
#elif defined(__LINUX__)//Linux
	#define __ENTER_FUNCTION {try{
	#define __LEAVE_FUNCTION }catch(...){}}
#endif


/*
#ifndef __cplusplus
}  
#endif */

enum {
	OBJ_NONE	= 0x1000,
	OBJ_USER	= 0x1001,
	OBJ_MONSTER	= 0x1002,
	OBJ_ITEM	= 0x1004,
	OBJ_MAP		= 0x1008,
	OBJ_FRIEND	= 0x1010,
	OBJ_EUDEMON	= 0x1020,
//	OBJ_NPCTYPE	= 0x1020, 
	OBJ_NPC		= 0x1040,
	OBJ_MAPITEM	= 0x1080,
	OBJ_SYN		= 0x1100,
	OBJ_BOOTH	= 0x1200,
	OBJ_TRAP	= 0x1400,
	OBJ_TUTOR	= 0x1800,
	OBJ_SYNRES	= 0x2000,
};
inline bool IsObjType(OBJID idObjType, OBJID idUnion) { return (idObjType & idUnion & 0x0FFF) != 0; }

#endif //__BASE_TYPE_H__
