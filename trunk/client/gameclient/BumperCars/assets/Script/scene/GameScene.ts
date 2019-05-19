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

import AudioManager from '../common/AudioManager';
import CameraControl from '../component/CameraControl';
import CarCom from '../component/CarCom';
import ColliderAnimPool from '../ColliderAnimPool';
import CountDownCom from '../component/CountDownCom';
import DeachAnimPool from '../DeachAnimPool';
import EngineUtil from '../common/EngineUtil';
import FSPManager from '../FSP/FSPManager';
import GameConfig from '../GameConfig';
import GameData from '../data/GameData';
import GameInput from '../GameInput';
import GameLogic from '../GameLogic';
import GlobalVar from '../GlobalVar';
import KillViewManager from '../KillViewManager';
import MapInfo from '../MapInfo';
import PlayerMgr from '../manager/PlayerMgr';
import PlayerOpData from '../PlayerOpData';
import SceneBase from './SceneBase';
import SceneMgr from '../manager/SceneMgr';
import StarHaloView from '../layer/StarHaloView';
import { CarTable } from '../table/CarTable';
import { GameModel, PlayerState, UserCmdType } from '../GameDefine';
import { MessageType } from '../common/EventCenter';
import { SceneType } from '../manager/SceneMgr';
import { SoundType } from '../common/AudioManager';

const {ccclass, property} = cc._decorator;

@ccclass
export default class GameScene extends SceneBase
{
    //ui层
    protected uiLayer = null;

    //碰撞动画节点池
    protected colliderAnimPool: ColliderAnimPool = null;

    //死亡动画节点池
    protected deathAnimPool: DeachAnimPool = null;

    //主角光环UI
    private starHalo: StarHaloView = null; 

    protected gameMap: cc.Node = null;

    protected camera: cc.Camera = null;

    protected colliderNode: cc.Node = null;

    protected carParent: cc.Node = null;

    protected otherParent: cc.Node = null;

    // use this for initialization
    public OnInit() 
    {
        super.OnInit();

        this.gameMap = cc.find("GameMap");
        this.camera = cc.find("Camera").getComponent(cc.Camera);
        this.colliderNode = this.gameMap.getChildByName("ColiderNode");
        this.carParent = this.gameMap.getChildByName("CarNode");
        this.otherParent = this.gameMap.getChildByName("OtherNode");

        MapInfo.Init(this.gameMap);

        this.colliderAnimPool = EngineUtil.GetComponent(this.node, "ColliderAnimPool");
        this.colliderAnimPool.init(this.otherParent);

        this.deathAnimPool = EngineUtil.GetComponent(this.node, "DeachAnimPool");
        this.deathAnimPool.init(this.gameMap);

        this.addStarHaloView();

        this.initEventListener();

        PlayerOpData.Init();

        GameLogic.GetInstance().Init();

        KillViewManager.GetInstance().Init();

        this.colliderAnimPool.init();
    }

    //进入游戏场景调用
    public OnEnter()
    {
        super.OnEnter();

        //监听用户输入
        GameInput.Init();
        GameInput.SetListener(this.onVKey.bind(this));

        //监听碰撞事件
        GameLogic.GetInstance().SetListener(this.onVKey.bind(this));
        
        //帧管理器 注册事件
        FSPManager.GetInstance().Start(GlobalVar.gameModel !== GameModel.GM_PracticeMode);

        //播放背景音乐
        AudioManager.PlayBackgroundSound();
    }

    //每帧都调用(游戏初始化后开始调用)
    protected FrameOnMove(dt: number) 
    {
        super.FrameOnMove(dt);

        FSPManager.GetInstance().FrameOnMove(dt);

        this.updateGame(dt);

        if (GameLogic.GetInstance().IsRunning() == false && GameLogic.GetInstance().IsWaitOver() == false) {
            return;
        }

        this.gameStartedUpdate(dt);
    }

    protected OnExit()
    {
        super.OnExit();

        PlayerMgr.Clear();

        GameData.ClearTempData();

        this.deathAnimPool.DestroyPool();

        this.colliderAnimPool.DestroyPool();

        this.starHalo.destory();

        MapInfo.Reset();

        PlayerOpData.Destroy();

        FSPManager.Destroy();

        GameLogic.Destroy();

        KillViewManager.Destroy();

        //停止背景音乐
        AudioManager.StopBackgroundSound();

        this.uiLayer = null;
        this.starHalo = null;
        this.colliderAnimPool = null;
        this.deathAnimPool = null;
        this.gameMap = null;
        this.camera = null;
        this.colliderNode = null;
        this.carParent = null;
        this.otherParent = null;
    }

    //开始游戏调用
    protected doStart()
    {
        GameInput.Start();
    }

    //游戏结束调用
    protected doOver(isSelf = false)
    {
        FSPManager.GetInstance().Over();

        //停止背景音乐
        AudioManager.StopBackgroundSound();
    }

    //每帧都调用(游戏开始后开始调用)
    protected gameStartedUpdate(dt)
    {
        
    }

    protected updateGame(dt: number)
    {
        let playerCount = PlayerMgr.GetPlayerCount();

        for (let i = 0; i < playerCount; i++) 
        {
            let player = PlayerMgr.GetPlayerByIndex(i);
            if (player) 
            {
                if (PlayerMgr.IsAlive(player.getUserID())) 
                {
                    let carNode = GameLogic.GetInstance().GetCarManager().getCarByID(player.getUserID());
                    if (!carNode) continue;

                    let CarCom: CarCom = carNode.getComponent("CarCom");
                    if (CarCom) 
                    {
                        CarCom.frameOnMove(dt);
                    }
                    let aiCom = carNode.getComponent("AICom");
                    if (aiCom) 
                    {
                        aiCom.frameOnMove(dt);
                    }
                } 
                else if (player.getState() === PlayerState.PS_WaitDeath) 
                {
                    let carNode = GameLogic.GetInstance().GetCarManager().getCarByID(player.getUserID());
                    if (!carNode) continue;

                    let CarCom: CarCom = carNode.getComponent("CarCom");
                    if (CarCom) 
                    {
                        CarCom.death();
                    }
                }
            }
        }

        if(this.starHalo)
        {
            this.starHalo.frameOnMove(dt);
        }

        GameLogic.GetInstance().FrameOnMove(dt);
    }

    protected initEventListener()
    {
        let self = this;

        this.registerEvent(MessageType.Leave_Map, function(userID){
            self.playerDeath(userID);
            // GameLogic.GetInstance().LeaveGameMap(carNode);
        });

        this.registerEvent(MessageType.Game_Over, function(){
            self.doOver(false);
        });

        this.registerEvent(MessageType.Kill_Player, function(data)
        {
            self.uiLayer.refreshUI();
        });

        this.registerEvent(MessageType.HitWeaknesses, function(id){
            self.playWeaknesses(id);
        });

        this.registerEvent(MessageType.First_Collider, function(pos){
            self.playColliderAnimate(pos);
        });

        this.registerEvent(MessageType.Player_Death, function(userID){
            // self.playerDeath(userID);
        });

        this.registerEvent(MessageType.Return_HomePage, function(){
            self.returnHomePage();
        });

        this.registerEvent(MessageType.Logic_FPS_End, function(){
            self.dispatchEvent(MessageType.Update_Camera_Pos);
        });

        this.registerEvent(MessageType.Collider_Event, function(pos){
            self.playColliderAnimate(pos);
        });

        this.registerEvent(MessageType.DLD_Reconnect_Suc, function(){
            self.reconnectSuc();
        });

        this.registerEvent(MessageType.Self_Level_Changed, function(curLevel: number){
            self.selfLevelChanged(curLevel);
        });

        //有玩家退出游戏
        this.registerEvent(MessageType.DLD_Player_Exit_Game, function(playerID: number){
            self.playerExitGameResp(playerID);
        });
    }

    //开始倒计时
    public startCountDown(callback, isShow = true)
    {
        let self = this;

        let countDownCom: CountDownCom = EngineUtil.GetComponent(this.node, "CountDownCom");
        if(countDownCom)
        {
            countDownCom.play(function()
            {
                if(callback)
                {
                    callback();
                }

                if(isShow)
                {
                    self.uiLayer.showCountDown(false);
                }
            }, isShow);
        }

        if(isShow)
        {
            this.uiLayer.showCountDown(true);
        }
    }

    //返回首页
    public returnHomePage()
    {
        SceneMgr.SwitchScene(SceneType.StartScene);
    }

    //设置相机的跟随目标
    public setCameraTarget(target)
    {
        if(this.camera)
        {
            let cameraCom: CameraControl = this.camera.getComponent("CameraControl");
            if(cameraCom)
            {
                cameraCom.setTarget(target);
            }
        }
    }

    //播放死亡动画
    public playDeaAnimate(userID)
    {
        let carNode = GameLogic.GetInstance().GetCarManager().getCarByID(userID);
        if(carNode)
        {
            let pos = carNode.getPosition();

            let deaNode: cc.Node = this.deathAnimPool.GetNode();
            if(deaNode)
            {
                AudioManager.PlaySoundEffect(SoundType.ST_LuoShui);

                deaNode.setPosition(pos);

                let sp_pumpkin = deaNode.getComponent(sp.Skeleton);

                sp_pumpkin.clearTracks();//清理所有播放队列的动画
                sp_pumpkin.setAnimation(0, "LUOSHUI", false);

                sp_pumpkin.setCompleteListener( ()=>{
                    this.deathAnimPool.PutNode(deaNode);
                } );
            }
        }
    }

    //播放击中弱点的动画
    public playWeaknesses(carID)
    {
        // let car = GameLogic.GetInstance().GetCarManager().getCarByID(carID);
        // if(car){
        //     let weaknesses = cc.instantiate(this.weaknessesParticle);
        //     if(weaknesses){
        //         car.addChild(weaknesses);
        //     }
        // }
    }

    //播放碰撞动画
    public playColliderAnimate(pos)
    {
        let self = this;

        if(self.colliderAnimPool)
        {
            let animNode = self.colliderAnimPool.GetNode();
            if(animNode)
            {
                let anim = animNode.getComponent(cc.Animation);
                if(anim)
                {
                    anim.play();
                    anim.on('finished', function()
                    {
                        self.colliderAnimPool.PutNode(animNode);
                    })
                }
                let nodePos = self.gameMap.convertToNodeSpaceAR(pos);

                animNode.setPosition(nodePos);
                animNode.zIndex = 100;
                // self.otherParent.addChild(animNode);
            }
        }
    }
    
    //播放击杀特效
    public playKillAnimate(killerID: number, decedentID: number, gold: number, score: number, killTimes: number)
    {
        if(PlayerMgr.IsValid(killerID) && PlayerMgr.IsValid(decedentID))
        {
            KillViewManager.GetInstance().PlayKillInfo(this.uiLayer.node, killerID, decedentID, gold, score, killTimes);
        }
    }

    //镜头放大
    public cameraLensMagnify()
    {
        let self = this;

        let temp_update = function(dt)
        {
            self.camera.zoomRatio += (dt * 0.25);
            if(self.camera.zoomRatio >= GameConfig.CameraZoomRatio)
            {
                this.unschedule(temp_update);
            }
        };

        this.schedule(temp_update, 0.016);
    }

    //镜头缩小
    public cameraShrink()
    {
        let self = this;

        let temp_update = function(dt)
        {
            self.camera.zoomRatio -= (dt * 0.25);
            if(self.camera.zoomRatio <= GameConfig.CameraMinZoomRation)
            {
                this.unschedule(temp_update);
            }
        };

        this.camera.node.setPosition(375, 667);

        this.schedule(temp_update, 0.016);
    }

    protected onVKey(playerID: number, cmd: UserCmdType, args: Array<any>, ts: number = 0): void 
    {
        let playerOpData: PlayerOpData = PlayerOpData.Get();
        playerOpData.setPlayerID(playerID);
        playerOpData.setCMD(cmd);
        playerOpData.copyArgs(args);
        playerOpData.setTimeStamp(ts);

        FSPManager.GetInstance().SendPlayerOp(cmd, playerOpData);

        PlayerOpData.Put(playerOpData);
    }
    
    //有人死了
    protected playerDeath(playerID)
    {
    }
    
    //收到玩家退出游戏的消息(按下返回按钮触发)
    playerExitGameResp(playerID: number)
    {
        GameLogic.GetInstance().GetCarManager().removeCar(playerID);
    }

    //自身等级改变
    private selfLevelChanged(curLevel: number)
    {
        return;

        let curZoomRatio = GameConfig.CameraZoomRatio;
        for(let i: number = 0;i < curLevel;i ++)
        {
            curZoomRatio /= (1 + CarTable.LevelVolume);
        }

        if(curZoomRatio < GameConfig.CameraMinZoomRation)
        {
            curZoomRatio = GameConfig.CameraMinZoomRation;
        }

        this.camera.zoomRatio = curZoomRatio;
    }

    //添加主角光环UI
    private addStarHaloView()
    {
        this.starHalo = new StarHaloView();
        this.starHalo.initView(this.carParent);
        this.starHalo.setStarID(GlobalVar.userID);
    }

    //重连成功的消息
    private reconnectSuc()
    {
        SceneMgr.SwitchScene(SceneType.StartScene);
    }
};