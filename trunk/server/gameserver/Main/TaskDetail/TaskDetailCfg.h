//////////////////////////////////////////////////////////////////////
//文件名称：CCTaskDetailConfig.h
//功能描述：
//版本说明：CCTaskDetailConfig API
//          
//编写作者：2019.01.02 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0 任务配置文件类
//
//////////////////////////////////////////////////////////////////////

#ifndef __TASK_CONFIG_H__
#define __TASK_CONFIG_H__

#include "../../Common/Type.h"
#include "../../Common/BaseType.h"
#include "../../DataBase/DBMySql.h"

#include <map>
#include <memory>
#include <string>

//////////////////////////////////////////////////////////////////////
///任务数据库配置列
enum TASKDETAIL_CFG
{
    TASKDETAIL_CFG_TASKID             = 0,          // 任务编号
    TASKDETAIL_CFG_TASKPHASE          = 1,          // 任务阶段
    TASKDETAIL_CFG_TASKCOMPLETENUM    = 2,          // 任务完成次数
    TASKDETAIL_CFG_MASK_1             = 3,          // 任务掩码位第一次领取奖励标识 0:未领取 1:已经领取
    TASKDETAIL_CFG_MASK_2             = 4,          // 任务掩码位再领一次奖励标识 0:未领取 1:已经领取
    TASKDETAIL_CFG_TASKDATA           = 5,          // 任意数据,意义可已自定义,奖励脚本入口
};
//////////////////////////////////////////////////////////////////////
///任务配置结构体
typedef struct
{
    UINT    unTaskId;
    UINT    unTaskPhase;
    UINT    unCompleteNum; 
    UINT    unMask1;
    UINT    unMask2;
    UINT    unTaskData;   //任务的任意数据
}stTaskDetailCfg;

//////////////////////////////////////////////////////////////////////
class CTaskDetailCfg
{
public:
    CTaskDetailCfg();
    virtual ~CTaskDetailCfg();

public:
    static CTaskDetailCfg*  CreateNew()             {   return new CTaskDetailCfg();  }
    BOOL                    Release()               {   delete this; return TRUE;     }
    stTaskDetailCfg&        QueryTaskDetailInfo()   {   return m_TaskDetailCfgInfo;   }
    BOOL                    Init();

public:    
    UINT  GetTaskId() { return     m_TaskDetailCfgInfo.unTaskId; }
    BOOL  SetTaskId(UINT idTask, BOOL bUpdate = FALSE);

    UINT  GetTaskPhase() { return m_TaskDetailCfgInfo.unTaskPhase;       }
    BOOL  SetTaskPhase(UINT taskPhase, BOOL bUpdate = FALSE);

    UINT  GetCompleteNum() { return m_TaskDetailCfgInfo.unCompleteNum; }
    BOOL  SetCompleteNum(UINT completeNum, BOOL bUpdate = FALSE);

    UINT  GetMask1() { return m_TaskDetailCfgInfo.unMask1; }
    BOOL  SetMask1(UINT unMask1, BOOL bUpdate = FALSE);

    UINT  GetMask2() { return m_TaskDetailCfgInfo.unMask2; }
    BOOL  SetMask2(UINT unMask2, BOOL bUpdate = FALSE);
    
    UINT  GetTaskData() { return m_TaskDetailCfgInfo.unTaskData; }
    BOOL  SetTaskData(UINT taskData, BOOL bUpdate = FALSE);
    
    //BOOL  SaveInfo();
    //BOOL  CreateRecord(const stTaskDetailCfg* pInfo);
    //BOOL  InsertRecord(const stTaskDetailCfg* pInfo);
    //BOOL  DeleteRecord(const stTaskDetailCfg* pInfo);

protected:
    stTaskDetailCfg m_TaskDetailCfgInfo;
};

#endif //__TASK_CONFIGURE_H__


