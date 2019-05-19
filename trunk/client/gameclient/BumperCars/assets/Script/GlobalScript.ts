import WXHelper from "./common/WXHelper";
import GameLog from "./Log/LogTool";
import SceneMgr from "./manager/SceneMgr";
import {SceneType} from './manager/SceneMgr';
import GameData from "./data/GameData"
import GlobalVar from "./GlobalVar";
import Socket_SendMgr from './network/socket/Socket_SendManager';
import FSPManager from './FSP/FSPManager';
import ComponentBase from './common/ComponentBase';
import { MessageType } from './common/EventCenter';
import { GameModel } from './GameDefine';
import GameUI from './manager/UIManager';
import MessageHandler from './network/socket/MessageHandler';
import OnlineRewardTime from './OnlineRewardTime';
import EventCenter from './common/EventCenter';

const {ccclass, property} = cc._decorator;

//全局脚本
@ccclass
export default class GlobalScript extends ComponentBase
{
    private static launchParam = null;

    protected onLoad () 
    {
        GlobalScript.launchParam = WXHelper.GetLaunchOptionsSync();

        GameLog.PrintLog(GlobalScript.launchParam);

        this.registerEvent(MessageType.DLD_Receive_Join, this.inviteJoinResp.bind(this));
        this.registerEvent(MessageType.DLD_Receive_Join_Fail, this.invateJoinFailResp.bind(this));
    }

    protected start () 
    {
        this.initEvent();
    }

    //注册生命周期函数
    public initEvent()
    {
        if(WXHelper.IsWXContext())
        {
            WXHelper.OnShow(this.onShow.bind(this));
            WXHelper.OnHide(this.onHide.bind(this));
        }else
        {
            let self = this;
            let hiddenProperty = 'hidden' in document ? 'hidden' : 'webkitHidden' in document ? 'webkitHidden' : 'mozHidden' in document ? 'mozHidden' : null;
            let visibilityChangeEvent = hiddenProperty.replace(/hidden/i, 'visibilitychange');
            let onVisibilityChange = function()
            {
                if (document[hiddenProperty]) 
                {    
                    self.onHide();
                }else
                {
                    self.onShow(null);
                }
            }
            document.addEventListener(visibilityChangeEvent, onVisibilityChange);
            window.onbeforeunload =this.exitMiniProgram.bind(this);
        }
    }

    //回到游戏前台事件
    private onShow(res)
    {
        GameLog.PrintLog("onShow:", Date.now(), res);

        GlobalScript.checkEnterStyle(res);

        if(SceneMgr.IsGameScene())
        {
            FSPManager.GetInstance().setIsQuickRun(false);
        }

        EventCenter.DispatchEvent(MessageType.Game_On_Show_Event);
    }

    //游戏进入后台事件
    private onHide()
    {
        GameLog.PrintLog("onHide:", Date.now());

        if(SceneMgr.IsGameScene())
        {
            GameData.Save();

            FSPManager.GetInstance().setIsQuickRun(true);
        }

        EventCenter.DispatchEvent(MessageType.Game_On_Hide_Event);
    }

    //退出游戏事件
    private exitMiniProgram()
    {
        GameLog.PrintLog("exitMiniProgram");

        if(SceneMgr.IsGameScene())
        {
            GameData.Save();
        }
    }

    //点击邀请链接加入房间的消息
    private inviteJoinResp(model)
    {
        GlobalVar.gameModel = model;

        if(model == GameModel.GM_DLDMode)
        {
            SceneMgr.SwitchScene(SceneType.DLDRoomScene);
        }
        else if(model == GameModel.GM_DQZMode)
        {
            SceneMgr.SwitchScene(SceneType.ZDZRoomScene);
        }
    }
    
    //邀请加入失败
    private invateJoinFailResp()
    {
        GameUI.ShowHint("加入失败, 房间不存在");

        setTimeout(function(){
            SceneMgr.SwitchNextScene();
        }, 1000);
    }

        
    //进入小游戏
    public static EnterGame()
    {
        let need = true
        if(this.launchParam)
        {
            need = this.checkEnterStyle(this.launchParam);
            this.launchParam = null;
        }
        return need
    }

    //检查进入小游戏的方式 做相应的逻辑
    public static checkEnterStyle(param)
    {
        if(!param){
            return true;
        }

        if(param.query.enterStyle == "ranklist")
        {
            //enterStyle=ranklist
            //如果是从查看群排行的链接点击进来的，则显示排行榜

            GlobalVar.shareTicket = param.shareTicket;
            GlobalVar.isOpenGroupRank = true;

            if(SceneMgr.GetCurSceneType() != SceneType.StartScene)
            {
                SceneMgr.SwitchScene(SceneType.StartScene);
                return false;
            }else
            {
                return true;
            }
        }
        else if(param.query.enterStyle == "invate")
        {
            //从好友的分享链接进来的
            let shareUID = param.query.uid;
            
            Socket_SendMgr.GetCommonSend().SendUserShare(shareUID);

            return true;
        }
        else if(param.query.enterStyle == "invateJoin")
        {
            //enterStyle=invateJoin&roomID=2&model=1
            //从邀请加入房间的链接进来的
            let roomID: number = parseInt(param.query.roomID);
            let model: number = parseInt(param.query.model);

            Socket_SendMgr.GetBettleSend().SendInviteJoinRoomMessage(roomID, model);
            return false;
        }
        else if(param.scene == 1103 || param.scene == 1104)
        {
            //从我的小程序进入
            Socket_SendMgr.GetActionSend().SendAddToAppListMessage();
        }
        return true;
    }

    protected update(dt)
    {
        //处理网络消息
        MessageHandler.FrameOnMove(dt);

        //在线奖励计时
        OnlineRewardTime.FrameOnMove(dt);
    }
}