//////////////////////////////////////////////////////////////////////
//文件名称：CTaskDetailConfig.h
//功能描述：
//版本说明：CTaskDetailConfig API
//          
//编写作者：2019.01.02 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0 任务配置文件类
//
//////////////////////////////////////////////////////////////////////
#include "TaskDetailCfg.h"

#include "../../MsgHandler/NetMsg.h"
#include "../../Common/Config.h"
#include "../../Common/Common.h"
#include "../../Main/UserData.h"
#include "../ClientManager.h"
#include "../../DataBase/DBMySql.h"
#include "../../Protocol/Command.h"
#include "../../Protocol/cJSON.h"
#include <string>


//////////////////////////////////////////////////////////////////////
///
CTaskDetailCfg::CTaskDetailCfg()
{
    Init();
}

CTaskDetailCfg::~CTaskDetailCfg()
{
}

BOOL CTaskDetailCfg::Init()
{
    memset(&m_TaskDetailCfgInfo, 0, sizeof(stTaskDetailCfg));
    return TRUE;
}

BOOL CTaskDetailCfg::SetTaskId(UINT idTask, BOOL bUpdate/* = TRUE*/)
{
    m_TaskDetailCfgInfo.unTaskId = idTask;
    return TRUE;
}

BOOL CTaskDetailCfg::SetTaskPhase(UINT taskPhase, BOOL bUpdate/* = TRUE*/)
{
    m_TaskDetailCfgInfo.unTaskPhase = taskPhase;
    return TRUE;
}

BOOL CTaskDetailCfg::SetCompleteNum(UINT completeNum, BOOL bUpdate/* = TRUE*/)
{
    m_TaskDetailCfgInfo.unCompleteNum = completeNum;
    return TRUE;
}

BOOL CTaskDetailCfg::SetMask1(UINT unMask1, BOOL bUpdate/* = TRUE*/)
{
    m_TaskDetailCfgInfo.unMask1 = unMask1;
    return TRUE;
}

BOOL CTaskDetailCfg::SetMask2(UINT unMask2, BOOL bUpdate/* = TRUE*/)
{
    m_TaskDetailCfgInfo.unMask2 = unMask2;
    return TRUE;
}

BOOL CTaskDetailCfg::SetTaskData(UINT taskData, BOOL bUpdate/* = TRUE*/)
{
    m_TaskDetailCfgInfo.unTaskData = taskData;
    return TRUE;
}
