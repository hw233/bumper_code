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

import GameConfig from "../../GameConfig";
import GlobalVar from "../../GlobalVar";
import GameLog from "../../Log/LogTool";
import Http_NetManager from "./HttpNetManager";
import ConfigData from '../../common/ConfigData';
import CarUtil from '../../CarUtil';
import EventCenter, { MessageType } from '../../common/EventCenter';
import AccountData from '../../manager/AccountData';

export default class Http_SendMgr{
    //登陆
    public static SendLogin(code, iv, encryptedData, successCb){
        let request = {
            code: code,
            game_id: GameConfig.GameID,
            iv: iv,
            encryptedData: encryptedData,
        };

        GameLog.PrintLog("发送登陆的消息: ",request);

        let isRelogin = false;
        
        let onMessage = function(data)
        {
            successCb(data);
        }

        let onFail = function(data)
        {
            if(isRelogin === false) 
            {
                // isRelogin = true;
                Http_NetManager.sendMsg("GET", GameConfig.LoginAddr, request, onMessage);
            }
        }
        Http_NetManager.sendMsg("GET", GameConfig.LoginAddr, request, onMessage, onFail);
    }

    //发送获取配置的消息
    public static SendGetConfigMessage(callback)
    {
        GameLog.PrintLog("发送获取配置的消息");

        if(!CC_WECHATGAME)
        {
            var data = {
                ad_relive: 1,
                share: [],
                recharge: "500,1,5,70",
                comegift_gold: 500,
                comegift_star: 1,
                keep_star_count: 5,
                fxjl_probability: 70,
            };
    
            ConfigData.Init();
            ConfigData.Deserialization(data);

            callback();
            return;
        }

        let request = {
            game_id: GameConfig.GameID
        };

        let onMessage = function(res){
            if(res.errno == 0){
                ConfigData.Init();
                ConfigData.Deserialization(res.data);
                callback && callback();
            }
        }

        Http_NetManager.sendMsg("GET", GameConfig.GetConfigAddr, request, onMessage);
    }

    //发送获取游戏列表的消息
    public static SendGetGameListMessage(locationID, callback){
        let request = {
            game_id: GameConfig.GameID,
            location: locationID,
        };

        GameLog.PrintLog("发送获取游戏列表的消息:", request);

        let onMessage = function(res){
            if(res.errno == 0){
                callback(res.data);
            }
        }

        Http_NetManager.sendMsg("GET", GameConfig.GetGameList, request, onMessage);
    }


    //发送获取游戏广告的的消息
    public static SendGetGameAdMessage(gid, callback){
        GameLog.PrintLog("SendGetGameAdMessage:");

        let request = {
            gid: gid,
        };

        let onMessage = function(res){
            GameLog.PrintLog("SendGetGameAdMessage == onMessage", res);
            if(res.errno == 0){
                callback(res.data);
            }
        }

        Http_NetManager.sendMsg("Get", GameConfig.GetGameADAddr, request, onMessage);
    }

    //发送匹配玩家的消息
    public static SendMatchPlayerMessage(callback)
    {
        let request = {
            num: 10,
        };

        GameLog.PrintLog("发送匹配玩家的消息:", request);

        if(!CC_WECHATGAME){
            let playerVec = [];
            for(let i = 0;i < 10 ;i ++){
                playerVec.push({
                    nickname: "波风水门-" + (i + 1),
                    avatar_url: "aaa",
                });
            }
            callback(playerVec);
            return;
        }

        let onMessage = function(res)
        {
            if(res.errno == 0)
            {
                callback(res.data);
            }
        }

        Http_NetManager.sendMsg("GET", GameConfig.SendMatchPlayerAddr, request, onMessage);
    }
}