
import {MsgID} from "../network/socket/MsgID";
import {Msg_Exchange_Type} from "../network/socket/MsgID";
import HandleBase from './HandleBase';

export default class ExchangeHandle extends HandleBase
{
    public constructor()
    {
        super();
    }

    public RegisterAllHandle()
    {
        //兑换道具的消息
        this.RegisterHandle(MsgID.MSG_EXCHANGE, Msg_Exchange_Type.EXCHANGE_ITEM, this.OnExchangeItem.bind(this));
    }

    //兑换道具的消息
    private OnExchangeItem(msg)
    {

    }
}
