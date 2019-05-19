import WXHelper from "../common/WXHelper";
import GameLog from "../Log/LogTool";

import {ShareSceneType} from "../common/ShareSceneType";
import GameLocalDataMgr from '../manager/GameLocalDataMgr';

export enum AdvertiseType{
    ADConfig_Min    = 0,

    ADConfig_Banner,

    ADConfig_Video,

    ADConfig_Video_JiaYou,

    ADConfig_Video_Unlock_Car,

    ADConfig_Video_Keep_Star,

    ADConfig_Max,
};

let AdvertiseConfig = {};
AdvertiseConfig[AdvertiseType.ADConfig_Banner] = {
    description: "游戏底部",

    adUnitID: "adunit-763ae0fb183c878e",
    
    show: {
        left: 0.5,
        bottom: 0,
        width: 100,
    },
};

AdvertiseConfig[AdvertiseType.ADConfig_Video] = {
    description: "三倍金币视频广告",

    adUnitID: "adunit-15fbba7c18a4e286",
};

AdvertiseConfig[AdvertiseType.ADConfig_Video_JiaYou] = {
    description: "加油礼包视频广告",

    adUnitID: "adunit-57a4ac1cac62b726",
};

AdvertiseConfig[AdvertiseType.ADConfig_Video_Unlock_Car] = {
    description: "解锁车子视频广告",

    adUnitID: "adunit-24ba2ed38bb6ffeb",
};

AdvertiseConfig[AdvertiseType.ADConfig_Video_Keep_Star] = {
    description: "保留星星视频广告",

    adUnitID: "adunit-a9ac385deb028e72",
};

export default class AdvertiseMgr{

    private static bannerAd = null;

    public static CreateBannerAd(type: AdvertiseType){
        if(!CC_WECHATGAME){
            return;
        }

        if(type <= AdvertiseType.ADConfig_Min || type >= AdvertiseType.ADConfig_Max){
            return;
        }

        let config = AdvertiseConfig[type];

        GameLog.PrintLog("CreateBannerAd------------------------------------");

        let modelWidth = window["SystemInfo"].windowWidth;
        let modelHeight = window["SystemInfo"].windowHeight;

        let bannerAd = wx.createBannerAd({
            adUnitId: config.adUnitID,
            style: {
                left: 0,
                top: 0,
                width: modelWidth,
            }
        });

        let self = this;

        bannerAd.onResize(res => {
            let buttonTop = config.show.top;
            let buttonLeft = modelWidth / 2 - bannerAd.style.realWidth * 0.5;

            if(config.show.top == undefined){
                buttonTop = modelHeight - bannerAd.style.realHeight;
            }
            bannerAd.style.left = buttonLeft;
            bannerAd.style.top = buttonTop;
        });
        bannerAd.show();

        bannerAd.onError(err => {
            GameLog.PrintLog(err);
        });

        this.bannerAd = bannerAd;
    }

    public static DestroyBannerAd(){
        if(this.bannerAd){
            this.bannerAd.hide();
            this.bannerAd.destroy();
        }
    }

    public static CreateVideoAd(type: AdvertiseType, callback: Function, shareType:ShareSceneType = null){
        if(!CC_WECHATGAME){
            callback(true);
            return;
        }

        if(type <= AdvertiseType.ADConfig_Min || type >= AdvertiseType.ADConfig_Max){
            return;
        }

        let config = AdvertiseConfig[type];

        // if(shareType && GameLocalDataMgr.GetInstance().GetWatchVideoCount() >= 5){
        //     WXHelper.CommonShare(shareType, function(){
        //         callback(true);
        //     });
        //     return;
        // }

        let videoAd = wx.createRewardedVideoAd({
            adUnitId: config.adUnitID,
        });

        videoAd.load().then(() => {
            videoAd.show();
        }).catch(err => {
            GameLog.PrintLog(err.errMsg)
        });

        let closeCallback = function(res){
            GameLog.PrintLog(res);

            videoAd.offClose(closeCallback);

            // 用户点击了【关闭广告】按钮
            // 小于 2.1.0 的基础库版本，res 是一个 undefined
            let isFinish = false;

            if (res && res.isEnded || res === undefined) {
              // 正常播放结束，可以下发游戏奖励
                isFinish = true;
                //记录观看视频次数
                GameLocalDataMgr.GetInstance().SetWatchVideoCount(GameLocalDataMgr.GetInstance().GetWatchVideoCount() + 1);
            }
            else {
                // 播放中途退出，不下发游戏奖励
                isFinish = false;
            }

            callback(isFinish);
        };

        videoAd.onClose(closeCallback);

        var errorCallback = function(err){
            videoAd.offError(errorCallback);
            console.log(err);
        };

        videoAd.onError(errorCallback);
    }
};