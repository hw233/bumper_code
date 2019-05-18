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

import DataPackage from './DataPackage';
import GameConfig from '../../GameConfig';
import GameLog from '../../Log/LogTool';
import GameUI from '../../manager/UIManager';
import MessageMgr from './MessageMgr';
import TestMessage from './TestMessage';
import { MsgID, Msg_Bettle_Type } from './MsgID';
import GlobalVar from '../../GlobalVar';
import Socket_SendMgr from './Socket_SendManager';
import ConfigData from '../../common/ConfigData';
import AccountData from '../../manager/AccountData';
import GameLocalDataMgr from '../../manager/GameLocalDataMgr';

// var WebSocket = WebSocket || window["WebSocket"] || window["MozWebSocket"];  

/**
* 消息管理器，负责跟服务器收发消息包--长连接
* @class
*/
export default class SocketNetManager{

    private static Instance: SocketNetManager   = null; 

    private m_WebSocket: WebSocket              = null;

    private m_IsConnected: boolean              = null;    //是否已连接到网络

    private m_FPSListener: Function             = null;

    private m_TimeOut                           = null;

    constructor()
    {
        this.m_TimeOut = null;
        this.m_WebSocket = null;
        this.m_IsConnected = false;
        this.m_FPSListener = null;
    }


    public static GetInstance(): SocketNetManager
    {
        if(this.Instance == null){
            this.Instance = new SocketNetManager();
        }
        return this.Instance;
    }

    //用于创建一个webSocket实例，执行后，客户端就会与服务端连接
    public Connect(addr, port): void
    {
        if(!GameConfig.ServerEnable)return;

        GameLog.PrintLog("Connect Server:", addr, ":", port);

        // this.m_WebSocket = new WebSocket("wss://gather.51weiwan.com:6505/test");
        // this.m_WebSocket = new WebSocket("wss://gather.51weiwan.com:5505?wxtest");
        this.m_WebSocket = new WebSocket(addr + ":" + port);

        this.m_IsConnected = false;

        this.m_WebSocket.onopen = this.onOpen.bind(this);
        this.m_WebSocket.onclose = this.onClose.bind(this);
        this.m_WebSocket.onmessage = this.onMessage.bind(this);
        this.m_WebSocket.onerror = this.onError.bind(this);

        //显示加载UI
        GameUI.ShowLoadingView();

        //设置连接超时回调函数
        this.m_TimeOut = setTimeout(this.connectTimeout.bind(this), GameConfig.ConnectTimeout);
    }

    //重连服务器
    public Reconnect()
    {
        GameLog.PrintLog("Reconnect!");

        this.m_WebSocket = null;

        GlobalVar.IsReconnected = true;

        this.Connect(ConfigData.ServerIP, ConfigData.ServerPort);
    }

    public SendFPSListener(listener: Function): void
    {
        this.m_FPSListener = listener;
    }

    /**
     * 发送消息到服务器
     * @param msgType   [消息类型]
     * @param data      [消息体]
     */
    public SendMsg(msgType: number, data, showLoadingView = true)  :void
    {
        if(GameConfig.ServerEnable && this.m_IsConnected === false)
        {
            GameLog.PrintLog("连接未建立！");
            return;
        }

        if(showLoadingView){
            //显示加载UI
            GameUI.ShowLoadingView();
        }

        let tempData: string = '';

        let strData:string = JSON.stringify(data);

        //包的类型
        tempData += this.fillData(msgType, 4);
        //包的长度
        tempData += this.fillData(this.getByteNum(strData), 4);

        //消息体
        tempData += strData;

        if(msgType === MsgID.MSG_WALK)
        {
            GameLog.PrintSendWalkLog(tempData);
        }
        else
        {
            GameLog.PrintSendNetLog(msgType, data.action, tempData);
        }

        //模拟服务器消息
        if(GameConfig.ServerEnable === false){
            TestMessage.HandleMessage(msgType, data);
            return;
        }

        this.sendData(tempData);
    }

    public IsConnect(): boolean
    {
        return this.m_IsConnected;
    }
    
    /**
     * 发送消息回调函数
     */
    public onMessage(event): void
    {
        //隐藏加载UI
        GameUI.HideLoadingView();

        let recvData = event.data;

        let msgID: number = parseInt(recvData.substring(0, 4));
        let msgLen: number = parseInt(recvData.substring(4, 4));
        let msgData: string = recvData.substring(8);
        
        let data: any = this.isJSON(msgData);
        if( !data ) 
        {
            return;
        }

        //添加到消息队列
        this.addPackage(msgID, data.action, data);

        if(msgID === MsgID.MSG_WALK)
        {
            GameLog.PrintRecvWalkLog(recvData);
        }
        else
        {
            GameLog.PrintRecvNetLog(msgID, data.action, recvData);
        }
    }

    //用于指定连接成功后的回调函数
    private onOpen(event) : void
    {
        GameLog.PrintLog("Connection Successed!");

        this.m_IsConnected = true;

        //隐藏加载UI
        GameUI.HideLoadingView();

        //如果已经登录过则需要重新登录
        if(GlobalVar.IsLoggedIn)
        {
            Socket_SendMgr.GetLoginSend().SendLoginMessage(AccountData.GetInstance().GetUserID(), AccountData.GetInstance().GetNickName(), AccountData.GetInstance().GetHeadImg());
        }
    }

    //用于指定连接关闭后的回调函数。
    private onClose(event): void
    {
        GameLog.PrintLog("Connection Closed!");

        this.m_IsConnected = false;

        clearTimeout(this.m_TimeOut);

        this.Reconnect();

        // //隐藏加载UI
        // GameUI.HideLoadingView();

        //显示失败窗口
        // GameUI.ShowLoadingFailView();
    }

    //通信发生错误时触发的回调函数
    private onError(err) : void
    {
        this.m_IsConnected = false;

        GameLog.PrintLog("服务器错误", err);
    }

    //添加到消息管理队列里
    private addPackage(msgID: MsgID, msgType: number, data: any): void
    {
        if(msgID === MsgID.MSG_WALK)
        {
            this.m_FPSListener && this.m_FPSListener(data);
        }
        else
        {
            let dataPackage: DataPackage = DataPackage.GetDataPacket();
            dataPackage.setMsgID(msgID);
            dataPackage.setMsgType(msgType);
            dataPackage.setData(data);

            if(msgID === MsgID.MSG_BATTLE)
            {
                if(msgType === Msg_Bettle_Type._BATTLE_ACTION_BATTLE_START || msgType === Msg_Bettle_Type._BATTLE_ACTION_TEAM_START_SUC )
                {
                    data.timeStamp = Date.now();
                }
            }
    
            MessageMgr.PushDataPackage(dataPackage);
        }
    }

    private sendData(data): void
    {
        if(this.m_WebSocket.readyState == 1)
        {
            this.m_WebSocket.send(data);

            if (this.m_WebSocket.bufferedAmount === 0) 
            {
                // 发送完毕
            } 
            else 
            {
                // 发送还没结束
            }
        }else
        {
            GameLog.PrintLog('this.m_WebSocket.readyState = ', this.m_WebSocket.readyState);
        }
    }

    private fillData(data, byteNum): string
    {

        if(typeof(data) !== "string"){
            data += "";
        }

        let tempStr: string = '';
        let len: number = byteNum - data.length;

        for(let i:number = 0;i < len; i ++)
        {
            tempStr += '0';
        }

        tempStr += data;

        return tempStr;
    }

    
    //获得字符串的字节数
    private getByteNum(str: string): number
    {
          let str_encode = escape(str),
              cnt = 0,
              len = str_encode.length;
    
          for( let i = 0; i < len; i++)
          {
              if( str_encode.charAt(i) == "%")
              {
                  if( str_encode.charAt(i+1) == "u" )
                  {
                      let value = parseInt(str_encode.substr(i+2,4),16);
                      if( value < 0x0800)
                      {
                          cnt += 2;
                      }
                      else
                      {
                          cnt += 3;
                      }
                      i = i+5;
                  }
                  else
                  {
                      cnt++;
                      i = i+2;
                  }
              }
              else
              {
                  cnt++;
              }
          }
        return cnt;
    }

    private isJSON(str: string): boolean
    {
        if (typeof str == 'string') {
            try {
                let obj = JSON.parse(str);
                if(typeof obj == 'object' && obj ){
                    return obj;
                }else{
                    return null;
                }
    
            } catch(e) {
                GameLog.PrintLog("isJSON:", "json字符串错误，不能解析:", str);
                return null;
            }
        }
    }

    //网络连接超时
    private connectTimeout(): void
    {
        if(this.m_IsConnected)
        {
            return;
        }

        GameLog.PrintLog("网络连接超时");

        this.Reconnect();

        // GameUI.ShowLoadingFailView();
    }
}
