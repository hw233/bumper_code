
import {MsgID} from "../network/socket/MsgID";
import {Msg_Vas_Type} from "../network/socket/MsgID";
import SocketNetManager from "../network/socket/SocketNetManager";

export default class VasSend
{
    //购买道具(buff)的消息
    public SendBuyBuffMessage(type)
    {
        let data = {
            action: Msg_Vas_Type.BUY,
            type: type,
            item_id: 1,
            item_type: 1,
            item_count: 1,
        };
        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_VAS, data);
    }

    //购买道具
    public SendBuyItemMessage(itemID: number, view: number = 0)
    {
        let data = {
            item_type: itemID,
            view: view,
            action: Msg_Vas_Type._VAS_SHOP_BUY,
        };
        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_VAS, data);
    }
}
