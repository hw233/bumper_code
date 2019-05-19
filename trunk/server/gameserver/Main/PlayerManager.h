//////////////////////////////////////////////////////////////////////
//文件名称：PlayerManager.h
//功能描述：机器人管理器
//版本说明：socket API
//			
//编写作者：2017.08.18 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#ifndef __PLAYERMANAGER_H__
#define __PLAYERMANAGER_H__

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include "../Common/Type.h"
#include "../NetBase/Socket.h"

#include "Player.h"
#include <map>
using namespace std;

//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//class
//////////////////////////////////////////////////////////////////////
//管理一群玩家功能的基本模块,一般为每个场景创建一个PlayerManager
//并且在一个独立的线程内执行
class PlayerManager
{
public :
	PlayerManager();
	virtual ~PlayerManager();

public:
	//增加一个玩家
	BOOL AddPlayer(Player* pPlayer);

	//删除一个玩家
	BOOL RemovePlayer(Player* pPlayer);
	BOOL RemovePlayer(PlayerID_t pid);        
	//add by huyf 2018.01.24:支持I/O多路复用poll
	//查找一个玩家
	Player* GetPlayerBySocket(SOCKET sock);
	//add end:支持I/O多路复用poll
        
	//清除当前模块内的数据信息
	VOID CleanUp();

	//逻辑接口
	BOOL BeatHeart(DWORD dwTime=0);

public:
	PlayerID_t* GetPlayerIDS()						{	return	m_nPlayerIDS;			}
	UINT 		GetPlayerAmount()					{	return	m_nPlayerAmount;		}

protected:
	PlayerID_t 		m_nPlayerIDS[MAX_PLAYER];
	UINT	   		m_nPlayerAmount;
	//add by huyf 2018.01.24:支持I/O多路复用poll
	typedef std::map<SOCKET, Player*>	PLAYERLIST_SET;
	PLAYERLIST_SET          m_setPlayer;
	//add end:支持I/O多路复用poll
};
#endif