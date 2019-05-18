import EngineUtil from '../common/EngineUtil';
import { SignInDayData } from '../SignIn/SignInDayData';
import GameLog from '../Log/LogTool';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import ComponentBase from '../common/ComponentBase';
import { MessageType } from '../common/EventCenter';
import SignInManager from '../SignIn/SignInManager';
import AdvertiseMgr from '../Advertise/AdvertiseMgr';
import { AdvertiseType } from '../Advertise/AdvertiseMgr';
import { SignInState } from '../SignIn/SignInSystem';
import { AtlasType } from '../common/EngineUtil';

const {ccclass, property} = cc._decorator;

@ccclass
export default class SignInItem extends ComponentBase
{
    @property(cc.Node)
    private dayNode:cc.Node = null

    @property(cc.Node)
    private flagNode: cc.Node = null

    @property(cc.Node)
    private rewardNum: cc.Node = null;

    private dayID: number = null;

    protected onLoad()
    {
        this.registerEvent(MessageType.SignIn_Refresh_SignIn_Item, this.refreshView.bind(this));
    }

    public initView(signInData: SignInDayData)
    {
        if(signInData)
        {
            let dayNumber: number = signInData.GetDayNumber();
            let dayID: number = signInData.GetDayID();
            let isCurDay: boolean = signInData.IsCurDay();
            let state: number = signInData.GetState();
            let rewardNum: number = signInData.GetRewardNum();

            this.dayID = dayID;
    
            EngineUtil.SetNodeText(this.dayNode, dayNumber.toString());
            EngineUtil.SetNodeText(this.rewardNum, rewardNum.toString());

            EngineUtil.SetNodeTextureByType(AtlasType.AT_TaskLayer, this.node, "SignIn_Frame_" + Number(isCurDay));

            //可以补签的话为当前item添加点击事件
            if(signInData.IsCanRetroactive())
            {
                EngineUtil.RegBtnTouchBeginEvent(this.node, this.itemClicked.bind(this));
            }

            this.refreshView();
        }
    }

    private refreshView()
    {
        let signInData: SignInDayData = SignInManager.GetInstance().GetSignInDataByID(this.dayID);
        if(signInData)
        {
            this.flagNode.active = false;

            let state: number = signInData.GetState();

            if(state === SignInState.SIS_NotSignIn || state === SignInState.SIS_AlreadySignIn)
            {
                this.flagNode.active = true;

                EngineUtil.SetNodeTextureByType(AtlasType.AT_TaskLayer, this.flagNode, "SignIn_Flag_" + state);
            }
        }
    }

    private itemClicked()
    {
        //发送补签消息
        GameLog.PrintDebug("补签");

        //选中双倍奖励需要先看视频
        AdvertiseMgr.CreateVideoAd(AdvertiseType.ADConfig_Video, (isFinish)=>
        {
            if(isFinish)
            {
                Socket_SendMgr.GetSignInSend().SendRetroactiveMessage(this.dayID);
            }
        });
    }
    // update (dt) {}
}
