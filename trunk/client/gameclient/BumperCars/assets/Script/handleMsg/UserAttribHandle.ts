import HandleBase from './HandleBase';
import { MsgID, Msg_UserAttrib_Type } from '../network/socket/MsgID';
import AccountData from '../manager/AccountData';
import EventCenter from '../common/EventCenter';
import { MessageType } from '../common/EventCenter';

export default class UserAttribHandle extends HandleBase
{
    public constructor()
    {
        super();
    }

    public RegisterAllHandle()
    {
        this.RegisterHandle(MsgID.MSG_USERATTRIB, Msg_UserAttrib_Type._USERATTRI_HEAD_PICURL, this.OnHeadPicUrlChangeMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_USERATTRIB, Msg_UserAttrib_Type._USERATTRI_NICK_NAME, this.OnNickNameChangeMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_USERATTRIB, Msg_UserAttrib_Type._USERATTRI_GOLD_SUM, this.OnGoldSumChangeMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_USERATTRIB, Msg_UserAttrib_Type._USERATTRI_KILL_SUM, this.OnKillSumChangeMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_USERATTRIB, Msg_UserAttrib_Type._USERATTRI_GRANDING_ID, this.OnGrandingChangeMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_USERATTRIB, Msg_UserAttrib_Type._USERATTRI_CUR_STAR_SUM, this.OnCurStarChangeMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_USERATTRIB, Msg_UserAttrib_Type._USERATTRI_UNLOCK_CARS, this.OnUnlockCarChangeMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_USERATTRIB, Msg_UserAttrib_Type._USERATTRI_USE_CAR, this.OnUsingCarChangeMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_USERATTRIB, Msg_UserAttrib_Type._USERATTRI_KING_SUM, this.OnKingSumChangeMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_USERATTRIB, Msg_UserAttrib_Type._USERATTRI_GIFT_SUM, this.OnGiftCountChangeMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_USERATTRIB, Msg_UserAttrib_Type._USERATTRI_STAR_SUM, this.OnKeepStarChangeMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_USERATTRIB, Msg_UserAttrib_Type._USERATTRI_ADD_SCORE, this.OnScoreAddChangeMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_USERATTRIB, Msg_UserAttrib_Type._USERATTRI_ADD_GOLD, this.OnGoldAddChangeMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_USERATTRIB, Msg_UserAttrib_Type._USERATTRIB_DAYTASKMASK_FIR, this.OnMaskDataChangeMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_USERATTRIB, Msg_UserAttrib_Type._USERATTRIB_SIGNINMASK_FIR, this.OnDayIndexChangeMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_USERATTRIB, Msg_UserAttrib_Type._USERATTRIB_SHOW_ITEM_TYPE, this.OnTimeLimitFreeItemMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_USERATTRIB, Msg_UserAttrib_Type._USERATTRIB_SHOW_REMAIN_TIME, this.OnTimeLimitFreeTimeMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_USERATTRIB, Msg_UserAttrib_Type._USERATTRIB_ONLINE_REMAIN_TIME, this.OnOnlineRewardTimeMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_USERATTRIB, Msg_UserAttrib_Type._USERATTRIB_WEEKTASKMASK, this.OnWeekMaskDataChangeMessage.bind(this));
    }

    //头像改变
    private OnHeadPicUrlChangeMessage(msg)
    {
        AccountData.GetInstance().SetHeadImg(msg.attrib_data);
    }

    //昵称改变
    private OnNickNameChangeMessage(msg)
    {
        AccountData.GetInstance().SetNickName(msg.attrib_data);
    }

    //金币数量改变(废弃 通过道具属性包更新)
    private OnGoldSumChangeMessage(msg)
    {
        // let data: number = Number(msg.attrib_data);
        // AccountData.GetInstance().SetGoldNum(data);
    }

    //击杀数改变
    private OnKillSumChangeMessage(msg)
    {
        let data: number = Number(msg.attrib_data);
        AccountData.GetInstance().SetKillNum(data);
    }

    //段位改变
    private OnGrandingChangeMessage(msg)
    {
        let data: number = Number(msg.attrib_data);
        AccountData.GetInstance().SetCurLevelID(data);
    }

    //星星数量改变
    private OnCurStarChangeMessage(msg)
    {
        let data: number = Number(msg.attrib_data);
        AccountData.GetInstance().SetCurStarNum(data);
    }

    //已解锁车辆改变
    private OnUnlockCarChangeMessage(msg)
    {
        // AccountData.GetInstance().SetUnlockCarID(this.convertUnlockCarData(msg.attrib_data));
    }

    //使用中的车辆改变
    private OnUsingCarChangeMessage(msg)
    {
        let data: number = Number(msg.attrib_data);
        AccountData.GetInstance().SetUseCarID(data);
    }

    //霸主令牌
    private OnKingSumChangeMessage(msg)
    {
        let data: number = Number(msg.attrib_data);
        AccountData.GetInstance().SetBZLPNum(data);
    }

    //领取礼包的次数改变
    private OnGiftCountChangeMessage(msg)
    {
        let data: number = Number(msg.attrib_data);
        AccountData.GetInstance().SetReceiveGiftCount(data || 0);
    }

    //保留星星的次数
    private OnKeepStarChangeMessage(msg)
    {
        let data: number = Number(msg.attrib_data);
        AccountData.GetInstance().SetKeepStarCount(data || 0);
    }

    //积分加成改变
    private OnScoreAddChangeMessage(msg)
    {
        let data: number = Number(msg.attrib_data);
        AccountData.GetInstance().SetAddScore(data);
    }        
    
    //金币加成改变
    private OnGoldAddChangeMessage(msg)
    {
        let data: number = Number(msg.attrib_data);
        AccountData.GetInstance().SetAddGold(data);
    }     

    //每日任务掩码位
    private OnMaskDataChangeMessage(msg)
    {
        let data: number = Number(msg.attrib_data);
        AccountData.GetInstance().SetTaskData(data);
    }
    
    //签到掩码位
    private OnWeekMaskDataChangeMessage(msg)
    {
        let data: number = Number(msg.attrib_data);
        AccountData.GetInstance().SetSignInData(data);
    }

    //周几
    private OnDayIndexChangeMessage(msg)
    {
        let data: number = Number(msg.attrib_data);
        AccountData.GetInstance().SetDayIndex(data);
    }

    private OnTimeLimitFreeTimeMessage(msg)
    {
        let data: number = Number(msg.attrib_data);
        AccountData.GetInstance().SetTimeLimitRemainTime(data);

        EventCenter.DispatchEvent(MessageType.StartScene_RefreshTimeLimit);
    }

    private OnTimeLimitFreeItemMessage(msg)
    {
        let data: number = Number(msg.attrib_data);
        AccountData.GetInstance().SetTimeLimitItemID(data);

        EventCenter.DispatchEvent(MessageType.StartScene_RefreshTimeLimit);
    }

    private OnOnlineRewardTimeMessage(msg)
    {
        let data: number = Number(msg.attrib_data);
        AccountData.GetInstance().SetOnlineRewardTime(data);

        if(data === 0)
        {
            EventCenter.DispatchEvent(MessageType.Online_Can_Receive_Event);
        }
    }

    private convertUnlockCarData(unlock: string): Array<number>
    {
        let cars = unlock.split(",");
        let carArr: Array<number> = [];
        for(let i = 0, len = cars.length; i < len; i ++)
        {
            carArr.push(parseInt(cars[i]));
        }

        return carArr;
    }
}