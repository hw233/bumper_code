import PopUpBase from "../UI/PopUpBase";
import EngineUtil from "../common/EngineUtil";
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import EventCenter from '../common/EventCenter';
import { MessageType } from '../common/EventCenter';

const {ccclass, property} = cc._decorator;

@ccclass
export default class RetrunView extends PopUpBase
{
    @property({type: cc.Node})
    private confirmBtn: cc.Node = null;

    public OnInit() 
    {
        super.OnInit();

        EngineUtil.RegTouchEndEvent(this.confirmBtn, this.confirmBtnClicked.bind(this));
    }

    //确认按钮点击
    private confirmBtnClicked()
    {
        //返回主界面，通知服务器
        Socket_SendMgr.GetBettleSend().SendLeaveRoomMessage();
        Socket_SendMgr.GetActionSend().SendReturnMainView();

        EventCenter.DispatchEvent(MessageType.Return_HomePage);
    }
}
