import { RewardData } from '../data/RewardData';
//宝箱数据
export default class BoxData
{
    private m_BoxID: number = null;

    //宝箱的奖励列表
    private m_RewardList: Array<RewardData> = null;

    //宝箱的领取状态(1: 未领取，2: 可领取 3：已领取)
    private m_nBoxState: number = null;

    constructor()
    {
        this.m_BoxID = 0;
        this.m_RewardList = [];
        this.m_nBoxState = 0;
    }

    //初始化宝箱数据
    public InitData(data: any)
    {

    }

    public SetBoxID(id: number): void
    {
        this.m_BoxID = id;
    }

    public GetBoxID(): number
    {
        return this.m_BoxID;
    }

    public AddReward(reward: RewardData): void
    {
        this.m_RewardList.push(reward);
    }

    //获取宝箱的奖励列表
    public GetRewardList(): Array<RewardData>
    {
        return this.m_RewardList;
    }

    public SetBoxState(state: number): void
    {
        this.m_nBoxState = state;
    }

    //获取宝箱的领取状态
    public GetBoxState(): number
    {
        return this.m_nBoxState;
    }

    public IsCanReceive(): boolean
    {
        return this.m_nBoxState === 2;
    }
}