//////////////////////////////////////////////////////////////////////
//文件名称：CItemAward.h
//功能描述：
//版本说明：CItemAward API
//			
//编写作者：2018.12.15 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0 道具配置类
//
//////////////////////////////////////////////////////////////////////

#ifndef __ITEMAWARD_H__
#define __ITEMAWARD_H__
#include "../../Common/Type.h"
#include "../../Common/BaseType.h"
#include "../../Common/Config.h"
#include "../../Common/Common.h"
#include "ItemType.h"

//////////////////////////////////////////////////////////////////////
///数据库表
enum _ITEMAWARD
{
	_ITEMAWARD_ID 							= 0,					// action id
	_ITEMAWARD_GOLD							= 1,					// 奖励的金币数量
	_ITEMAWARD_RATIO0 						= 2,					// 权重
	_ITEMAWARD_GROUP1						= 3,					// 奖励道具组1
	_ITEMAWARD_RATIO1						= 4,					// 权重
	_ITEMAWARD_GROUP2						= 5,					// 奖励道具组2
	_ITEMAWARD_RATIO2 						= 6,					// 权重
	_ITEMAWARD_GROUP3						= 7,					// 奖励道具组3
	_ITEMAWARD_RATIO3						= 8,					// 权重
	_ITEMAWARD_GROUP4						= 9,					// 奖励道具组4
	_ITEMAWARD_RATIO4 						= 10,					// 权重
	_ITEMAWARD_GROUP5						= 11,					// 奖励道具组5
	_ITEMAWARD_RATIO5						= 12,					// 权重
};

//道具结构体
typedef struct
{
	UINT nId;
	UINT nGold;
	UINT nRatio0;
	UINT nGroup1;
	UINT nRatio1;
	UINT nGroup2;
	UINT nRatio2;
	UINT nGroup3;
	UINT nRatio3;
	UINT nGroup4;
	UINT nRatio4;
	UINT nGroup5;
	UINT nRatio5;
	UINT nRatio6;
}stItemAward;
//////////////////////////////////////////////////////////////////////
///奖励id划分
const INT SIGN_UP_AWARD_ID = 10000;							//签到奖励组编号 10001 ~ 100007 星期一~星期七的脚本奖励入口
const INT OPEN_BOX_AWARD_ID = 20000;						//开启宝箱奖励组编号 20001 ~ 20004 开启宝箱脚本入口（4个宝箱）
const INT OPEN_BOX_DAY_MASK_OFFSET = 6;						//开启宝箱奖励组编号 掩码位偏移6 前面签到占用

const INT  PERMANENT_TASK_DETAIL_TYPE_AWARD_ID   			= 30000; 		//永久任务类型-奖励编号 30001 39999 支持9999种奖励
const INT  DAY_TASK_DETAIL_TYPE_AWARD_ID   					= 40000; 		//日常任务类型-奖励编号 40001 49999 支持9999种奖励
const INT  DAY_TASK_DETAIL_TRY_GAME_TYPE_AWARD_ID   		= 50000; 		//试玩任务类型-奖励编号 50001 59999 支持9999种奖励		

//////////////////////////////////////////////////////////////////////
class CItemAward
{
public:
	CItemAward();
	virtual ~CItemAward();

public:
	static CItemAward* 	CreateNew()											{	return new CItemAward();	}
	BOOL				Release()											{ 	delete this; return TRUE; 		}
	stItemAward&		QueryItemAward()										{ 	return m_ItemAwardInfo; 		}

public:
	UINT	GetId()															{	return m_ItemAwardInfo.nId;							}
	VOID	SetId(UINT nId)													{	m_ItemAwardInfo.nId = nId;							}
	UINT	GetGold()														{	return m_ItemAwardInfo.nGold;					}
	VOID	SetGold(UINT nGold)												{	m_ItemAwardInfo.nGold = nGold;			}	
	UINT	GetRatio0()														{	return m_ItemAwardInfo.nRatio0;						}	
	VOID	SetRatio0(UINT nRatio0)											{	m_ItemAwardInfo.nRatio0 = nRatio0;					}
	UINT	GetGroup1()														{	return m_ItemAwardInfo.nGroup1;					}
	VOID	SetGroup1(UINT nGroup1)											{	m_ItemAwardInfo.nGroup1 = nGroup1;			}	
	UINT	GetRatio1()														{	return m_ItemAwardInfo.nRatio1;						}	
	VOID	SetRatio1(UINT nRatio1)											{	m_ItemAwardInfo.nRatio1 = nRatio1;					}
	UINT	GetGroup2()														{	return m_ItemAwardInfo.nGroup2;					}	
	VOID	SetGroup2(UINT nGroup2)											{	m_ItemAwardInfo.nGroup2 = nGroup2;			}
	UINT	GetRatio2()														{	return m_ItemAwardInfo.nRatio2;						}
	VOID	SetRatio2(UINT nRatio2)											{	m_ItemAwardInfo.nRatio2 = nRatio2;					}
	UINT	GetGroup3()														{	return m_ItemAwardInfo.nGroup3;					}	
	VOID	SetGroup3(UINT nGroup3)											{	m_ItemAwardInfo.nGroup3 = nGroup3;			}
	UINT	GetRatio3()														{	return m_ItemAwardInfo.nRatio3;						}
	VOID	SetRatio3(UINT nRatio3)											{	m_ItemAwardInfo.nRatio3 = nRatio3;					}
	UINT	GetGroup4()														{	return m_ItemAwardInfo.nGroup4;					}	
	VOID	SetGroup4(UINT nGroup4)											{	m_ItemAwardInfo.nGroup4 = nGroup4;			}
	UINT	GetRatio4()														{	return m_ItemAwardInfo.nRatio4;						}
	VOID	SetRatio4(UINT nRatio4)											{	m_ItemAwardInfo.nRatio4 = nRatio4;					}
	UINT	GetGroup5()														{	return m_ItemAwardInfo.nGroup5;					}	
	VOID	SetGroup5(UINT nGroup5)											{	m_ItemAwardInfo.nGroup5 = nGroup5;			}
	UINT	GetRatio5()														{	return m_ItemAwardInfo.nRatio5;						}
	VOID	SetRatio5(UINT nRatio5)											{	m_ItemAwardInfo.nRatio5 = nRatio5;					}
	
private:
	stItemAward m_ItemAwardInfo;

};

#endif//__ITEMAWARD_H__
