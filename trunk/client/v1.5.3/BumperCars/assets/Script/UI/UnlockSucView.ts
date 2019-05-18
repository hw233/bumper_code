import PopUpBase from "../UI/PopUpBase";
import WXHelper from "../common/WXHelper";
import {ShareSceneType} from "../common/ShareSceneType";
import { AtlasType } from '../common/EngineUtil';
import EngineUtil from '../common/EngineUtil';
import { ItemType } from '../Item/ItemDefine';
import ItemSystem from '../Item/ItemSystem';
import CarTableInfo from "../table/CarTableInfo";
import MotionTableInfo from '../table/MotionTableInfo';

const {ccclass, property} = cc._decorator;

@ccclass
export default class UnlockSucView extends PopUpBase
{
    @property({type: cc.Node})
    private shareBtn: cc.Node = null;

    @property(cc.Node)
    private icon: cc.Node = null;

    public OnInit () 
    {
        super.OnInit();

        this.registerClickEvent(this.shareBtn, this.shareBtnClicked.bind(this));
    }
        
    public RefreshView()
    {
        super.RefreshView();

        let itemType: ItemType = ItemSystem.GetInstance().GetTypeByItemID(this.param);
        if(itemType === ItemType.IT_CarType)
        {
            let carInfo: CarTableInfo = ItemSystem.GetInstance().GetTableInfoByItemID(this.param);
            if(carInfo)
            {
                EngineUtil.SetNodeTextureByType(AtlasType.AT_StartScene, this.icon, 'Car_' + carInfo.GetImgID());
            }
        }
        else if(itemType === ItemType.IT_MotionType)
        {
            let motionInfo: MotionTableInfo = ItemSystem.GetInstance().GetTableInfoByItemID(this.param);

            if(motionInfo)
            {
                EngineUtil.SetNodeTextureByType(AtlasType.AT_CarLibLayer, this.icon, "CarLib_MotionIcon_" + motionInfo.GetImgID());
            }
        }
    }

    private shareBtnClicked()
    {
        WXHelper.CommonShare(ShareSceneType.UnlockCarShare);
    }

}