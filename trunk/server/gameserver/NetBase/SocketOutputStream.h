////////////////////////////////////////////////////////////////
//文件名称：SocketOutputStream.h
//功能描述：发送缓存
//文件名称：SocketOutputStream API
//			
//编写作者：2017.08.15 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
////////////////////////////////////////////////////////////////

#ifndef __SOCKET_OUTPUT_STREAM_H__
#define __SOCKET_OUTPUT_STREAM_H__

#include "../Common/BaseType.h"
#include "../Common/Common.h"

#include <sys/socket.h>  // fro SOCKADDR_IN、 INVALID_SOCKET

class Socket;
////////////////////////////////////////////////////////////////
//常量
////////////////////////////////////////////////////////////////
//!alter by huyf 2018.03.19:修改读取配置文件
//默认输出缓存大小
//#define DEFAULTSOCKETOUTPUTBUFFERSIZE 1*1024
//最大输出缓存大小
//#define DISCONNECTSOCKETOUTPUTSIZE 2*1024
//!alter end:修改读取配置文件

class CNetMsg;
class SocketOutputStream
{
public:
	// constructor
	SocketOutputStream(Socket* sock, UINT BufferSize /*= DEFAULTSOCKETOUTPUTBUFFERSIZE*/, UINT MaxBufferLen /*= DISCONNECTSOCKETOUTPUTSIZE*/);	
	// destructor
	virtual ~SocketOutputStream();

public:
	VOID		InitSize();
	BOOL 		Resize(UINT size);
	VOID		CleanUp();
	UINT		Write(const CHAR* buf, UINT len);
	//BOOL		WritePacket(const CNetMsg* pMsg);
	INT			Flush();

public:
	UINT		Length() const;
	UINT		Size() const 						{	return 	Length();			}
	BOOL		IsEmpty() const						{	return 	m_Head == m_Tail;	}
	UINT		Capacity() const      				{	return	m_BufferLen;		}
	UINT		GetHead() const 					{	return	m_Head;				}
	UINT		GetTail() const 					{	return	m_Tail;				}
	UINT		GetBufferLen() const 				{	return	m_BufferLen;		}
	UINT		GetMaxBufferLen() const 			{	return	m_MaxBufferLen;		}
	CHAR*		GetBuffer() const 					{	return	m_Buffer;			}
	//CHAR*		GetTail( )const 					{ 	return 	&(m_Buffer[m_Tail]);}

public:
	Socket* 	m_pSocket;
	CHAR* 		m_Buffer;
	UINT 		m_Head;
	UINT 		m_Tail;
	UINT 		m_BufferLen;
	UINT 		m_MaxBufferLen;
};
#endif
