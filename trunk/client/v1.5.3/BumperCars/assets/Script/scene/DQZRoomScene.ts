import GlobalVar from '../GlobalVar';
import PlayerData from '../data/PlayerData';
import PlayerMgr from '../manager/PlayerMgr';
import RoomScene from './RoomScene';
import SceneMgr from '../manager/SceneMgr';
import { GameModel } from '../GameDefine';
import { SceneType } from '../manager/SceneMgr';

const {ccclass, property} = cc._decorator;

@ccclass
export default class DQZRoomScene extends RoomScene
{
    //自己队伍
    @property({type: cc.Node})
    private selfTeamView: cc.Node = null

    //对方的队伍
    @property({type: cc.Node})
    private otherTeamView: cc.Node = null

    @property({type: cc.Animation})
    private vsAnimate: cc.Animation = null

    private selfTeamUIList = null
    private otherTeamUIList = null

    public OnInit () 
    {
        super.OnInit();
    }

    protected OnExit()
    {
        super.OnExit();

        this.selfTeamUIList.length = 0;
        this.otherTeamUIList.length = 0;
        this.selfTeamUIList = null;
        this.otherTeamUIList = null;
    }

    //初始化玩家列表
    protected initPlayerList()
    {
        if(this.selfTeamView && this.otherTeamView)
        {
            if(this.matchPrefab)
            {
                this.selfTeamUIList = [];
                this.otherTeamUIList = [];
                for(let i = 0; i < 5; i ++)
                {
                    let matchItemNode1 = cc.instantiate(this.matchPrefab);
                    if(matchItemNode1){
                        matchItemNode1.state = 1;
                        this.selfTeamView.addChild(matchItemNode1);
                        this.selfTeamUIList.push(matchItemNode1);
                    }

                    let matchItemNode2 = cc.instantiate(this.matchPrefab);
                    if(matchItemNode2){
                        matchItemNode2.state = 1;
                        this.otherTeamView.addChild(matchItemNode2);
                        this.otherTeamUIList.push(matchItemNode2);
                    }
                }
            }
        }

        let playerCount = PlayerMgr.GetPlayerCount();
        for(let i = 0; i < playerCount; i ++)
        {
            let player:PlayerData = PlayerMgr.GetPlayerByIndex(i);
            if(!player)continue;

            let teamID = player.getTeamID();

            let teamUI = this.getTeamUI(teamID);
            if(teamUI || teamUI.length <= 0)continue;

            teamUI.teamID = teamID;
            let playerUI = teamUI[i];
            if(playerUI && playerUI.state != 2){
                playerUI.state = 2;
                playerUI.userID = player.getUserID();
                playerUI.getComponent("MatchItem").initView(player);
                playerUI.getComponent("MatchItem").refreshUI();
            }
        }
    }

    //刷新玩家列表
    protected refreshPlayerList()
    {

        for(let i = 0;i < this.selfTeamUIList.length;i ++){
            let playerUI = this.selfTeamUIList[i];
            if(playerUI){
                playerUI.getComponent("MatchItem").refreshUI();
            }
        }

        for(let i = 0;i < this.otherTeamUIList.length;i ++){
            let playerUI = this.otherTeamUIList[i];
            if(playerUI){
                playerUI.getComponent("MatchItem").refreshUI();
            }
        }
    }

    //加入一个玩家
    protected addPlayer(userID)
    {
        let player = PlayerMgr.GetPlayer(userID);
        if(!player)return;

        let teamID = player.getTeamID();

        let teamUI = this.getTeamUI(teamID);

        if(teamUI && teamUI.length > 0){
            teamUI.teamID = teamID;
            for(let i = 0; i < teamUI.length; i ++){
                let playerUI = teamUI[i];
                if(playerUI && playerUI.state != 2){
                    playerUI.state = 2;
                    playerUI.userID = userID;
                    playerUI.getComponent("MatchItem").initView(player);
                    playerUI.getComponent("MatchItem").refreshUI();
                    break;
                }
            }
        }
    }

    //移除一个玩家
    protected removePlayer(userID)
    {
        let player = PlayerMgr.GetPlayer(userID);
        if(!player)return;

        let teamID = player.getTeamID();

        let teamUI = this.getTeamUI(teamID);

        if(teamUI && teamUI.length > 0){
            for(let i = 0;i < teamUI.length;i ++){
                let playerUI = teamUI[i];
                if(playerUI && playerUI.userID == userID && playerUI.state == 2){
                    playerUI.userID = 0;
                    playerUI.state = (this.isMatchState ? 3 : 1);
                    playerUI.getComponent("MatchItem").refreshUI();
                }
            }
        }
    }

    //改变没有玩家的位置为加载状态
    protected setLoadingState()
    {
        for(let i = 0;i < this.selfTeamUIList.length;i ++){
            let playerUI = this.selfTeamUIList[i];
            if(playerUI && playerUI.state == 1){
                playerUI.state = 3;
            }
        }

        for(let i = 0;i < this.otherTeamUIList.length;i ++){
            let playerUI = this.otherTeamUIList[i];
            if(playerUI && playerUI.state == 1){
                playerUI.state = 3;
            }
        }
    }

    //快速匹配状态
    protected enterMatchState()
    {
        super.enterMatchState();
        
        //播放VS动画
        if(this.vsAnimate)
        {
            this.vsAnimate.play();
            this.vsAnimate.on('finished', ()=>{
                this.setLoadingState();
                this.refreshPlayerList();
            });
        }
    }

    //根据队伍ID获得队伍UI
    private getTeamUI(teamID)
    {
        if(this.selfTeamUIList.teamID === undefined || this.selfTeamUIList.teamID === teamID)
        {
            return this.selfTeamUIList;
        }else if(this.otherTeamUIList.teamID === undefined || this.otherTeamUIList.teamID === teamID)
        {
            return this.otherTeamUIList;
        }
        return null;
    }

/**********************网络消息处理---Start*************************/

    //战斗开始
    protected bettleStartResp()
    {
        super.bettleStartResp();

        //组队战模式
        GlobalVar.gameModel = GameModel.GM_DQZMode;

        // //TODO
        // if(GlobalVar.userID == 579232)
        // {
        //     this.scheduleOnce(function(){
        //         SceneMgr.SwitchScene(SceneType.ZDZGameScene);
        //     }, 60);
        // }else
        // {
        //     SceneMgr.SwitchScene(SceneType.ZDZGameScene);
        // }

        this.scheduleOnce(function()
        {
            SceneMgr.SwitchScene(SceneType.ZDZGameScene);
        }, 1);
    }
/**********************网络消息处理---End*************************/
}
