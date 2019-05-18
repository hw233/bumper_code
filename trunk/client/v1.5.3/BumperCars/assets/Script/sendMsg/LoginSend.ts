
import {MsgID} from "../network/socket/MsgID";
import SocketNetManager from "../network/socket/SocketNetManager";

export default class LoginSend{
    //发送登陆的消息
    public  SendLoginMessage(userID: number, nickname: string, avatarUrl: string): void
    {
        let msg = 
        {
            uid: userID,
            nick_name: nickname,
            head_picurl: avatarUrl,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_LOGIN, msg);
    }
}
