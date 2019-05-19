
import WXHelper from "../common/WXHelper";
import EngineUtil from "../common/EngineUtil";
import SceneMgr from "../manager/SceneMgr";
import GameLog from "../Log/LogTool";
import GameConfig from "../GameConfig";
import GameUI from "../manager/UIManager";
import Http_SendMgr from "../network/http/Http_SendManager";
import {SceneType} from "../manager/SceneMgr";
import GlobalVar from "../GlobalVar";
import SceneBase from './SceneBase';

const {ccclass, property} = cc._decorator;

@ccclass
export default class LogoScene extends SceneBase
{
    //巨网logo
    @property({type: cc.Node})
    private jwLogo= null        

    @property({type: cc.Node})
    private gameAdBg= null

    public OnInit () 
    {
        //获取设备信息
        WXHelper.GetSystemInfo();
        //初始化UI管理类
        GameUI.Init();
        //设置公共UI根节点
        GameUI.SetRootNode(cc.find("GlobalNode"));
        //初始化场景管理器
        SceneMgr.Init();

        this.getLaunchOptionsSync();
    }

    //获取启动参数
    private getLaunchOptionsSync() 
    {
        let self = this;

        if (WXHelper.IsWXContext()) 
        {
            
            wx.onShow(res => 
                {
                    //监听小游戏回到前台的事件
                    GameLog.PrintLog('onshow', res.query);
                    if (res.query.game_id == GameConfig.GameID) 
                    {
                        //跳到自己的游戏
                        GameLog.PrintLog("[loading] 游戏返回到前台的参数中  跳转到自己的游戏中");
                        self.showJWLogo();
                    }
                }
            );

            let wx_onLaunch = wx.getLaunchOptionsSync();

            GameLog.PrintLog(wx_onLaunch);

            let gid = wx_onLaunch.query.gid;

            if (gid) 
            {
                GameLog.PrintLog("开始走非正常流程,gid= ", gid);
                GlobalVar.GameAdGID = gid;
                this.jwLogo.active = false;
                this.getGameAd(gid);
                return;
            }
        }

        //显示巨网logo
        this.showJWLogo();
    }

    //获取游戏广告
    private getGameAd(gid) 
    {
        let self = this;
        if (WXHelper.IsWXContext()) 
        {
            Http_SendMgr.SendGetGameAdMessage(gid, function(data)
            {
                GlobalVar.GameAdInfo = data;
                let bgUrl = data.img;
                EngineUtil.LoadRemoteImg(self.gameAdBg, bgUrl);
                self.gameAdBg.active = true;
                self.gameAdBg.on(cc.Node.EventType.TOUCH_END,self.logoClicked.bind(self));
            });
        }
    }

    //落地页点击
    private logoClicked()
    {
        if (!GlobalVar.GameAdInfo) 
        {
            return
        }

        let self = this;

        let status = GlobalVar.GameAdInfo.state;

        let ToMiniProgram:any = 
        {
            envVersion: "release", //'develop',//'trial',
            success: function () 
            {
                GameLog.PrintLog("跳转成功", GlobalVar.GameAdGID);
                self.showJWLogo();
            },
            fail: function (res) 
            {
                GameLog.PrintLog("跳转失败", res);
                self.showJWLogo();
            },
        };

        if (status == 1) 
        {
            //审核中  跳到游戏
            if(GlobalVar.GameAdInfo.appp_id == GameConfig.AppID)
            {
                // self.showJWLogo();
                return;
            }
            ToMiniProgram.appId = GlobalVar.GameAdInfo.app_id;
            ToMiniProgram.extraData = GlobalVar.GameAdInfo.param;
            ToMiniProgram.path = GlobalVar.GameAdInfo.path;
            GameLog.PrintLog("跳转到游戏中");
        } else if (status == 2) 
        {//通过审核 跳盒子
            GameLog.PrintLog("跳转到盒子中");
            ToMiniProgram.appId = GlobalVar.GameAdInfo.hz_app_id;
            ToMiniProgram.path = GlobalVar.GameAdInfo.hz_path;
            ToMiniProgram.extraData = {gid: GlobalVar.GameAdGID};
        }
        if (WXHelper.IsWXContext()) 
        {
            wx.navigateToMiniProgram(ToMiniProgram);
        }
    }

    private showJWLogo()
    {
        let self = this;
        if(this.jwLogo)
        {
            this.jwLogo.active = true;

            self.scheduleOnce(self.juLogoFinish.bind(self), GameConfig.JWLogoDuration);
        }
        if(this.gameAdBg)
        {
            this.gameAdBg.active = false;
        }
    }

    private juLogoFinish()
    {
        SceneMgr.SwitchScene(SceneType.StartScene);
    }
}
