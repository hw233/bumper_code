import {MsgID} from "../network/socket/MsgID";
import {Msg_Friend_Type} from "../network/socket/MsgID";
import SocketNetManager from "../network/socket/SocketNetManager";

//处理好友系统的消息
export default class FriendSend
{
    //获取好友列表
    public SendGetFriendList()
    {
        let data = 
        {
            action: Msg_Friend_Type._FRIEND_GETLIST,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_FRIEND, data);
    }

    //申请好友的消息
    public SendApplyFriendMessage(targetID: number)
    {
        let data = 
        {
            action: Msg_Friend_Type._FRIEND_APPLY,
            target_id: targetID,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_FRIEND, data, false);
    }

    //接受申请好友的消息
    public SendAgreeApplyMessage(inviteID: number)
    {
        let data = 
        {
            action: Msg_Friend_Type._FRIEND_ACCEPT,
            invite_id: inviteID,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_FRIEND, data, false);
    }
}
