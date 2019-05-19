//////////////////////////////////////////////////////////////////////
//文件名称：PacketFactoryManager.cpp
//功能描述：通信包工厂
//版本说明：PacketFactoryManager API
//			
//编写作者：2017.08.22 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include "PacketFactoryManager.h"
#include "../MsgHandler/AllMsg.h"



//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////
PacketFactoryManager* g_pPacketFactoryManager = NULL;

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
PacketFactoryManager::PacketFactoryManager()
{
        m_FactoryMap.clear();

        //m_Factories = NULL;
        m_Size = 999999;//PACKET_MAX;

        ASSERT(m_Size>0);

/*		
        m_Factories = new PacketFactory*[m_Size];
        assert(m_Factories);
        m_pPacketAllocCount = new UINT[m_Size];
        assert(m_pPacketAllocCount);

        for(INT i=0; i<m_Size; i++) 
        {
                m_Factories[i] = NULL;
                m_pPacketAllocCount[i] = 0;
        }
*/	
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
PacketFactoryManager::~PacketFactoryManager()
{
    FactoryMap::iterator it;
    for (it = m_FactoryMap.begin(); it != m_FactoryMap.end(); it++)
    {
            SAFE_DELETE(it->second);
    }
    m_FactoryMap.clear();

/*
    assert(m_Factories!= NULL);

    for(INT i=0; i<m_Size; i++) 
    {
            SAFE_DELETE(m_Factories[i]);
    }
*/		
    //SAFE_DELETE_ARRAY(m_Factories);
    //SAFE_DELETE_ARRAY(m_pPacketAllocCount);
	
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	初始化所有消息
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		BOOL
BOOL PacketFactoryManager::Init()
{

//  此处添加每种消息的Factory信息
//例如：
	AddFactory(new CMsgActionFactory());
	AddFactory(new CMsgBattleFactory());
	AddFactory(new CMsgBumperTransferFactory());
	AddFactory(new CMsgExchangeFactory());
	AddFactory(new CMsgFriendFactory());
	AddFactory(new CMsgFriendInfoFactory());
	AddFactory(new CMsgItemFactory());
    AddFactory(new CMsgItemAttribFactory());
	AddFactory(new CMsgLoginFactory());
	AddFactory(new CMsgSingUpFactory());
	AddFactory(new CMsgSynRoomInfoFactory());
	AddFactory(new CMsgTaskDetailFactory());
	AddFactory(new CMsgTopListFactory());
	AddFactory(new CMsgUserAttribFactory());
	AddFactory(new CMsgUserInfoFactory());
	AddFactory(new CMsgVasFactory());
	AddFactory(new CMsgWalkFactory());
	AddFactory(new CMsgTestFactory());
	AddFactory(new CMsgTickFactory());
    
//  .....
	
	return TRUE;

}

//////////////////////////////////////////////////////////////////////
//
// func description	:	服务器启动之时添加所有消息包到管理器
//
//-----input------
// Parameters
//		pFactory 		：	消息指针
//------output------
// Return 
//		无
VOID PacketFactoryManager::AddFactory(PacketFactory* pFactory) 
{               
    static int count = 0;
                
	if (m_FactoryMap.find(pFactory->GetPacketID()) == m_FactoryMap.end())
	{
        LOGMSG("[PacketFactoryManager::AddFactory] insert to m_FactoryMap Is Suc Class=【%s】\tPacketID=【%d】\tCount=【%d】", typeid(*pFactory).name(), pFactory->GetPacketID(), ++count);
		m_FactoryMap.insert(FactoryMap::value_type(pFactory->GetPacketID(), pFactory));
	}
	else
	{
        LOGERROR("[PacketFactoryManager::AddFactory] Is existed in m_FactoryMap Is Error Class=【%s】\tPacketID=【%d】\tCount=【%d】", typeid(*pFactory).name(), pFactory->GetPacketID(), ++count);
		//m_FactoryMap[pFactory->GetPacketID()] = pFactory;
	}

/*
	//LOGDEBUG("PacketID %u wait for add", pFactory->GetPacketID());
	if(m_Factories[pFactory->GetPacketID()] != NULL) 
	{//重复设定
		LOGDEBUG("PacketID %u exist", pFactory->GetPacketID());
		//assert(FALSE);
		return;
	}

	m_Factories[pFactory->GetPacketID()] = pFactory;
*/			
	
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	根据消息ID创建相关消息
//
//-----input------
// Parameters
//		packetID 		：	消息id
//------output------
// Return 
//		Packet* 		: 返回对应消息指针
CNetMsg* PacketFactoryManager::CreatePacket(PacketID_t packetID) 
{
		FactoryMap::iterator it;
		it = m_FactoryMap.find(packetID);
		if (it == m_FactoryMap.end())
		{
			LOGERROR("[PacketFactoryManager::CreatePacket] can't find PacketID[%d]", packetID);
			return NULL;
		}
		CNetMsg* pMsg = NULL;
		//Lock();
        pMsg = it->second->CreatePacket();
        ASSERT(pMsg);
        //m_pPacketAllocCount[packetID]++;
		//Unlock();


/*
		if(packetID >= m_Size || m_Factories[packetID] == NULL) 
		{
			//assert(FALSE);
			return NULL;
		}

		Packet* pMsg = NULL;
		Lock();
		MY_TRY
		{
			pMsg = m_Factories[packetID]->CreatePacket();
			assert(pMsg);
			m_pPacketAllocCount[packetID]++;
		}
		MY_CATCH
		{
			pMsg = NULL;
		}
		Unlock();
*/
		return pMsg;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	根据消息id获取消息的最大size
//
//-----input------
// Parameters
//		packetID 		：	消息id
//------output------
// Return 
//		UINT 			: 消息长度
UINT PacketFactoryManager::GetPacketMaxSize(PacketID_t packetID) 
{
		FactoryMap::iterator it;
		it = m_FactoryMap.find(packetID);
		if (it == m_FactoryMap.end())
		{
			LOGERROR("[PacketFactoryManager::CreatePacket] can't find PacketID[%d]", packetID);
			return 0;
		}
		//Lock();
		UINT iRet = it->second->GetPacketMaxSize();
		//Unlock();

		return iRet;

/*
		if(packetID >= m_Size || m_Factories[packetID] == NULL) 
		{
			char buff[256] = {0};
			sprintf(buff,"PacketID= %d 消息没有注册到PacketFactoryManager上", packetID);
			//assert(FALSE,buff);
			return 0;
		}

		Lock();
		UINT iRet = m_Factories[packetID]->GetPacketMaxSize();
		Unlock();

		return iRet;
*/			
	
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	删除指定的消息
//
//-----input------
// Parameters
//		Packet* 		：	消息指针
//------output------
// Return 
//		无
VOID PacketFactoryManager::RemovePacket(CNetMsg* msg)
{

        CHECK(msg);

        PacketID_t packetID = msg->GetPacketID();
        if(packetID >= m_Size) 
        {
            LOGERROR("[PacketFactoryManager::RemovePacket] packetID >= m_Size packetID[%d], m_Size[%d]", packetID, m_Size);
            return;
        }

        //Lock();
        SAFE_DELETE(msg);
        //m_pPacketAllocCount[packetID] --;
        //Unlock();
        return;
}



