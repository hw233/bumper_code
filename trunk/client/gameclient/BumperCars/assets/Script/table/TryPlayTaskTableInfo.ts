import BaseTableInfo from './BaseTableInfo';
import BaseTableReader from './BaseTableReader';
import Util from '../common/Util';

export default class TryPlayTaskTableInfo extends BaseTableInfo
{
    //试玩游戏ID
    private m_nGameID: number               = null;

    //任务奖励类型
    private m_TaskRewardType: Array<number> = null;

    //任务奖励数量
    private m_TaskRewardNum: Array<number>  = null;

    public constructor(taskID: number, gameid: number, taskRewardType: string, taskRewardNum: string)
    {
        super(taskID);

        this.m_nGameID = gameid;

        this.m_TaskRewardType = Util.StringConvertToArray(taskRewardType);
        this.m_TaskRewardNum = Util.StringConvertToArray(taskRewardNum);
    }

    public GetGameID(): number
    {
        return this.m_nGameID;
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

export class TryPlayTaskTableReader extends BaseTableReader
{
    public constructor(filename: string)
    {
        super(filename);
    }

    public ReadOneData(id: number): BaseTableInfo
    {
        this.SetID(id);

        return new TryPlayTaskTableInfo(
            this.GetNumber("TaskID"),
            this.GetNumber("GameID"),
            this.GetString("RewardType"),
            this.GetString("RewardNum"),
        );
    }
}