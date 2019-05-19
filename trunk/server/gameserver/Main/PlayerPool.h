//////////////////////////////////////////////////////////////////////
//文件名称：PlayerPool.h
//功能描述：机器人预分配数据池
//版本说明：PlayerPool API
//			
//编写作者：2017.08.18 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#ifndef __PLAYERPOOL_H__
#define __PLAYERPOOL_H__

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include <map>

#include "Type.h"
#include "BaseType.h"
#include "ServerPlayer.h"
#include "GameUtil.h"

//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//class
//////////////////////////////////////////////////////////////////////
//分配玩家数据的数据池
//
typedef std::map<UInt32, Player*> PlayerMap; 

class PlayerPool
{
public:
	PlayerPool();
	virtual ~PlayerPool();

public:
	//初始化数据池
	BOOL 				Init(UINT PlayerPoolMaxCount = MAX_POOL_SIZE);
	//根据玩家的PlayerID取得数据指针
	ServerPlayer* 		GetPlayer(PlayerID_t PlayerID);		
	//从空闲数据池里分配一个玩家数据
	ServerPlayer*		NewPlayer();
	//将一个玩家数据清空回收
	void				DelPlayer(PlayerID_t PlayerID);

	//Set Account ID with Player
	void 				SetAccountID(Player* player, UInt32 accountID);
	//Get Player by Account ID
	Player* 			GetPlayerByAccout(UInt32 accountID);
	//Delete Player by Account ID
	void 				DelPlayerByAccout(UInt32 accountID);
	//Show Map
	void 				ShowPlayerMap();
	//根据玩家id获取玩家对象
	Player* 			GetPlayerByUid(UInt32 Uid);


	//锁
	void				Lock()								{	m_Lock.Lock();		} 
	void				Unlock()							{	m_Lock.Unlock();	} 

private :
	ServerPlayer*		m_pPlayerS;					//玩家数组
	//UINT				m_Position;	
	MyLock				m_Lock;
	UINT				m_CurPlayerCount;			//当前在线人数
	UINT				m_PlayerPoolMaxCount;		//预分配的最大玩家缓冲区人数	

	PlayerMap 			m_playerMap;				//玩家账号相关的容器
	DWORD 				m_ReportIntervalTime;		//人数爆满日志报警时间间隔

};

extern PlayerPool* g_pPlayerPool;
#endif