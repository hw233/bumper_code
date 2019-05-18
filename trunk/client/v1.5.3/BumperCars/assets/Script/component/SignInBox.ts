import EngineUtil, { AtlasType } from '../common/EngineUtil';
import GameLog from '../Log/LogTool';
import BoxData from '../SignIn/BoxData';
import { BoxState } from '../SignIn/SignInSystem';
import ComponentBase from '../common/ComponentBase';
import { MessageType } from '../common/EventCenter';
import SignInManager from '../SignIn/SignInManager';
import Socket_SendMgr from '../network/socket/Socket_SendManager';

const {ccclass, property} = cc._decorator;

@ccclass
export default class SignInBox extends ComponentBase
{
    @property(cc.Node)
    private stateImg: cc.Node = null;

    @property(cc.Node)
    private boxImg: cc.Node = null;

    private isCanClick: boolean = null;

    private boxID: number = null;

    protected onLoad()
    {
        this.boxID = 0;
        this.isCanClick = false;

        this.registerEvent(MessageType.SignIn_Refresh_Box_Item, this.refreshView.bind(this));

        EngineUtil.RegTouchEndEvent(this.boxImg, this.boxClicked.bind(this));
    }

    public initView(boxID: number)
    {
        this.boxID = boxID;

        this.refreshView();
    }

    private refreshView()
    {
        let boxData: BoxData = SignInManager.GetInstance().GetBoxDataByID(this.boxID);
        if(boxData)
        {
            let state: number = boxData.GetBoxState();
            let id: number = boxData.GetBoxID();

            if(state <= BoxState.BS_None || state > BoxState.BS_AlreadyReceive)
            {
                GameLog.PrintLog("SignInBox: initView: state = ", state);
                return;
            }

            EngineUtil.SetNodeTextureByType(AtlasType.AT_TaskLayer, this.stateImg, "SignIn_State_" + boxData.GetBoxState());

            let boxImgFile: string = "SignIn_Box_" + id + "_1";
            if(state === BoxState.BS_AlreadyReceive)
            {
                boxImgFile = "SignIn_Box_Opend";
            }
                
            EngineUtil.SetNodeTextureByType(AtlasType.AT_TaskLayer, this.boxImg, boxImgFile);
        }

        if(boxData.IsCanReceive())
        {
            this.shakBox();
            this.isCanClick = true;
        }
        else
        {
            this.stopShakBox();
            this.isCanClick = false;
        }

    }

    //可领取 晃动宝箱
    private shakBox()
    {
        if(this.boxImg)
        {
            let anim: cc.Animation = this.boxImg.getComponent(cc.Animation);
            if(anim)
            {
                anim.play();
            }
        }
    }

    //停止宝箱晃动
    private stopShakBox()
    {
        if(this.boxImg)
        {
            let anim: cc.Animation = this.boxImg.getComponent(cc.Animation);
            if(anim)
            {
                anim.stop();
            }
        }
    }

    //点击宝箱
    private boxClicked()
    {
        if(this.isCanClick === false)
        {
            return;
        }

        //发送领取宝箱
        Socket_SendMgr.GetActionSend().SendSignInBoxReceive(this.boxID);
    }
}
