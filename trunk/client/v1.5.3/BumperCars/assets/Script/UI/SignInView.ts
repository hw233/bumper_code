import AdvertiseMgr from '../Advertise/AdvertiseMgr';
import BoxData from '../SignIn/BoxData';
import PopUpBase from './PopUpBase';
import SignInBox from '../component/SignInBox';
import SignInItem from '../component/SignInItem';
import SignInManager from '../SignIn/SignInManager';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import { AdvertiseType } from '../Advertise/AdvertiseMgr';
import { SignInDayData } from '../SignIn/SignInDayData';
import { MessageType } from '../common/EventCenter';
import EngineUtil from '../common/EngineUtil';
import AccountData from '../manager/AccountData';
import { SignInState } from '../SignIn/SignInSystem';

const {ccclass, property} = cc._decorator;

@ccclass
export default class SignInView extends PopUpBase 
{
    @property(cc.Node)
    private signInList: cc.Node         = null

    @property(cc.Node)
    private boxList: cc.Node            = null

    @property(cc.Node)
    private receiveBtn: cc.Node         = null

    @property(cc.Toggle)
    private isDoubleNode: cc.Toggle     = null

    @property(cc.Prefab)
    private signInPrefab: cc.Prefab     = null

    @property(cc.Prefab)
    private boxPrefab: cc.Prefab        = null

    public OnInit()
    {
        super.OnInit();

        this.registerEvent(MessageType.SignIn_Refresh_SignIn_View, this.refreshView.bind(this));

        this.registerClickEvent(this.receiveBtn, this.receiveBtnClicked.bind(this));
    }

    public InitView()
    {
        let dayNum: number = SignInManager.GetInstance().GetSignInDayNum();

        for(let i = 0; i < dayNum; i ++)
        {
            let signInNode: cc.Node = cc.instantiate(this.signInPrefab);
            if(signInNode)
            {
                let signInData: SignInDayData = SignInManager.GetInstance().GetSiginDataByIndex(i);
                if(signInData)
                {
                    this.signInList.addChild(signInNode);
                    let signInItem: SignInItem = signInNode.getComponent("SignInItem");
                    if(signInItem)
                    {
                        signInItem.initView(signInData);
                    }
                }
            }
        }

        let boxNum: number = SignInManager.GetInstance().GetBoxNum();
        for(let i = 0; i < boxNum; i ++)
        {
            let boxNode: cc.Node = cc.instantiate(this.boxPrefab);
            if(boxNode)
            {

                let boxData: BoxData = SignInManager.GetInstance().GetBoxDataByIndex(i);
                if(boxData)
                {
                    this.boxList.addChild(boxNode);

                    let boxItem: SignInBox = boxNode.getComponent("SignInBox");
                    if(boxItem)
                    {
                        boxItem.initView(boxData.GetBoxID());
                    }
                }
            }
        }
    }

    public RefreshView()
    {
        super.RefreshView();

        this.refreshView();
    }

    //签到领奖按钮点击
    private receiveBtnClicked()
    {
        //是否选择双倍奖励
        let isSelectedDouble: boolean = this.isDoubleNode.isChecked;

        if(isSelectedDouble)
        {
            //选中双倍奖励需要先看视频
            AdvertiseMgr.CreateVideoAd(AdvertiseType.ADConfig_Video, function(isFinish)
            {
                if(isFinish)
                {
                    Socket_SendMgr.GetSignInSend().SendSignInMessage(1);
                }
            });
        }
        else
        {
            Socket_SendMgr.GetSignInSend().SendSignInMessage(0);
        }
    }

    private refreshView()
    {
        let curDayIndex: number = AccountData.GetInstance().GetDayIndex();

        let signInData: SignInDayData = SignInManager.GetInstance().GetSignInDataByID(curDayIndex);
        if(signInData)
        {
            if(signInData.GetState() === SignInState.SIS_AlreadySignIn)
            {
                EngineUtil.SetButtonInteractable(this.receiveBtn, false);
            }
        }
    }
}
