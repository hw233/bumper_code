//////////////////////////////////////////////////////////////////////
//文件名称：ClientPool.h
//功能描述：内部连接预分配数据池
//版本说明：ClientPool API
//			
//编写作者：2017.09.05 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#ifndef __CLIENTPOOL_H__
#define __CLIENTPOOL_H__

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include "Type.h"
#include "ClientPlayer.h"
#include "GameUtil.h"

//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////
#define MAX_CLIENT_POOL_SIZE 100

//////////////////////////////////////////////////////////////////////
//class
//////////////////////////////////////////////////////////////////////
//内部连接预分配数据池
//
class ClientPool
{
public:
	ClientPool();
	virtual ~ClientPool();

public:
	//初始化数据池
	BOOL 				Init(UINT ClientPoolMaxCount = MAX_CLIENT_POOL_SIZE);
	//根据玩家的ClientID取得数据指针
	ClientPlayer* 		GetClient(ClientID_t ClientID);		
	//从空闲数据池里分配一个玩家数据
	ClientPlayer*		NewClient();
	//将一个玩家数据清空回收
	void				DelClient(ClientID_t ClientID);
	//锁
	void				Lock()								{	m_Lock.Lock();		} 
	void				Unlock()							{	m_Lock.Unlock();	} 

private :
	ClientPlayer*		m_pClientS;
	UINT				m_Position;
	MyLock				m_Lock;
	UINT				m_ClientCount;
	UINT				m_ClientPoolMaxCount;	


};

extern ClientPool* g_pClientPool;
#endif