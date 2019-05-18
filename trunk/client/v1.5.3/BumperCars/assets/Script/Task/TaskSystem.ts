import EventCenter from '../common/EventCenter';
import GameTable from '../manager/TableManger';
import GameUI from '../manager/UIManager';
import TaskData from './TaskData';
import TaskManager from './TaskManager';
import TaskTableInfo from '../table/TaskTableInfo';
import { MessageType } from '../common/EventCenter';
import { RewardData } from '../data/RewardData';
import { TableType } from '../table/TableDefine';
import { TaskDefine, TaskState, TaskType } from './TaskDefine';
import { UIType } from '../UI/UIDefine';

//任务系统
export default class TaskSystem
{
    private static instance: TaskSystem              = null;

    private static EventTag: string                  = "TaskSystem";

    private m_IsGetTask: boolean = false;

    constructor()
    {
        this.m_IsGetTask = false;
    }

    public Init()
    {
        EventCenter.RegisterEvent(TaskSystem.EventTag, MessageType.Task_Add_Task_Event, this.OnAddTask.bind(this));
        EventCenter.RegisterEvent(TaskSystem.EventTag, MessageType.Task_Get_TaskList_Event, this.OnGetTaskList.bind(this));
        EventCenter.RegisterEvent(TaskSystem.EventTag, MessageType.Task_Update_Task_Event, this.OnUpdateTask.bind(this));
        EventCenter.RegisterEvent(TaskSystem.EventTag, MessageType.Task_Receive_Reward_Event, this.OnRecvReward.bind(this));
    }

    //获取任务列表成功
    private OnGetTaskList(): void
    {
        this.m_IsGetTask = true;

        this.HintNewMessage();
    }

    //添加任务
    private OnAddTask(data: any): void
    {
        let taskData: TaskData = TaskManager.GetInstance().GetTaskData(data.soleID);
        if(taskData)
        {
            this.OnUpdateTask(data);
            return;
        }

        let taskTable: any = null;
        let taskType: number = this.GetTaskTypeByTaskID(data.taskID);

        taskData = new TaskData();
        taskData.SetSoleID(data.soleID);
        taskData.SetTaskProgress(data.progress);
        taskData.SetTaskType(taskType);

        if(taskType === TaskType.TT_DailyTask || taskType === TaskType.TT_PermanentTask)
        {
            taskTable = <TaskTableInfo>GameTable.GetInstance().GetDataInfo(TableType.TTD_TaskType, data.taskID);
            if(taskTable)
            {
                taskData.SetTaskTarget(taskTable.GetTaskTarget());
                taskData.SetTaskDes(taskTable.GetTaskDes());
                taskData.SetSkipViewID(taskTable.GetViewID());
                taskData.SetSkipViewType(taskTable.GetSkipPaream());

                TaskManager.GetInstance().AddDailyTask(taskData);
            }
        }
        else if(taskType === TaskType.TT_TryPlayTask)
        {
            taskTable = GameTable.GetInstance().GetDataInfo(TableType.TTD_TryTaskType, data.taskID);
            if(taskTable)
            {
                taskData.SetTaskTarget(1);
                taskData.SetCustom(Number(taskTable.GetGameID()));
                TaskManager.GetInstance().AddTryPlayTask(taskData);
            }
        }

        if(taskTable)
        {
            taskData.SetTaskID(taskTable.GetID());

            let rewardList: Array<any> = taskTable.GetTaskRewardType();
            let rewardNum: Array<any> = taskTable.GetTaskRewardNum();
            for(let j = 0; j < rewardList.length; j ++)
            {
                let rewardData: RewardData = new RewardData();
                rewardData.ItemID = rewardList[j];
                rewardData.ItemNum = rewardNum[j];
                taskData.addReward(rewardData);
            }
        }

        this.TaskStateConvert(taskData, data.state);
    }

    //更新任务
    private OnUpdateTask(data: any): void
    {
        let taskData: TaskData = TaskManager.GetInstance().GetTaskData(data.soleID);
        if(taskData)
        {
            taskData.SetTaskState(data.state);
            taskData.SetTaskProgress(data.progress);
        }
        this.HintNewMessage();

        this.TaskStateConvert(taskData, data.state);

        EventCenter.DispatchEvent(MessageType.TaskView_RefreshView);
    }

    //领取任务奖励成功
    private OnRecvReward(data: any): void
    {
        let taskData: TaskData = TaskManager.GetInstance().GetTaskData(data.soleID);
        if(taskData)
        {
            if(taskData.GetTaskType() === TaskType.TT_TryPlayTask || taskData.GetTaskType() === TaskType.TT_PermanentTask)
            {
                taskData.SetTaskState(TaskState.Ts_Finished);
            }
            else
            {
                //日常任务和永久任务可以多领一次
                if(data.type === 0)
                {
                    taskData.SetTaskState(TaskState.TS_AgainReceive);
                }
                else if(data.type === 1)
                {
                    taskData.SetTaskState(TaskState.Ts_Finished);
                }
            }

            let rewardList: Array<RewardData> = taskData.GetRewardList();

            if(rewardList && rewardList.length > 0)
            {
                for(let i = 0, len = rewardList.length; i < len; i ++)
                {
                    let reward = rewardList[i];
                    if(reward)
                    {
                        GameUI.AddViewToQuene(UIType.UT_RewardView, {
                            itemID: reward.ItemID,
                            itemNum: reward.ItemNum,
                        })
                    }
                }

                GameUI.ShowViewByQuene();
            }
        }
        this.HintNewMessage();

        EventCenter.DispatchEvent(MessageType.TaskView_RefreshView);
        EventCenter.DispatchEvent(MessageType.StartScene_RefreshTopView);
    }

    public SetIsGetTask(value: boolean): void
    {
        this.m_IsGetTask = value;
    }

    public IsGetTask(): boolean
    {
        return this.m_IsGetTask;
    }

    public GetTaskTypeByTaskID(taskID: number): TaskType
    {
        if(taskID >= TaskDefine.TD_PermanentTask_Begin && taskID <= TaskDefine.TD_PermanentTask_End)
        {
            return TaskType.TT_PermanentTask;
        }

        if(taskID >= TaskDefine.TD_DailyTask_Begin && taskID <= TaskDefine.TD_DailyTask_End)
        {
            return TaskType.TT_DailyTask;
        }

        if(taskID >= TaskDefine.TD_TryPlayTask_Begin && taskID <= TaskDefine.TD_TryPlayTask_End)
        {
            return TaskType.TT_TryPlayTask;
        }
    }

    //新消息提示
    public HintNewMessage(): void
    {
        let isHasNewMsg: boolean = false;

        let count  = TaskManager.GetInstance().GetTaskCount();
        for(let i = 0; i < count;i ++)
        {
            let taskData: TaskData = TaskManager.GetInstance().GetTaskDataByIndex(i);
            if(taskData)
            {
                if(taskData.GetTaskState() === TaskState.TS_ToFinish || taskData.GetTaskState() === TaskState.TS_AgainReceive || taskData.GetTaskState() === TaskState.TS_Receive)
                {
                    isHasNewMsg = true;
                    break;
                }
            }
        }

        EventCenter.DispatchEvent(MessageType.NewMessage_Task, isHasNewMsg);
    }

    private TaskStateConvert(taskData: TaskData, stateData: number)
    {
        if(taskData)
        {
            if(taskData.GetTaskType() === TaskType.TT_TryPlayTask || taskData.GetTaskType() === TaskType.TT_PermanentTask)
            {
                if(stateData === 1)
                {
                    taskData.SetTaskState(TaskState.Ts_Finished);
                    return;
                }
            }

            if(taskData.GetTaskProgress() >= taskData.GetTaskTarget())
            {
                //已完成
                let taskState: TaskState = TaskState.TS_ToFinish;
                if(stateData === 0)
                {
                    taskState = TaskState.TS_Receive;
                }
                else if(stateData === 1)
                {
                    taskState = TaskState.TS_AgainReceive;
                }
                else if(stateData === 2)
                {
                    taskState = TaskState.Ts_Finished;
                }
                taskData.SetTaskState(taskState);
            }
            else
            {
                //未完成
                taskData.SetTaskState(TaskState.TS_ToFinish);
            }
        }
    }

    //获取一个未完成的试玩任务
    public GetNotComplateTryTask()
    {
        let count: number = TaskManager.GetInstance().GetTryPlayTaskCount();
        for(let i = 0;i < count;i ++)
        {
            let taskID: number = TaskManager.GetInstance().GetTryPlayTaskDataByIndex(i);
            let taskData: TaskData = TaskManager.GetInstance().GetTaskData(taskID);
            if(taskData)
            {
                if(taskData.GetTaskState() === TaskState.TS_ToFinish)
                {
                    return taskData.GetCustom();
                }
            }
        }

        let taskID2: number = TaskManager.GetInstance().GetTryPlayTaskDataByIndex(0);
        let taskData2: TaskData = TaskManager.GetInstance().GetTaskData(taskID2);
        if(taskData2)
        {
            return taskData2.GetCustom();
        }
    }
    
    public static GetInstance(): TaskSystem
    {
        if(!this.instance)
        {
            this.instance = new TaskSystem;
        }
        return this.instance;
    }

    public static Destroy()
    {
        if(this.instance)
        {
            this.instance = null;
        }
        EventCenter.RemoveEvent(TaskSystem.EventTag);
    }
}