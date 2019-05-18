import AdvertiseMgr from '../Advertise/AdvertiseMgr';
import AudioManager from '../common/AudioManager';
import DLDSettleData from '../data/net/DLDSettleData';
import EngineUtil from '../common/EngineUtil';
import EventCenter from '../common/EventCenter';
import GameData from '../data/GameData';
import GameLocalDataMgr from '../manager/GameLocalDataMgr';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import { AdvertiseType } from '../Advertise/AdvertiseMgr';
import { MessageType } from '../common/EventCenter';
import { PlayerSettleData } from '../data/net/DLDSettleData';
import UIBase from './UIBase';
import OverRankItem from '../layer/DLDOverRankItem';

const {ccclass, property} = cc._decorator;

@ccclass
export default class DLDOverLayer extends UIBase
{
    @property({type: cc.Node})
    private goldNumNode: cc.Node = null;

    @property({type: cc.Node})
    private bzlpNumNode: cc.Node = null;

    @property({type: cc.Node})
    private rewardBtn: cc.Node = null;

    @property({type: cc.Node})
    private returnBtn: cc.Node = null;

    @property({type: cc.Node})
    private ranklistContent: cc.Node = null;

    @property({type: cc.Prefab})
    private rankItemPrefab: cc.Prefab = null;

    public OnInit () 
    {
        super.OnInit();

        this.registerClickEvent(this.rewardBtn, this.rewardBtnClicked.bind(this));
        this.registerClickEvent(this.returnBtn, this.returnBtnClicked.bind(this));

        //3倍积分
        this.registerEvent(MessageType.DLD_Receive_Three_Reward, this.threeTimesRewardResp.bind(this));

        GameLocalDataMgr.GetInstance().ReadPlayGameCount();

        GameLocalDataMgr.GetInstance().SetPlayGameCount(GameLocalDataMgr.GetInstance().GetPlayGameCount() + 1);
    }

    public InitView()
    {
        super.InitView();

        let self = this;

        let settleData:DLDSettleData = GameData.GetDLDSettleData();
        if(settleData){
            let allPlayerData: Array<PlayerSettleData> = settleData.GetAllPlayerData();
            if(allPlayerData){
                for(let i: number = 0;i < allPlayerData.length;i ++){
                    let playerInfo: PlayerSettleData = allPlayerData[i];
                    let item: cc.Node = cc.instantiate(self.rankItemPrefab);
                    let rankItem: OverRankItem = item.getComponent('DLDOverRankItem');
                    if(rankItem){
                        rankItem.initView(i + 1, playerInfo);
                        self.ranklistContent.addChild(item);
                    }
                }
            }

            EngineUtil.SetNodeText(this.goldNumNode, settleData.GetGoldNum().toString());
            EngineUtil.SetNodeText(this.bzlpNumNode, settleData.GetBZLPNum().toString());
        }
    }

    //三倍奖励点击
    private rewardBtnClicked()
    {
        AdvertiseMgr.CreateVideoAd(AdvertiseType.ADConfig_Video, function(isFinish)
        {
            if(isFinish)
            {
                Socket_SendMgr.GetBettleSend().SendThreeTimesRewardMessage();
            }
        });
    }

    //返回按钮点击
    private returnBtnClicked()
    {
        AudioManager.PlayBtnSound();

        //返回主界面，通知服务器
        Socket_SendMgr.GetActionSend().SendReturnMainView();

        EventCenter.DispatchEvent(MessageType.Return_HomePage);
    }

    //加金币的动画
    private playAddGoldAnim(goldNum: number, bzlpNum: number)
    {
        EngineUtil.SetNodeText(this.goldNumNode, goldNum.toString());
        EngineUtil.SetNodeText(this.bzlpNumNode, bzlpNum.toString());
    }

    private threeTimesRewardResp()
    {
        let settleData = GameData.GetDLDSettleData();
        if(settleData)
        {
            EngineUtil.SetButtonInteractable(this.rewardBtn, false);
            EngineUtil.CancelClickEvent(this.rewardBtn);   
    
            this.playAddGoldAnim(settleData.GetGoldNum(), settleData.GetBZLPNum());
        }
    }
}
