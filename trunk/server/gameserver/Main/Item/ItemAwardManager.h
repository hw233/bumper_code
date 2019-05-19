//////////////////////////////////////////////////////////////////////
//文件名称：CItemAwardManager.h
//功能描述：
//版本说明：CItemAwardManager API
//			
//编写作者：2018.12.13 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#ifndef __ITEMAWARD_MANAGER_H__
#define __ITEMAWARD_MANAGER_H__

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
#include "ItemAward.h"

//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////
///礼盒、礼包、箱子等一系列相关的奖励
typedef std::map<UINT, CItemAward*> MAP_ITEMAWARD_SET;
typedef MAP_ITEMAWARD_SET::const_iterator MAP_ITEMAWARD_CIT;

class Player;
//////////////////////////////////////////////////////////////////////
//class
//////////////////////////////////////////////////////////////////////
class CItemAwardManager
{
public:
	CItemAwardManager();
	virtual ~CItemAwardManager();
	BOOL 					Init();
	//获取道具管理容器
	MAP_ITEMAWARD_SET& 		QueryItemAwardSet() 				{	return m_mapItemAwardSet;	}
	//查询道具接口
	CItemAward*				QueryItemAward(UINT nAwardId);
	//发放奖励给某人
	BOOL 					AwardItemToPlayer(Player* pPlayer, UINT nAwardId, INT nAwardMulti=1);
public:	
	//加载itemtype配置文件信息
	BOOL 					LoadInfo();

private:
    MAP_ITEMAWARD_SET 		m_mapItemAwardSet;
};

//全局变量
extern CItemAwardManager* g_pItemAwardManager;
#define	QueryItemAwardManager() (g_pItemAwardManager)

#endif //__ITEMAWARD_MANAGER_H__