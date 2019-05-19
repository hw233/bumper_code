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

#ifndef __SOCKET_H__
#define __SOCKET_H__

#include "../Common/BaseType.h"
#include "../Common/Common.h"
#include "SocketAPI.h"
#include <sys/socket.h>  // fro SOCKADDR_IN、 INVALID_SOCKET
#include "../WebSocket/WebSocketHandler.h"


class CNetMsg;
class SocketInputStream;
class SocketOutputStream;

enum _SYN_MSG
{
	_SYN_MSG_NONE,
	_SYN_MSG_PREVIOUS,				//执行之前的操作(可以处理玩家操作 或者 空操作帧)
	_SYN_MSG_COLLIDE,				//下一帧立马执行碰撞操作(空帧)持续5帧
};
///////////////////////////////////////////////////////////////////
///
class Socket
{
public:
	// constructor
	Socket();
	Socket(const CHAR* host, UINT port);
	// destructor
	virtual ~Socket();

public:
	//!alter by huyf 2018.08.01:支持双栈支持IPV4和IPV6
	//create
	BOOL create(int nIPModule = 0);
	//!alter end:支持双栈支持IPV4和IPV6

	// try connect to remote host
	BOOL connect();
	BOOL connect(const CHAR* host, UINT port);
	// close previous connection and connect to another socket
	BOOL reconnect(const CHAR* host, UINT port);

	// send data to peer
	INT send(const VOID* buf, UINT len, UINT flags=0);

	// receive data from peer
	INT receive(VOID* buf, UINT len, UINT flags=0);

	//accept
	SOCKET accept(struct sockaddr* addr, UINT* len);

	//bind
	//!alter by huyf 2018.04.03:只定义不现实此函数,请注意调用bind带参数的函数。
	BOOL bind();
	BOOL bind(UINT port, UINT nAddr = 0);
	//!alter end:只定义不现实此函数

	//listen
	BOOL listen(INT backlog);

	// close connection
	VOID close();

	//get socket error
	UINT getSocketError();

	//check if socket is valid
	BOOL isValid() const;

	//get amount of data in socket input buffer
	INT available() const;
        
        //add by huyf 2018.01.24:支持I/O多路复用poll
#ifdef _POLL_
        void                                    SetClientPollFd(struct pollfd* pfd)                                     { CHECK(pfd);  m_pClientPollFd = pfd;   }
        struct pollfd*                          GetClientPollFd()                                                       { return m_pClientPollFd;               }
#endif
        //add end:支持I/O多路复用poll 
        
        //add by huyf 2018.01.24:支持I/O多路复用epoll
#ifdef _EPOLL_
        void                                    SetClientEPollEvent(struct epoll_event* pEpollEvent)                   { CHECK(pEpollEvent);  m_pEpollEvent = pEpollEvent;   }
        struct epoll_event*                     GetClientEPollEvent()                                                  { return m_pEpollEvent;                               }
#endif
        //add end:支持I/O多路复用epoll 
	//////////////////////////////////////////
	//设置socket attrs
public:
	// get/set socket's linger status
	UINT getLinger() const;
	BOOL setLinger(UINT lingertime);

	// get/set socket's nonblocking status
    BOOL isNonBlocking()const;
    BOOL setNonBlocking(BOOL on = TRUE);
	
	//keepalive
	BOOL isKeepAlive() const;
	BOOL setKeepAlive(BOOL on=TRUE);	

	//resuse addr
	BOOL isReuseAddr() const;
	BOOL setReuseAddr(BOOL on=TRUE);

	//resuse port
	BOOL isReusePort() const;
	BOOL setReusePort(BOOL on=TRUE);

	//接收缓冲区低水位标记
	INT  getRcvLowAt() const;
	BOOL setRcvLowAt(BOOL on=TRUE);

	//发送缓冲区低水位标记
	INT  getSendLowAt() const;
	BOOL setSendLowAt(BOOL on=TRUE);

	//接收超时
	BOOL getRcvTimeO(struct timeval& timeout);
	BOOL setRcvTimeO(struct timeval& timeout);

	//发送超时
	BOOL getSendTimeO(struct timeval& timeout);
	BOOL setSendTimeO(struct timeval& timeout);	

	//get/set receive buffer size
	UINT getReceiveBufferSize() const;
	BOOL setReceiveBufferSize(UINT size);

	//get/set send buffer size
	UINT getSendBufferSize() const;
	BOOL setSendBufferSize(UINT size);

	//check is error
	INT  isSockError() const;	

	//////////////////////////////////////////
	//禁止nagle算法
	BOOL isNoDelay() const;
	BOOL setNoDelay(BOOL on=TRUE);

	//TCP最大分节大小
	INT  getMaxSeg() const;
	BOOL setMaxSeg(INT size);

	//不经历TIME_WAIT的过程*设置了SO_DONTLINGER（也就是说将linger结构的l_onoff域设为零*
	//SO_DONTLINGER 若为真，则SO_LINGER选项被禁止
	BOOL setDontLinger(BOOL on=TRUE);

	BOOL setSocketOpt(UINT bLinger=TRUE, BOOL bReuseAddr=TRUE, BOOL bReusePort=TRUE, BOOL bNoDelay=TRUE, BOOL bKeepAlive=TRUE, BOOL bNonBlocking=TRUE, BOOL bDontLinger=TRUE,UINT unSendBuffSize=16*1024, UINT unRecvBuffSize=32*1024);

	WebsocketHandler*       GetWebSocketHandler()												{	return m_handler; }
	INT 					GetFirstHand()														{ 	return m_nFirstHand; }
	VOID 					SetFirstHand(INT nHandIndex) 										{ 	m_nFirstHand = nHandIndex; }
	VOID 					SetRecvMsgTimeStamp(long long llTimeStamp)							{	m_llRecvMsgTimeStamp = llTimeStamp;	}
	long long 				GetRecvMsgTimeStamp()												{	return m_llRecvMsgTimeStamp;	}
	VOID 					SetCollideSynMsgTimeStamp(long long llCollideSynMsgTimeStamp)		{	m_llCollideSynMsgTimeStamp = llCollideSynMsgTimeStamp;	}
	long long 				GetCollideSynMsgTimeStamp()											{	return m_llCollideSynMsgTimeStamp;	}
	VOID 					SetCollideSynMsgCount(INT nCollideSynMsgCount)						{	m_nCollideSynMsgCount = nCollideSynMsgCount;	}
	INT 					GetCollideSynMsgCount()												{	return m_nCollideSynMsgCount;	}
	VOID 					SetSynMsgStatus(INT nSynMsgStatus)									{	m_nSynMsgStatus = nSynMsgStatus;	}
	INT 					GetSynMsgStatus()													{	return m_nSynMsgStatus;	}
	INT       				ReadNetData(SocketInputStream* pSocketInputStream, INT& nPackageID, UINT& nNeedReadTotalLengh);
	BOOL 					SendMsgToClient(SocketOutputStream* pSocketOutputStream, CNetMsg* pMsg);
	BOOL 					Write(SocketOutputStream* pSocketOutputStream, const CHAR* pData, UINT nDataLen);

public:
	//get socket discreption
	SOCKET  getSocketID();
    void    setSocketID(SOCKET sock){  m_SocketID = sock;  }

	//get hostip
	//CHAR*  getHostIP();
	IP_t  getHostIP() const;

	//get port
	UINT   getPort() const;

	//!alter by huyf 2018.08.01:支持双栈支持IPV4和IPV6
	//get sockaddr
	SOCKADDR_IN& getSocketAddIn();   
	SOCKADDR_IN6& getSocketAddIn6();        
	//!alter end:支持双栈支持IPV4和IPV6
       
#ifdef _EPOLL_
    /*
     * 函数说明：该函数生成一个epoll专用的文件描述符
     * size: 其中的参数是指定生成描述符的最大范围。
     */
    int epollCreate(int size); 
    
    /*
     * 函数说明：用于控制某个文件描述符上的事件，可以注册事件、修改事件、删除事件
     * epfd：由 epoll_create 生成的epoll专用的文件描述符； 
     * op：要进行的操作，可能的取值EPOLL_CTL_ADD 注册、EPOLL_CTL_MOD 修改、EPOLL_CTL_DEL 删除； 
     * fd：关联的文件描述符； 
     * event：指向epoll_event的指针； 
     * 如果调用成功则返回0，不成功则返回-1。
     */
    int epollCtl(int epfd,int op, int fd, struct epoll_event *event); 
    
    /*
     * 函数说明：该函数用于轮询I/O事件的发生。 
     * epfd：由epoll_create 生成的epoll专用的文件描述符； 
     * epoll_event：用于回传代处理事件的数组； 
     * maxevents：每次能处理的事件数； 
     * timeout：等待I/O事件发生的超时值； 
     * 返回发生事件数。
     */
    //int epollWait(int epfd, struct epoll_event * events, int maxevents, int timeout);
#endif 

public:
	//socket id
	SOCKET m_SocketID;
      
    //add by liufei 2018.10.19:添加websocket的对接
    //*****************注意******************
    ///*websocket会对原始数据进行加密，对端websocket对收到的数据进行揭秘
    ///*数据长度小126的时候，websocket封装后包头增加6个字节
    ///*大于126~XXX的时候，websocket封装后包头增加8个字节
    ///*大于XXX的时候，websocket封装后包头增加10个字节
    ///*/
	WebsocketHandler*      m_handler;
	int 					m_nFirstHand; 
	//add end:添加websocket的对接
	
    //add by huyf 2018.01.24:支持I/O多路复用poll
	//留作后期使用
#ifdef _POLL_  
    struct pollfd*   		m_pClientPollFd;
#endif
    //add end:支持I/O多路复用poll

    //add by huyf 2018.01.24:支持I/O多路复用epoll
#ifdef _EPOLL_
    // 创建一个epoll句柄  
    int 					m_EpollFd;
    struct epoll_event*		m_pEpollEvent;
#endif        
    //add end:支持I/O多路复用epoll
	
	//!alter by huyf 2018.08.01:支持双栈支持IPV4和IPV6
	// socket address structure
	SOCKADDR_IN  m_SockAddr;
	SOCKADDR_IN6 m_SockAddr6;
	//!alter end 2018.08.01:支持双栈支持IPV4和IPV6
	
	// peer host
	CHAR m_Host[IP_SIZE];
	
	// peer port
	UINT m_Port;

	//add by huyf 2018.11.02:添加服务器收到前端消息的时间戳,暂时不做缓存帧
	//时序图:前端逻辑帧（33ms : 30HZ/1秒） 服务器逻辑帧（66ms : 15HZ/1秒），暂时先不做帧缓存试试。
	//                          
	//	  Client-A 							      Server 							   	 Client-B					commit
	//	  |----a1----|>>>>>>>>>>>>>[a1]>>>>>>>>>>|  [a1]   |					 		|----------|				客户端A请求操作包【1】
	//	  |---[a1]---|<<<<<<<<<<<<<[a1]<<<<<<<<<<|---------|>>>>>>>>>>>>>>>>>>>>>>>>>>>>|---[a1]---|				服务器广播A的操作包【1】，此时B未发送操作包
	//	  |---[a2]---|>>>>>>>>>>>>>[a2]>>>>>>>>>>| [a2,b1] |<<<<<<<<<<<<<[b1]<<<<<<<<<<<|----------|				客户端A和客户单B都发送了操作请求包，则服务器进行立即转发
	//	  |-[a2,b1]--|<<<<<<<<<<<<<[a1,b2]<<<<<<<|---------|>>>>>>>>>>>>[a2,b1]<<<<<<<<<|-[a2,b1]--|				客户端A和客户端B收到操作包a2和操作数据包b1,进行渲染工作
	//	  |----------|			没有操作包同步   |  [b2]   |<<<<<<<<<<<<<[b2|<<<<<<<<<<<|----------|				客户端A没有发送操作，客户端B发送操作包B
	//	  |-[ac,b2]--|<<<<<<<<<<<<<[ac,b2]<<<<<<<|---------|>>>>>>>>>>>>>[ac,b2]>>>>>>>>|-[ac,b2]--|				服务器广播客户端A和客户端B操作包【ac,b2】于全部的客户端玩家，收到ac的信息，继续做上一帧的动画。
	//用作服务器同步前端继续执行信息包，服务器做帧轮休，当没有某一帧没有前端操作数据包过来的时候
	long long m_llRecvMsgTimeStamp;
	long long m_llCollideSynMsgTimeStamp;
	INT       m_nCollideSynMsgCount;						//碰撞之后服务器主动给前端发送数据包，执行碰撞后的操作
	INT       m_nSynMsgStatus;								//同步消息的状态
	//add end:添加服务器收到前端消息的时间戳,暂时不做缓存帧
};

#endif
