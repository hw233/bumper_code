//////////////////////////////////////////////////////////////////////
//文件名称：CTaskDetailManager.cpp
//功能描述：
//版本说明：CTaskDetailManager API
//			
//编写作者：2018.11.22 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#include "TaskDetailCfgManager.h"
#include "TaskDetailCfg.h"
#include "TaskDetailManager.h"
#include "TaskDetailData.h"
#include "../../DataBase/DBMySql.h"
#include "../I_mydb.h"
#include "../Player.h"
#include "../../MsgHandler/AllMsg.h"


//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////
//构造
CTaskDetailManager::CTaskDetailManager()
{
    this->Init();
}

//析构函数
CTaskDetailManager::~CTaskDetailManager()
{ 
    this->CleanUp();
}

//类构造函数调用-内部使用
VOID CTaskDetailManager::Init()
{
    this->Clear();
    this->SetOwner(NULL);
}

//用于player类中CleanUp()调用
VOID CTaskDetailManager::CleanUp()
{ 
    this->Clear();
    this->SetOwner(NULL);
}

//清空管理器操作，内部调用
VOID CTaskDetailManager::Clear()
{ 
    for (auto iter = m_mapTaskDetailDataSet.begin(); iter != m_mapTaskDetailDataSet.end(); iter++)
    {   
        if(iter->second)
        {
            SAFE_RELEASE(iter->second);
        } 
    }

    m_mapTaskDetailDataSet.clear();
}

//OnTimer函数
BOOL CTaskDetailManager::OnTimer() 
{ 
    // for (auto iter = m_mapTaskDetailDataSet.begin(); iter != m_mapTaskDetailDataSet.end(); iter++)
    // {   
    //     if(iter->second)
    //     {
    //         (iter->second)->OnTimer();
    //     }
    // }
    return TRUE;
}

//加载数据库好友信息
BOOL CTaskDetailManager::LoadInfo() 
{
    if (!Database())
    { 
    	LOGERROR("[CTaskDetailManager::LoadInfo] Database==NULL");
        return FALSE;
    }

    if (!m_pOwner)
    { 
        return FALSE;
        LOGERROR("[CTaskDetailManager::LoadInfo] m_pOwner==NULL");
    }

    //执行一次清空操作
    this->Clear();

    UINT nUid = m_pOwner->GetUid();
    std::string sql = string("SELECT * FROM g_bumper_task WHERE user_id=")+std::to_string(nUid);
    MYSQL_RES* result = Database()->Query(sql);
    if(result == NULL)
    {
        LOGERROR("[CTaskDetailManager::LoadInfo] sql error: %s\n", sql.c_str());
        return FALSE;
    }

    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)))
    {    
    	CTaskDetailData* pData = CTaskDetailData::CreateNew();
    	if (pData)
    	{
    		pData->SetId(SafeAtoi(row[TASKDETAILDATA_ID]));
	        pData->SetUserId(SafeAtoi(row[TASKDETAILDATA_USERID]), FALSE);
	        pData->SetTaskId(SafeAtoi(row[TASKDETAILDATA_TASKID]), FALSE);
	        pData->SetTaskPhase(SafeAtoi(row[TASKDETAILDATA_TASKPHASE]), FALSE, TRUE);
	        pData->SetCompleteNum(SafeAtoi(row[TASKDETAILDATA_TASKCOMPLETENUM]), FALSE, TRUE);
            pData->SetTaskBeginTime(SafeAtoi(row[TASKDETAILDATA_TASKBEGINTIME]), FALSE, TRUE);
    		pData->SetTaskData(SafeAtoi(row[TASKDETAILDATA_TASKDATA]), FALSE, TRUE);

	        //插入容器
	        m_mapTaskDetailDataSet[pData->GetId()] = pData;
	        
	        LOGDEBUG("[CTaskDetailManager::LoadInfo] size=[%d] Id=[%d] Uid=[%d] TaskId=[%d] TaskPhase=[%d] CompleteNum=[%d] TaskBeginTime=[%d] TaskData=[%d]", 
            	m_mapTaskDetailDataSet.size(), pData->GetId(), pData->GetUserId(), pData->GetTaskId(), pData->GetTaskPhase(), pData->GetCompleteNum(), pData->GetTaskBeginTime(), pData->GetTaskData());
    	}
    }
    mysql_free_result(result);
    result = NULL;
    LOGDEBUG("[CTaskDetailManager::LoadInfo] Loading table g_bumper_task end");

    //检查有不存在的日常任务，则进行任务的插入
    CTaskDetailCfgManager* pTaskDetailCfgManager = QueryTaskDetailCfgManager();
    if (pTaskDetailCfgManager)
    {
        pTaskDetailCfgManager->CheckDayTaskDetailRecord(m_pOwner);
    }
    return TRUE;
    
}
//保存任务信息
BOOL CTaskDetailManager::SaveInfo(BOOL bUpdate /*= TRUE*/)
{
    for (auto iter = m_mapTaskDetailDataSet.begin(); iter != m_mapTaskDetailDataSet.end(); iter++)
    {   
        if((iter->second))
        {
            (iter->second)->SaveInfo(bUpdate);
        }
    }
    return TRUE;
}

//清理所有每日任务completenum字段
BOOL CTaskDetailManager::ResetDayTaskMaskCompltetNum(UINT nTaskType, BOOL bUpdate, BOOL bSynMsg/*=TRUE*/)
{
    for (auto iter = m_mapTaskDetailDataSet.begin(); iter != m_mapTaskDetailDataSet.end(); iter++)
    {   
        if((iter->second)->GetTaskId()/1000 == nTaskType)    //10000开头为日常任务
        {
            (iter->second)->SetCompleteNum(0, bUpdate);

            if (bSynMsg)
            {
                //更新任务数据
                CMsgTaskDetail msg;
                IF_OK(msg.CreateMsg(_TASK_DETAIL_UPDATE, m_pOwner, (iter->second), (iter->second)->GetCompleteNum()))
                {
                    m_pOwner->SendMsgToClient(&msg);
                }
            }
        }
    }
    return TRUE;
}

//获取任务列表信息
BOOL CTaskDetailManager::GetTaskDetailListInfo()
{
    if (!m_pOwner)
    { 
    	LOGERROR("[CTaskDetailManager::GetTaskDetailListInfo] m_pOwner==NULL");
        return FALSE;
    }

	for (auto iter = m_mapTaskDetailDataSet.begin(); iter != m_mapTaskDetailDataSet.end(); iter++)
    {	
    	if(iter->second)
    	{    		
            //同步前端任务信息
            CMsgTaskDetail msg;			
			IF_OK(msg.CreateMsg(_TASK_DETAIL_ADD, m_pOwner, iter->second))
            {
                m_pOwner->SendMsgToClient(&msg);
            }

			LOGDEBUG("[CTaskDetailManager::GetTaskDetailListInfo] CreateMsg m_unId=[%d] m_unUid=[%d] TaskId=[%d] TaskPhase=[%d] CompleteNum=[%d] TaskBeginTime=[%d] TaskData=[%d]",
				(iter->second)->GetId(),(iter->second)->GetUserId(),(iter->second)->GetTaskId(),(iter->second)->GetTaskPhase(),(iter->second)->GetCompleteNum(),(iter->second)->GetTaskBeginTime(),(iter->second)->GetTaskData());
    	}
    }

    return TRUE;
}

//插入数据库记录
UINT CTaskDetailManager::InsertRecord(UINT nTaskId, UINT nTaskPhase, UINT nCompleteNum, UINT nTaskBeginTime, UINT nTaskData)
{
    UINT nId = 0;
	if (!Database())
    { 
    	LOGERROR("[CTaskDetailManager::InsertRecord] Database==NULL");
        return nId;
    }

	if (!m_pOwner)
    { 
    	LOGERROR("[CTaskDetailManager::InsertRecord] m_pOwner==NULL");
        return nId;
    }

    time_t ltime;
    time(&ltime);
    tm*    pTm = localtime(&ltime);
    char    buf[128] = {0};
    sprintf(buf, "%04d-%02d-%02d %02d:%02d:%02d", pTm->tm_year, pTm->tm_mon, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);

    //nTaskBeginTime = DateStamp();

    UINT nUid = m_pOwner->GetUid();
    char szSql[1024];
    memset(szSql, 0, 1024);
    sprintf(szSql, "INSERT INTO g_bumper_task (user_id, task_id, task_phase, task_complete_num, task_begin_time, task_data) VALUES (%d,%d,%d,%d,%d,%d)", nUid, nTaskId, nTaskPhase, nCompleteNum, nTaskBeginTime, nTaskData);
    if(Database()->UpdateRecord(szSql)) 
    {
        nId = Database()->GetInsertId();
        LOGDEBUG("[CTaskDetailManager::InsertRecord] Is Suc... nId=[%d] sql=[%s]\n", nId, szSql);
    }
    else 
    {
        LOGERROR("[CTaskDetailManager::InsertRecord] Is Error... sql=[%s]\n", szSql);
        return nId;
    }
    return nId;
}

//删除数据库记录
BOOL CTaskDetailManager::DeleteRecord(UINT nId)
{
	if (!Database())
    { 
    	LOGERROR("[CTaskDetailManager::DeleteRecord] Database==NULL");
        return FALSE;
    }

    if (!m_pOwner)
    { 
    	LOGERROR("[CTaskDetailManager::DeleteRecord] m_pOwner==NULL");
        return FALSE;
    }

    char szSql[1024];
    memset(szSql, 0, 1024);
    sprintf(szSql, "DELETE FROM g_bumper_task WHERE id=%d", nId);
    if(Database()->UpdateRecord(szSql)) 
    {
        LOGDEBUG("[CTaskDetailManager::DeleteRecord] Is Suc... sql=[%s]\n", szSql);
    }
    else 
    {
        LOGERROR("[CTaskDetailManager::DeleteRecord] Is Error... sql=[%s]\n", szSql);
        return FALSE;
    }
    return TRUE;
}

//添加任务
BOOL CTaskDetailManager::AddTaskDetailData(CTaskDetailData* pData, BOOL bSynMsg/*=TRUE*/)
{
	if (!Database())
    { 
    	LOGERROR("[CTaskDetailManager::AddTaskDetailData] Database==NULL");
        return FALSE;
    }

    if (!pData)
    { 
    	LOGERROR("[CTaskDetailManager::AddTaskDetailData] pData==NULL");
        return FALSE;
    }

    if (!m_pOwner)
    { 
    	LOGERROR("[CTaskDetailManager::AddTaskDetailData] m_pOwner==NULL");
        return FALSE;
    }

	//插入数据库
    INT nNewId = InsertRecord(pData->GetTaskId(), pData->GetTaskPhase(), pData->GetCompleteNum(), pData->GetTaskBeginTime(), pData->GetTaskData());
	if(nNewId == 0)
	{
		return FALSE;
	}

	//插入成功设置数据库的ID
	pData->SetId(nNewId);

	//插入容器
	m_mapTaskDetailDataSet[pData->GetId()] = pData;

	//同步信息
    if (bSynMsg)
    {
        CMsgTaskDetail msg;
        if(msg.CreateMsg(_TASK_DETAIL_ADD, m_pOwner, pData))
        {
            m_pOwner->SendMsgToClient(&msg);
        }
    }

	LOGDEBUG("[CTaskDetailManager::AddTaskDetailData] Is Suc...Id=[%d] m_unUid=[%d] TaskId=[%d] TaskPhase=[%d] CompleteNum=[%d] TaskBeginTime=[%d] TaskData=[%d]",
		pData->GetId(),pData->GetUserId(),pData->GetTaskId(),pData->GetTaskPhase(),pData->GetCompleteNum(),pData->GetTaskBeginTime(),pData->GetTaskData());

    return TRUE;
}

//删除任务数据
BOOL CTaskDetailManager::DelTaskDetailData(UINT nId)
{
	CHECKF(nId);
    CHECKF(m_pOwner);

	for (auto iter = m_mapTaskDetailDataSet.begin(); iter != m_mapTaskDetailDataSet.end(); iter++)
    {	
    	//找到对应玩家
    	if((iter->second)->GetId() ==  nId)
    	{
    		if(DeleteRecord(nId))
    		{
                //同步信息
                BOOL bSynMsg = TRUE;
                if (bSynMsg)
                {
                    CMsgTaskDetail msg;
                    if(msg.CreateMsg(_TASK_DETAIL_DEL, m_pOwner, iter->second))
                    {
                        m_pOwner->SendMsgToClient(&msg);
                    }
                }

                //释放内存
    			SAFE_RELEASE(iter->second);
                //清理容器
    			m_mapTaskDetailDataSet.erase(iter);
    			return TRUE;
    		}    		
    	}
    }

    return FALSE;
}

//查找任务数据
CTaskDetailData* CTaskDetailManager::QueryTaskDetailDataById(UINT nId)
{
    CHECKF(nId);

    for (auto iter = m_mapTaskDetailDataSet.begin(); iter != m_mapTaskDetailDataSet.end(); iter++)
    {   
        //找到对应任务
        if((iter->second)->GetId() ==  nId)
        {
            return (iter->second);                 
        }
    }
    return NULL;
}

//查找任务数据
CTaskDetailData* CTaskDetailManager::QueryTaskDetailDataByTaskId(UINT nTaskId)
{
	CHECKF(nTaskId);

	for (auto iter = m_mapTaskDetailDataSet.begin(); iter != m_mapTaskDetailDataSet.end(); iter++)
    {	
    	//找到对应任务
    	if((iter->second)->GetTaskId() ==  nTaskId)
    	{
    		return (iter->second);    		   		
    	}
    }
    return NULL;
}