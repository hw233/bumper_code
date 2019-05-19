//////////////////////////////////////////////////////////////////////
//文件名称：COnLineAwardCfg.h
//功能描述：
//版本说明：COnLineAwardCfg API
//			
//编写作者：2018.12.27 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0 在线领奖配置类
//
//////////////////////////////////////////////////////////////////////

#ifndef __ONLINE_AWARD_CFG_H__
#define __ONLINE_AWARD_CFG_H__
#include "../../Common/Type.h"
#include "../../Common/BaseType.h"
#include "../../Common/Config.h"
#include "../../Common/Common.h"
#include "../Item/ItemType.h"

//////////////////////////////////////////////////////////////////////
///数据库表
enum _ONLINE_AWARD_CFG
{
	_ONLINE_AWARD_CFG_ID 								= 0,					// id 自增量就可以
	_ONLINE_AWARD_CFG_NEXT_ID							= 1,					// 下一个限时免费的id
	_ONLINE_AWARD_CFG_ITEM_TYPE							= 2,					// 奖励的道具itemt
	_ONLINE_AWARD_CFG_ITEM_AMOUNT						= 3,					// 奖励的道具amount
	_ONLINE_AWARD_CFG_INTERVAL_TIME						= 4,					// 限时时间
	_ONLINE_AWARD_CFG_AWARD_ID							= 5,					// 奖励awardid
};

//道具结构体
typedef struct
{
	UINT nId;
	UINT nNextId;
	UINT nItemType;
	UINT nItemAmount;
	UINT nIntervalTime;
	UINT nAwardId;
}stOnLineAwardCfg;
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
class COnLineAwardCfg
{
public:
	COnLineAwardCfg();
	virtual ~COnLineAwardCfg();

public:
	static COnLineAwardCfg* 	CreateNew()									{	return new COnLineAwardCfg();	}
	BOOL						Release()									{ 	delete this; return TRUE; 		}
	stOnLineAwardCfg&			QueryOnLineAwardCfgInfo()					{ 	return m_OnLineAwardCfgInfo; 		}

public:
	UINT	GetId()															{	return m_OnLineAwardCfgInfo.nId;							}
	VOID	SetId(UINT nId)													{	m_OnLineAwardCfgInfo.nId = nId;							}
	UINT	GetNextId()														{	return m_OnLineAwardCfgInfo.nNextId;					}
	VOID	SetNextId(UINT nNextId)											{	m_OnLineAwardCfgInfo.nNextId = nNextId;			}	
	UINT	GetItemType()													{	return m_OnLineAwardCfgInfo.nItemType;					}
	VOID	SetItemType(UINT nItemType)										{	m_OnLineAwardCfgInfo.nItemType = nItemType;			}
	UINT	GetItemAmount()													{	return m_OnLineAwardCfgInfo.nItemAmount;					}
	VOID	SetItemAmount(UINT nItemAmount)									{	m_OnLineAwardCfgInfo.nItemAmount = nItemAmount;			}
	UINT	GetIntervalTime()												{	return m_OnLineAwardCfgInfo.nIntervalTime;						}	
	VOID	SetIntervalTime(UINT nIntervalTime)								{	m_OnLineAwardCfgInfo.nIntervalTime = nIntervalTime;					}
	UINT	GetAwardId()													{	return m_OnLineAwardCfgInfo.nAwardId;					}
	VOID	SetAwardId(UINT nAwardId)										{	m_OnLineAwardCfgInfo.nAwardId = nAwardId;					}
	
private:
	stOnLineAwardCfg	m_OnLineAwardCfgInfo;

};

#endif//__ONLINE_AWARD_CFG_H__
