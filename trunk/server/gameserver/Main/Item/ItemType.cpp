//////////////////////////////////////////////////////////////////////
//文件名称：CItemType.cpp
//功能描述：
//版本说明：CItemType API
//			
//编写作者：2018.12.13 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#include "ItemType.h"
#include "../../DataBase/DBMySql.h"
#include "../I_mydb.h"
#include "../Player.h"


//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////
//构造
CItemType::CItemType()
{
    memset(&m_ItemTypeInfo, 0, sizeof(m_ItemTypeInfo));
}

//析构函数
CItemType::~CItemType()
{ 	
}
   
BOOL CItemType::SetItemType(UINT nItemType)
{
    m_ItemTypeInfo.nItemType = nItemType;
    return TRUE;
} 

BOOL CItemType::SetName(const CHAR* szName)
{
    CHECKF(szName);
    SafeCopy(m_ItemTypeInfo.szName, szName, 32);
    return TRUE;
}

BOOL CItemType::SetDescribe(const CHAR* szDescribe)
{
    CHECKF(szDescribe);
    SafeCopy(m_ItemTypeInfo.szDescribe, szDescribe, 128);
    return TRUE;
}

BOOL CItemType::SetType(UINT nType)
{
    m_ItemTypeInfo.nType = nType;
    return TRUE;
}

BOOL CItemType::SetLifeTime(UINT nLifeTime)
{
    m_ItemTypeInfo.nLifeTime = nLifeTime;
    return TRUE;
} 

BOOL CItemType::SetVasGold(UINT nVasGold)
{
    m_ItemTypeInfo.nVasGold = nVasGold;
    return TRUE;
} 

BOOL CItemType::SetVasItemType(UINT nVasItemType)
{
    m_ItemTypeInfo.nVasItemType = nVasItemType;
    return TRUE;
}

BOOL CItemType::SetAmount(UINT nAmount)
{
    m_ItemTypeInfo.nAmount = nAmount;
    return TRUE;
} 
BOOL CItemType::SetAction(UINT nAction)
{
    m_ItemTypeInfo.nAction = nAction;
    return TRUE;
} 
