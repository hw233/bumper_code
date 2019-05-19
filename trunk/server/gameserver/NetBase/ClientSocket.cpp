////////////////////////////////////////////////////////////////
//文件名称：ClientSocket.cpp
//功能描述：服务器端socket
//文件名称：ClientSocket API
//			
//编写作者：2017.09.05 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
// include files
////////////////////////////////////////////////////////////////
#include "ClientSocket.h"

////////////////////////////////////////////////////////////////
//常量
////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//
// func description	:	constructor
//
//-----input------
// Parameters
//	   	port 			: 	端口
//	   	backlog			:	规定了内核应该为相应套接字排队的最大连接个数
//------output------
// Return 
//     出错抛出异常
//
ClientSocket::ClientSocket()
{
	DEBUG_TRY
        BOOL ret = FALSE;

        // create socket implementation object
        m_pSocket = new Socket();
        CHECK(m_pSocket);

        // create socket
        ret = m_pSocket->create();
        if (!ret)
        {
                LOGERROR("[ClientSocket] new is Error");
        }

        // set buff size		
        //...
	DEBUG_CATCH("ClientSocket::ClientSocket")
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
ClientSocket::~ClientSocket()
{	
        if (m_pSocket != NULL) 
        {
                //m_pSocket->close();
                SAFE_DELETE(m_pSocket);
        }
}
//////////////////////////////////////////////////////////////////////
//
// func description	:	Close
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
VOID ClientSocket::Close()
{	
	DEBUG_TRY
        if (m_pSocket != NULL) 
        {
                m_pSocket->close();
        }
	DEBUG_CATCH("ClientSocket::Close")
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	Connect
//
//-----input------
// Parameters
//		socket 			:	
//------output------
// Return 
//		BOOL 使用之前请调用构造函数，初始化m_pSocket
BOOL ClientSocket::Connect(const CHAR* host, UINT port)
{	
	DEBUG_TRY
        BOOL ret = FALSE;
        CHECKF(m_pSocket);

        // connect address to socket
        ret = m_pSocket->connect(host, port);
        if (!ret)
        {
                LOGERROR("[ClientSocket::Connect] new is Error [%s:%u]", m_pSocket->m_Host, m_pSocket->m_Port);
                return false;
        }
        LOGDEBUG("[ClientSocket::Connect] OK server [%s:%u]", m_pSocket->m_Host, m_pSocket->m_Port);
        return TRUE;
	DEBUG_CATCHF("ClientSocket::Connect")
}