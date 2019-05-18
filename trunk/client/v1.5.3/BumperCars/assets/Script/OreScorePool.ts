import MyNodePool from "./common/MyNodePool";

const {ccclass, property} = cc._decorator;

@ccclass
export default class OreScorePool extends MyNodePool
{
    protected CreateNode()
    {
        let node = super.CreateNode();
        if(node)
        {
            node.getComponent(cc.Label).setIsUseCostom(true);
            return node;
        }
        return null;
    }
}
