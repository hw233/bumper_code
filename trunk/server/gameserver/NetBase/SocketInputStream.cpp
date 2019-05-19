////////////////////////////////////////////////////////////////
//文件名称：SocketInputStream.cpp
//功能描述：socket接收缓存
//版本说明：SocketInputStream API
//			
//编写作者：2017.08.14 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
////////////////////////////////////////////////////////////////
#include "../MsgHandler/NetMsg.h"
#include "../Common/Config.h"
#include "SocketInputStream.h"
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
SocketInputStream::SocketInputStream(Socket* sock, UINT BufferSize /*= DEFAULTSOCKETINPUTBUFFERSIZE*/, UINT MaxBufferLen /*= DISCONNECTSOCKETINPUTSIZE*/)
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
SocketInputStream::~SocketInputStream()
{	
    SAFE_DELETE_ARRAY(m_Buffer);
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
VOID SocketInputStream::InitSize()
{	
    SAFE_DELETE_ARRAY(m_Buffer);

    m_Head = 0;
    m_Tail = 0;
    m_BufferLen = g_config.ONLINE_DEFAULT_INPUT_BUFFER_SIZE;
    m_MaxBufferLen = g_config.ONLINE_DISCONNECT_INPUT_BUFFER_SIZE;
    m_Buffer = new CHAR[g_config.ONLINE_DEFAULT_INPUT_BUFFER_SIZE];
    memset(m_Buffer, 0, g_config.ONLINE_DEFAULT_INPUT_BUFFER_SIZE);
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
UINT SocketInputStream::Length() const
{	
    //LOGERROR("[SocketInputStream::Length()] m_Head=[%d] m_Tail=[%d]\n", m_Tail, m_Head);
    if (m_Head < m_Tail)
    {
        return m_Tail - m_Head;
    }
    else if (m_Head > m_Tail)
    {
        return m_BufferLen - m_Head + m_Tail;
    }

    return 0;
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
VOID SocketInputStream::CleanUp()
{	
    m_Head = 0;
    m_Tail = 0;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	从socket的缓存中读取数据
//
//-----input------
// Parameters
//     	buf 			:	用户输入缓存（recv buffer）
//		len 			:   请求读取的字节数（recv data length）
//------output------
// Return 		
//		INT 返回值：成功返回读取的字节数，出错返回-1并设置errno，无数据可以读取，则返回0

INT SocketInputStream::Read(CHAR* buf, UINT len)
{
    //LOGDEBUG("[SocketInputStream::Read] read_len=[%u] m_BufferLen=[%u]", len, Length());
	DEBUG_TRY
    CHECKF(buf);
    CHECKF(len);

    //异常判断
    if (len == 0)
    {
        LOGERROR("[SocketInputStream::Read] len=[0]");
        return 0;
    }

    //所需数据长度大于缓存区中的数据长度
    if (len > Length())
    {
        LOGDEBUG("[SocketInputStream::Read] wait recive data...read_len=[%u] m_BufferLen=[%u]", len, Length());
        return 0;
    }

    //有数据，读取数据
    if (m_Head < m_Tail)//尾指针在前，头指针在后
    {
        //
        // H   T		LEN=10         				H   T		LEN=10
        // 0123456789   							0123456789
        // abcd......								...abcd...
        // 	

        memcpy(buf, &m_Buffer[m_Head], len);
    }
    else //if (m_Head > m_Tail)//尾指针在后，头指针在前
    {
        //
        //     T  H		LEN=10
        // 0123456789
        // abcd...efg
        //

        UINT rightlen = m_BufferLen - m_Head;//剩余的后段数据长度
        if (len <= rightlen)//读取数据长度小于后段数据长度直接读取
        {
            memcpy(buf, &m_Buffer[m_Head], len);
        }
        else //if (len > rightlen)//读取数据长度大于后段数据长度直接读取，则需要从前段继续读取
        {
            memcpy(buf, &m_Buffer[m_Head], rightlen);	
            memcpy(&buf[rightlen], m_Buffer, len - rightlen);
        }
    }  
    /*      
    else //m_Head > m_Tail：尾指针在后，头指针在前,此处不会进来
    {
            //...
            LOGDEBUG("[SocketInputStream::Read] have not data to read... m_Head=[%u] m_Tail=[%u]", m_Head, m_Tail);
            return 0;
    }
    */        

    //更新头部指针
    //UINT unBeforeHead = m_Head;

    m_Head = (m_Head + len) % m_BufferLen;

    //LOGDEBUG("[SocketInputStream::Read] 数据被读取... Ip=[%s] Port=[%u] SocketID==[%d] m_Head=[%u ===> %u] m_Tail=[%u] m_BufferLen=[%u] BufferSize=[%u] ReadLen=[%u]", 
    //    m_pSocket->m_Host,m_pSocket->m_Port, m_pSocket->getSocketID(), unBeforeHead, m_Head, m_Tail, m_BufferLen, Length(), len);    
    
    //返回读取的数据长度
    return (INT)len;
	DEBUG_CATCH0("SocketInputStream::Read")
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	ReadPacket
//
//-----input------
// Parameters
//     	pPacket 			:	
//------output------
// Return 		
//		BOOL 返回值：成功返回 TRUE 失败：FALSE
BOOL SocketInputStream::ReadPacket(CNetMsg* pMsg, UINT nLengh) 
{
	DEBUG_TRY
    CHECKF(pMsg);
    return pMsg->Read(*this, nLengh);		
	DEBUG_CATCHF("SocketInputStream::ReadPacket")
}
//////////////////////////////////////////////////////////////////////
//
// func description	:	从用户缓冲区读取数据，组装相关包
//
//-----input------
// Parameters
//     	buf 			:	用户输入缓存（recv buffer）
//		len 			:   请求读取的字节数（recv data length）
//------output------
// Return 		
//		BOOL 返回值：成功返回 TRUE 失败：FALSE

BOOL SocketInputStream::Peek(CHAR* buf, UINT len)
{
	DEBUG_TRY
    CHECKF(buf);
    CHECKF(len);
    //异常判断
    if (len == 0)
    {
        LOGERROR("[SocketInputStream::Peek] len = 0");
        return FALSE;
    }
    if (len > Length())
    {
        //LOGDEBUG("[SocketInputStream::Peek] 缓存中的数据长度不够... Ip=[%s] Port=[%u] SocketID==[%d] PeekLen=[%u] Length=[%d] m_Head=[%u] m_Tail=[%u] m_BufferLen=[%u]",
        //    m_pSocket->m_Host, m_pSocket->m_Port, m_pSocket->getSocketID(), len, Length(), m_Head, m_Tail, m_BufferLen);
        return FALSE;
    }
    //LOGDEBUG("[SocketInputStream::Peek] sockt=[%d] peek_len=[%u] Length=[%d] m_BufferLen=[%u] m_Head=[%u] m_Tail=[%u]", m_pSocket->getSocketID(), len, m_BufferLen, Length(), m_Head, m_Tail);

    //读取数据
    if (m_Head < m_Tail)//头指针在前，尾指针在后
    {
        memcpy(buf, &m_Buffer[m_Head], len);
    }
    else //if (m_Head >= m_Tail)//头指针在后，尾指针在前
    {
        UINT rightlen = m_BufferLen - m_Head;//剩余的后段数据长度
        if (len <= rightlen)//读取数据长度小于后段数据长度直接读取
        {
            memcpy(buf, &m_Buffer[m_Head], len);
        }
        else //if (len > rightlen)//读取数据长度大于后段数据长度直接读取，则需要从前段继续读取
        {
            memcpy(buf, &m_Buffer[m_Head], rightlen);	
            memcpy(&buf[rightlen], m_Buffer, len - rightlen);
        }
    }
    /*
    else //m_Head == m_Tail : 无数据可以读取
    {
        //...
        LOGDEBUG("[SocketInputStream::Peek] have no data to peek... sockt=[%d] peek_len=[%u] m_BufferLen=[%u] m_Head=[%u] m_Tail=[%u]", m_pSocket->getSocketID(), len, Length(), m_Head, m_Tail);
        return FALSE;
    }
    */

    //返回TRUE
   // LOGDEBUG("[SocketInputStream::Peek] peek_len=[%u] m_BufferLen=[%u] m_Head=[%u] m_Tail=[%u] m_Buffer=[%s]");
    return TRUE;
	DEBUG_CATCHF("SocketInputStream::Peek")

}

//////////////////////////////////////////////////////////////////////
//
// func description	:	Find
//
//-----input------
// Parameters
//     	buf 			:	用户输入缓存（recv buffer）
//------output------
// Return 		
//		BOOL 返回值：成功返回 TRUE 失败：FALSE

BOOL SocketInputStream::Find(CHAR* buf)
{
	DEBUG_TRY
    CHECKF(buf);
    BOOL isFind = FALSE;

    while(Length() > PACK_COMPART_SIZE)
    {
        //读取数据
        if (m_Head < m_Tail)//头指针在前，尾指针在后
        {
            memcpy(buf, &m_Buffer[m_Head], PACK_COMPART_SIZE);
        }
        else //if (m_Head > m_Tail)//头指针在后，尾指针在前
        {
            UINT rightlen = m_BufferLen - m_Head;//剩余的后段数据长度
            if (PACK_COMPART_SIZE <= rightlen)//读取数据长度小于后段数据长度直接读取
            {
                memcpy(buf, &m_Buffer[m_Head], PACK_COMPART_SIZE);
            }
            else //if (PACK_COMPART_SIZE > rightlen)//读取数据长度大于后段数据长度直接读取，则需要从前段继续读取
            {
                memcpy(buf, &m_Buffer[m_Head], rightlen);	
                memcpy(&buf[rightlen], m_Buffer, PACK_COMPART_SIZE - rightlen);
            }

        }
        /*
        else //m_Head == m_Tail : 无数据可以读取
        {
            //...
            return FALSE;
    }
        */

        //检查数据
        if (strstr(buf, PACK_COMPART) == NULL)
        {
            m_Head++;
            if(m_Head >= m_BufferLen)
            {
                m_Head -= m_BufferLen;
            }
        }
        else
        {
            isFind = TRUE;
            break;
        }
    }

    //没有查到到
    if (!isFind)
    {
        return FALSE;
    }

    m_Head += (UINT)(PACK_COMPART_SIZE);
    if(m_Head >= m_BufferLen)
    {
         m_Head -= m_BufferLen;
    }

    //返回true
    return TRUE;
	DEBUG_CATCHF("SocketInputStream::Find")
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	Skip
//
//-----input------
// Parameters
//		len 			:   请求读取的字节数（recv data length）
//------output------
// Return 		
//		BOOL 返回值：成功返回 TRUE 失败：FALSE

BOOL SocketInputStream::Skip(UINT len)
{
    //异常判断
    if (len == 0)
    {
        LOGERROR("[SocketInputStream::Skip] len = 0");
        return FALSE;
    }
    if (len > Length())
    {
        LOGDEBUG("[SocketInputStream::Skip] len > Length()");
        return FALSE;
    }
    m_Head = (m_Head + len) % m_BufferLen;
    return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	Fill:从socket缓存区中receive数据到用户buffer
//
//-----input------
// Parameters
//     	无
//------output------
// Return 		
//		INT 返回值：成功返回 装载的字节数 失败：错误码

INT SocketInputStream::Fill()
{
	DEBUG_TRY
    CHECKF(m_pSocket);

    INT nFilled = 0;
    INT nReceived  = 0;
    INT nFree = 0;

    //add by huyf 2018.03.09:添加日志
    UINT unBeforeTail = m_Tail;
    UINT unHeadFree = 0;//注意此处-1：为了头尾部重叠 
    UINT unTailFree = 0; 
    UINT unTotalFree = 0;
    //add end:添加日志

    //!alter by huyf 2018.04.18:当没有数据了，异常断开的时候，如果直接返回SOCKET_ERROR_WOULDBLOCK则不能立即断开客户端链接，必须等待超时的到来。
    //unsigned long bytes = 0;  
    //ioctl(m_pSocket->getSocketID(), FIONREAD, &bytes); // 探测内核缓冲区中有多少数据可以接收  
    //LOGDEBUG("[SocketInputStream::Fill] 探测TCP缓冲区中有多少数据可以接收 Ip=[%s] Port=[%d] bytes=[%d]", m_pSocket->m_Host, m_pSocket->m_Port, bytes);
    
    //add by huyf 2018.03.15:没有数据直接返回
    //if (bytes == 0)
    //{
        //return SOCKET_ERROR_WOULDBLOCK;
    //}
    //add end:没有数据直接返回
    //!alter end:当没有数据了，异常断开的时候，如果直接返回SOCKET_ERROR_WOULDBLOCK则不能立即断开客户端链接，必须等待超时的到来。

    if (m_Head <= m_Tail)
    {
        //
        // H   T		LEN=10
        // 0123456789
        // abcd......
        //
        if (m_Head == 0)
        {		
            nReceived = 0;
            nFree = m_BufferLen - m_Tail - 1;//注意此处-1：为了头尾部重叠
            if (nFree != 0)
            {
                nReceived = m_pSocket->receive(&m_Buffer[m_Tail], nFree);
                //不算出错，可以继续执行	
                if (nReceived == -1)
                {
                    
                    if (errno == EAGAIN || errno == EINTR)
                    {
                        return SOCKET_ERROR_WOULDBLOCK;
                    }
                     
                    return -1;                                                              
                }			
                if (nReceived == 0)//主动关闭
                        return 0;                             

                //更新buffer尾指针、读取字节数
                m_Tail = (m_Tail + nReceived) % m_BufferLen;
                nFilled += nReceived;

                //LOGDEBUG("[SocketInputStream::Fill] 头指针==0 nFree=[%d] nReceived=[%d] m_Tail=[%u ===> %u]", nFree, nReceived, unBeforeTail, m_Tail);

                //add by huyf 2018.03.09:添加日志
                unHeadFree = 0;
                unTailFree = nFree - nReceived;
                unTotalFree = unHeadFree + unTailFree;
                //LOGDEBUG("[SocketInputStream::Fill] m_Head <= m_Tail 头指针==0 接受数据填充至===>尾部 Received Ip=[%s] Port=[%d] socket_id=[%u] m_Head=[%u] m_Tail=[%u ===> %u] unHeadFree=[%u] unTailFree=[%u] unTotalFree=[%u ===> %u] nReceived=[%u] m_BufferLen=[%u] m_MaxBufferLen=[%u]",
                //    m_pSocket->m_Host,m_pSocket->m_Port, m_pSocket->getSocketID(), m_Head, unBeforeTail, m_Tail, unHeadFree, unTailFree, nFree, unTotalFree, nReceived, m_BufferLen, m_MaxBufferLen);
                unBeforeTail = m_Tail;
                //add end:添加日志
            }                        

            //正确读取数据
            if (nReceived == nFree)
            {
                //还有未读取的数据
                UINT available = m_pSocket->available();
                if (available > 0)
                {
                    if ((m_BufferLen + available + 1) > m_MaxBufferLen)
                    {
                        LOGERROR("[SocketInputStream::Fill]-1 InitSize() m_BufferLen=[%u] available=[%u] m_MaxBufferLen=[%u] m_Head=[%u m_Tail=[%u]", m_BufferLen, available, m_MaxBufferLen, m_Head, m_Tail);
                        InitSize();                        
                        return -2;
                    }

                    //m_BufferLen + available + 1 <= m_MaxBufferLen)
                    if (!Resize(available + 1))
                    {
                        LOGERROR("[SocketInputStream::Fill]-1 Resize() m_BufferLen=[%u] available=[%u] m_MaxBufferLen=[%u] m_Head=[%u m_Tail=[%u]", m_BufferLen, available, m_MaxBufferLen, m_Head, m_Tail);
                        return -3;
                    }

                    //继续读取available数据
                    nReceived = m_pSocket->receive(&m_Buffer[m_Tail], available);
                    //不算出错，可以继续执行   
                    if (nReceived == -1)
                    {
                        if (errno == EAGAIN || errno == EINTR)
                        {
                            return SOCKET_ERROR_WOULDBLOCK;
                        }
                        //LOGDEBUG("[SocketInputStream::Fill] 头指针>0 available=[%d] nReceived=[%d] nReceived == SOCKET_ERROR_WOULDBLOCK", available, nReceived, SOCKET_ERROR_WOULDBLOCK);   
                        return -4;                                                              
                    }                                                
                    if (nReceived == 0)
                            return 0;

                    //更新buffer尾指针、读取字节数
                    m_Tail = (m_Tail + available) % m_BufferLen;
                    nFilled += available;

                    //add by huyf 2018.03.09:添加日志
                    //LOGDEBUG("[SocketInputStream::Fill] 头指针==0 && 经过扩容后再次接受数据 Received Ip=[%s] Port=[%d] socket_id=[%u] m_Head=[%u] m_Tail=[%u ===> %u] m_BufferLen=[%u] m_MaxBufferLen=[%u] nFree=[%u] available=[%u] nReceived=[%u]",m_pSocket->m_Host,m_pSocket->m_Port, m_pSocket->getSocketID(), m_Head, unBeforeTail, m_Tail, m_BufferLen, m_MaxBufferLen,nFree,available,nReceived);
                    unBeforeTail = m_Tail;
                    //add end:添加日志
                }
            }
        }
        //
        //    H   T		LEN=10
        // 0123456789
        // ...abcd...
        //
        else //if (m_Head > 0)
        {
            nReceived = 0;
            nFree = m_BufferLen - m_Tail;//注意此处：为了尾部可以全部装在数据
            nReceived = m_pSocket->receive(&m_Buffer[m_Tail], nFree);
            //不算出错，可以继续执行   
            if (nReceived == -1)
            {
                if (errno == EAGAIN || errno == EINTR)
                {
                    //LOGDEBUG("[SocketInputStream::Fill] 头指针>0 Free=[%d] nReceived=[%d] nReceived == SOCKET_ERROR_WOULDBLOCK", nFree, nReceived, SOCKET_ERROR_WOULDBLOCK);
                    return SOCKET_ERROR_WOULDBLOCK;
                }
                //LOGDEBUG("[SocketInputStream::Fill] 头指针>0 available=[%d] nReceived=[%d] nReceived == SOCKET_ERROR_WOULDBLOCK", available, nReceived, SOCKET_ERROR_WOULDBLOCK);   
                return -5;                                                            
            }                                
            if (nReceived == 0)
                    return 0;                  

            //更新buffer尾指针、读取字节数
            m_Tail = (m_Tail + nReceived) % m_BufferLen;
            nFilled += nReceived;

            //add by huyf 2018.03.09:添加日志
            unHeadFree = m_Head - 1;//注意此处-1：为了头尾部重叠
            unTailFree = nFree - nReceived;  
            unTotalFree = unHeadFree + unTailFree;
            //LOGDEBUG("[SocketInputStream::Fill] m_Head <= m_Tail 头指针>0 接受数据填充至===>尾部 Received Ip=[%s] Port=[%d] socket_id=[%u] m_Head=[%u] m_Tail=[%u ===> %u] unHeadFree=[%u] unTailFree=[%u] unTotalFree=[%u ===> %u]",
            //           m_pSocket->m_Host,m_pSocket->m_Port, m_pSocket->getSocketID(), m_Head, unBeforeTail, m_Tail, unHeadFree, unTailFree, nFree+m_Head-1, unTotalFree);
            
            unBeforeTail = m_Tail;
            //add end:添加日志

            //正确读取数据
            if (nReceived == nFree)
            {
                nReceived = 0;
                nFree = m_Head - 1;//注意此处-1：为了头尾不重叠
                if (nFree > 0)
                {
                    nReceived = m_pSocket->receive(&m_Buffer[0], nFree);
                    //不算出错，可以继续执行   
                    if (nReceived == -1)
                    {
                        if (errno == EAGAIN || errno == EINTR)
                        {
                            //LOGDEBUG("[SocketInputStream::Fill] 头指针>0 尾部全部填满数据 Free=[%d] nReceived=[%d] nReceived == SOCKET_ERROR_WOULDBLOCK", nFree, nReceived, SOCKET_ERROR_WOULDBLOCK); 
                            return SOCKET_ERROR_WOULDBLOCK;
                        }
                        //LOGDEBUG("[SocketInputStream::Fill] 头指针>0 available=[%d] nReceived=[%d] nReceived == SOCKET_ERROR_WOULDBLOCK", available, nReceived, SOCKET_ERROR_WOULDBLOCK);   
                        return -6;                                                          
                    }    
                    if (nReceived == 0)
                            return 0;                                      

                    //更新buffer尾指针、读取字节数
                    m_Tail = (m_Tail + nReceived) % m_BufferLen;
                    nFilled += nReceived;

                    //add by huyf 2018.03.09:添加日志
                    unHeadFree = nFree - nReceived;//注意此处-1：为了头尾部重叠
                    unTailFree = 0;  
                    unTotalFree = unHeadFree + unTailFree;
                    //LOGDEBUG("[SocketInputStream::Fill] m_Head <= m_Tail 头指针>0 接受数据尾部空间已被填满，剩余数据填充至===>头部 Received Ip=[%s] Port=[%d] socket_id=[%u] m_Head=[%u] m_Tail=[%u ===> %u] unHeadFree=[%u ===> %u] unTailFree=[%u] unTotalFree=[%u ===> %u]",
                    //            m_pSocket->m_Host,m_pSocket->m_Port, m_pSocket->getSocketID(), m_Head, unBeforeTail, m_Tail, nFree, unHeadFree, unTailFree, nFree, unTotalFree);
                    unBeforeTail = m_Tail;
                    //add end:添加日志

                }
                if (nReceived == nFree)
                {

                    //还有未读取的数据
                    UINT available = m_pSocket->available();
                    if (available > 0)
                    {
                        if ((m_BufferLen + available + 1) > m_MaxBufferLen)
                        {
                            LOGERROR("[SocketInputStream::Fill]-2 InitSize() m_BufferLen=[%u] available=[%u] m_MaxBufferLen=[%u] m_Head=[%u] m_Tail=[%u]", m_BufferLen, available, m_MaxBufferLen, m_Head, m_Tail);
                            InitSize();   
                            return -7;
                        }

                        //m_BufferLen + available + 1 <= m_MaxBufferLen)
                        if (!Resize(available + 1))
                        {
                            LOGERROR("[SocketInputStream::Fill]-2 Resize() m_BufferLen=[%u] available=[%u] m_MaxBufferLen=[%u] m_Head=[%u] m_Tail=[%u]", m_BufferLen, available, m_MaxBufferLen, m_Head, m_Tail);
                            return -8;
                        }

                        //继续读取available数据
                        nReceived = m_pSocket->receive(&m_Buffer[m_Tail], available);
                        //不算出错，可以继续执行   
                        if (nReceived == -1)
                        {
                            if (errno == EAGAIN || errno == EINTR)
                            {
                                //LOGDEBUG("[SocketInputStream::Fill] 头指针>0 经过扩容后再次接受数据 available=[%d] nReceived=[%d] nReceived == SOCKET_ERROR_WOULDBLOCK", available, nReceived, SOCKET_ERROR_WOULDBLOCK); 
                                return SOCKET_ERROR_WOULDBLOCK;
                            }
                            //LOGDEBUG("[SocketInputStream::Fill] 头指针>0 available=[%d] nReceived=[%d] nReceived == SOCKET_ERROR_WOULDBLOCK", available, nReceived, SOCKET_ERROR_WOULDBLOCK);   
                            return -9;                                                          
                        } 
                        if (nReceived == 0)
                                return 0;

                        //更新buffer尾指针、读取字节数
                        m_Tail = (m_Tail + available) % m_BufferLen;
                        nFilled += available;

                        //add by huyf 2018.03.09:添加日志
                        //LOGDEBUG("[SocketInputStream::Fill] 头指针>0 && 经过扩容后再次接受数据 Received Ip=[%s] Port=[%d] socket_id=[%u] m_Head=[%u] m_Tail=[%u ===> %u] m_BufferLen=[%u] m_MaxBufferLen=[%u] nFree=[%u] available=[%u] nReceived=[%u]",m_pSocket->m_Host,m_pSocket->m_Port, m_pSocket->getSocketID(), m_Head, unBeforeTail, m_Tail, m_BufferLen, m_MaxBufferLen,nFree,available,nReceived);
                        unBeforeTail = m_Tail;
                        //add end:添加日志
                    }
                }
            }
        }
    }
    else //if (m_Head > m_Tail)
    {
        //
        //     T  H		LEN=10
        // 0123456789
        // abcd...efg
        //
        nReceived = 0;
        nFree = m_Head - m_Tail - 1;//注意此处-1：为了头尾部重叠
        if (nFree > 0)
        {
            nReceived = m_pSocket->receive(&m_Buffer[m_Tail], nFree);
            //不算出错，可以继续执行   
            if (nReceived == -1)
            {
                if (errno == EAGAIN || errno == EINTR)
                {
                    //LOGDEBUG("[SocketInputStream::Fill] 头指针>尾指 Free=[%d] nReceived=[%d] nReceived == SOCKET_ERROR_WOULDBLOCK", nFree, nReceived, SOCKET_ERROR_WOULDBLOCK);
                    return SOCKET_ERROR_WOULDBLOCK;
                }
                //LOGDEBUG("[SocketInputStream::Fill] 头指针>0 available=[%d] nReceived=[%d] nReceived == SOCKET_ERROR_WOULDBLOCK", available, nReceived, SOCKET_ERROR_WOULDBLOCK);   
                return -10;                                                          
            } 
            if (nReceived == 0)
                    return 0;                         

            //更新buffer尾指针、读取字节数
            m_Tail = (m_Tail + nReceived) % m_BufferLen;
            nFilled += nReceived;

            //add by huyf 2018.03.09:添加日志
            unHeadFree = 0;
            unTailFree = nFree - nReceived;
            unTotalFree = unHeadFree + unTailFree;
            //LOGDEBUG("[SocketInputStream::Fill] 头指针>尾指针 接受数据尾部空间已被填满，剩余数据填充至===>头部 Received Ip=[%s] Port=[%d] socket_id=[%u] m_Head=[%u] m_Tail=[%u ===> %u] unHeadFree=[%u] unTailFree=[%u ===> %u] unTotalFree=[%u ===> %u]",
            //             m_pSocket->m_Host,m_pSocket->m_Port, m_pSocket->getSocketID(), m_Head, unBeforeTail, m_Tail, unHeadFree, nFree, unTailFree, nFree, unTotalFree);
            unBeforeTail = m_Tail;
            //add end:添加日志
        }
        if (nReceived == nFree)
        {
            //还有未读取的数据
            UINT available = m_pSocket->available();
            if (available > 0)
            {
                if (m_BufferLen + available + 1 > m_MaxBufferLen) 
                {
                    LOGERROR("[SocketInputStream::Fill]-3 InitSize() m_BufferLen=[%u] available=[%u] m_MaxBufferLen=[%u] m_Head=[%u] m_Tail=[%u]", m_BufferLen, available, m_MaxBufferLen, m_Head, m_Tail);
                    InitSize();                    
                    return -11;
                }
                if(!Resize(available + 1))
                {
                    LOGERROR("[SocketInputStream::Fill]-3 Resize() m_BufferLen=[%u] available=[%u] m_MaxBufferLen=[%u] m_Head=[%u] m_Tail=[%u]", m_BufferLen, available, m_MaxBufferLen, m_Head, m_Tail);
                    return -12;
                }

                nReceived = m_pSocket->receive(&m_Buffer[m_Tail], available);
                //不算出错，可以继续执行   
                if (nReceived == -1)
                {
                    if (errno == EAGAIN || errno == EINTR)
                    {
                        //LOGDEBUG("[SocketInputStream::Fill] 头指针>尾指 available=[%d] nReceived=[%d] nReceived == SOCKET_ERROR_WOULDBLOCK", available, nReceived, SOCKET_ERROR_WOULDBLOCK);
                        return SOCKET_ERROR_WOULDBLOCK;
                    }
                    //LOGDEBUG("[SocketInputStream::Fill] 头指针>0 available=[%d] nReceived=[%d] nReceived == SOCKET_ERROR_WOULDBLOCK", available, nReceived, SOCKET_ERROR_WOULDBLOCK);   
                    return -13;                                                          
                } 
                if (nReceived == 0)
                        return 0;

                //更新buffer尾指针、读取字节数
                m_Tail = (m_Tail + nReceived) % m_BufferLen;
                nFilled += nReceived;

                //add by huyf 2018.03.09:添加日志
                //LOGDEBUG("[SocketInputStream::Fill] 头指针>尾指针 && 经过扩容后再次接受数据 Received Ip=[%s] Port=[%d] socket_id=[%u] m_Head=[%u] m_Tail=[%u ===> %u] m_BufferLen=[%u] m_MaxBufferLen=[%u] nFree=[%u] available=[%u] nReceived=[%u]",m_pSocket->m_Host,m_pSocket->m_Port, m_pSocket->getSocketID(), m_Head, unBeforeTail, m_Tail, m_BufferLen, m_MaxBufferLen,nFree,available,nReceived);
                unBeforeTail = m_Tail;
                //add end:添加日志
            }
        }
        //LOGDEBUG("nReceived:%d\n", nReceived);
    }
    if (m_pSocket->m_Port != (UINT)g_config.MTS_PORT && m_pSocket->m_Port != (UINT)g_config.ACC_PORT && m_pSocket->m_Port != (UINT)g_config.PW_PORT)
    {
        /* code */    
        //LOGDEBUG("[SocketInputStream::Fill] Received Ip=[%s] Port=[%d] socket_id=[%u] nReceived=[%u] m_Head=[%u] m_Tail=[%u] m_BufferLen=[%u] m_MaxBufferLen=[%u]",
        //    m_pSocket->m_Host,m_pSocket->m_Port, m_pSocket->getSocketID(), nFilled, m_Head, m_Tail, m_BufferLen, m_MaxBufferLen);
    }
    //返回长度
    return nFilled;
	DEBUG_CATCH0("SocketInputStream::Fill")

}

//////////////////////////////////////////////////////////////////////
//
// func description :   Resize
//
//-----input------
// Parameters
//      size            :   增加的buff的长度
//------output------
// Return 
//      BOOL    :   成功：TRUE 失败：FALSE
BOOL SocketInputStream::Resize(UINT size)
{
    DEBUG_TRY   
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
        //
        // H   T        LEN=10      //    H   T        LEN=10
        // 0123456789               // 0123456789
        // abcd......               // ...abcd...
        //                          //
        memcpy(newBuffer, &m_Buffer[m_Head], m_Tail - m_Head);
    }
    else if (m_Head > m_Tail)
    {
        //
        //     T  H     LEN=10
        // 0123456789
        // abcd...efg
        //
        memcpy(newBuffer, &m_Buffer[m_Head], m_BufferLen - m_Head); 
        memcpy(&newBuffer[m_BufferLen - m_Head], m_Buffer, m_Tail);
    }
    else//m_Head == m_Tail:不存在==的情况，因为接收数据的时候m_Head 不会有==m_Tail的情况，进行了-1限制
    {
        //...
        LOGERROR("[SocketInputStream::Resize] Is Error sockt=[%d] len=[%u] newBufferLen=[%d] m_BufferLen=[%u] m_Head=[%u] m_Tail=[%u] m_BufferLen=[%d]", m_pSocket->getSocketID(), len, newBufferLen, m_Head, m_Tail ,m_BufferLen);
    }
    
    
    //LOGDEBUG("[SocketInputStream::Resize] sockt=[%d] len=[%u] newBufferLen=[%d] m_BufferLen=[%u] m_Head=[%u] m_Tail=[%u] m_BufferLen=[%d]", m_pSocket->getSocketID(), len, newBufferLen, m_Head, m_Tail ,m_BufferLen);
    m_Head = 0;
    m_Tail = len;
    m_BufferLen = newBufferLen;

    //释放原来buffer缓存
    SAFE_DELETE_ARRAY(m_Buffer);

    //将缓冲区指针重定向为新申请的内存空间
    m_Buffer = newBuffer;

    return TRUE;
    DEBUG_CATCHF("SocketInputStream::Resize")
}



