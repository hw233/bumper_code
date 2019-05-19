import PopUpBase from "../UI/PopUpBase";
import GameConfig from "../GameConfig";
import {MessageType} from "../common/EventCenter";
import Socket_SendMgr from "../network/socket/Socket_SendManager";
import EngineUtil from "../common/EngineUtil";
import GameUI from '../manager/UIManager';

const {ccclass, property} = cc._decorator;

@ccclass
export default class InMatchView extends PopUpBase
{
    @property({type: cc.Node})
    private matchTimeNode: cc.Node = null;

    private matchTime: number = null

    public OnInit () 
    {
        super.OnInit();

        this.matchTime = GameConfig.QuickMatchTime;

        this.registerEvent(MessageType.DLD_Receive_Cancel_Quick_Join, this.cancelQuickJoinResp.bind(this));
    }

    public OnEnter()
    {
        super.OnEnter();

        this.schedule(this.refreshTime, 1);
    }

    public OnExit()
    {
        super.OnExit();

        this.unschedule(this.refreshTime);
    }

    private refreshTime()
    {
        this.matchTime --;

        if(this.matchTime <= 0)
        {
            this.matchTime = GameConfig.QuickMatchTime;
        }

        EngineUtil.SetNodeText(this.matchTimeNode, this.matchTime.toString() );
    }

    protected closeBtnClicked()
    {
        super.closeBtnClicked();

        Socket_SendMgr.GetBettleSend().SendCancelQuickJoinMessage();
    }

    //收到取消快速匹配的消息
    private cancelQuickJoinResp()
    {
        GameUI.HideUI(this.GetType());
    }
}