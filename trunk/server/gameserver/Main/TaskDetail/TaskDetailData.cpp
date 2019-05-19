//////////////////////////////////////////////////////////////////////
//文件名称：CTaskDetailData.cpp
//功能描述：
//版本说明：CTaskDetailData API
//			
//编写作者：2018.11.6 yanfeng.hu, fliu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#include "TaskDetailData.h"
#include "../../DataBase/DBMySql.h"
#include "../I_mydb.h"
#include "../Player.h"

char	szTaskTable[] = _TBL_TASK;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaskDetailData::CTaskDetailData()
{
    this->Init();
}

CTaskDetailData::~CTaskDetailData()
{
}

//类构造函数调用-内部使用
VOID CTaskDetailData::Init()
{
    memset(&m_TaskDetailDataInfo, 0, sizeof(stTaskDetailData));
    m_bUpdate = FALSE;
    m_pOwner = NULL;
}

//OnTimer函数
BOOL CTaskDetailData::OnTimer()
{
    return TRUE;
}

//保存信息
BOOL CTaskDetailData::SaveInfo(BOOL bUpdate/* = TRUE*/)
{
    if (!Database()) 
        return FALSE;

    if (bUpdate)
    {
        if (m_bUpdate)
        {
            char szSql[1028];
            memset(szSql, 0, 1028);
            sprintf(szSql, "UPDATE g_bumper_task SET task_phase=%d, task_complete_num=%d, task_data=%d WHERE id=%d", 
                m_TaskDetailDataInfo.unTaskPhase, m_TaskDetailDataInfo.unCompleteNum, m_TaskDetailDataInfo.unTaskData, m_TaskDetailDataInfo.unId);
            
            LOGDEBUG("[CTaskDetailData::SaveInfo] szSql = [%s]\n", szSql);
           
            if(Database()->UpdateRecord(szSql))
            {
                LOGDEBUG("[CTaskDetailData::SaveInfo] Is Suc..., sql=[%s] \n", szSql);
            }
            else
            {
                LOGDEBUG("[CTaskDetailData::SaveInfo] Is Error..., sql=[%s] \n", szSql);
                return FALSE;
            }
        }
    }
    
    return TRUE;
}

UINT CTaskDetailData::CreateRecord(const stTaskDetailData* pInfo)
{
    CHECKF(pInfo);

    UINT nRet = this->InsertRecord(pInfo);

    //this->SetId(pInfo->unId);
    this->SetUserId(pInfo->unUserId);
    this->SetTaskId(pInfo->unTaskId);
    this->SetTaskPhase(pInfo->unTaskPhase, FALSE, TRUE);
    this->SetCompleteNum(pInfo->unCompleteNum, FALSE, TRUE);
    //this->SetMask1(pInfo->unMask1);
    //this->SetMask2(pInfo->unMask2);
    this->SetTaskBeginTime(pInfo->unTaskBeginTime, FALSE, TRUE);
    this->SetTaskData(pInfo->unTaskData, FALSE, TRUE);

    return nRet;
}

UINT CTaskDetailData::InsertRecord(const stTaskDetailData* pInfo)
{
    UINT nId = 0; 
	SQLBUF szSql;
    sprintf(szSql, "INSERT INTO g_bumper_task (user_id, task_id, task_phase, task_complete_num, task_begin_time, task_data)"\
                   " values (%d, %d, %d, %d, %d, %d)", pInfo->unUserId, pInfo->unTaskId, pInfo->unTaskPhase, pInfo->unCompleteNum,
                   pInfo->unTaskBeginTime, pInfo->unTaskData);
    if(Database()->UpdateRecord(string(szSql))) 
    {
        nId = Database()->GetInsertId();
        this->SetId(nId);

        LOGDEBUG("[CTaskDetailData::InsertRecord] Is Suc... nId=[%d] sql=[%s]\n", nId, szSql);
    }
    else 
    {
        LOGDEBUG("[CTaskDetailData::InsertRecord] Is Error... sql=[%s]\n", szSql);
        return nId;
    }

    return nId;
}

BOOL CTaskDetailData::DeleteRecord(const stTaskDetailData* pInfo)
{
    SQLBUF szSql;
    sprintf(szSql, "DELETE from g_bumper_task where id = %d ", pInfo->unId);
    
    if(Database()->UpdateRecord(string(szSql))) 
    {
        LOGDEBUG("[CTaskDetailData::DeleteRecord] Is Suc... sql=[%s]\n", szSql);
    }
    else 
    {
        LOGDEBUG("[CTaskDetailData::DeleteRecord] Is Error... sql=[%s]\n", szSql);
        return FALSE;
    }
    return TRUE;
}


BOOL CTaskDetailData::SetUserId(UINT idUser, BOOL bUpdate/* = FALSE*/)
{
    m_TaskDetailDataInfo.unUserId  = idUser;
    if (!Database())
        return FALSE;

    if (bUpdate)
    {
        std::string sql=string("update g_bumper_task set user_id=")+std::to_string(idUser)+" where id=" + std::to_string(m_TaskDetailDataInfo.unId);
        if(Database()->UpdateRecord(sql)) 
        {
            LOGDEBUG("[CTaskDetailData::SetUserId] Is Suc... sql=[%s]\n", sql.c_str());
        }
        else 
        {
            LOGDEBUG("[CTaskDetailData::SetUserId] Is Error... sql=[%s]\n", sql.c_str());
            return FALSE;
        }
    }
    return TRUE;
}

BOOL CTaskDetailData::SetTaskId(UINT idTask, BOOL bUpdate/* = FALSE*/)
{
	m_TaskDetailDataInfo.unTaskId  = idTask;
    if (!Database())
        return FALSE;

    if (bUpdate)
    {
        std::string sql=string("update g_bumper_task set task_id=")+std::to_string(idTask)+" where id="+std::to_string(m_TaskDetailDataInfo.unId);
        if(Database()->UpdateRecord(sql)) 
        {
            LOGDEBUG("[CTaskDetailData::SetTaskId] Is Suc... sql=[%s]\n", sql.c_str());
        }
        else 
        {
            LOGDEBUG("[CTaskDetailData::SetTaskId] Is Error... sql=[%s]\n", sql.c_str());
            return FALSE;
        }
    }
    return TRUE;
}

BOOL CTaskDetailData::SetTaskPhase(UINT taskPhase, BOOL bUpdate/* = FALSE*/, BOOL bHappenChange /*= FALSE*/)
{
    if (bHappenChange)              //首次加载
    {
        //首次加载不用设置标志位
    }
    else                            //后续改变
    {
        if (m_TaskDetailDataInfo.unTaskPhase != taskPhase)
        {
            m_bUpdate = TRUE;       //发生了变化
        }
    }

	m_TaskDetailDataInfo.unTaskPhase  = taskPhase;
    if (!Database())
        return FALSE;

    if (bUpdate)
    {
        std::string sql=string("update g_bumper_task set task_phase=")+std::to_string(taskPhase)+" where id="+std::to_string(m_TaskDetailDataInfo.unId);
        if(Database()->UpdateRecord(sql)) 
        {
            LOGDEBUG("[CTaskDetailData::SetTaskPhase] Is Suc... sql=[%s]\n", sql.c_str());
        }
        else 
        {
            LOGDEBUG("[CTaskDetailData::SetTaskPhase] Is Error... sql=[%s]\n", sql.c_str());
            return FALSE;
        }
    }
    return TRUE;
}

BOOL CTaskDetailData::SetCompleteNum(UINT completeNum, BOOL bUpdate/* = FALSE*/, BOOL bHappenChange /*= FALSE*/)
{
    if (bHappenChange)              //首次加载
    {
        //首次加载不用设置标志位
    }
    else                            //后续改变
    {
        if (m_TaskDetailDataInfo.unCompleteNum != completeNum)
        {
            m_bUpdate = TRUE;       //发生了变化
        }
    }

	m_TaskDetailDataInfo.unCompleteNum  = completeNum;
    if (!Database())
        return FALSE;

    if (bUpdate)
    {
        std::string sql=string("update g_bumper_task set task_complete_num=")+std::to_string(completeNum)+" where id="+std::to_string(m_TaskDetailDataInfo.unId);
        if(Database()->UpdateRecord(sql)) 
        {
            LOGDEBUG("[CTaskDetailData::SetCompleteNum] Is Suc... sql=[%s]\n", sql.c_str());
        }
        else 
        {
            LOGDEBUG("[CTaskDetailData::SetCompleteNum] Is Error... sql=[%s]\n", sql.c_str());
            return FALSE;
        }
    }
    return TRUE;
}
  
BOOL CTaskDetailData::SetTaskData(UINT taskData, BOOL bUpdate/* = FALSE*/, BOOL bHappenChange /*= FALSE*/)
{
    if (bHappenChange)              //首次加载
    {
        //首次加载不用设置标志位
    }
    else                            //后续改变
    {
        if (m_TaskDetailDataInfo.unTaskData != taskData)
        {
            m_bUpdate = TRUE;       //发生了变化
        }
    }

	m_TaskDetailDataInfo.unTaskData  = taskData;
    if (!Database())
        return FALSE;

    if (bUpdate)
    {
        std::string sql=string("update g_bumper_task set task_data=")+std::to_string(taskData)+" where id="+std::to_string(m_TaskDetailDataInfo.unId);
        if(Database()->UpdateRecord(sql)) 
        {
            LOGDEBUG("[CTaskDetailData::SetTaskData] Is Suc... sql=[%s]\n", sql.c_str());
        }
        else 
        {
            LOGDEBUG("[CTaskDetailData::SetTaskData] Is Error... sql=[%s]\n", sql.c_str());
            return FALSE;
        }
    }
    return TRUE;
} 

BOOL CTaskDetailData::SetTaskBeginTime(UINT taskBeginTime, BOOL bUpdate/* = FALSE*/, BOOL bHappenChange /*= FALSE*/)
{
    if (bHappenChange)              //首次加载
    {
        //首次加载不用设置标志位
    }
    else                            //后续改变
    {
        if (m_TaskDetailDataInfo.unTaskBeginTime != taskBeginTime)
        {
            m_bUpdate = TRUE;       //发生了变化
        }
    }

	m_TaskDetailDataInfo.unTaskBeginTime  = taskBeginTime;
    if (!Database())
        return FALSE;

    if (bUpdate)
    {
        std::string sql=string("update g_bumper_task set task_begin_time=")+std::to_string(taskBeginTime)+" where id="+std::to_string(m_TaskDetailDataInfo.unId);
        if(Database()->UpdateRecord(sql)) 
        {
            LOGDEBUG("[CTaskDetailData::SetTaskBeginTime] Is Suc... sql=[%s]\n", sql.c_str());
        }
        else 
        {
            LOGDEBUG("[CTaskDetailData::SetTaskBeginTime] Is Error... sql=[%s]\n", sql.c_str());
            return FALSE;
        }
    }
    return TRUE;
}

// BOOL CTaskDetailData::SetMask1(UINT unMask1, BOOL bUpdate/* = FALSE*/, BOOL bHappenChange /*= FALSE*/)
// {
//     m_TaskDetailDataInfo.unMask1  = unMask1;
//     if (!Database())
//         return FALSE;

//     if (bUpdate)
//     {
//         std::string sql=string("update g_bumper_task set mask1=")+std::to_string(unMask1)+" where id="+std::to_string(m_TaskDetailDataInfo.unId);
//         if(Database()->UpdateRecord(sql)) 
//         {
//              LOGDEBUG("[CTaskDetailData::SetMask1] Is Suc... sql=[%s]\n", sql.c_str());
//         }
//         else 
//         {
//              LOGDEBUG("[CTaskDetailData::SetMask1] Is Error... sql=[%s]\n", sql.c_str());
//              return FALSE;
//         }
//     }
//     return TRUE;
// }

// BOOL CTaskDetailData::SetMask2(UINT unMask2, BOOL bUpdate/* = FALSE*/, BOOL bHappenChange /*= FALSE*/)
// {
//     m_TaskDetailDataInfo.unMask2  = unMask2;
//     if (!Database())
//         return FALSE;

//     if (bUpdate)
//     {
//         std::string sql=string("update g_bumper_task set mask2=")+std::to_string(unMask2)+" where id="+std::to_string(m_TaskDetailDataInfo.unId);
//         if(Database()->UpdateRecord(sql)) 
//         {
//              LOGDEBUG("[CTaskDetailData::SetMask2] Is Suc... sql=[%s]\n", sql.c_str());
//         }
//         else 
//         {
//              LOGDEBUG("[CTaskDetailData::SetMask2] Is Error... sql=[%s]\n", sql.c_str());
//              return FALSE;
//         }
//     }
//     return TRUE;
// }
