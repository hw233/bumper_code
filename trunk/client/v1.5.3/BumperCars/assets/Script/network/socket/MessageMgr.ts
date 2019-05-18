import DataPackage from './DataPackage';
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

//管理所有收到的数据包(使用队列实现)
export default class MessageMgr
{
    
    private static allMsg: Array<DataPackage> = null;

    public static InitMgr()
    {
        this.allMsg = [];
    }

    //添加一个数据包
    public static PushDataPackage(dataPackage: DataPackage): void
    {
        if(dataPackage)
        {
            this.allMsg.push(dataPackage);
        }
    }

    //获取一个数据包
    public static PopDataPackage(): DataPackage
    {
        if(this.allMsg.length > 0)
        {
            return this.allMsg.shift();
        }
    }

    //获得数据包的数量
    public static GetDataPackageCount(): number
    {
        if(this.allMsg)
        {
            return this.allMsg.length;
        }
        return 0;
    }

}
