import BeaconManager from '../manager/BeaconManager';
import EngineUtil from '../common/EngineUtil';
import EventCenter from '../common/EventCenter';
import GameData from '../data/GameData';
import GameLog from '../Log/LogTool';
import GameLogic from '../GameLogic';
import GameScene from './GameScene';
import GlobalVar from '../GlobalVar';
import MapInfo from '../MapInfo';
import PlayerData from '../data/PlayerData';
import PlayerMgr from '../manager/PlayerMgr';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import { GameState } from '../GameDefine';
import { MessageType } from '../common/EventCenter';
import { TeamType } from '../GameDefine';
import GameUI from '../manager/UIManager';
import { UIType } from '../UI/UIDefine';

const {ccclass, property} = cc._decorator;

@ccclass
export default class DQZScene extends GameScene
{
    private beaconManager: BeaconManager    = null; //玩家指向标

    // use this for initialization
    public OnInit() 
    {
        super.OnInit();

        this.uiLayer = EngineUtil.GetComponent(this.node.getChildByName("UILayer"), "DQZUILayer");

        this.beaconManager = cc.find('GameMap/BeaconNode').getComponent("BeaconManager");
        this.beaconManager.init();

        //初始化游戏临时数据
        GameData.InitTempData();

        Socket_SendMgr.GetBettleSend().Send5V5JoinGameMessage();
    }
    
    //进入游戏场景调用
    public OnEnter()
    {
        super.OnEnter();

        this.createAllPlayer();

        //开始倒计时
        this.startCountDown(this.doStart.bind(this));

        //镜头缩小
        this.cameraLensMagnify();
    }
    
    // called every frame
    protected FrameOnMove(dt: number) 
    {
        super.FrameOnMove(dt);
    }
    
    protected OnExit()
    {
        super.OnExit();

        GameData.ClearNetData();

        if(this.beaconManager)
        {
            this.beaconManager.clearAll();
            this.beaconManager = null;
        }
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

        this.scheduleOnce(function()
        {
            self.showOverLayer();
            self.cameraShrink();
            GameLogic.GetInstance().GetCarManager().clearAll();
            GameLogic.GetInstance().SetState(GameState.GS_Over);
        }, 1);
    }

    protected gameStartedUpdate(dt)
    {
        super.gameStartedUpdate(dt);

        //显示屏幕外的玩家的指向标
        this.beaconManager.frameOnMove(dt);
    }

    protected initEventListener()
    {
        super.initEventListener();

        let self = this;

        //有人复活
        this.registerEvent(MessageType.Player_Relive_Suc, function(playerID)
        {
            self.playerRelive(playerID);
        });

        //接受到有人死亡的消息
        this.registerEvent(MessageType.DLD_Receive_Death, function(data)
        {
            self.playerDeathResp(data);
        }); 

        //接受到战斗结束的消息
        this.registerEvent(MessageType.DLD_Receive_Bettle_End, function()
        {
            self.bettleOverResp();
        });
        
        //接受到有玩家进入游戏的消息
        this.registerEvent(MessageType.DLD_Player_Join_Game, function(uid)
        {
            self.playerJoinGameResp(uid);
        });

        //接受到同步玩家积分的消息
        this.registerEvent(MessageType.DLD_Receive_Syn_Score, function()
        {
            self.synScoreResp();
        });
    }


    //创建所有玩家
    private createAllPlayer()
    {
        //创建所有车
        let count: number = PlayerMgr.GetPlayerCount();
        for(let i: number = 0; i < count; i ++)
        {
            let player = PlayerMgr.GetPlayerByIndex(i);
            if(!player)continue;

            if(PlayerMgr.IsSelf(player.getUserID()))
            {
                this.createMyCar(GlobalVar.userID);
            }else
            {
                // if(!player.getIsJoinGame())continue;    //没有进入游戏的玩家不进行渲染

                this.createOtherCar(player.getUserID());
            }
        }
    }

    private createCarNode(id: number)
    {
        let carPrefab = EngineUtil.GetPrefab('CarNode');
        if(!carPrefab)return null;

        let carNode: cc.Node = cc.instantiate(carPrefab);
        carNode.userID = id;

        //设置出生位置和方向
        let playerData = PlayerMgr.GetPlayer(id);
        if(playerData)
        {
            //如果是蓝队的话就显示蓝底(夺旗战中使用)
            carNode.getChildByName("BlueImg").active = (playerData.getTeamType() === TeamType.TT_Blue);

            //设置出生方向
            let birthDirection = this.getBirthDiretion(playerData.getTeamType());
            playerData.setBirthDirection(birthDirection.x, birthDirection.y);

            //所有的出生点
            let BirthPointVec = MapInfo.GetCarPosVec();
            //位置编号
            let birthPointID = playerData.getBirthPointID();

            let pos = BirthPointVec[birthPointID];

            playerData.setPosition(pos);
        }
        
        //添加到地图节点上
        this.carParent.addChild(carNode);

        //初始化车
        let carCom = carNode.getComponent("CarCom");
        carCom.setID(id);
        carCom.initCarUI();

        //添加到管理器中
        GameLogic.GetInstance().GetCarManager().addCar(carNode);

        return carNode;
    }

    //创建我自己的车
    private createMyCar(id: number)
    {
        let carNode = this.createCarNode(id);
        if(!carNode)return;

        //设置自己为相机的跟随目标
        this.setCameraTarget(carNode);
    }

    //创建其他玩家的车
    private createOtherCar(id: number)
    {
        if(GameLogic.GetInstance().GetCarManager().getCarByID(id))return;

        let carNode = this.createCarNode(id);
        if(!carNode)return;

        this.beaconManager.addBeacon(id);

        // //其他车不需要检测是否离开地图，所有关掉碰撞组件
        // let circleColliderCom = carNode.getComponent(cc.CircleCollider);
        // circleColliderCom.enabled = false;
    }

    //显示结算界面
    private showOverLayer()
    {
        GameUI.ShowUI(UIType.UT_DQZOverView);
    }

    //有人复活
    private playerRelive(playerID: number)
    {
        if(PlayerMgr.IsSelf(playerID))
        {
            //倒计时UI
            this.startCountDown(function()
            {
                GameLogic.GetInstance().isCanInput = true;
            }, true);
        }
    }

    //有人死了
    protected playerDeath(playerID: number)
    {
        super.playerDeath(playerID);

        let playerData = PlayerMgr.GetPlayer(playerID);
        if(!playerData)return;

        if(PlayerMgr.IsSelf(playerID))
        {
            //死亡后禁止操作
            GameLogic.GetInstance().isCanInput = false;

            //FIXED: 玩家在被撞击后死亡的瞬间发出操作指令，车子会在复活321倒数时，并开始移动
            GameLogic.GetInstance().isSendDeath = true;
        }

        if(PlayerMgr.IsAlive(playerID))// if(PlayerMgr.IsSelf(playerID))
        {
            let killerID = playerData.getLastColliderCarID();

            //发送消息给服务器
            Socket_SendMgr.GetBettleSend().SendKillMessage(killerID, playerID);
        }
    }

    //根据队伍类型来返回出生方向
    private getBirthDiretion(teamType): cc.Vec2
    {
        if(teamType === TeamType.TT_Red)
        {
            return cc.v2(0, -1);
        }else if(teamType === TeamType.TT_Blue)
        {
            return cc.v2(0, 1);
        }
    }

    /**********************网络消息处理---Start*************************/

    //有玩家进入游戏
    private playerJoinGameResp(uid: number)
    {
        //渲染出该玩家
        this.createOtherCar(uid);
    }


    //收到同步积分的消息
    private synScoreResp()
    {
        //刷新UI
        this.uiLayer.refreshUI();
    }

    //收到有人死亡的消息
    private playerDeathResp(data)
    {
        let decedentID = data.power;    //死者
        let killerID = data.killerID;  //击杀者
        let goldNum = data.goldNum;  //奖励击杀者的金币
        let scoreNum = data.scoreNum;  //奖励击杀者的积分

        let playerData:PlayerData = PlayerMgr.GetPlayer(decedentID);

        if(PlayerMgr.IsSelf(decedentID))
        {
            //Fixed: 修复死亡瞬间操作的导致的问题
            this.scheduleOnce(function()
            {
                //直接发送复活 
                Socket_SendMgr.GetBettleSend().SendDQZReliveMessage(playerData.getBirthPointID());
            }, 0.1);
        }else
        {
            if(PlayerMgr.IsSelf(killerID))
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
}