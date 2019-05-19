//////////////////////////////////////////////////////////////////////
//文件名称：COnLineAwardCfgManager.cpp
//功能描述：
//版本说明：COnLineAwardCfgManager API
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
#include "OnLineAwardCfgManager.h"


//////////////////////////////////////////////////////////////////////
//常量预定于
char	szOnLineAwardCfgTable[] = _TBL_ONLINE_AWARD_CFG;
COnLineAwardCfgManager* g_pOnLineAwardCfgManager = NULL;

//////////////////////////////////////////////////////////////////////
//构造
COnLineAwardCfgManager::COnLineAwardCfgManager()
{
	m_mapOnLineAwardCfgSet.clear();
}

//析构函数
COnLineAwardCfgManager::~COnLineAwardCfgManager()
{ 
	for (auto iter = m_mapOnLineAwardCfgSet.begin(); iter != m_mapOnLineAwardCfgSet.end(); iter++)
    {	
		if(iter->second)
		{
			SAFE_RELEASE(iter->second);
		} 
    }

    m_mapOnLineAwardCfgSet.clear();
}

BOOL COnLineAwardCfgManager::Init()
{
    for (auto iter = m_mapOnLineAwardCfgSet.begin(); iter != m_mapOnLineAwardCfgSet.end(); iter++)
    {   
        if(iter->second)
        {
            SAFE_RELEASE(iter->second);
        } 
    }

    m_mapOnLineAwardCfgSet.clear();

    return this->LoadInfo();
}

//加载itemtype配置文件信息
BOOL COnLineAwardCfgManager::LoadInfo() 
{
    if (!Database())
    { 
         return FALSE;
    }

    std::string sql = string("SELECT * FROM g_bumper_online_award");  
    MYSQL_RES* result = Database()->Query(sql);
    if(result == NULL)
    {
        LOGERROR("[COnLineAwardCfgManager::LoadInfo] is not exist g_bumper_online_award sql error: %s\n", sql.c_str());
        return FALSE;
    }
    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)))
    {    
    	COnLineAwardCfg* pInfo = COnLineAwardCfg::CreateNew();
    	if (pInfo)
    	{
    		pInfo->SetId(SafeAtoi(row[_ONLINE_AWARD_CFG_ID]));            
            pInfo->SetNextId(SafeAtoi(row[_ONLINE_AWARD_CFG_NEXT_ID]));
            pInfo->SetItemType(SafeAtoi(row[_ONLINE_AWARD_CFG_ITEM_TYPE]));
            pInfo->SetItemAmount(SafeAtoi(row[_ONLINE_AWARD_CFG_ITEM_AMOUNT]));
            pInfo->SetIntervalTime(SafeAtoi(row[_ONLINE_AWARD_CFG_INTERVAL_TIME]));
            pInfo->SetAwardId(SafeAtoi(row[_ONLINE_AWARD_CFG_AWARD_ID]));

	        //插入容器
	        m_mapOnLineAwardCfgSet[pInfo->GetId()] = pInfo;
	        
	        LOGDEBUG("[COnLineAwardCfgManager::LoadInfo] size=[%d] Id=[%d] NextId=[%d] ItemTyp=[%d] ItemAmount=[%d] IntervalTime=[%d] AwardId=[%d]", 
            	m_mapOnLineAwardCfgSet.size(), pInfo->GetId(), pInfo->GetNextId(), pInfo->GetItemType(), pInfo->GetItemAmount(), pInfo->GetIntervalTime(), pInfo->GetAwardId());
    	}
    }
    mysql_free_result(result);
    result = NULL;
    LOGDEBUG("[COnLineAwardCfgManager::LoadInfo] Loading table g_bumper_online_award end");
    return TRUE;
    
}

//查询道具接口
COnLineAwardCfg* COnLineAwardCfgManager::QueryOnLineAwardCfg(UINT nId)
{
    for (auto iter = m_mapOnLineAwardCfgSet.begin(); iter != m_mapOnLineAwardCfgSet.end(); iter++)
    {   
        //找到对应玩家
        if((iter->second)->GetId() ==  nId)
        {
            return (iter->second);                 
        }
    }
    return NULL;
}

//发放奖励给某人
BOOL COnLineAwardCfgManager::AwardItemToPlayer(Player* pPlayer, UINT nId, INT nAwardMulti/*=1*/)
{
    CHECKF(pPlayer);

    COnLineAwardCfg* pInfo = this->QueryOnLineAwardCfg(nId);
    if (pInfo)
    {
        //奖励配置的一个显示道具
        UINT nItemType = pInfo->GetItemType();
        UINT nItemAmount = pInfo->GetItemAmount();
        if (nItemType != 0 && nItemAmount != 0)
        {
            //发放奖励
            CItemDataManager* pItemDataManager = pPlayer->QueryItemDataManager();
            if (pItemDataManager)
            {
                if(!pItemDataManager->AwardItem(nItemType, nItemAmount))
                {
                    LOGERROR("[COnLineAwardCfgManager::AwardItemToPlayer] AwardItem Is Error pPlayerID=[%d] nItemType=[%d] nItemAmount=[%d]", pPlayer->GetUid(), nItemType, nItemAmount);
                    return FALSE;
                }
            }
            else//pItemAwardManager
            {
                LOGERROR("[COnLineAwardCfgManager::AwardItemToPlayer] pItemDataManager == NULL");
                return FALSE;
            }
        }

        //查看是否配置了awardid
        UINT nAwardId = pInfo->GetAwardId();
        if (nAwardId > 0)
        {
           CItemAwardManager* pItemAwardManager = QueryItemAwardManager();
            if (!pItemAwardManager)
            {
                LOGERROR("[COnLineAwardCfgManager::AwardItemToPlayer] pItemAwardManager == NULL");
                return FALSE;
            }

            //奖励组奖励道具
            if(!pItemAwardManager->AwardItemToPlayer(pPlayer, nAwardId))
            {
                LOGERROR("[COnLineAwardCfgManager::AwardItemToPlayer] AwardItemToPlayer Is Error pPlayerID=[%d] nItemType=[%d] nItemAmount=[%d]", pPlayer->GetUid(), nItemType, nItemAmount);
                return FALSE;
            }
        }
        
    }

    return TRUE;
}
