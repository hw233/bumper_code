import AudioManager from '../common/AudioManager';
import GameUI from '../manager/UIManager';
import PopUpBase from '../UI/PopUpBase';
import SocketNetManager from '../network/socket/SocketNetManager';

const {ccclass, property} = cc._decorator;

@ccclass
export default class LoadingFail extends PopUpBase
{
    @property({type: cc.Node})
    private retryBtn: cc.Node = null;

    public OnInit() 
    {
        super.OnInit();

        this.registerTouchEndEvent(this.retryBtn, this.retryBtnClicked.bind(this));
    }

    //重试按钮点击
    private retryBtnClicked()
    {
        AudioManager.PlayBtnSound();

        GameUI.HideLoadingFailView();

        //连接服务器
        SocketNetManager.GetInstance().Reconnect();
    }
}