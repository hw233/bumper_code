//////////////////////////////////////////////////////////////////////
//文件名称：CItemAwardGroupManager.h
//功能描述：
//版本说明：CItemAwardGroupManager API
//			
//编写作者：2018.12.19 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#ifndef __ITEMAWARD_GROUP_MANAGER_H__
#define __ITEMAWARD_GROUP_MANAGER_H__

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
#include "ItemAwardGroup.h"

//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////
typedef std::multimap<UINT, CItemAwardGroup*> MULTIMAP_ITEMAWARD_GROUD_SET;
//typedef MAP_ITEMAWARD_GROUD_SET::const_iterator MAP_ITEMAWARD_GROUD_CIT;
typedef std::multimap<UINT, CItemAwardGroup*>::iterator MULTIMAP_ITEMAWARD_GROUD_CIT;
typedef pair<MULTIMAP_ITEMAWARD_GROUD_CIT, MULTIMAP_ITEMAWARD_GROUD_CIT> MULTIMAP_ITEMAWARD_GROUD_PAIR;

class Player;
//////////////////////////////////////////////////////////////////////
//class
//////////////////////////////////////////////////////////////////////
class CItemAwardGroupManager
{
public:
	CItemAwardGroupManager();
	virtual ~CItemAwardGroupManager();
	BOOL 								Init();
	//获取道具管理容器
	MULTIMAP_ITEMAWARD_GROUD_SET& 		QueryItemAwardGroupSet() 				{	return m_mulitmapItemAwardGroupSet;	}
	//查询道具接口
	CItemAwardGroup*					QueryItemAwardGroup(UINT nGroupId);
	//奖励道具接口，每条配置单独做概率就算
	BOOL 								AwardItemByAwardGroup(Player* pPlayer, UINT nGroupId, INT nAwardMulti);

public:	
	//加载itemtype配置文件信息
	BOOL 								LoadInfo();

private:
    MULTIMAP_ITEMAWARD_GROUD_SET 		m_mulitmapItemAwardGroupSet;
};

//全局变量
extern CItemAwardGroupManager* g_pItemAwardGroupManager;
#define	QueryItemAwardGroupManager() (g_pItemAwardGroupManager)

#endif //__ITEMAWARD_GROUP_MANAGER_H__