//////////////////////////////////////////////////////////////////////
//文件名称：CItemAwardGroupManager.cpp
//功能描述：
//版本说明：CItemAwardGroupManager API
//			
//编写作者：2018.12.19 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#include "ItemAwardGroupManager.h"
#include "ItemAwardGroup.h"
#include "../../DataBase/DBMySql.h"
#include "../I_mydb.h"
#include "../../MsgHandler/AllMsg.h"
#include "../Player.h"


//////////////////////////////////////////////////////////////////////
//常量预定于
char	szItemAwardGroupTable[] = _TBL_ITEMAWARD_GROUP;
CItemAwardGroupManager* g_pItemAwardGroupManager = NULL;

//////////////////////////////////////////////////////////////////////
//构造
CItemAwardGroupManager::CItemAwardGroupManager()
{
	m_mulitmapItemAwardGroupSet.clear();
}

//析构函数
CItemAwardGroupManager::~CItemAwardGroupManager()
{ 
	for (auto iter = m_mulitmapItemAwardGroupSet.begin(); iter != m_mulitmapItemAwardGroupSet.end(); iter++)
    {	
		if(iter->second)
		{
			SAFE_RELEASE(iter->second);
		} 
    }

    m_mulitmapItemAwardGroupSet.clear();
}

BOOL CItemAwardGroupManager::Init()
{
    for (auto iter = m_mulitmapItemAwardGroupSet.begin(); iter != m_mulitmapItemAwardGroupSet.end(); iter++)
    {   
        if(iter->second)
        {
            SAFE_RELEASE(iter->second);
        } 
    }

    m_mulitmapItemAwardGroupSet.clear();

    return this->LoadInfo();
}

//加载itemtype配置文件信息
BOOL CItemAwardGroupManager::LoadInfo() 
{
    if (!Database())
    { 
         return FALSE;
    }

    std::string sql = string("SELECT * FROM g_bumper_item_award_group");  
    MYSQL_RES* result = Database()->Query(sql);
    if(result == NULL)
    {
        LOGERROR("[CItemAwardGroupManager::LoadInfo] is not exist g_bumper_item_award_group  sql error: %s\n", sql.c_str());
        return FALSE;
    }
    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)))
    {    
    	CItemAwardGroup* pItemAwardGroup = CItemAwardGroup::CreateNew();
    	if (pItemAwardGroup)
    	{
            pItemAwardGroup->SetId(SafeAtoi(row[_ITEMAWARD_GROUP_ID]));
    		pItemAwardGroup->SetGroupId(SafeAtoi(row[_ITEMAWARD_GROUP_GROUP_ID]));
            pItemAwardGroup->SetItemType(SafeAtoi(row[_ITEMAWARD_GROUP_ITEMTYPE]));
            pItemAwardGroup->SetItemAmount(SafeAtoi(row[_ITEMAWARD_GROUP_ITEMAMOUNT]));
            pItemAwardGroup->SetRatio(SafeAtoi(row[_ITEMAWARD_GROUP_RATIO]));
            
	        //插入容器
            pair<UINT, CItemAwardGroup*> groupPair(pItemAwardGroup->GetGroupId(), pItemAwardGroup);
	        m_mulitmapItemAwardGroupSet.insert(groupPair);
	        
	        LOGDEBUG("[CItemAwardGroupManager::LoadInfo] size=[%d] Id=[%d] GroupId=[%d] ItemType=[%d] ItemAmount=[%d] Ratio=[%d]", 
            	m_mulitmapItemAwardGroupSet.size(), pItemAwardGroup->GetId(), pItemAwardGroup->GetGroupId(),pItemAwardGroup->GetItemType(), pItemAwardGroup->GetItemAmount(), pItemAwardGroup->GetRatio());
    	}
    }
    mysql_free_result(result);
    result = NULL;
    LOGDEBUG("[CItemAwardGroupManager::LoadInfo] Loading table g_bumper_item_award_group end");
    return TRUE;
    
}

//查询道具接口
CItemAwardGroup* CItemAwardGroupManager::QueryItemAwardGroup(UINT nGroupId)
{
    UINT nRandom = Random(100)+1;
    MULTIMAP_ITEMAWARD_GROUD_PAIR ItemAwardGroupPair =  m_mulitmapItemAwardGroupSet.equal_range(nGroupId);
    while(ItemAwardGroupPair.first != ItemAwardGroupPair.second)//有收到该玩家的操作包
    {
        if (nRandom > ItemAwardGroupPair.first->second->GetRatio())
        {
            return ItemAwardGroupPair.first->second;
        }
    }
    
    return NULL;
}

//奖励道具接口，每条配置单独做概率就算
BOOL CItemAwardGroupManager::AwardItemByAwardGroup(Player* pPlayer, UINT nGroupId, INT nAwardMulti)
{
    CHECKF(pPlayer);

    MULTIMAP_ITEMAWARD_GROUD_PAIR ItemAwardGroupPair =  m_mulitmapItemAwardGroupSet.equal_range(nGroupId);
    while(ItemAwardGroupPair.first != ItemAwardGroupPair.second)//有收到该玩家的操作包
    {
        UINT nRandom = Random(100);
        if (nRandom < ItemAwardGroupPair.first->second->GetRatio())
        {
            CItemDataManager* pItemDataManager = pPlayer->QueryItemDataManager();
            CHECKF(pItemDataManager);

            CItemData* pAwardItem = pItemDataManager->AwardItem(ItemAwardGroupPair.first->second->GetItemType(), ItemAwardGroupPair.first->second->GetItemAmount()*nAwardMulti);
            if (pAwardItem)
            {
                LOGDEBUG("[CItemAwardGroupManager::AwardItemByAwardGroup] Is Suc... pPlayerID=[%d] nId=[%d] nUserId=[%d] nItemType=[%d] nType=[%d] nLifeTime=[%d] nAmount=[%d]",
                    pPlayer->GetUid(),pAwardItem->GetId(), pAwardItem->GetUserId(),pAwardItem->GetItemType(),pAwardItem->GetType(),pAwardItem->GetLifeTime(),pAwardItem->GetAmount());
            }
            else
            {
                 LOGERROR("[CItemAwardGroupManager::AwardItemByAwardGroup] Is Error... pPlayerID=[%d] nGroupId=[%d] nItemType=[%d] nAmount=[%d]",
                    pPlayer->GetUid(), nGroupId, ItemAwardGroupPair.first->second->GetItemType(), ItemAwardGroupPair.first->second->GetItemAmount()*nAwardMulti);
            }
        }
        
        ++ItemAwardGroupPair.first;
    }
    
    return TRUE;
}