//////////////////////////////////////////////////////////////////////
//文件名称：CFriendManager.cpp
//功能描述：
//版本说明：CFriendManager API
//			
//编写作者：2018.11.22 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#include "FriendManager.h"
#include "FriendData.h"
#include "../../DataBase/DBMySql.h"
#include "../I_mydb.h"
#include "../Player.h"
#include "../../MsgHandler/MsgFriendInfo.h"


//////////////////////////////////////////////////////////////////////
//常量预定于
char	szFriendTable[] = _TBL_FRIEND;
//////////////////////////////////////////////////////////////////////
//构造
CFriendManager::CFriendManager()
{
    this->Init();
}

//析构函数
CFriendManager::~CFriendManager()
{ 
	this->CleanUp();
}

//类构造函数调用-内部使用
VOID CFriendManager::Init()
{
    this->Clear();
    this->SetOwner(NULL);
}

//用于player类中CleanUp()调用
VOID CFriendManager::CleanUp()
{ 
    this->Clear();
    this->SetOwner(NULL);
}

//清空管理器操作，内部调用
VOID CFriendManager::Clear()
{ 
    for (auto iter = m_vFriendSet.begin(); iter != m_vFriendSet.end(); iter++)
    {   
        if(*iter)
        {
            SAFE_RELEASE(*iter);
        } 
    }

    m_vFriendSet.clear();
}

//OnTimer函数
BOOL CFriendManager::OnTimer()
{
    return TRUE;
}

//加载数据库好友信息
BOOL CFriendManager::LoadInfo() 
{
    if (!Database())
    { 
    	LOGERROR("[CFriendManager::LoadInfo] Database==NULL");
        return FALSE;
    }

    if (!m_pOwner)
    { 
        return FALSE;
        LOGERROR("[CFriendManager::LoadInfo] m_pOwner==NULL");
    }

    //执行一次清空操作
    this->Clear();

    UINT nUid = m_pOwner->GetUid();
    std::string sql = string("SELECT * FROM g_friend WHERE user_id=")+std::to_string(nUid)+string(" LIMIT 100");
    MYSQL_RES* result = Database()->Query(sql);
    if(result == NULL)
    {
        LOGERROR("[CFriendManager::LoadInfo] sql error: %s\n", sql.c_str());
        return FALSE;
    }

    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)))
    {    
    	CFriendData* pFriendData = CFriendData::CreateNew();
    	if (pFriendData)
    	{
    		pFriendData->SetId(SafeAtoi(row[_FRIEND_DATA_ID]));
	        pFriendData->SetUid(SafeAtoi(row[_FRIEND_DATA_USERID]), FALSE);
	        pFriendData->SetFriendId(SafeAtoi(row[_FRIEND_DATA_FRIENDID]), FALSE);
	        pFriendData->SetGrandingId(SafeAtoi(row[_FRIEND_DATA_GRANDINGID]), FALSE);
	        pFriendData->SetCurStarSum(SafeAtoi(row[_FRIEND_DATA_CURSTARSUM]), FALSE);

    		if(row[_FRIEND_DATA_NICKNAME] && strlen(row[_FRIEND_DATA_NICKNAME]))
	        {
	            pFriendData->SetNickName(string(row[_FRIEND_DATA_NICKNAME]), FALSE);
	        }
	        else 
	        {
	            pFriendData->SetNickName("default", FALSE);
	        }
	        
	        if(row[_FRIEND_DATA_HEADPICURL] && strlen(row[_FRIEND_DATA_HEADPICURL]))
	        {
	            pFriendData->SetHeadPicurl(string(row[_FRIEND_DATA_HEADPICURL]), FALSE);
	        }
	        else 
	        {
	            pFriendData->SetHeadPicurl("default", FALSE);
	        }

	        //插入容器
	        m_vFriendSet.push_back(pFriendData);
	        
	        LOGDEBUG("[CFriendManager::LoadInfo] size=[%d] Id=[%d] Uid=[%d] FriendId=[%d] GrandingId=[%d] CurStarSum=[%d] NickName=[%s] HeadPicurl=[%s]", 
            	m_vFriendSet.size(), pFriendData->GetId(), pFriendData->GetUid(), pFriendData->GetFriendId(), pFriendData->GetGrandingId(), pFriendData->GetCurStarSum(), pFriendData->GetNickName().c_str(), pFriendData->GetHeadPicurl().c_str());
    	}
    }
    mysql_free_result(result);
    result = NULL;
    LOGDEBUG("[CFriendManager::LoadInfo] Loading table g_friend end");
    return TRUE;
    
}
//保存好友信息
BOOL CFriendManager::SaveInfo(BOOL bUpdate /*= TRUE*/)
{
    for (auto iter = m_vFriendSet.begin(); iter != m_vFriendSet.end(); iter++)
    {   
        //找到对应玩家
        if((*iter))
        { 
             (*iter)->SaveInfo(bUpdate);
        }
    }

    return TRUE;
}

//获取好友列表信息
BOOL CFriendManager::GetListInfo() 
{
    if (!m_pOwner)
    { 
    	LOGERROR("[CFriendManager::GetListInfo] m_pOwner==NULL");
        return FALSE;
    }

    UINT nIndex = 0;
    UINT nStatus = _FRIEND_STATUS_OFFLINE;
    CMsgFriendInfo msg;
	for (auto iter = m_vFriendSet.begin(); iter != m_vFriendSet.end(); iter++)
    {	
    	//找到对应玩家
    	if((*iter))
    	{    		
    		//查找玩家在线
            Player* pFriend = g_pPlayerPool->GetPlayerByUid((*iter)->GetFriendId());
    		if (pFriend)
    		{
    			//nStatus = _FRIEND_STATUS_ONLINE;
                nStatus = pFriend->GetGameStatus();
    		}
    		else
    		{
    			nStatus = _FRIEND_STATUS_OFFLINE;
    		}

    		//构造好友数据包
    		if (nIndex%10 == 0)
    		{
    			//同步信息			
				msg.CreateMsg(_FRIEND_INFO_GETLIST, (*iter), nStatus, ++nIndex);
				LOGDEBUG("[CFriendManager::GetListInfo] CreateMsg m_unId=[%d] m_unUid=[%d] m_unFriendId=[%d] GrandingId=[%d] CurStarSum=[%d] m_strNickName=[%s] m_strHeadPicurl=[%s] status=[%d] Index=[%d]",
					(*iter)->GetId(),(*iter)->GetUid(),(*iter)->GetFriendId(),(*iter)->GetGrandingId(),(*iter)->GetCurStarSum(),(*iter)->GetNickName().c_str(),(*iter)->GetHeadPicurl().c_str(), nStatus, nIndex);
    		}
    		else//nIndex == 0
    		{
				//同步信息			
				msg.AppendMsg(_FRIEND_INFO_GETLIST, (*iter), nStatus, ++nIndex);
				LOGDEBUG("[CFriendManager::GetListInfo] AppendMsg m_unId=[%d] m_unUid=[%d] m_unFriendId=[%d] GrandingId=[%d] CurStarSum=[%d] m_strNickName=[%s] m_strHeadPicurl=[%s] status=[%d] Index=[%d]",
					(*iter)->GetId(),(*iter)->GetUid(),(*iter)->GetFriendId(),(*iter)->GetGrandingId(),(*iter)->GetCurStarSum(),(*iter)->GetNickName().c_str(),(*iter)->GetHeadPicurl().c_str(), nStatus, nIndex);				
    		}

    		if (nIndex%10 == 0)
		    {
			    //发送给前端请求者
			    m_pOwner->SendMsgToClient(&msg);
			    msg.Reset();
			}
    	}
    }

    if (nIndex%10 != 0)
    {
	    //发送给前端请求者
	    m_pOwner->SendMsgToClient(&msg);
	}
    return TRUE;
}

//插入数据库好友记录
BOOL CFriendManager::InsertRecord(UINT nFriendId, UINT nGrandingId, UINT nCurStarSum, std::string const& strNickName, std::string const& strHeadPicurl)
{
	if (!Database())
    { 
    	LOGERROR("[CFriendManager::InsertRecord] Database==NULL");
        return FALSE;
    }

	if (!m_pOwner)
    { 
    	LOGERROR("[CFriendManager::InsertRecord] m_pOwner==NULL");
        return FALSE;
    }

    time_t ltime;
    time(&ltime);
    tm*    pTm = localtime(&ltime);
    char    buf[128] = {0};
    sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", pTm->tm_year, pTm->tm_mon, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);

    UINT nUid = m_pOwner->GetUid();
    char szSql[1024];
    memset(szSql, 0, 1024);
    sprintf(szSql, "INSERT INTO g_friend (user_id, friend_id, granding_id, cur_star_sum, nick_name, head_picurl, create_time) VALUES (%d,%d,%d,%d,\"%s\",\"%s\",\"%s\")", nUid, nFriendId, nGrandingId, nCurStarSum, strNickName.c_str(), strHeadPicurl.c_str(), buf);
    if(Database()->UpdateRecord(szSql)) 
    {
        INT nId = Database()->GetInsertId();
        LOGDEBUG("[CFriendManager::InsertRecord] Is Suc... nId=[%d] sql=[%s]\n", nId, szSql);

    }
    else 
    {
        LOGERROR("[CFriendManager::InsertRecord] Is Error... sql=[%s]\n", szSql);
        return FALSE;
    }
    return TRUE;
}

//删除数据库好友记录
BOOL CFriendManager::DeleteRecord(UINT nFriendId)
{
	if (!Database())
    { 
    	LOGERROR("[CFriendManager::DeleteRecord] Database==NULL");
        return FALSE;
    }

    if (!m_pOwner)
    { 
    	LOGERROR("[CFriendManager::DeleteRecord] m_pOwner==NULL");
        return FALSE;
    }

    UINT nUid = m_pOwner->GetUid();
    char szSql[1024];
    memset(szSql, 0, 1024);
    sprintf(szSql, "DELETE FROM g_friend WHERE user_id=%d AND friend_id=%d", nUid, nFriendId);
    if(Database()->UpdateRecord(szSql)) 
    {
        LOGDEBUG("[CFriendManager::DeleteRecord] Is Suc... sql=[%s]\n", szSql);
    }
    else 
    {
        LOGERROR("[CFriendManager::DeleteRecord] Is Error... sql=[%s]\n", szSql);
        return FALSE;
    }
    return TRUE;
}

//添加好友
BOOL CFriendManager::AddFriend(CFriendData* pFriend)
{
	if (!Database())
    { 
    	LOGERROR("[CFriendManager::AddFriend] Database==NULL");
        return FALSE;
    }

    if (!pFriend)
    { 
    	LOGERROR("[CFriendManager::AddFriend] pFriend==NULL");
        return FALSE;
    }

    if (!m_pOwner)
    { 
    	LOGERROR("[CFriendManager::AddFriend] m_pOwner==NULL");
        return FALSE;
    }

	//插入数据库
	if(!InsertRecord(pFriend->GetFriendId(), pFriend->GetGrandingId(), pFriend->GetCurStarSum(), pFriend->GetNickName(), pFriend->GetHeadPicurl()))
	{
		return FALSE;
	}

	//插入成功设置数据库的ID
	INT nNewId = Database()->GetInsertId();
	pFriend->SetId(nNewId);

	//插入容器
	m_vFriendSet.push_back(pFriend);

	//同步信息
	CMsgFriendInfo msg;
	if(msg.CreateMsg(_FRIEND_INFO_ADD, pFriend, _FRIEND_STATUS_ONLINE, 0))
	{
		m_pOwner->SendMsgToClient(&msg);
	}

	LOGDEBUG("[CFriendManager::AddFriend] Is Suc...m_unId=[%d] m_unUid=[%d] m_unFriendId=[%d] GrandingId=[%d] CurStarSum=[%d] m_strNickName=[%s] m_strHeadPicurl=[%s]",
		pFriend->GetId(),pFriend->GetUid(),pFriend->GetFriendId(),pFriend->GetGrandingId(),pFriend->GetCurStarSum(),pFriend->GetNickName().c_str(),pFriend->GetHeadPicurl().c_str());

    return TRUE;
}

//删除好友
BOOL CFriendManager::DelFriend(UINT nFriendId)
{
	CHECKF(nFriendId);

	for (auto iter = m_vFriendSet.begin(); iter != m_vFriendSet.end(); iter++)
    {	
    	//找到对应玩家
    	if((*iter)->GetFriendId() ==  nFriendId)
    	{
    		if(DeleteRecord(nFriendId))
    		{
    			SAFE_RELEASE(*iter);
    			m_vFriendSet.erase(iter);
    			return TRUE;
    		}    		
    	}
    }
    return FALSE;
}

//查找好友
CFriendData* CFriendManager::QueryFriend(UINT nFriendId)
{
	CHECKF(nFriendId);

	for (auto iter = m_vFriendSet.begin(); iter != m_vFriendSet.end(); iter++)
    {	
    	//找到对应玩家
    	if((*iter)->GetFriendId() ==  nFriendId)
    	{
    		return (*iter);    		   		
    	}
    }
    return NULL;
}

//是否为好友
BOOL CFriendManager::IsFriend(UINT nFriendId)
{
	CHECKF(nFriendId);

	for (auto iter = m_vFriendSet.begin(); iter != m_vFriendSet.end(); iter++)
    {	
    	//找到对应玩家
    	if((*iter)->GetFriendId() ==  nFriendId)
    	{
    		return TRUE;    		   		
    	}
    }
    return FALSE;
}

//////////////////////////////////////////////////////////////////////
//广播消息给队伍成员
VOID CFriendManager::BroadcastFriendSMsg(CNetMsg* pMsg, Player* pSender/*=NULL*/)
{
	if(!pMsg)
		return;

	for (auto iter = m_vFriendSet.begin(); iter != m_vFriendSet.end(); iter++)
    {	
    	//找到对应玩家
    	UINT nFriendId = (*iter)->GetFriendId();
    	Player* pMember = g_pPlayerPool->GetPlayerByUid(nFriendId);
    	//在线
    	if(pMember)
    	{
    		if (pMember != pSender)
				pMember->SendMsgToClient(pMsg);   		   		
    	}
    }
}