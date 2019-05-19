//////////////////////////////////////////////////////////////////////
//文件名称：CItemDataManager.cpp
//功能描述：
//版本说明：CItemDataManager API
//			
//编写作者：2018.12.13 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#include "ItemDataManager.h"
#include "ItemType.h"
#include "../../DataBase/DBMySql.h"
#include "../I_mydb.h"
#include "../Player.h"
#include "ItemTypeManager.h"
#include "../../MsgHandler/AllMsg.h"


//////////////////////////////////////////////////////////////////////
//常量预定于
char	szItemDatTable[] = _TBL_ITEM;

//////////////////////////////////////////////////////////////////////
//构造
CItemDataManager::CItemDataManager()
{
    this->Init();
}

//析构函数
CItemDataManager::~CItemDataManager()
{ 
    this->CleanUp();
}

//类构造函数调用-内部使用
VOID CItemDataManager::Init()
{
    this->Clear();
    this->SetOwner(NULL);
}

//用于player类中CleanUp()调用
VOID CItemDataManager::CleanUp() 
{ 
    this->Clear();
    this->SetOwner(NULL);
}

//清理道具管理器-内部使用
VOID CItemDataManager::Clear() 
{ 
    for (auto iter = m_mapItemDataSet.begin(); iter != m_mapItemDataSet.end(); iter++)
    {   
        if(iter->second)
        {
            SAFE_RELEASE(iter->second);
        } 
    }

    m_mapItemDataSet.clear();
}

//OnTimer函数
BOOL CItemDataManager::OnTimer() 
{ 
    //检查道具是否超时
    //time_t TT = 0;
    for (auto iter = m_mapItemDataSet.begin(); iter != m_mapItemDataSet.end(); iter++)
    {   
        CItemData* pItem = iter->second;
        if(pItem && pItem->GetItemType()%10 > 0)                            //尾数 > 0 为限时道具
        {
            //检查道具的剩余时间是否超时
            UINT nRemainTime = pItem->GetLifeTime();                        //查看剩余时间戳
            //BOOL IsActive = pItem->QueryLeftTimeOut().IsActive();
            if (!pItem->QueryLeftTimeOut().IsActive() && nRemainTime > 0)               //计时器未启动，则进行启动定时器
            {
                //启动定时器
                pItem->QueryLeftTimeOut().Startup(nRemainTime);
            }
            else                                                          //计时器启动，判断倒计时结束
            {
                if (pItem->QueryLeftTimeOut().TimeOver())   //超时
                {
                    //设置道具倒计时时间戳
                    pItem->SetLifeTime(0);

                    //同步前端道具属性信息
                    CMsgItemAttrib msg;
                    IF_OK(msg.CreateMsg(_ITEMATTRIB_LIFE_TIME, pItem->GetId(), 0 ))
                    {
                        m_pOwner->SendMsgToClient(&msg);
                    }

                    //数据库删除道具记录
                    // UINT nItemId = pItem->GetId();
                    // if(!this->DeleteRecord(nItemId))
                    // {
                    //     LOGERROR("[CItemDataManager::OnTimer] DeleteRecord Is Error...nItemId=[%d]", nItemId);
                    //     continue;
                    // }

                    
                    // The truth is that ‘erase’ in associative containers doesn’t invalidate any iterators except those that point to elements being erased (that’s also true for ’sid::list’). 
                    // For this reason, you don’t really need ‘map::erase’ to return an iterator. Just do this       
                    
                    // //释放item内存
                    // SAFE_RELEASE(iter->second);
                    // //管理器删除元素
                    // m_mapItemDataSet.erase(iter++);
                    // continue;

                    //同步玩家道具删除信息
                    //msgItem
                    //LOGDEBUG("[CItemDataManager::OnTimer] Delete One Item Is Suc...nItemId=[%d]", nItemId);
                }
            }
            
            //TT = pItem->QueryLeftTimeOut().GetUpdateTime();
        } 

        //迭代器++
        //++iter;
    }
    return TRUE;
}

//加载item信息
BOOL CItemDataManager::LoadInfo()
{
    CHECKF(m_pOwner);

    if (!Database())
    { 
         return FALSE;
    }

    //执行一次清空操作
    this->Clear();

    char szSql[1024];
    memset(szSql, 0, 1024);
    sprintf(szSql, "SELECT * FROM g_bumper_item WHERE user_id=%d", m_pOwner->GetUid());    
    MYSQL_RES* result = Database()->Query(szSql);
    if(result == NULL)
    {
        LOGERROR("[CItemDataManager::LoadInfo] szSql error: %s\n", szSql);
        return FALSE;
    }
    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)))
    {    
        CItemData* pItem = CItemData::CreateNew();
        if (pItem)
        {
            pItem->SetId(SafeAtoi(row[_ITEMDATA_ID]));  
            pItem->SetUserId(SafeAtoi(row[_ITEMDATA_USER_ID])); 
            pItem->SetItemType(SafeAtoi(row[_ITEMDATA_ITEMTYPE]));            
            pItem->SetType(SafeAtoi(row[_ITEMDATA_TYPE]));
            pItem->SetLifeTime(SafeAtoi(row[_ITEMDATA_LIFE_TIME]), FALSE, TRUE);
            pItem->SetAmount(SafeAtoi(row[_ITEMDATA_AMOUNT]), FALSE, TRUE);

            pItem->SetOwner(m_pOwner);

            //插入容器
            m_mapItemDataSet[pItem->GetId()] = pItem;
            
            LOGDEBUG("[CItemDataManager::LoadInfo] size=[%d] Id=[%d] UserId=[%d] ItemType=[%d] Type=[%d] LifeTime=[%d] Amount=[%d]", 
                m_mapItemDataSet.size(), pItem->GetId(), pItem->GetUserId(), pItem->GetItemType(), pItem->GetType(), pItem->GetLifeTime(), pItem->GetAmount());
        }
    }
    mysql_free_result(result);
    result = NULL;
    LOGDEBUG("[CItemDataManager::LoadInfo] Loading table g_bumper_item end");
    return TRUE;
    
}

//保存管理器内部数据操作，内部调用
BOOL CItemDataManager::SaveInfo(BOOL bUpdate/* = TRUE*/) 
{ 
    //检查道具
    for (auto iter = m_mapItemDataSet.begin(); iter != m_mapItemDataSet.end(); )
    {   
        if(iter->second)                            //尾数 > 0 为限时道具
        {
            //设置道具倒计时时间戳
            (iter->second)->SaveInfo(bUpdate);
        } 

        //迭代器++
        ++iter;
    }
    return TRUE;
}

//奖励道具
CItemData* CItemDataManager::AwardItem(UINT nItemType, UINT nAmount/*=1*/)
{
    CHECKF(nItemType);
    CHECKF(nAmount);
    
    if (!Database())
    { 
        LOGERROR("[CItemDataManager::AwardItem] Database==NULL");
        return NULL;
    }

    if (!m_pOwner)
    { 
        LOGERROR("[CItemDataManager::AwardItem] m_pOwner==NULL");
        return NULL;
    }

    if (!nAmount)
    {
        LOGERROR("[CItemDataManager::AwardItem] nAmount==0");
        return NULL;
    }

    //查询奖励到具是否配置
    CItemType* pItemType = NULL;
    if (QueryItemTypeManager())
    {
        pItemType = QueryItemTypeManager()->QueryItemType(nItemType);
        if(!pItemType)
        {
            LOGERROR("[CItemDataManager::AwardItem] QueryItemType Is Error 数据库item_type表中未查询到该道具的配置 nItemType=[%d]", nItemType);
            return NULL;
        }
    }
    else
    {
        LOGERROR("[CItemDataManager::AwardItem] QueryItemTypeManager==NULL");
        return NULL;
    }

    //检查数据库是否存在此类型道具-[限时道具-时间戳进行累加]
    CItemData* pItemData = this->QueryItemTypeData(nItemType);
    if (pItemData)              //已经存在此类型道具
    {
        //限时道具
        if (pItemData->GetItemType()%10 > 0)                                    //尾数 > 0 为限时道具
        {
            UINT nLiftTime = pItemType->GetLifeTime();                         
            if (nLiftTime > 0)                                                  //配置的限时道具时间戳 > 0
            {
                //更新剩余时间戳
                pItemData->SetLifeTime(pItemData->GetLifeTime() + nLiftTime);
                pItemData->QueryLeftTimeOut().IncInterval(nLiftTime, 24*3600*365);  //做倒计时一年最高上限处理

                //同步前端信息包
                CMsgItemAttrib msg;
                IF_OK(msg.CreateMsg(_ITEMATTRIB_LIFE_TIME, pItemData->GetId(), pItemData->GetLifeTime()))
                {
                    m_pOwner->SendMsgToClient(&msg);
                }

            }
            return pItemData;
        }
        else   //永久道具                     
        {
            if (pItemType->GetAmount() > pItemData->GetAmount())
            {
                //更新叠加数量--------叠加数量上限为1----通过配置控制
                pItemData->AddAmount(nAmount);

                //同步前端信息包
                CMsgItemAttrib msg;
                IF_OK(msg.CreateMsg(_ITEMATTRIB_AMOUNT, pItemData->GetId(), pItemData->GetAmount()))
                {
                    m_pOwner->SendMsgToClient(&msg);
                }
            }
            
            return pItemData;
        }
    }
    else              //未存在此类型道具
    {
        //创建新的道具，奖励道具
        CItemData* pNewItem =  CItemData::CreateNew();
        
        CHECKF(pNewItem);

        pNewItem->SetOwner(m_pOwner);

        //设置道具的相应属性
        pNewItem->SetUserId(m_pOwner->GetUid());
        pNewItem->SetItemType(nItemType);
        pNewItem->SetType(pItemType->GetType());    
        pNewItem->SetLifeTime(pItemType->GetLifeTime(), FALSE, TRUE);
        pNewItem->SetAmount(nAmount, FALSE, TRUE);

        //添加道具到管理器
        if(this->AddItem(pNewItem))
        {
            //如果新获得道具和当前正在限时领奖的车辆一个类型则进行刷新
            if (nItemType/10 == m_pOwner->GetShowItemType()/10)
            {
                CItemTypeManager* pItemTypeManager = QueryItemTypeManager();
                if (pItemTypeManager)
                {
                    CItemType* pItemType = pItemTypeManager->QueryNoExistItemType(m_pOwner, ITEM_TYPE_CAR_FACE);
                    if (pItemType)
                    {
                        //查到没有的类型道具
                        m_pOwner->SetShowItemType(pItemType->GetItemType());
                        //设置倒计时 2小时
                        UINT nLifeTime = 3600*2;
                        m_pOwner->SetShowRemainTs(nLifeTime);
                        //启动时钟计时器
                        m_pOwner->QueryLimitTimeInterval().Startup(nLifeTime);

                        //同步前端倒计时结束
                        CMsgUserAttrib msg;
                        IF_OK(msg.CreateMsg(_ACT_USERATTRIB_SHOW_ITEM_TYPE, pItemType->GetItemType()))
                        {
                            m_pOwner->SendMsgToClient(&msg);
                        }

                        CMsgUserAttrib msg2;
                        IF_OK(msg2.CreateMsg(_ACT_USERATTRIB_SHOW_REMAIN_Ts, nLifeTime))
                        {
                            m_pOwner->SendMsgToClient(&msg2);
                        }
                    }
                    else //未有道具
                    {
                        m_pOwner->QueryLimitTimeInterval().Clear();
                        //查到没有的类型道具
                        //超时后没有新的道具可以供玩家展现了，则不去处理showItemType,直接把ts设置位0就可以
                        //this->SetShowItemType(0);

                        m_pOwner->SetShowRemainTs(0);

                        //同步前端
                        // CMsgUserAttrib msg;
                        // IF_OK(msg.CreateMsg(_ACT_USERATTRIB_SHOW_ITEM_TYPE, pItemType->GetItemType()))
                        // {
                        //     this->SendMsgToClient(&msg);
                        // }
                        
                        //同步前端倒计时结束
                        CMsgUserAttrib msg;
                        IF_OK(msg.CreateMsg(_ACT_USERATTRIB_SHOW_REMAIN_Ts, 0))
                        {
                            m_pOwner->SendMsgToClient(&msg);
                        }
                    }
                }
                else
                {
                    LOGERROR("[MsgLogin::Process] pItemTypeManager == NULL");
                }
            }

            return pNewItem;
        }
        else
        {
            SAFE_RELEASE(pNewItem);
            return NULL;
        }
    }

    return NULL;
}

//添加道具
BOOL CItemDataManager::AddItem(CItemData* pItem)
{
    if (!Database())
    { 
        LOGERROR("[CItemDataManager::AddItem] Database==NULL");
        return FALSE;
    }

    if (!pItem)
    { 
        LOGERROR("[CItemDataManager::AddItem] pItem==NULL");
        return FALSE;
    }

    if (!m_pOwner)
    { 
        LOGERROR("[CItemDataManager::AddItem] m_pOwner==NULL");
        return FALSE;
    }

    //插入数据库
    if(!InsertRecord(pItem))
    {
        return FALSE;
    }

    //插入成功设置数据库的ID
    INT nNewId = Database()->GetInsertId();
    pItem->SetId(nNewId);

    //插入容器
    m_mapItemDataSet[pItem->GetId()] = pItem;

    //同步前端添加道具信息包
    CMsgItem msg;
    IF_OK(msg.CreateMsg(_ACT_ITEM_ADD, pItem))
    {
        m_pOwner->SendMsgToClient(&msg);
    }


    LOGDEBUG("[CItemDataManager::AddItem] Is Suc...nId=[%d] UserId=[%d] ItemType=[%d] nType=[%d] nLifeTime=[%d] nAmount=[%d]",
        pItem->GetId(),pItem->GetUserId(),pItem->GetItemType(),pItem->GetType(),pItem->GetLifeTime(),pItem->GetAmount());


    return TRUE;
}

//删除道具
BOOL CItemDataManager::DelItem(UINT nId, UINT nData/*=1*/)
{
    CHECKF(nId);
    CHECKF(m_pOwner);

    for (auto iter = m_mapItemDataSet.begin(); iter != m_mapItemDataSet.end(); )
    {   
        //找到对应道具,数量小于1个的时候删除数据库记录
        if((iter->second)->GetId() == nId)
        {
            if ((iter->second)->GetAmount() <= 1)               //加强容错判断，可以把限时道具增加进去...后期统一再添加
            {
                //金币和碎片不删除数据库记录，直接把数据amount置0就行
                if ((iter->second)->GetType() == ITEM_TYPE_GOLD || (iter->second)->GetType() == ITEM_TYPE_CHIP)
                {
                    if((iter->second)->DecAmount(nData, TRUE))
                    {
                        //发送消息给前端
                        CMsgItemAttrib msg;
                        if (msg.CreateMsg(_ITEMATTRIB_AMOUNT, (iter->second)->GetId(), (iter->second)->GetAmount()))
                        {
                            m_pOwner->SendMsgToClient(&msg);
                        }

                        return TRUE;
                    }
                    else
                    {
                        LOGERROR("[CItemDataManager::DelItem] DecAmount Is Error nId=[%d] nData=[%d]", nId, nData);
                        return FALSE;
                    }
                }
                //暂时先屏蔽都不删除记录，只是单独设置amount字段为0
                else
                {
                    //删除数据库记录
                    if(DeleteRecord(nId))
                    {
                        //发送消息给前端
                        CMsgItem msg;
                        if (msg.CreateMsg(_ACT_ITEM_DEL, nId))
                        {
                            m_pOwner->SendMsgToClient(&msg);
                        }

                        LOGDEBUG("[CItemDataManager::DelItem] DeleteRecord Is Suc nId=[%d] nData=[%d]", nId, nData);

                        //释放掉内存
                        SAFE_RELEASE(iter->second);

                        //清理容器对应道具
                        m_mapItemDataSet.erase(iter++);
                        return TRUE;
                    }
                    else
                    {
                        LOGERROR("[CItemDataManager::DelItem] DeleteRecord Is Error nId=[%d] nData=[%d]", nId, nData);
                        return FALSE;
                    }
                    
                    //不删除记录
                    // if((iter->second)->DecAmount(nData, TRUE))
                    // {
                    //     //发送消息给前端
                    //     CMsgItemAttrib msg;
                    //     if (msg.CreateMsg(_ITEMATTRIB_AMOUNT, (iter->second)->GetId(), (iter->second)->GetAmount()))
                    //     {
                    //         m_pOwner->SendMsgToClient(&msg);
                    //     }

                    //     return TRUE;
                    // }
                    // else
                    // {
                    //     LOGERROR("[CItemDataManager::DelItem] DecAmount Is Error nId=[%d] nData=[%d]", nId, nData);
                    //     return FALSE;
                    // }
                }
                //暂时先屏蔽都不删除记录，只是单独设置amount字段为0
            }
            else    //否则更改道具的叠加数量
            {
                if((iter->second)->DecAmount(nData, TRUE))
                {
                    //发送消息给前端
                    CMsgItemAttrib msg;
                    if (msg.CreateMsg(_ITEMATTRIB_AMOUNT, (iter->second)->GetId(), (iter->second)->GetAmount()))
                    {
                        m_pOwner->SendMsgToClient(&msg);
                    }

                    return TRUE;
                }
                else
                {
                    LOGERROR("[CItemDataManager::DelItem] DecAmount Is Error nId=[%d] nData=[%d]", nId, nData);
                    return FALSE;
                }
            }
        }
        ++iter;
    }
    return FALSE;
}

//插入数据库道具记录
BOOL CItemDataManager::InsertRecord(CItemData* pItem)
{
    if (!Database())
    { 
        LOGERROR("[CItemDataManager::InsertRecord] Database==NULL");
        return FALSE;
    }

    if (!pItem)
    { 
        LOGERROR("[CItemDataManager::InsertRecord] pItem==NULL");
        return FALSE;
    }

    if (!m_pOwner)
    { 
        LOGERROR("[CItemDataManager::InsertRecord] m_pOwner==NULL");
        return FALSE;
    }

    char szSql[1024];
    memset(szSql, 0, 1024);
    sprintf(szSql, "INSERT INTO g_bumper_item (user_id, item_type, type_, life_time, amount) VALUES (%d,%d,%d,%d,%d)",
        pItem->GetUserId(), pItem->GetItemType(), pItem->GetType(), pItem->GetLifeTime(), pItem->GetAmount());
    if(Database()->UpdateRecord(szSql)) 
    {
        INT nId = Database()->GetInsertId();
        LOGDEBUG("[CItemDataManager::InsertRecord] Is Suc... nId=[%d] sql=[%s]\n", nId, szSql);

    }
    else 
    {
        LOGERROR("[CItemDataManager::InsertRecord] Is Error... sql=[%s]\n", szSql);
        return FALSE;
    }
    return TRUE;
}

//删除数据库记录
BOOL CItemDataManager::DeleteRecord(UINT nId)
{
    if (!Database())
    { 
        LOGERROR("[CItemDataManager::DeleteRecord] Database==NULL");
        return FALSE;
    }

    char szSql[1024];
    memset(szSql, 0, 1024);
    sprintf(szSql, "DELETE FROM g_bumper_item WHERE id=%d LIMIT 1", nId);
    if(Database()->UpdateRecord(szSql)) 
    {
        LOGDEBUG("[CItemDataManager::DeleteRecord] Is Suc... sql=[%s]\n", szSql);
    }
    else 
    {
        LOGERROR("[CItemDataManager::DeleteRecord] Is Error... sql=[%s]\n", szSql);
        return FALSE;
    }
    return TRUE;
}

//查询道具接口
CItemData* CItemDataManager::QueryItemData(UINT nId)
{
    for (auto iter = m_mapItemDataSet.begin(); iter != m_mapItemDataSet.end(); iter++)
    {   
        //找到对应玩家
        if((iter->second)->GetId() ==  nId)
        {
            return (iter->second);                 
        }
    }
    return NULL;
}

//查询道具接口
CItemData* CItemDataManager::QueryItemTypeData(UINT nItemType)
{
    for (auto iter = m_mapItemDataSet.begin(); iter != m_mapItemDataSet.end(); iter++)
    {   
        //找到对应玩家
        if((iter->second)->GetItemType() ==  nItemType)
        {
            return (iter->second);                 
        }
    }
    return NULL;
}

//设置道具的相关属性
BOOL CItemDataManager::SetItemData(UINT nItemType, UINT nItemAttrib, UINT nData, BOOL bUpdate/*=TRUE*/)
{
    if (!m_pOwner)
    { 
        LOGERROR("[CItemDataManager::GetItemDataListInfo] m_pOwner==NULL");
        return FALSE;
    }

    for (auto iter = m_mapItemDataSet.begin(); iter != m_mapItemDataSet.end(); iter++)
    {   
        //找到对应玩家
        if((iter->second)->GetItemType() ==  nItemType)
        {
            //设置数量
            if (nItemAttrib == _ITEMATTRIB_AMOUNT)
            {
                if((iter->second)->SetAmount(nData, bUpdate))
                {
                    //同步前端道具属性-数量信息
                    CMsgItemAttrib msg;
                    IF_OK(msg.CreateMsg(_ITEMATTRIB_AMOUNT, (iter->second)->GetId(), nData))
                    {
                        m_pOwner->SendMsgToClient(&msg);
                    }
                    return TRUE;
                }
            }         
        }
    }
    return FALSE;         
}

//获取道具列表信息
BOOL CItemDataManager::GetItemDataListInfo()
{
    if (!m_pOwner)
    { 
        LOGERROR("[CItemDataManager::GetItemDataListInfo] m_pOwner==NULL");
        return FALSE;
    }

    for (auto iter = m_mapItemDataSet.begin(); iter != m_mapItemDataSet.end(); iter++)
    {   
        if(iter->second)
        {           
            //同步前端添加道具信息包
            CMsgItem msg;
            IF_OK(msg.CreateMsg(_ACT_ITEM_ADD, iter->second))
            {
                m_pOwner->SendMsgToClient(&msg);
            }

            LOGDEBUG("[CItemDataManager::GetItemDataListInfo] CreateMsg m_unId=[%d] m_unUid=[%d] ItemType=[%d] Type=[%d] LifeTime=[%d] Amount=[%d]",
                (iter->second)->GetId(),(iter->second)->GetUserId(),(iter->second)->GetItemType(),(iter->second)->GetType(),(iter->second)->GetLifeTime(),(iter->second)->GetAmount());
        }
    }

    return TRUE;
}