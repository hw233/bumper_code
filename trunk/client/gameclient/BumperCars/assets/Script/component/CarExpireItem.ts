import ComponentBase from '../common/ComponentBase';
import CarTableInfo from '../table/CarTableInfo';
import EngineUtil from '../common/EngineUtil';
import { AtlasType } from '../common/EngineUtil';
import ItemData from '../Item/ItemData';
import ItemManager from '../Item/ItemManager';
import ItemSystem from '../Item/ItemSystem';

const {ccclass, property} = cc._decorator;

@ccclass
export default class CarExpireItem extends ComponentBase
{
    @property(cc.Node)
    protected carIconNode: cc.Node        = null;

    @property(cc.Node)
    protected carNameNode: cc.Node        = null;

    @property(cc.Toggle)
    private toggle: cc.Toggle             = null;

    @property(cc.Node)
    private eventNode: cc.Node            = null;

    public soleID: number               = null;

    protected onLoad()
    {

    }

    public refreshView(soleID: number)
    {
        this.soleID = soleID;

        let itemData: ItemData = ItemManager.GetInstance().GetItemByID(soleID);
        if(itemData)
        {
            let carInfo: CarTableInfo = ItemSystem.GetInstance().GetTableInfoByItemID(itemData.GetItemID());
            if(carInfo)
            {
                EngineUtil.SetNodeTextureByType(AtlasType.AT_StartScene, this.carIconNode, "Car_" + carInfo.GetImgID());
                EngineUtil.SetNodeTextureByType(AtlasType.AT_StartScene, this.carNameNode, "CarNameImg_" + carInfo.GetImgID());
            }
        }
    }

    protected btnClicked()
    {
    }
}