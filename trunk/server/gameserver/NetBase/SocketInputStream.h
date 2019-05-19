////////////////////////////////////////////////////////////////
//文件名称：SocketInputStream.h
//功能描述：接收缓存
//版本说明：SocketInputStream API
//			
//编写作者：2017.08.14 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
////////////////////////////////////////////////////////////////

#ifndef __SOCKET_INPUT_STREAM_H__
#define __SOCKET_INPUT_STREAM_H__
#include <sys/socket.h>  // fro SOCKADDR_IN、 INVALID_SOCKET
#include <sys/ioctl.h>

#include "../Common/BaseType.h"
#include "../Common/Common.h"



class Socket;
////////////////////////////////////////////////////////////////
//常量
////////////////////////////////////////////////////////////////
//!alter by huyf 2018.03.19:修改读取配置文件
//默认接收缓存大小
//#define DEFAULTSOCKETINPUTBUFFERSIZE 1*1024
//最大接收缓存大小
//#define DISCONNECTSOCKETINPUTSIZE 2*1024
//!alter end:修改读取配置文件

class CNetMsg;
class SocketInputStream
{
public:
	// constructor
	SocketInputStream(Socket* sock, UINT BufferSize /*= DEFAULTSOCKETINPUTBUFFERSIZE*/, UINT MaxBufferLen /*= DISCONNECTSOCKETINPUTSIZE*/);	
	// destructor
	virtual ~SocketInputStream();

public:
	VOID		InitSize();
	BOOL 		Resize(UINT size);
	
	INT			Read(CHAR* buf, UINT len);
	BOOL 		ReadPacket(CNetMsg* pMsg, UINT nLengh); 
	BOOL 		Peek(CHAR* buf, UINT len);
	BOOL 		Find(CHAR* buf);
	INT 		Fill();
	BOOL 		Skip(UINT size);
	VOID 		CleanUp();	

	UINT		Length() const;
	UINT		Size() const 						{	return 	Length();			}
	BOOL		IsEmpty() const						{	return 	m_Head == m_Tail;	}
	UINT		Capacity() const      				{	return	m_BufferLen;		}
	UINT		GetHead() const 					{	return	m_Head;				}
	UINT		GetTail() const 					{	return	m_Tail;				}
	UINT		GetBufferLen() const 				{	return	m_BufferLen;		}
	UINT		GetMaxBufferLen() const 			{	return	m_MaxBufferLen;		}
	CHAR*		GetBuffer() const 					{	return	m_Buffer;			}

public:
	Socket* 	m_pSocket;
	CHAR* 		m_Buffer;
	UINT 		m_Head;
	UINT 		m_Tail;
	UINT 		m_BufferLen;
	UINT 		m_MaxBufferLen;

};
#endif
