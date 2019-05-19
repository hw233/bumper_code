import MessageUtil from "../network/socket/MessageUtil"

import {MsgID} from "../network/socket/MsgID";
import {Msg_Item_Type} from "../network/socket/MsgID";
import SocketNetManager from "../network/socket/SocketNetManager";

//处理道具的消息
export default class ItemSend{
    //生成矿石的消息
    SendGenerateItem(posX: number, posY: number, tag: number)
    {
        let data = 
        {
            action: Msg_Item_Type._ITEM_ACTION_CREATE_RANDOM_ITEM_POS,
            point_x: MessageUtil.ConvertToInt(posX),
            point_y: MessageUtil.ConvertToInt(posY),
            tag: tag,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_ITEM_ATTR, data, false);
    }

    //采集矿石的消息
    SendPickMessage(itemID)
    {
        let data = 
        {
            action: Msg_Item_Type._ITEM_ACTION_PICKUP_RANDOM_ITEM,
            item_id: itemID,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_ITEM_ATTR, data, false);
    }

    SendDeleteItemMessage(itemID)
    {
        let data = 
        {
            action: Msg_Item_Type._ITEM_ACTION__DEL,
            item_id: itemID,
        };

        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_ITEM_ATTR, data, false);
    }
}
