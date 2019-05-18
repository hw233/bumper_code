import GameConfig from "../GameConfig";

const {ccclass, property} = cc._decorator;

@ccclass

//弹窗效果类
export default class PopUpEffect
{

    //显示-从小到大
    public static Show_ScaleEffect(node: cc.Node, duration: number, callback: Function = null)
    {
        if(!node)return;

        if(duration == undefined)
        {
            duration = GameConfig.PopUpDuration;
        }
        node.active = true;

        let oldScaleX = 1;
        let oldScaleY = 1;

        node.setScale(0.1, 0.1);

        let action1 = cc.scaleTo(duration, oldScaleX, oldScaleY);
        // let action2 = new cc.scaleTo(duration, 1, 1);
        node.runAction(cc.sequence(action1, cc.callFunc(function()
        {
            node.setScale(oldScaleX, oldScaleY);
            if(callback)
            {
                callback();
            }
        })));
    }

    //隐藏-从大到小
    public static Hide_ScaleEffect(node: cc.Node, duration: number)
    {
        if(!node)return;

        if(duration == undefined){
            duration = GameConfig.PopUpDuration;
        }

        let action1 = cc.scaleTo(duration, 0, 0);
        node.runAction(cc.sequence(action1, cc.callFunc(function(){
            node.active = false;
        })));
    }

    //移除-从大到小
    public static Remove_ScaleEffect(parent: cc.Node, node: cc.Node, duration: number)
    {
        if(!node)return;

        if(duration == undefined){
            duration = GameConfig.PopUpDuration;
        }

        let action1 = cc.scaleTo(duration, 0, 0);
        node.runAction(cc.sequence(action1, cc.callFunc(function(){
            if(parent){
                parent.destroy();
            }
        })));
    }
}
