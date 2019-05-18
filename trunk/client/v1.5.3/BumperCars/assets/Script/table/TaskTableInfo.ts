import BaseTableInfo from './BaseTableInfo';
import BaseTableReader from './BaseTableReader';
import Util from '../common/Util';

export default class TaskTableInfo extends BaseTableInfo
{
    //任务描述
    private m_TaskDes: string               = null;

    //任务条件ID
    private m_TaskCon: number               = null;

    //任务目标
    private m_TaskTarget: number            = null;

    //任务奖励类型
    private m_TaskRewardType: Array<number> = null;

    //任务奖励数量
    private m_TaskRewardNum: Array<number>  = null;

    //任务跳转界面
    private m_TaskSkipViewID: number        = null;

    //任务跳转参数
    private m_TaskSkipParam: any            = null;

    public constructor(taskID: number, taskDes: string, taskCon: number, taskTarget: number, taskRewardType: string, taskRewardNum: string, viewID: number, skipParam: any)
    {
        super(taskID);

        this.m_TaskDes = taskDes;
        this.m_TaskCon = taskCon;
        this.m_TaskTarget = taskTarget;
        this.m_TaskSkipViewID = viewID;
        this.m_TaskSkipParam = skipParam;
        this.m_TaskRewardType = Util.StringConvertToArray(taskRewardType);
        this.m_TaskRewardNum = Util.StringConvertToArray(taskRewardNum);
    }

    public GetTaskDes(): string
    {
        return this.m_TaskDes;
    }

    public GetTaskCon(): number
    {
        return this.m_TaskCon;
    }

    public GetTaskTarget(): number
    {
        return this.m_TaskTarget;
    }

    public GetTaskRewardType(): Array<number>
    {
        return this.m_TaskRewardType;
    }

    public GetTaskRewardNum(): Array<number>
    {
        return this.m_TaskRewardNum;
    }

    public GetViewID(): number
    {
        return this.m_TaskSkipViewID;
    }

    public GetSkipPaream(): number
    {
        return this.m_TaskSkipParam;
    }
}

export class TaskTableReader extends BaseTableReader
{
    public constructor(filename: string)
    {
        super(filename);
    }

    public ReadOneData(id: number): BaseTableInfo
    {
        this.SetID(id);

        return new TaskTableInfo(
            this.GetNumber("TaskID"),
            this.GetString("TaskDes"),
            this.GetNumber("TaksConID"),
            this.GetNumber("TaskTarget"),
            this.GetString("RewardType"),
            this.GetString("RewardNum"),
            this.GetNumber("SkipView"),
            this.GetNumber("SkipParam"),
        );
    }
}