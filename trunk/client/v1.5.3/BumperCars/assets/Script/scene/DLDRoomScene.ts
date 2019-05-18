import AccountData from '../manager/AccountData';
import EngineUtil from '../common/EngineUtil';
import GlobalVar from '../GlobalVar';
import PlayerMgr from '../manager/PlayerMgr';
import RoomScene from './RoomScene';
import SceneMgr from '../manager/SceneMgr';
import WXHelper from '../common/WXHelper';
import { AtlasType } from '../common/EngineUtil';
import { GameModel } from '../GameDefine';
import { SceneType } from '../manager/SceneMgr';
import GameUI from '../manager/UIManager';
import { UIType } from '../UI/UIDefine';

const {ccclass, property} = cc._decorator;

@ccclass
export default class DLDRoomScene extends RoomScene
{
    //兑换奖品按钮
    @property({type: cc.Node})
    private exchangeBtn: cc.Node = null

    //进入商城按钮
    @property({type: cc.Node})
    private enterVasBtn: cc.Node = null
    
    //匹配玩家视图
    @property({type: cc.Node})
    private matchView: cc.Node = null

    //霸主令牌数量
    @property({type: cc.Node})
    private kingNode: cc.Node = null;

    private playerListUI = null;

    public OnInit() 
    {
        EngineUtil.SetNodeText(this.kingNode, AccountData.GetInstance().GetBZLPNum().toString());
        EngineUtil.RegBtnClickEvent(this.exchangeBtn, this.exchangeBtnClicked.bind(this));
        EngineUtil.RegBtnClickEvent(this.enterVasBtn, this.enterVasBtnClicked.bind(this));

        super.OnInit();
    }

    protected OnExit()
    {
        super.OnExit();

        this.playerListUI.length = 0;
        this.playerListUI = null;
    }

    //初始化玩家列表UI
    protected initPlayerList()
    {
        if(this.matchView)
        {
            if(this.matchPrefab)
            {
                this.playerListUI = [];
                for(let i = 0;i < 10;i ++)
                {
                    let matchItemNode = cc.instantiate(this.matchPrefab);
                    if(matchItemNode)
                    {
                        matchItemNode.state = 1;
                        this.matchView.addChild(matchItemNode);
                        this.playerListUI.push(matchItemNode);
                    }
                }
            }
        }

        let playerCount = PlayerMgr.GetPlayerCount();
        for(let i = 0;i < playerCount;i ++)
        {
            let player = PlayerMgr.GetPlayerByIndex(i);
            let playerUI = this.playerListUI[i];
            if(player && playerUI)
            {
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
        if(this.playerListUI)
        {
            for(let i = 0;i < this.playerListUI.length;i ++)
            {
                let playerUI = this.playerListUI[i];
                if(playerUI)
                {
                    playerUI.getComponent("MatchItem").refreshUI();
                }
            }
        }
    }

    //加入一个玩家
    protected addPlayer(userID)
    {
        let player = PlayerMgr.GetPlayer(userID);

        if(this.playerListUI && this.playerListUI.length > 0 && player)
        {
            for(let i = 0;i < this.playerListUI.length;i ++)
            {
                let playerUI = this.playerListUI[i];
                if(playerUI && playerUI.state != 2)
                {
                    playerUI.state = 2;
                    playerUI.userID = userID;
                    playerUI.getComponent("MatchItem").initView(player);
                    playerUI.getComponent("MatchItem").refreshUI();
                    break;
                }
            }
        }

        if(PlayerMgr.GetPlayerCount() >= 10)
        {
            this.showStartBtn();
        }
    }

    //移除一个玩家
    protected removePlayer(userID)
    {
        if(this.playerListUI && this.playerListUI.length > 0)
        {
            for(let i = 0;i < this.playerListUI.length;i ++)
            {
                let playerUI = this.playerListUI[i];
                if(playerUI && playerUI.userID == userID && playerUI.state == 2)
                {
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
        if(this.playerListUI){
            for(let i = 0;i < this.playerListUI.length;i ++)
            {
                let playerUI = this.playerListUI[i];
                if(playerUI && playerUI.state == 1)
                {
                    playerUI.state = 3;
                }
            }
        }
    }

    //快速匹配状态
    protected enterMatchState()
    {
        super.enterMatchState();
        
        this.setLoadingState();
        this.refreshPlayerList();
    }

    //兑换按钮点击
    private exchangeBtnClicked()
    {
        GameUI.ShowHint("敬请期待");
    }

    //进入商城按钮点击
    private enterVasBtnClicked()
    {
        GameUI.ShowUI(UIType.UT_VasView);
    }

    //显示开始按钮
    private showStartBtn()
    {
        EngineUtil.SetNodeTextureByType(AtlasType.AT_RoomScene, this.matchBtn, "Room_StartBtn");
    }

/**********************网络消息处理---Start*************************/
    //战斗开始
    protected bettleStartResp()
    {
        super.bettleStartResp();

        //大乱斗模式
        GlobalVar.gameModel = GameModel.GM_DLDMode;

        // //TODO
        // if(GlobalVar.userID == 579232)
        // {
        //     this.scheduleOnce(function(){
        //         SceneMgr.SwitchScene(SceneType.DLDGameScene);
        //     }, 60);
        // }else
        // {
        //     SceneMgr.SwitchScene(SceneType.DLDGameScene);
        // }
        SceneMgr.SwitchScene(SceneType.DLDGameScene);
    }
}