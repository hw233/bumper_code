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

import WXHelper from '../common/WXHelper';
import GameLog from '../Log/LogTool';
import AudioManager from '../common/AudioManager';
import Http_SendMgr from '../network/http/Http_SendManager';
import Util from '../common/Util';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import GlobalVar from '../GlobalVar';
import GameLocalDataMgr from './GameLocalDataMgr';

//处理用户的登陆和授权
export default class Account 
{
    private static instance: Account = null;

    private constructor()
    {

    }

    public static GetInstance()
    {
        if(!this.instance)
        {
            this.instance = new Account();
        }
        return this.instance;
    }

    public static InitMgr()
    {
        
    }

    //授权
    public OnAuth()
    {
        if(this.isNeedAuthorize())
        {
            let self = this;

            if(WXHelper.IsWXContext())
            {
                WXHelper.CreateUserInfoButton(function(res)
                {
                    GameLog.PrintDebug("点击授权按钮");

                    AudioManager.PlayBtnSound();

                    if(res && res.iv && res.encryptedData)
                    {
                        WXHelper.Login(function(resCode)
                        {
                            if(resCode)
                            {
                                //FIXED: 偶尔出现请求己方服务器授权登录失败,原因在于请求顺序，这里的请求顺序是先请求 wx.getUserInfo(按钮) 再请求 wx.login()。有时候获取code后iv已经失效，所以失败。解决方案：在请求 wx.login() 后，调用一次 wx.getUserInfo(废弃接口) ，更新iv等信息。注意：这里调用的废弃接口只是不再弹出授权请求窗口，但还是能够获取到 iv 等信息，授权接口弹窗已将在点击按钮的时候弹出，并且授权了，所以没毛病。
                                WXHelper.GetUserInfo(function(res2)
                                {
                                    Http_SendMgr.SendLogin(resCode, res2.iv, res2.encryptedData, function(resData)
                                    {
                                        let uid: number = parseInt(resData.data.uid);
                                        let nickName: string = resData.data.nickName + "";
                                        let avatarUrl: string = resData.data.avatarUrl;
                                        //保存到本地
                                        GameLocalDataMgr.GetInstance().SaveUserID(uid);
                                        GameLocalDataMgr.GetInstance().SaveUserNickName(nickName);
                                        GameLocalDataMgr.GetInstance().SaveUserHeadUrl(avatarUrl);
                                        //授权成功后，直接登陆，不用再次点击界面
                                        self.OnLogin(uid, nickName, avatarUrl);
                                    });
                                });
                            }
                            else
                            {
                                GameLog.PrintLog("code 获取失败!");
                            }
                        });
                    }
                });
                return true;
            }
            else
            {
                let userID: number = Util.RandomNum(1, 10);
                let nickname: string = userID.toString();
                let headUrl: string = "aaa";

                GameLocalDataMgr.GetInstance().SaveUserID(userID);
                GameLocalDataMgr.GetInstance().SaveUserNickName(nickname);
                GameLocalDataMgr.GetInstance().SaveUserHeadUrl(headUrl);

                return false;
            }
        }
        else
        {
            return false;
        }
    }

    //登陆
    public OnLogin(userid: number, nickname: string, avatarUrl: string)
    {
        //TODO 电脑测试
        if(WXHelper.IsWXContext() === false)
        {
            userid = Util.RandomNum(1000, 100000);//window["Game"]["uid"];//
            if(window["Game"]["uid"] > 0)
            {
                userid = window["Game"]["uid"];
            }
        }

        GlobalVar.userID = userid;

        //获取用户信息成功，销毁微信的按钮
        WXHelper.DestroyUserInfoBtn();

        Socket_SendMgr.GetLoginSend().SendLoginMessage(userid, nickname, avatarUrl);
    }

    //是否需要授权
    private isNeedAuthorize(): boolean
    {
        let userid: number = GameLocalDataMgr.GetInstance().ReadUserID();
        let nickname: string = GameLocalDataMgr.GetInstance().ReadUserNickName();
        let headUrl: string = GameLocalDataMgr.GetInstance().ReadUserHeadUrl();

        if(userid && nickname && headUrl)
        {
            return false;
        }

        return true;
    }
}
