//////////////////////////////////////////////////////////////////////
//文件名称：CLimitTimeAwardCfg.h
//功能描述：
//版本说明：CLimitTimeAwardCfg API
//			
//编写作者：2018.12.26 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0 限时领奖配置类
//
//////////////////////////////////////////////////////////////////////

#ifndef __LIMIT_TIME_AWARD_CFG_H__
#define __LIMIT_TIME_AWARD_CFG_H__
#include "../../Common/Type.h"
#include "../../Common/BaseType.h"
#include "../../Common/Config.h"
#include "../../Common/Common.h"
#include "../Item/ItemType.h"

//////////////////////////////////////////////////////////////////////
///数据库表
enum _LIMIT_TIME_AWARD_CFG
{
	_LIMIT_TIME_AWARD_CFG_ID 								= 0,					// id 自增量就可以
	_LIMIT_TIME_AWARD_CFG_NEXT_ID							= 1,					// 下一个限时免费的id
	//_LIMIT_TIME_AWARD_CFG_ITEM_TYPE							= 2,					// 下一个限时免费的id
	_LIMIT_TIME_AWARD_CFG_LIMIT_TIME						= 2,					// 限时时间
	_LIMIT_TIME_AWARD_CFG_AWARD_ID							= 3,					// 奖励actionid
};

//道具结构体
typedef struct
{
	UINT nId;
	UINT nNextId;
	//UINT nItemType;
	UINT nLimitTime;
	UINT nAwardId;
}stLimitTimeAwardCfg;
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
class CLimitTimeAwardCfg
{
public:
	CLimitTimeAwardCfg();
	virtual ~CLimitTimeAwardCfg();

public:
	static CLimitTimeAwardCfg* 	CreateNew()									{	return new CLimitTimeAwardCfg();	}
	BOOL						Release()									{ 	delete this; return TRUE; 		}
	stLimitTimeAwardCfg&		QueryLimitTimeAwardCfgInfo()				{ 	return m_LimitTimeAwardCfgInfo; 		}

public:
	UINT	GetId()															{	return m_LimitTimeAwardCfgInfo.nId;							}
	VOID	SetId(UINT nId)													{	m_LimitTimeAwardCfgInfo.nId = nId;							}
	UINT	GetNextId()														{	return m_LimitTimeAwardCfgInfo.nNextId;					}
	VOID	SetNextId(UINT nNextId)											{	m_LimitTimeAwardCfgInfo.nNextId = nNextId;			}	
	//UINT	GetItemType()														{	return m_LimitTimeAwardCfgInfo.nItemType;					}
	//VOID	SetItemType(UINT nItemType)											{	m_LimitTimeAwardCfgInfo.nItemType = nItemType;			}
	UINT	GetLimitTime()													{	return m_LimitTimeAwardCfgInfo.nLimitTime;						}	
	VOID	SetLimitTime(UINT nLimitTime)									{	m_LimitTimeAwardCfgInfo.nLimitTime = nLimitTime;					}
	UINT	GetAwardId()													{	return m_LimitTimeAwardCfgInfo.nAwardId;					}
	VOID	SetAwardId(UINT nAwardId)										{	m_LimitTimeAwardCfgInfo.nAwardId = nAwardId;					}
	
private:
	stLimitTimeAwardCfg m_LimitTimeAwardCfgInfo;

};

#endif//__LIMIT_TIME_AWARD_CFG_H__
