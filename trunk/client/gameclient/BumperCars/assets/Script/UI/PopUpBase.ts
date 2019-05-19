import GameConfig from "../GameConfig";
import PopUpEffect from "../common/PopUpEffect";
import AudioManager from "../common/AudioManager";
import UIBase from './UIBase';
import GameUI from '../manager/UIManager';

const {ccclass, property} = cc._decorator;

//弹窗基类
@ccclass
export default class PopUpBase extends UIBase
{
    //关闭按钮
    @property({type: cc.Node})
    protected closeBtn: cc.Node  = null;

    //半透明遮罩层
    @property({type: cc.Node})
    protected maskLayer: cc.Node = null;

    //弹窗层
    @property({type: cc.Node})
    protected popupLayer: cc.Node = null;

    public OnInit() 
    {
        super.OnInit();

        //阻止事件向下传递
        if(this.maskLayer.getComponent(cc.BlockInputEvents) === null)
        {
            this.maskLayer.addComponent(cc.BlockInputEvents);
        }

        this.registerClickEvent(this.closeBtn, this.closeBtnClicked.bind(this));
    }
    
    public OnDestroy()
    {
        super.OnDestroy();
    }

    public ShowView(isEffect: boolean = true)
    {
        super.ShowView(isEffect);

        if(this.isShow)
        {
            return;
        }

        this.isShow = true;

        this.maskLayer.active = true;

        if(isEffect)
        {
            PopUpEffect.Show_ScaleEffect(this.popupLayer, GameConfig.PopUpDuration, this.OnShowFinished.bind(this));
        }
        else
        {
            if(this.popupLayer)
            {
                this.popupLayer.scale = 1;
                this.popupLayer.active = true;
            }
        }
    }

    public HideView(isEffect: boolean = true)
    {
        super.HideView(isEffect);

        if(!this.isShow)
        {
            return;
        }

        this.isShow = false;

        this.maskLayer.active = false;

        if(isEffect)
        {
            PopUpEffect.Hide_ScaleEffect(this.popupLayer, GameConfig.PopUpDuration);
        }
        else
        {
            if(this.popupLayer)
            {
                this.popupLayer.active = true;

            }
        }
    }

    //界面显示完成调用
    protected OnShowFinished()
    {

    }

    protected closeBtnClicked()
    {
        AudioManager.PlayBtnSound();

        GameUI.HideUI(this.GetType());
    }
};
