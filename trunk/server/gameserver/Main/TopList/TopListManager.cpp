//////////////////////////////////////////////////////////////////////
//文件名称：CTopListManager.cpp
//功能描述：
//版本说明：CTopListManager API
//			
//编写作者：2018.10.10 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#include "../Common/Config.h"
#include "../Common/IniFile.h"
#include "../DataBase/DBMySql.h"
#include "../Protocol/Command.h"
#include "../Protocol/cJSON.h"
#include "TopListManager.h"
//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////
 
//全局变量
CTopListManager* g_pTopListManager = NULL;

CTopListManager::CTopListManager()
{
	m_UpdateTime = NULL;
    m_vRankTopList.clear();
    m_vKillNumTopList.clear();
}

CTopListManager::~CTopListManager()
{
	m_vRankTopList.clear();
    m_vKillNumTopList.clear(); 
}

BOOL CTopListManager::Init()
{
    m_UpdateTime = NULL;
    return (LoadRankTopList() && LoadKillNumTopList());
}
//LOOP 接口
BOOL CTopListManager::Loop()
{
    time_t ltime;
    time(&ltime);
    tm * CurTime = localtime(&ltime);
    if (m_UpdateTime && CurTime->tm_mday != m_UpdateTime->tm_mday)
    {
        LoadRankTopList();
        LoadKillNumTopList();
    }
	
	return TRUE;
	
}

//加载排行榜数据---by Rank
BOOL CTopListManager::LoadRankTopList()
{
	if (!Database())
    { 
         return FALSE;
    }
    //财富排行榜
	string sql = "SELECT head_picurl, nick_name, star_num, granding_id, user_id FROM g_bumper_info ORDER BY granding_id DESC, star_num DESC LIMIT 0, 30";

    #if 0

       _Rank_TOP_LIST_HEAD_PICURL       = 0, 
       _Rank_TOP_LIST_NICKNAME          = 1,
       _Rank_TOP_LIST_STAR_NUM          = 3,
       _Rank_TOP_LIST_GRANDING_ID       = 4,
       _Rank_TOP_LIST_RANK              = 5,
       _Rank_TOP_LIST_IS_RANK           = 6,

    #endif

    MYSQL_RES* result = Database()->Query(sql);
    MYSQL_ROW row;
    RankTopListStruct* pInfo = NULL;
    int cnt = 0;
    while((row = mysql_fetch_row(result)))
    {
        pInfo = new RankTopListStruct;
        cnt++;

        if(row[_Rank_TOP_LIST_HEAD_PICURL] && strlen(row[_Rank_TOP_LIST_HEAD_PICURL]))
        {
            pInfo->m_HeadPicurl = row[_Rank_TOP_LIST_HEAD_PICURL];
        }
        else 
        {
            pInfo->m_HeadPicurl = "default";
        }
        
        if(row[_Rank_TOP_LIST_NICKNAME] && strlen(row[_Rank_TOP_LIST_NICKNAME]))
        {
            pInfo->m_NickName = row[_Rank_TOP_LIST_NICKNAME];
        }
        else 
        {
            pInfo->m_NickName = "default";
        }
        
        pInfo->m_nStarNum = SafeAtoi(row[_Rank_TOP_LIST_STAR_NUM]);
        pInfo->m_nGrandingId = SafeAtoi(row[_Rank_TOP_LIST_GRANDING_ID]);

        pInfo->m_nUserId = SafeAtoi(row[_Rank_TOP_LIST_USER_ID]);

        pInfo->m_nRank = cnt;

        m_vRankTopList.push_back(pInfo);	
        LOGDEBUG("[CTopListManager::LoadRankTopList] size=[%d] UserId=[%d], NickName=[%s] HeadPicurl=[%s] StarNum=[%d] Granding_Id=[%d] Rank=[%d] Is_Rank=[%d]\n", 
                  m_vRankTopList.size(), pInfo->m_nUserId, pInfo->m_NickName.c_str(), pInfo->m_HeadPicurl.c_str(), 
                  pInfo->m_nStarNum, pInfo->m_nGrandingId, pInfo->m_nRank, pInfo->m_nIsRank);
    }

    time_t ltime;
    time(&ltime);
    m_UpdateTime = localtime(&ltime);
    char    buf[64] = "";                
    sprintf(buf, "%d-%02d-%02d %02d:%02d:%02d", m_UpdateTime->tm_year+1900, m_UpdateTime->tm_mon, m_UpdateTime->tm_mday, m_UpdateTime->tm_hour, m_UpdateTime->tm_min, m_UpdateTime->tm_sec);

    mysql_free_result(result);
    result = NULL;
    LOGDEBUG("[CTopListManager::LoadRankTopList] Loading table g_bumper_info end m_UpdateTime=[%s]", buf);
    return TRUE;
}


BOOL CTopListManager::LoadKillNumTopList()
{
    if (!Database())
    { 
         return FALSE;
    }

    #if 1

    string sql = "SELECT head_picurl, nick_name, kill_num, star_num, user_id FROM g_bumper_info ORDER BY granding_id DESC, star_num DESC LIMIT 0, 30";

#if 0




    _KILLNUM_TOP_LIST_HEAD_PICURL     =  0, 
    _KILLNUM_TOP_LIST_NICKNAME        =  1,
    _KILLNUM_TOP_LIST_KILL_NUM        =  2,
    _KILLNUM_TOP_LIST_STAR_NUM        =  3,
     _KILLNUM_TOP_LIST_USER_ID        = 4,

#endif


    
    MYSQL_RES* result = Database()->Query(sql);
    MYSQL_ROW row;
    KillNumTopListStruct* pInfo = NULL;
    int cnt=0;

    while((row = mysql_fetch_row(result)))
    {

        pInfo = new KillNumTopListStruct;
        cnt++;
        
       if(row[_KILLNUM_TOP_LIST_HEAD_PICURL] && strlen(row[_KILLNUM_TOP_LIST_HEAD_PICURL]))
        {
            pInfo->m_HeadPicurl = row[_KILLNUM_TOP_LIST_HEAD_PICURL];
        }
        else 
        {
            pInfo->m_HeadPicurl = "default";
        }
        
        if(row[_KILLNUM_TOP_LIST_NICKNAME] && strlen(row[_KILLNUM_TOP_LIST_NICKNAME]))
        {
            pInfo->m_NickName = row[_KILLNUM_TOP_LIST_NICKNAME];
        }
        else 
        {
            pInfo->m_NickName = "default";
        }

         pInfo->m_nKillNum = SafeAtoi(row[_KILLNUM_TOP_LIST_KILL_NUM]);

         pInfo->m_nStarNum = SafeAtoi(row[_KILLNUM_TOP_LIST_STAR_NUM]);
        
       pInfo->m_nUserId = SafeAtoi(row[_Rank_TOP_LIST_USER_ID]);

        pInfo->m_nRank = cnt;
        m_vKillNumTopList.push_back(pInfo);

        LOGDEBUG("[CTopListManager::LoadKillNumTopList] size=[%d] UserId=[%d], NickName=[%s] HeadPicurl=[%s] StarNum=[%d]\n", 
                  m_vKillNumTopList.size(), pInfo->m_nUserId, pInfo->m_NickName.c_str(), pInfo->m_HeadPicurl.c_str(), 
                  pInfo->m_nStarNum);

         
    }

    time_t ltime;
    time(&ltime);
    m_UpdateTime = localtime(&ltime);
    char    buf[64] = "";                
    sprintf(buf, "%d-%02d-%02d %02d:%02d:%02d", m_UpdateTime->tm_year+1900, m_UpdateTime->tm_mon, m_UpdateTime->tm_mday, m_UpdateTime->tm_hour, m_UpdateTime->tm_min, m_UpdateTime->tm_sec);

    mysql_free_result(result);
    result = NULL;
    LOGDEBUG("[CTopListManager::LoadKillNumTopList] Loading table g_bumper_info end m_UpdateTime=[%s]", buf);

    #endif
    return TRUE;
}






