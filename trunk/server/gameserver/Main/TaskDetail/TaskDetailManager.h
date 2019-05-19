//////////////////////////////////////////////////////////////////////
//文件名称：CTaskDetailManager.h
//功能描述：
//版本说明：CTaskDetailManager API
//			
//编写作者：2018.11.6 yanfeng.hu, fliu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#include "../../Common/Type.h"
#include "../../Common/BaseType.h"
#include "../../Common/Config.h"
#include "../../Common/Common.h"
#include "TaskDetailData.h"

#include <vector>
using namespace std;


#ifndef __TASKDETAILDATA_MANAGER_H__
#define __TASKDETAILDATA_MANAGER_H__


//////////////////////////////////////////////////////////////////////
//常量
typedef std::map<UINT, CTaskDetailData*> MAP_TASKDETAILDATA_SET;
typedef MAP_TASKDETAILDATA_SET::const_iterator MAP_TASKDETAILDATA_CIT;
class Player;

//////////////////////////////////////////////////////////////////////
///玩家任务管理类
class CTaskDetailManager
{
public:
	CTaskDetailManager();
	virtual ~CTaskDetailManager();
	//类构造函数调用-内部使用
	VOID 	Init();
	//OnTimer函数
	BOOL 	OnTimer();
	//设置归属者指针
	BOOL	SetOwner(Player* pPlayer)						{	m_pOwner = pPlayer;  return TRUE;}
	//用于player类中CleanUp()调用
	VOID 	CleanUp();
	//清空管理器操作，内部调用
	VOID 	Clear();
	//加载数据信息
	BOOL 	LoadInfo();
	//保存信息
	BOOL 	SaveInfo(BOOL bUpdate = TRUE);

	//获取任务管理器容器
	MAP_TASKDETAILDATA_SET& GetTaskDetailDataSet() 				{	return m_mapTaskDetailDataSet;	}


public:	
	//添加任务
	BOOL 					AddTaskDetailData(CTaskDetailData* pData, BOOL bSynMsg=TRUE);
	//删除任务
	BOOL 					DelTaskDetailData(UINT nId);
	//查找任务
	CTaskDetailData* 		QueryTaskDetailDataById(UINT nId);
	//查找任务
	CTaskDetailData* 		QueryTaskDetailDataByTaskId(UINT nTaskId);
	//获取任务列表信息
	BOOL 					GetTaskDetailListInfo();
	//清理所有每日任务completenum字段
	BOOL 					ResetDayTaskMaskCompltetNum(UINT nTaskType, BOOL bUpdate, BOOL bSynMsg=TRUE); 
	
	//删除数据库任务记录
	BOOL 					DeleteRecord(UINT nTaskId);
	UINT 					InsertRecord(UINT nTaskId, UINT nTaskPhase, UINT nCompleteNum, UINT nTaskBeginTime, UINT nTaskData);

private:
	MAP_TASKDETAILDATA_SET 	m_mapTaskDetailDataSet;		//任务管理容器
	Player* 				m_pOwner;					//玩家指针

};
#endif //__TASKDETAILDATA_MANAGER_H__
