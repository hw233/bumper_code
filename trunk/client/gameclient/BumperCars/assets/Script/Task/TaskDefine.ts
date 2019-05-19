export const TaskDefine = 
{
    TD_PermanentTask_Begin      : 1000001,     //永久任务ID——Begin
    TD_PermanentTask_End        : 1001000,     //永久任务ID——End

    TD_DailyTask_Begin          : 2000001,      //日常任务ID——Begin
    TD_DailyTask_End            : 2001000,      //日常任务ID——End

    TD_TryPlayTask_Begin        : 3000001,      //试玩任务ID——Begin
    TD_TryPlayTask_End          : 3001000,      //试玩任务ID——End
}

//任务类型
export enum TaskType
{
    TT_PermanentTask        = 1,    //永久任务
    TT_DailyTask            = 2,    //日常任务
    TT_TryPlayTask          = 3,    //试玩任务
};

//任务状态
export enum TaskState{
    TS_ToFinish         = 0,    //去完成
    TS_Receive          = 1,    //领取奖励
    TS_AgainReceive     = 2,    //再领一次  
    Ts_Finished         = 3,    //已完成
}