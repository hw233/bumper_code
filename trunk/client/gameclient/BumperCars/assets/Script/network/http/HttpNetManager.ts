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

/**
* 消息管理器，负责跟服务器收发消息包
* @class
*/

import GameLog from "../../Log/LogTool";
import GameUI from '../../manager/UIManager';

export default class Http_NetManager{
    private static m_xhr = null;

    public static InitMgr(){
        this.m_xhr = XMLHttpRequest ? new XMLHttpRequest() : null;//new ActiveXObject("MSXML2.XMLHTTP");
    }

    public static connectP (addr, port) {
    
    }

    public static onOpen (event) {
    }

    /**
     * 发送消息回调函数
     */
    public static onMessage () {
        let self = this;

        let m_xhr = self.m_xhr;

        try {
            if(m_xhr.readyState == 4) {
                if(m_xhr.status == 200) {
                    let rawData = JSON.parse(m_xhr.responseText);
                    console.log(rawData);
                } else {
                    self.onError("Net Status Error, Status = " + m_xhr.status);
                }
            }
        }catch (e){
        }
    }

    /*
    *   网络错误
    */
   public static onError (err) {
        GameLog.PrintLog(err);
    }

    /**
     * 发送消息到服务器
     * @param msgType   [大消息Id]
     * @param msgId     [小消息Id]
     * @param data      [消息体]
     */
    public static sendMsg (type, url, requestData, successCb, failCb = null) {

        if(window["wx"] == undefined){
            return;
        }

        GameUI.ShowLoadingView();

        let self = this;

        wx.request({
            url: url,
            data: requestData,
            method: type,
            success (res) {
                GameLog.PrintLog("服务器相应数据", res);
                
                let my_data = res.data;

                if (my_data.errno == 0) 
                {
                    //返回结果正确
                    //隐藏加载UI
                    GameUI.HideLoadingView();
                    successCb(my_data);
                } else {
                    if(failCb)
                    {
                        failCb(my_data);
                    }
                    GameLog.PrintLog("服务器错误，请重新尝试", my_data);
                }
            },
        });
    }

    public static sendMsg2(type, url, requestData, successCb){
        let self = this;

        let xhr = self.m_xhr;

        if (xhr != null) {
            xhr.open(type, url, true);
            xhr.send(requestData);
            xhr.onreadystatechange = function () {
                // readyState 五种状态
                // 0 － （未初始化）调用了open()方法，未调用send()方法
                // 1 － （载入）send()方法,正在发送请求
                // 2 － （载入完成）send()方法执行完成，已经接收到全部响应内容
                // 3 － （交互）正在解析响应内容
                // 4 － （完成）响应内容解析完成
                if (xhr.readyState == 4) {
                    // status：http状态码
                    if (xhr.status >= 200 && xhr.status < 300) {
                        let rawData = JSON.parse(this.m_xhr.responseText);
                        GameLog.PrintLog("服务器相应数据", rawData);
                        successCb(rawData.data);
                    } else {
                        GameLog.PrintLog("服务器错误，请重新尝试", xhr.status);
                    }
                }
            }
        }
    }
}
