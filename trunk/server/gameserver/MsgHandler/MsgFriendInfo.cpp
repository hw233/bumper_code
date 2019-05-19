#include "MsgFriendInfo.h"

#include "../Base/logger.h"


//////////////////////////////////////////////////// CMsgFriendInfo ////////////////////////////////////////////////////
//构造函数
CMsgFriendInfo::CMsgFriendInfo() 
{ 
}

//析构函数
CMsgFriendInfo::~CMsgFriendInfo() 
{ 	
}

//读取数据
BOOL CMsgFriendInfo::Read(SocketInputStream& iStream, UINT nLen)
{
	CNetMsg::Read(iStream, nLen);
	return TRUE;
}

BOOL CMsgFriendInfo::Write(SocketOutputStream& oStream) const
{
	return TRUE;
}
BOOL CMsgFriendInfo::CreateMsg(UINT nRetStatus, UINT nAction)
{
	return TRUE;
}
//创建消息
BOOL CMsgFriendInfo::CreateMsg(UINT nAction, Player* pPlayer, UINT nStatus, UINT nData/*=0*/)
{
	if (!pPlayer)
	{
		return FALSE;
	}

	if (m_pMsg)
	{
		cJSON_AddNumberToObject(m_pMsg, "action", nAction);
	
		//在线
		if (nAction == _FRIEND_INFO_ONLINE)
		{
			cJSON_AddNumberToObject(m_pMsg, "friend_id", pPlayer->GetUid());	
			cJSON_AddNumberToObject(m_pMsg, "online", nStatus);
		}
		//离线
		else if (nAction == _FRIEND_INFO_OFFLINE)
		{
			cJSON_AddNumberToObject(m_pMsg, "friend_id", pPlayer->GetUid());
			cJSON_AddNumberToObject(m_pMsg, "online", nStatus);
		}
		//更新好友数据
		else if (nAction == _FRIEND_INFO_UPDATE)
		{
			cJSON_AddNumberToObject(m_pMsg, "friend_id", pPlayer->GetUid());
			if (nData == _FRIEND_INFO_ATTRIB_GAMESTATUS)
			{
				cJSON_AddNumberToObject(m_pMsg, "online", nStatus);
			}
			
		}

		m_pData = cJSON_Print(m_pMsg);
		string strMsg(m_pData);
	    size_t nLen = strMsg.size();
		LOGDEBUG("[CMsgFriendInfo::CreateMsg]:\n m_pData=[%s] nLen=[%d]\n", m_pData, nLen);
	}

	return TRUE;
}

//创建消息
BOOL CMsgFriendInfo::CreateMsg(UINT nAction)
{
	if (m_pMsg)
	{
		cJSON_AddNumberToObject(m_pMsg, "action", nAction);

		m_pData = cJSON_Print(m_pMsg);
		string strMsg(m_pData);
	    INT nLen = strMsg.size();
		LOGDEBUG("[CMsgFriendInfo::CreateMsg]:\n m_pData=%s nLen=[%d]\n", m_pData, nLen);
	}

	return TRUE;
}

//创建消息
BOOL CMsgFriendInfo::CreateMsg(UINT nAction, CFriendData* pFriendData, UINT nStatus, UINT nIndex)
{
	if (!pFriendData)
	{
		return FALSE;
	}

	if (m_pMsg)
	{
		cJSON_AddNumberToObject(m_pMsg, "action", nAction);

		//申请好友
		if (nAction == _FRIEND_INFO_ADD)
		{
			cJSON_AddNumberToObject(m_pMsg, "friend_id", pFriendData->GetFriendId());	
			cJSON_AddNumberToObject(m_pMsg, "granding_id", pFriendData->GetGrandingId());	
			cJSON_AddNumberToObject(m_pMsg, "cur_star_sum", pFriendData->GetCurStarSum());		
			cJSON_AddStringToObject(m_pMsg, "head_picurl", pFriendData->GetHeadPicurl().c_str());
			cJSON_AddStringToObject(m_pMsg, "nick_name", pFriendData->GetNickName().c_str());
			cJSON_AddNumberToObject(m_pMsg, "online", nStatus);
		}
		//好友列表信息
		else if (nAction == _FRIEND_INFO_GETLIST)
		{
			cJSON* cjParam = cJSON_CreateObject();
			IF_OK(cjParam)
			{
				std::string strParamKey = "members" + std::to_string(nIndex);

				cJSON_AddNumberToObject(cjParam, "friend_id", pFriendData->GetFriendId());
				cJSON_AddNumberToObject(cjParam, "granding_id", pFriendData->GetGrandingId());	
				cJSON_AddNumberToObject(cjParam, "cur_star_sum", pFriendData->GetCurStarSum());		
				cJSON_AddStringToObject(cjParam, "head_picurl", pFriendData->GetHeadPicurl().c_str());
				cJSON_AddStringToObject(cjParam, "nick_name", pFriendData->GetNickName().c_str());
				cJSON_AddNumberToObject(cjParam, "online", nStatus);

				cJSON_AddItemToObject(m_pMsg, strParamKey.c_str(), cjParam);					
			}
		}	

		m_pData = cJSON_Print(m_pMsg);
		string strMsg(m_pData);
	    INT nLen = strMsg.size();
		LOGDEBUG("[CMsgFriendInfo::CreateMsg]:\n m_pData=%s nLen=[%d]\n", m_pData, nLen);
	}

	return TRUE;
}

//追加消息
BOOL CMsgFriendInfo::AppendMsg(UINT nAction, CFriendData* pFriendData, UINT nStatus, UINT nIndex)
{
	if (!pFriendData)
	{
		return FALSE;
	}

	if (m_pMsg)
	{
		//申请好友
		if (nAction == _FRIEND_INFO_GETLIST)
		{
			cJSON* cjParam = cJSON_CreateObject();
			IF_OK(cjParam)
			{
				std::string strParamKey = "members" + std::to_string(nIndex);

				cJSON_AddNumberToObject(cjParam, "friend_id", pFriendData->GetFriendId());	
				cJSON_AddNumberToObject(cjParam, "granding_id", pFriendData->GetGrandingId());	
				cJSON_AddNumberToObject(cjParam, "cur_star_sum", pFriendData->GetCurStarSum());	
				cJSON_AddStringToObject(cjParam, "head_picurl", pFriendData->GetHeadPicurl().c_str());
				cJSON_AddStringToObject(cjParam, "nick_name", pFriendData->GetNickName().c_str());
				cJSON_AddNumberToObject(cjParam, "online", nStatus);

				cJSON_AddItemToObject(m_pMsg, strParamKey.c_str(), cjParam);					
			}
		}

		m_pData = cJSON_Print(m_pMsg);
		string strMsg(m_pData);
	    INT nLen = strMsg.size();
		LOGDEBUG("[CMsgFriendInfo::AppendMsg]:\n m_pData=%s nLen=[%d]\n", m_pData, nLen);	
	}

	return TRUE;
}

//发送json数据
BOOL CMsgFriendInfo::SendMsg(CNetMsg* pMsg)
{
	m_pPlayer->SendMsgToClient(pMsg);
	return TRUE;	
}

//处理前端发送过来的消息
UINT CMsgFriendInfo::Process(VOID *pInfo)
{	
	LOGDEBUG("[CMsgFriendInfo::Process] begin");	
	return PACKET_EXE_CONTINUE;
}