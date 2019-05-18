import AccountData from '../manager/AccountData';
import BoxData from './BoxData';
import EventCenter from '../common/EventCenter';
import GameLog from '../Log/LogTool';
import GameUI from '../manager/UIManager';
import SignInManager from './SignInManager';
import Util from '../common/Util';
import { MessageType } from '../common/EventCenter';
import { RewardData } from '../data/RewardData';
import { SignInDayData } from './SignInDayData';
import { UIType } from '../UI/UIDefine';
import GameTable from '../manager/TableManger';
import { TableType } from '../table/TableDefine';
import SignInTableInfo from '../table/SignInTableInfo';
import BoxTableInfo from '../table/BoxTableInfo';

export enum SignInState
{
    SIS_None                = 0,
    SIS_CannotSignIn        = 1,    //不可签到
    SIS_CanSignIn           = 2,    //可签到
    SIS_NotSignIn           = 3,    //未签到
    SIS_AlreadySignIn       = 4,    //已签到
}

export enum BoxState
{
    BS_None                 = 0,
    BS_NotReceive           = 1,     //未领取
    BS_CanReceive           = 2,    //可领取
    BS_AlreadyReceive       = 3,    //已领取
}

//签到系统
export default class SignInSystem
{
    private static instance: SignInSystem              = null;

    private static EventTag: string                    = "SignInSystem";

    //累计签到次数
    private m_SignInCount: number = null;

    private m_bInited: boolean      = null;

    constructor()
    {
        this.m_SignInCount = 0;
        this.m_bInited = false;
    }

    public Init()
    {
        if(this.m_bInited)
        {
            return;
        }

        this.m_bInited = true;

        this.InitSignInData();
        this.InitBoxData();

        EventCenter.RegisterEvent(SignInSystem.EventTag, MessageType.SignIn_Receive_Suc_Event, this.signInSucResp.bind(this));
        EventCenter.RegisterEvent(SignInSystem.EventTag, MessageType.SignIn_Receive_Fail_Event, this.signInFailResp.bind(this));
        EventCenter.RegisterEvent(SignInSystem.EventTag, MessageType.SignIn_Retroactive_Suc_Event, this.retroactiveSucResp.bind(this));
        EventCenter.RegisterEvent(SignInSystem.EventTag, MessageType.SignIn_Retroactive_Fail_Event, this.retroactiveFailResp.bind(this));
        EventCenter.RegisterEvent(SignInSystem.EventTag, MessageType.SignIn_Receive_Box_Suc_Event, this.boxReceiveSucResp.bind(this))
    }

    //初始化签到数据
    private InitSignInData()
    {
        //当前是星期几
        let curDay: number = AccountData.GetInstance().GetDayIndex();

        //签到状态记录
        let signInRecord: number = AccountData.GetInstance().GetSignInData();
        //签到次数
        let signInCount: number = 0;
        
        for(let i = 1; i <= 7; i ++)
        {
             let signInTableInfo: SignInTableInfo = GameTable.GetInstance().GetDataInfo(TableType.TTD_SignInType, i);
             if(signInTableInfo)
             {
                let signData: SignInDayData = new SignInDayData();
                if(signData)
                {
                    let dayID: number = i === 7 ? 0 : i;
                    let state: number = SignInState.SIS_None;
                    //是否领取标记位 
                    let receiveFlag: number = i * 1;
                    //是否领取
                    let isReceive: boolean = Util.ValueAtBit(signInRecord, receiveFlag) === 1;

                    if(isReceive)
                    {
                        state = SignInState.SIS_AlreadySignIn;
                    }
                    else
                    {
                        if(curDay > i)
                        {
                            state = SignInState.SIS_NotSignIn;
                        }
                        else if(curDay === i)
                        {
                            state = SignInState.SIS_CanSignIn;
                        }
                        else if(curDay < i)
                        {
                            state = SignInState.SIS_CannotSignIn;
                        }
                    }

                    if(state === 4)
                    {
                        signInCount ++;
                    }

                    signData.SetState(state);
                    signData.SetDayID(dayID);
                    signData.SetDayNumber(i);
                    signData.SetRewardItemID(signInTableInfo.GetTaskRewardType());
                    signData.SetRewardNum(signInTableInfo.GetTaskRewardNum());

                    SignInManager.GetInstance().AddSignInData(signData);
                }
             }
        }

        this.m_SignInCount = signInCount;
    }

    //初始化宝箱数据
    private InitBoxData()
    {
        let initFlag: number = 8;
        let signInRecord: number = AccountData.GetInstance().GetSignInData();

        for(let i = 0, len = 4; i < len; i ++)
        {
             let boxTableInfo: BoxTableInfo = GameTable.GetInstance().GetDataInfoByIndex(TableType.TTD_BoxType, i);
             if(boxTableInfo)
             {
                 let boxData: BoxData = new BoxData();
                 if(boxData)
                 {
                    let rewardTypeList = boxTableInfo.GetTaskRewardType();
                    let rewardNumList = boxTableInfo.GetTaskRewardNum();

                    for(let j = 0, rewardListLen = rewardTypeList.length; j < rewardListLen; j ++)
                    {
                        let reward: RewardData = new RewardData();
                        if(reward)
                        {
                            reward.ItemID = Number(rewardTypeList[j]);
                            reward.ItemNum = Number(rewardNumList[j]);
                        }
                        boxData.AddReward(reward);
                    }

                    //宝箱状态
                    let state: number = BoxState.BS_None;
                    //是否领取标志位
                    let receiveFlag: number = initFlag + i;
                    //是否领取
                    let isReceive: boolean = Util.ValueAtBit(signInRecord, receiveFlag) === 1;

                    if(isReceive)
                    {
                        state = BoxState.BS_AlreadyReceive;
                    }
                    else
                    {
                        //领取需要的签到次数
                        let needDay: number = boxTableInfo.GetSignInDays();

                        //是否可以领取
                        let isCanReceive: boolean = this.m_SignInCount >= needDay;

                        if(isCanReceive)
                        {
                            state = BoxState.BS_CanReceive;
                        }
                        else
                        {
                            state = BoxState.BS_NotReceive;
                        }
                    }

                    boxData.SetBoxID(boxTableInfo.GetID());
                    boxData.SetBoxState(state);

                    SignInManager.GetInstance().AddBoxData(boxData);
                 }
             }
        }
    }

    private refreshBoxData()
    {
        let initFlag: number = 8;
        let signInRecord: number = AccountData.GetInstance().GetSignInData();

        let boxCount: number = SignInManager.GetInstance().GetBoxNum();
        for(let i = 0; i < boxCount; i ++)
        {
            let boxData: BoxData = SignInManager.GetInstance().GetBoxDataByIndex(i);
            if(boxData)
            {
                let state: number = BoxState.BS_None;

                let boxTableInfo: BoxTableInfo = GameTable.GetInstance().GetDataInfo(TableType.TTD_BoxType, boxData.GetBoxID());

                //领取需要的签到次数
                let needDay: number = boxTableInfo.GetSignInDays();
                //是否可以领取
                let isCanReceive: boolean = this.m_SignInCount >= needDay;
                if(isCanReceive)
                {
                    //是否领取标志位
                    let receiveFlag: number = initFlag + i;
                    //是否领取
                    let isReceive: boolean = Util.ValueAtBit(signInRecord, receiveFlag) === 1;
                    if(isReceive)
                    {
                        state = BoxState.BS_AlreadyReceive;
                    }
                    else
                    {
                        state = BoxState.BS_CanReceive;
                    }
                }
                else
                {
                    state = BoxState.BS_NotReceive;
                }

                boxData.SetBoxState(state);
            }
        }
    }

    //签到成功
    private signInSucResp(data: any)
    {
        this.m_SignInCount ++;

        this.refreshBoxData();
        EventCenter.DispatchEvent(MessageType.SignIn_Refresh_Box_Item);

        let signInData: SignInDayData = SignInManager.GetInstance().GetSignInDataByID(data.dayID);
        if(signInData)
        {
            //改变状态
            signInData.SetState(SignInState.SIS_AlreadySignIn);

            this.HintNewMessage();

            //刷新UI
            EventCenter.DispatchEvent(MessageType.SignIn_Refresh_SignIn_Item);
            EventCenter.DispatchEvent(MessageType.SignIn_Refresh_SignIn_View);
            EventCenter.DispatchEvent(MessageType.StartScene_RefreshTopView);

            //倍数
            let type: number = 1;
            if(data.type === 1)
            {
                type = 2;
            }

            GameUI.AddViewToQuene(UIType.UT_RewardView, {
                itemID: signInData.GetRewardItemID(),
                itemNum: signInData.GetRewardNum() * type,
            });

            GameUI.ShowViewByQuene();
        }
    }

    //签到失败
    private signInFailResp()
    {
        GameLog.PrintDebug("签到失败");

        GameUI.ShowHint("签到失败");
    }

    //补签成功
    private retroactiveSucResp(dayID: number)
    {
        this.m_SignInCount ++;

        let signInData:SignInDayData = SignInManager.GetInstance().GetSignInDataByID(dayID);
        if(signInData)
        {
            //改变状态
            signInData.SetState(SignInState.SIS_AlreadySignIn);

            this.HintNewMessage();

            //刷新UI
            EventCenter.DispatchEvent(MessageType.SignIn_Refresh_SignIn_Item);
            EventCenter.DispatchEvent(MessageType.StartScene_RefreshTopView);
        }
    }

    //补签失败
    private retroactiveFailResp(msg: any)
    {
        GameLog.PrintDebug("补签失败");
    }

    //宝箱领取成功
    private boxReceiveSucResp(boxID: number)
    {
        let boxData: BoxData = SignInManager.GetInstance().GetBoxDataByID(boxID);
        if(boxData)
        {
            boxData.SetBoxState(BoxState.BS_AlreadyReceive);

            GameUI.ShowUI(UIType.UT_SignInBoxRewardView, {
                boxID: boxID,
                rewardList: boxData.GetRewardList(),
            });
        }

        this.HintNewMessage();

        EventCenter.DispatchEvent(MessageType.SignIn_Refresh_Box_Item);
        EventCenter.DispatchEvent(MessageType.StartScene_RefreshTopView);
    }

    //新消息提示
    public HintNewMessage(): void
    {
        let curDayIndex: number = AccountData.GetInstance().GetDayIndex();

        let curDayNumber: number = this.DayIndexConvertDayNumber(curDayIndex)

        let isHasNewMsg: boolean = false;
        for(let i = 1;i <= 7;i ++)
        {
            if(i <= curDayNumber)
            {
                let dayIndex: number = this.DayNumberToDayIndex(i);
                let signInData: SignInDayData = SignInManager.GetInstance().GetSignInDataByID(dayIndex);
                if(signInData)
                {
                    if(signInData.GetState() === SignInState.SIS_CanSignIn || signInData.GetState() === SignInState.SIS_NotSignIn)
                    {
                        isHasNewMsg = true;
                        break;
                    }
                }
            }
        }

        EventCenter.DispatchEvent(MessageType.NewMessage_SignIn, isHasNewMsg);
    }

    private DayIndexConvertDayNumber(dayindex: number): number
    {
        return dayindex === 7 ? 0 : dayindex;
    }

    private DayNumberToDayIndex(daynumber: number): number
    {
        return (daynumber === 7 ? 0 : daynumber);
    }

    public static GetInstance(): SignInSystem
    {
        if(!this.instance)
        {
            this.instance = new SignInSystem;
        }
        return this.instance;
    }

    public static Destroy()
    {
        if(this.instance)
        {
            this.instance = null;
        }
        EventCenter.RemoveEvent(SignInSystem.EventTag);
    }
}