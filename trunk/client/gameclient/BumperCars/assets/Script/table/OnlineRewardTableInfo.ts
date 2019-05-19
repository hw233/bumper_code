import BaseTableInfo from './BaseTableInfo';
import BaseTableReader from './BaseTableReader';
import Util from '../common/Util';

export default class OnlineRewardTableInfo extends BaseTableInfo
{
    //任务奖励类型
    private m_TaskRewardType: Array<number> = null;

    //任务奖励数量
    private m_TaskRewardNum: Array<number>  = null;

    public constructor(ID: number, rewardType: string, rewardNum: string)
    {
        super(ID);

        this.m_TaskRewardType = Util.StringConvertToArray(rewardType);
        this.m_TaskRewardNum = Util.StringConvertToArray(rewardNum);
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

export class OnlineRewardTableReader extends BaseTableReader
{
    public constructor(filename: string)
    {
        super(filename);
    }

    public ReadOneData(id: number): BaseTableInfo
    {
        this.SetID(id);
        
        return new OnlineRewardTableInfo(
            this.GetNumber("id"),
            this.GetString("type"),
            this.GetString("reward"),
        );
    }
}