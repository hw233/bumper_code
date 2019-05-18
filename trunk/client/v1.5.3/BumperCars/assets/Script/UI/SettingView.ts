import PopUpBase from "./PopUpBase";
import GlobalVar from "../GlobalVar";
import AudioManager from "../common/AudioManager";
import WXHelper from '../common/WXHelper';
import { MessageType } from '../common/EventCenter';

const {ccclass, property} = cc._decorator;

@ccclass
export default class SettingView extends PopUpBase
{
    @property({type: cc.Node})
    private vibrationNode: cc.Node = null;

    @property({type: cc.Node})
    private effectNode: cc.Node = null;

    @property({type: cc.Node})
    private vibrationBtn: cc.Node = null;

    @property({type: cc.Node})
    private effectBtn: cc.Node = null;

    private vibrationBtnState: number = null;
    private effectBtnState: number = null;

    public OnInit () 
    {
        super.OnInit();

        this.vibrationBtnState = (GlobalVar.OpenVibration === true) ? 1 : 0;
        this.effectBtnState = (AudioManager.IsOpenEffect === true) ? 1 : 0;

        this.registerTouchEndEvent(this.vibrationBtn, this.vibrationBtnClicked.bind(this));
        this.registerTouchEndEvent(this.effectBtn, this.effectBtnClicked.bind(this));

        this.refreshVibrationBtnState();
        this.refreshEffectBtnState();
    }

    public OnEnter()
    {
        super.OnEnter();

        this.dispatchEvent(MessageType.GameLoopBtn_Hide_Event);
    }

    public OnExit()
    {
        super.OnExit();

        this.dispatchEvent(MessageType.GameLoopBtn_Show_Event);
    }

    //振动开关按钮点击
    private vibrationBtnClicked()
    {
        AudioManager.PlayBtnSound();

        if(this.vibrationBtnState == 0)
        {
            this.vibrationBtnState = 1;
        }else
        {
            this.vibrationBtnState = 0;
        }

        this.refreshVibrationBtnState();
    }

    //音效开关按钮点击
    private effectBtnClicked()
    {
        AudioManager.PlayBtnSound();

        if(this.effectBtnState == 0)
        {
            this.effectBtnState = 1;
        }else
        {
            this.effectBtnState = 0;
        }

        this.refreshEffectBtnState();
    }

    private refreshVibrationBtnState()
    {
        if(this.vibrationBtnState == 0)
        {
            GlobalVar.OpenVibration = false;
            this.playCloseAnim(this.vibrationNode);
        }else
        {
            GlobalVar.OpenVibration = true;
            this.playOpenAnim(this.vibrationNode);
        }
    }

    private refreshEffectBtnState()
    {
        if(this.effectBtnState == 0)
        {
            AudioManager.CloseEffect();
            this.playCloseAnim(this.effectNode);
        }else
        {
            AudioManager.OpenEffect();
            this.playOpenAnim(this.effectNode);
        }
    }

    private playOpenAnim(node){
        if(node)
        {
            let anim = node.getComponent(cc.Animation);
            if(anim)
            {
                anim.play("musicOpenAnim");
            }
        }
    }

    private playCloseAnim(node)
    {
        if(node)
        {
            let anim = node.getComponent(cc.Animation);
            if(anim)
            {
                anim.play("musicCloseAnim");
            }
        }
    }
}
