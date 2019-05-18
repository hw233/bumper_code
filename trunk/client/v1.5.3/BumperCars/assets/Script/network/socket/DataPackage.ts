////////////////////////////////////////////////////////////////////
//                            _ooOoo_                             //
//                           o8888888o                            //
//                           88" . "88                            //
//                           (| ^_^ |)                            //
//                           O\  =  /O                            //
//                        ____/`---'\____                         //
//                      .'  \\|     |//  `.                       //
//                     /  \\|||  :  |||//  \                      //
//                    /  _||||| -:- |||||-  \                     //
//                    |   | \\\  -  /// |   |                     //
//                    | \_|  ''\---/''  |   |                     //
//                    \  .-\__  `-`  ___/-. /                     //
//                  ___`. .'  /--.--\  `. . ___                   //
//                ."" '<  `.___\_<|>_/___.'  >'"".                //
//              | | :  `- \`.;`\ _ /`;.`/ - ` : | |               //
//              \  \ `-.   \_ __\ /__ _/   .-` /  /               //
//        ========`-.____`-.___\_____/___.-`____.-'========       //
//                             `=---='                            //
//        ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^      //
//            佛祖保佑       无BUG        不修改                   //
////////////////////////////////////////////////////////////////////

import { MsgID } from './MsgID';

//数据包
export default class DataPacket
{
    private static _pool        = null;

    private _msgID: MsgID       = null;       //消息ID
    private _msgType: number    = null;        //消息类型
    private _data: any          = null;     //数据

    private constructor()
    {
        this.init();
    }

    public init()
    {
        this._msgID = 0;
        this._msgType = 0;
        this._data = null;
    }

    public setMsgID(msgID: MsgID): void
    {
        this._msgID = msgID;
    }

    public getMsgID(): MsgID
    {
        return this._msgID;
    }

    public setMsgType(type: number): void
    {
        this._msgType = type;
    }

    public getMsgType(): number
    {
        return this._msgType;
    }

    public setData(data: any): void
    {
        this._data = data;
    }

    public getData(): any
    {
        return this._data;
    }

    public destroy(): void
    {
        this._data = null;
    }

    public static GetDataPacket()
    {
        if(!this._pool)
        {
            this._pool = [];
        }
        
        let last = this._pool.length - 1;
        if (last < 0) 
        {
            return new DataPacket();
        }
        else 
        {
            // Pop the last object in pool
            let data = this._pool[last];
            this._pool.length = last;

            data.init();

            return data;
        }
    }

    public static PutDataPacket(data: DataPacket)
    {
        if(this._pool)
        {
            this._pool.push(data);
        }
    }

    public static Destroy()
    {
        if(this._pool)
        {
            for(let i = 0, len = this._pool.length; i < len; i ++)
            {
                this._pool[i].destroy();
            }
            this._pool.length = 0;
            this._pool = null;
        }
    }
}