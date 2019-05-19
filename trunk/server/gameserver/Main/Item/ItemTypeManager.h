//////////////////////////////////////////////////////////////////////
//文件名称：CItemTypeManager.h
//功能描述：
//版本说明：CItemTypeManager API
//			
//编写作者：2018.12.13 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#ifndef __ITEMTYPE_MANAGER_H__
#define __ITEMTYPE_MANAGER_H__

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
#include "ItemType.h"

class Player;
//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////
typedef std::map<UINT, CItemType*> MAP_ITEMTYPE_SET;
typedef MAP_ITEMTYPE_SET::const_iterator MAP__FRIEND_CIT;

//////////////////////////////////////////////////////////////////////
//class
//////////////////////////////////////////////////////////////////////
class CItemTypeManager
{
public:
	CItemTypeManager();
	virtual ~CItemTypeManager();
	BOOL 					Init();
	//获取道具管理容器
	MAP_ITEMTYPE_SET& 		QueryItemTypeSet() 				{	return m_mapItemTypeSet;	}
	//查询道具接口
	CItemType*				QueryItemType(UINT nItemType);
	//查询某个玩家还未获得的道具
	CItemType*				QueryNoExistItemType(Player* pPlayer, UINT nType);

public:	
	//加载itemtype配置文件信息
	BOOL 					LoadInfo();

private:
    MAP_ITEMTYPE_SET 		m_mapItemTypeSet;
};

//全局变量
extern CItemTypeManager* g_pItemTypeManager;
#define QueryItemTypeManager() (g_pItemTypeManager)

#endif //__ITEMTYPE_MANAGER_H__