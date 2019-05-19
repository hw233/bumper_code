import PopUpBase from './PopUpBase';
import EngineUtil from '../common/EngineUtil';
import GameUI from '../manager/UIManager';
import ItemTableInfo from '../table/ItemTableInfo';
import ItemSystem from '../Item/ItemSystem';

const {ccclass, property} = cc._decorator;

@ccclass
export default class RewardView extends PopUpBase 
{
    @property(cc.Node)
    private icon: cc.Node       = null;

    @property(cc.Node)
    private label: cc.Node       = null;

    public OnInit()
    {
        super.OnInit();

        this.registerTouchEndEvent(this.maskLayer, this.clickView.bind(this));
    }

    public RefreshView()
    {
        super.RefreshView();
        
        let itemID: number = Number(this.param.itemID);
        let itemNum: number = Number(this.param.itemNum);

        let itemInfo: ItemTableInfo = ItemSystem.GetInstance().GetTableInfoByItemID(itemID);
        if(itemInfo)
        {
            let itemName: string = itemInfo.GetItemName();
            let str: string = itemName + "*" + itemNum;

            let itemType: number = ItemSystem.GetInstance().GetTypeByItemID(itemID)

            EngineUtil.SetNodeText(this.label, str);
            EngineUtil.SetNodeTexture(this.icon, "common/Common_Icon_Big_" + itemType);
        }
    }

    private clickView()
    {
        this.node.destroy();

        GameUI.ShowViewByQuene();
    }
}
