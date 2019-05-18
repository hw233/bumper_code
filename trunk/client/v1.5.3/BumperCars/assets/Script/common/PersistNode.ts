
const {ccclass, property} = cc._decorator;

@ccclass

//常驻节点组件
export default class NewClass extends cc.Component {
    protected onLoad () 
    {
        cc.game.addPersistRootNode(this.node);
    }
    
    protected onDestroy()
    {
        cc.game.removePersistRootNode(this.node);
    }
}
