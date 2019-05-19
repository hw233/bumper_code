import { RewardData } from '../data/RewardData';
//一条任务数据
export default class TaskData
{
    //任务唯一ID
    private m_nSoleID: number = null;

    //任务ID
    private m_nTaskID: number = null;

    //任务类型(试玩任务和每日任务)
    private m_nTaskType: number = null;

    //任务描述
    private m_strTaskDes: string = null;

    //任务进度
    private m_nTaskProgress: number = null;

    //任务目标
    private m_nTaskTarget: number = null;

    //任务状态(0: 去完成, 1: 领取奖励, 2：再领一次, 3：已完成)
    private m_nTaskState: number = null;

    //任务的奖励列表
    private m_RewardList: Array<RewardData> = null;

    //去完成任务对应的界面ID
    private m_nSkipViewID: number = null;

    private m_nSkipViewType: number = null;

    //自定义数据
    private m_Custom: any = null;

    constructor()
    {
        this.m_nSoleID = 0;
        this.m_nTaskID = 0;
        this.m_nTaskType = 0;
        this.m_nSkipViewID = 0;
        this.m_Custom = null;
        this.m_nTaskProgress = 0;
        this.m_nTaskTarget = 0;
        this.m_nTaskState = 0;
        this.m_RewardList = [];
        this.m_strTaskDes = "";
        this.m_nSkipViewType = 0;
    }

    public SetSoleID(id: number)
    {
        this.m_nSoleID = id;
    }

    public GetSoleID(): number
    {
        return this.m_nSoleID;
    } 

    public SetTaskID(id: number)
    {
        this.m_nTaskID = id;
    }

    private GetTaskID(): number
    {
        return this.m_nTaskID;
    }

    public SetTaskType(type: number): void
    {
        this.m_nTaskType = type;
    }

    public GetTaskType(): number
    {
        return this.m_nTaskType;
    }

    public SetSkipViewID(viewID: number): void
    {
        this.m_nSkipViewID = viewID;
    }

    public GetSkipViewID(): number
    {
        return this.m_nSkipViewID;
    }

    public SetSkipViewType(type: number): void
    {
        this.m_nSkipViewType = type;
    }

    public GetSkipViewType(): number
    {
        return this.m_nSkipViewType;
    }

    public SetTaskProgress(progress: number): void
    {
        this.m_nTaskProgress = progress;
    }

    public GetTaskProgress(): number
    {
        return this.m_nTaskProgress;
    }

    public SetTaskTarget(target: number): void
    {
        this.m_nTaskTarget = target;
    }

    public GetTaskTarget(): number
    {
        return this.m_nTaskTarget;
    }

    public addReward(reward: RewardData): void
    {
        if(reward)
        {
            this.m_RewardList.push(reward);
        }
    }

    public GetRewardList(): Array<RewardData>
    {
        return this.m_RewardList;
    }

    public SetCustom(custom: any): void
    {
        this.m_Custom = custom;
    }

    public GetCustom(): any
    {
        return this.m_Custom;
    }

    public SetTaskState(state: number): void
    {
        this.m_nTaskState = state;
    }

    public GetTaskState(): number
    {
        return this.m_nTaskState;
    }

    public SetTaskDes(des: string): void
    {
        this.m_strTaskDes = des;
    }

    public GetTaskDes(): string
    {
        return this.m_strTaskDes;
    }
}