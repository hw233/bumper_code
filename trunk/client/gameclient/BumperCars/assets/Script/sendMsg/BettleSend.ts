import {MsgID} from "../network/socket/MsgID";
import {Msg_Bettle_Type} from "../network/socket/MsgID";
import GlobalVar from '../GlobalVar';
import SocketNetManager from "../network/socket/SocketNetManager";
import MessageUtil from "../network/socket/MessageUtil"

export default class BettleSend{
    //创建房间消息
    SendCreateRoomMessage(){
        let data = {
            model: GlobalVar.gameModel,
            action: Msg_Bettle_Type._BATTLE_ACTION_CREATE_ROOM,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_BATTLE, data);
    }

    //快速匹配的消息
    SendQuickMatchMessage(){
        let data = {
            model: GlobalVar.gameModel,
            action: Msg_Bettle_Type._BATTLE_ACTION_QUICK_MATCH,
        };
        

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_BATTLE, data);
    }

    //快速加入的消息
    SendQuickJoinMessage(){
        let data = {
            model: GlobalVar.gameModel,
            action: Msg_Bettle_Type._BATTLE_ACTION_QUICK_JOIN,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_BATTLE, data);
    }

    //发送离开房间的消息
    SendLeaveRoomMessage(){
        
        let data = {
            model: GlobalVar.gameModel,
            action: Msg_Bettle_Type._BATTLE_ACTION_LEAVE_TEAM,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_BATTLE, data);
    }

    //取消快速加入的消息
    SendCancelQuickJoinMessage(){
    }

    //邀请好友加入房间的消息
    SendInviteJoinRoomMessage(roomID: number, model: number)
    {
        let data = {
            model: model,
            action: Msg_Bettle_Type._BATTLE_ACTION_INVITE_JOIN,
            team_id: roomID,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_BATTLE, data);
    }

    //邀请好友加入队伍
    SendInviteFriendJoinRoomMessage(friendID: number)
    {
        let data = {
            model: GlobalVar.gameModel,
            action: Msg_Bettle_Type._BATTLE_ACTION_INVITE_FRIEND_JOIN,
            friend_id: friendID,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_BATTLE, data, false);
    }

    //大乱斗进入游戏通知服务器
    SendDLDJoinGameMessage(){
        let data = {
            model: GlobalVar.gameModel,
            action: Msg_Bettle_Type._BATTLE_ACTION_LOADING_COMPLETE,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_BATTLE, data);
    }

    //夺旗战进入游戏通知服务器
    Send5V5JoinGameMessage(){
        let data = {
            model: GlobalVar.gameModel,
            action: Msg_Bettle_Type._BATTLE_ACTION_TEAM_LOADING_COMPLETE,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_BATTLE, data);
    }

    //死亡的消息
    SendKillMessage(killerID, decedentID){

        let data = 
        {
            model: GlobalVar.gameModel,
            
            action: Msg_Bettle_Type._BATTLE_ACTION_BATTLE_KILL,

            bekiller_id: decedentID,

            killer_id: killerID,

            point_x: 0,
            point_y: 0,

            ts: GlobalVar.LatestFrameTimeStamp,
        }
        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_BATTLE, data, false);
    }

    //复活的消息
    SendReliveMessage(type, posID){
        let data = 
        {
            model: GlobalVar.gameModel,
            action: Msg_Bettle_Type._BATTLE_ACTION_BATTLE_RELIVE,
            point_x: 0,
            point_y: 0,
            region_id: posID,
            type: type,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_BATTLE, data);
    }

    //夺旗战中复活的消息
    SendDQZReliveMessage(posID){
        let data = {
            model: GlobalVar.gameModel,
            action: Msg_Bettle_Type._BATTLE_ACTION_TEAM_RELIVE,
            region_id: posID,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_BATTLE, data, false);
    }

    //3倍奖励的消息-大乱斗
    SendThreeTimesRewardMessage(){
        let data = {
            model: GlobalVar.gameModel,
            action: Msg_Bettle_Type._BATTLE_ACTION_VIEW_AWARD,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_BATTLE, data);
    }

    //3倍奖励的消息-组队
    Send5v5ThreeTimesRewardMessage(){
        let data = {
            model: GlobalVar.gameModel,
            action: Msg_Bettle_Type._BATTLE_ACTION_TEAM_VIEW_AWARD,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_BATTLE, data);
    }
}
