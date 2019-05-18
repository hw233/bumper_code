import { MsgID, Msg_SignIn_Type } from '../network/socket/MsgID';
import EventCenter, { MessageType } from '../common/EventCenter';
import HandleBase from './HandleBase';

export default class SignInHandle extends HandleBase
{
    public constructor()
    {
        super();
    }

    public RegisterAllHandle()
    {
        //领奖成功
        this.RegisterHandle(MsgID.MSG_SIGNIN, Msg_SignIn_Type._SIGNIN_ACTION_SIGNIN_SUC, this.OnSignInSucMessage.bind(this));
        //领奖失败
        this.RegisterHandle(MsgID.MSG_SIGNIN, Msg_SignIn_Type._SIGNIN_ACTION_SIGNIN_FAIL, this.OnSignInFailMessage.bind(this));
    }

    //签到成功
    private OnSignInSucMessage(msg)
    {
        EventCenter.DispatchEvent(MessageType.SignIn_Receive_Suc_Event,{
            dayID: msg.dayindex,
            type: msg.type,
        });
    }

    //签到失败
    private OnSignInFailMessage(msg)
    {
        EventCenter.DispatchEvent(MessageType.SignIn_Receive_Fail_Event);
    }
}