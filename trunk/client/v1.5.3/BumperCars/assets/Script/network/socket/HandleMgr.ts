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

import HandleBase from '../../handleMsg/HandleBase';
import BettleHandle from '../../handleMsg/BettleHandle';
import CommonHandle from '../../handleMsg/CommonHandle';
import ExchangeHandle from '../../handleMsg/ExchangeHandle';
import FriendHandle from '../../handleMsg/FriendHandle';
import ItemHandle from '../../handleMsg/ItemHandle';
import LoginHandle from '../../handleMsg/LoginHandle';
import SignInHandle from '../../handleMsg/SignInHandle';
import VasHandle from '../../handleMsg/VasHandle';
import TaskHandle from '../../handleMsg/TaskHandle';
import ActionHandle from '../../handleMsg/ActionHandle';
import UserAttribHandle from '../../handleMsg/UserAttribHandle';
import ItemAttrbHandle from '../../handleMsg/ItemAttrbHandle';

//负责管理所有的处理服务器消息的句柄
export default class HandleMgr
{
    //管理所有消息处理者
    private static allHandler: Array<HandleBase> = [];

    //保存所有的消息处理函数
    private static handleList: any = {};

    //注册管理者
    public static RegisterHandler(handler: HandleBase)
    {
        if(handler)
        {
            this.allHandler.push(handler);
        }
    }

    //注册所有消息处理函数
    public static InitMgr()
    {
        new BettleHandle();
        new CommonHandle();
        new ExchangeHandle();
        new FriendHandle();
        new ItemHandle();
        new LoginHandle();
        new SignInHandle();
        new VasHandle();
        new TaskHandle();
        new ActionHandle();
        new UserAttribHandle();
        new ItemAttrbHandle();

        // for(let i = 0, len = this.allHandler.length; i < len; i ++)
        // {
        //     if(this.allHandler[i])
        //     {
        //         this.allHandler[i].RegisterAllHandle();
        //     }
        // }
    }

    public static Clear()
    {
        this.handleList = null;

        this.allHandler.length = 0;
        this.allHandler = null;
    }

    //注册消息处理函数
    public static RigisterMessageHandle(msgID, msgType, handler)
    {
        if(!this.handleList[msgID])
        {
            this.handleList[msgID] = {};
        }

        if(msgType){
            this.handleList[msgID][msgType] = handler;
        }else{
            this.handleList[msgID] = handler;
        }
    }

    //获取消息处理函数
    public static GetMessageHandle(msgID, msgType){
        if(this.handleList){
            if(this.handleList[msgID]){
                if(msgType){
                    if(this.handleList[msgID][msgType]){
                        return this.handleList[msgID][msgType];
                    }
                }else{
                    return this.handleList[msgID];
                }
            }
        }
        return null;
    }
}
