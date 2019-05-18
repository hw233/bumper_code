import SocketNetManager from '../network/socket/SocketNetManager';
import { Msg_Action_Type, MsgID } from '../network/socket/MsgID';


export default class ActionSend
{
    //发送返回主界面的消息
    public SendReturnMainView(): void
    {
        let data = {
            action: Msg_Action_Type._ACTION_GO_BACK_HOME_SCREEN,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_ACTION, data, true);
    }

    //发送领取签到宝箱
    public SendSignInBoxReceive(boxID: number)
    {
        let data = 
        {
            action: Msg_Action_Type._ACTION_SIGNIN_BOX_RECEIVE,
            idx: boxID,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_ACTION, data, true);
    }

    //发送限时领奖的消息
    public SendTimeLimitRewardMessage()
    {
        let data = 
        {
            action: Msg_Action_Type._ACTION_LIMIT_TIME_AWARD,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_ACTION, data, true);
    }

    //发送在线领奖的消息
    public SendOnlineRecvRewardMessage()
    {
        let data = 
        {
            action: Msg_Action_Type._ACTION_ONLINE_AWARD,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_ACTION, data, true);
    }

    //发送在线加速的消息
    public SendOnlineSpeedUpMessage()
    {
        let data = 
        {
            action: Msg_Action_Type._ACTION_ONLINE_SPEED_UP,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_ACTION, data, true);
    }

    //发送皮肤试用的消息
    public SendSkinTryUseMessage(carID: number, deleteFlag: boolean)
    {
        let data = 
        {
            item_id: carID,
            del_falg: deleteFlag ? 1 : 0,
            action: Msg_Action_Type._ACTIOIN_LIMIT_TIME_CONTINUE_TRY,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_ACTION, data, true);
    }

    //发送添加到小游戏的消息
    public SendAddToAppListMessage()
    {
        let data = 
        {
            action: Msg_Action_Type._ACTION_ADD_APP_LIST,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_ACTION, data, true);
    }
}
