import BaseTableInfo from './BaseTableInfo';
import BaseTableReader from './BaseTableReader';
import Util from '../common/Util';

export default class SignInTableInfo extends BaseTableInfo
{
    //任务奖励类型
    private m_TaskRewardType: number = null;

    //任务奖励数量
    private m_TaskRewardNum: number  = null;

    public constructor(id: number, rewardtype: number, rewardnum: number)
    {
        super(id);

        this.m_TaskRewardType = rewardtype;
        this.m_TaskRewardNum = rewardnum;
    }

    public GetTaskRewardType(): number
    {
        return this.m_TaskRewardType;
    }

    public GetTaskRewardNum(): number
    {
        return this.m_TaskRewardNum;
    }
}

export class SignInTableReader extends BaseTableReader
{
    public constructor(filename: string)
    {
        super(filename);
    }

    public ReadOneData(id: number): BaseTableInfo
    {
        this.SetID(id);

        return new SignInTableInfo(
            this.GetNumber("id"),
            this.GetNumber("type"),
            this.GetNumber("reward"),
        );
    }
}