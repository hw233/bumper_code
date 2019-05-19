
import {MsgID} from "../network/socket/MsgID";
import {Msg_Vas_Type} from "../network/socket/MsgID";
import EventCenter from "../common/EventCenter";
import {MessageType} from "../common/EventCenter";
import HandleBase from './HandleBase';
import ItemSystem from '../Item/ItemSystem';

export default class VasHandle extends HandleBase
{
    constructor()
    {
        super();
    }

    public RegisterAllHandle(){
        //购买道具(buff)的消息
        this.RegisterHandle(MsgID.MSG_VAS, Msg_Vas_Type.BUY_SUCCESS, this.OnBuyBuffSucMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_VAS, Msg_Vas_Type.BUY_FAIL, this.OnBuyBuffFailMessage.bind(this));

        //购买道具的消息
        this.RegisterHandle(MsgID.MSG_VAS, Msg_Vas_Type._VAS_SHOP_BUY_SUC, this.OnBuyItemSucMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_VAS, Msg_Vas_Type._VAS_SHOP_BUY_FAIL, this.OnBuyItemFailMessage.bind(this));
    }

    //购买buff成功的消息
    private OnBuyBuffSucMessage(msg)
    {
        EventCenter.DispatchEvent(MessageType.DLD_Receive_Buy_Buff, msg.type);
    }

    //购买buff失败的消息
    private OnBuyBuffFailMessage(msg)
    {

    }

    //购买道具成功的消息
    private OnBuyItemSucMessage(msg)
    {
        if(ItemSystem.GetInstance().IsTimeLimtiByItemID(msg.item_type))
        {
            //免费使用
            EventCenter.DispatchEvent(MessageType.Item_Try_Suc_Event, {
                itemID: msg.item_type,
                liftTime: msg.lift_time
            });
            return;
        }
        EventCenter.DispatchEvent(MessageType.StartScene_RefreshCarList);
        EventCenter.DispatchEvent(MessageType.StartScene_RefreshTopView);
        EventCenter.DispatchEvent(MessageType.CarLib_Unlock_Item_Suc, msg.item_type);
    }

    //购买道具失败的消息
    private OnBuyItemFailMessage(msg)
    {
        EventCenter.DispatchEvent(MessageType.CarLib_Unlock_Item_Fail, msg.item_type);
    }
}