import ComponentBase from '../common/ComponentBase';
import EngineUtil, { AtlasType } from '../common/EngineUtil';
import EventCenter from '../common/EventCenter';
import { MessageType } from '../common/EventCenter';
import MotionTableInfo from '../table/MotionTableInfo';
import ItemSystem from '../Item/ItemSystem';

const {ccclass, property} = cc._decorator;

@ccclass
export default class CarLibMotionCom extends ComponentBase
{
    @property(cc.Node)
    private motionIcon: cc.Node = null;

    @property(cc.Node)
    private eventNode: cc.Node = null;

    @property(cc.Node)
    private motionNameNode: cc.Node = null;

    @property(cc.Node)
    private lockImgNode: cc.Node = null;

    @property(cc.Node)
    private tryFlagNode: cc.Node = null;

    private motionID: number = null;

    protected onLoad()
    {
        this.registerTouchEndEvent(this.eventNode, this.btnClicked.bind(this));
    }

    public initView(id: number)
    {
        this.motionID = id;

        this.refreshView();
    }

    public refreshView()
    {
        let motionInfo: MotionTableInfo = ItemSystem.GetInstance().GetTableInfoByItemID(this.motionID);
        if(motionInfo)
        {
            if(this.tryFlagNode)
            {
                this.tryFlagNode.active = ItemSystem.GetInstance().IsTimeLimitItem(this.motionID);
            }

            EngineUtil.SetNodeTextureByType(AtlasType.AT_CarLibLayer, this.motionIcon, "CarLib_MotionIcon_" + motionInfo.GetImgID());
            EngineUtil.SetNodeTextureByType(AtlasType.AT_CarLibLayer, this.motionNameNode, "MotionName_" + motionInfo.GetImgID());
        }

        let isUnlock = false;   //carInfo.GetConditionID() === 1;
        if(ItemSystem.GetInstance().IsHasItemByItemID(this.motionID))
        {
            isUnlock = true;
        }
        
        this.lockImgNode.active = !isUnlock;
    }

    private btnClicked()
    {
        EventCenter.DispatchEvent(MessageType.CarLibMotionClicked, this.motionID);
    }
}