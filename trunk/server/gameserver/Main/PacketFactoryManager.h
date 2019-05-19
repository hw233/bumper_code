//////////////////////////////////////////////////////////////////////
//文件名称：PacketFactoryManager.h
//功能描述：通信包工厂
//版本说明：PacketFactoryManager API
//			
//编写作者：2017.08.22 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#ifndef __PACKETFACTORYMANAGER_H__
#define __PACKETFACTORY_MANAGER_H__

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include <map>

#include "../Common/Type.h"
#include "../Base/GameUtil.h"
#include "../MsgHandler/NetMsg.h"
#include "PacketFactory.h"


//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////

typedef std::map<PacketID_t, PacketFactory*> FactoryMap; 

class PacketFactoryManager
{
public:
	PacketFactoryManager();
	~PacketFactoryManager();
	
private :
	VOID				AddFactory(PacketFactory* pFactory);
	
public :
	//外部调用通用接口

	//初始化接口
	BOOL				Init();
	//根据消息类型从内存里分配消息实体数据（允许多线程同时调用）
	CNetMsg*				CreatePacket(PacketID_t packetID);
	//根据消息类型取得对应消息的最大尺寸（允许多线程同时调用）
	UINT				GetPacketMaxSize(PacketID_t packetID);
	//删除消息实体（允许多线程同时调用）
	VOID				RemovePacket(CNetMsg* msg);
	
	VOID				Lock(){ m_Lock.Lock(); }
	VOID				Unlock(){ m_Lock.Unlock(); }

private :
	
	//PacketFactory **	m_Factories;

	FactoryMap 			m_FactoryMap;
	
	UINT				m_Size;

	MyLock				m_Lock;

public :
	//UINT*				m_pPacketAllocCount;

};

extern PacketFactoryManager* g_pPacketFactoryManager;

#endif