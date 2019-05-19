//////////////////////////////////////////////////////////////////////
//文件名称：CTaskDetailCfgManager.cpp
//功能描述：
//版本说明：CTaskDetailCfgManager API
//			
//编写作者：2018.12.28 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#include "../../DataBase/DBMySql.h"
#include "../I_mydb.h"
#include "../../MsgHandler/AllMsg.h"
#include "../Player.h"
#include "TaskDetailCfgManager.h"


//////////////////////////////////////////////////////////////////////
//常量预定于
//char	szTaskDetailTable[] = _TBL_TASKDETAIL;
CTaskDetailCfgManager* g_pTaskDetailCfgManager = NULL;

//////////////////////////////////////////////////////////////////////
//构造
CTaskDetailCfgManager::CTaskDetailCfgManager()
{
	m_mapTaskDetailCfgSet.clear();
}

//析构函数
CTaskDetailCfgManager::~CTaskDetailCfgManager()
{ 
	for (auto iter = m_mapTaskDetailCfgSet.begin(); iter != m_mapTaskDetailCfgSet.end(); iter++)
    {	
		if(iter->second)
		{
			SAFE_RELEASE(iter->second);
		} 
    }

    m_mapTaskDetailCfgSet.clear();
}

BOOL CTaskDetailCfgManager::Init()
{
    for (auto iter = m_mapTaskDetailCfgSet.begin(); iter != m_mapTaskDetailCfgSet.end(); iter++)
    {   
        if(iter->second)
        {
            SAFE_RELEASE(iter->second);
        } 
    }

    m_mapTaskDetailCfgSet.clear();

    return this->LoadInfo();
}

//加载itemtype配置文件信息
BOOL CTaskDetailCfgManager::LoadInfo() 
{
    if (!Database())
    { 
         return FALSE;
    }

    std::string sql = string("SELECT * FROM g_bumper_task_detail");  
    MYSQL_RES* result = Database()->Query(sql);
    if(result == NULL)
    {
        LOGERROR("[CTaskDetailCfgManager::LoadInfo] is not exist g_bumper_task_detail sql error: %s\n", sql.c_str());
        return FALSE;
    }
    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)))
    {    
    	CTaskDetailCfg* pInfo = CTaskDetailCfg::CreateNew();
    	if (pInfo)
    	{         
            pInfo->SetTaskId(SafeAtoi(row[TASKDETAIL_CFG_TASKID]));
            pInfo->SetTaskPhase(SafeAtoi(row[TASKDETAIL_CFG_TASKPHASE]));
            pInfo->SetCompleteNum(SafeAtoi(row[TASKDETAIL_CFG_TASKCOMPLETENUM]));
            pInfo->SetMask1(SafeAtoi(row[TASKDETAIL_CFG_MASK_1]));
            pInfo->SetMask2(SafeAtoi(row[TASKDETAIL_CFG_MASK_2]));
            pInfo->SetTaskData(SafeAtoi(row[TASKDETAIL_CFG_TASKDATA]));

	        //插入容器
	        m_mapTaskDetailCfgSet[pInfo->GetTaskId()] = pInfo;
	        
	        LOGDEBUG("[CTaskDetailCfgManager::LoadInfo] size=[%d] TaskId=[%d] TaskPhase=[%d] CompleteNum=[%d] Mask1=[%d] Mask2=[%d] TaskData=[%d]", 
            	m_mapTaskDetailCfgSet.size(), pInfo->GetTaskId(), pInfo->GetTaskPhase(), pInfo->GetCompleteNum(), pInfo->GetMask1(), pInfo->GetMask2(), pInfo->GetTaskData());
    	}
    }
    mysql_free_result(result);
    result = NULL;
    LOGDEBUG("[CTaskDetailCfgManager::LoadInfo] Loading table g_bumper_taskd_etail end");
    return TRUE;
    
}

//查询道具接口
CTaskDetailCfg* CTaskDetailCfgManager::QueryTaskDetailCfgInfo(UINT nTaskId)
{
    for (auto iter = m_mapTaskDetailCfgSet.begin(); iter != m_mapTaskDetailCfgSet.end(); iter++)
    {   
        //找到对应玩家
        if((iter->second)->GetTaskId() ==  nTaskId)
        {
            return (iter->second);                 
        }
    }
    return NULL;
}

//检测日常任务是否有更新
BOOL CTaskDetailCfgManager::CheckDayTaskDetailRecord(Player* pPlayer)
{
    CHECKF(pPlayer);

    CTaskDetailManager* pTaskDetailManager = pPlayer->QueryTaskDetailManager();
    if(pTaskDetailManager)
    {
        for (auto iter = m_mapTaskDetailCfgSet.begin(); iter != m_mapTaskDetailCfgSet.end(); ++iter)
        {   
            //找到所有日常类型的任务
            UINT nTaskId = (iter->second)->GetTaskId();
            if((iter->second))// && (nTaskId/1000 == DAY_TASK_DETAIL_TYPE || nTaskId/1000 == DAY_TASK_DETAIL_TRY_GAME_TYPE))
            {
                //不存在此日常任务
                if (!pTaskDetailManager->QueryTaskDetailDataByTaskId(nTaskId))
                {
                    //如果永久任务掩码位已经设置位1了，证明已经做过此任务领取过奖励了不再插入相关任务
                    if (nTaskId / 1000 == PERMANENT_TASK_DETAIL_TYPE)   //永久任务-一次性-单次奖励
                    {
                        UINT nMaskIdx = (iter->second)->GetMask1();
                        if (pPlayer->ChkTaskMask(nMaskIdx))
                        {
                            LOGDEBUG("[CTaskDetailCfgManager::CheckDayTaskDetailRecord] Hava Finish");
                            continue;
                        }
                    }
                    
                    //插入数据库
                    CTaskDetailData* pData = new CTaskDetailData();
                    if(!pData)
                    {
                        LOGWARNING("[CTaskDetailCfgManager::CheckDayTaskDetailRecord] pData Is NULL");
                        continue;
                    }

                    // //创建对应数据结构对象
                    // stTaskDetailData pInfo;
                    // memset(&pInfo, 0, sizeof(stTaskDetailData));
                    // //pInfo.unId = 0;
                    // pInfo.unUserId = pPlayer->GetUid();
                    // pInfo.unTaskId = nTaskId;
                    // pInfo.unTaskPhase = (iter->second)->GetTaskPhase();
                    // pInfo.unCompleteNum = (iter->second)->GetCompleteNum();
                    // pInfo.unTaskBeginTime = DateStamp();
                    // pInfo.unTaskData = (iter->second)->GetTaskData();

                    // //插入数据
                    // UINT nNewId = pData->CreateRecord(&pInfo);
                    // if (nNewId > 0)
                    // {
                    //     pTaskDetailManager->AddTaskDetailData(pData);
                    // }
                    // else
                    // {
                    //     SAFE_RELEASE(pData);
                    //     pData = NULL;
                    // }
                    
                    //设置对象属性
                    pData->SetUserId(pPlayer->GetUid());
                    pData->SetTaskId(nTaskId);
                    pData->SetTaskPhase(0, FALSE, TRUE);
                    pData->SetCompleteNum(0, FALSE, TRUE);
                    pData->SetTaskBeginTime(DateStamp(), FALSE, TRUE);
                    pData->SetTaskData((iter->second)->GetTaskData(), FALSE, TRUE);
                    
                    //添加到任务管理器并且插入数据
                    if(!pTaskDetailManager->AddTaskDetailData(pData, FALSE))
                    {
                        SAFE_RELEASE(pData);
                    }
                }                
            }
        }

        return TRUE;
    }

    return FALSE;
}