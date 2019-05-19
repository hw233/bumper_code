//////////////////////////////////////////////////////////////////////
//文件名称：COnLineAwardCfgManager.h
//功能描述：
//版本说明：COnLineAwardCfgManager API
//			
//编写作者：2018.12.27 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0 在线领奖配置管理类
//
//////////////////////////////////////////////////////////////////////

#ifndef __ONLINE_AWARD_CFG_MANAGER_H__
#define __ONLINE_AWARD_CFG_MANAGER_H__

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
#include "OnLineAwardCfg.h"

//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////
///礼盒、礼包、箱子等一系列相关的奖励
typedef std::map<UINT, COnLineAwardCfg*> MAP_ONLINEIAWARDCFG_SET;
typedef MAP_ONLINEIAWARDCFG_SET::const_iterator MAP_ONLINEAWARDCFG_CIT;

class Player;
//////////////////////////////////////////////////////////////////////
//class
//////////////////////////////////////////////////////////////////////
class COnLineAwardCfgManager
{
public:
	COnLineAwardCfgManager();
	virtual ~COnLineAwardCfgManager();
	BOOL 							Init();
	//获取道具管理容器
	MAP_ONLINEIAWARDCFG_SET& 		QueryOnLineAwardCfgSet() 				{	return m_mapOnLineAwardCfgSet;	}
	//查询道具接口
	COnLineAwardCfg*				QueryOnLineAwardCfg(UINT nId);
	//发放奖励给某人
	BOOL 							AwardItemToPlayer(Player* pPlayer, UINT nId, INT nAwardMulti=1);
public:	
	//加载itemtype配置文件信息
	BOOL 							LoadInfo();

private:
    MAP_ONLINEIAWARDCFG_SET 		m_mapOnLineAwardCfgSet;
};

//全局变量
extern COnLineAwardCfgManager* g_pOnLineAwardCfgManager;
#define	QueryOnLineAwardCfgManager() (g_pOnLineAwardCfgManager)

#endif //__ONLINE_AWARD_CFG_MANAGER_H__