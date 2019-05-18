
import { MsgID, Msg_SignIn_Type } from '../network/socket/MsgID';
import SocketNetManager from "../network/socket/SocketNetManager";

export default class SignInSend
{
    //发送签到的消息
    public SendSignInMessage(type: number): void
    {
        let msg = 
        {
            action: Msg_SignIn_Type._SIGNIN_ACTION_SIGNIN,
            type: type,
            // dayindex: 3,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_SIGNIN, msg);
    }

    //发送补签的消息
    public SendRetroactiveMessage(id: number): void
    {
        let msg = 
        {
            action: Msg_SignIn_Type._SIGNIN_ACTION_SIGNIN,
            dayindex: id,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_SIGNIN, msg);
    }
}
