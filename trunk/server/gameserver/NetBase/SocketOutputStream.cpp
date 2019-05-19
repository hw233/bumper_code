////////////////////////////////////////////////////////////////
//文件名称：SocketOutputStream.cpp
//功能描述：接收缓存
//版本说明：SocketOutputStream API
//			
//编写作者：2017.08.15 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
////////////////////////////////////////////////////////////////
#include "../Common/Config.h"
#include "SocketOutputStream.h"
#include "../MsgHandler/NetMsg.h"
#include <string.h>
#include "Socket.h"

////////////////////////////////////////////////////////////////
//常量
////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//
// func description	:	constructor
//
//-----input------
// Parameters
//     	sock 			: 	套接字描述符
//	   	BufferSize		:   buffer长度
//     	MaxBufferLen 	：  buffer最大长度
//------output------
// Return 
//		无
SocketOutputStream::SocketOutputStream(Socket* sock, UINT BufferSize /*= DEFAULTSOCKETOUTPUTBUFFERSIZE*/, UINT MaxBufferLen /*= DISCONNECTSOCKETOUTPUTSIZE*/)
{	
    ASSERT(sock);

    m_pSocket = sock;
    m_BufferLen = BufferSize;
    m_MaxBufferLen = MaxBufferLen;
    m_Head = 0;
    m_Tail = 0;
    m_Buffer = new CHAR[BufferSize];
    memset(m_Buffer, 0, BufferSize);
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
SocketOutputStream::~SocketOutputStream()
{	
    SAFE_DELETE_ARRAY(m_Buffer);
}


//////////////////////////////////////////////////////////////////////
//
// func description	:	获取BUFF的有效数据长度
//
//-----input------
// Parameters
//     	无
//------output------
// Return 
//		BOOL 	:	成功：TRUE 失败：FALSE
UINT SocketOutputStream::Length() const
{
    if (m_Head < m_Tail)
    {
        return m_Tail-m_Head;
    }
    else if (m_Head > m_Tail)
    {
        return m_BufferLen - m_Head + m_Tail;
    }

    return 0;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	InitSize
//
//-----input------
// Parameters
//     	无
//------output------
// Return 
//		无
VOID SocketOutputStream::InitSize()
{
    SAFE_DELETE_ARRAY(m_Buffer);

    m_Head = 0;
    m_Tail = 0;
    m_BufferLen = g_config.ONLINE_DEFAULT_OUTPUT_BUFFER_SIZE;
    m_MaxBufferLen = g_config.ONLINE_DISCONNECT_OUTPUT_BUFFER_SIZE;
    m_Buffer = new CHAR[g_config.ONLINE_DEFAULT_OUTPUT_BUFFER_SIZE];
    memset(m_Buffer, 0, g_config.ONLINE_DEFAULT_OUTPUT_BUFFER_SIZE);
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	Write：将数据写入用户buffer缓冲区。
//
//-----input------
// Parameters
//	   	buf 			:   缓存区指针
//     	len 		 	：  buf长度
//------output------
// Return 
//		UINT 成功: len  失败：0
UINT SocketOutputStream::Write(const CHAR* buf, UINT len)
{
	DEBUG_TRY	
    CHECKF(buf);
    CHECKF(len);

    UINT unBeforeTail = m_Tail;

    //					//
    //     T  H			//    H   T			LEN=10
    // 0123456789		// 0123456789
    // abcd...efg		// ...abcd...
    //					//
    //检查剩余空间
    UINT nFree = ((m_Head <= m_Tail) ? (m_BufferLen - m_Tail + m_Head - 1) : (m_Head - m_Tail - 1));
    if (len >= nFree)//剩余空间不足，重新分配下内存
    {
        if (!Resize(len - nFree + 1))
        {
            LOGERROR("[SocketOutputStream::Write] Resize() write_len=[%u] nFree=[%u] m_Head=[%u] m_Tail=[%u] m_BufferLen=[%u]", len, nFree, m_Head, m_Tail, m_BufferLen);
            return 0;
        }
    }

    //分配好后继续写数据
    if (m_Head <= m_Tail)
    {
        //
        //H   T      LEN=10
        //0123456789
        //abcd......
        //
        if(m_Head == 0)
        {
            nFree = m_BufferLen - m_Tail - 1;
            memcpy(&m_Buffer[m_Tail], buf, len);
        }
        //
        //   H   T      LEN=10
        //0123456789
        //...abcd...
        //
        else //if(m_Head > 0)
        {
            nFree = m_BufferLen - m_Tail;
            if (len <= nFree)
            {
                memcpy(&m_Buffer[m_Tail], buf, len);
            }
            else    //(len > nFree)
            {
                memcpy(&m_Buffer[m_Tail], buf, nFree);
                memcpy(m_Buffer, &buf[nFree], len - nFree);
            }
        }
    }
    else
    {
        //
        //   T  H      LEN=10
        //0123456789
        //efg...abcd
        //
        memcpy(&m_Buffer[m_Tail], buf, len);
    }

    m_Tail = (m_Tail + len) % m_BufferLen;

    //LOGDEBUG("[SocketOutputStream::Write] Write SocketID=[%u] len=[%u] nFree=[%u] m_Head=[%u] m_Tail=[%u ===> %u] m_BufferLen=[%u] m_MaxBufferLen=[%u]", m_pSocket->getSocketID(), len, nFree, m_Head, unBeforeTail, m_Tail, m_BufferLen, m_MaxBufferLen);
    return len;
	DEBUG_CATCH0("SocketInputStream::Write")
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	Flush：将用户发送缓冲区的数据发送到socket缓冲区
//
//-----input------
// Parameters
//	   	无
//------output------
// Return 
//		无
INT SocketOutputStream::Flush()
{
	
	CHECKF(m_pSocket);
	INT nFlushed = 0;
	INT nSent = 0;
	INT nLeft = 0;
    UINT unBeforeHead = m_Head;


    DEBUG_TRY	
	if (m_BufferLen > m_MaxBufferLen)
	{
		InitSize();
		LOGERROR("[SocketOutputStream::Flush] m_BufferLen > m_MaxBufferLen m_BufferLen=[%u] m_MaxBufferLen=[%u]", m_BufferLen, m_MaxBufferLen);
		return -1;
	}

#if defined(__WINDOWS__)
	UINT flag = MSG_DONTROUTE;
#elif defined(__LINUX__)
	UINT flag = MSG_NOSIGNAL;//禁止 send() 函数向系统发送常消息。
#endif

	if (m_Head < m_Tail)
	{
		//					//
        // H   T			//    H   T			LEN=10
        // 0123456789		// 0123456789
        // abcd......		// ...abcd...
        //					//
        nLeft = m_Tail - m_Head;
        while(nLeft > 0)
        {
        	nSent = m_pSocket->send(&m_Buffer[m_Head], nLeft, flag);
        	if(nSent == -1) 
            {
                if (errno == EAGAIN || errno == EINTR)
                {
                    return SOCKET_ERROR_WOULDBLOCK;
                }
        		return -2;
            }
			if(nSent == 0) 
				return 0;

			nFlushed += nSent;
            //m_Head += nSent;
        	m_Head = (m_Head + nSent) % m_BufferLen;      //!alter by huyf 2018.03.09:谨慎做%处理
        	nLeft -= nSent;
        }
	}
	else if (m_Head > m_Tail)
	{
		//					//
        // T   H			//    T   H			LEN=10
        // 0123456789		// 0123456789
        // ....abcdef		// defg...abc
        //
        nLeft = m_BufferLen - m_Head;
        while(nLeft > 0)
        {
        	nSent = m_pSocket->send(&m_Buffer[m_Head], nLeft, flag);
        	if(nSent == -1) 
            {
                if (errno == EAGAIN || errno == EINTR)
                {
                    return SOCKET_ERROR_WOULDBLOCK;
                }
                return -3; 
            }
			if(nSent == 0) 
				return 0;

			nFlushed += nSent;
        	//m_Head += nSent;
            m_Head = (m_Head + nSent) % m_BufferLen;      //!alter by huyf 2018.03.09:谨慎做%处理
        	nLeft -= nSent;	
        }

        //!alter by huyf 2018.03.09:当全部发出去之后再进行前一段的数据发送
        //设置头指针和剩余数据
        m_Head = m_Head % m_BufferLen;
        nLeft += m_Tail;

        //继续发送剩余的数据
        while(nLeft > 0)
    	{
    		nSent = m_pSocket->send(m_Buffer, nLeft, flag);
    		if(nSent == -1) 
            {
                if (errno == EAGAIN || errno == EINTR)
                {
                    return SOCKET_ERROR_WOULDBLOCK;
                }
                return -4; 
            }
			if(nSent == 0) 
				return 0;

			nFlushed += nSent;
			//m_Head += nSent;
            m_Head = (m_Head + nSent) % m_BufferLen;      //!alter by huyf 2018.03.09:谨慎做%处理
			nLeft -= nSent;
    	}
	}
	else//m_Head == m_Tail
	{
		LOGDEBUG("[SocketOutputStream::Flush] m_Head == m_Tail have no data to Flush m_Head=[%u] m_Tail=[%u] m_BufferLen=[%u]", m_Head, m_Tail, m_BufferLen);
	}

	//此时应该是头和尾指针指向相同的位置
	if (m_Head != m_Tail)
	{
        LOGERROR("[SocketOutputStream::Flush] 数据没有全部被发送... m_Head != m_Tail m_Head=[%u ===> %u] m_Tail=[%u] nSent=[%u] nLeft=[%u] m_BufferLen=[%u]", unBeforeHead, m_Head, m_Tail, nSent, nLeft, m_BufferLen);
        return nFlushed;
	}

	DEBUG_CATCH0("SocketOutputStream::Flush")
	//{
	//	if( nSent>0 )
	//	{
	//		m_Head += nSent;
	//	}
    //}

	//if (m_pSocket->m_Port != g_config.MTS_PORT && m_pSocket->m_Port != g_config.ACC_PORT && m_pSocket->m_Port != g_config.PW_PORT)
    {
        /* code */
    
       //LOGDEBUG("[SocketOutputStream::Flush] 数据全部被发送...Send Ip=[%s] Port=[%d] socket_id=[%u] nSent=[%u] m_Head=[%u ===> %u ===> 0] m_Tail=[%u ===> 0] m_BufferLen=[%u] m_MaxBufferLen=[%u]",m_pSocket->m_Host,m_pSocket->m_Port, m_pSocket->getSocketID(), nFlushed, unBeforeHead, m_Head, m_Tail, m_BufferLen, m_MaxBufferLen);
    }
    m_Head = m_Tail = 0;
    return nFlushed;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	CleanUp
//
//-----input------
// Parameters
//     	无
//------output------
// Return 
//		BOOL 	:	成功：TRUE 失败：FALSE
VOID SocketOutputStream::CleanUp()
{	
    m_Head = 0;
    m_Tail = 0;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	Resize
//
//-----input------
// Parameters
//     	size 			: 	增加的buff的长度
//------output------
// Return 
//		BOOL 	:	成功：TRUE 失败：FALSE
BOOL SocketOutputStream::Resize(UINT size)
{	
    //扩充一半的内存空间
    size = max(size, (UINT)(m_BufferLen>>1));
    UINT newBufferLen = m_BufferLen + size;
    UINT len = Length();

    if (size < 0)
    {
        if (newBufferLen < 0 || newBufferLen < len)
        {
            return FALSE;
        }
    }		

    //新申请buffer缓存空间
    CHAR* newBuffer = new CHAR[newBufferLen];
    CHECKF(newBuffer);

    //复制内存数据
    if (m_Head < m_Tail)
    {
        memcpy(newBuffer, &m_Buffer[m_Head], m_Tail - m_Head);
    }
    else if (m_Head > m_Tail)
    {
        memcpy(newBuffer, &m_Buffer[m_Head], m_BufferLen - m_Head);	
        memcpy(&newBuffer[m_BufferLen - m_Head], m_Buffer, m_Tail);
    }
    else//m_Head == m_Tail:无数据
    {
        //...
    }

    m_Head = 0;
    m_Tail = len;
    m_BufferLen = newBufferLen;

    //释放原来buffer缓存
    SAFE_DELETE_ARRAY(m_Buffer);

    //将缓冲区指针重定向为新申请的内存空间
    m_Buffer = newBuffer;

    return TRUE;
}
