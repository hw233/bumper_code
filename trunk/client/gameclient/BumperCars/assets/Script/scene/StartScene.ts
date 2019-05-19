import AccountData from '../manager/AccountData';
import AddGoldEffect from '../component/AddGoldEffect';
import AdvertiseMgr from '../Advertise/AdvertiseMgr';
import AudioManager from '../common/AudioManager';
import CarItemView from '../component/CarItemView';
import EngineUtil from '../common/EngineUtil';
import EventCenter from '../common/EventCenter';
import FriendSystem from '../Friend/FriendSystem';
import GameLocalDataMgr from '../manager/GameLocalDataMgr';
import GameLog from '../Log/LogTool';
import GameUI from '../manager/UIManager';
import GlobalVar from '../GlobalVar';
import GoldEffect from '../component/GoldEffect';
import SceneBase from './SceneBase';
import SceneMgr from '../manager/SceneMgr';
import SignInSystem from '../SignIn/SignInSystem';
import WXHelper from '../common/WXHelper';
import { AdvertiseType } from '../Advertise/AdvertiseMgr';
import { GameModel, CarUnlockState } from '../GameDefine';
import { MessageType } from '../common/EventCenter';
import { RankList } from '../UI/RanklistLayer';
import { SceneType } from '../manager/SceneMgr';
import { UIType } from '../UI/UIDefine';
import GameTable from '../manager/TableManger';
import { TableType } from '../table/TableDefine';
import CarTableInfo from '../table/CarTableInfo';
import { TaskType } from '../Task/TaskDefine';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import ItemSystem from '../Item/ItemSystem';
import PanelItem from '../component/PanelItem';
import TaskSystem from '../Task/TaskSystem';

const {ccclass, property} = cc._decorator;

@ccclass
export default class StartScene extends SceneBase
{
    @property({type: cc.Node})
    private topNode: cc.Node = null

    @property({type: cc.Node})
    private topBg: cc.Node = null

    @property({type: cc.Node})
    private middleNode: cc.Node = null

    @property({type: cc.Node})
    private bottomNode: cc.Node = null

    @property({type: cc.Node})
    private carListNode: cc.Node = null

    //加油礼包按钮
    @property({type: cc.Node})
    private giftBtn: cc.Node = null

    //大乱斗按钮
    @property({type: cc.Node})
    private dldBtn: cc.Node = null

    //组队战按钮
    @property({type: cc.Node})
    private zdzBtn: cc.Node = null

    //练习赛按钮
    @property({type: cc.Node})
    private practiceBtn: cc.Node = null

    //车库按钮
    @property({type: cc.Node})
    private carLibBtn: cc.Node = null

    //菜单按钮
    @property(cc.Node)
    private menuBtn: cc.Node = null

    //任务按钮
    @property(cc.Node)
    private taskBtn: cc.Node = null

    //签到按钮
    @property(cc.Node)
    private signInBtn: cc.Node = null

    //其他游戏的按钮
    @property(cc.Node)
    private otherGameBtn: cc.Node = null

    @property(PanelItem)
    private otherPanelItem: PanelItem = null

    @property(cc.Node)
    private tryPlayBtn: cc.Node = null

    public OnInit () 
    {
        super.OnInit();

        EngineUtil.RegBtnClickEvent(this.giftBtn, this.giftBtnClicked.bind(this));
        EngineUtil.RegTouchEndEvent(this.taskBtn, this.taskBtnClicked.bind(this));
        EngineUtil.RegTouchEndEvent(this.signInBtn, this.signInBtnClicked.bind(this));
        EngineUtil.RegTouchEndEvent(this.otherGameBtn, this.otherGameBtnClicked.bind(this));
        EngineUtil.RegTouchEndEvent(this.menuBtn, this.menuBtnClicked.bind(this));
        EngineUtil.RegTouchEndEvent(this.dldBtn, this.dldBtnClicked.bind(this));
        EngineUtil.RegTouchEndEvent(this.zdzBtn, this.zdzBtnClicked.bind(this));
        EngineUtil.RegTouchEndEvent(this.practiceBtn, this.startBtnClicked.bind(this));
        EngineUtil.RegTouchEndEvent(this.carLibBtn, this.carLibBtnClicked.bind(this));
        EngineUtil.RegTouchEndEvent(this.tryPlayBtn, this.tryPlayClicked.bind(this));
        
        this.registerEvent(MessageType.ComeOnView_Receive_Suc, this.comeOnViewReceiveResp.bind(this));
        this.registerEvent(MessageType.CarBtnClicked, this.carBtnClicked.bind(this));
        this.registerEvent(MessageType.StartScene_RefreshCarList, this.refreshUI.bind(this));
        this.registerEvent(MessageType.StartScene_RefreshTopView, this.refreshTopUI.bind(this));
        this.registerEvent(MessageType.TimeLimit_Refresh_Event, this.timelimetResp.bind(this));

        if(cc.director.getWinSize().height / cc.director.getWinSize().width - 16 / 9 > 0.1)
        { 
            this.topBg.scaleY = 1.5;
            this.topNode.getComponent(cc.Widget).top = 50;
        }

        this.initLayer();

        GameUI.ShowUI(UIType.UT_FriendView);

        //处理游戏中收到的好友申请
        FriendSystem.GetInstance().HandleCacheFriendApply();

        //初始化签到数据
        SignInSystem.GetInstance().Init();

        GameLocalDataMgr.GetInstance().ReadSignIn();
    }

    public OnEnter () 
    {
        super.OnEnter();

        //首次进入自动打开签到页
        if(GameLocalDataMgr.GetInstance().GetSignIn() === false)
        {
            GameUI.ShowUI(UIType.UT_SignInView);
            GameLocalDataMgr.GetInstance().SetSignIn(true);
        }

        AdvertiseMgr.CreateBannerAd(AdvertiseType.ADConfig_Banner);

        ItemSystem.GetInstance().ShowExpireItem();

        SignInSystem.GetInstance().HintNewMessage();

        if(TaskSystem.GetInstance().IsGetTask() === false)
        {
            //获取任务列表
            Socket_SendMgr.GetTaskSend().SendGetTaskListMessage();
        }
    }

    protected OnExit()
    {
        super.OnExit();

        AdvertiseMgr.DestroyBannerAd();

        GameUI.HideUI(UIType.UT_FriendView);
    }

    private initLayer()
    {
        this.initCarListUI();
        this.refreshTopUI();

        if(GlobalVar.isOpenGroupRank)
        {
            //打开排行榜
            GlobalVar.isOpenGroupRank = false;

            this.showRankList(RankList.GroupRankList);
        }

        if(ItemSystem.GetInstance().IsHasItemByItemID(100020) === false)
        {
            //购买道具
            Socket_SendMgr.GetVasSend().SendBuyItemMessage(100020);
        }
    }

    //任务按钮点击
    private taskBtnClicked()
    {
        GameUI.ShowUI(UIType.UT_TaskView, TaskType.TT_DailyTask);
        EventCenter.DispatchEvent(MessageType.NewMessage_Task, true);
    }

    //签到按钮点击
    private signInBtnClicked()
    {
        GameUI.ShowUI(UIType.UT_SignInView);
    }

    //其他游戏按钮点击
    private otherGameBtnClicked()
    {
        if(this.otherPanelItem)
        {
            this.otherPanelItem.onBtnClickItem();
        }
    }

    //组队战按钮点击
    private  zdzBtnClicked()
    {
        //组队模式
        GlobalVar.gameModel = GameModel.GM_DQZMode;

        GameUI.ShowUI(UIType.UT_CreateRoomView, GameModel.GM_DQZMode);
    }

    //大乱斗按钮点击
    private dldBtnClicked()
    {
        //大乱斗模式
        GlobalVar.gameModel = GameModel.GM_DLDMode;

        GameUI.ShowUI(UIType.UT_CreateRoomView, GameModel.GM_DLDMode);
    }

    //加油礼包按钮点击
    private giftBtnClicked()
    {
        GameUI.ShowUI(UIType.UT_ComeOnView);
    }

    //菜单按钮点击
    private menuBtnClicked()
    {
        GameUI.ShowUI(UIType.UT_MenuView);
    }

    //开始按钮点击
    private startBtnClicked()
    {
        AudioManager.PlayBtnSound();

        //练习模式
        GlobalVar.gameModel = GameModel.GM_PracticeMode;

        SceneMgr.SwitchScene(SceneType.GameScene);
    }

    //车库按钮点击
    private carLibBtnClicked()
    {
        AudioManager.PlayBtnSound();

        GameUI.ShowUI(UIType.UT_CarLibView, AccountData.GetInstance().GetUseCarID());
    }

    //车按钮点击
    private carBtnClicked(data)
    {
        AudioManager.PlayBtnSound();

        if(data.state === CarUnlockState.CUS_Using)
        {
            //出战中 不做处理
        }
        else if(data.state === CarUnlockState.CUS_AlreadyUnlock)
        {
            //已解锁 点击使用
            let soleID: number = ItemSystem.GetInstance().GetSoleIDByItemID(data.carID);

            Socket_SendMgr.GetCommonSend().SendInUseCarID(soleID);
        }
        else if(data.state === CarUnlockState.CUS_NotUnlock)
        {
            //未解锁 跳转至车库解锁
            GameUI.ShowUI(UIType.UT_CarLibView, data.carID);
        }
    }

    //试玩按钮点击
    private tryPlayClicked()
    {
        GameUI.ShowUI(UIType.UT_TaskView, TaskType.TT_TryPlayTask);
    }

    //播放金币特效
    private playGoldEffect()
    {
        let self = this;

        let goldEffectPrefab: cc.Prefab = EngineUtil.GetPrefab("GoldEffectNode");
        if(goldEffectPrefab)
        {
            let goldNode = cc.instantiate(goldEffectPrefab);
            let goldNumNode: cc.Node = this.topNode.getChildByName("GoldNum");
            if(goldNumNode)
            {
                let targetWorldPos: cc.Vec2 = goldNumNode.convertToWorldSpaceAR(cc.Vec2.ZERO);
                let goldEffect: GoldEffect = goldNode.getComponent("GoldEffect");
                if(goldEffect)
                {
                    goldEffect.playEffect(targetWorldPos, function()
                    {
                        if(goldNumNode)
                        {
                            let addGoldEffect: AddGoldEffect = goldNumNode.getComponent("AddGoldEffect");
                            if(addGoldEffect)
                            {
                                addGoldEffect.playEffect();
                            }
                        }
                    });
                }
            }
            self.node.addChild(goldNode);
        }
    }

    //显示排行榜
    private showRankList(type)
    {
        GameUI.ShowUI(UIType.UT_RankListView, type);
    }

    private refreshUI()
    {
        if(this.middleNode)
        {
            if(this.carListNode)
            {
                let itemID = AccountData.GetInstance().GetUseCarID();
                let content = this.carListNode.getComponent(cc.ScrollView).content;
                for(let i = 0;i < content.children.length;i ++)
                {
                    let child = content.children[i];
                    if(child)
                    {
                        let carViewCom: CarItemView = child.getComponent('CarItemView');
                        if(carViewCom)
                        {
                            carViewCom.refreshView(carViewCom.carID, carViewCom.carID == itemID);
                        }
                    }
                }
            }
        }
    }

    private refreshTopUI()
    {
        if(this.topNode)
        {
            let headImg = this.topNode.getChildByName("HeadImg");
            let playerName = this.topNode.getChildByName("PlayerName");

            EngineUtil.LoadRemoteImg(headImg, AccountData.GetInstance().GetHeadImg());
            EngineUtil.SetNodeText(playerName, AccountData.GetInstance().GetNickName());

            let KillNumNode = this.topNode.getChildByName("KillNum");
            if(KillNumNode)
            {
                EngineUtil.SetNodeText(KillNumNode, ItemSystem.GetInstance().GetDebrisNum().toString());
            }

            let GoldNumNode = this.topNode.getChildByName("GoldNum");
            if(GoldNumNode)
            {
                EngineUtil.SetNodeText(GoldNumNode, ItemSystem.GetInstance().GetGoldNum().toString());
            }
        }
    }

    private initCarListUI()
    {
        if(this.middleNode)
        {
            if(this.carListNode)
            {
                let content = this.carListNode.getComponent(cc.ScrollView).content;
                let itemID = AccountData.GetInstance().GetUseCarID();
                if(content)
                {
                    let carNum: number = GameTable.GetInstance().GetDataCount(TableType.TTD_CarType);
                    for(let i: number = 0, len = carNum; i < len; i ++)
                    {
                        let carInfo: CarTableInfo = GameTable.GetInstance().GetDataInfoByIndex(TableType.TTD_CarType, i);
                        if(carInfo)
                        {
                            let carID: number = carInfo.GetID();
                            let carItemPrefab: cc.Prefab = EngineUtil.GetPrefab("CarItemView");
                            let carItem = cc.instantiate(carItemPrefab);
                            if(carItem)
                            {
                                content.addChild(carItem);
                                let carItemView = carItem.getComponent("CarItemView");
                                if(carItemView)
                                {
                                    carItemView.refreshView(carID, itemID === carID);
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    //限时领奖成功
    private timelimetResp(remainTime: number)
    {
        GameUI.ShowUI(UIType.UT_TryUseSucView, remainTime);
    }

    //领取加油礼包
    private comeOnViewReceiveResp()
    {
        this.playGoldEffect();
        this.refreshUI();
        this.refreshTopUI();
    }
}
