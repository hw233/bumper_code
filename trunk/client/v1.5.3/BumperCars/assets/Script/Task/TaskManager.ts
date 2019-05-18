import TaskData from './TaskData';
import { HashMap } from '../common/HashMap';

//任务管理器
export default class TaskManager
{
    private static instance: TaskManager = null;

    //任务列表
    private m_TaskList: HashMap<number, TaskData> = null;

    //试玩任务列表
    private m_TryPlayTaskList: Array<number> = null;
    //日常任务列表
    private m_DailyTaskList: Array<number> = null;

    constructor()
    {
        this.m_TaskList = new HashMap<number, TaskData>();
        this.m_DailyTaskList = [];
        this.m_TryPlayTaskList = [];
    }

    public Init()
    {
    }

    //添加一条日常任务
    public AddDailyTask(taskData: TaskData): void
    {
        if(taskData)
        {
            this.m_DailyTaskList.push(taskData.GetSoleID());
            this.m_TaskList.Add(taskData.GetSoleID(), taskData);
        }
    }

    //添加一条试玩任务
    public AddTryPlayTask(taskData: TaskData): void
    {
        if(taskData)
        {
            this.m_TryPlayTaskList.push(taskData.GetSoleID());
            this.m_TaskList.Add(taskData.GetSoleID(), taskData);
        }
    }

    public GetTaskCount(): number
    {
        return this.m_TaskList.Size();
    }

    //根据唯一id返回一条任务
    public GetTaskData(soleID: number): TaskData
    {
        return this.m_TaskList.Get(soleID);
    }

    public GetTaskDataByIndex(index: number): TaskData
    {
        return this.m_TaskList.IndexOf(index);
    }

    public GetDailyTaskList(): Array<number>
    {
        return this.m_DailyTaskList;
    }

    public GetTryPlayTaskList(): Array<number>
    {
        return this.m_TryPlayTaskList;
    }

    //返回日常任务的数量
    public GetDailyTaskCount(): number
    {
        if(this.m_DailyTaskList)
        {
            return this.m_DailyTaskList.length;
        }
        return 0;
    }

    //返回试玩任务的数量
    public GetTryPlayTaskCount(): number
    {
        if(this.m_TryPlayTaskList)
        {
            return this.m_TryPlayTaskList.length;
        }
        return 0;
    }

    //根据索引返回一条日常任务
    public GetDailyTaskDataByIndex(index: number): number
    {
        if(this.m_DailyTaskList[index])
        {
            return this.m_DailyTaskList[index];
        }
        return null;
    }
    
    //根据索引返回一条试玩任务
    public GetTryPlayTaskDataByIndex(index: number): number
    {
        if(this.m_TryPlayTaskList[index])
        {
            return this.m_TryPlayTaskList[index];
        }
        return null;
    }

    public Destroy()
    {
        this.m_DailyTaskList.length = 0;
        this.m_TryPlayTaskList.length = 0;
        this.m_TaskList.Destroy();

        this.m_TaskList = null;
        this.m_DailyTaskList = null;
        this.m_TryPlayTaskList = null;
    }

    public static GetInstance(): TaskManager
    {
        if(!this.instance)
        {
            this.instance = new TaskManager;
        }
        return this.instance;
    }

    public static Destroy(): void
    {
        if(this.instance)
        {
            this.instance.Destroy();
            this.instance = null;
        }
    }
}