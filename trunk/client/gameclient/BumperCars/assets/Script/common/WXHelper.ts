import {ShareSceneType} from "./ShareSceneType";
import {SubMessage}     from "./SubMessage";
import {UserBtnConfig}  from "./UserBtnConfig";

import GameLog          from "../Log/LogTool";
import GlobalVar        from "../GlobalVar";
import GameConfig       from "../GameConfig";
import ConfigData from './ConfigData';
import { ShareConfig } from './ConfigData';
import { GameModel } from '../GameDefine';

export default class WXHelper{

    private static userInfoBtn  = null;

    private static clubBtn      = null;

    public static IsWXContext(): boolean
    {
        return CC_WECHATGAME;
    }

    //返回小程序的启动参数
    public static GetLaunchOptionsSync(): void
    {
        if(this.IsWXContext())
        {
            return wx.getLaunchOptionsSync();
        }
        return null;
    }

    //监听小游戏返回到前台的事件
    public static OnShow(callback: Function): void
    {
        if(this.IsWXContext())
        {
            wx.onShow(callback);
        }
    }

    //监听小游戏进入后台的事件
    public static OnHide(callback: Function): void
    {
        if(this.IsWXContext())
        {
            wx.onHide(callback);
        }
    }

    //监听小游戏退出的事件
    public static ExitMiniProgram(obj: Object = null): void
    {
        if(this.IsWXContext()){
            wx.exitMiniProgram({
                success(){
                    GameLog.PrintLog("ExitMiniProgram:success");
                    // callback();
                },
                fail(){
                    GameLog.PrintLog("ExitMiniProgram:fail");
                },
                complete(){
                    GameLog.PrintLog("ExitMiniProgram:complete");
                },

            });
        }
    }

    public static Login(callback: Function): void
    {
        let self = this;

        if(this.IsWXContext() == false)
        {
            return;
        }

        let success = function(res)
        {
            GameLog.PrintLog("login success:resCode = " + res.code);
            callback(res.code);
        };
        let fail = function()
        {
            GameLog.PrintLog("login fail");
        };
        
        wx.login({
            success: success,
            fail: fail
        });
    }

    public static GetUserInfo(callback: Function)
    {
        if(this.IsWXContext() == false)
        {
            return;
        }

        wx.getUserInfo({
            success: function (res) {
              // 保存微信登录参数
              if(callback)
              {
                callback(res);
              }
            }
        });
    }


    //振动
    public static Vibrate(): void
    {
        if(this.IsWXContext() == false)
        {
            return;
        }

        if(GlobalVar.OpenVibration === false)
        {
            return;
        }

        wx.vibrateShort({
            success(){
                // GameLog.PrintLog("播放振动成功");
            },
            fail(){
                // GameLog.PrintLog("播放振动失败");
            },
        });
    }

    //查看群排行的分享
    public static GroupRankListShare(): void
    {
        GameLog.PrintLog("GroupRankListShare");

        if(WXHelper.IsWXContext() == false){
            return;
        }

        let shareConfig: ShareConfig = ConfigData.ShareConfig[ShareSceneType.GroupRankList];

        if(!shareConfig){
            return;
        }

        wx.shareAppMessage({
            title: shareConfig.Title,
            imageUrl: shareConfig.Img,
            query: "enterStyle=ranklist&uid=" + GlobalVar.userID,
            success(res){
                GameLog.PrintLog("分享成功");
            },
            fail(res){
                GameLog.PrintLog("分享失败",res);
                WXHelper.ShowToast("分享失败");
            }
        });
    }

    //邀请好友加入房间的分享
    public static InviteJoinShare(teamID, model, callback = null): void
    {
        GameLog.PrintLog("邀请好友加入房间");

        if(WXHelper.IsWXContext() == false){
            return;
        }
 
        let shareConfig: ShareConfig = null;

        if(model === GameModel.GM_DLDMode)
        {
            shareConfig = ConfigData.ShareConfig[ShareSceneType.DLDInvateJoin];
        }
        else if(model === GameModel.GM_DQZMode)
        {
            shareConfig = ConfigData.ShareConfig[ShareSceneType.ZDZInvateJoin];
        }

        if(!shareConfig){
            return;
        }

        wx.shareAppMessage({
            title: shareConfig.Title,
            imageUrl: shareConfig.Img,
            query: "enterStyle=invateJoin&uid=" + GlobalVar.userID + "&roomID=" + teamID + "&model=" + model,
            success(res){
                GameLog.PrintLog("分享成功");
                if(callback){
                    callback(res);
                }
            },
            fail(res){
                GameLog.PrintLog("分享失败",res);
                WXHelper.ShowToast("分享失败");
            }
        });
    }

    //求助好友的分享
    public static ForHelpFriendShare(callback = null): void
    {
        GameLog.PrintLog("ForHelpFriendShare");

        if(WXHelper.IsWXContext() == false)
        {
            return;
        }
 
        let shareConfig: ShareConfig = ConfigData.ShareConfig[ShareSceneType.ForHelpFriend];

        if(!shareConfig)
        {
            return;
        }

        wx.shareAppMessage({
            title: shareConfig.Title,
            imageUrl: shareConfig.Img,
            query: "enterStyle=invate&uid=" + GlobalVar.userID,
            success(res){
                GameLog.PrintLog("分享成功");
                if(callback){
                    callback(res);
                }
            },
            fail(res){
                GameLog.PrintLog("分享失败",res);
                WXHelper.ShowToast("分享失败");
            }
        });
    }
    
    //显示转发按钮
    public static ShowShareButton(): void
    {
        if(this.IsWXContext())
        {
            //设置分享按钮，方便获取群id展示群排行榜
            wx.showShareMenu({withShareTicket: true});
        }
    }

    //被动分享
    public static PassiveShare(): void
    {
        GameLog.PrintLog("PassiveShare");

        if(WXHelper.IsWXContext() == false)
        {
            return;
        }

        let shareConfig: ShareConfig = ConfigData.ShareConfig[ShareSceneType.PassiveShare];

        if(!shareConfig)
        {
            return;
        }

        wx.onShareAppMessage(function()
        {
            return {
                title: shareConfig.Title,
                imageUrl: shareConfig.Img,
            };
        });
    }

    //普通分享
    public static CommonShare(sceneType: ShareSceneType, callback = null, failCallback = null): void
    {
        GameLog.PrintLog("CommonShare");

        if(WXHelper.IsWXContext() == false){
            return;
        }

        let shareConfig: ShareConfig = ConfigData.ShareConfig[sceneType];

        if(!shareConfig){
            return;
        }

        wx.shareAppMessage({
            title: shareConfig.Title,
            imageUrl: shareConfig.Img,
            success(res){
                GameLog.PrintLog("分享成功", res);

                let isgroup = false;
                if(res.shareTickets == null || res.shareTickets == undefined || res.shareTickets == ""){ 
                    GameLog.PrintLog("分享给个人");
                    
                }else{
                    GameLog.PrintLog("分享到群");
                    isgroup = true;
                }

                if(callback){
                    callback(res, isgroup);
                }
            },
            fail(res){
                GameLog.PrintLog("分享失败",res);
                WXHelper.ShowToast("分享失败");
                if(failCallback){
                    failCallback();
                }
            }
        });
    }

    // //截屏分享
    // public static ScreenShotsShare(callback){
    //     GameLog.PrintLog("Share");

    //     if(WXHelper.IsWXContext() == false){
    //         return;
    //     }

    //     let canvas = cc.game.canvas;
    //     let width  = cc.winSize.width;
    //     let height  = cc.winSize.height;
    //     canvas.toTempFilePath({
    //         x: 0,
    //         y: 0,
    //         width: width,
    //         height: height,
    //         destWidth: 500,
    //         destHeight: 400,
    //         success (res) {
    //             if (window.wx != undefined) {
    //                 window.wx.shareAppMessage({
    //                     title: GameConfig.ShareText,
    //                     imageUrl: res.tempFilePath,
    //                     success(res){
    //                         GameLog.PrintLog("分享成功");
    //                         callback(res);
    //                     },
    //                     fail(res){
    //                         GameLog.PrintLog("分享失败",res);
    //                         WXHelper.ShowToast(GameConfig.Share_Text2);
    //                     }
    //                 });
    //             }
    //         }
    //     })
    // }


    public static UploadUserCloudStorage(data): void
    {
        GameLog.PrintLog("UploadUserCloudStorage" + data);
        
        if (this.IsWXContext()) {
            wx.setUserCloudStorage({  
                KVDataList: data,
                success (res) {  
                    GameLog.PrintLog('setUserCloudStorage','success',res)  
                },  
                fail (res) {  
                    GameLog.PrintLog('setUserCloudStorage','fail')  
                },  
                complete (res) {  
                    GameLog.PrintLog('setUserCloudStorage','ok')  
                }  
            });
        }
    }

    //展示好友排行榜
    public static FetchFriendRankList(rankType: number): void
    {
        if (this.IsWXContext()) 
        {
            WXHelper.InitSharedCanvas();

            wx.postMessage({
                messageType: SubMessage.MT_FriendRankList,
                rankType: rankType,
            });

            GameLog.PrintLog("发送消息到子域!--->>>展示好友排行榜");
        }
    }

    //展示游戏结束界面排行榜
    public static GameOverRankList(): void
    {
        if(this.IsWXContext()){
            // WXHelper.InitSharedCanvas();

            // wx.postMessage({
            //     messageType: SubMessage.MT_OverRankList,
            //     curLevel: GlobalVar.curLevel,
            // });
            // GameLog.PrintLog("发送消息到子域!--->>>展示游戏结束界面排行榜");
        }
    }

    //展示即将超越的玩家
    public static JJCYPlayer(): void
    {
        if(this.IsWXContext())
        {
        }
    }

    //刷新即将超越的玩家
    public static RefreshJJCYPlayer(): void
    {
        if(this.IsWXContext())
        {
        }
    }

    //展示群排行榜
    public static GroupRankList(rankType: number): void
    {
        if (this.IsWXContext()) 
        {
            WXHelper.InitSharedCanvas();

            wx.postMessage({
                messageType: SubMessage.MT_GroupRankList,
                groupShareTicket: GlobalVar.shareTicket,
                rankType: rankType,
            });

            GameLog.PrintLog("发送消息到子域!--->>>展示群排行榜");
        }
    }

    public static InitSharedCanvas(): void
    {
        if (this.IsWXContext()) 
        {
            let openDataContext = wx.getOpenDataContext();
            let sharedCanvas = openDataContext.canvas;
            if (sharedCanvas) 
            {
                sharedCanvas.width = cc.game.canvas.width * 2;
                sharedCanvas.height = cc.game.canvas.height * 2;
            }
        }
    }

    public static ClearSharedCanvas(): void
    {
        if (this.IsWXContext()) {
            // let openDataContext = wx.getOpenDataContext();
            // let sharedCanvas = openDataContext.canvas;
            // if (sharedCanvas) {
            //     sharedCanvas.width = 0;
            //     sharedCanvas.height = 0;
            // }
            wx.postMessage({
                messageType: SubMessage.MT_Clear
            });

            GameLog.PrintLog("发送消息到子域!--->>>清理子域");
        }
    }

    public static CreateUserInfoButton(callback): void
    {
        let self = this;

        if(WXHelper.IsWXContext() == false){
            return;
        }

        if(window["SystemInfo"]){
            let modelWidth = window["SystemInfo"].windowWidth;
            let modelHeight = window["SystemInfo"].windowHeight;
            let buttonWidth = UserBtnConfig.width * (modelWidth / (cc.winSize.width));
            let buttonHeight = UserBtnConfig.height * (modelHeight / (cc.winSize.height));
            let buttonLeft = UserBtnConfig.left;
            let buttonTop = UserBtnConfig.top;

            let button = wx.createUserInfoButton({
                type: 'image ',
                image: UserBtnConfig.imgSrc,
                style: {
                    left: buttonLeft,
                    top: buttonTop,
                    width: buttonWidth,
                    height: buttonHeight,
                }
            });

            button.onTap((res) => {
                callback(res);
            });

            self.userInfoBtn = button;

            GameLog.PrintLog("创建授权按钮");
        }
    }

    public static DestroyUserInfoBtn(): void
    {
        if(this.userInfoBtn)
        {
            this.userInfoBtn.destroy();
            this.userInfoBtn = null;
        }
    }

    //隐藏用户信息按钮
    public static HideUserInfoBtn(): void
    {
        if(this.userInfoBtn)
        {
            this.userInfoBtn.hide();
        }
    }

    //显示用户信息按钮
    public static ShowUserInfoBtn(): void
    {
        if(this.userInfoBtn)
        {
            this.userInfoBtn.show();
        }
    }
    
    public static GetSystemInfo(): void
    {
        //调用微信接口 获取设备信息
        if(window["SystemInfo"])
        {
            return;
        }
        if (WXHelper.IsWXContext()) {
            window["SystemInfo"] = wx.getSystemInfoSync();
        }
    }

    //更多游戏
    public static MoreGame(): void
    {
        if (WXHelper.IsWXContext()) 
        {
            wx.navigateToMiniProgram({
                appId: GameConfig.AppID,
                path: '',
                envVersion: 'release',
                success () {
                    GameLog.PrintLog("跳转到更多游戏成功");
                },
                fail (res) {
                    GameLog.PrintLog("跳转到更多游戏失败", res);
                },
            });
        }
    }

    public static ShowToast(title: string): void
    {
        if(this.IsWXContext())
        {
            wx.showToast({
                title: title,
                icon: 'none',
            });
        }
    }
}
