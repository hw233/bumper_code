import ComponentBase from '../common/ComponentBase';
import EngineUtil, { AtlasType } from '../common/EngineUtil';
import EventCenter, { MessageType } from '../common/EventCenter';
import { CarUnlockState } from '../GameDefine';
import CarTableInfo from '../table/CarTableInfo';
import ItemSystem from '../Item/ItemSystem';


const {ccclass, property} = cc._decorator;

@ccclass
export default class CarItemView extends ComponentBase
{
    protected bgNode: cc.Node             = null;

    protected carIconNode: cc.Node        = null;

    protected lockImgNode: cc.Node        = null;

    protected stateBtnNode: cc.Node       = null;

    protected carNameNode: cc.Node        = null;

    protected eventNode: cc.Node          = null;

    protected curState: CarUnlockState    = null;

    protected tryFlagNode: cc.Node        = null;
    
    public carID: number                  = null //车辆id

    protected onLoad()
    {
        this.bgNode             = this.node.getChildByName("Panel");
        this.stateBtnNode       = this.node.getChildByName("StateBtn");
        this.carNameNode        = this.node.getChildByName("CarNameImg");
        this.eventNode          = this.node.getChildByName("EventNode");
        this.carIconNode        = this.node.getChildByName("CarIcon");
        this.lockImgNode        = this.node.getChildByName("LockImg");
        this.tryFlagNode        = this.node.getChildByName("CarLib_TryFlag");

        this.registerClickEvent(this.stateBtnNode, this.btnClicked.bind(this));
        this.registerTouchEndEvent(this.eventNode, this.btnClicked.bind(this));
    }

    public refreshView(carID: number = null, isUse: boolean = false)
    {
        if(carID !== null)
        {
            this.carID = carID;
        }

        let carInfo: CarTableInfo = ItemSystem.GetInstance().GetTableInfoByItemID(this.carID);
        if(carInfo)
        {
            if(this.tryFlagNode)
            {
                this.tryFlagNode.active = ItemSystem.GetInstance().IsTimeLimitItem(this.carID);
            }

            let isUnlock = false;   //carInfo.GetConditionID() === 1;
            if(ItemSystem.GetInstance().IsHasItemByItemID(this.carID))
            {
                isUnlock = true;
            }
            
            this.lockImgNode.active = !isUnlock;

            if(isUnlock)
            {
                this.curState = CarUnlockState.CUS_AlreadyUnlock;
                EngineUtil.SetNodeTextureByType(AtlasType.AT_StartScene, this.stateBtnNode, "shiyong");
                EngineUtil.SetNodeTextureByType(AtlasType.AT_StartScene, this.bgNode, "putongkuang");
            }
            else
            {
                this.curState = CarUnlockState.CUS_NotUnlock;
                EngineUtil.SetNodeTextureByType(AtlasType.AT_StartScene, this.stateBtnNode, "jiesuo");
                EngineUtil.SetNodeTextureByType(AtlasType.AT_StartScene, this.bgNode, "jiesuokuang");
            }

            EngineUtil.SetNodeTextureByType(AtlasType.AT_StartScene, this.carIconNode, "Car_" + carInfo.GetImgID());
            EngineUtil.SetNodeTextureByType(AtlasType.AT_StartScene, this.carNameNode, "CarNameImg_" + carInfo.GetImgID());

            if(isUse)
            {
                this.curState = CarUnlockState.CUS_Using;
                EngineUtil.SetNodeTextureByType(AtlasType.AT_StartScene, this.bgNode, "chuzhankluang");
                EngineUtil.SetNodeTextureByType(AtlasType.AT_StartScene, this.stateBtnNode, "chuzhan");
            }
        }
    }

    protected btnClicked()
    {
        EventCenter.DispatchEvent(MessageType.CarBtnClicked, {
            state: this.curState,
            carID : this.carID,
        });
    }
}