import { SignInState } from './SignInSystem';
//某一天的签到数据
export class SignInDayData
{
    //哪一天
    private m_nDayNumber: number = null;

    //第几天id(0~6)
    private m_nDayID: number = null;

    //奖励的道具ID
    private m_nRewardItemID: number = null;

    //奖励的道具数量
    private m_nRewardNum: number = null;

    //状态(4已签到，3未签到，2可签到，1不可签到)
    private m_nState: number = null;

    constructor()
    {
        this.m_nDayNumber = 0;
        this.m_nDayID = 0;
        this.m_nRewardItemID = 0;
        this.m_nRewardNum = 0;
        this.m_nState = 0;
    }

    public SetDayID(dayID: number): void
    {
        this.m_nDayID = dayID;
    }

    public GetDayID(): number
    {
        return this.m_nDayID;
    }

    public SetDayNumber(day: number): void
    {
        this.m_nDayNumber = day;
    }

    public GetDayNumber(): number
    {
        return this.m_nDayNumber;
    }

    public SetRewardItemID(itemID: number): void
    {
        this.m_nRewardItemID = itemID;
    }
    
    public GetRewardItemID(): number
    {
        return this.m_nRewardItemID;
    }

    public SetRewardNum(num: number): void
    {
        this.m_nRewardNum = num;
    }

    public GetRewardNum(): number
    {
        return this.m_nRewardNum;
    }

    public SetState(state: number): void
    {
        this.m_nState = state;
    }

    public GetState(): number
    {
        return this.m_nState;
    }

    public IsCurDay(): boolean
    {
        return this.m_nState === SignInState.SIS_CanSignIn;
    }

    public IsCanRetroactive(): boolean
    {
        return this.m_nState === SignInState.SIS_NotSignIn;
    }
}