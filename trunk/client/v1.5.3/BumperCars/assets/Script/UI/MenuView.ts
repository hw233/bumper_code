import UIBase from './UIBase';
import AudioManager from '../common/AudioManager';
import WXHelper from '../common/WXHelper';
import { UIType } from './UIDefine';
import { RankList } from './RanklistLayer';
import GameUI from '../manager/UIManager';
import { MessageType } from '../common/EventCenter';

const {ccclass, property} = cc._decorator;

@ccclass
export default class MenuView extends UIBase
{
    @property({type: cc.Node})
    private settingBtn: cc.Node = null

    @property({type: cc.Node})
    private rankListBtn: cc.Node = null

    @property({type: cc.Node})
    private moreGameBtn: cc.Node = null

    @property(cc.Animation)
    private viewAnimate: cc.Animation = null

    @property(cc.Node)
    private closeBtn: cc.Node = null

    @property(cc.Node)
    private maskLayer: cc.Node = null

    public OnInit()
    {
        super.OnInit();

        //阻止事件向下传递
        if(this.maskLayer.getComponent(cc.BlockInputEvents) === null)
        {
            this.maskLayer.addComponent(cc.BlockInputEvents);
        }

        this.registerClickEvent(this.moreGameBtn, this.moreGameBtnClicked.bind(this));
        this.registerClickEvent(this.rankListBtn, this.rankListBtnClicked.bind(this));
        this.registerClickEvent(this.settingBtn, this.settingBtnClicked.bind(this));
        this.registerTouchEndEvent(this.closeBtn, this.closeBtnClicked.bind(this));

        if(this.viewAnimate)
        {
            this.viewAnimate.on("finished", this.playFinished.bind(this));
        }
    }

    public ShowView()
    {
        super.ShowView();

        this.isShow = true;

        this.maskLayer.active = true;
        this.closeBtn.active = true;

        if(this.viewAnimate)
        {
            this.viewAnimate.play("MenuOpenAnim");
        }
    }

    public HideView()
    {
        super.HideView();

        this.isShow = false;

        if(this.viewAnimate)
        {
            this.viewAnimate.play("MenuCloseAnim");
        }

        this.dispatchEvent(MessageType.GameLoopBtn_Hide_Event);
    }

    //关闭按钮点击
    private closeBtnClicked()
    {
        GameUI.HideUI(this.GetType());
    }

    //更多游戏按钮点击
    private moreGameBtnClicked()
    {
        AudioManager.PlayBtnSound();

        WXHelper.MoreGame();
    }

    //排行榜按钮点击
    private rankListBtnClicked()
    {
        AudioManager.PlayBtnSound();

        GameUI.ShowUI(UIType.UT_RankListView, RankList.WorldRankList);
    }

    //设置按钮点击
    private settingBtnClicked()
    {
        AudioManager.PlayBtnSound();

        GameUI.ShowUI(UIType.UT_SettingView);
    }

    private playFinished()
    {
        if(this.isShow === false)
        {
            this.maskLayer.active = false;
            this.closeBtn.active = false;
        }
        else
        {
            this.dispatchEvent(MessageType.GameLoopBtn_Show_Event);
        }
    }
}
