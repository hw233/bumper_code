import BaseTableInfo from './BaseTableInfo';
import BaseTableReader from './BaseTableReader';
import Util from '../common/Util';

export default class BoxTableInfo extends BaseTableInfo
{
    //领取宝箱所需的签到天数
    private m_SignInDays: number = null;

    //任务奖励类型
    private m_TaskRewardType: Array<number> = null;

    //任务奖励数量
    private m_TaskRewardNum: Array<number>  = null;

    public constructor(id: number, dasy: number, rewardtype: string, rewardnum: string)
    {
        super(id);

        this.m_SignInDays = dasy;
        this.m_TaskRewardType = Util.StringConvertToArray(rewardtype);
        this.m_TaskRewardNum = Util.StringConvertToArray(rewardnum);
    }

    public GetSignInDays(): number
    {
        return this.m_SignInDays;
    }

    public GetTaskRewardType(): Array<number>
    {
        return this.m_TaskRewardType;
    }

    public GetTaskRewardNum(): Array<number>
    {
        return this.m_TaskRewardNum;
    }
}

export class BoxTableReader extends BaseTableReader
{
    public constructor(filename: string)
    {
        super(filename);
    }

    public ReadOneData(id: number): BaseTableInfo
    {
        this.SetID(id);

        return new BoxTableInfo(
            this.GetNumber("id"),
            this.GetNumber("days"),
            this.GetString("type"),
            this.GetString("reward"),
        );
    }
}