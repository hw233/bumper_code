
//////////////////////////////////////////////////////////////////////
//文件名称：ClientPool.cpp
//功能描述：内部连接预分配数据池
//版本说明：PlayerPool API
//			
//编写作者：2017.09.05 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//include头文件
//////////////////////////////////////////////////////////////////////
#include "ClientPool.h"

//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////
ClientPool* g_pClientPool = NULL;

//////////////////////////////////////////////////////////////////////
//
// func description	:	constructor
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
ClientPool::ClientPool()
{
        DEBUG_TRY	
        m_pClientS = NULL;
        m_Position = 0;
        m_ClientCount = 0;
        m_ClientPoolMaxCount = 0;
        DEBUG_CATCH("ClientPool::ClientPool")
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
ClientPool::~ClientPool()
{
	DEBUG_TRY	
	SAFE_DELETE_ARRAY(m_pClientS);
	DEBUG_CATCH("ClientPool::~ClientPool")
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	初始化数据池
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL ClientPool::Init(UINT ClientPoolMaxCount /*= MAX_CLIENT_POOL_SIZE*/)
{
	DEBUG_TRY
        m_ClientPoolMaxCount = ClientPoolMaxCount;

        m_pClientS = new ClientPlayer[ClientPoolMaxCount];
        CHECKF(m_pClientS);

        for (UINT i = 0; i < ClientPoolMaxCount; i++)
        {
                //printf("------------PlayerPool::Init i=[%u]-----------\n",(PlayerID_t)i);
                //此处的playerid可由开发者可以按照一定的值进行生成，而不是自增量形式
                m_pClientS[i].SetClientID((ClientID_t)i);
                m_pClientS[i].SetEmpty(TRUE);
        }

        m_Position = 0;
        m_ClientCount = m_ClientPoolMaxCount;        
	DEBUG_CATCHF("ClientPool::Init")
        return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	根据内部的连接id取得数据指针
//
//-----input------
// Parameters
//		PlayerID 		:	内部分配的id
//------output------
// Return 
//		无
ClientPlayer* ClientPool::GetClient(ClientID_t ClientID)
{
	DEBUG_TRY	
        if (ClientID == INVALID_ID || ClientID >= (ClientID_t)m_ClientPoolMaxCount || ClientID < 0)
        {
                return NULL;
        }

        return &m_pClientS[ClientID];		
	DEBUG_CATCH("ClientPool::GetClient")
	return NULL;
}
//////////////////////////////////////////////////////////////////////
//
// func description	:	从空闲数据池里分配一个玩家数据
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
ClientPlayer* ClientPool::NewClient()
{
	DEBUG_TRY
        //Lock();
        int iRet = 0;
        for (UINT i = 0; i < m_ClientPoolMaxCount; ++i)
        {
                if(m_pClientS[m_Position].IsEmpty())
                {

                        iRet = m_Position;
                        m_pClientS[m_Position].SetEmpty(FALSE);

                        m_Position++;
                        if(m_Position >= m_ClientPoolMaxCount)
                                m_Position = 0;

                        m_ClientCount--;
                        //Unlock();
                        LOGDEBUG("[ClientPool::NewClient] ok m_Position[%u], m_ClientCount[%d], m_ClientPoolMaxCount=[%u], ClientId=[%u]", m_Position, m_ClientCount, m_ClientPoolMaxCount, m_pClientS[iRet].GetClientID());
                        return &m_pClientS[iRet];
                }
        }
        
        m_Position++;
        if(m_Position >= m_ClientPoolMaxCount)
                m_Position = 0;

        //Unlock();
        printf("[ClientPool::NewClient] Error");
        DEBUG_CATCH("ClientPool::NewClient")
        return NULL;
}	

//////////////////////////////////////////////////////////////////////
//
// func description	:	从空闲数据池里刪除一个内部连接数据
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
VOID ClientPool::DelClient(ClientID_t ClientID)
{
	DEBUG_TRY	
        //Lock();
        printf("[ClientPool::DelClient] ClientID[%u]", (UINT)ClientID);
        CHECK((UINT)ClientID < m_ClientPoolMaxCount);		

        m_pClientS[ClientID].SetEmpty(TRUE);

        m_ClientCount++;

        //Unlock();
        printf("[ClientPool::DelClient] ok");        
	DEBUG_CATCH("ClientPool::DelClient")
        return;
}	
