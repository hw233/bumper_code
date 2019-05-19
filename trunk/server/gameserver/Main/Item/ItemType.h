//////////////////////////////////////////////////////////////////////
//文件名称：CItemType.h
//功能描述：
//版本说明：CItemType API
//			
//编写作者：2018.12.13 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0 道具配置类
//
//////////////////////////////////////////////////////////////////////

#ifndef __ITEMTYPE_H__
#define __ITEMTYPE_H__
#include "../../Common/Type.h"
#include "../../Common/BaseType.h"
#include "../../Common/Config.h"
#include "../../Common/Common.h"

//////////////////////////////////////////////////////////////////////
///数据库表
enum _ITEMTYPE 
{
	_ITEMTYPE_ITEMTYPE 						= 0,					// 道具类型itemtype
	_ITEMTYPE_NAME 							= 1,					// 道具名字
	_ITEMTYPE_DESCRIBE 						= 2,					// 道具描述
	_ITEMTYPE_TYPE 							= 3,					// 道具类型type
	_ITEMTYPE_LIFE_TIME 					= 4,					// 试用时间
	_ITEMTYPE_VAS_GOLD						= 5,					// 商城价格(金币)
	_ITEMTYPE_VAS_ITEMTYPE					= 6,					// 商城价格道具itemtype
	_ITEMTYPE_AMOUNT						= 7,					// 叠加数量
	_ITEMTYPE_ACTION						= 8,					// 此道具触发的脚本接口
	
};

//道具结构体
typedef struct
{
	UINT nItemType;
	CHAR szName[32];
	CHAR szDescribe[128];
	UINT nType;
	UINT nLifeTime;
	UINT nVasGold;
	UINT nVasItemType;
	UINT nAmount;
	UINT nAction;
}stItemType;
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
///道具类型type取值 item_type/10000
#define ITEM_TYPE_CAR_FACE 10 										//车皮肤类型道具 永久类道具尾数为0，//限时类道具尾数为1
#define ITEM_TYPE_CAR_TAIL 11 										//拖尾类道具

#define ITEM_TYPE_BOX2 12 											//皮肤类道具
#define ITEM_TYPE_GOLD 13 											//金币类道具
#define ITEM_TYPE_CHIP 14 											//碎片类道具
#define ITEM_TYPE_BOX5 15 											//鞋子类道具

//////////////////////////////////////////////////////////////////////
class CItemType
{
public:
	CItemType();
	virtual ~CItemType();

public:
	static CItemType* 		CreateNew()										{	return new CItemType();	}
	BOOL					Release()										{ 	delete this; return TRUE; 		}
	stItemType&				QueryItemType()									{ 	return m_ItemTypeInfo; 		}

public:
	UINT	GetItemType()													{	return m_ItemTypeInfo.nItemType;			}
	BOOL	SetItemType(UINT nItemType);		

	CHAR*	GetName()														{ 	return m_ItemTypeInfo.szName; 	}
	BOOL	SetName(const CHAR* szName);
	
	CHAR*	GetDescribe()													{ 	return m_ItemTypeInfo.szDescribe; }
	BOOL	SetDescribe(const CHAR* szDescribe);						
	
	UINT	GetType()														{	return m_ItemTypeInfo.nType;			}
	BOOL 	SetType(UINT nType);

	UINT	GetLifeTime()													{	return m_ItemTypeInfo.nLifeTime;	}
	BOOL	SetLifeTime(UINT nLifeTime);

	UINT	GetVasGold()													{	return m_ItemTypeInfo.nVasGold;	}
	BOOL	SetVasGold(UINT nVasGold);

	UINT	GetVasItemType()												{	return m_ItemTypeInfo.nVasItemType;	}
	BOOL	SetVasItemType(UINT nVasItemType);

	UINT	GetAmount()														{	return m_ItemTypeInfo.nAmount;	}
	BOOL	SetAmount(UINT nAmount);

	UINT	GetAction()														{	return m_ItemTypeInfo.nAction;	}
	BOOL	SetAction(UINT nAction);

private:
	stItemType m_ItemTypeInfo;

};

#endif//__ITEMTYPE_H__
