import EngineUtil from "../common/EngineUtil";
import AudioManager from "../common/AudioManager";
import PopUpBase from "./PopUpBase";
import AdvertiseMgr from "../Advertise/AdvertiseMgr";
import WXHelper from "../common/WXHelper";
import SceneMgr from "../manager/SceneMgr";
import {ShareSceneType} from "../common/ShareSceneType";
import {AdvertiseType} from "../Advertise/AdvertiseMgr";
import {SceneType} from "../manager/SceneMgr";
import ConfigData from '../common/ConfigData';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import GameLocalDataMgr from '../manager/GameLocalDataMgr';
import AccountData from '../manager/AccountData';
import GameUI from '../manager/UIManager';
import GameConfig from '../GameConfig';

const {ccclass, property} = cc._decorator;

//加油礼包弹窗
@ccclass
export default class ComeOnGiftView extends PopUpBase
{
    @property(cc.Node)
    protected receiveBtn = null;

    @property({type: cc.Node})
    private conditionNode: cc.Node = null;

    @property({type: cc.Node})
    private receiveLayer: cc.Node = null;

    @property({type: cc.Node})
    private hintLayer: cc.Node = null;

    @property({type: cc.Node})
    private goldNum: cc.Node = null;

    @property({type: cc.Node})
    private starNum: cc.Node = null;

    @property({type: cc.Node})
    private watchTip: cc.Node = null;

    @property({type: cc.Node})
    private startBtn: cc.Node = null;

    
    public OnInit () 
    {
        super.OnInit();

        this.registerTouchEndEvent(this.receiveBtn, this.receiveBtnClicked.bind(this));
        this.registerTouchEndEvent(this.startBtn, this.startBtnClicked.bind(this));

        GameLocalDataMgr.GetInstance().ReadPlayGameCount();
    }

    public OnExit()
    {
        super.OnExit();
    }

    public RefreshView()
    {
        super.RefreshView();

        let receiveCount = AccountData.GetInstance().GetReceiveGiftCount();
        let playGameCount = GameLocalDataMgr.GetInstance().GetPlayGameCount();
        if(receiveCount >= 4)
        {
            this.receiveLayer.active = false;
            this.hintLayer.active = true;
            EngineUtil.SetNodeText(this.conditionNode, this.getHintString(receiveCount));
        }
        else if(playGameCount >= (receiveCount * 5) )
        {
            //可以领取
            this.receiveLayer.active = true;
            this.hintLayer.active = false;
            EngineUtil.SetNodeText(this.goldNum, GameConfig.ComeGiftGoldNum.toString());
            EngineUtil.SetNodeText(this.starNum, GameConfig.ComeGiftStrNum.toString());
        }
        else
        {
            this.receiveLayer.active = false;
            this.hintLayer.active = true;
            EngineUtil.SetNodeText(this.conditionNode, this.getHintString(receiveCount, playGameCount));
        }

        this.watchTip.active = (GameLocalDataMgr.GetInstance().GetWatchVideoCount() < 5);
    }

    private receiveBtnClicked()
    {
        let self = this;

        AudioManager.PlayBtnSound();

        AdvertiseMgr.CreateVideoAd(AdvertiseType.ADConfig_Video_JiaYou, function(isFinish){
            if(isFinish)
            {
                GameUI.HideUI(self.GetType());
                GameLocalDataMgr.GetInstance().SetPlayGameCount(0);
                Socket_SendMgr.GetCommonSend().SendReceiveComeOnGift();
            }
        }, ShareSceneType.ReceiveGift);
    }

    private startBtnClicked()
    {
        AudioManager.PlayBtnSound();

        AdvertiseMgr.DestroyBannerAd();

        SceneMgr.SwitchScene(SceneType.GameScene);
    }

    private getHintString(receiveCount, playgameCount = 0)
    {
        if(receiveCount >= 4)
        {
            return "请您明天再来";
        }

        if(playgameCount >= (receiveCount * 5) )
        {
            return "";
        }

        return "完成" + (receiveCount * 5 - playgameCount) + "局比赛获得额外次数";
    }
}