import ComponentBase from '../common/ComponentBase';
import EngineUtil, { AtlasType } from '../common/EngineUtil';
import Util from '../common/Util';
import AccountData from '../manager/AccountData';
import { MessageType } from '../common/EventCenter';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import GameConfig from '../GameConfig';
import { UIType } from '../UI/UIDefine';
import GameUI from '../manager/UIManager';
import OnlineRewardTableInfo from '../table/OnlineRewardTableInfo';
import GameTable from '../manager/TableManger';
import { TableType } from '../table/TableDefine';
import AdvertiseMgr from '../Advertise/AdvertiseMgr';
import { AdvertiseType } from '../Advertise/AdvertiseMgr';
import ConfigData from '../common/ConfigData';

const {ccclass, property} = cc._decorator;

//在线奖励
@ccclass
export default class OnLineReward extends ComponentBase
{
    private timeNode: cc.Node               = null;
    private btnNode: cc.Node                = null;

    //是否可以领取
    private state: boolean                  = null; 

    protected onLoad()
    {
        this.state          = false;

        this.btnNode        = this.node.getChildByName("Btn");
        this.timeNode       = this.node.getChildByName("Time");

        this.registerEvent(MessageType.Online_Reward_Time_Update, this.refreshTime.bind(this));
        this.registerEvent(MessageType.Online_Recv_Reward_Event, this.onReceiveRewardResp.bind(this));
        this.registerEvent(MessageType.Online_Speed_Up_Event, this.onToSpeedUpResp.bind(this));
        this.registerEvent(MessageType.Online_Can_Receive_Event, this.onCanReceiveResp.bind(this));

        this.registerClickEvent(this.btnNode, this.btnClicked.bind(this));
    }

    public start()
    {
        let time: number = AccountData.GetInstance().GetOnlineRewardTime();
        if(time > 0)
        {
            this.state = false;
        }
        else
        {
            this.state = true;
        }

        this.refreshView();
        this.refreshTime();
    }

    private btnClicked()
    {
        if(this.state)
        {
            //发送领奖消息
            Socket_SendMgr.GetActionSend().SendOnlineRecvRewardMessage();
        }
        else
        {
            if(ConfigData.Switch_AddSpeedAd)
            {
                //选中双倍奖励需要先看视频
                AdvertiseMgr.CreateVideoAd(AdvertiseType.ADConfig_Video, function(isFinish)
                {
                    if(isFinish)
                    {
                        //发送加速的消息
                        Socket_SendMgr.GetActionSend().SendOnlineSpeedUpMessage();
                    }
                });
            }
            else
            {
                //发送加速的消息
                Socket_SendMgr.GetActionSend().SendOnlineSpeedUpMessage();
            }
        }
    }

    private refreshTime()
    {
        let residueTime: number = AccountData.GetInstance().GetOnlineRewardTime();
        if(residueTime >= 0)
        {
            EngineUtil.SetNodeText(this.timeNode, Util.ConvertTimeString(residueTime));
        }
    }

    //收到加速的消息
    private onToSpeedUpResp()
    {
        this.refreshTime();
    }

    //收到领奖的消息
    private onReceiveRewardResp(rewardID: number)
    {
        this.state = false;

        let rewardTableInfo: OnlineRewardTableInfo = GameTable.GetInstance().GetDataInfo(TableType.TTD_OnlineRewardType, rewardID);
        if(rewardTableInfo)
        {
            let rewardList = rewardTableInfo.GetTaskRewardType();
            let rewardNum = rewardTableInfo.GetTaskRewardNum();

            if(rewardList && rewardList.length > 0 && rewardNum && rewardNum.length > 0)
            {
                for(let i = 0, len = rewardList.length; i < len; i ++)
                {
                    let reward = rewardList[i];
                    if(reward)
                    {
                        GameUI.AddViewToQuene(UIType.UT_RewardView, {
                            itemID: rewardList[i],
                            itemNum: rewardNum[i],
                        });
                    }
                }
                GameUI.ShowViewByQuene();
            }
            this.refreshView();
        }
    }

    //收到可以领奖的消息
    private onCanReceiveResp()
    {
        this.state = true;

        this.refreshView();
    }

    private refreshView()
    {
        if(this.state)
        {
            EngineUtil.SetNodeTextureByType(AtlasType.AT_StartScene, this.btnNode, "HomePage_RecvBtn");
        }
        else
        {
            EngineUtil.SetNodeTextureByType(AtlasType.AT_StartScene, this.btnNode, "HomePage_JSBtn");
        }
    }
}