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

import SceneMgr from "../../manager/SceneMgr";
import MessageMgr from "./MessageMgr";
import MessageDipatcher from "./MessageDipatcher";
import DataPackage from './DataPackage';

export default class MessageHandler
{
    public static FrameOnMove(dt: number)
    {
        while(MessageMgr.GetDataPackageCount() > 0)
        {
            //切换场景中不允许处理网络消息包
            if(SceneMgr.IsSwitched())break;

            let dataPackage: DataPackage = MessageMgr.PopDataPackage();
            if(dataPackage)
            {
                let msgID = dataPackage.getMsgID();
                let msgType = dataPackage.getMsgType();
                let msgData = dataPackage.getData();
                
                MessageDipatcher.handleMessage(msgID, msgType, msgData);

                DataPackage.PutDataPacket(dataPackage);
            }
        }
    }
}
