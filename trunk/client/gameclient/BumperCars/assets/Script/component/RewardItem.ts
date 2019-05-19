import EngineUtil from '../common/EngineUtil';
import ItemSystem from '../Item/ItemSystem';
import ItemTableInfo from '../table/ItemTableInfo';
const {ccclass, property} = cc._decorator;

@ccclass
export default class RewardItem extends cc.Component
{
    @property(cc.Node)
    private icon: cc.Node = null

    @property(cc.Node)
    private num: cc.Node = null;

    public initView(itemID: number, rewardNum: number) 
    {
        let itemInfo: ItemTableInfo = ItemSystem.GetInstance().GetTableInfoByItemID(itemID);
        if(itemInfo)
        {
            let fileName: string = "common/Common_Icon_" + itemInfo.GetImgID();

            EngineUtil.SetNodeText(this.num, rewardNum.toString());
            EngineUtil.SetNodeTexture(this.icon, fileName);
        }
    }
}
