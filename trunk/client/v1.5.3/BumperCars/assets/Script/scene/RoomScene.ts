import EngineUtil from "../common/EngineUtil";
import WXHelper from "../common/WXHelper";
import {RoomState} from "../GameDefine";
import GlobalVar from "../GlobalVar";
import EventCenter from "../common/EventCenter";
import {MessageType} from "../common/EventCenter";
import GameData from '../data/GameData';
import Socket_SendMgr from "../network/socket/Socket_SendManager";
import PlayerMgr from "../manager/PlayerMgr";
import SceneMgr from "../manager/SceneMgr";
import {SceneType} from "../manager/SceneMgr";
import FSPManager from '../FSP/FSPManager';
import GameUI from '../manager/UIManager';
import GameLocalDataMgr from '../manager/GameLocalDataMgr';
import SceneBase from './SceneBase';
import { UIType } from "../UI/UIDefine";

const {ccclass, property} = cc._decorator;

@ccclass
export default class RoomScene extends SceneBase
{
    //返回按钮
    @property({type: cc.Node})
    protected returnBtn: cc.Node      = null

    //邀请按钮
    @property({type: cc.Node})
    protected inviteBtn: cc.Node      = null

    //匹配按钮
    @property({type: cc.Node})
    protected matchBtn: cc.Node       = null

    //匹配时间
    @property({type: cc.Node})
    protected matchTimeNode: cc.Node  = null

    //匹配提示
    @property({type: cc.Node})
    protected matchTip: cc.Node       = null

    //匹配项预制体
    @property({type: cc.Prefab})
    protected matchPrefab: cc.Prefab  = null

    protected isMatchState: boolean   = null;
    protected matchTime: number       = null;

    public OnInit() 
    {
        super.OnInit();

        this.matchTime              = 15;
        this.matchTimeNode.active   = false; 
        this.matchTip.active        = false; 
        this.isMatchState           = false;

        //初始化玩家管理器
        PlayerMgr.InitMgr();

        //初始化帧管理器
        FSPManager.GetInstance().Init();

        //初始化网络通信数据
        GameData.InitNetData();

        EngineUtil.SetNodeText(this.matchTimeNode, this.matchTime.toString());
        EngineUtil.RegBtnClickEvent(this.returnBtn, this.returnBtnClicked.bind(this));
        EngineUtil.RegBtnClickEvent(this.inviteBtn, this.inviteBtnClicked.bind(this));

        this.registerEvent(MessageType.DLD_Join_Player, this.joinPlayerResp.bind(this));
        this.registerEvent(MessageType.DLD_Leave_Player, this.leavePlayerResp.bind(this));
        this.registerEvent(MessageType.DLD_Bettle_Start, this.bettleStartResp.bind(this));
        this.registerEvent(MessageType.DLD_Receive_Quick_Match, this.quickMatchResp.bind(this));
        this.registerEvent(MessageType.DLD_Exit_Room, this.exitRoomResp.bind(this));
        this.registerEvent(MessageType.DLD_Leader_Changed, this.leaderChangedResp.bind(this));
        this.registerEvent(MessageType.DLD_Reconnect_Suc, this.reconnectSuc.bind(this));

        this.initPlayerList();

        this.refreshMatchBtn();

        GameUI.ShowUI(UIType.UT_FriendView);
    }

    public OnEnter () 
    {
        super.OnEnter();

        //如果是快速加入的话直接进入匹配状态
        if(GameData.GetRoomState() === RoomState.RS_Match)
        {
            this.enterMatchState();
        }
    }

    protected OnExit()
    {
        super.OnExit();

        GameUI.HideUI(UIType.UT_FriendView);
    }

    protected initPlayerList()
    {
    }

    //返回大厅按钮点击
    private returnBtnClicked()
    {
        //发送离开房间的消息
        Socket_SendMgr.GetBettleSend().SendLeaveRoomMessage();
    }

    //邀请按钮点击
    private inviteBtnClicked()
    {
        WXHelper.InviteJoinShare(GameData.GetRoomID(), GlobalVar.gameModel, null);
    }

    //匹配按钮点击
    private matchBtnClicked()
    {
        //发送快速匹配的消息
        Socket_SendMgr.GetBettleSend().SendQuickMatchMessage();
    }

    //刷新匹配时间
    protected refreshMatchTime()
    {
        this.matchTime --;

        if(this.matchTime <= 0)
        {
            this.matchTime = 15;
        }
        
        EngineUtil.SetNodeText(this.matchTimeNode, this.matchTime.toString());
    }  

    //进入快速匹配状态
    protected enterMatchState()
    {
        this.matchTimeNode.active = true; 
        this.matchTip.active = true; 

        this.isMatchState = true;
        
        EngineUtil.SetButtonInteractable(this.matchBtn, false);
        EngineUtil.SetButtonInteractable(this.inviteBtn, false);

        this.schedule(this.refreshMatchTime, 1);
    }

    //刷新玩家列表
    protected refreshPlayerList()
    {
    }

    //加入一个玩家
    protected addPlayer(userID: number)
    {
    }

    //移除一个玩家
    protected removePlayer(userID: number)
    {
    }

    //改变没有玩家的位置为加载状态
    protected setLoadingState()
    {
    }

    
    //刷新匹配按钮(只有房主可以展示)
    private refreshMatchBtn()
    {
        if(GlobalVar.isMaster)
        {
            EngineUtil.RegBtnClickEvent(this.matchBtn, this.matchBtnClicked.bind(this));
        }
        EngineUtil.SetButtonInteractable(this.matchBtn, GlobalVar.isMaster);
    }

/**********************网络消息处理---Start*************************/
    //战斗开始
    protected bettleStartResp()
    {
        //保存最近玩家数据
        GameLocalDataMgr.GetInstance().SaveRecentlyPlayer();

        //开始游戏计时
        EventCenter.DispatchEvent(MessageType.Start_Game_Time_Count);
    }

    //重连成功的消息
    private reconnectSuc()
    {
        PlayerMgr.Clear();

        SceneMgr.SwitchScene(SceneType.StartScene);
    }

    //快速匹配的消息
    private quickMatchResp()
    {
        this.enterMatchState();
    }

    //退出房间的消息
    private exitRoomResp()
    {
        SceneMgr.SwitchScene(SceneType.StartScene);
    }

    //有玩家离开
    private leavePlayerResp(userID)
    {
        //移除一个玩家
        this.removePlayer(userID);
    }

    //有玩家加入
    private joinPlayerResp(userID)
    {
        //添加一个玩家
        this.addPlayer(userID);
    }

    //队长变更
    private leaderChangedResp()
    {
        this.refreshMatchBtn();
    }
}