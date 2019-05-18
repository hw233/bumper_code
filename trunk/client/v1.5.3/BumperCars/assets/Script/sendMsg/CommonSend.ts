import GameLog from '../Log/LogTool';
import GlobalVar from '../GlobalVar';
import MessageUtil from '../network/socket/MessageUtil';
import SocketNetManager from '../network/socket/SocketNetManager';
import { Msg_Action_Type } from '../network/socket/MsgID';
import { Msg_Common_Type, Msg_Walk_Type, MsgID } from '../network/socket/MsgID';


export default class CommonSend
{
    //发送玩家的操作的消息
    public SendPlayerOp(args: Array<any>, ts: number): void
    {
        let data = 
        {
            target_id: GlobalVar.userID,

            action: Msg_Walk_Type._WALK_ACTION_USER_CMD,

            model: GlobalVar.gameModel,

            ts: ts,
            
            point_x: 0,//MessageUtil.ConvertToInt(curLocation.x),
            point_y: 0,//MessageUtil.ConvertToInt(curLocation.y),

            point_start_x: MessageUtil.ConvertToInt(args[0]),
            point_start_y: MessageUtil.ConvertToInt(args[1]),

            point_end_x: 0,//MessageUtil.ConvertToInt(opEndPoint.x),
            point_end_y: 0,//MessageUtil.ConvertToInt(opEndPoint.y),
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_WALK, data, false);
    }

    //碰撞的消息
    public SendCollideMessage(args, ts: number)
    {
        let data = 
        {
            ts: ts,

            model: GlobalVar.gameModel,

            action: Msg_Walk_Type._WALK_ACTION_USER_COLLIDE_CMD,

            type: args[0],
            power_x: MessageUtil.ConvertToInt(args[1]),
            power_y: MessageUtil.ConvertToInt(args[2]),

            becollide_id: args[3],

            t_type: args[4],
            t_power_x: MessageUtil.ConvertToInt(args[5]),
            t_power_y: MessageUtil.ConvertToInt(args[6]),

        };
        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_WALK, data, false);
    }

    //废弃 走登陆了
    //获取用户数据
    public SendGetUserData(): void
    {
        let data = {
            user_id: GlobalVar.userID,
            action: Msg_Common_Type._BUMPER_TRANSFER_GET_USER_INFO,
        };
        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_BUMPER_TRANSFER, data, true);
    }

    //上传游戏结束数据

    //解锁车辆(废弃 走商城购买道具包)
    public SendUnlockCarData(carID: number): void
    {
    //     let data = {
    //         user_id: GlobalVar.userID,
    //         car_id: carID,
    //         action: Msg_Common_Type._BUMPER_TRANSFER_UNLOCK_CAR,
    //     };

    //     SocketNetManager.GetInstance().SendMsg(MsgID.MSG_BUMPER_TRANSFER, data, true);
    }

    //被分享者进入游戏通知服务器
    public SendUserShare(shareUID: number): void
    {
        let data = 
        {
            user_id: GlobalVar.userID,
            parent_id: shareUID,
            action: Msg_Common_Type._BUMPER_TRANSFER_INVITE_USER,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_BUMPER_TRANSFER, data, true);
    }


    //发送使用车辆的消息
    public SendInUseCarID(carID: number): void
    {
        let data = {
            user_id: GlobalVar.userID,
            car_id: carID,
            action: Msg_Common_Type._BUMPER_TRANSFER_USE_CAR,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_BUMPER_TRANSFER, data, true);
    }

    //发送使用拖尾的消息
    public SendUseMotionMessage(motionID: number)
    {
        let data = {
            tail_id: motionID,
            action: Msg_Common_Type._BUMPER_TRANSFER_TAIL,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_BUMPER_TRANSFER, data, true);
    }

    //根据名次获取当前段位和等级
    public SendGetSettleInfoMessage(rank: number): void
    {
        let data = {
            user_id: GlobalVar.userID,
            rank: rank,
            action: Msg_Common_Type._BUMPER_TRANSFER_RANK_GET_GRANDING_STARNUM,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_BUMPER_TRANSFER, data, true);
    }

    //金币和击杀数据上传
    public SendGoldMessage(goldNum: number, killNum: number): void
    {
        let data = {
            user_id: GlobalVar.userID,
            gold_num: goldNum,
            kill_num: killNum,
            action: Msg_Common_Type._BUMPER_TRANSFER_UPLOAD_GOLDNUM_KILLNUM,
        };
        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_BUMPER_TRANSFER, data, true);
    }

    //保留星星
    public SendKeepStarMessage(): void
    {
        let data = {
            user_id: GlobalVar.userID,
            action: Msg_Common_Type._BUMPER_TRANSFER_GET_NOT_DECREASE_STAR_GIFT,
        };
        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_BUMPER_TRANSFER, data, true);
    }

    //领取加油礼包
    public SendReceiveComeOnGift(): void
    {
        let data = {
            user_id: GlobalVar.userID,
            action: Msg_Common_Type._BUMPER_TRANSFER_GET_BIG_GIFT,
        };
        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_BUMPER_TRANSFER, data, true);
    }

    //排行榜
    public SendGetRankListMessage(type: number): void
    {
        let data = 
        {
            action: 0,
            user_id: GlobalVar.userID,
        };

        if(type == 1)
        {
            data.action = Msg_Common_Type._BUMPER_TRANSFER_RANKING_LIST_BY_KILLNUM;
        }else if(type == 2)
        {
            data.action = Msg_Common_Type._BUMPER_TRANSFER_RANKING_LIST_BY_RANK;
        }
        
        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_BUMPER_TRANSFER, data, true);
    }
}
