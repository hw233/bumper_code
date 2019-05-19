//////////////////////////////////////////////////////////////////////
//文件名称：CItemData.h
//功能描述：
//版本说明：CItemData API
//			
//编写作者：2018.12.13 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0 玩家道具类
//
//////////////////////////////////////////////////////////////////////

#ifndef __ITEMDATA_H__
#define __ITEMDATA_H__
#include "../../Common/Type.h"
#include "../../Common/BaseType.h"
#include "../../Common/Config.h"
#include "../../Common/Common.h"
#include "../../Common/TimeOut.h"
#include "ItemType.h"



//////////////////////////////////////////////////////////////////////
///数据库表
enum _ITEMDATA 
{
	_ITEMDATA_ID 							= 0,					// 道具唯一ID(数据库自增量)
	_ITEMDATA_USER_ID 						= 1,					// 用户ID
	_ITEMDATA_ITEMTYPE 						= 2,					// 道具类型typeID
	_ITEMDATA_TYPE 							= 3,					// 道具类型type	
	_ITEMDATA_LIFE_TIME 					= 4,					// 试用时间（剩余）
	_ITEMDATA_AMOUNT						= 5,					// 叠加数量(当前)
};

//道具结构体
typedef struct
{
	UINT nId;
	UINT nUserId;
	UINT nItemType;
	UINT nType;
	UINT nLifeTime;
	UINT nAmount;
}stItemData;
//////////////////////////////////////////////////////////////////////
///

//////////////////////////////////////////////////////////////////////
///前置声明
class Player;

//////////////////////////////////////////////////////////////////////
///玩家道具类
class CItemData
{
public:
	CItemData();
	virtual ~CItemData();
	
public:
	static CItemData* CreateNew()										{	return new CItemData();	}
	BOOL	Release()													{ 	delete this; return TRUE; 		}
	//类构造函数调用-内部使用
	VOID 	Init();
	//OnTimer函数
	BOOL 	OnTimer();
	//设置归属者指针
	BOOL	SetOwner(Player* pPlayer)									{	CHECKF(pPlayer);  m_pOwner = pPlayer;  return TRUE;}
	//保存信息
	BOOL 	SaveInfo(BOOL bUpdate = TRUE);

	stItemData&		QueryItemData()										{ 	return m_ItemDataInfo; 		}
	CTimeOut&		QueryLeftTimeOut()									{ 	return m_tLeftTimeOut; 		}

	//属性相关
public:
	UINT	GetId()															{	return m_ItemDataInfo.nId;			}
	BOOL	SetId(UINT nId, BOOL bUpdate = TRUE);		
	
	UINT	GetUserId()														{	return m_ItemDataInfo.nUserId;			}
	BOOL	SetUserId(UINT nUserId, BOOL bUpdate = TRUE);

	UINT	GetItemType()													{	return m_ItemDataInfo.nItemType;			}
	BOOL	SetItemType(UINT ItemType, BOOL bUpdate = TRUE);		

	UINT	GetType()														{	return m_ItemDataInfo.nType;			}
	BOOL 	SetType(UINT nType, BOOL bUpdate = TRUE);

	UINT	GetLifeTime()													{	return m_ItemDataInfo.nLifeTime;	}
	BOOL	SetLifeTime(UINT nLifeTime, BOOL bUpdate = TRUE, BOOL bHappenChange = FALSE);

	UINT	GetAmount()														{	return m_ItemDataInfo.nAmount;	}
	BOOL	SetAmount(UINT nAmount, BOOL bUpdate = TRUE, BOOL bHappenChange = FALSE);
	BOOL 	AddAmount(UINT nData, BOOL bUpdate = TRUE);
	BOOL	DecAmount(UINT nAmount, BOOL bUpdate = TRUE);

	//接口相关
public:
	//BOOL    IsBoxItem()														{	return (m_ItemDataInfo.nId/10000 == ITEM_TYPE_BOX) ? TRUE : TRUE;	}

	//成员变量
private:
	stItemData 			m_ItemDataInfo;
	Player* 			m_pOwner;
	CTimeOut 			m_tLeftTimeOut;
	BOOL    			m_bUpdate;							// 数据是否有变更，有则进行下线数据库更新
};

#endif//__ITEMDATA_H__
