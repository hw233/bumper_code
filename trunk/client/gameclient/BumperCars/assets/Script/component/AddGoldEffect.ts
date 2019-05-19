import EngineUtil from "../common/EngineUtil";
import ItemSystem from '../Item/ItemSystem';

const {ccclass, property} = cc._decorator;

@ccclass
export default class AddGoldEffect extends cc.Component
{
    @property(cc.Node)
    private goldNode: cc.Node = null;

    public playEffect()
    {
        let self = this;

        if(this.goldNode)
        {
            let GoldNumNode = this.goldNode;

            let goldLable: cc.Label = GoldNumNode.getComponent(cc.Label);
            if(goldLable)
            {
                let curGoldNum: number = parseInt(goldLable.string);
                let finalGoldNum: number = ItemSystem.GetInstance().GetGoldNum();
                let func = function()
                {
                    if( (curGoldNum + 50) > finalGoldNum)
                    {
                        self.unschedule(func);

                        EngineUtil.SetNodeText(GoldNumNode, finalGoldNum.toString())
                        return;
                    }else
                    {
                        curGoldNum += 50;
                    }
                    EngineUtil.SetNodeText(GoldNumNode, curGoldNum + "")
                };

                this.schedule(func, 1 / 60);
            }
        }
    }
}