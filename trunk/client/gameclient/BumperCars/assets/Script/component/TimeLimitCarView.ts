import ComponentBase from '../common/ComponentBase';
import { MessageType } from '../common/EventCenter';
import AccountData from '../manager/AccountData';
import EngineUtil from '../common/EngineUtil';
import Util from '../common/Util';
import CarTableInfo from '../table/CarTableInfo';
import { AtlasType } from '../common/EngineUtil';
import AdvertiseMgr from '../Advertise/AdvertiseMgr';
import { AdvertiseType } from '../Advertise/AdvertiseMgr';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import ItemSystem from '../Item/ItemSystem';
import ConfigData from '../common/ConfigData';

const {ccclass, property} = cc._decorator;

//限时免费车辆
@ccclass
export default class TimeLimitCarView extends ComponentBase
{

    private btnNode: cc.Node                = null;
    private carIconNode: cc.Node            = null;
    private carNameNode: cc.Node            = null;

    private eventNode: cc.Node              = null;

    private timeNode: cc.Node               = null;

    private isHas: boolean                  = null;

    protected onLoad()
    {
        this.carIconNode    = this.node.getChildByName("CarIcon");
        this.btnNode        = this.node.getChildByName("Btn");
        this.carNameNode    = this.node.getChildByName("CarNameImg");
        this.eventNode      = this.node.getChildByName("EventNode");
        this.timeNode       = this.node.getChildByName("Time");

        this.isHas = false;

        this.registerEvent(MessageType.Online_Reward_Time_Update, this.refreshTime.bind(this));
        this.registerEvent(MessageType.StartScene_RefreshTimeLimit, this.refreshView.bind(this));

        this.registerClickEvent(this.btnNode, this.btnClicked.bind(this));
        this.registerTouchEndEvent(this.eventNode, this.btnClicked.bind(this));
    }

    protected start()
    {
        this.refreshView();
    }

    public refreshView()
    {
        let timeLimitItemID: number = AccountData.GetInstance().GetTimeLimitItemID();
        //刷新展示的限时免费的车辆
        let itemInfo: CarTableInfo = ItemSystem.GetInstance().GetTableInfoByItemID(timeLimitItemID);
        if(itemInfo)
        {
            this.isHas = true;

            let imgID: number = itemInfo.GetImgID();
            
            EngineUtil.SetNodeTextureByType(AtlasType.AT_StartScene, this.carIconNode, "Car_" + imgID);
            EngineUtil.SetNodeTextureByType(AtlasType.AT_StartScene, this.carNameNode, "CarNameImg_" + imgID);
        }
        else
        {
            this.isHas = false;
            this.node.active = false;
        }
    }

    private refreshTime()
    {
        if(this.isHas)
        {
            let residueTime: number = AccountData.GetInstance().GetTimeLimitRemainTime();
            if(residueTime >= 0)
            {
                EngineUtil.SetNodeText(this.timeNode, Util.ConvertTimeString(residueTime));
            }
        }
    }

    private btnClicked()
    {
        if(this.isHas)
        {
            if(ConfigData.Switch_TimeLimitAd)
            {
                AdvertiseMgr.CreateVideoAd(AdvertiseType.ADConfig_Video_JiaYou, function(isFinish){
                    if(isFinish)
                    {
                        Socket_SendMgr.GetActionSend().SendTimeLimitRewardMessage();
                    }
                });
            }
            else
            {
                Socket_SendMgr.GetActionSend().SendTimeLimitRewardMessage();
            }
        }
    }
}