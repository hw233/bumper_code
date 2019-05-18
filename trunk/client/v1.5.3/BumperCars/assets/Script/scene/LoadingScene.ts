import Account from '../manager/Account';
import AccountData from '../manager/AccountData';
import ConfigData from '../common/ConfigData';
import FriendSystem from '../Friend/FriendSystem';
import GameConfig from '../GameConfig';
import GameData from '../data/GameData';
import GameLocalDataMgr from '../manager/GameLocalDataMgr';
import GameLog from '../Log/LogTool';
import GameTimeCount from '../manager/GameTimeCount';
import GameUI from '../manager/UIManager';
import GlobalScript from '../GlobalScript';
import HandleMgr from '../network/socket/HandleMgr';
import Http_SendMgr from '../network/http/Http_SendManager';
import MessageMgr from '../network/socket/MessageMgr';
import SceneBase from './SceneBase';
import SceneMgr from '../manager/SceneMgr';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import SocketNetManager from '../network/socket/SocketNetManager';
import TaskSystem from '../Task/TaskSystem';
import Util from '../common/Util';
import WXHelper from '../common/WXHelper';
import { GameRes } from '../GameRes';
import { MessageType } from '../common/EventCenter';
import { SceneType } from '../manager/SceneMgr';
import GlobalVar from '../GlobalVar';
import GameTable from '../manager/TableManger';
import ItemSystem from '../Item/ItemSystem';
import GameJump from '../GameJump';

const {ccclass, property} = cc._decorator;

@ccclass
export default class LoadingScene extends SceneBase
{
    @property({type: cc.Node})
    private loadingBar: cc.Node = null

    @property({type: cc.Node})
    private click_hint: cc.Node = null

    @property({type: cc.Node})
    private bg: cc.Node = null

    //加载资源
    private loadIndex: number           = null;
    private isLoading: boolean          = null;
    private isLoadFinish: boolean       = null;
    private progress: number            = null;
    private comingLoadingRes: any       = null;

    public OnInit () 
    {
        super.OnInit();

        this.click_hint.active = false;

        this.registerEvent(MessageType.Login_Success, this.loginSuccess.bind(this));
    }

    public OnEnter()
    {
        super.OnEnter();

        this.comingLoadingRes = [];

        if(SceneMgr.GetNextSceneType() == SceneType.StartScene)
        {
            this.comingLoadingRes = GameRes.StartSceneRes;
            if(GlobalVar.IsLoadConfig === false)
            {
                GlobalVar.IsLoadConfig = true;

                this.comingLoadingRes = this.comingLoadingRes.concat(GameRes.TableRes);
            }
        }
        else if(SceneMgr.GetNextSceneType() == SceneType.GameScene)
        {
            this.comingLoadingRes = GameRes.GameSceneRes.concat(GameRes.PracticeSceneRes);
        }
        else if(SceneMgr.GetNextSceneType() == SceneType.DLDGameScene)
        {
            this.comingLoadingRes = GameRes.GameSceneRes.concat(GameRes.DLDSceneRes);
        }
        else if(SceneMgr.GetNextSceneType() == SceneType.ZDZGameScene)
        {
            this.comingLoadingRes = GameRes.GameSceneRes.concat(GameRes.DQZSceneRes);
        }

        this.loadIndex = 0;
        this.isLoading = false;
        this.isLoadFinish = false;

        this.progress = 0;

        this.loadRes();
    }

    protected OnExit()
    {
        super.OnExit();

        this.comingLoadingRes = null;
    }

    protected FrameOnMove(dt: number)
    {
        super.FrameOnMove(dt);

        //加载资源
        if(!this.isLoadFinish)
        {
            if(!this.isLoading)
            {
                this.loadRes();
            }

            if(this.loadingBar)
            {
                this.loadingBar.getComponent(cc.ProgressBar).progress = this.progress / 100;
            }
        }
    }

    private loadRes()
    {
        if(this.loadIndex >= this.comingLoadingRes.length)
        {
            this.isLoadFinish = true;

            this.loadFinish();

            return;
        }

        let temp = 100 / this.comingLoadingRes.length;
        
        let self = this;
        let type = null;
        let res = this.comingLoadingRes[this.loadIndex];
        let url = res;
        let isConfig = false;

        if(Util.IsObject(res))
        {
            if(res.type)
            {
                type = res.type;
            }
            url = res.url;
            isConfig = res.isConfig;
        }
        self.isLoading = true;

        // if(isConfig && CC_WECHATGAME)
        // {
        //     url = url + ".json";
        //     // url = "https://img.51weiwan.com/pengpengche/v" + GameConfig.GameVersion + "/" + url + ".json";
        //     cc.loader.load(url, function (err, result) 
        //     {
        //         if(err)
        //         {
        //             GameLog.PrintLog(err);
        //         }
        //         else
        //         {
        //             self.progress += temp;
        //             self.isLoading = false;
        //         }
        //     });
        // }
        // else
        {
            cc.loader.loadRes(url, type, function (err, texture) {
                if(err){
                    GameLog.PrintLog(err);
                }else{
                    self.progress += temp;
                    self.isLoading = false;
                }
            });
        }
        
        this.loadIndex ++;
    }

    //加载完成
    private loadFinish()
    {
        let self = this;

        if(SceneMgr.GetNextSceneType() === SceneType.StartScene)
        {
            this.loadingBar.active = false;
            this.showClickHint();

            if(ConfigData.IsInited === false)
            {
                //获取游戏配置
                Http_SendMgr.SendGetConfigMessage(function()
                {
                    self.initGame();

                    if(SocketNetManager.GetInstance().IsConnect() === false)
                    {
                        //连接服务器
                        SocketNetManager.GetInstance().Connect(ConfigData.ServerIP, ConfigData.ServerPort);
                    }

                    if(Account.GetInstance().OnAuth())
                    {

                    }
                    else
                    {
                        self.bg.on(cc.Node.EventType.TOUCH_END, self.btnClicked.bind(self));
                    }
                });
            }
        }
        else
        {
            SceneMgr.SwitchNextScene();
        }
    }

    private btnClicked()
    {
        let userid: number = GameLocalDataMgr.GetInstance().ReadUserID();
        let nickname: string = GameLocalDataMgr.GetInstance().ReadUserNickName();
        let headUrl: string = GameLocalDataMgr.GetInstance().ReadUserHeadUrl();

        Account.GetInstance().OnLogin(userid, nickname, headUrl);
    }

    //登陆成功
    private loginSuccess()
    {
        if(GlobalScript.EnterGame())
        {
            SceneMgr.SwitchNextScene();
        }
    }

    private showClickHint()
    {
        if(this.click_hint)
        {
            this.click_hint.active = true;
            this.click_hint.runAction(cc.repeatForever(cc.blink(0.5, 1)));
        }
    }

    private initGame()
    {
        //用户点击右上角菜单的转发触发的事件
        WXHelper.ShowShareButton();
        WXHelper.PassiveShare();

        //初始化logo信息
        GameLog.InitLog();
        //初始化网络消息管理类
        MessageMgr.InitMgr();
        //注册消息处理函数
        HandleMgr.InitMgr();
        //初始化发送消息管理类
        Socket_SendMgr.InitMgr();
        //初始化任务数据
        TaskSystem.GetInstance().Init();
        //初始化用户管理器
        Account.InitMgr();
        //初始化游戏数据
        GameData.Init();
        //初始化用户数据
        AccountData.GetInstance().Init();
        //初始化本地数据
        GameLocalDataMgr.GetInstance().Init();
        //是否首次进入游戏
        GameLocalDataMgr.GetInstance().ReadIsFirstEnter();
        //初始化好友系统
        FriendSystem.GetInstance().Init();
        //初始化道具系统
        ItemSystem.GetInstance().Init();
        //初始化游戏到计时
        GameTimeCount.Init();
        //初始化静态表
        GameTable.GetInstance().ReadAllTable();
        GameJump.Init();

        if(GameConfig.ServerEnable === false)
        {
            GameUI.ShowTestMessageView();
        }
    }
}

