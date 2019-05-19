import UIBase from './UIBase';
const {ccclass, property} = cc._decorator;

@ccclass
export default class LoadingView extends UIBase
{
    @property({type: cc.Node})
    private loadingNode: cc.Node = null;

    public OnEnter () 
    {
        super.OnEnter();
        
        this.loadingNode.runAction(cc.repeatForever(cc.blink(1, 2)));
    }

    public OnExit()
    {
        super.OnExit();

        this.loadingNode.stopAllActions();
    }
}
