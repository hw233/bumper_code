//////////////////////////////////////////////////////////////////////
//文件名称：CItemAwardManager.cpp
//功能描述：
//版本说明：CItemAwardManager API
//			
//编写作者：2018.12.13 yanfeng.hu
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
#include "ItemType.h"
#include "ItemData.h"


//////////////////////////////////////////////////////////////////////
//常量预定于
char	szItemAwardTable[] = _TBL_ITEMAWARD;
CItemAwardManager* g_pItemAwardManager = NULL;

//////////////////////////////////////////////////////////////////////
//构造
CItemAwardManager::CItemAwardManager()
{
	m_mapItemAwardSet.clear();
}

//析构函数
CItemAwardManager::~CItemAwardManager()
{ 
	for (auto iter = m_mapItemAwardSet.begin(); iter != m_mapItemAwardSet.end(); iter++)
    {	
		if(iter->second)
		{
			SAFE_RELEASE(iter->second);
		} 
    }

    m_mapItemAwardSet.clear();
}

BOOL CItemAwardManager::Init()
{
    for (auto iter = m_mapItemAwardSet.begin(); iter != m_mapItemAwardSet.end(); iter++)
    {   
        if(iter->second)
        {
            SAFE_RELEASE(iter->second);
        } 
    }

    m_mapItemAwardSet.clear();

    return this->LoadInfo();
}

//加载itemtype配置文件信息
BOOL CItemAwardManager::LoadInfo() 
{
    if (!Database())
    { 
         return FALSE;
    }

    std::string sql = string("SELECT * FROM g_bumper_item_award");  
    MYSQL_RES* result = Database()->Query(sql);
    if(result == NULL)
    {
        LOGERROR("[CItemAwardManager::LoadInfo] is not exist g_bumper_item_award sql error: %s\n", sql.c_str());
        return FALSE;
    }
    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)))
    {    
    	CItemAward* pItemAward = CItemAward::CreateNew();
    	if (pItemAward)
    	{
    		pItemAward->SetId(SafeAtoi(row[_ITEMAWARD_ID]));
            
            pItemAward->SetGold(SafeAtoi(row[_ITEMAWARD_GOLD]));
            pItemAward->SetRatio0(SafeAtoi(row[_ITEMAWARD_RATIO0]));
            pItemAward->SetGroup1(SafeAtoi(row[_ITEMAWARD_GROUP1]));
            pItemAward->SetRatio1(SafeAtoi(row[_ITEMAWARD_RATIO1]));
            pItemAward->SetGroup2(SafeAtoi(row[_ITEMAWARD_GROUP2]));
            pItemAward->SetRatio2(SafeAtoi(row[_ITEMAWARD_RATIO2]));
            pItemAward->SetGroup3(SafeAtoi(row[_ITEMAWARD_GROUP3]));
            pItemAward->SetRatio3(SafeAtoi(row[_ITEMAWARD_RATIO3]));
            pItemAward->SetGroup4(SafeAtoi(row[_ITEMAWARD_GROUP4]));
            pItemAward->SetRatio4(SafeAtoi(row[_ITEMAWARD_RATIO4]));
            pItemAward->SetGroup5(SafeAtoi(row[_ITEMAWARD_GROUP5]));
            pItemAward->SetRatio5(SafeAtoi(row[_ITEMAWARD_RATIO5]));

	        //插入容器
	        m_mapItemAwardSet[pItemAward->GetId()] = pItemAward;
	        
	        LOGDEBUG("[CItemAwardManager::LoadInfo] size=[%d] Id=[%d] Gold=[%d] Ratio0=[%d] Group0=[%d] Ratio1=[%d] Group0=[%d] Ratio2=[%d] Group0=[%d] \
                Ratio3=[%d] Group0=[%d] Ratio4=[%d] Group0=[%d] Ratio5=[%d]", 
            	m_mapItemAwardSet.size(), pItemAward->GetId(), pItemAward->GetGold(), pItemAward->GetRatio0(), pItemAward->GetGroup1(), pItemAward->GetRatio1(), 
                pItemAward->GetGroup2(),pItemAward->GetRatio2(), pItemAward->GetGroup3(), pItemAward->GetRatio3(), pItemAward->GetGroup4(), pItemAward->GetRatio4(), 
                pItemAward->GetGroup5(),pItemAward->GetRatio5());
    	}
    }
    mysql_free_result(result);
    result = NULL;
    LOGDEBUG("[CItemAwardManager::LoadInfo] Loading table g_bumper_item_award end");
    return TRUE;
    
}

//查询道具接口
CItemAward* CItemAwardManager::QueryItemAward(UINT nAwardId)
{
    for (auto iter = m_mapItemAwardSet.begin(); iter != m_mapItemAwardSet.end(); iter++)
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
BOOL CItemAwardManager::AwardItemToPlayer(Player* pPlayer, UINT nAwardId, INT nAwardMulti/*=1*/)
{
    CHECKF(pPlayer);

    CItemAward* pAwardInfo = this->QueryItemAward(nAwardId);
    if (pAwardInfo)
    {
        // UINT nAwardGold = pAwardInfo->GetGold() * nAwardMulti;
        // UINT nRatio0 = pAwardInfo->GetRatio0();
        // UINT nRandom = Random(100);
        // //奖励金币
        // if (nRandom < nRatio0)
        // {
        //     //更新玩家金币
        //     pPlayer->SetGoldSum(pPlayer->GetGoldSum() + nAwardGold);

        //     //发送玩家属性同步包
        //     CMsgUserAttrib msg;
        //     IF_OK(msg.CreateMsg(_ACT_USERATTRIB_GOLD_SUM, pPlayer->GetGoldSum()))
        //     {
        //         pPlayer->SendMsgToClient(&msg);
        //     }
        // }
        // LOGDEBUG("[CItemAwardManager::AwardItemToPlayer] nRandom=[%d] nRatio0=[%d]", nRandom, nRatio0);

        //金币转换道具--通过同步道具包信息
        UINT nAwardGold = pAwardInfo->GetGold() * nAwardMulti;
        UINT nRatio0 = pAwardInfo->GetRatio0();
        UINT nRandom = Random(100);
        //奖励金币
        if (nRandom < nRatio0)
        {
            CItemDataManager* pItemDataManager = pPlayer->QueryItemDataManager();
            if (pItemDataManager)
            {
                //更新玩家金币
                pItemDataManager->AwardItem(130010, nAwardGold);
            }
        }
        LOGDEBUG("[CItemAwardManager::AwardItemToPlayer] nRandom=[%d] nRatio0=[%d] nAwardItemType=[130010] nAwardGold=[%d]", nRandom, nRatio0, nAwardGold);
        
        CItemAwardGroupManager* pItemAwardGroupManager = QueryItemAwardGroupManager();
        if (!pItemAwardGroupManager)
        {
            LOGERROR("[CItemAwardManager::AwardItemToPlayer] pItemAwardGroupManager == NULL");
            return FALSE;
        }

        //奖励组-道具奖励
        //#define GET_METHOD(p, f, i) (p->f##i())
        UINT nGroup = 0;
        UINT nRatio = 0;
        const UINT nAwardGroupCount = 5;    //5个奖励组
        for (UINT i = 1; i <= nAwardGroupCount; ++i)
        {
            nRandom = Random(100);

            //nGroup = GET_METHOD(pAwardInfo, GetGroup, 1);
            //nRatio = GET_METHOD(pAwardInfo, GetRatio, 1);
            
            //奖励组-1
            if (i == 1)
            {
                nGroup = pAwardInfo->GetGroup1();
                nRatio = pAwardInfo->GetRatio1();
            }
            //奖励组-2
            else if (i == 2)
            {
                nGroup = pAwardInfo->GetGroup2();
                nRatio = pAwardInfo->GetRatio2();
            }
            //奖励组-3
            else if (i == 3)
            {
                nGroup = pAwardInfo->GetGroup3();
                nRatio = pAwardInfo->GetRatio3();
            }
            //奖励组-4
            else if (i == 4)
            {
                nGroup = pAwardInfo->GetGroup4();
                nRatio = pAwardInfo->GetRatio4();
            }
            //奖励组-5
            else //if (i == 5)
            {
                nGroup = pAwardInfo->GetGroup5();
                nRatio = pAwardInfo->GetRatio5();
            }
            
            //奖励道具组
            if (nRandom < nRatio)
            {
                //奖励发放
                if (!pItemAwardGroupManager->AwardItemByAwardGroup(pPlayer, nGroup, nAwardMulti))
                {
                    LOGERROR("[CItemAwardManager::AwardItemToPlayer] pItemAwardGroup Is Error pPlayerID=[%d] nGroup=[%d] nRatio=[%d]", pPlayer->GetUid(), nGroup, nRatio);
                    continue;
                }
            }
        }

        //#undef GET_METHOD
        return TRUE;
    }
    else//pAwardInfo
    {
        LOGERROR("[CItemAwardManager::AwardItemToPlayer] pAwardInfo == NULL nAwardId=[%d]", nAwardId);
        return FALSE;
    }
}
