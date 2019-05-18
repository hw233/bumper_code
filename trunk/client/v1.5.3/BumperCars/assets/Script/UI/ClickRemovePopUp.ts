import ComponentBase from '../common/ComponentBase';
import UIBase from './UIBase';
import GameUI from '../manager/UIManager';

const {ccclass, property} = cc._decorator;

//点击空白区域自动移除的弹窗
@ccclass
export default class ClickRemovePopUp extends ComponentBase
{
    //半透明遮罩层
    @property({type: cc.Node})
    protected maskLayer: cc.Node = null;

    //弹窗层
    @property({type: cc.Node})
    protected popupLayer: cc.Node = null;

    protected onLoad() 
    {
        this.registerTouchEndEvent(this.maskLayer, this.clickMaskLayer.bind(this) );
        this.registerTouchEndEvent(this.popupLayer, function(){});
    }

    private clickMaskLayer()
    {
        let uiView: UIBase = this.getComponent(UIBase);
        if(uiView)
        {
            GameUI.HideUI(uiView.GetType())
        }
    }
}

