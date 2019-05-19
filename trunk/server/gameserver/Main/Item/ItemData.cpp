//////////////////////////////////////////////////////////////////////
//文件名称：CItemData.cpp
//功能描述：
//版本说明：CItemData API
//			
//编写作者：2018.12.13 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#include "ItemData.h"
#include "../../DataBase/DBMySql.h"
#include "../I_mydb.h"
#include "../Player.h"
#include "ItemTypeManager.h"


//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////
//构造
CItemData::CItemData()
{
    Init();
}

//析构函数
CItemData::~CItemData()
{ 	
}

//类构造函数调用-内部使用
VOID CItemData::Init()
{
    m_pOwner = NULL;
    memset(&m_ItemDataInfo, 0, sizeof(m_ItemDataInfo));
    m_tLeftTimeOut.Clear();
    m_bUpdate = FALSE;
}

//OnTimer函数
BOOL CItemData::OnTimer()
{
    /*
    //检查道具的剩余时间是否超时
    UINT nRemainTime = this->GetLifeTime();                        //查看剩余时间戳
    if (!m_tLeftTimeOut.IsActive() && nRemainTime > 0)               //计时器未启动，则进行启动定时器
    {
        //启动定时器
        m_tLeftTimeOut.Startup(nRemainTime);
    }
    else                                                          //计时器启动，判断倒计时结束
    {
        if (m_tLeftTimeOut.TimeOver())   //超时
        {
            //设置道具倒计时时间戳
            this->SetLifeTime(0);
        }
    }
    */
    return TRUE;
}

//保存信息
BOOL CItemData::SaveInfo(BOOL bUpdate /*= TRUE*/)
{
    if (!Database())
    { 
        return FALSE;
    }

    if (bUpdate)
    {
        if (m_bUpdate || m_ItemDataInfo.nItemType%10 > 0)  //限时的道具强制更新到数据库，发生了改变的进行数据库更新
        {
            //先获取剩余时间
            UINT nRemainTime = this->GetLifeTime();                        //查看剩余时间戳
            if (nRemainTime > 0)
            {
                nRemainTime = m_tLeftTimeOut.GetRemain();
            }

            char szSql[1024];
            memset(szSql, 0, 1024);
            sprintf(szSql, "UPDATE g_bumper_item SET life_time=%d, amount=%d WHERE id=%d", nRemainTime, m_ItemDataInfo.nAmount, m_ItemDataInfo.nId);
            if(Database()->UpdateRecord(szSql))
            {
                LOGDEBUG("[CItemData::SaveInfo] Is Suc..., sql=[%s] \n", szSql);
            }
            else
            {
                LOGDEBUG("[CItemData::SaveInfo] Is Error..., sql=[%s] \n", szSql);
                return FALSE;
            }
        }
        
    }
    return TRUE;
} 

BOOL CItemData::SetId(UINT nId, BOOL bUpdate /*= TRUE*/)
{
    m_ItemDataInfo.nId = nId;
    return TRUE;
}  

BOOL CItemData::SetItemType(UINT nItemType, BOOL bUpdate /*= TRUE*/)
{
    m_ItemDataInfo.nItemType = nItemType;
    return TRUE;
}  

BOOL CItemData::SetUserId(UINT nUserId, BOOL bUpdate /*= TRUE*/)
{
    m_ItemDataInfo.nUserId = nUserId;
    return TRUE;
}   
                
BOOL CItemData::SetType(UINT nType, BOOL bUpdate /*= TRUE*/)
{
    m_ItemDataInfo.nType = nType;
    return TRUE;
} 

BOOL CItemData::SetLifeTime(UINT nLifeTime, BOOL bUpdate /*= TRUE*/, BOOL bHappenChange /*= FALSE*/)
{
    if (bHappenChange)              //首次加载
    {
        //首次加载不用设置标志位
    }
    else                            //后续改变
    {
        if (m_ItemDataInfo.nLifeTime != nLifeTime)
        {
            m_bUpdate = TRUE;       //发生了变化
        }
    }

    m_ItemDataInfo.nLifeTime = nLifeTime;

    if (!Database())
    { 
        return FALSE;
    }

    if (bUpdate)
    {
        char szSql[1024];
        memset(szSql, 0, 1024);
        sprintf(szSql, "UPDATE g_bumper_item SET life_time=%d WHERE id=%d", nLifeTime, m_ItemDataInfo.nId);
        if(!Database()->UpdateRecord(szSql))
        {
            return FALSE;
        }
    }
    return TRUE;
} 

BOOL CItemData::SetAmount(UINT nAmount, BOOL bUpdate /*= TRUE*/, BOOL bHappenChange /*= FALSE*/)
{
    if (bHappenChange)              //首次加载
    {
        //首次加载不用设置标志位
    }
    else                            //后续改变
    {
        if (m_ItemDataInfo.nAmount != nAmount)
        {
            m_bUpdate = TRUE;       //发生了变化
        }
    }

    m_ItemDataInfo.nAmount = nAmount;
    if (!Database())
    { 
        return FALSE;
    }

    if (bUpdate)
    {
        char szSql[1024];
        memset(szSql, 0, 1024);
        sprintf(szSql, "UPDATE g_bumper_item SET amount=%d WHERE id=%d", nAmount, m_ItemDataInfo.nId);
        if(!Database()->UpdateRecord(szSql))
        {
            return FALSE;
        }
    }
    return TRUE;
} 

BOOL CItemData::AddAmount(UINT nData, BOOL bUpdate /*= TRUE*/)
{
    UINT nLimitAmount = 0;
    if (QueryItemTypeManager())
    {
        CItemType* pItemType = QueryItemTypeManager()->QueryItemType(m_ItemDataInfo.nItemType);
        if (pItemType)
        {
            nLimitAmount = pItemType->GetAmount();
        }
    }

    m_bUpdate = TRUE;       //发生了变化
    m_ItemDataInfo.nAmount = min(m_ItemDataInfo.nAmount+nData, nLimitAmount);
    
    if (!Database())
    { 
        return FALSE;
    }

    if (bUpdate)
    {
        char szSql[1024];
        memset(szSql, 0, 1024);
        sprintf(szSql, "UPDATE g_bumper_item SET amount=%d WHERE id=%d", m_ItemDataInfo.nAmount, m_ItemDataInfo.nId);
        if(!Database()->UpdateRecord(szSql))
        {
            return FALSE;
        }
    }
    return TRUE;
} 

BOOL CItemData::DecAmount(UINT nData, BOOL bUpdate /*= TRUE*/)
{
    if (m_ItemDataInfo.nAmount < nData)
    {
        LOGERROR("[CItemData::DecAmount] m_ItemDataInfo.nAmount < nData nHaveAmount=[%d] nData=[%d]", m_ItemDataInfo.nAmount, nData);
        return FALSE;
    }

    m_bUpdate = TRUE;       //发生了变化
    m_ItemDataInfo.nAmount -= nData;

    if (!Database())
    { 
        return FALSE;
    }

    if (bUpdate)
    {
        char szSql[1024];
        memset(szSql, 0, 1024);
        sprintf(szSql, "UPDATE g_bumper_item SET amount=%d WHERE id=%d", m_ItemDataInfo.nAmount, m_ItemDataInfo.nId);
        if(!Database()->UpdateRecord(szSql))
        {
            return FALSE;
        }
    }
    return TRUE;
} 


