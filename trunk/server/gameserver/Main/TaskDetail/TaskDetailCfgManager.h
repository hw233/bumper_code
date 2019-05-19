//////////////////////////////////////////////////////////////////////
//文件名称：CTaskDetailCfgManager.h
//功能描述：
//版本说明：CTaskDetailCfgManager API
//			
//编写作者：2018.12.28 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0 任务配置文件管理器
//
//////////////////////////////////////////////////////////////////////

#ifndef __TASK_DETAIL_CFG_MANAGER_H__
#define __TASK_DETAIL_CFG_MANAGER_H__

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
#include "TaskDetailCfg.h"

//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////
///任务相关
typedef std::map<UINT, CTaskDetailCfg*> MAP_TASKDETAILCFG_SET;
typedef MAP_TASKDETAILCFG_SET::const_iterator MAP_TASKDETAILCFG_CIT;

class Player;
//////////////////////////////////////////////////////////////////////
//class
//////////////////////////////////////////////////////////////////////
class CTaskDetailCfgManager
{
public:
	CTaskDetailCfgManager();
	virtual ~CTaskDetailCfgManager();
	BOOL 						Init();
	//获取任务管理容器
	MAP_TASKDETAILCFG_SET& 		QueryTaskDetailCfgSet() 					{	return m_mapTaskDetailCfgSet;	}
	//查询任务接口
	CTaskDetailCfg*				QueryTaskDetailCfgInfo(UINT nTaskId);
	//检测日常任务是否有更新
	BOOL 						CheckDayTaskDetailRecord(Player* pPlayer);
public:	
	//加载任务配置文件信息
	BOOL 						LoadInfo();

private:
    MAP_TASKDETAILCFG_SET 		m_mapTaskDetailCfgSet;
};

//全局变量
extern CTaskDetailCfgManager* g_pTaskDetailCfgManager;
#define	QueryTaskDetailCfgManager() (g_pTaskDetailCfgManager)

#endif //__TASK_DETAIL_CFG_MANAGER_H__