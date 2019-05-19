//////////////////////////////////////////////////////////////////////
//文件名称：CTaskDetailData.h
//功能描述：
//版本说明：CTaskDetailData API
//			
//编写作者：2018.11.6 yanfeng.hu     , fliu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#ifndef __TASKDETAILDATA_H__
#define __TASKDETAILDATA_H__

#include "../../Common/Type.h"
#include "../../Common/BaseType.h"
#include "../../Common/Config.h"
#include "../../Common/Common.h"
#include <vector>
using namespace std;

#define TASKSELECTED_LIMIT		65535
#define COMPLETENUM_LIMIT 		65535
#define PHASE_LIMIT 			65535 
#define SHORT_LIMIT				65536
#define BYTE_LIMIT				255
#define TASKROUNDNUM_LIMIT              BYTE_LIMIT //任务的轮数的限制
#define TASKALLROUNDCOMPLETENUM_LIMIT   BYTE_LIMIT //所有轮的完成次数

#define PERMANENT_TASK_DETAIL_TYPE   			1000 		//永久任务类型-任务编号 1000 001~999 支持999种永久任务
#define DAY_TASK_DETAIL_TYPE   					2000 		//日常任务类型-任务编号 2000 001~999 支持999种日常任务
#define DAY_TASK_DETAIL_TRY_GAME_TYPE   		3000 		//试玩任务类型-任务编号 3000 001~999 支持999种试玩任务

//////////////////////////////////////////////////////////////////////
///日常任务id   编号 1000001 ~ 1000099
/*
1000001,0000,0000,0001,0000,0000,"添加到微信小程序"
2000001,0000,0001,0012,0013,0000,"每日签到"
2000002,0000,0003,0014,0015,0000,"完成3次试玩任务"
2000003,0000,0010,0016,0017,0000,"达成10次击杀"
2000004,0000,0001,0018,0019,0000,"完成1次双杀"
2000005,0000,0001,0020,0021,0000,"完成1次三杀"
2000006,0000,0001,0022,0023,0000,"完成1次大乱斗"
2000007,0000,0003,0024,0025,0000,"完成3次大乱斗"
2000008,0000,0001,0026,0027,0000,"完成1次组队战"
2000009,0000,0003,0028,0029,0000,"完成3次组队战"
2000010,0000,0001,0030,0031,0000,"获得1次大乱斗排名第1"
3000001,0000,0001,0032,0000,0000,"保卫萝卜1"
3000002,0000,0001,0033,0000,0000,"保卫萝卜2"
3000003,0000,0001,0034,0000,0000,"保卫萝卜3"
3000004,0000,0001,0035,0000,0000,"保卫萝卜4"
3000005,0000,0001,0036,0000,0000,"保卫萝卜5"


*/
const INT PERMANENT_TASK_DETAIL_TYPE_1 		= 1000001;							// 添加到微信小程序
const INT DAY_TASK_DETAIL_TYPE_1   			= 2000001;                        	// 每日签到
const INT DAY_TASK_DETAIL_TYPE_2   			= 2000002;                        	// 完成3次试玩任务
const INT DAY_TASK_DETAIL_TYPE_3   			= 2000003;                        	// 达成10次击杀
const INT DAY_TASK_DETAIL_TYPE_4   			= 2000004;                        	// 完成1次双杀
const INT DAY_TASK_DETAIL_TYPE_5   			= 2000005;                        	// 完成1次三杀
const INT DAY_TASK_DETAIL_TYPE_6   			= 2000006;                        	// 完成1次大乱斗
const INT DAY_TASK_DETAIL_TYPE_7   			= 2000007;                        	// 完成3次大乱斗
const INT DAY_TASK_DETAIL_TYPE_8   			= 2000008;                        	// 完成1次组队战
const INT DAY_TASK_DETAIL_TYPE_9   			= 2000009;                        	// 完成3次组队战
const INT DAY_TASK_DETAIL_TYPE_10  			= 2000010;                        	// 获得1次大乱斗排名第1
const INT DAY_TASK_DETAIL_TRY_GAME_TYPE_1	= 3000001;							// 保卫萝卜1						
const INT DAY_TASK_DETAIL_TRY_GAME_TYPE_2	= 3000002;							// 保卫萝卜2
const INT DAY_TASK_DETAIL_TRY_GAME_TYPE_3	= 3000003;							// 保卫萝卜3
const INT DAY_TASK_DETAIL_TRY_GAME_TYPE_4	= 3000004;							// 保卫萝卜4
const INT DAY_TASK_DETAIL_TRY_GAME_TYPE_5	= 3000005;							// 保卫萝卜5


//////////////////////////////////////////////////////////////////////
enum TASKDETAILDATA 
{
	TASKDETAILDATA_ID 					= 0,				// 任务ID  , 唯一标识task
	TASKDETAILDATA_USERID 				= 1,				// 玩家ID
	TASKDETAILDATA_TASKID 				= 2,				// 任务编号
	TASKDETAILDATA_TASKPHASE 			= 3,				// 任务阶段
	TASKDETAILDATA_TASKCOMPLETENUM 		= 4, 				// 任务完成次数
	TASKDETAILDATA_TASKBEGINTIME  		= 5,  				// 任务开始时间
	//TASKDETAILDATA_MASK_1 			= 6,				// 任务掩码位第一次领取奖励标识 0:未领取 1:已经领取
	//TASKDETAILDATA_MASK_2 			= 7,				// 任务掩码位再领一次奖励标识 0:未领取 1:已经领取
	TASKDETAILDATA_TASKDATA	      		=6,  				// 任意数据,意义可已自定义
};
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
typedef struct
{
	UINT    unId;
	UINT 	unUserId;
	UINT 	unTaskId;
	UINT	unTaskPhase;
	UINT	unCompleteNum; 
	UINT    unTaskBeginTime;
	//UINT    unMask1;
	//UINT    unMask2;
	UINT    unTaskData;		//任务的任意数据
}stTaskDetailData;
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
///前置声明
class DBMySql;
class Player;

class CTaskDetailData
{
public:
	CTaskDetailData();
	virtual ~CTaskDetailData();

public:
	static CTaskDetailData* CreateNew()	{	return new CTaskDetailData();	}
	BOOL	Release()					{ 	delete this; return TRUE; 		}
	//类构造函数调用-内部使用
	VOID 	Init();
	//OnTimer函数
	BOOL 	OnTimer();
	//设置归属者指针
	BOOL	SetOwner(Player* pPlayer)						{	CHECKF(pPlayer);  m_pOwner = pPlayer;  return TRUE;}
	//保存信息
	BOOL	SaveInfo(BOOL bUpdate = TRUE);

	stTaskDetailData&		QueryTaskDetailDataInfo() {return m_TaskDetailDataInfo;}

public:

	UINT	GetId()	{   return m_TaskDetailDataInfo.unId;   }
	BOOL	SetId(UINT unId)	{	m_TaskDetailDataInfo.unId = unId; return TRUE;	}

	UINT	GetUserId()	{ return m_TaskDetailDataInfo.unUserId; }
	BOOL	SetUserId(UINT idUser, BOOL bUpdate = FALSE);
	
	UINT	GetTaskId()	{ return     m_TaskDetailDataInfo.unTaskId; }
	BOOL	SetTaskId(UINT idTask, BOOL bUpdate = FALSE);

	UINT	GetTaskPhase() { return m_TaskDetailDataInfo.unTaskPhase;       }
	BOOL	SetTaskPhase(UINT taskPhase, BOOL bUpdate = FALSE, BOOL bHappenChange = FALSE);

	UINT	GetCompleteNum() { return m_TaskDetailDataInfo.unCompleteNum; }
	BOOL	SetCompleteNum(UINT completeNum, BOOL bUpdate = FALSE, BOOL bHappenChange = FALSE);

	UINT	GetTaskBeginTime() { return m_TaskDetailDataInfo.unTaskBeginTime; }
	BOOL	SetTaskBeginTime(UINT taskBeginTime, BOOL bUpdate = FALSE, BOOL bHappenChange = FALSE);

	//UINT	GetMask1() { return m_TaskDetailDataInfo.unMask1; }
	//BOOL	SetMask1(UINT unMask1, BOOL bUpdate = FALSE);

	//UINT	GetMask2() { return m_TaskDetailDataInfo.unMask2; }
	//BOOL	SetMask2(UINT unMask2, BOOL bUpdate = FALSE);
	
	UINT	GetTaskData() { return m_TaskDetailDataInfo.unTaskData; }
	BOOL	SetTaskData(UINT taskData, BOOL bUpdate = FALSE, BOOL bHappenChange = FALSE);
	
	UINT 	CreateRecord(const stTaskDetailData* pInfo);
    UINT    InsertRecord(const stTaskDetailData* pInfo);
    BOOL    DeleteRecord(const stTaskDetailData* pInfo);
	
protected:
	stTaskDetailData 	m_TaskDetailDataInfo;
	Player* 			m_pOwner;
	BOOL    			m_bUpdate;							// 数据是否有变更，有则进行下线数据库更新
};

#endif//__TASKDETAILDATA_H__
