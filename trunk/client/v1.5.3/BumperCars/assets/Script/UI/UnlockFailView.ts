import PopUpBase from './PopUpBase';
import AudioManager from '../common/AudioManager';
import EngineUtil from '../common/EngineUtil';
import CarTableInfo from '../table/CarTableInfo';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import ItemSystem from '../Item/ItemSystem';
import GameUI from '../manager/UIManager';
import { UIType } from './UIDefine';
import { AtlasType } from '../common/EngineUtil';
import AdvertiseMgr from '../Advertise/AdvertiseMgr';
import { AdvertiseType } from '../Advertise/AdvertiseMgr';
const {ccclass, property} = cc._decorator;

@ccclass
export default class UnlockFailView extends PopUpBase
{
    @property(cc.Node)
    private trialBtn: cc.Node = null;

    @property(cc.Node)
    private titleNode: cc.Node = null;

    private itemID: number        = null;

    public OnInit () 
    {
        super.OnInit();

        this.registerClickEvent(this.trialBtn, this.trialBtnClicked.bind(this));
    }

    public RefreshView()
    {
        this.itemID = Number(this.param);

        let carInfo: CarTableInfo = ItemSystem.GetInstance().GetTableInfoByItemID(this.itemID);
        if(carInfo)
        {
            EngineUtil.SetNodeTextureByType(AtlasType.AT_CarLibLayer, this.titleNode, "PopUp_UnlockFailTitle_" + carInfo.GetConditionID());
        }
    }

    //免费试用按钮点击
    private trialBtnClicked()
    {
        let self = this;

        AudioManager.PlayBtnSound();

        AdvertiseMgr.CreateVideoAd(AdvertiseType.ADConfig_Video, (isFinish)=>
        {
            if(isFinish)
            {
                GameUI.HideUI(UIType.UT_UnlockFailView);
                Socket_SendMgr.GetVasSend().SendBuyItemMessage(self.itemID, 1);
            }
        });
    }
}
