import EventCenter from '../common/EventCenter';
import GameLog from '../Log/LogTool';
import HandleBase from './HandleBase';
import { MessageType } from '../common/EventCenter';
import { Msg_Action_Type } from '../network/socket/MsgID';
import { MsgID } from '../network/socket/MsgID';
import AccountData from '../manager/AccountData';

//处理零碎的消息
export default class ActionHandle extends HandleBase
{
    constructor()
    {
        super();
    }

    public RegisterAllHandle()
    {
        //宝箱领取成功
        this.RegisterHandle(MsgID.MSG_ACTION, Msg_Action_Type._ACTION_SIGNIN_BOX_RECEIVE_SUC, this.OnSignInBoxReceiveSuccess.bind(this));
        //宝箱领取失败
        this.RegisterHandle(MsgID.MSG_ACTION, Msg_Action_Type._ACTION_SIGNIN_BOX_RECEIVE_FAIL, this.OnSignInBoxReceiveFail.bind(this));

        //返回主界面的消息 - 成功
        this.RegisterHandle(MsgID.MSG_ACTION, Msg_Action_Type._ACTION_GO_BACK_HOME_SCREEN_SUC, this.OnReturnMainView.bind(this));

        //限时领奖领取成功
        this.RegisterHandle(MsgID.MSG_ACTION, Msg_Action_Type._ACTION_LIMIT_TIME_AWARD_SUC, this.OnTimeLimitRewardSucMessage.bind(this));
        //限时奖励领取失败
        this.RegisterHandle(MsgID.MSG_ACTION, Msg_Action_Type._ACTION_LIMIT_TIME_AWARD_FAIL, this.OnTimeLimitRewardFailMessage.bind(this));
        //在线领奖成功
        this.RegisterHandle(MsgID.MSG_ACTION, Msg_Action_Type._ACTION_ONLINE_AWARD_SUC, this.OnOnlineRecvRewardSucMessage.bind(this));
        //在线领奖失败
        this.RegisterHandle(MsgID.MSG_ACTION, Msg_Action_Type._ACTION_ONLINE_AWARD_FAIL, this.OnOnlineRecvRewardFailMessage.bind(this));
        //在线加速成功
        this.RegisterHandle(MsgID.MSG_ACTION, Msg_Action_Type._ACTION_ONLINE_SPEED_UP_SUC, this.OnOnlineSpeedUpSucMessage.bind(this));
        //在线加速失败
        this.RegisterHandle(MsgID.MSG_ACTION, Msg_Action_Type._ACTION_ONLINE_SPEED_UP_FAIL, this.OnOnlineSpeedUpFailMessage.bind(this));
        //皮肤试用成功
        this.RegisterHandle(MsgID.MSG_ACTION, Msg_Action_Type._ACTIOIN_LIMIT_TIME_CONTINUE_TRY_SUC, this.OnSkinTryUseSucMessage.bind(this));
        //皮肤试用失败
        this.RegisterHandle(MsgID.MSG_ACTION, Msg_Action_Type._ACTIOIN_LIMIT_TIME_CONTINUE_TRY_FAIL, this.OnSkinTryUseFailMessage.bind(this));
        //添加到小游戏成功
        this.RegisterHandle(MsgID.MSG_ACTION, Msg_Action_Type._ACTION_ADD_APP_LIST_SUC, this.OnAddAppListSucMessage.bind(this));
        //添加到小游戏失败
        this.RegisterHandle(MsgID.MSG_ACTION, Msg_Action_Type._ACTION_ADD_APP_LIST_FAIL, this.OnAddAppListFailMessage.bind(this));
    }

    //返回主界面的消息
    private OnReturnMainView(msg)
    {

    }

    //宝箱领取成功
    private OnSignInBoxReceiveSuccess(msg): void
    {   
        EventCenter.DispatchEvent(MessageType.SignIn_Receive_Box_Suc_Event, msg.idx);
    }

    private OnSignInBoxReceiveFail(msg): void
    {
        //宝箱领取失败
        GameLog.PrintLog("宝箱领取失败", msg);
    }

    //限时领奖领取成功
    private OnTimeLimitRewardSucMessage(msg): void
    {
        let itemID: number = msg.show_item_type;
        let remainTime: number = msg.show_remain_ts;

        let liftTime: number = msg.life_time;

        AccountData.GetInstance().SetTimeLimitItemID(itemID);
        AccountData.GetInstance().SetTimeLimitRemainTime(remainTime);

        EventCenter.DispatchEvent(MessageType.TimeLimit_Refresh_Event, liftTime);
        EventCenter.DispatchEvent(MessageType.StartScene_RefreshCarList);
        EventCenter.DispatchEvent(MessageType.StartScene_RefreshTimeLimit);
    }

    //限时奖励领取失败
    private OnTimeLimitRewardFailMessage(msg)
    {
        GameLog.PrintLog("限时奖励领取失败", msg);
    }

    //在线领奖成功
    private OnOnlineRecvRewardSucMessage(msg)
    {
        EventCenter.DispatchEvent(MessageType.StartScene_RefreshTopView);
        EventCenter.DispatchEvent(MessageType.Online_Recv_Reward_Event, msg.online_idx);
    }

    //在线领奖失败
    private OnOnlineRecvRewardFailMessage(msg)
    {
        GameLog.PrintLog("在线领奖失败", msg);
    }

    //在线加速成功
    private OnOnlineSpeedUpSucMessage(msg)
    {
        EventCenter.DispatchEvent(MessageType.Online_Speed_Up_Event);
    }

    //在线加速失败
    private OnOnlineSpeedUpFailMessage(msg)
    {
        GameLog.PrintLog("在线加速失败", msg);
    }

    //皮肤试用成功
    private OnSkinTryUseSucMessage(msg)
    {
        // let itemData: ItemData = ItemManager.GetInstance().GetItemByID();
        EventCenter.DispatchEvent(MessageType.Item_Try_Suc_Event, {
            itemID: msg.item_id,
            liftTime: null,
        });
    }

    //皮肤试用失败
    private OnSkinTryUseFailMessage(msg)
    {
        GameLog.PrintLog("皮肤试用失败", msg);
    }

    //添加到小游戏成功
    private OnAddAppListSucMessage()
    {
        // EventCenter.DispatchEvent(MessageType.Item_Try_Suc_Event);
    }

    //添加到小游戏失败
    private OnAddAppListFailMessage()
    {
        GameLog.PrintLog("添加到小游戏失败");
    }
}
