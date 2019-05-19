import BeaconManager from '../manager/BeaconManager';
import CarCom from '../component/CarCom';
import EnemyHaloView from '../layer/EnemyHaloView';
import EngineUtil from '../common/EngineUtil';
import EventCenter from '../common/EventCenter';
import GameData from '../data/GameData';
import GameInput from '../GameInput';
import GameLog from '../Log/LogTool';
import GameLogic from '../GameLogic';
import GameScene from './GameScene';
import GlobalVar from '../GlobalVar';
import OreLogic from '../OreLogic';
import MapInfo from '../MapInfo';
import OverlordHaloView from '../layer/OverlordHalo';
import PlayerMgr from '../manager/PlayerMgr';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import { GameState } from '../GameDefine';
import { MessageType } from '../common/EventCenter';
import GameUI from '../manager/UIManager';
import { UIType } from '../UI/UIDefine';

const
    { ccclass, property } = cc._decorator;

//大乱斗模式---游戏场景
@ccclass
export default class DLDScene extends GameScene 
{
    private beaconManager: BeaconManager    = null; //玩家指向标

    private oreLogic: OreLogic            = null; //道具逻辑

    private overlordHalo: OverlordHaloView  = null; //霸主光环UI

    private enemyHalo: EnemyHaloView        = null; //仇人光环UI

    // use this for initialization
    public OnInit() 
    {
        super.OnInit();

        this.uiLayer = EngineUtil.GetComponent(this.node.getChildByName("UILayer"), "DLDUILayer");

        this.oreLogic = cc.find('GameMap/OreNode').getComponent("OreLogic");
        this.oreLogic.init();

        this.beaconManager = cc.find('GameMap/BeaconNode').getComponent("BeaconManager");
        this.beaconManager.init();

        this.overlordHalo = new OverlordHaloView();
        this.overlordHalo.initView(this.carParent);

        this.enemyHalo = new EnemyHaloView();
        this.enemyHalo.initView(this.carParent);

        //初始化游戏临时数据
        GameData.InitTempData();

        Socket_SendMgr.GetBettleSend().SendDLDJoinGameMessage();
    }
    
    //进入游戏场景调用
    public OnEnter() 
    {
        super.OnEnter();

        //创建所有玩家
        this.createAllPlayer();

        //开始倒计时
        this.startCountDown(this.doStart.bind(this));

        //镜头缩小
        this.cameraLensMagnify();
    }

    protected OnExit() 
    {
        super.OnExit();

        if(this.beaconManager)
        {
            this.beaconManager.clearAll();
            this.beaconManager = null;
        }

        if(this.oreLogic)
        {
            this.oreLogic = null;
        }

        if(this.overlordHalo)
        {
            this.overlordHalo.destory();
            this.overlordHalo = null;
        }

        if(this.enemyHalo)
        {
            this.enemyHalo.destory();
            this.enemyHalo = null;
        }

        GameData.ClearNetData();
    }

    
    //开始游戏调用
    protected doStart() 
    {
        super.doStart();

        this.uiLayer.doStart();

        GameLogic.GetInstance().SetState(GameState.GS_Running);

        GameLogic.GetInstance().doStart();
        GameLogic.GetInstance().GetCarManager().doStart();
    }

    //游戏结束调用
    protected doOver() 
    {
        super.doOver();

        GameLog.PrintLog("GameOver");

        GameLogic.GetInstance().SetState(GameState.GS_WaitOver);

        let self = this;

        this.setCameraTarget(null);

        this.uiLayer.doOver();

        //停止游戏时间的刷新
        EventCenter.DispatchEvent(MessageType.Stop_Game_Time_Count);

        this.scheduleOnce(function () {
            self.showOverLayer();
            self.cameraShrink();
            GameLogic.GetInstance().GetCarManager().clearAll();
            GameLogic.GetInstance().SetState(GameState.GS_Over);
        }, 1);
    }

    protected initEventListener() 
    {
        super.initEventListener();

        let self = this;

        //有人复活
        this.registerEvent(MessageType.Player_Relive_Suc, function(playerID){
            self.playerRelive(playerID);
        });

        //接受到有人死亡的消息
        this.registerEvent(MessageType.DLD_Receive_Death, function (data) {
            self.playerDeathResp(data);
        });

        //接受到战斗结束的消息
        this.registerEvent(MessageType.DLD_Receive_Bettle_End, function () {
            self.bettleOverResp();
        });

        //接受到同步玩家积分的消息
        this.registerEvent(MessageType.DLD_Receive_Syn_Score, function () {
            self.synScoreResp();
        });

        //接受到有玩家进入游戏的消息
        this.registerEvent(MessageType.DLD_Player_Join_Game, function (uid) {
            self.playerJoinGameResp(uid);
        });
    }

    protected gameStartedUpdate(dt)
    {
        super.gameStartedUpdate(dt);

        //更新道具逻辑
        this.oreLogic.frameOnMove(dt);

        //显示屏幕外的玩家的指向标
        this.beaconManager.frameOnMove(dt);
    }

    protected updateGame(dt: number)
    {
        super.updateGame(dt);

        //更新霸主光环
        if(this.overlordHalo)
        {
            this.overlordHalo.frameOnMove(dt);
        }

        //更新仇人光环
        if(this.enemyHalo)
        {
            this.enemyHalo.frameOnMove(dt);
        }
    }

    //创建所有玩家
    private createAllPlayer() 
    {

        //创建所有车
        let count: number = PlayerMgr.GetPlayerCount();

        for (let i: number = 0; i < count; i++) {
            let player = PlayerMgr.GetPlayerByIndex(i);
            if (!player) continue;

            if (PlayerMgr.IsSelf(player.getUserID())) 
            {
                this.createMyCar(player.getUserID());
            } else 
            {
                if (!player.getIsJoinGame()) continue;    //没有进入游戏的玩家不进行渲染

                this.createOtherCar(player.getUserID());
            }
        }
    }

    private createCarNode(id: number) 
    {
        let carPrefab = EngineUtil.GetPrefab('CarNode');
        if (!carPrefab) 
        {
            GameLog.PrintLog("createCarNode:EngineUtil.GetPrefab('CarNode') == null");
            return null;
        }

        let playerData = PlayerMgr.GetPlayer(id);
        if(!playerData)
        {
            GameLog.PrintLog("createCarNode:PlayerMgr.GetPlayer(id) == null");
            return null;
        } 

        let carNode = cc.instantiate(carPrefab);

        carNode.userID = id;

        let BirthPointVec = MapInfo.GetCarPosVec();
        let BirthPointID = playerData.getBirthPointID();

        // BirthPointID %= 2;
        // TODO
        // if(id == 820462)
        // {
        //     BirthPointID = 0;
        //     playerData.setBirthDirection(1, 0);
        // }
        // if(id == 820518)
        // {
        //     BirthPointID = 1;
        //     playerData.setBirthDirection(-1, 0);
        // }

        //添加到地图节点上
        this.carParent.addChild(carNode);

        let carCom: CarCom = carNode.getComponent("CarCom");
        if (carCom) {
            carCom.setID(id);
            carCom.initCarUI();
        }

        playerData.setPosition(BirthPointVec[BirthPointID]);

        //添加到管理器内
        GameLogic.GetInstance().GetCarManager().addCar(carNode);

        return carNode;
    }

    //创建我自己的车
    private createMyCar(id: number)
    {
        let carNode = this.createCarNode(id);
        if (!carNode) return;

        GameLog.PrintLog("创建自己的车！")

        //设置自己为相机的跟随目标
        this.setCameraTarget(carNode);
    }

    //创建其他玩家的车
    private createOtherCar(id: number)
    {
        if (GameLogic.GetInstance().GetCarManager().getCarByID(id)) return;

        let carNode = this.createCarNode(id);
        if (!carNode) return;

        GameLog.PrintLog("创建其他的车！")

        this.beaconManager.addBeacon(id);

        // //其他车不需要检测是否离开地图，所有关掉碰撞组件
        // let circleColliderCom = carNode.getComponent(cc.CircleCollider);
        // circleColliderCom.enabled = false;
    }

    /**
     * 弹出复活弹窗(只有自己死亡才会触发)
     * @param killerID 击杀者的id
     */
    private showReliveView(killerID: number) 
    {
        let param: any = {};

        let enemyPlayer = PlayerMgr.GetPlayer(killerID);
        if(enemyPlayer)
        {
            //他杀
            param.killType = 2;
            param.nickName = enemyPlayer.getNickName();
            param.headUrl = enemyPlayer.getAvatarUrl();
        }
        else
        {
            //自杀
            let selfData = PlayerMgr.GetPlayer(GlobalVar.userID);

            param.killType = 1;
            param.nickName = selfData.getNickName();
            param.headUrl = selfData.getAvatarUrl();
        }

        GameUI.ShowUI(UIType.UT_ReliveView, param);
    }

    //显示结算界面
    private showOverLayer() 
    {
        GameUI.ShowUI(UIType.UT_DLDOverView);
    }

    //有人复活
    private playerRelive(playerID: number)
    {
        if (PlayerMgr.IsSelf(playerID)) 
        {
            //自己复活 隐藏复活弹窗
            GameUI.HideUI(UIType.UT_ReliveView);

            //倒计时UI
            this.startCountDown(function(){
                GameLogic.GetInstance().isCanInput = true;
            }, true);
        }

        let enemyID = GameData.GetBettleData().getEnemyUserID();
        if(this.enemyHalo)
        {
            this.enemyHalo.refreshEnemyID(enemyID);
        }
    }

    //有人死了
    protected playerDeath(playerID) 
    {
        super.playerDeath(playerID);

        let playerData = PlayerMgr.GetPlayer(playerID);
        if (!playerData) return;

        // if (PlayerMgr.IsSelf(playerID)) 
        if(PlayerMgr.IsAlive(playerID))
        {
            let killerID = playerData.getLastColliderCarID();

            //发送消息给服务器
            Socket_SendMgr.GetBettleSend().SendKillMessage(killerID, playerID);
        }
    }

    /**********************网络消息处理---Start*************************/

    //有玩家进入游戏
    private playerJoinGameResp(uid) 
    {
        //渲染出该玩家
        this.createOtherCar(uid);
    }

    //收到同步积分的消息
    private synScoreResp() 
    {
        let overlordID: number = GameData.GetBettleData().getOverlordUserID();
        if(this.overlordHalo)
        {
            this.overlordHalo.refreshOwnerID(overlordID);
        }
        //刷新排行榜
        this.uiLayer.refreshUI();
    }

    //收到有人死亡的消息
    private  playerDeathResp(data) 
    {
        let self = this;

        let decedentID = data.power;    //死者
        let killerID = data.killerID;  //击杀者
        let goldNum = data.goldNum;  //奖励击杀者的金币
        let scoreNum = data.scoreNum;  //奖励击杀者的积分

        if (PlayerMgr.IsSelf(decedentID)) 
        {
            //显示复活界面
            self.showReliveView(killerID);
        } else 
        {
            if (PlayerMgr.IsSelf(killerID)) 
            {
                GameData.GetTempData().AddCurKillNum(1);
                GameLogic.GetInstance().KillLogic(killerID, decedentID, goldNum, scoreNum);
            }
        }
        
        //播放死亡特效
        this.playDeaAnimate(decedentID);

        //播放击杀动画
        this.playKillAnimate( killerID, decedentID, goldNum, scoreNum,  GameData.GetTempData().GetContinueKillNum() );
    }

    //收到战斗结束的消息
    private bettleOverResp() 
    {
        this.doOver();
    }
    /**********************网络消息处理---End*************************/
}