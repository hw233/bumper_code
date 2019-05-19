import PopUpBase from "../UI/PopUpBase";
import {AdvertiseType} from "../Advertise/AdvertiseMgr";
import AdvertiseMgr from "../Advertise/AdvertiseMgr";
import {MessageType} from "../common/EventCenter";
import AudioManager from "../common/AudioManager";
import Socket_SendMgr from "../network/socket/Socket_SendManager";
import AccountData from '../manager/AccountData';


///购买类型
export enum BUY_TYPE{
	_VAS_BUY_TYPE_GOLD	= 1,			//金币买道具
	_VAS_BUY_TYPE_VIWE	= 2,			//视频买道具
};

const {ccclass, property} = cc._decorator;

@ccclass
export default class VasLayer extends PopUpBase
{
    @property({type: cc.Node})
    private scoreBuffBtn: cc.Node = null;

    @property({type: cc.Node})
    private goldBuffBtn: cc.Node = null;

    @property({type: cc.Node})
    private scoreMask: cc.Node = null;

    @property({type: cc.Node})
    private goldMask: cc.Node = null;

    @property(cc.Node)
    private scoreNode: cc.Node = null;

    @property(cc.Node)
    private goldNode: cc.Node = null;

    public OnInit () 
    {
        super.OnInit();

        this.scoreMask.active = false;
        this.goldMask.active = false;

        this.registerTouchEndEvent(this.scoreNode, this.goldBtnClicked.bind(this));
        this.registerTouchEndEvent(this.goldNode, this.watchBtnClicked.bind(this));
        this.registerTouchEndEvent(this.scoreBuffBtn, this.goldBtnClicked.bind(this));
        this.registerTouchEndEvent(this.goldBuffBtn, this.watchBtnClicked.bind(this));
        
        //接受到购买buff的消息
        this.registerEvent(MessageType.DLD_Receive_Buy_Buff, this.recvBuyBuffResp.bind(this) );
    }

    public RefreshView()
    {
        super.RefreshView();

        if(AccountData.GetInstance().GetAddScore() > 0)
        {
            this.showScoreBuffMask();
        }

        if(AccountData.GetInstance().GetAddGold() > 0)
        {
            this.showGoldBuffMask();
        }
    }

    //金币按钮点击
    private goldBtnClicked()
    {
        AudioManager.PlayBtnSound();

        Socket_SendMgr.GetVasSend().SendBuyBuffMessage(BUY_TYPE._VAS_BUY_TYPE_GOLD);
    }

    //观看按钮点击
    private watchBtnClicked()
    {
        AudioManager.PlayBtnSound();
        
        AdvertiseMgr.CreateVideoAd(AdvertiseType.ADConfig_Video, function(isFinish)
        {
            if(isFinish)
            {
                Socket_SendMgr.GetVasSend().SendBuyBuffMessage(BUY_TYPE._VAS_BUY_TYPE_VIWE);
            }
        });
    }

    private showScoreBuffMask()
    {
        this.scoreMask.active = true;
    }

    private showGoldBuffMask()
    {
        this.goldMask.active = true;
    }

    //接受到购买buff的消息
    private recvBuyBuffResp(type)
    {
        if(type == BUY_TYPE._VAS_BUY_TYPE_GOLD)
        {
            this.showScoreBuffMask();
        }else if(type == BUY_TYPE._VAS_BUY_TYPE_VIWE)
        {
            this.showGoldBuffMask();
        }
    }
}
