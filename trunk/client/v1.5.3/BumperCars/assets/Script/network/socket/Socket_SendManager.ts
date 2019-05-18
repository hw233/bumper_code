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

import BettleSend from "../../sendMsg/BettleSend";
import CommonSend from "../../sendMsg/CommonSend";
import ExchangeSend from "../../sendMsg/ExchangeSend";
import ItemSend from "../../sendMsg/ItemSend";
import VasSend from "../../sendMsg/VasSend";
import LoginSend from "../../sendMsg/LoginSend";
import FriendSend from '../../sendMsg/FriendSend';
import SignInSend from '../../sendMsg/SignInSend';
import TaskSend from '../../sendMsg/TaskSend';
import ActionSend from '../../sendMsg/ActionSend';

export default class Socket_SendMgr{
    private static _loginSend       = null;
    private static _bettleSend      = null;
    private static _commonSend      = null;
    private static _actionSend      = null;
    private static _exchangeSend    = null;
    private static _itemSend        = null;
    private static _vasSend         = null;
    private static _friendSend      = null;
    private static _signInSend      = null;
    private static _taskSend        = null;

    public static InitMgr(){
        this._loginSend = new LoginSend();
        this._bettleSend = new BettleSend();
        this._commonSend = new CommonSend();
        this._actionSend = new ActionSend();
        this._exchangeSend = new ExchangeSend();
        this._itemSend = new ItemSend();
        this._vasSend = new VasSend();
        this._friendSend = new FriendSend();
        this._signInSend = new SignInSend();
        this._taskSend = new TaskSend();
    }

    public static GetLoginSend(): LoginSend
    {
        return this._loginSend;
    }

    public static GetBettleSend():BettleSend
    {
        return this._bettleSend;
    }

    public static GetCommonSend(): CommonSend
    {
        return this._commonSend;
    }

    public static GetExchangeSend(): ExchangeSend
    {
        return this._exchangeSend;
    }

    public static GetItemSend(): ItemSend
    {
        return this._itemSend;
    }

    public static GetVasSend(): VasSend
    {
        return this._vasSend;
    }

    public static GetFriendSend(): FriendSend
    {
        return this._friendSend;
    }

    public static GetSignInSend(): SignInSend
    {
        return this._signInSend;
    }

    public static GetTaskSend(): TaskSend
    {
        return this._taskSend;
    }

    public static GetActionSend(): ActionSend
    {
        return this._actionSend;
    }
}