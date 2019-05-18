import { MsgID, Msg_Friend_Info_Type, Msg_Friend_Type } from '../network/socket/MsgID';
import EventCenter from "../common/EventCenter";
import {MessageType} from "../common/EventCenter";
import FriendManager from '../Friend/FriendManager';
import FriendData from '../Friend/FriendData';
import FriendApply from '../Friend/FriendApply';
import HandleBase from './HandleBase';

export default class FriendHandle extends HandleBase
{
    constructor()
    {
        super();
    }

    public RegisterAllHandle()
    {
        //获取好友列表的消息
        this.RegisterHandle(MsgID.MSG_FRIEND, Msg_Friend_Type._FRIEND_GETLIST, this.OnGetFriendListMessage.bind(this));
        //好友申请的消息
        this.RegisterHandle(MsgID.MSG_FRIEND, Msg_Friend_Type._FRIEND_APPLY, this.OnFriendApplyMessage.bind(this));

        //添加好友的消息
        this.RegisterHandle(MsgID.MSG_FRIEND_INFO, Msg_Friend_Info_Type._FRIEND_INFO_ADD, this.OnAddFriendMessage.bind(this));
        //好友上线的消息
        this.RegisterHandle(MsgID.MSG_FRIEND_INFO, Msg_Friend_Info_Type._FRIEND_INFO_ONLINE, this.OnFriendOnline.bind(this));
        //好友离线的消息
        this.RegisterHandle(MsgID.MSG_FRIEND_INFO, Msg_Friend_Info_Type._FRIEND_INFO_OFFLINE, this.OnLeaveline.bind(this));
        //更新好友数据
        this.RegisterHandle(MsgID.MSG_FRIEND_INFO, Msg_Friend_Info_Type._FRIEND_INFO_UPDATE, this.OnUpdateFriendData.bind(this));
        //获取好友列表的消息
        this.RegisterHandle(MsgID.MSG_FRIEND_INFO, Msg_Friend_Info_Type._FRIEND_INFO_GETLIST, this.OnGetFriendListMessage.bind(this));
    }

    //获取好友列表的消息
    private OnGetFriendListMessage(msg)
    {
        FriendManager.GetInstance().AddFriendsToList(msg);

        EventCenter.DispatchEvent(MessageType.Get_Friend_List_Event);
    }

    //好友申请的消息
    private OnFriendApplyMessage(msg)
    {
        let friendApply: FriendApply = new FriendApply();
        friendApply.setUserID( parseInt(msg.invite_id) || 0 );
        friendApply.setAvatarUrl(msg.head_picurl);
        friendApply.setNickName(msg.nick_name);

        EventCenter.DispatchEvent(MessageType.Friend_Apply_Event, friendApply);
    }

    //添加好友的消息
    private OnAddFriendMessage(msg)
    {
        let friendID: number = parseInt(msg.friend_id);

        if(FriendManager.GetInstance().IsMyFriend(friendID))
        {
            return;
        }
        
        let friendData: FriendData = FriendData.Deserialization(msg);
        if(friendData)
        {
            FriendManager.GetInstance().AddFriendToMe(friendData);

            EventCenter.DispatchEvent(MessageType.Add_Friend_Event, friendID);
        }
    }

    //更新好友数据
    private OnUpdateFriendData(msg)
    {
        let friendID: number = parseInt( msg.friend_id );

        let friendData: FriendData = FriendManager.GetInstance().GetMyFriend(friendID);
        if(friendData)
        {
            friendData.setInGame(msg.online === 2);

            EventCenter.DispatchEvent( MessageType.Refresh_Friend_Info_Event, friendID );
        }
    }

    // //好友进入游戏
    // private OnInGameMessage(msg)
    // {
    //     let friendID: number = parseInt( msg.friend_id );

    //     let friendData: FriendData = FriendManager.GetInstance().GetMyFriend(friendID);
    //     if(friendData)
    //     {
    //         // friendData.setInGame();

    //         EventCenter.DispatchEvent( MessageType.Refresh_Friend_Info_Event, friendID );
    //     }
    // }

    //好友上线的消息
    private OnFriendOnline(msg)
    {
        let friendID: number = parseInt( msg.friend_id );

        let friendData: FriendData = FriendManager.GetInstance().GetMyFriend(friendID);
        if(friendData)
        {
            friendData.setOnLineState(msg.online);

            EventCenter.DispatchEvent( MessageType.Refresh_Friend_Info_Event, friendID );
        }
    }

    //好友离线的消息
    private OnLeaveline(msg)
    {
        let friendID: number = parseInt( msg.friend_id );

        let friendData: FriendData = FriendManager.GetInstance().GetMyFriend(friendID);
        if(friendData)
        {
            friendData.setOnLineState(msg.online);
            friendData.setInGame(false);

            EventCenter.DispatchEvent( MessageType.Refresh_Friend_Info_Event, friendID );
        }
    }
}
