//////////////////////////////////////////////////////////////////////
//文件名称：CLimitTimeAwardCfgManager.cpp
//功能描述：
//版本说明：CLimitTimeAwardCfgManager API
//			
//编写作者：2018.12.26 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#include "ItemAwardManager.h"
#include "ItemAward.h"
#include "../../DataBase/DBMySql.h"
#include "../I_mydb.h"
#include "../../MsgHandler/AllMsg.h"
#include "../Player.h"
#include "ItemAwardGroupManager.h"
#include "../Item/ItemType.h"
#include "../Item/ItemData.h"
#include "LimitTimeAwardCfgManager.h"


//////////////////////////////////////////////////////////////////////
//常量预定于
char	szLimitTimeAwardCfgTable[] = _TBL_LIMIT_TIME_AWARD_CFG;
CLimitTimeAwardCfgManager* g_pLimitTimeAwardCfgManager = NULL;

//////////////////////////////////////////////////////////////////////
//构造
CLimitTimeAwardCfgManager::CLimitTimeAwardCfgManager()
{
	m_mapLimitTimeAwardCfgSet.clear();
}

//析构函数
CLimitTimeAwardCfgManager::~CLimitTimeAwardCfgManager()
{ 
	for (auto iter = m_mapLimitTimeAwardCfgSet.begin(); iter != m_mapLimitTimeAwardCfgSet.end(); iter++)
    {	
		if(iter->second)
		{
			SAFE_RELEASE(iter->second);
		} 
    }

    m_mapLimitTimeAwardCfgSet.clear();
}

BOOL CLimitTimeAwardCfgManager::Init()
{
    for (auto iter = m_mapLimitTimeAwardCfgSet.begin(); iter != m_mapLimitTimeAwardCfgSet.end(); iter++)
    {   
        if(iter->second)
        {
            SAFE_RELEASE(iter->second);
        } 
    }

    m_mapLimitTimeAwardCfgSet.clear();

    return this->LoadInfo();
}

//加载itemtype配置文件信息
BOOL CLimitTimeAwardCfgManager::LoadInfo() 
{
    if (!Database())
    { 
         return FALSE;
    }

    std::string sql = string("SELECT * FROM g_bumper_limit_time_award_cfg");  
    MYSQL_RES* result = Database()->Query(sql);
    if(result == NULL)
    {
        LOGERROR("[CLimitTimeAwardCfgManager::LoadInfo] sql error: %s\n", sql.c_str());
        return FALSE;
    }
    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)))
    {    
    	CLimitTimeAwardCfg* pInfo = CLimitTimeAwardCfg::CreateNew();
    	if (pInfo)
    	{
    		pInfo->SetId(SafeAtoi(row[_LIMIT_TIME_AWARD_CFG_ID]));            
            pInfo->SetNextId(SafeAtoi(row[_LIMIT_TIME_AWARD_CFG_NEXT_ID]));
            pInfo->SetLimitTime(SafeAtoi(row[_LIMIT_TIME_AWARD_CFG_LIMIT_TIME]));
            pInfo->SetAwardId(SafeAtoi(row[_LIMIT_TIME_AWARD_CFG_AWARD_ID]));

	        //插入容器
	        m_mapLimitTimeAwardCfgSet[pInfo->GetId()] = pInfo;
	        
	        LOGDEBUG("[CLimitTimeAwardCfgManager::LoadInfo] size=[%d] Id=[%d] NextId=[%d] LimitTime=[%d] AwardId=[%d]", 
            	m_mapLimitTimeAwardCfgSet.size(), pInfo->GetId(), pInfo->GetNextId(), pInfo->GetLimitTime(), pInfo->GetAwardId());
    	}
    }
    mysql_free_result(result);
    result = NULL;
    LOGDEBUG("[CLimitTimeAwardCfgManager::LoadInfo] Loading table g_bumper_limit_time_award_cfg end");
    return TRUE;
    
}

//查询道具接口
CLimitTimeAwardCfg* CLimitTimeAwardCfgManager::QueryLimitTimeAwardCfg(UINT nAwardId)
{
    for (auto iter = m_mapLimitTimeAwardCfgSet.begin(); iter != m_mapLimitTimeAwardCfgSet.end(); iter++)
    {   
        //找到对应玩家
        if((iter->second)->GetId() ==  nAwardId)
        {
            return (iter->second);                 
        }
    }
    return NULL;
}

//发放奖励给某人
BOOL CLimitTimeAwardCfgManager::AwardItemToPlayer(Player* pPlayer, UINT nAwardId, INT nAwardMulti/*=1*/)
{
    CHECKF(pPlayer);

    CLimitTimeAwardCfg* pInfo = this->QueryLimitTimeAwardCfg(nAwardId);
    if (pInfo)
    {
        UINT nAwardId = pInfo->GetAwardId();
        CItemAwardManager* pItemAwardManager = QueryItemAwardManager();
        if (!pItemAwardManager)
        {

        }

        if(!pItemAwardManager->AwardItemToPlayer(pPlayer, nAwardId, nAwardMulti))
        {

        }
    }

    return TRUE;
}
