////////////////////////////////////////////////////////////////
//文件名称：ClientSocket.h
//功能描述：前端socket
//文件名称：ClientSocket API
//			
//编写作者：2017.09.05 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
////////////////////////////////////////////////////////////////

#ifndef __CLIENT_SOCKET_H__
#define __CLIENT_SOCKET_H__

//////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////
#include "../Common/BaseType.h"
#include "../Common/Common.h"
#include "Socket.h"


////////////////////////////////////////////////////////////////
//常量
////////////////////////////////////////////////////////////////

class ClientSocket
{
public:
	// constructor
	ClientSocket();

	// destructor
	virtual ~ClientSocket();

//////////////////////////////////////////////////
// methods
//////////////////////////////////////////////////
public:
	// close socket
	VOID Close();
	
	//connect to host:port
	BOOL Connect(const CHAR* host, UINT port);

	// get/set socket's linger status
    UINT GetLinger() 							{ 	return m_pSocket->getLinger(); 				}
    VOID SetLinger(UINT lingertime) 			{ 	m_pSocket->setLinger(lingertime); 			}

    // get/set socket's reuseaddr status
    UINT GetReuseAddr() 						{ 	return m_pSocket->isReuseAddr(); 			}
    VOID SetReuseAddr(BOOL on=TRUE) 			{ 	m_pSocket->setReuseAddr(on); 				}

    // get/set socket's reuseaport status
    UINT GetReusePort() 						{ 	return m_pSocket->isReusePort(); 			}
    VOID SetReusePort(BOOL on=TRUE) 			{ 	m_pSocket->setReusePort(on); 				}

    //禁止nagle算法
	BOOL GetNoDelay() 							{ 	return m_pSocket->isNoDelay(); 				}
	VOID SetNoDelay(BOOL on=TRUE)				{ 	m_pSocket->setNoDelay(on); 					}
 
     // get/set socket's nonblocking status
    BOOL GetNonBlocking() const 				{ 	return m_pSocket->isNonBlocking(); 			}
    VOID SetNonBlocking(BOOL on = TRUE) 		{ 	m_pSocket->setNonBlocking(on); 				}
 
    // get/set receive buffer size
    UINT GetReceiveBufferSize() const 			{ 	return m_pSocket->getReceiveBufferSize();	}
    VOID SetReceiveBufferSize(UINT size) 		{ 	m_pSocket->setReceiveBufferSize(size); 		}
 
    // get/set send buffer size
    UINT GetSendBufferSize() const 				{ 	return m_pSocket->getSendBufferSize(); 		}
    VOID SetSendBufferSize(UINT size) 			{ 	m_pSocket->setSendBufferSize(size); 		}
 
	SOCKET GetSocketID() 						{ 	return m_pSocket->getSocketID(); 			}

//////////////////////////////////////////////////
// attributes
//////////////////////////////////////////////////
protected :
	
	// socket implementation object
	Socket* m_pSocket;

	
};

#endif