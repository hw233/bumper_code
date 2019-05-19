#include "MsgSynRoomInfo.h"

#include "../Base/logger.h"
#include "../Main/ServerManager.h"


//////////////////////////////////////////////////// CMsgSynRoomInfo ////////////////////////////////////////////////////
//构造函数
CMsgSynRoomInfo::CMsgSynRoomInfo() 
{ 
}

//析构函数
CMsgSynRoomInfo::~CMsgSynRoomInfo() 
{ 	
}

//读取数据
BOOL CMsgSynRoomInfo::Read(SocketInputStream& iStream, UINT nLen)
{
	CNetMsg::Read(iStream, nLen);
	return TRUE;
}

BOOL CMsgSynRoomInfo::Write(SocketOutputStream& oStream) const
{
	return TRUE;
}

//创建消息
BOOL CMsgSynRoomInfo::CreateMsg(UINT nRetStatus, UINT nAction)
{
	ASSERT(!"CMsgUserInfo::CreateMsg()");
	return TRUE;
}

//创建消息
BOOL CMsgSynRoomInfo::CreateMsg(Player* player, UINT nAction, INT nTeamId, INT nTeamColour)
{
	CHECKF(player);
	if (m_pMsg)
	{
		// std::stringstream ss;
		// ss << pAccID->valueint;
		// std::string str = ss.str();
		
		CUserData* pUserData = player->QueryData();
		if (!pUserData)
		{
			LOGERROR("[CMsgUserInfo::CreateMsg] pUserData=NULL");
			return FALSE;
		}

		//!alter by huyf 2019.01.09:车辆改造修改
		CItemDataManager* pItemDataManager = player->QueryItemDataManager();
		if (!pItemDataManager)
		{
			LOGERROR("[CMsgUserInfo::CreateMsg] pItemDataManager=NULL");
			return FALSE;
		}
		//!alter end:车辆改造修改
		
		//cJSON_AddNumberToObject(m_pMsg, "model", nModelType);
		cJSON_AddNumberToObject(m_pMsg, "action", nAction);
		cJSON_AddNumberToObject(m_pMsg, "uid", pUserData->GetUid());
		cJSON_AddStringToObject(m_pMsg, "head_picurl", pUserData->GetHeadPicurl().c_str()==NULL?"default":pUserData->GetHeadPicurl().c_str());
		cJSON_AddStringToObject(m_pMsg, "nick_name", pUserData->GetNickName().c_str()==NULL?"default":pUserData->GetNickName().c_str());	
		
		//!alter by huyf 2019.01.09:车辆改造修改
		//cJSON_AddNumberToObject(m_pMsg, "use_car", pUserData->GetUseCar());
		UINT nCurCarId = pUserData->GetUseCar();
		CItemData* pItemData = pItemDataManager->QueryItemData(nCurCarId);
		if (pItemData)
		{
			cJSON_AddNumberToObject(m_pMsg, "use_car", pItemData->GetItemType());	
		}
		else
		{
			cJSON_AddNumberToObject(m_pMsg, "use_car", 100010);
		}
		//添加车尾
		UINT nCurTailId = pUserData->GetTail();
		pItemData = pItemDataManager->QueryItemData(nCurTailId);
		if (pItemData)
		{
			cJSON_AddNumberToObject(m_pMsg, "use_motion", pItemData->GetItemType());	
		}
		else
		{
			cJSON_AddNumberToObject(m_pMsg, "use_motion", 110010);
		}
		//!alter end:车辆改造修改
		
		//红蓝对战使用字段：team_id、team_colour
		cJSON_AddNumberToObject(m_pMsg, "team_id", nTeamId);
		cJSON_AddNumberToObject(m_pMsg, "team_colour", nTeamColour);
		
		m_pData = cJSON_Print(m_pMsg);
		string strMsg(m_pData);
    	size_t nLen = strMsg.size();
		LOGDEBUG("[CMsgSynRoomInfo::CreateMsg]:\n m_pData=[%s] nLen=[%d]\n", m_pData, nLen);
	}		

	return TRUE;
}

//发送json数据
BOOL CMsgSynRoomInfo::SendMsg(CNetMsg* pMsg)
{
	m_pPlayer->SendMsgToClient(pMsg);
	return TRUE;	
}

//
UINT CMsgSynRoomInfo::Process(VOID *pInfo)
{	
	ASSERT(!"CMsgUserInfo::Process()");
	return PACKET_EXE_CONTINUE;
}


