//////////////////////////////////////////////////////////////////////
//文件名称：RobotBase.cpp
//功能描述：机器人接口（基类）
//版本说明：socket API
//			
//编写作者：2017.08.18 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//include头文件
//////////////////////////////////////////////////////////////////////
#include "Player.h"
#include "../MsgHandler/NetMsg.h"
#include "PlayerManager.h"
#include "PlayerPool.h"

#include "../Base/logger.h"

//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////

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
PlayerManager::PlayerManager()
{
    for (int i = 0; i < MAX_PLAYER; ++i)
    {
        m_nPlayerIDS[i] = INVALID_ID;
    }
    m_nPlayerAmount = 0; 
    //add by huyf 2018.01.24:支持I/O多路复用poll
    m_setPlayer.clear();
    //add end:支持I/O多路复用poll
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
PlayerManager::~PlayerManager()
{	
	CleanUp();
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	CleanUp
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
VOID PlayerManager::CleanUp()
{
    DEBUG_TRY	
    for (int i = 0; i < MAX_PLAYER; ++i)
    {
        m_nPlayerIDS[i] = INVALID_ID;
    }
    m_nPlayerAmount = 0; 
    //add by huyf 2018.01.24:支持I/O多路复用poll
    m_setPlayer.clear();
    //add end:支持I/O多路复用poll
    DEBUG_CATCH("PlayerManager::CleanUp")
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	//增加一个玩家
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL PlayerManager::AddPlayer(Player* pPlayer)
{
	DEBUG_TRY
    CHECKF(pPlayer);
    if(m_nPlayerAmount >= MAX_PLAYER)
    {
        LOGERROR("[PlayerManager::AddPlayer]已经到达最大在线人数，无法再接受新的前端登录请求.PlayerAmount=[%d] MAX_PLAYER=[%d]", m_nPlayerAmount, MAX_PLAYER);
        return FALSE;
    }

    IF_OK(m_nPlayerIDS[m_nPlayerAmount] == INVALID_ID)
    {
        //LOGDEBUG("[PlayerManager::AddPlayer] OK... Online PlayerAmount[%d], PlayerID[%d]", m_nPlayerAmount, pPlayer->GetPlayerID());
        m_nPlayerIDS[m_nPlayerAmount] = pPlayer->GetPlayerID();
        pPlayer->SetPlayerManagerID(m_nPlayerAmount);
        m_nPlayerAmount++;
    }	
    //add by huyf 2018.01.24:支持I/O多路复用poll
    m_setPlayer.insert(std::map<SOCKET, Player*>::value_type(pPlayer->GetSocket()->getSocketID(), pPlayer));
    //add end:支持I/O多路复用poll  
    LOGDEBUG("[PlayerManager::AddPlayer] Is Ok... m_nPlayerAmount=[%d] SocketID=[%d] m_setPlayer.size=[%lu] AccountID=[%d] pPlayerId=[%d] RobotID=[%s]", m_nPlayerAmount, pPlayer->GetSocket()->getSocketID(), m_setPlayer.size(),pPlayer->GetAccountID(),pPlayer->GetPlayerID(),pPlayer->GetRobot()->getRobotId()); 
    
    //!alter by huyf 2018.12.05:屏蔽
    //for (UINT i = 0; i < m_nPlayerAmount; ++i)
    //{
    //    LOGDEBUG("[PlayerManager::AddPlayer] m_nPlayerIDS[%d] = [%d]", i, m_nPlayerIDS[i]);
    //}   
    //!alter end:屏蔽

	DEBUG_CATCHF("PlayerManager::AddPlayer")
    return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	删除一个玩家
//
//-----input------
// Parameters
//		pPlayer 		:	玩家指针
//------output------
// Return 
//		无
BOOL PlayerManager::RemovePlayer(Player* pPlayer)
{
	DEBUG_TRY	
    CHECKF(pPlayer);

    //add by huyf 2018.01.24:支持I/O多路复用poll
    CHECKF(pPlayer->GetSocket());
    auto iter = m_setPlayer.find(pPlayer->GetSocket()->getSocketID());    
    if (iter != m_setPlayer.end())
    {
        m_setPlayer.erase(pPlayer->GetSocket()->getSocketID());
        LOGDEBUG("[PlayerManager::RemovePlayer(Player* pPlayer)] Erase SocketID[%d] From m_setPlayer m_setPlayer.size=[%lu]", pPlayer->GetSocket()->getSocketID(), m_setPlayer.size());
    }
    //add end:支持I/O多路复用poll

    //取得删除玩家的managerID
    ID_t PlayerManagerID = pPlayer->GetPlayerManagerID();
    CHECKF(PlayerManagerID >= 0 && PlayerManagerID < MAX_PLAYER);		

    //取出最后一个位置的玩家
    Player* pPlayer_new = g_pPlayerPool->GetPlayer(m_nPlayerIDS[m_nPlayerAmount-1]);
    CHECKF(pPlayer_new);	

    //将最后的数据，移动到删除的位置。
    m_nPlayerIDS[PlayerManagerID] = m_nPlayerIDS[m_nPlayerAmount-1];
    //设置最后位置的数据初始化
    m_nPlayerIDS[m_nPlayerAmount-1] = INVALID_ID;
    //更新最后一个玩家managerid
    pPlayer_new->SetPlayerManagerID(PlayerManagerID);

    //减少总人数
    m_nPlayerAmount--;
    CHECKF(m_nPlayerAmount >= 0);
    LOGDEBUG("[PlayerManager::RemovePlayer] Is Ok... m_nPlayerAmount=[%d] AccountID=[%d] pPlayerId=[%d] RobotID=[%s]", m_nPlayerAmount, pPlayer->GetAccountID(),pPlayer->GetPlayerID(),pPlayer->GetRobot()->getRobotId()); 

	DEBUG_CATCHF("PlayerManager::RemovePlayer")
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	删除一个玩家
//
//-----input------
// Parameters
//		pid 			:	玩家id
//------output------
// Return 
//		无
BOOL PlayerManager::RemovePlayer(PlayerID_t pid)
{
	DEBUG_TRY	        
    Player* pPlayer = g_pPlayerPool->GetPlayer(pid);
    CHECKF(pPlayer);

    //add by huyf 2018.01.24:支持I/O多路复用poll
    CHECKF(pPlayer->GetSocket());
    auto iter = m_setPlayer.find(pPlayer->GetSocket()->getSocketID());    
    if (iter != m_setPlayer.end())
    {
        m_setPlayer.erase(pPlayer->GetSocket()->getSocketID());
        LOGDEBUG("[PlayerManager::RemovePlayer(PlayerID_t pid)] Erase SocketID=[%d] From m_setPlayer m_setPlayer.size=[%lu]", pPlayer->GetSocket()->getSocketID(), m_setPlayer.size());
    }
    //add end:支持I/O多路复用poll

    //取得删除玩家的managerID
    ID_t PlayerManagerID = pPlayer->GetPlayerManagerID();
    CHECKF(PlayerManagerID >= 0 && PlayerManagerID < MAX_PLAYER);

    //更新为最后一个位置的玩家
    Player* pPlayer_new = g_pPlayerPool->GetPlayer(m_nPlayerIDS[m_nPlayerAmount-1]);
    CHECKF(pPlayer_new);

    //将最后的数据，移动到删除的位置。
    m_nPlayerIDS[PlayerManagerID] = m_nPlayerIDS[m_nPlayerAmount-1];
    //设置最后位置的数据初始化
    m_nPlayerIDS[m_nPlayerAmount-1] = INVALID_ID;
    //更新最后一个玩家managerid
    pPlayer_new->SetPlayerManagerID(PlayerManagerID);

    //减少总人数
    m_nPlayerAmount--;
    CHECKF(m_nPlayerAmount >= 0);
    LOGDEBUG("[PlayerManager::RemovePlayer(PlayerID_t pid)] m_nPlayerAmount=[%d]", m_nPlayerAmount);

    //清理掉网络数据
    //pPlayer->CleanUp();
    //g_pPlayerPool->DelPlayer(pid)
	DEBUG_CATCHF("PlayerManager::RemovePlayer")
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	心跳 逻辑接口
//
//-----input------
// Parameters
//		id 			:	玩家id
//------output------
// Return 
//		无
BOOL PlayerManager::BeatHeart(DWORD dwTime /*=0*/)
{	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	根据socket获取player
//
//-----input------
// Parameters
//		id 			:	sock
//------output------
// Return 
//		无
Player* PlayerManager::GetPlayerBySocket(SOCKET sock)
{
    CHECKF(sock != INVALID_SOCKET);
    
    Player* pPlayer = NULL;
    auto iter = m_setPlayer.find(sock);
    if (iter != m_setPlayer.end())
    {
        pPlayer = iter->second;
    }
    return pPlayer;    
}