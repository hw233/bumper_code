//////////////////////////////////////////////////////////////////////
//文件名称：CLimitTimeAwardCfgManager.h
//功能描述：
//版本说明：CLimitTimeAwardCfgManager API
//			
//编写作者：2018.12.26 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#ifndef __LIMIT_TIME_AWARD_CFG_MANAGER_H__
#define __LIMIT_TIME_AWARD_CFG_MANAGER_H__

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include "../../Common/Type.h"
#include "../../Common/BaseType.h"
//!alter by huyf 2018.04.23:添加连接开关可配置 
#include "../../Common/Config.h"
#include "../../Common/Common.h"
//!alter end:添加连接开关可配置 
#include <vector>
#include <map>
using namespace std;
#include "../Item/ItemAward.h"
#include "LimitTimeAwardCfg.h"

//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////
///礼盒、礼包、箱子等一系列相关的奖励
typedef std::map<UINT, CLimitTimeAwardCfg*> MAP_LIMITTIMEAWARDCFG_SET;
typedef MAP_LIMITTIMEAWARDCFG_SET::const_iterator MAP_LIMITTIMEAWARDCFG_CIT;

class Player;
//////////////////////////////////////////////////////////////////////
//class
//////////////////////////////////////////////////////////////////////
class CLimitTimeAwardCfgManager
{
public:
	CLimitTimeAwardCfgManager();
	virtual ~CLimitTimeAwardCfgManager();
	BOOL 							Init();
	//获取道具管理容器
	MAP_LIMITTIMEAWARDCFG_SET& 		QueryLimitTimeAwardCfgSet() 				{	return m_mapLimitTimeAwardCfgSet;	}
	//查询道具接口
	CLimitTimeAwardCfg*				QueryLimitTimeAwardCfg(UINT nAwardId);
	//发放奖励给某人
	BOOL 							AwardItemToPlayer(Player* pPlayer, UINT nAwardId, INT nAwardMulti=1);
public:	
	//加载itemtype配置文件信息
	BOOL 							LoadInfo();

private:
    MAP_LIMITTIMEAWARDCFG_SET 		m_mapLimitTimeAwardCfgSet;
};

//全局变量
extern CLimitTimeAwardCfgManager* g_pLimitTimeAwardCfgManager;
#define	QueryLimitTimeAwardCfgManager() (g_pLimitTimeAwardCfgManager)

#endif //__LIMIT_TIME_AWARD_CFG_MANAGER_H__