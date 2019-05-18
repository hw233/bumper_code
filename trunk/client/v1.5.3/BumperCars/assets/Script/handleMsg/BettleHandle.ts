import DQZSettleData from '../data/net/DQZSettleData';
import EventCenter from '../common/EventCenter';
import GameData from '../data/GameData';
import GameLog from '../Log/LogTool';
import GlobalVar from '../GlobalVar';
import PlayerMgr from '../manager/PlayerMgr';
import { MessageType } from '../common/EventCenter';
import { Msg_Bettle_Type } from '../network/socket/MsgID';
import { MsgID } from '../network/socket/MsgID';
import { RoomState, TeamType } from '../GameDefine';
import DLDSettleData from '../data/net/DLDSettleData';
import PlayerData from '../data/PlayerData';
import HandleBase from './HandleBase';



export default class BettleHandle extends HandleBase
{
    constructor()
    {
        super();
    }

    //注册消息处理函数
    public RegisterAllHandle()
    {
        //创建房间成功消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_CREATE_ROOM_SUC, this.OnCreateRoomSuccessMessage.bind(this));
        //创建房间失败消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_CREATE_ROOM_FAIL, this.OnCreateRoomFailMessage.bind(this));

        //退出房间成功消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_LEAVE_TEAM_SUC, this.OnExitRoomSuccessMessage.bind(this));
        //退出房间失败消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_LEAVE_TEAM_FAIL, this.OnExitRoomFailMessage.bind(this));

        //快速匹配成功的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_QUICK_MATCH_SUC, this.OnQuickMatchSuccessMessage.bind(this));
        //快速匹配失败的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_QUICK_MATCH_FAIL, this.OnQuickMatchFailMessage.bind(this));

        //快速加入成功的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_QUICK_JOIN_SUC, this.OnQuickJoinSuccessMessage.bind(this));
        //快速加入失败的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_QUICK_JOIN_FAIL, this.OnQuickJoinFailMessage.bind(this));

        //邀请好友加入房间成功的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_INVITE_JOIN_SUC, this.OnInviteJoinRoomSuccessMessage.bind(this));
        //邀请好友加入房间失败的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_INVITE_JOIN_FAIL, this.OnInviteJoinRoomFailMessage.bind(this));

        //有玩家进入游戏的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_LOADING_COMPLETE_SUC, this.OnPlayerJoinGameMessage.bind(this));

        //队长变更的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_CHANGE_TEAM_LEADER_SUC, this.OnLeaderChangedMessage.bind(this));

        //战斗开始成功的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_BATTLE_START, this.OnBettleStartSuccess.bind(this));
        //战斗开始失败的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_BATTLE_START_FAIL, this.OnBettleStartFail.bind(this));

        // //碰撞成功的消息
        // this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_BATTLE_COLLIDE_SUC, this.OnCollideSuccessMessage.bind(this));
        // //碰撞失败的消息
        // this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_BATTLE_COLLIDE_FAIL, this.OnCollideFailMessage.bind(this));

        //战斗中死亡成功的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_BATTLE_KILL_SUC, this.OnDeathSuccessMessage.bind(this));
        //战斗中死亡失败的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_BATTLE_KILL_FAIL, this.OnDeathFailMessage.bind(this));

        //同步积分成功的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_BATTLE_SYN_SCORE, this.OnSyncScoreSuccessMessage.bind(this));
        //同步积分失败的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_BATTLE_SYN_SCORE_FAIL, this.OnSyncScoreFailMessage.bind(this));

        //复活成功的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_BATTLE_RELIVE_SUC, this.OnReliveSuccessMessage.bind(this));
        //复活失败的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_BATTLE_RELIVE_FAIL, this.OnReliveFailMessage.bind(this));

        //战斗结束成功的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_BATTLE_END_SUC, this.OnGameOverSuccessMessage.bind(this));
        //战斗结束失败的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_BATTLE_END_FAIL, this.OnGameOverFailMessage.bind(this));

        
        //战斗中结算奖励的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_END_AWARD_SUC, this.OnSettleRewardMessage.bind(this));

        //3倍奖励成功的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_VIEW_AWARD_SUC, this.OnThreeTimesRewardSuccessMessage.bind(this));
        //3倍奖励失败的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_VIEW_AWARD_FAIL, this.OnThreeTimesRewardFailMessage.bind(this));

        //红蓝对战战斗开始成功的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_TEAM_START_SUC, this.On5V5BettleStartSuccess.bind(this));
        //红蓝对战战斗开始失败的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_TEAM_START_FAIL, this.On5V5BettleStartFail.bind(this));

        //有玩家进入游戏的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_TEAM_LOADING_COMPLETE_SUC, this.On5V5PlayerJoinGameMessage.bind(this));

        //红蓝对战复活成功的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_TEAM_RELIVE_SUC, this.On5V5ReliveSuccessMessage.bind(this));
        //红蓝对战复活失败的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_TEAM_RELIVE_FAIL, this.On5V5ReliveFailMessage.bind(this));

        //红蓝对战中战斗结束成功的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_TEAM_END_SUC, this.On5V5GameOverSuccessMessage.bind(this));
        //红蓝对战中战斗结束失败的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_TEAM_END_FAIL, this.On5V5GameOverFailMessage.bind(this));

        //红蓝对战中同步积分成功的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_TEAM_SYN_SCORE_SUC, this.On5V5SyncScoreSuccessMessage.bind(this));
        //红蓝对战中同步积分失败的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_TEAM_SYN_SCORE_FAIL, this.On5V5SyncScoreFailMessage.bind(this));

        //5V5 3倍奖励的消息 成功
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_TEAM_VIEW_AWARD_SUC, this.On5V5ThreeTimesRewardSuccessMessage.bind(this));
        //5V5 3倍奖励的消息 失败
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_TEAM_VIEW_AWARD_FAIL, this.On5V5ThreeTimesRewardFailMessage.bind(this));
        
        //接受到好友邀请加入队伍的消息
        this.RegisterHandle(MsgID.MSG_BATTLE, Msg_Bettle_Type._BATTLE_ACTION_INVITE_FRIEND_JOIN, this.OnFriendInviteJoinMessage.bind(this));
    }

    //创建房间成功消息
    private OnCreateRoomSuccessMessage(msg)
    {

        GlobalVar.isMaster = true;

        GameData.SetRoomID(msg.team_id);
        GameData.SetRoomState(RoomState.RS_Invite);

        EventCenter.DispatchEvent(MessageType.DLD_Receive_Create_Room, msg);
    }

    //创建房间失败消息
    private OnCreateRoomFailMessage(msg)
    {
    }

    //快速匹配成功的消息
    private OnQuickMatchSuccessMessage(msg)
    {
        GameData.SetRoomState(RoomState.RS_Match);

        EventCenter.DispatchEvent(MessageType.DLD_Receive_Quick_Match, msg.result);
    }

    //快速匹配失败的消息
    private OnQuickMatchFailMessage(msg)
    {
    }
    
    //快速加入成功的消息
    private OnQuickJoinSuccessMessage(msg)
    {
        GlobalVar.isMaster = false;

        GameData.SetRoomState(RoomState.RS_Match);

        EventCenter.DispatchEvent(MessageType.DLD_Receive_Quick_Join, msg);
    }

    //快速加入失败的消息
    private OnQuickJoinFailMessage(msg)
    {
        GameLog.PrintLog("快速进入失败:", msg);
    }

    //退出房间成功
    private OnExitRoomSuccessMessage(msg)
    {
        GlobalVar.isMaster = false;

        GameData.SetRoomState(RoomState.RS_Null);

        EventCenter.DispatchEvent(MessageType.DLD_Exit_Room, msg);
    }

    //退出房间失败
    private OnExitRoomFailMessage(msg)
    {
        GameLog.PrintLog("退出房间失败:", msg);
    }


    //邀请好友加入房间成功的消息
    private OnInviteJoinRoomSuccessMessage(msg)
    {

        GameData.SetRoomState(RoomState.RS_Invite);

        // if(!PlayerMgr.IsExistPlayer(msg.uid))
        {
            GlobalVar.isMaster = false;
            EventCenter.DispatchEvent(MessageType.DLD_Receive_Join, msg.model);
        }
    }

    //邀请好友加入房间失败的消息
    private OnInviteJoinRoomFailMessage(msg)
    {
        EventCenter.DispatchEvent(MessageType.DLD_Receive_Join_Fail, msg.model);
    }

    //队长变更的消息
    private OnLeaderChangedMessage(msg)
    {
        if(PlayerMgr.IsSelf(msg.leader_id))
        {
            GlobalVar.isMaster = true;
        }

        EventCenter.DispatchEvent(MessageType.DLD_Leader_Changed);
    }

    //接受到好友邀请加入队伍的消息
    private OnFriendInviteJoinMessage(msg)
    {
        EventCenter.DispatchEvent(MessageType.Game_Invite_Event, msg);
    }

    //战斗开始成功的消息
    private OnBettleStartSuccess(msg)
    {
        if(msg){
            GameData.SetGameTime(msg.interval_time);
            GameData.SetGameStartTime(msg.timeStamp);

            for(let key in msg)
            {
                if(key.indexOf("members") >= 0)
                {
                    let member = msg[key];
                    if(member)
                    {
                        let player: PlayerData = PlayerMgr.GetPlayer(member.target_id);
                        if(player)
                        {
                            player.setBirthPointID( parseInt(member.region_id) || 0 );
                            player.setKillNum( parseInt(member.kill_sum) || 0);
                            player.setLevel( parseInt(member.fight_lev) || 0);
                            // player.setBirthDirection(MessageUtil.ConvertToFloat(member.directionX), MessageUtil.ConvertToFloat(member.directionY));
                            player.setBirthDirection(0, 1);
                            player.setScore( parseInt(member.fight_score) || 0 );
                            player.setGrandingID( parseInt(member.granding_id) || 0 );
                            player.setCurStarNum( parseInt(member.cur_star_sum) || 0 );
                        }
                    }
                }
            }

            EventCenter.DispatchEvent(MessageType.DLD_Bettle_Start);
        }
    }

    //战斗开始失败的消息
    private  OnBettleStartFail(msg)
    {

    }

    
    //有玩家进入游戏的消息
    private OnPlayerJoinGameMessage(msg)
    {
        let player = PlayerMgr.GetPlayer(msg.target_id);
        if(player)
        {
            player.setIsJoinGame(true);
            EventCenter.DispatchEvent(MessageType.DLD_Player_Join_Game, msg.target_id);
        }
    }


    // 废弃
    // //碰撞的消息 成功
    // OnCollideSuccessMessage(msg){
    //     GameData.GetBettleData().setImpluse(MessageUtil.ConvertToFloat(msg.power_x), MessageUtil.ConvertToFloat(msg.power_y));
    //     GameData.GetBettleData().setColliderID( parseInt(msg.target_id) );
    //     GameData.GetBettleData().setType( parseInt(msg.type) );

    //     EventCenter.DispatchEvent(MessageType.DLD_Recv_Collider);
    // }

    // //碰撞的消息 失败
    // OnCollideFailMessage(msg){

    // }


    // 废弃
    // 死亡的消息 成功
    private OnDeathSuccessMessage(msg)
    {
        // let decedentID: number = msg.power;     //死者
        // let killerID: number = msg.killer_id;   //击杀者

        // if(PlayerMgr.IsSelf(decedentID) && PlayerMgr.IsValid(killerID))
        // {
        //     //如果死者是我,并且不是自杀的话， 记录击杀者
        //     GameData.GetBettleData().setEnemyUserID(killerID);
        // }

        // if(PlayerMgr.IsSelf(killerID) && GameData.GetBettleData().getEnemyUserID() === decedentID)
        // {
        //     //击杀者是我,死者是我的仇人，复仇成功,清除仇人ID
        //     GameData.GetBettleData().setEnemyUserID(0);
        // }

        // if(PlayerMgr.IsValid(decedentID))
        // {
        //     //改变状态为死亡状态
        //     let playerData = PlayerMgr.GetPlayer(decedentID);
        //     playerData.setState(PlayerState.PS_WaitDeath);
        // }

        // EventCenter.DispatchEvent(MessageType.DLD_Receive_Death, msg);
    }

    //死亡的消息 失败
    private OnDeathFailMessage()
    {

    }

    //同步积分的消息 成功
    private OnSyncScoreSuccessMessage(msg)
    {
        let playerID: number = parseInt(msg.target_id);
        let player = PlayerMgr.GetPlayer(playerID);
        if(player)
        {
            let kingID: number = parseInt(msg.king_id) || 0;

            player.setKillNum( parseInt(msg.kill_sum) );
            player.setScore( parseInt(msg.fight_score) );
            // player.setIsOverlord(kingID === playerID);

            GameData.GetBettleData().setOverlordUserID( kingID );

            EventCenter.DispatchEvent(MessageType.DLD_Receive_Syn_Score);
        }
    }

    //同步积分的消息 失败
    private OnSyncScoreFailMessage(){

    }

    //废弃
    //复活的消息 成功
    private OnReliveSuccessMessage(msg){
        // GameData.GetBettleData().setReliveUserID( parseInt(msg.target_id) );
        // GameData.GetBettleData().setRelivePosID( parseInt(msg.region_id) );
        // GameData.GetBettleData().setReliveDirection( MessageUtil.ConvertToFloat(msg.directionX), MessageUtil.ConvertToFloat(msg.directionY) );

        // //TODO
        // if(msg.target_id == 778560)
        // {
        //     GameData.GetBettleData().setRelivePosID( 0 );
        //     GameData.GetBettleData().setReliveDirection( 1, 0 );
        // }
        // if(msg.target_id == 820454)
        // {
        //     GameData.GetBettleData().setRelivePosID( 8 );
        //     GameData.GetBettleData().setReliveDirection( -1, 0 );
        // }

        // // GameData.GetBettleData().setRelivePos( MessageUtil.ConvertToFloat(msg.point_x), MessageUtil.ConvertToFloat(msg.point_y) );

        // EventCenter.DispatchEvent(MessageType.DLD_Receive_Relive);
    }

    //复活的消息 失败
    private OnReliveFailMessage()
    {

    }

    //战斗结束的消息--成功
    private OnGameOverSuccessMessage(msg)
    {
        let settleData: DLDSettleData = GameData.GetDLDSettleData();
        if(settleData){
            for(let key in msg){
                if(key.indexOf("members") >=0 ){
                    let data = msg[key];
                    if(data){
                        settleData.InitPlayerSettleData(data);
                    }
                }
            }

            if(settleData.IsSettleSuc()){
                EventCenter.DispatchEvent(MessageType.DLD_Receive_Bettle_End);
            }

            settleData.SetSettleSuc();
        }
    }

    //战斗结束的消息--失败
    private OnGameOverFailMessage()
    {

    }

    //战斗结算奖励
    private OnSettleRewardMessage(msg)
    {
        let settleData: DLDSettleData = GameData.GetDLDSettleData();
        if(settleData)
        {
            settleData.SetGoldNum( parseInt(msg.award_gold) );
            settleData.SetBZLPNum( parseInt(msg.king_sum) );

            if(settleData.IsSettleSuc())
            {
                EventCenter.DispatchEvent(MessageType.DLD_Receive_Bettle_End);
            }

            settleData.SetSettleSuc();
        }
    }

    //3倍奖励的消息 成功
    private OnThreeTimesRewardSuccessMessage(msg)
    {
        let settleData: DLDSettleData = GameData.GetDLDSettleData();
        if(settleData){
            settleData.SetGoldNum( parseInt(msg.award_gold) );
            settleData.SetBZLPNum( parseInt(msg.king_sum) );
        }

        EventCenter.DispatchEvent(MessageType.DLD_Receive_Three_Reward);
    }

    //3倍奖励的消息 失败
    private OnThreeTimesRewardFailMessage()
    {

    }

    //5V5战斗开始成功的消息
    private On5V5BettleStartSuccess(msg)
    {
        if(msg){
            GameData.SetGameTime(msg.interval_time);
            GameData.SetGameStartTime(msg.timeStamp);

            for(let key in msg)
            {
                if(key.indexOf("members") >= 0)
                {
                    let member = msg[key];
                    if(member)
                    {
                        let userID: number = parseInt(member.target_id);

                        let player: PlayerData = PlayerMgr.GetPlayer(userID);

                        if(player === null)
                        {
                            PlayerMgr.AddPlayerByInfo(member);
                            
                            EventCenter.DispatchEvent(MessageType.DLD_Join_Player, userID);

                            player = PlayerMgr.GetPlayer(userID);
                        }

                        if(player)
                        {
                            let teamID: number = parseInt(member.team_id);
                            let teamType: number = parseInt(member.team_colour);
                            let carLev: number = parseInt(member.car_lev);
                            let posID: number = parseInt(member.region_id);

                            player.setBirthPointID( posID );
                            player.setKillNum(0);
                            player.setTeamType( teamType );
                            player.setLevel( carLev );
                            player.setScore(0);
                            player.setGrandingID( parseInt(member.granding_id) || 0 );
                            player.setCurStarNum( parseInt(member.cur_star_sum) || 0 );

                            PlayerMgr.AddTeamTypeByTeamID(teamID, teamType);
                        }
                    }
                }
            }

            EventCenter.DispatchEvent(MessageType.DLD_Bettle_Start);
        }
    }

    //5V5战斗开始失败的消息
    private On5V5BettleStartFail()
    {

    }

    //有玩家进入游戏的消息
    private On5V5PlayerJoinGameMessage(msg)
    {
        let playerID = parseInt(msg.target_id);

        let player: PlayerData = PlayerMgr.GetPlayer(playerID);
        if(player){
            player.setIsJoinGame(true);
            EventCenter.DispatchEvent(MessageType.DLD_Player_Join_Game, playerID);
        }
    }
    
    //废弃
    //红蓝对战复活成功的消息
    private On5V5ReliveSuccessMessage(msg)
    {
        // let playerID: number = parseInt(msg.target_id);
        // let posID: number = parseInt(msg.region_id);
        // let carLv: number = parseInt(msg.car_lev);

        // let player: PlayerData = PlayerMgr.GetPlayer(playerID);
        // if(player){
        //     player.setLevel(carLv);
        // }

        // GameData.GetBettleData().setRelivePosID( posID );
        // GameData.GetBettleData().setReliveUserID( playerID );

        // EventCenter.DispatchEvent(MessageType.DLD_Receive_Relive);
    }

    //红蓝对战复活失败的消息
    private On5V5ReliveFailMessage(msg)
    {

    }

    //5V5同步积分的消息 成功
    private On5V5SyncScoreSuccessMessage(msg)
    {
        let playerID: number = parseInt(msg.target_id);

        let bettleData = GameData.GetBettleData();
        if(msg && bettleData)
        {
            let score: number = parseInt(msg.score);
            let teamID: number = parseInt(msg.team_id);
            let teamType: TeamType = PlayerMgr.GetTeamTypeByTeamID(teamID);

            if(teamType === TeamType.TT_Red)
            {
                bettleData.setRedScore( score );
            }else if(teamType === TeamType.TT_Blue)
            {
                bettleData.setBlueScore( score );
            }
            bettleData.setTeamID( teamID );
            bettleData.setTargetUserID( playerID );

            EventCenter.DispatchEvent(MessageType.DLD_Receive_Syn_Score);
        }
    }

    //5V5同步积分的消息 失败
    private On5V5SyncScoreFailMessage()
    {

    }

    //5V5战斗结束的消息--成功
    private On5V5GameOverSuccessMessage(msg)
    {
        let redScore: number    = parseInt(msg.r_score);
        let blueScore: number   = parseInt(msg.b_score);

        let settleData = GameData.GetDQZSettleData();
        if(settleData){
            let redGold: number     = parseInt(msg.r_awardgold);
            let blueGold: number    = parseInt(msg.b_awardgold);
            let redResult: number   = parseInt(msg.r_flag);
            let blueResult: number  = parseInt(msg.b_flag);

            settleData.setRedScore( redScore );
            settleData.setBlueScore( blueScore );
            settleData.setRedGoldNum( redGold );
            settleData.setBlueGoldNum( blueGold );
            settleData.setRedResult( redResult );
            settleData.setBlueResult( blueResult );

            EventCenter.DispatchEvent( MessageType.DLD_Receive_Bettle_End);
        }

        let bettleData = GameData.GetBettleData();
        if(bettleData){
            bettleData.setRedScore( redScore );
            bettleData.setBlueScore( blueScore );

            EventCenter.DispatchEvent(MessageType.DLD_Receive_Syn_Score);
        }
    }

    //5V5战斗结束的消息--失败
    private On5V5GameOverFailMessage()
    {

    }

    //5V5 3倍奖励的消息 成功
    private On5V5ThreeTimesRewardSuccessMessage(msg)
    {
        let awardGold: number = parseInt(msg.award_gold);

        let settleData: DQZSettleData = GameData.GetDQZSettleData();
        if(settleData){
            settleData.setDoubleGold(awardGold);
        }

        EventCenter.DispatchEvent(MessageType.DLD_Receive_Three_Reward);
    }

    //5V53倍奖励的消息 失败
    private On5V5ThreeTimesRewardFailMessage()
    {
        
    }
}
