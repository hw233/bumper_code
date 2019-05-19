//////////////////////////////////////////////////////////////////////
//文件名称：CFriendData.cpp
//功能描述：
//版本说明：CFriendData API
//			
//编写作者：2018.11.22 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#include "FriendData.h"
#include "../../DataBase/DBMySql.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFriendData::CFriendData()
{
    SetObjType(OBJ_USER);
	Init();
}

CFriendData::~CFriendData()
{
}

//类构造函数调用-内部使用
VOID CFriendData::Init()
{
    m_unId = 0;                             // 自增量ID
    m_unUid = 0;                            // 玩家ID
    m_unFriendId = 0;                       // 好友id
    m_nGrandingId = 0;                      // 段位id
    m_nCurStarSum = 0;                      // 当前星星数量
    m_strNickName = "";                      // 昵称
    m_strHeadPicurl = "";                    // 头像

    m_pOwner = NULL;
    m_bUpdate = FALSE;
}

//OnTimer函数
BOOL CFriendData::OnTimer()
{
    return TRUE;
}

//保存信息
BOOL CFriendData::SaveInfo(BOOL bUpdate /*= TRUE*/)
{
    if (!Database())
    { 
        return FALSE;
    }

    if (bUpdate)
    {
        char szSql[1024];
        memset(szSql, 0, 1024);
        sprintf(szSql, "UPDATE g_friend SET nick_name=%s,head_picurl=%s,granding_id=%d,cur_star_sum=%d WHERE id=%d", 
            m_strNickName.c_str(),m_strHeadPicurl.c_str(),m_nGrandingId,m_nCurStarSum,m_unId);
        if(Database()->UpdateRecord(szSql))
        {
            LOGDEBUG("[CFriendData::SaveInfo] Is Suc..., sql=[%s] \n", szSql);
        }
        else
        {
            LOGDEBUG("[CFriendData::SaveInfo] Is Error..., sql=[%s] \n", szSql);
            return FALSE;
        }
    }
    return TRUE;
} 

BOOL CFriendData::SetUid(UINT nUid, BOOL bUpdate/* = TRUE*/)
{
    m_unUid = nUid;
    return TRUE;
}

BOOL CFriendData::SetFriendId(UINT nFriendId, BOOL bUpdate/* = TRUE*/)
{
	m_unFriendId = nFriendId;
    return TRUE;
}

BOOL CFriendData::SetNickName(string strNickName, BOOL bUpdate/* = TRUE*/)
{
	m_strNickName = strNickName;
    if (!Database())
    { 
         return FALSE;
    }

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_friend SET nick_name=\"")+strNickName+"\" WHERE id="+std::to_string(m_unId);
        //std::string sql=string("UPDATE g_friend SET nick_name=\"")+strNickName+"\" WHERE user_id="+std::to_string(m_unUid)+" AND friend_id="+std::to_string(m_unFriendId);
        Database()->UpdateRecord(sql);
    }
    return TRUE;
}

BOOL CFriendData::SetHeadPicurl(string strHeadPicurl, BOOL bUpdate /*= TRUE*/)
{
    m_strHeadPicurl = strHeadPicurl;
	if (!Database())
    { 
         return FALSE;
    }

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_friend SET head_picurl=\"")+strHeadPicurl+"\" WHERE id="+std::to_string(m_unId);
        //std::string sql=string("UPDATE g_friend SET head_picurl=\"")+strHeadPicurl+"\" WHERE user_id="+std::to_string(m_unUid)+" AND friend_id="+std::to_string(m_unFriendId);
        Database()->UpdateRecord(sql);
    }
    return TRUE;
}

BOOL CFriendData::SetGrandingId(UINT nGrandingId, BOOL bUpdate/* = TRUE*/)
{
    m_nGrandingId = nGrandingId;
    if (!Database())
    { 
         return FALSE;
    }

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_friend SET granding_id=")+std::to_string(nGrandingId)+" WHERE id="+std::to_string(m_unId);
        //std::string sql=string("UPDATE g_friend SET granding_id=")+std::to_string(nGrandingId)+" WHERE user_id="+std::to_string(m_unUid)+" AND friend_id="+std::to_string(m_unFriendId);
        Database()->UpdateRecord(sql);
    }
    return TRUE;
}

BOOL CFriendData::SetCurStarSum(UINT nCurStarSum, BOOL bUpdate/* = TRUE*/)
{
    m_nCurStarSum = nCurStarSum;
    if (!Database())
    { 
         return FALSE;
    }

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_friend SET cur_star_sum=")+std::to_string(nCurStarSum)+" WHERE id="+std::to_string(m_unId);
        //std::string sql=string("UPDATE g_friend SET cur_star_sum=")+std::to_string(nCurStarSum)+" WHERE user_id="+std::to_string(m_unUid)+" AND friend_id="+std::to_string(m_unFriendId);
        Database()->UpdateRecord(sql);
    }
    return TRUE;
}
