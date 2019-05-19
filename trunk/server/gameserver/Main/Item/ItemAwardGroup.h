//////////////////////////////////////////////////////////////////////
//文件名称：CItemAwardGroup.h
//功能描述：
//版本说明：CItemAwardGroup API
//			
//编写作者：2018.12.19 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0 道具奖励组配置类
//
//////////////////////////////////////////////////////////////////////

#ifndef __ITEMAWARD_GROUP_H__
#define __ITEMAWARD_GROUP_H__
#include "../../Common/Type.h"
#include "../../Common/BaseType.h"
#include "../../Common/Config.h"
#include "../../Common/Common.h"
#include "ItemType.h"

//////////////////////////////////////////////////////////////////////
///数据库表
enum _ITEMAWARD_GROUP
{
	_ITEMAWARD_GROUP_ID 							= 0,					// increment id
	_ITEMAWARD_GROUP_GROUP_ID 						= 1,					// group id
	_ITEMAWARD_GROUP_ITEMTYPE						= 2,					// 奖励的道具itemtype
	_ITEMAWARD_GROUP_ITEMAMOUNT						= 3,					// 奖励的道具amount
	_ITEMAWARD_GROUP_RATIO							= 4,					// 权重
};

//道具结构体
typedef struct
{	
	UINT nId;
	UINT nGroupId;
	UINT nItemType;
	UINT nItemAmount;
	UINT nRatio;
}stItemAwardGroup;
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
class CItemAwardGroup
{
public:
	CItemAwardGroup();
	virtual ~CItemAwardGroup();

public:
	static CItemAwardGroup* 	CreateNew()									{	return new CItemAwardGroup();	}
	BOOL						Release()									{ 	delete this; return TRUE; 		}
	stItemAwardGroup&			QueryItemAwardGroup()						{ 	return m_ItemAwardGroupInfo; 		}

public:
	UINT	GetId()															{	return m_ItemAwardGroupInfo.nId;							}
	VOID	SetId(UINT nId)													{	m_ItemAwardGroupInfo.nId = nId;							}
	UINT	GetGroupId()													{	return m_ItemAwardGroupInfo.nGroupId;							}
	VOID	SetGroupId(UINT nGroupId)										{	m_ItemAwardGroupInfo.nGroupId = nGroupId;							}
	UINT	GetItemType()													{	return m_ItemAwardGroupInfo.nItemType;						}	
	VOID	SetItemType(UINT nItemType)										{	m_ItemAwardGroupInfo.nItemType = nItemType;					}
	UINT	GetItemAmount()													{	return m_ItemAwardGroupInfo.nItemAmount;						}	
	VOID	SetItemAmount(UINT nAmount)										{	m_ItemAwardGroupInfo.nItemAmount = nAmount;					}
	UINT	GetRatio()														{	return m_ItemAwardGroupInfo.nRatio;						}
	VOID	SetRatio(UINT nRatio)											{	m_ItemAwardGroupInfo.nRatio = nRatio;					}

private:
	stItemAwardGroup m_ItemAwardGroupInfo;

};

#endif//__ITEMAWARD_GROUP_H__
