
//////////////////////////////////////////////////////////////////////
//文件名称：PlayerPool.cpp
//功能描述：机器人预分配数据池
//版本说明：PlayerPool API
//          
//编写作者：2017.08.18 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//include头文件
//////////////////////////////////////////////////////////////////////
#include "PlayerPool.h"

#include "logger.h"
#include <ctime>
#include <chrono>

using namespace chrono;
//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////
PlayerPool* g_pPlayerPool = NULL;

//////////////////////////////////////////////////////////////////////
//
// func description :   constructor
//
//-----input------
// Parameters
//      无
//------output------
// Return 
//      无
PlayerPool::PlayerPool()
{   
    m_pPlayerS = NULL;
    //m_Position = 0;
    m_CurPlayerCount = 0;
    m_PlayerPoolMaxCount = 0;
    //!alter by huyf 2018.03.01://人数爆满日志报警时间间隔
    system_clock::time_point time = system_clock::now(); 
    m_ReportIntervalTime = system_clock::to_time_t(time);
    //!alter end://人数爆满日志报警时间间隔
}

//////////////////////////////////////////////////////////////////////
//
// func description :   destructor
//
//-----input------
// Parameters
//      无
//------output------
// Return 
//      无
PlayerPool::~PlayerPool()
{   
    SAFE_DELETE_ARRAY(m_pPlayerS);
}

//////////////////////////////////////////////////////////////////////
//
// func description :   初始化数据池
//
//-----input------
// Parameters
//      无
//------output------
// Return 
//      无
BOOL PlayerPool::Init(UINT PlayerPoolMaxCount /*= MAX_POOL_SIZE*/)
{
    DEBUG_TRY   
    m_playerMap.clear();
    m_PlayerPoolMaxCount = PlayerPoolMaxCount;

    //!alter by huyf 2018.03.01://人数爆满日志报警时间间隔
    system_clock::time_point time = system_clock::now(); 
    m_ReportIntervalTime = system_clock::to_time_t(time);
    //!alter end://人数爆满日志报警时间间隔

    m_pPlayerS = new ServerPlayer[PlayerPoolMaxCount];
    CHECKF(m_pPlayerS);

    for (UINT i = 0; i < PlayerPoolMaxCount; i++)
    {
        //此处的playerid可由开发者可以按照一定的值进行生成，而不是自增量形式
        m_pPlayerS[i].SetPlayerID((PlayerID_t)i);
        m_pPlayerS[i].SetEmpty(TRUE);
        //设置套接字接受缓冲区***net.ipv4.tcp_wmem = 4096 16384 4194304 *** 初始.默认.最大
        //m_pPlayerS[i].SetReceiveBufferSize(RECEIVE_BUFFER_SIZE);
        //设置套接字发送缓冲区***net.ipv4.tcp_rmem = 4096 87380 4194304 *** 初始.默认.最大
        //m_pPlayerS[i].SetSendBufferSize(SEND_BUFFER_SIZE);
    }

    //m_Position = 0;
    m_CurPlayerCount = 0;
    //LOGDEBUG("g_pPlayerPool->Init() Load m_Position=【%d】 MAX_POOL_SIZE=【%d】", m_Position, m_PlayerPoolMaxCount);

    DEBUG_CATCHF("PlayerPool::Init")
    return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
// func description :   根据玩家的PlayerID取得数据指针
//
//-----input------
// Parameters
//      PlayerID        :   玩家id
//------output------
// Return 
//      无
ServerPlayer* PlayerPool::GetPlayer(PlayerID_t PlayerID)
{   
    if (PlayerID == INVALID_ID || PlayerID >= (PlayerID_t)m_PlayerPoolMaxCount || PlayerID < 0)
    {
        return NULL;
    }   
    return &m_pPlayerS[PlayerID];
}

//////////////////////////////////////////////////////////////////////
//
// func description :   从空闲数据池里分配一个玩家数据
//
//-----input------
// Parameters
//      无
//------output------
// Return 
//      无
ServerPlayer* PlayerPool::NewPlayer()
{
    DEBUG_TRY
    //Lock();
    //int iRet = 0;
    for (UINT i = 0; i < m_PlayerPoolMaxCount; ++i)
    {
        if(m_pPlayerS[i/*m_Position*/].IsEmpty())
        {

            //iRet = i/*m_Position*/;
            m_pPlayerS[i/*m_Position*/].SetEmpty(FALSE);

            //m_Position++;
            //if(m_Position >= m_PlayerPoolMaxCount)
            //        m_Position = 0;

            //!alter by huyf 2018.02.07:设置初始时间，创建连接后，如果30秒收不到心跳包则断开连接
            system_clock::time_point time = system_clock::now(); 
            m_pPlayerS[i].SetNowTime(system_clock::to_time_t(time));
            //!alter end:设置初始时间，创建连接后，如果30秒收不到心跳包则断开连接

            m_CurPlayerCount++;
            //Unlock();
            LOGDEBUG("[PlayerPool::NewPlayer] Is Ok... Index=[%d] m_CurPlayerCount=[%d] m_PlayerPoolMaxCount=[%d] PlayerId=[%d] m_time=[%u] accountID=[%d] RobotId=[%s]", 
                i, m_CurPlayerCount, m_PlayerPoolMaxCount, m_pPlayerS[i].GetPlayerID(), m_pPlayerS[i].GetNowTime(),m_pPlayerS[i].GetAccountID(),m_pPlayerS[i].GetRobot()->getRobotId());
            return &m_pPlayerS[i/*iRet*/];

        }
    }

    //!alter by huyf 2018.03.01://人数爆满日志报警时间间隔
    system_clock::time_point oldTime = system_clock::from_time_t(m_ReportIntervalTime); 
    system_clock::time_point time = system_clock::now(); 
    auto duration = duration_cast<seconds>(time - oldTime);
    if ((duration.count() * seconds::period::num / seconds::period::den) >= g_config.ONLINE_REPORT_INTERVAL_TIME/*REPORT_INTERVAL_TIME*/)
    { 
        m_ReportIntervalTime = system_clock::to_time_t(time);
        LOGERROR("[PlayerPool::NewPlayer] Is Error Reason : [No Empty Item] m_CurPlayerCount=[%d] m_PlayerPoolMaxCount=[%d]", m_CurPlayerCount, m_PlayerPoolMaxCount);        //说明连接的客户端已经达到上限  
    }
    //!alter end://人数爆满日志报警时间间隔
    
    //m_Position++;
    //if(m_Position >= m_PlayerPoolMaxCount)
    //        m_Position = 0;

    //Unlock();
    DEBUG_CATCH("PlayerPool::NewPlayer")

    return NULL;        
}   

//////////////////////////////////////////////////////////////////////
//
// func description :   从空闲数据池里一个玩家数据
//
//-----input------
// Parameters
//      无
//------output------
// Return 
//      无
VOID PlayerPool::DelPlayer(PlayerID_t PlayerID)
{
    DEBUG_TRY   
    //Lock();
    //LOGDEBUG("[PlayerPool::DelPlayer] PlayerID[%u]",(UINT)PlayerID);
    if((UINT)PlayerID >= m_PlayerPoolMaxCount)
    {
        LOGERROR("[PlayerPool::DelPlayer] PlayerID >= m_PlayerPoolMaxCount PlayerID=[%d] m_PlayerPoolMaxCount=[%d]",PlayerID, m_PlayerPoolMaxCount);
        //assert(FALSE);
        //Unlock();
        return;
    }

    m_pPlayerS[PlayerID].SetEmpty(TRUE);

    m_CurPlayerCount--;

    //!alter by huyf 2018.02.07:设置初始时间，创建连接后，如果30秒收不到心跳包则断开连接
    system_clock::time_point time = system_clock::now(); 
    m_pPlayerS[PlayerID].SetNowTime(system_clock::to_time_t(time));
    //!alter end:设置初始时间，创建连接后，如果30秒收不到心跳包则断开连接

    //Unlock();
    LOGDEBUG("[PlayerPool::DelPlayer] Is Ok PlayerID=[%d] m_CurPlayerCount=[%d] Empty[ TRUE ]", PlayerID, m_CurPlayerCount);
    DEBUG_CATCH("PlayerPool::DelPlayer")
    return;
}   


//////////////////////////////////////////////////////////////////////
//
// func description :   Set Account ID with Player
//
//-----input------
// Parameters
//      无
//------output------
// Return 
//      无
void PlayerPool::SetAccountID(Player* player, UInt32 accountID)
{
    DEBUG_TRY
    //LOGDEBUG("[PlayerPool::SetAccountID] player[%d]", player);
    if(m_playerMap.find(accountID) == m_playerMap.end())
    {
        LOGDEBUG("[PlayerPool::SetAccountID] Insert AccountID=[%d] and playerID=[%d] player===>[%p] RobotId=[%s] to m_playerMap", accountID, player->m_PID, player, player->GetRobot()->getRobotId());
        m_playerMap.insert(PlayerMap::value_type(accountID, player));
    }
    else
    {
        PlayerMap::iterator iter = m_playerMap.find(accountID);
        LOGWARNING("[PlayerPool::SetAccountID] 重复收到了设置账号信息......AccountID=[%d] player=[%p===>%p] RobotId=[%s] ", accountID, iter->second, player, player->GetRobot()->getRobotId());
        //m_playerMap[accountID] = player;
    }

    PlayerMap::iterator iter;
    for (iter = m_playerMap.begin(); iter != m_playerMap.end(); iter++)
    {
        LOGDEBUG("[PlayerPool::SetAccountID] m_playerMap[%d] playerid=[%d] RobotId=[%s]===>[%p] -------size=[%d]", iter->first, iter->second->GetPlayerID(),iter->second->GetRobot()->getRobotId(), iter->second, m_playerMap.size());                    
    }

    ShowPlayerMap();
    DEBUG_CATCH("PlayerPool::SetAccountID")
    return;
}

//////////////////////////////////////////////////////////////////////
//
// func description :   Get Player by Account ID
//
//-----input------
// Parameters
//      无
//------output------
// Return 
//      无
Player* PlayerPool::GetPlayerByAccout(UInt32 accountID)
{
    PlayerMap::iterator it;
    it = m_playerMap.find(accountID);
    if(it == m_playerMap.end())
    {
        LOGWARNING("[PlayerPool::GetPlayerByAccout] Can't Find AccountID=[%d] Reason May Be NoRecvive C2GSLoginPacket", accountID);
        PlayerMap::iterator iter;
        for (iter = m_playerMap.begin(); iter != m_playerMap.end(); iter++)
        {
            LOGDEBUG("[PlayerPool::GetPlayerByAccout] m_playerMap[%d]===>[%p]--------Size=[%d]", iter->first, iter->second, m_playerMap.size());                    
        }
        return NULL;
    }

    LOGDEBUG("[PlayerPool::GetPlayerByAccout] Find AccountID=[%d]===>[%p] This AccountID", accountID, it->second);
    return it->second;
}

//////////////////////////////////////////////////////////////////////
//
// func description :   Delete Player by Account ID
//
//-----input------
// Parameters
//      无
//------output------
// Return 
//      无
void PlayerPool::DelPlayerByAccout(UInt32 accountID)
{
    PlayerMap::iterator it;
    it = m_playerMap.find(accountID);
    if (it != m_playerMap.end())
    {
        m_playerMap.erase(accountID);
        LOGDEBUG("[PlayerPool::DelPlayerByAccout] Erase Player From m_playerMap By AccountID=[%d]", accountID);
    }
    else
            LOGERROR("[PlayerPool::DelPlayerByAccout] Can't Find In m_playerMap By AccountID=[%d]", accountID);

    ShowPlayerMap();
}

void PlayerPool::ShowPlayerMap()
{
    PlayerMap::iterator it;
    for (it = m_playerMap.begin(); it != m_playerMap.end(); it++)
    {
        // LOGDEBUG("[PlayerPool::ShowPlayerMap] [%d, %d]", it->first, it->second->m_PID);
    }
}

//////////////////////////////////////////////////////////////////////
//
// func description :   Get Player by user ID
//
//-----input------
// Parameters
//      无
//------output------
// Return 
//      无
Player* PlayerPool::GetPlayerByUid(UInt32 Uid)
{
    for (UINT i = 0; i < m_PlayerPoolMaxCount; ++i)
    {
        //LOGERROR("[PlayerPool::GetPlayerByUid]-1 m_pPlayerS[i].IsEmpty=[%d] Uid=[%d]", m_pPlayerS[i].IsEmpty(), Uid);
        if(!m_pPlayerS[i].IsEmpty() && m_pPlayerS[i].QueryData()->GetUid() == Uid)
        {
            //LOGERROR("[PlayerPool::GetPlayerByUid] m_pPlayerS[i].IsEmpty=[%d] Uid=[%d]", m_pPlayerS[i].IsEmpty(), Uid);
            return &m_pPlayerS[i];
        }
    }
    return NULL;
}