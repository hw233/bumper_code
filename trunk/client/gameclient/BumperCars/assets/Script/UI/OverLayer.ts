import Util from "../common/Util";
import {ShareSceneType} from "../common/ShareSceneType";
import {AdvertiseType} from "../Advertise/AdvertiseMgr";
import AdvertiseMgr from "../Advertise/AdvertiseMgr";
import GameData from '../data/GameData';
import EventCenter from "../common/EventCenter";
import {MessageType} from "../common/EventCenter";
import Http_SendMgr from "../network/http/Http_SendManager";
import GameConfig from "../GameConfig";
import AudioManager from "../common/AudioManager";
import {SoundType} from "../common/AudioManager";
import {GradingSystem} from "../Grading/GradingSystem";
import EngineUtil from "../common/EngineUtil";
import ConfigData from '../common/ConfigData';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import { AtlasType } from '../common/EngineUtil';
import GameLocalDataMgr from '../manager/GameLocalDataMgr';
import AccountData from '../manager/AccountData';
import UIBase from './UIBase';
import GameUI from '../manager/UIManager';
import { UIType } from "./UIDefine";

const {ccclass, property} = cc._decorator;

@ccclass
export default class OverLayer extends UIBase
{
    @property({type: cc.Node})
    private rankNum: cc.Node = null;
        
    @property({type: cc.Node})
    private killNum: cc.Node = null;

    @property({type: cc.Node})
    private goldNum: cc.Node = null;

    @property({type: cc.Node})
    private continueKillNode: cc.Node = null;

    @property({type: cc.Node})
    private gradingIcon: cc.Node = null;

    @property({type: cc.Node})
    private gradingText: cc.Node = null;

    @property({type: cc.Node})
    private showOffBtn: cc.Node = null;

    @property({type: cc.Node})
    private resetGameBtn: cc.Node = null;

    @property([cc.Node])
    private fullStarNode:Array<cc.Node> = [];

    @property({type: cc.Node})
    private fullStarLayer: cc.Node = null;

    @property({type: cc.Node})
    private emptyStarLayer: cc.Node = null;

    @property({type: cc.Node})
    private wangzheLayer: cc.Node = null;

    @property({type: cc.Node})
    private popupLyaer: cc.Node = null;

    @property({type: cc.Node})
    private retrunBtn: cc.Node = null;

    @property({type: cc.Node})
    private wangzheStarNum: cc.Node = null;

    @property({type: cc.Node})
    private moreGameLayout: cc.Node = null;

    @property({type: cc.Node})
    private watchTip: cc.Node = null;

    @property({type: cc.Prefab})
    private upLevelPerfab: cc.Prefab = null;

    @property({type: cc.Prefab})
    private cblPrefab: cc.Prefab = null;

    @property({type: cc.Prefab})
    private keepStarPrefab: cc.Prefab = null;
    
    private tempStarNum: number = null;

    private whangzheStar: cc.Node = null;

    //结算前的数据
    private curGradingID: number = null;
    private curStarNum: number = null;

    public OnInit () 
    {
        super.OnInit();

        this.node.zIndex = 100;
        EngineUtil.RegBtnClickEvent(this.retrunBtn, this.returnBtnClicked.bind(this));
        EngineUtil.RegBtnClickEvent(this.showOffBtn, this.showOffClicked.bind(this));
        EngineUtil.RegBtnClickEvent(this.resetGameBtn, this.resetGameClicked.bind(this));
        
        GameLocalDataMgr.GetInstance().ReadPlayGameCount();
        GameLocalDataMgr.GetInstance().ReadVideoCount();

        GameLocalDataMgr.GetInstance().SetPlayGameCount(GameLocalDataMgr.GetInstance().GetPlayGameCount() + 1);

        this.showOffBtn.active = GameConfig.Switch_Relive;
        this.watchTip.active = GameConfig.Switch_Relive;

        this.registerEvent(MessageType.Keep_Star, this.keepStar.bind(this) );
        this.registerEvent(MessageType.OverLayer_Get_Grading_Suc, this.getGradingDataResp.bind(this) );

        this.initMoreGameUI();
        AdvertiseMgr.CreateBannerAd(AdvertiseType.ADConfig_Banner);

        //保存数据
        GameData.Save();

        //上传游戏数据
        Socket_SendMgr.GetCommonSend().SendGoldMessage(GameData.GetTempData().GetCurGoldNum(), GameData.GetTempData().GetCurKillNum());
    }
    
    private initMoreGameUI()
    {
        let self = this;

        Http_SendMgr.SendGetGameListMessage(GameConfig.GameListID, function(data){
            if(data){
                let gameList = data.redirect;
                for (let i = 0; i < gameList.length, i < 4; i++) {
                    let item = cc.instantiate(self.cblPrefab);
                    let script = item.getComponent("PanelItem");
                    item.setPosition(0, 0);
                    if (script) {
                        let directInfo = {
                            img_url: gameList[i].img_url,
                            game_id: gameList[i].game_id,
                            name: gameList[i].name,
                            app_id: data.hz_app_id,
                            path: data.hz_path
                        };
                        script.setItemData(directInfo);
                    }
                    self.moreGameLayout.addChild(item);
                }
            }
        });
    }

    public RefreshView()
    {
        super.RefreshView();
        
        this.curGradingID   = AccountData.GetInstance().GetCurLevelID();
        this.curStarNum     = AccountData.GetInstance().GetCurStarNum();

        let curRankNum      = GameData.GetTempData().GetRankNum();
        let curKillNum      = GameData.GetTempData().GetCurKillNum();
        let curGoldNum      = GameData.GetTempData().GetCurGoldNum();
        let maxKillNum      = GameData.GetTempData().GetMaxContinueKillNum();

        EngineUtil.SetNodeText(this.goldNum, curGoldNum);
        EngineUtil.SetNodeText(this.rankNum, curRankNum);
        EngineUtil.SetNodeText(this.killNum, curKillNum);
        EngineUtil.SetNodeText(this.wangzheStarNum, this.curStarNum.toString());
        EngineUtil.SetNodeText(this.gradingText, GradingSystem.GetGradingName(this.curGradingID));
        EngineUtil.SetNodeTextureByType(AtlasType.AT_Common, this.gradingIcon, GradingSystem.GetGradingFileName(this.curGradingID));

        if(curRankNum == 1)
        {
            AudioManager.PlaySoundEffect(SoundType.ST_Nomber_One);
        }

        if(maxKillNum > 1){
            //要改请注意，UILayer.js
            let fileName = null;
            switch(maxKillNum){
                case 2:
                {
                    fileName = "shuangsha";
                    break;
                }
                case 3:
                {
                    fileName = "sansha";
                    break;
                }
                case 4:
                {
                    fileName = "fengkuangshalu";
                    break;
                }
                case 5:
                {
                    fileName = "baozou";
                    break;
                }
            }
            if(fileName)
            {
                EngineUtil.SetNodeTextureByType(AtlasType.AT_GameOver, this.continueKillNode, fileName);
            }
        }else{
            this.continueKillNode.active = false;
        }

        if(this.curGradingID >= 31)
        {
            this.fullStarLayer.active = false;
            this.emptyStarLayer.active = false;
            this.wangzheLayer.active = true;
        }else
        {
            this.fullStarLayer.active = true;
            this.emptyStarLayer.active = true;
            this.wangzheLayer.active = false;

            if(this.fullStarNode && this.fullStarNode.length > 0)
            {
                for(let i = 0;i < this.fullStarNode.length;i ++)
                {
                    let fullStar = this.fullStarNode[i];
                    if(fullStar)
                    {
                        fullStar.active = i < this.curStarNum;
                    }
                }
            }
        }

        Socket_SendMgr.GetCommonSend().SendGetSettleInfoMessage(GameData.GetTempData().GetRankNum());
    }

    //连续加星逻辑
    private continueAddStar()
    {
        if(this.curGradingID >= 31)
        {
            if(this.tempStarNum && this.tempStarNum > 0)
            {
                this.tempStarNum --;
                this.addStar(this.continueAddStar.bind(this));
            }
            else
            {
                this.playStarFinish();
            }
        }else
        {
            if(this.curStarNum >= 5)
            {
                this.playStarFinish();
            }else
            {
                if(this.tempStarNum && this.tempStarNum > 0)
                {
                    this.tempStarNum --;
                    this.addStar(this.continueAddStar.bind(this));
                }else
                {
                    this.playStarFinish();
                }
            }
        }
    }

    //加星
    private addStar(callback)
    {
        //声音
        AudioManager.PlaySoundEffect(SoundType.ST_AddStar);

        let curStarNum = this.curStarNum;

        this.curStarNum += 1;

        if(this.curGradingID >= 31)
        {

            if(curStarNum > 0)
            {
                this.wangzheLayer.getChildByName("FullStar1").active = true;
                this.whangzheStar = this.wangzheLayer.getChildByName("FullStar2");
                this.whangzheStar.active = false;
            }else if(curStarNum == 0)
            {
                this.wangzheLayer.getChildByName("FullStar2").active = false;
                this.whangzheStar = this.wangzheLayer.getChildByName("FullStar1");
                this.whangzheStar.active = false;
            }

            if(this.whangzheStar)
            {
                this.whangzheStar.active = true;
                this.whangzheStar.getComponent(cc.Animation).play('starAnim');
                this.whangzheStar.getComponent(cc.Animation).on('finished', callback);
                EngineUtil.SetNodeText(this.wangzheStarNum, this.curStarNum.toString());
            }
        }else
        {
            if(this.fullStarNode[curStarNum])
            {
                let fullStar = this.fullStarNode[curStarNum];
                if(fullStar)
                {
                    fullStar.opacity = 255;
                    fullStar.active = true;
                    fullStar.getComponent(cc.Animation).play('starAnim');
                    fullStar.getComponent(cc.Animation).on('finished', callback);
                }
            }
        }
    }

    //保留星星逻辑
    private keepStarLogic()
    {
        if(AccountData.GetInstance().GetKeepStaroCount() < GameConfig.KeepStarCount)
        {
            //百分之50的几率保留星星
            if(Util.RandomPercentage(GameConfig.FXJLProbability))
            {
                GameUI.ShowUI(UIType.UT_FXJLView);
            }
        }
        return false;
    }

    //减星
    private reduceStar()
    {

        //播放声音
        AudioManager.PlaySoundEffect(SoundType.ST_ReduceStar);

        //当前星星数量
        let curStarNum = this.curStarNum;

        //当前段位
        let curGradingID = this.curGradingID;

        //减星
        if(curStarNum <= 0)
        {
            //降段位
            if(curGradingID >= 31)
            {
                this.wangzheLayer.getChildByName("FullStar2").active = false;
                this.wangzheLayer.getChildByName("FullStar1").active = false;
                this.whangzheStar = null;
            }

            if(curGradingID > 1)
            {
                this.curGradingID -= 1;
                this.curStarNum = 4;
                this.playDownLevel();
            }
        }else
        {
            this.curStarNum -= 1;

            if(curGradingID >= 31)
            {
                if(curStarNum > 1)
                {
                    this.wangzheLayer.getChildByName("FullStar1").active = true;
                    this.whangzheStar = this.wangzheLayer.getChildByName("FullStar2");
                    this.whangzheStar.active = true;
                }else if(curStarNum == 1)
                {
                    this.wangzheLayer.getChildByName("FullStar2").active = false;
                    this.whangzheStar = this.wangzheLayer.getChildByName("FullStar1");
                    this.whangzheStar.active = true;
                }

                if(this.whangzheStar)
                {
                    this.whangzheStar.getComponent(cc.Animation).play('jianStarAnim');
                    this.whangzheStar.getComponent(cc.Animation).on('finished', this.playStarFinish.bind(this));
                    EngineUtil.SetNodeText(this.wangzheStarNum, this.curStarNum.toString());
                }
            }else
            {
                if(this.fullStarNode[curStarNum - 1])
                {
                    let fullStar = this.fullStarNode[curStarNum - 1];
                    if(fullStar)
                    {
                        fullStar.getComponent(cc.Animation).play('jianStarAnim');
                        fullStar.getComponent(cc.Animation).on('finished', this.playStarFinish.bind(this));
                    }
                }
            }
        }
    }

    private  playStarFinish()
    {

        //当前段位
        let curGradingID = this.curGradingID;

        if(curGradingID >= 31)
        {

        }else
        {
            //如果当前星星数量大于等于5，则可以升级段位
            if(this.curStarNum >= 5)
            {
                //段位提升
                this.curGradingID += 1;
                //延迟0.3秒后播放升级段位的特效
                this.scheduleOnce(this.playUpLevel, 0.3);
                //段位升级当前星星数量变成0
                this.curStarNum = 0;
            }
        }
    }

    //段位进阶动画
    private playUpLevel()
    {
        if(this.fullStarLayer)
        {
            AudioManager.PlaySoundEffect(SoundType.ST_UpLevel);

            let self = this;

            let curGradingID = this.curGradingID;

            let anim = this.fullStarLayer.getComponent(cc.Animation);
            anim.play();
            anim.on('finished', function()
            {
                let upLevelNode = cc.instantiate(self.upLevelPerfab);
                if(upLevelNode)
                {
                    upLevelNode.zIndex = self.gradingIcon.zIndex;
                    self.popupLyaer.addChild(upLevelNode);
                    let upLevelAnim = upLevelNode.getComponent(cc.Animation);
                    upLevelAnim.play();
                    upLevelAnim.on('finished', function()
                    {
                        upLevelNode.destroy();
                    });
                }

                self.gradingIcon.runAction(cc.sequence(cc.scaleTo(0.3, 0, 0), cc.callFunc(function()
                {
                    EngineUtil.SetNodeTextureByType(AtlasType.AT_Common, self.gradingIcon, GradingSystem.GetGradingFileName(curGradingID));
                    EngineUtil.SetNodeText(self.gradingText, GradingSystem.GetGradingName(curGradingID));
                }), cc.scaleTo(0.3, 1, 1), cc.callFunc(self.upLevelFinish.bind(self))));
            });
        }
    }

    //段位降低的动画
    private playDownLevel()
    {
        let self = this;

        let curGradingID = this.curGradingID;

        let curStarNum = this.curStarNum;

        self.gradingIcon.runAction(cc.sequence(cc.scaleTo(0.3, 0, 0), cc.callFunc(function()
        {
            EngineUtil.SetNodeTextureByType(AtlasType.AT_Common, self.gradingIcon, GradingSystem.GetGradingFileName(curGradingID));
        }), cc.scaleTo(0.3, 1, 1), cc.callFunc(function()
        {
            if(curGradingID == 30)
            {
                self.fullStarLayer.active = true;
                self.emptyStarLayer.active = true;
                self.wangzheLayer.active = false;
            }
            if(self.fullStarNode && self.fullStarNode.length > 0)
            {
                for(let i = 0;i < 5;i ++)
                {
                    let fullStar = self.fullStarNode[i];
                    if(fullStar)
                    {
                        fullStar.active = i < curStarNum;
                    }
                }
            }
        })));
    }

    //段位提升动画播放完成
    private upLevelFinish()
    {
        //加到一半升级段位去了，升完之后回来继续加星
        if(this.tempStarNum && this.tempStarNum > 0)
        {
            if(this.curGradingID >= 31)
            {
                this.fullStarLayer.active = false;
                this.emptyStarLayer.active = false;
                this.wangzheLayer.active = true;
    
                if(this.curStarNum > 0)
                {
                    this.wangzheLayer.getChildByName("FullStar1").active = true;
                    this.whangzheStar = this.wangzheLayer.getChildByName("FullStar2");
                    this.whangzheStar.active = false;
                }else if(this.curStarNum == 0)
                {
                    this.wangzheLayer.getChildByName("FullStar2").active = false;
                    this.whangzheStar = this.wangzheLayer.getChildByName("FullStar1");
                    this.whangzheStar.active = false;
                }
            }
            this.continueAddStar();
        }else
        {
            this.tempStarNum = 0;
        }
    }

    private showOffClicked()
    {
        let self = this;

        AudioManager.PlayBtnSound();

        let finishFunc = function()
        {
            let curGoldNum = GameData.GetTempData().GetCurGoldNum();
            GameData.GetTempData().AddCurGoldNum(curGoldNum * 2);
            self.doubleGold(curGoldNum * 2);
            self.showOffBtn.getComponent(cc.Button).interactable = false;
            EngineUtil.CancelClickEvent(self.showOffBtn);   
        };

        AdvertiseMgr.CreateVideoAd(AdvertiseType.ADConfig_Video, function(isFinish)
        {
            if(isFinish)
            {
                finishFunc();
            }
        }, ShareSceneType.OverShare);
    }

    //加金币的动画
    private playAddGoldAnim(num)
    {
        let count = 0;
        let self = this;

        let curNum = parseInt(this.goldNum.getComponent(cc.Label).string);

        let func = function()
        {
            if(count >= num)
            {
                this.unschedule(func);
                return;
            }

            count ++;

            EngineUtil.SetNodeText(self.goldNum, (curNum + count).toString());
        };

        this.schedule(func, 1 / 60);
    }

    private doubleGold(num)
    {
        let self = this;

        this.playAddGoldAnim(num);

        Socket_SendMgr.GetCommonSend().SendGoldMessage(num, 0);

        // this.goldNum.runAction(cc.sequence(cc.scaleTo(0.5, 2, 2), cc.callFunc(function(){
        //     EngineUtil.SetNodeText(self.goldNum, GameData.GetTempData().GetCurGoldNum());
        // }), cc.scaleTo(0.5, 1, 1)));
    }

    private returnBtnClicked()
    {
        AudioManager.PlayBtnSound();

        AdvertiseMgr.DestroyBannerAd();

        EventCenter.DispatchEvent(MessageType.Return_HomePage);
    }

    private resetGameClicked()
    {
        AudioManager.PlayBtnSound();

        AdvertiseMgr.DestroyBannerAd();

        this.node.destroy();
        EventCenter.DispatchEvent(MessageType.Reset_Game);
    }

    private keepStar(isKeep: boolean)
    {
        if(isKeep === false)
        {
            this.scheduleOnce(this.reduceStar, 0.5);
        }else
        {
            //通知服务器保留星星
            Socket_SendMgr.GetCommonSend().SendKeepStarMessage();
        }
    }

    private getGradingDataResp(status: number): void
    {
        // //TODO
        // status = -1;

        this.scheduleOnce(()=>{
            if(status == 0)
            {
                //不加不减返回
            }else if(status > 0)
            {
                this.tempStarNum = status;
                //加星
                this.continueAddStar();
            }else if(status < 0)
            {
                //减星
                if(this.keepStarLogic() == false)
                {
                    this.reduceStar();
                }
            }
        }, 0.6);
    }
}