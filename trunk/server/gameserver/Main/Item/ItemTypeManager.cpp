//////////////////////////////////////////////////////////////////////
//文件名称：CItemTypeManager.cpp
//功能描述：
//版本说明：CItemTypeManager API
//			
//编写作者：2018.12.13 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#include "ItemTypeManager.h"
#include "ItemType.h"
#include "../../DataBase/DBMySql.h"
#include "../I_mydb.h"
#include "../Player.h"


//////////////////////////////////////////////////////////////////////
//常量预定于
char	szItemTypeTable[] = _TBL_ITEMTYPE;
CItemTypeManager* g_pItemTypeManager = NULL;

//////////////////////////////////////////////////////////////////////
//构造
CItemTypeManager::CItemTypeManager()
{
	m_mapItemTypeSet.clear();
}

//析构函数
CItemTypeManager::~CItemTypeManager()
{ 
	for (auto iter = m_mapItemTypeSet.begin(); iter != m_mapItemTypeSet.end(); iter++)
    {	
		if(iter->second)
		{
			SAFE_RELEASE(iter->second);
		} 
    }

    m_mapItemTypeSet.clear();
}

BOOL CItemTypeManager::Init()
{
    for (auto iter = m_mapItemTypeSet.begin(); iter != m_mapItemTypeSet.end(); iter++)
    {   
        if(iter->second)
        {
            SAFE_RELEASE(iter->second);
        } 
    }

    m_mapItemTypeSet.clear();

    return this->LoadInfo();
}

//加载itemtype配置文件信息
BOOL CItemTypeManager::LoadInfo() 
{
    if (!Database())
    { 
        return FALSE;
    }

    std::string sql = string("SELECT * FROM g_bumper_itemtype");  
    MYSQL_RES* result = Database()->Query(sql);
    if(result == NULL)
    {
        LOGERROR("[CItemTypeManager::LoadInfo] sql error: %s\n", sql.c_str());
        return FALSE;
    }
    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)))
    {    
    	CItemType* pItemType = CItemType::CreateNew();
    	if (pItemType)
    	{
    		pItemType->SetItemType(SafeAtoi(row[_ITEMTYPE_ITEMTYPE]));
            if(row[_ITEMTYPE_NAME] && strlen(row[_ITEMTYPE_NAME]))
            {
                pItemType->SetName(row[_ITEMTYPE_NAME]);
            }
            else 
            {
                pItemType->SetName("default");
            }
            if(row[_ITEMTYPE_DESCRIBE] && strlen(row[_ITEMTYPE_DESCRIBE]))
            {
                pItemType->SetDescribe(row[_ITEMTYPE_DESCRIBE]);
            }
            else 
            {
                pItemType->SetDescribe("discribe");
            }
            pItemType->SetType(SafeAtoi(row[_ITEMTYPE_TYPE]));
            //pItemType->SetINumber(SafeAtoi(row[_ITEMTYPE_INUMBER]));
            //pItemType->SetUpNumber(SafeAtoi(row[_ITEMTYPE_UPNUMBER]));
            //pItemType->SetILength(SafeAtoi(row[_ITEMTYPE_ILENGTH]));
            //pItemType->SetUpLength(SafeAtoi(row[_ITEMTYPE_UPLENGTH]));
            //pItemType->SetISpeed(SafeAtoi(row[_ITEMTYPE_ISPEED]));
            //pItemType->SetUpSpeed(SafeAtoi(row[_ITEMTYPE_UPSPEED]));
            pItemType->SetLifeTime(SafeAtoi(row[_ITEMTYPE_LIFE_TIME]));
            pItemType->SetVasGold(SafeAtoi(row[_ITEMTYPE_VAS_GOLD]));
            pItemType->SetVasItemType(SafeAtoi(row[_ITEMTYPE_VAS_ITEMTYPE]));
            //pItemType->SetFrangmente(SafeAtoi(row[_ITEMTYPE_FRAGMENT]));
            //pItemType->SetTargetSkinId(SafeAtoi(row[_ITEMTYPE_TARGET_SKIN_ID]));
            pItemType->SetAmount(SafeAtoi(row[_ITEMTYPE_AMOUNT]));
            pItemType->SetAction(SafeAtoi(row[_ITEMTYPE_ACTION]));

	        //插入容器
	        m_mapItemTypeSet[pItemType->GetItemType()] = pItemType;
	        
	        LOGDEBUG("[CItemTypeManager::LoadInfo] size=[%d] ItemType=[%d] Name=[%s] Describe=[%s] Type=[%d]\
                LifeTime=[%d] VasGold=[%d] VasItemType=[%d] Amount=[%d] action=[%d]", 
            	m_mapItemTypeSet.size(), pItemType->GetItemType(), pItemType->GetName(), pItemType->GetDescribe(), pItemType->GetType(), pItemType->GetLifeTime(), pItemType->GetVasGold(), 
                pItemType->GetVasItemType(), pItemType->GetAmount(), pItemType->GetAction());
    	}
    }
    mysql_free_result(result);
    result = NULL;
    LOGDEBUG("[CItemTypeManager::LoadInfo] Loading table g_bumper_itemtype end");
    return TRUE;
    
}

//查询道具接口
CItemType* CItemTypeManager::QueryItemType(UINT nItemType)
{
    for (auto iter = m_mapItemTypeSet.begin(); iter != m_mapItemTypeSet.end(); iter++)
    {   
        //找到对应玩家
        if((iter->second)->GetItemType() ==  nItemType)
        {
            return (iter->second);                 
        }
    }
    return NULL;
}

//查询某个玩家还未获得的道具
CItemType* CItemTypeManager::QueryNoExistItemType(Player* pPlayer, UINT nType)
{
    LOGDEBUG("[CItemTypeManager::QueryNoExistItemType] nType=[%d]", nType);

    CHECKF(pPlayer);
    for (auto iter = m_mapItemTypeSet.begin(); iter != m_mapItemTypeSet.end(); iter++)
    {   
        if (iter->second)
        {
            //同一类型的道具
            //道具类型为: ITEM_TYPE_CAR_FACE : 10 末尾数>0为限时道具
            if((iter->second)->GetType() == nType && (iter->second)->GetItemType()%10 > 0)
            {
                CItemDataManager* pItemDataManager = pPlayer->QueryItemDataManager();
                if (pItemDataManager)
                {
                    CItemData* pItemData = pItemDataManager->QueryItemTypeData((iter->second)->GetItemType());
                    if (!pItemData) //如果没有此道具
                    {
                        //在查找永久的有没有
                        pItemData = pItemDataManager->QueryItemTypeData(((iter->second)->GetItemType()/10)*10);
                        if (!pItemData)
                        {
                            LOGDEBUG("[CItemTypeManager::QueryNoExistItemType] Not This ItemType Will SetShowItemType nItemType=[%d] ", (iter->second)->GetItemType());
                            return (iter->second); 
                        }
                    }
                }
            }
        }
        else
        {
            LOGWARNING("[CItemTypeManager::QueryNoExistItemType] iter->second==NULL nType=[%d]", nType);
            continue;
        }
        
    }


    return NULL;
}
