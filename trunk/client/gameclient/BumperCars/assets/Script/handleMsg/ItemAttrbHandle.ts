import HandleBase from './HandleBase';
import { MsgID, Msg_ItemAttrib_Type } from '../network/socket/MsgID';
import ItemData from '../Item/ItemData';
import ItemManager from '../Item/ItemManager';
import ItemSystem from '../Item/ItemSystem';

export default class ItemAttrbHandle extends HandleBase
{
    public constructor()
    {
        super();
    }

    public RegisterAllHandle()
    {
        this.RegisterHandle(MsgID.MSG_ITEMTTRIB, Msg_ItemAttrib_Type._ITEMATTRIB_ITEMTYPE, this.OnItemTypeMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_ITEMTTRIB, Msg_ItemAttrib_Type._ITEMATTRIB_TYPE, this.OnTypeMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_ITEMTTRIB, Msg_ItemAttrib_Type._ITEMATTRIB_LIFE_TIME, this.OnLiftTimeMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_ITEMTTRIB, Msg_ItemAttrib_Type._ITEMATTRIB_AMOUNT, this.OnItemAmountMessage.bind(this));
    }

    private OnItemTypeMessage(msg)
    {
        let itemID: number = Number(msg.item_id);
        let data: number = Number(msg.attrib_data);

        let itemData: ItemData = ItemManager.GetInstance().GetItemByID(itemID);
        if(itemData)
        {
            itemData.SetItemID(data);
        }
    }

    private OnTypeMessage(msg)
    {
        let itemID: number = Number(msg.item_id);
        let data: number = Number(msg.attrib_data);

        let itemData: ItemData = ItemManager.GetInstance().GetItemByID(itemID);
        if(itemData)
        {
            itemData.SetItemType(data);
        }
    }

    private OnLiftTimeMessage(msg)
    {
        let itemID: number = Number(msg.item_id);
        let data: number = Number(msg.attrib_data);

        let itemData: ItemData = ItemManager.GetInstance().GetItemByID(itemID);
        if(itemData)
        {
            itemData.SetLiftTime(data);
        }

        ItemSystem.GetInstance().ShowExpireItem();
    }

    private OnItemAmountMessage(msg)
    {
        let itemID: number = Number(msg.item_id);
        let data: number = Number(msg.attrib_data);

        let itemData: ItemData = ItemManager.GetInstance().GetItemByID(itemID);
        if(itemData)
        {
            itemData.SetAmount(data);
        }
    }
}