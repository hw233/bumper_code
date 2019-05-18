import BettleData from '../data/net/BettleData';
import EventCenter from '../common/EventCenter';
import GameData from '../data/GameData';
import HandleBase from './HandleBase';
import MessageUtil from '../network/socket/MessageUtil';
import { MessageType } from '../common/EventCenter';
import { Msg_Item_Type } from '../network/socket/MsgID';
import { MsgID } from '../network/socket/MsgID';


export default class ItemHandle extends HandleBase
{
    public constructor()
    {
        super();
    }

    public RegisterAllHandle()
    {
        //随机一个矿石位置的消息
        this.RegisterHandle(MsgID.MSG_ITEM_ATTR, Msg_Item_Type._ITEM_ACTION_CREATE_RANDOM_ITEM_POS, this.OnRandomItemLocation.bind(this));
        //生成矿石的消息
        this.RegisterHandle(MsgID.MSG_ITEM_ATTR, Msg_Item_Type._ITEM_ACTION_CREATE_RANDOM_ITEM, this.OnGenerateItem.bind(this));
        //采集矿石的消息
        this.RegisterHandle(MsgID.MSG_ITEM_ATTR, Msg_Item_Type._ITEM_ACTION_PICKUP_RANDOM_ITEM, this.OnCollectMessage.bind(this));
        //添加道具
        this.RegisterHandle(MsgID.MSG_ITEM_ATTR, Msg_Item_Type._ITEM_ACTION__ADD, this.OnAddItemMessage.bind(this));
        //删除道具
        this.RegisterHandle(MsgID.MSG_ITEM_ATTR, Msg_Item_Type._ITEM_ACTION__DEL, this.OnDeleteItemMessage.bind(this));
        //更新道具
        this.RegisterHandle(MsgID.MSG_ITEM_ATTR, Msg_Item_Type._ITEM_ACTION__UPDATE, this.OnUpdateItemMessage.bind(this));
        //使用道具
        this.RegisterHandle(MsgID.MSG_ITEM_ATTR, Msg_Item_Type._ITEM_ACTION__USE, this.OnUseItemMessage.bind(this));
    }

    //随机一个矿石位置的消息
    OnRandomItemLocation()
    {
        EventCenter.DispatchEvent(MessageType.DLD_Receive_Random_Point);
    }

    //生成矿石的消息
    OnGenerateItem(msg)
    {
        let bettleData: BettleData = GameData.GetBettleData();
        if(bettleData)
        {
            bettleData.setLocationTag(msg.tag);
            bettleData.setTypeID(msg.item_type);
            bettleData.setID(msg.item_id);
            bettleData.setItemScore(msg.score);
            bettleData.setLocation(MessageUtil.ConvertToFloat(msg.point_x), MessageUtil.ConvertToFloat(msg.point_y));
            EventCenter.DispatchEvent(MessageType.DLD_Recv_Generate_Item);
        }
    }

    //采集矿石的消息
    OnCollectMessage(msg)
    {
        let bettleData: BettleData = GameData.GetBettleData();
        if(bettleData)
        {
            bettleData.setPickItemID(msg.item_id);
            bettleData.setPickerID(msg.target_id);
            EventCenter.DispatchEvent(MessageType.DLD_Recv_Pick_Item);
        }
    }

    //添加道具
    private OnAddItemMessage(msg)
    {
        EventCenter.DispatchEvent(MessageType.Item_Add_Item_Event, {
            soleID: Number(msg.item_id),
            itemID: Number(msg.item_type),
            type: Number(msg.type),
            liftTime: Number(msg.lift_time),
            amount: Number(msg.amount),
        });
    }

    //删除道具
    private OnDeleteItemMessage(msg)
    {
        EventCenter.DispatchEvent(MessageType.Item_Delete_Item_Event, {
            soleID: Number(msg.item_id),
        });

    }

    //更新道具
    private OnUpdateItemMessage(msg)
    {
        EventCenter.DispatchEvent(MessageType.Item_Update_Item_Event, {
            soleID: Number(msg.item_id),
            liftTime: Number(msg.lift_time),
            amount: Number(msg.amount),
        });
    }

    //使用道具
    private OnUseItemMessage(msg)
    {
        EventCenter.DispatchEvent(MessageType.Item_Used_Item_Event);
    }
}
