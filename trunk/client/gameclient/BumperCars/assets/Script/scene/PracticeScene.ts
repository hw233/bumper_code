import AccountData from '../manager/AccountData';
import AICom from '../component/AICom';
import CarCom from '../component/CarCom';
import EngineUtil from '../common/EngineUtil';
import EventCenter from '../common/EventCenter';
import FSPManager from '../FSP/FSPManager';
import GameConfig from '../GameConfig';
import GameData from '../data/GameData';
import GameLog from '../Log/LogTool';
import GameLogic from '../GameLogic';
import GameScene from './GameScene';
import GameUI from '../manager/UIManager';
import GlobalVar from '../GlobalVar';
import Http_SendMgr from '../network/http/Http_SendManager';
import MapInfo from '../MapInfo';
import PlayerData from '../data/PlayerData';
import PlayerMgr from '../manager/PlayerMgr';
import RandomMatch from '../RandomMatch';
import SceneMgr from '../manager/SceneMgr';
import Util from '../common/Util';
import { CarTable } from '../table/CarTable';
import { GameState, UserCmdType } from '../GameDefine';
import { MessageType } from '../common/EventCenter';
import { PlayerState } from '../GameDefine';
import { SceneType } from '../manager/SceneMgr';
import { UIType } from '../UI/UIDefine';
import ItemSystem from '../Item/ItemSystem';
import GameTable from '../manager/TableManger';
import TaskTableInfo from '../table/TaskTableInfo';
import { TableType } from '../table/TableDefine';

//练习模式---游戏场景
const {ccclass, property} = cc._decorator;

@ccclass
export default class PracticeScene extends GameScene
{
    public OnInit() 
    {
        super.OnInit();

        this.uiLayer = EngineUtil.GetComponent(this.node.getChildByName("UILayer"), "PracticeUILayer");

        //初始化玩家管理器
        PlayerMgr.InitMgr();

        //初始化帧管理器
        FSPManager.GetInstance().Init();

        //初始化游戏临时数据
        GameData.InitTempData();
    }

    //进入游戏场景调用
    public OnEnter()
    {
        super.OnEnter();

        this.uiLayer.init();

        this.startMatchPlayer();
    }
    
    protected FrameOnMove (dt: number) 
    {
        super.FrameOnMove(dt);
    }

    protected OnExit()
    {
        super.OnExit();
    }

    //开始游戏调用
    public doStart()
    {
        super.doStart();

        GameLogic.GetInstance().SetState(GameState.GS_Running);

        GameLogic.GetInstance().doStart();
        GameLogic.GetInstance().GetCarManager().doStart();

        //TODO
        // this.onVKey(GlobalVar.userID, UserCmdType.UCT_DLD_Syn_Level, [7]);
    }

    //游戏结束调用
    public doOver(isSelf)
    {
        super.doOver(isSelf);

        GameLog.PrintLog("GameOver");
        
        GameLogic.GetInstance().SetState(GameState.GS_WaitOver);

        let self = this;

        this.setCameraTarget(null);

        let rankNum = PlayerMgr.GetAcitivePlayer();
        if(isSelf)
        {
            GameData.GetTempData().SetRankNum(rankNum + 1);
            EventCenter.DispatchEvent(MessageType.Refresh_UI);
        }else
        {
            GameData.GetTempData().SetRankNum(rankNum);
        }

        this.scheduleOnce(function()
        {
            self.showGameOverView();
            self.cameraShrink();
            GameLogic.GetInstance().SetState(GameState.GS_Over);
            GameLogic.GetInstance().GetCarManager().clearAll();
        }, 1);
    }

    protected initEventListener()
    {
        super.initEventListener();

        let self = this;

        this.registerEvent(MessageType.Reset_Game, function()
        {
            self.resetGame();
        });

        this.registerEvent(MessageType.Refresh_UI, function()
        {
            self.uiLayer.refreshUI();
        });
    }

    //开始匹配玩家
    private startMatchPlayer()
    {
        let self = this;

        let generateUserID = GlobalVar.userID;

        let index = 0;

        let BirthPointVec = MapInfo.GetCarPosVec();
        Util.Shuffle(BirthPointVec);

        let itemID: number = AccountData.GetInstance().GetUseCarID();
        let motinItemID: number = AccountData.GetInstance().GetUsedMotionID();
        
        //先添加自己的数据到玩家管理器中
        PlayerMgr.AddPlayerByInfo({
            target_id: generateUserID, 
            nick_name: AccountData.GetInstance().GetNickName(), 
            head_picurl:"", 
            use_car: itemID,
            use_motion: motinItemID,
            region_id: index,
        });

        //创建自己的车
        this.createMyCar(generateUserID);

        let func = function(playerVec)
        {
            GameLog.PrintLog("匹配玩家成功", playerVec);

            for(let i = 0; i < CarTable.AINum; i ++)
            {
                let carID = GameTable.GetInstance().GetDataInfoByRandom(TableType.TTD_CarType).GetID();

                let motionID = GameTable.GetInstance().GetDataInfoByRandom(TableType.TTD_MotionType).GetID();

                ++ generateUserID;
                ++ index;

                PlayerMgr.AddPlayerByInfo({
                    target_id: generateUserID,
                    nick_name: playerVec[i].nickname,
                    head_picurl:playerVec[i].avatar_url,
                    use_car: carID,
                    use_motion: motionID,
                    region_id: index,
                });
            }

            index = 1;

            let matchCom = <RandomMatch>EngineUtil.GetComponent(self.node, "RandomMatch");
            if(matchCom)
            {
                matchCom.startMatch(CarTable.AINum, 2, 3, function(matchCount)
                {
                    let playerData: PlayerData = PlayerMgr.GetPlayerByIndex(index ++);
                    self.createAICar(playerData.getUserID());
                }, function()
                {
                    self.uiLayer.matchFinish();
                    self.matchFinish();
                });
            }
            self.uiLayer.startMatch();
        }

        //匹配玩家
        Http_SendMgr.SendMatchPlayerMessage(function(data)
        {
            func(data);
        });

        //刷新ui
        this.uiLayer.refreshUI();
    }

    //匹配完成
    private matchFinish()
    {
        this.startCountDown(this.doStart.bind(this), true);

        //镜头放大
        this.cameraLensMagnify();
    }

    //再来一局
    private resetGame()
    {
        SceneMgr.SwitchScene(SceneType.GameScene);
    }

    //添加物理相关的组件(GameConfig.PhysicsSwitch = true时有效)
    private addPhysicsComponent(carNode: cc.Node)
    {
        if(carNode)
        {
            //碰撞回调
            carNode.addComponent("Callback");

            //刚体
            let bodyCom: cc.RigidBody = carNode.addComponent(cc.RigidBody);
            if(bodyCom)
            {
                bodyCom.enabledContactListener = true;
                bodyCom.bullet = false;
                bodyCom.type = cc.RigidBodyType.Dynamic;
                bodyCom.fixedRotation = true;
            }

            //碰撞体
            let physicsCom: cc.PhysicsCircleCollider = carNode.addComponent("cc.PhysicsCircleCollider");
            if(physicsCom)
            {
                physicsCom.density = 0;
                physicsCom.sensor = false;
                physicsCom.friction = 0.2;
                physicsCom.restitution = 1;
                physicsCom.offset.x = 0;
                physicsCom.offset.y = -10;
                physicsCom.radius = 170;
            }
        }

    }

    private createCarNode(id: number) 
    {
        let carPrefab = EngineUtil.GetPrefab('CarNode');
        if (!carPrefab) return null;

        let playerData = PlayerMgr.GetPlayer(id);
        if(!playerData) return null;

        let carNode: cc.Node = cc.instantiate(carPrefab);
        if(!carNode) return null;

        carNode.userID = id;

        let BirthPointVec = MapInfo.GetCarPosVec();
        let BirthPointID = playerData.getBirthPointID();

        // TODO
        // if(id == (GlobalVar.userID + 1) )
        // {
        //     BirthPointID = 1;
        // }
        // else if(id == (GlobalVar.userID + 2) )
        // {
        //     BirthPointID = 2;
        // }

        if(GameConfig.PhysicsSwitch)
        {
            this.addPhysicsComponent(carNode);
        }

        //添加到地图节点上
        this.carParent.addChild(carNode);

        let carCom: CarCom = carNode.getComponent("CarCom");
        if (carCom) 
        {
            carCom.setID(id);
            carCom.initCarUI();
            carCom.initPos(BirthPointVec[BirthPointID]);
        }

        playerData.setPosition(BirthPointVec[BirthPointID]);

        //添加到管理器内
        GameLogic.GetInstance().GetCarManager().addCar(carNode);

        //更新剩余人数
        GameData.GetTempData().AddtRemainingNum(1);

        //刷新UI
        this.uiLayer.refreshUI();

        return carNode;
    }

    //创建AI车辆
    private createAICar(id: number)
    {

        let carNode: cc.Node = this.createCarNode(id);
        if(carNode)
        {
            let aiCom: AICom = carNode.addComponent("AICom");
            if(aiCom)
            {
                aiCom.setID(id);
                aiCom.setListener(this.onVKey.bind(this));
            }
        }
    }

    //创建我自己的车
    private createMyCar(id: number)
    {
        let carNode: cc.Node = this.createCarNode(id);

        //设置自己为相机的跟随目标
        this.setCameraTarget(carNode);
    }

    //有人死了
    protected playerDeath(userID)
    {
        super.playerDeath(userID);

        let playerData = PlayerMgr.GetPlayer(userID);
        if(!playerData)return;

        //播放死亡特效
        this.playDeaAnimate(userID);

        if(PlayerMgr.IsSelf(userID))
        {
            //玩家被撞出地图，游戏结束
            playerData.setState(PlayerState.PS_WaitDeath);
            this.doOver(true);
        }
        else
        {
            playerData.setState(PlayerState.PS_WaitDeath);

            let colliderID = playerData.getLastColliderCarID();
            let addGoldNum = 0;

            if(PlayerMgr.IsSelf(colliderID))
            {
                GameData.GetTempData().AddCurKillNum(1);
                addGoldNum = GameLogic.GetInstance().KillLogic(colliderID, userID);
            }
            else
            {
                addGoldNum = CarTable.KillGold;
            }

            let colliderPlayerData: PlayerData = PlayerMgr.GetPlayer(colliderID);
            if(colliderPlayerData)
            {
                let level: number = colliderPlayerData.getLevel() + playerData.getLevel();
                this.onVKey(colliderID, UserCmdType.UCT_DLD_Syn_Level, [level]);
            }

            if(PlayerMgr.GetAcitivePlayer() <= 1)
            {
                //游戏结束--赢了
                this.doOver(false);
            }

            //播放击杀动画
            this.playKillAnimate( colliderID, userID, addGoldNum, 0,  GameData.GetTempData().GetContinueKillNum() );
        }
        
        //更新剩余人数
        GameData.GetTempData().AddtRemainingNum(-1);

        //刷新UI
        this.uiLayer.refreshUI();
    }

    //打开结束界面
    private showGameOverView()
    {
        GameUI.ShowUI(UIType.UT_PracticeOverView);
    }
}
