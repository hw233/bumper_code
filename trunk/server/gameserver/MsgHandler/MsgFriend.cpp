#include "MsgFriend.h"

#include "../Base/logger.h"
#include "../Main/ServerManager.h"
#include "MsgFriendInfo.h"


//////////////////////////////////////////////////// CMsgFriend ////////////////////////////////////////////////////
//构造函数
CMsgFriend::CMsgFriend() 
{ 
}

//析构函数
CMsgFriend::~CMsgFriend() 
{ 	
}

//读取数据
BOOL CMsgFriend::Read(SocketInputStream& iStream, UINT nLen)
{
	CNetMsg::Read(iStream, nLen);
	return TRUE;
}

BOOL CMsgFriend::Write(SocketOutputStream& oStream) const
{
	return TRUE;
}
BOOL CMsgFriend::CreateMsg(UINT nRetStatus, UINT nAction)
{
	return TRUE;
}
//创建消息
BOOL CMsgFriend::CreateMsg(UINT nAction/*=_FRIEND_APPLY*/, UINT nUid/*=0*/, const string& strNickName/*="default"*/, const string& strHeadPicurl/*="default"*/)
{	
	if (m_pMsg)
	{
		cJSON_AddNumberToObject(m_pMsg, "action", nAction);

		//申请好友
		if (nAction == _FRIEND_APPLY)
		{
			cJSON_AddNumberToObject(m_pMsg, "invite_id", nUid);		
			cJSON_AddStringToObject(m_pMsg, "head_picurl", strHeadPicurl.c_str());
			cJSON_AddStringToObject(m_pMsg, "nick_name", strNickName.c_str());
		}

		m_pData = cJSON_Print(m_pMsg);
		string strMsg(m_pData);
	    size_t nLen = strMsg.size();
		LOGDEBUG("[CMsgFriend::CreateMsg]:\n m_pData=[%s] nLen=[%d]\n", m_pData, nLen);
	}

	return TRUE;
}

//发送json数据
BOOL CMsgFriend::SendMsg(CNetMsg* pMsg)
{
	m_pPlayer->SendMsgToClient(pMsg);
	return TRUE;	
}

//处理前端发送过来的消息
UINT CMsgFriend::Process(VOID *pInfo)
{	
	LOGDEBUG("[CMsgFriend::Process] begin");

	DEBUG_TRY
	//设置需要回包的玩家
	m_pPlayer = static_cast<Player*>(pInfo);
	CHECKF(m_pPlayer);

	//处理前端发过来的消息
	m_pMsg = cJSON_Parse(this->m_pData);
	cJSON* pJsonMsg = m_pMsg;	
	if(pJsonMsg)
	{
		cJSON* pAction = cJSON_GetObjectItem(pJsonMsg, "action");				//_BATTLE_ACTION		
		CHECKF(pAction);

		LOGDEBUG("[CMsgFriend::Process] PlayerId=[%d] pAction=[%d] pPlayer=[%p]", m_pPlayer->GetUid(), pAction->valueint, m_pPlayer);
		switch(pAction->valueint)
		{
			///////////////////////////////////////////////////////////////////////////
			///申请好友
			case _FRIEND_APPLY:
			{
				cJSON* pTargetId = cJSON_GetObjectItem(pJsonMsg, "target_id");			//目标玩家id
				CHECKF(pTargetId);

				//获取在线玩家，离线不处理
				UINT nTargetId = pTargetId->valueint;
				//自己不能添加自己为好友
				if (nTargetId == m_pPlayer->GetUid())
				{
					LOGERROR("[CMsgFriend::Process] Not Make Friends With Self nTargetId==m_pPlayer->GetUid nTargetId=[%d]", nTargetId);
					return PACKET_EXE_CONTINUE;
				}

				//获取目标玩家
				Player* pTarget = g_pPlayerPool->GetPlayerByUid(nTargetId);
				if (!pTarget)
				{
					LOGERROR("[CMsgFriend::Process] pTarget==NULL nTargetId=[%d] Is OffLine", nTargetId);
					return PACKET_EXE_CONTINUE;
				}

				//检查好友数量
				if(m_pPlayer->QueryFriendManager().GetFriendSet().size() >= (size_t)g_config.ONLINE_MAX_FRIEND_AMOUNT)
				{
					LOGERROR("[CMsgFriend::Process] Friend Amount Have Full size=[%d] ONLINE_MAX_FRIEND_AMOUNT=[%d]", m_pPlayer->QueryFriendManager().GetFriendSet().size(), g_config.ONLINE_MAX_FRIEND_AMOUNT);
					return PACKET_EXE_CONTINUE;
				}

				//判断是否已经为好友
				if(m_pPlayer->QueryFriendManager().IsFriend(nTargetId))
				{
					LOGERROR("[CMsgFriend::Process] Playerid=[%d] nTargetId=[%d] Is Friend", m_pPlayer->GetUid(), nTargetId);
					return PACKET_EXE_CONTINUE;
				}

				//同步玩家属性
				CMsgFriend msg;
				if(msg.CreateMsg(_FRIEND_APPLY, m_pPlayer->GetUid(), m_pPlayer->GetNickName(), m_pPlayer->GetHeadPicurl()))
				{
					pTarget->SendMsgToClient(&msg);
				}
			}
			break;
			///接受请求
			case _FRIEND_ACCEPT:
			{
				cJSON* pInviteId = cJSON_GetObjectItem(pJsonMsg, "invite_id");			//目标玩家id
				CHECKF(pInviteId);

				//获取在线玩家，离线不处理
				UINT nInviteId = pInviteId->valueint;

				//自己不能添加自己为好友
				if (nInviteId == 0 || nInviteId == m_pPlayer->GetUid())
				{
					LOGERROR("[CMsgFriend::Process] Not Make Friends With Self nInviteId==m_pPlayer->GetUid nInviteId=[%d]", nInviteId);
					return PACKET_EXE_CONTINUE;
				}
				
				Player* pInviter = g_pPlayerPool->GetPlayerByUid(nInviteId);
				if (!pInviter)
				{
					LOGERROR("[CMsgFriend::Process] pInviter==NULL nInviteId=[%d] Is OffLine", nInviteId);
					return PACKET_EXE_CONTINUE;
				}

				//判断是否已经为好友
				if(m_pPlayer->QueryFriendManager().IsFriend(nInviteId))
				{
					LOGERROR("[CMsgFriend::Process] Playerid=[%d] nInviteId=[%d] Is Friend", m_pPlayer->GetUid(), nInviteId);
					return PACKET_EXE_CONTINUE;
				}

				//检查好友数量
				if(m_pPlayer->QueryFriendManager().GetFriendSet().size() >= (size_t)g_config.ONLINE_MAX_FRIEND_AMOUNT)
				{
					LOGERROR("[CMsgFriend::Process] Playerid=[%d] Friend Amount Have Full size=[%d] ONLINE_MAX_FRIEND_AMOUNT=[%d]", m_pPlayer->GetUid(), m_pPlayer->QueryFriendManager().GetFriendSet().size(), g_config.ONLINE_MAX_FRIEND_AMOUNT);
					return PACKET_EXE_CONTINUE;
				}

				//检查好友数量
				if(pInviter->QueryFriendManager().GetFriendSet().size() >= (size_t)g_config.ONLINE_MAX_FRIEND_AMOUNT)
				{
					LOGERROR("[CMsgFriend::Process] nInviteId=[%d] Friend Amount Have Full size=[%d] ONLINE_MAX_FRIEND_AMOUNT=[%d]", nInviteId, pInviter->QueryFriendManager().GetFriendSet().size(), g_config.ONLINE_MAX_FRIEND_AMOUNT);
					return PACKET_EXE_CONTINUE;
				}

				//添加到自己的好友列表
				CFriendData* pFriend = CFriendData::CreateNew();
				if (!pFriend)
				{
					LOGERROR("[CMsgFriend::Process] pFriend==NULL");
					return PACKET_EXE_CONTINUE;
				}
				pFriend->SetUid(m_pPlayer->GetUid(), FALSE);
				pFriend->SetFriendId(nInviteId, FALSE);
				pFriend->SetGrandingId(pInviter->GetGrandingId(), FALSE);
				pFriend->SetCurStarSum(pInviter->GetCurStarSum(), FALSE);
				pFriend->SetNickName(pInviter->GetNickName(), FALSE);
				pFriend->SetHeadPicurl(pInviter->GetHeadPicurl(), FALSE);
				if(!m_pPlayer->QueryFriendManager().AddFriend(pFriend))
				{
       				SAFE_RELEASE(pFriend);
       				LOGERROR("[CMsgFriend::Process] m_pPlayer AddFriend Is Error nInviteId=[%d]", nInviteId);
					return PACKET_EXE_CONTINUE;
				}

				pFriend = CFriendData::CreateNew();
				if (!pFriend)
				{
					LOGERROR("[CMsgFriend::Process] pFriend==NULL");
					return PACKET_EXE_CONTINUE;
				}
				pFriend->SetUid(nInviteId, FALSE);
				pFriend->SetFriendId(m_pPlayer->GetUid(), FALSE);
				pFriend->SetGrandingId(m_pPlayer->GetGrandingId(), FALSE);
				pFriend->SetCurStarSum(m_pPlayer->GetCurStarSum(), FALSE);
				pFriend->SetNickName(m_pPlayer->GetNickName(), FALSE);
				pFriend->SetHeadPicurl(m_pPlayer->GetHeadPicurl(), FALSE);
				//添加到对方好友列表
				if(!pInviter->QueryFriendManager().AddFriend(pFriend))
				{
					SAFE_RELEASE(pFriend);
       				LOGERROR("[CMsgFriend::Process] pInviter AddFriend Is Error nInviteId=[%d]", nInviteId);
					return PACKET_EXE_CONTINUE;
				}
				
			}
			break;
			///拒绝请求
			case _FRIEND_REFUSE:
			{
				LOGERROR("[CMsgFriend::Process] Action==_FRIEND_REFUSE");
				return PACKET_EXE_CONTINUE;
			}
			break;
			///获取好友信息
			case _FRIEND_GETLIST:
			{
				CMsgFriend msg;
				if(msg.CreateMsg(_FRIEND_GETLIST))
				{
					SendMsg(&msg);
				}
				m_pPlayer->QueryFriendManager().GetListInfo();
			}
			break;
		}
	}

	return PACKET_EXE_CONTINUE;
	DEBUG_CATCH0("[CMsgFriend::Process] Is Catch...");
}