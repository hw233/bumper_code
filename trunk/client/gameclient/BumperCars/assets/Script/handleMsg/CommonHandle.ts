import AccountData from '../manager/AccountData';
import EventCenter from '../common/EventCenter';
import GameData from '../data/GameData';
import GameLog from '../Log/LogTool';
import HandleBase from './HandleBase';
import MessageUtil from '../network/socket/MessageUtil';
import PlayerMgr from '../manager/PlayerMgr';
import { MessageType } from '../common/EventCenter';
import { Msg_Action_Type, Msg_Common_Type } from '../network/socket/MsgID';
import { Msg_Syn_Room_Type } from '../network/socket/MsgID';
import { Msg_Walk_Type, MsgID } from '../network/socket/MsgID';
import ItemSystem from '../Item/ItemSystem';


//处理零碎的消息
export default class CommonHandle extends HandleBase
{
    constructor()
    {
        super();
    }

    public RegisterAllHandle()
    {
        //同步房间信息的消息-添加
        this.RegisterHandle(MsgID.MSG_SYN_ROOM, Msg_Syn_Room_Type.SYNROOMINFO_ACTION_ADD_MEMBER, this.OnSynRoomInfoAdd.bind(this));
        
        //同步房间信息的消息-删除
        this.RegisterHandle(MsgID.MSG_SYN_ROOM, Msg_Syn_Room_Type.SYNROOMINFO_ACTION_DEL_MEMBER, this.OnSynRoomInfoDelete.bind(this));
        
        //解锁车辆成功
        this.RegisterHandle(MsgID.MSG_BUMPER_TRANSFER, Msg_Common_Type._BUMPER_TRANSFER_UNLOCK_CAR_SUC, this.OnUnlockCarSuccess.bind(this));
        //解锁车辆失败
        this.RegisterHandle(MsgID.MSG_BUMPER_TRANSFER, Msg_Common_Type._BUMPER_TRANSFER_UNLOCK_CAR_FAIL, this.OnUnlockCarFail.bind(this));

        //邀请用户成功
        this.RegisterHandle(MsgID.MSG_BUMPER_TRANSFER, Msg_Common_Type._BUMPER_TRANSFER_INVITE_USER_SUC, this.OnInviteUserSuccess.bind(this));
        //邀请用户失败
        this.RegisterHandle(MsgID.MSG_BUMPER_TRANSFER, Msg_Common_Type._BUMPER_TRANSFER_INVITE_USER_FAIL, this.OnInviteUserFail.bind(this));

        //使用车辆成功
        this.RegisterHandle(MsgID.MSG_BUMPER_TRANSFER, Msg_Common_Type._BUMPER_TRANSFER_USE_CAR_SUC, this.OnUseCarSuccess.bind(this));
        //使用车辆失败
        this.RegisterHandle(MsgID.MSG_BUMPER_TRANSFER, Msg_Common_Type._BUMPER_TRANSFER_USE_CAR_FAIL, this.OnUseCarFail.bind(this));
        
        //使用拖尾成功
        this.RegisterHandle(MsgID.MSG_BUMPER_TRANSFER, Msg_Common_Type._BUMPER_TRANSFER_TAIL_SUC, this.OnUseMotionSuccess.bind(this));
        //使用拖尾失败
        this.RegisterHandle(MsgID.MSG_BUMPER_TRANSFER, Msg_Common_Type._BUMPER_TRANSFER_TAIL_FAIL, this.OnUseMotionFail.bind(this));


        //根据名次获取当前段位和等级成功
        this.RegisterHandle(MsgID.MSG_BUMPER_TRANSFER, Msg_Common_Type._BUMPER_TRANSFER_RANK_GET_GRANDING_STARNUM_SUC, this.OnGetGradingSuccess.bind(this));
        //根据名次获取当前段位和等级失败
        this.RegisterHandle(MsgID.MSG_BUMPER_TRANSFER, Msg_Common_Type._BUMPER_TRANSFER_RANK_GET_GRANDING_STARNUM_FAIL, this.OnGetGradingFail.bind(this));
        
        //金币和击杀数据上传成功
        this.RegisterHandle(MsgID.MSG_BUMPER_TRANSFER, Msg_Common_Type._BUMPER_TRANSFER_UPLOAD_GOLDNUM_KILLNUM_SUC, this.OnUploadGoldSuccess.bind(this));
        //金币和击杀数据上传失败
        this.RegisterHandle(MsgID.MSG_BUMPER_TRANSFER, Msg_Common_Type._BUMPER_TRANSFER_UPLOAD_GOLDNUM_KILLNUM_FAIL, this.OnUploadGoldFail.bind(this));
        
        //领取不减星礼包成功
        this.RegisterHandle(MsgID.MSG_BUMPER_TRANSFER, Msg_Common_Type._BUMPER_TRANSFER_GET_NOT_DECREASE_STAR_GIFT_SUC, this.OnKeepStarSuccess.bind(this));
        //领取不减星礼包失败
        this.RegisterHandle(MsgID.MSG_BUMPER_TRANSFER, Msg_Common_Type._BUMPER_TRANSFER_GET_NOT_DECREASE_STAR_GIFT_FAIL, this.OnKeepStarFail.bind(this));
        
        //领取大礼包成功
        this.RegisterHandle(MsgID.MSG_BUMPER_TRANSFER, Msg_Common_Type._BUMPER_TRANSFER_GET_BIG_GIFT_SUC, this.OnComeOnGiftSuccess.bind(this));
        //领取大礼包失败
        this.RegisterHandle(MsgID.MSG_BUMPER_TRANSFER, Msg_Common_Type._BUMPER_TRANSFER_GET_BIG_GIFT_FAIL, this.OnComeOnGiftFail.bind(this));

        //获取击杀排行榜数据成功
        this.RegisterHandle(MsgID.MSG_BUMPER_TRANSFER, Msg_Common_Type._BUMPER_TRANSFER_RANKING_LIST_BY_KILLNUM_SUC, this.OnGetKillRanklistSuccess.bind(this));
        //获取击杀排行榜数据失败
        this.RegisterHandle(MsgID.MSG_BUMPER_TRANSFER, Msg_Common_Type._BUMPER_TRANSFER_RANKING_LIST_BY_KILLNUM_FAIL, this.OnGetKillRanklistFail.bind(this));

         //获取段位排行榜数据成功
         this.RegisterHandle(MsgID.MSG_BUMPER_TRANSFER, Msg_Common_Type._BUMPER_TRANSFER_RANKING_LIST_BY_RANK_SUC, this.OnGetGradingRanklistSuccess.bind(this));
         //获取段位排行榜数据失败
         this.RegisterHandle(MsgID.MSG_BUMPER_TRANSFER, Msg_Common_Type._BUMPER_TRANSFER_RANKING_LIST_BY_RANK_FAIL, this.OnGetGradingRanklistFail.bind(this));
    }

    //同步房间信息的消息-添加
    private OnSynRoomInfoAdd(msg)
    {

        if(!PlayerMgr.IsExistPlayer(msg.uid)) 
        {
            if(PlayerMgr.IsSelf(msg.uid))
            {
                GameData.SetRoomID(msg.team_id);
            }

            PlayerMgr.AddPlayerByInfo(msg);
            
            EventCenter.DispatchEvent(MessageType.DLD_Join_Player, msg.uid);
        }

    }

    //同步房间信息的消息-删除
    private OnSynRoomInfoDelete(msg)
    {
        let playerID: number = parseInt( msg.uid );
        
        if(PlayerMgr.IsExistPlayer(playerID))
        {
            EventCenter.DispatchEvent(MessageType.DLD_Leave_Player, playerID);

            //最后删除 因为移除UI的时候要用
            PlayerMgr.RemovePlayer(playerID);

            EventCenter.DispatchEvent(MessageType.DLD_Player_Exit_Game, playerID);
        }
    }

    //获取用户信息成功
    private OnGetUserInfoSuccess(): void
    {

    }

    //获取用户信息失败
    private OnGetUserInfoFail(): void
    {

    }

    //解锁车辆成功(废弃 使用商城购买道具包)
    private OnUnlockCarSuccess(msg): void
    {
        // let carID: number = parseInt(msg.data.car_id);

        // if(msg.data.status === 1)
        // {
        //     //解锁成功
        //     GameLog.PrintLog("车辆解锁成功");

        //     let goldNum: number = parseInt(msg.data.gold_num);

        //     // AccountData.GetInstance().SetGoldNum(goldNum);
        //     // AccountData.GetInstance().AddUnlockCarID(carID);

        //     EventCenter.DispatchEvent(MessageType.CarLib_Unlock_Car_Suc, carID);
        // }
        // else if(msg.data.status === 2)
        // {
        //     GameLog.PrintLog("车辆已解锁");
        // }
        // else if(msg.data.status === -1)
        // {
        //     GameLog.PrintLog("解锁车辆失败：", msg);

        //     //解锁失败
        //     EventCenter.DispatchEvent(MessageType.CarLib_Unlock_Car_Fail, carID);
        // }
    }

    //解锁车辆失败(废弃 使用商城购买道具包)
    private OnUnlockCarFail(msg): void
    {
        // GameLog.PrintLog("解锁车辆失败：", msg);
    }

    //被分享者进入游戏通知服务器成功
    private OnInviteUserSuccess(): void
    {
        GameLog.PrintLog("被分享者进入游戏通知服务器成功");
    }

    //被分享者进入游戏通知服务器失败
    private OnInviteUserFail(msg): void
    {
        GameLog.PrintLog("被分享者进入游戏通知服务器失败", msg);
    }

    //使用车辆成功
    private OnUseCarSuccess(msg): void
    {
        GameLog.PrintLog("使用车辆成功", msg);

        let curCarItemID: number = ItemSystem.GetInstance().GetItemIDBySoleID(msg.use_car);

        AccountData.GetInstance().SetUseCarID(curCarItemID);

        EventCenter.DispatchEvent(MessageType.Use_Car_Suc);
        EventCenter.DispatchEvent(MessageType.StartScene_RefreshCarList);
    }

    //使用车辆失败
    private OnUseCarFail(msg): void
    {
        GameLog.PrintLog("使用车辆失败", msg);
    }
    
    //使用拖尾成功
    private OnUseMotionSuccess(msg): void
    {
        GameLog.PrintLog("使用拖尾成功", msg);

        let curMotionItemID: number = ItemSystem.GetInstance().GetItemIDBySoleID(msg.tail_id);

        AccountData.GetInstance().SetUseMotionID(curMotionItemID);

        EventCenter.DispatchEvent(MessageType.Use_Motion_Suc);
    }

    //使用拖尾失败
    private OnUseMotionFail(msg): void
    {
        GameLog.PrintLog("使用拖尾失败", msg);
    }

    //获得段位信息成功
    private OnGetGradingSuccess(msg): void
    {
        GameLog.PrintLog("获得段位信息成功", msg);

        let gradingID: number = parseInt(msg.data.granding_id);
        let starNum: number = parseInt(msg.data.star_num);

        AccountData.GetInstance().SetCurLevelID(gradingID);
        AccountData.GetInstance().SetCurStarNum(starNum);

        EventCenter.DispatchEvent(MessageType.OverLayer_Get_Grading_Suc, msg.data.status);
    }

    //获得段位信息失败
    private OnGetGradingFail(msg): void
    {
        GameLog.PrintLog("获得段位信息失败", msg);

        EventCenter.DispatchEvent(MessageType.OverLayer_Get_Grading_Fail, msg.data.status);
    }

    //上传金币成功
    private OnUploadGoldSuccess(): void
    {
        GameLog.PrintLog("上传金币成功");

    }

    //上传金币失败
    private OnUploadGoldFail(msg): void
    {
        GameLog.PrintLog("上传金币失败", msg);
    }

    //保星成功
    private OnKeepStarSuccess(msg): void
    {
        GameLog.PrintLog("保星成功", msg);

        let gradingID: number = parseInt(msg.data.granding_id);
        let starNum: number = parseInt(msg.data.star_num);
        let keepStarCount: number = parseInt(msg.data.keep_star);

        AccountData.GetInstance().SetCurLevelID(gradingID);
        AccountData.GetInstance().SetCurStarNum(starNum);
        AccountData.GetInstance().SetKeepStarCount(keepStarCount);

        EventCenter.DispatchEvent(MessageType.OverLayer_Keep_Star_Suc);
    }

    //保星失败
    private OnKeepStarFail(msg): void
    {
        GameLog.PrintLog("保星失败", msg);

        EventCenter.DispatchEvent(MessageType.OverLayer_Keep_Star_Fail);
    }

    //加油礼包领取成功
    private OnComeOnGiftSuccess(msg): void
    {
        GameLog.PrintLog("加油礼包领取成功", msg);

        let gradingID: number = parseInt(msg.data.granding_id);
        let starNum: number = parseInt(msg.data.star_num);
        let giftCount: number = parseInt(msg.data.get_gift);
        let gradingNum: number = parseInt(msg.data.granding_num);

        // AccountData.GetInstance().SetGoldNum(goldNum);
        AccountData.GetInstance().SetCurLevelID(gradingID);
        AccountData.GetInstance().SetCurStarNum(starNum);
        AccountData.GetInstance().SetCurLevelNum(gradingNum);
        AccountData.GetInstance().SetReceiveGiftCount(giftCount);

        EventCenter.DispatchEvent(MessageType.ComeOnView_Receive_Suc);
    }

    //加油礼包领取失败
    private OnComeOnGiftFail(msg): void
    {
        EventCenter.DispatchEvent(MessageType.ComeOnView_Receive_Fail);
    }

    //击杀排行榜数据获取成功
    private OnGetKillRanklistSuccess(msg): void
    {
        EventCenter.DispatchEvent(MessageType.StartScene_GetRankList_Suc, msg.data);
    }

    //击杀排行榜数据获取失败
    private OnGetKillRanklistFail(msg): void
    {
        EventCenter.DispatchEvent(MessageType.StartScene_GetRankList_Fail);
    }

    //击杀排行榜数据获取成功
    private OnGetGradingRanklistSuccess(msg): void
    {
        EventCenter.DispatchEvent(MessageType.StartScene_GetRankList_Suc, msg.data);
    }

    //击杀排行榜数据获取失败
    private OnGetGradingRanklistFail(msg): void
    {
        EventCenter.DispatchEvent(MessageType.StartScene_GetRankList_Fail);
    }
}
