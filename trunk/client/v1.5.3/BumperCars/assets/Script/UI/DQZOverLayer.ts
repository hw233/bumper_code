import AdvertiseMgr from '../Advertise/AdvertiseMgr';
import AudioManager from '../common/AudioManager';
import DQZSettleData from '../data/net/DQZSettleData';
import EngineUtil, { AtlasType } from '../common/EngineUtil';
import EventCenter from '../common/EventCenter';
import GameConfig from '../GameConfig';
import GameData from '../data/GameData';
import GameLocalDataMgr from '../manager/GameLocalDataMgr';
import Http_SendMgr from '../network/http/Http_SendManager';
import PlayerMgr from '../manager/PlayerMgr';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import { AdvertiseType } from '../Advertise/AdvertiseMgr';
import { GameResult, TeamType } from '../GameDefine';
import { MessageType } from '../common/EventCenter';
import { ShareSceneType } from '../common/ShareSceneType';
import { SoundType } from '../common/AudioManager';
import UIBase from './UIBase';

const {ccclass, property} = cc._decorator;

@ccclass
export default class DQZOverLayer extends UIBase
{
    @property({type: cc.Node})
    private rewardBtn: cc.Node = null;

    @property({type: cc.Node})
    private retrunBtn: cc.Node = null;

    @property({type: cc.Node})
    private moreGameLayout: cc.Node = null;

    @property({type: cc.Prefab})
    private cblPrefab: cc.Prefab = null;

    @property({type: cc.Node})
    private redScoreNode: cc.Node = null;

    @property({type: cc.Node})
    private blueScoreNode: cc.Node = null;

    @property({type: cc.Node})
    private goldNumNode: cc.Node = null;

    @property({type: cc.Node})
    private titleNode: cc.Node = null;

    public OnInit () 
    {
        super.OnInit();

        this.registerClickEvent(this.retrunBtn, this.returnBtnClicked.bind(this));
        this.registerClickEvent(this.rewardBtn, this.rewardBtnClicked.bind(this));
        
        //3倍积分
        this.registerEvent(MessageType.DLD_Receive_Three_Reward, this.threeTimesRewardResp.bind(this));

        GameLocalDataMgr.GetInstance().ReadPlayGameCount();

        GameLocalDataMgr.GetInstance().SetPlayGameCount(GameLocalDataMgr.GetInstance().GetPlayGameCount() + 1);

    }

    public OnEnter() 
    {
        super.OnEnter();

        AdvertiseMgr.CreateBannerAd(AdvertiseType.ADConfig_Banner);
    }
    
    public InitView()
    {
        super.InitView();

        this.initMoreGameUI();

        let settleData = GameData.GetDQZSettleData();
        if(settleData)
        {
            EngineUtil.SetNodeText(this.redScoreNode, settleData.getRedScore().toString());
            EngineUtil.SetNodeText(this.blueScoreNode, settleData.getBlueScore().toString());
            EngineUtil.SetNodeText(this.goldNumNode, this.getSelfReward().toString());

            let gameResult = this.getSelfBettleResult();
            EngineUtil.SetNodeTextureByType(AtlasType.AT_GameOver, this.titleNode, "GameResult_" + gameResult);

                this.playResultEffect(gameResult);
        }
    }

    private initMoreGameUI()
    {
        let self = this;

        Http_SendMgr.SendGetGameListMessage(GameConfig.GameListID, function(data)
        {
            if(data)
            {
                let gameList = data.redirect;
                for (let i = 0; i < gameList.length, i < 4; i++) 
                {
                    let item = cc.instantiate(self.cblPrefab);
                    let script = item.getComponent("PanelItem");
                    item.setPosition(0, 0);
                    if (script) 
                    {
                        let directInfo = 
                        {
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

    //加金币的动画
    private playAddGoldAnim(num: number)
    {
        // let count = 0;
        // let self = this;

        // let curNum = parseInt(this.goldNum.getComponent(cc.Label).string);

        // let func = function(){
        //     if(count >= num){
        //         this.unschedule(func);
        //         return;
        //     }

        //     count ++;

        //     EngineUtil.SetNodeText(self.goldNum, curNum + count);
        // };

        // this.schedule(func, 1 / 60);

        EngineUtil.SetNodeText(this.goldNumNode, num.toString());
    }

    //获得自己的奖励
    private getSelfReward(): number
    {
        let settleData = GameData.GetDQZSettleData();
       if(!settleData)return;

        let selfTeamType = PlayerMgr.GetSelfTeamType();
        if(selfTeamType == TeamType.TT_Red)
        {
            return settleData.getRedGoldNum();
        }else if(selfTeamType == TeamType.TT_Blue)
        {
            return settleData.getBlueGoldNum();
        }

        return 0;
    }

    //获得自己的战斗结果
    private getSelfBettleResult()
    {
       let settleData = GameData.GetDQZSettleData();
       if(!settleData)return;

        let selfTeamType = PlayerMgr.GetSelfTeamType();
        if(selfTeamType == TeamType.TT_Red)
        {
            return settleData.getRedResult();
        }else if(selfTeamType == TeamType.TT_Blue)
        {
            return settleData.getBlueResult();
        }
    }

    //根据战斗结果播放音效
    private playResultEffect(result: number)
    {
        if(result == GameResult.GR_Victory)
        {
            AudioManager.PlaySoundEffect(SoundType.ST_Victory);
        }
        else if(result == GameResult.GR_Dogfall)
        {
            AudioManager.PlaySoundEffect(SoundType.ST_Dogfall);
        }
        else if(result == GameResult.GR_Defeat)
        {
            AudioManager.PlaySoundEffect(SoundType.ST_Defeat);
        }
    }

    private returnBtnClicked()
    {
        AudioManager.PlayBtnSound();

        AdvertiseMgr.DestroyBannerAd();

        //返回主界面，通知服务器
        Socket_SendMgr.GetActionSend().SendReturnMainView();

        EventCenter.DispatchEvent(MessageType.Return_HomePage);
    }

    private rewardBtnClicked()
    {
        AudioManager.PlayBtnSound();

        AdvertiseMgr.CreateVideoAd(AdvertiseType.ADConfig_Video, function(isFinish){
            if(isFinish)
            {
                Socket_SendMgr.GetBettleSend().Send5v5ThreeTimesRewardMessage();
            }
        }, ShareSceneType.OverShare);
    }

    private threeTimesRewardResp()
    {
        var settleData: DQZSettleData = GameData.GetDQZSettleData();

        EngineUtil.SetButtonInteractable(this.rewardBtn, false);
        this.rewardBtn.off('click', this.rewardBtnClicked.bind(this));

        this.playAddGoldAnim(settleData.getDoubleGold());
    }
}
