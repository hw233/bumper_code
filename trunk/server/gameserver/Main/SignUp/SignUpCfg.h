//////////////////////////////////////////////////////////////////////
//文件名称：CSignUpCfg.h
//功能描述：
//版本说明：CSignUpCfg API
//			
//编写作者：2018.12.24 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0 签到配置类：
// v1.0版本支持：奖励金币+权重 奖励道具分组（支持5分组），每个分组可以无限制的配置道具和奖励道具的概率控制
//
//////////////////////////////////////////////////////////////////////

#ifndef __SIGNUPCFG_H__
#define __SIGNUPCFG_H__
#include "../../Common/Type.h"
#include "../../Common/BaseType.h"
#include "../../Common/Config.h"
#include "../../Common/Common.h"

//////////////////////////////////////////////////////////////////////
///数据库表
enum _SIGNUPCFG
{
	_SIGNUPCFG_ID 							= 0,					// action id
	_SIGNUPCFG_GOLD							= 1,					// 奖励的金币数量
	_SIGNUPCFG_RATIO0 						= 2,					// 权重
	_SIGNUPCFG_GROUP1						= 3,					// 奖励道具组ID_1
	_SIGNUPCFG_RATIO1						= 4,					// 权重
	_SIGNUPCFG_GROUP2						= 5,					// 奖励道具组ID_2
	_SIGNUPCFG_RATIO2 						= 6,					// 权重
	_SIGNUPCFG_GROUP3						= 7,					// 奖励道具组ID_3
	_SIGNUPCFG_RATIO3						= 8,					// 权重
	_SIGNUPCFG_GROUP4						= 9,					// 奖励道具组ID_4
	_SIGNUPCFG_RATIO4 						= 10,					// 权重
	_SIGNUPCFG_GROUP5						= 11,					// 奖励道具组ID_5
	_SIGNUPCFG_RATIO5						= 12,					// 权重
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
}signup_cfg_t;
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
///签到配置类
class CSignUpCfg
{
public:
	CSignUpCfg();
	virtual ~CSignUpCfg();

public:
	static CSignUpCfg* 	CreateNew()											{	return new CSignUpCfg();	}
	BOOL				Release()											{ 	delete this; return TRUE; 		}
	signup_cfg_t&		QuerySignUpCfgInfo()								{ 	return m_SignUpCfgInfo; 		}

public:
	UINT	GetId()															{	return m_SignUpCfgInfo.nId;							}
	VOID	SetId(UINT nId)													{	m_SignUpCfgInfo.nId = nId;							}
	UINT	GetGold()														{	return m_SignUpCfgInfo.nGold;					}
	VOID	SetGold(UINT nGold)												{	m_SignUpCfgInfo.nGold = nGold;			}	
	UINT	GetRatio0()														{	return m_SignUpCfgInfo.nRatio0;						}	
	VOID	SetRatio0(UINT nRatio0)											{	m_SignUpCfgInfo.nRatio0 = nRatio0;					}
	UINT	GetGroup1()														{	return m_SignUpCfgInfo.nGroup1;					}
	VOID	SetGroup1(UINT nGroup1)											{	m_SignUpCfgInfo.nGroup1 = nGroup1;			}	
	UINT	GetRatio1()														{	return m_SignUpCfgInfo.nRatio1;						}	
	VOID	SetRatio1(UINT nRatio1)											{	m_SignUpCfgInfo.nRatio1 = nRatio1;					}
	UINT	GetGroup2()														{	return m_SignUpCfgInfo.nGroup2;					}	
	VOID	SetGroup2(UINT nGroup2)											{	m_SignUpCfgInfo.nGroup2 = nGroup2;			}
	UINT	GetRatio2()														{	return m_SignUpCfgInfo.nRatio2;						}
	VOID	SetRatio2(UINT nRatio2)											{	m_SignUpCfgInfo.nRatio2 = nRatio2;					}
	UINT	GetGroup3()														{	return m_SignUpCfgInfo.nGroup3;					}	
	VOID	SetGroup3(UINT nGroup3)											{	m_SignUpCfgInfo.nGroup3 = nGroup3;			}
	UINT	GetRatio3()														{	return m_SignUpCfgInfo.nRatio3;						}
	VOID	SetRatio3(UINT nRatio3)											{	m_SignUpCfgInfo.nRatio3 = nRatio3;					}
	UINT	GetGroup4()														{	return m_SignUpCfgInfo.nGroup4;					}	
	VOID	SetGroup4(UINT nGroup4)											{	m_SignUpCfgInfo.nGroup4 = nGroup4;			}
	UINT	GetRatio4()														{	return m_SignUpCfgInfo.nRatio4;						}
	VOID	SetRatio4(UINT nRatio4)											{	m_SignUpCfgInfo.nRatio4 = nRatio4;					}
	UINT	GetGroup5()														{	return m_SignUpCfgInfo.nGroup5;					}	
	VOID	SetGroup5(UINT nGroup5)											{	m_SignUpCfgInfo.nGroup5 = nGroup5;			}
	UINT	GetRatio5()														{	return m_SignUpCfgInfo.nRatio5;						}
	VOID	SetRatio5(UINT nRatio5)											{	m_SignUpCfgInfo.nRatio5 = nRatio5;					}
	
private:
	signup_cfg_t m_SignUpCfgInfo;

};

#endif//__SIGNUPCFG_H__
