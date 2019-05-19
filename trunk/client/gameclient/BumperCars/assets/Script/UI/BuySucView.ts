import PopUpBase from './PopUpBase';
import AudioManager from '../common/AudioManager';
const {ccclass, property} = cc._decorator;

@ccclass
export default class BuySucView extends PopUpBase
{
    @property(cc.Node)
    private showOffBtn: cc.Node = null;

    public OnInit () 
    {
        super.OnInit();

        this.registerClickEvent(this.showOffBtn, this.showOffBtnClicked.bind(this));
    }

    //炫耀按钮点击
    private showOffBtnClicked()
    {
        AudioManager.PlayBtnSound();
    }
}
