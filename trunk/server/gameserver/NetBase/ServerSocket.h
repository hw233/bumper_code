////////////////////////////////////////////////////////////////
//文件名称：ServerSocket.h
//功能描述：服务器端socket
//文件名称：ServerSocket API
//			
//编写作者：2017.08.15 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
////////////////////////////////////////////////////////////////

#ifndef __SERVER_SOCKET_H__
#define __SERVER_SOCKET_H__

//////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////
#include "../Common/BaseType.h"
#include "../Common/Common.h"
#include "Socket.h"


////////////////////////////////////////////////////////////////
//常量
////////////////////////////////////////////////////////////////

class ServerSocket 
{
public :
	
	// constructor
	ServerSocket();
	
	// destructor
	virtual ~ServerSocket();
	

//////////////////////////////////////////////////
// methods
//////////////////////////////////////////////////
public:
    //creat socket
    BOOL Create();
    
    //bind socket
    BOOL Bind(INT port, UINT nAddr = 0);
    
    //lisent socket
    BOOL Listen(UINT backlog = MAX_BACKLOG); 
    
    //add by huyf 2018.01.24:支持I/O多路复用epoll
#ifdef _EPOLL_        
    //EpollCreate
    BOOL EpollCreate(INT maxEvents);
    
    //EpollCtl
    BOOL EpollCtl(int epfd, int op, int fd, struct epoll_event *event);
#endif        
    //add end:支持I/O多路复用epoll
        
        
	// close socket
	VOID Close();
	
	// accept new connection
	INT Accept(Socket* socket);
        
	// get/set socket's linger status
    UINT GetLinger() 							{ 	return m_pSocket->getLinger(); 				}
    BOOL SetLinger(UINT lingertime) 			{ 	return m_pSocket->setLinger(lingertime); 			}

    // get/set socket's reuseaddr status
    UINT GetReuseAddr() 						{ 	return m_pSocket->isReuseAddr(); 			}
    BOOL SetReuseAddr(BOOL on=TRUE) 			{ 	return m_pSocket->setReuseAddr(on); 				}

    // get/set socket's reuseaport status
    UINT GetReusePort() 						{ 	return m_pSocket->isReusePort(); 			}
    BOOL SetReusePort(BOOL on=TRUE) 			{ 	return m_pSocket->setReusePort(on); 				}

    //禁止nagle算法
	BOOL GetNoDelay() 							{ 	return m_pSocket->isNoDelay(); 				}
	BOOL SetNoDelay(BOOL on=TRUE)				{ 	return m_pSocket->setNoDelay(on); 					}
 
     // get/set socket's nonblocking status
    BOOL GetNonBlocking() const 				{ 	return m_pSocket->isNonBlocking(); 			}
    BOOL SetNonBlocking(BOOL on = TRUE) 		{ 	return m_pSocket->setNonBlocking(on); 				}
 
    // get/set receive buffer size
    UINT GetReceiveBufferSize() const 			{ 	return m_pSocket->getReceiveBufferSize();	}
    BOOL SetReceiveBufferSize(UINT size) 		{ 	return m_pSocket->setReceiveBufferSize(size); 		}
 
    // get/set send buffer size
    UINT GetSendBufferSize() const 				{ 	return m_pSocket->getSendBufferSize(); 		}
    BOOL SetSendBufferSize(UINT size) 			{ 	return m_pSocket->setSendBufferSize(size); 		}

    BOOL SetDontLinger(BOOL on=TRUE)            {   return m_pSocket->setDontLinger(on);      }
 
	SOCKET GetSocketID() 						{ 	return m_pSocket->getSocketID(); 			}
        
    //add by huyf 2018.01.24:支持I/O多路复用poll
    Socket* GetSocket() 						{ 	return m_pSocket; 			}
    //add end:支持I/O多路复用poll

//////////////////////////////////////////////////
// attributes
//////////////////////////////////////////////////
protected :
	
	// socket implementation object
	Socket* m_pSocket;
};

#endif
