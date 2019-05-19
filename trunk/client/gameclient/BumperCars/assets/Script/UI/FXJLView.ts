import {AdvertiseType} from "../Advertise/AdvertiseMgr";
import AdvertiseMgr from "../Advertise/AdvertiseMgr";
import {ShareSceneType} from "../common/ShareSceneType";
import EventCenter from "../common/EventCenter";
import {MessageType} from "../common/EventCenter";
import GameLocalDataMgr from '../manager/GameLocalDataMgr';
import AccountData from '../manager/AccountData';
import PopUpBase from './PopUpBase';
import GameUI from '../manager/UIManager';

const {ccclass, property} = cc._decorator;

@ccclass
export default class FXJLView extends PopUpBase
{
    @property({type: cc.Node})
    private watchBtn: cc.Node = null;

    @property({type: cc.Node})
    private watchTip: cc.Node = null;

    public OnInit () 
    {
        super.OnInit();

        this.registerClickEvent(this.watchBtn, this.watchBtnClicked.bind(this));

        if(CC_WECHATGAME)
        {
            if(GameLocalDataMgr.GetInstance().GetWatchVideoCount() < 5)
            {
            }
            else
            {
                this.watchTip.active = false;
            }
        }
    }

    public OnExit()
    {
        super.OnExit();

        EventCenter.DispatchEvent(MessageType.Keep_Star, false);
    }

    private watchBtnClicked()
    {
        let self = this;

        AdvertiseMgr.CreateVideoAd(AdvertiseType.ADConfig_Video_Keep_Star, function(isFinish)
        {
            if(isFinish)
            {
                AccountData.GetInstance().SetKeepStarCount(AccountData.GetInstance().GetKeepStaroCount() + 1);
                EventCenter.DispatchEvent(MessageType.Keep_Star, true);
            }
            else
            {
                EventCenter.DispatchEvent(MessageType.Keep_Star, false);
            }
            GameUI.HideUI(self.GetType());
        }, ShareSceneType.OverShare);
    }
}