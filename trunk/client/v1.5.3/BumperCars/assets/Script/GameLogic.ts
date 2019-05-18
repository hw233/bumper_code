import AudioManager from './common/AudioManager';
import CarCom from './component/CarCom';
import CarManager from './manager/CarManager';
import EventCenter from './common/EventCenter';
import GameConfig from './GameConfig';
import GameData from './data/GameData';
import GameLog from './Log/LogTool';
import GameTempData from './data/GameTempData';
import GlobalVar from './GlobalVar';
import PlayerData from './data/PlayerData';
import PlayerMgr from './manager/PlayerMgr';
import Util from './common/Util';
import WXHelper from './common/WXHelper';
import { CarTable } from './table/CarTable';
import CarUtil from './CarUtil';
import {
    GameState,
    PlayerState,
    UserCmdType
    } from './GameDefine';
import { GameModel } from './GameDefine';
import { MessageType } from './common/EventCenter';
import { SoundType } from './common/AudioManager';
import NumberUtil from './common/NumberUtil';

export default class GameLogic{
    
    private static instance: GameLogic = null; 

    private carManager: CarManager          = null;
    private physicsManager      = null;
    private colliderManager     = null;
    private recordKillTime      = null;
    private colliderTimeVec     = null; //记录当前碰撞的时间戳(处理辆车碰撞只调用一次的逻辑)
    private gameState           = null;        //游戏状态

    private convertNode: cc.Node= null;

    private m_Time: number      = null;

    //为调用者抛出用户操作的事件
    private onVkey: Function    = null;

    public isCanInput = null;

    public isSendDeath = null;

    constructor(){
    }

    Init(){
        this.gameState = 0;

        this.m_Time = 1.0 / 60.0;

        this.colliderTimeVec = [];
        this.recordKillTime = 0;

        this.isSendDeath = false;

        this.isCanInput = true;

        this.carManager = cc.find('Canvas').getComponent("CarManager");
        this.carManager.init();

        this.physicsManager = cc.find('Canvas').getComponent("PhysicsManager");
        this.physicsManager.init();

        this.colliderManager = cc.find('Canvas').getComponent("ColliderManager");
        this.colliderManager.init();

        this.convertNode = cc.find('GameMap/ConvertNode');
    };

    doStart () {
    };

    Reset(){
        this.colliderTimeVec.length = 0;
    };
    
    public SetListener(listener: Function): void
    {
        this.onVkey = listener;
    }

    public ReboundLogicForPhysics(collidePoint: cc.Vec2, aPlayerID: number, bPlayerID: number, ts: number = 0)
    {

        if(GlobalVar.gameModel !== GameModel.GM_PracticeMode)
        {
            return;
        }
                
        let otherPlayerData = PlayerMgr.GetPlayer(bPlayerID);

        let power = otherPlayerData.getImpactForce();
        let info = CarUtil.CalcReboundImpulse(collidePoint, power, aPlayerID, bPlayerID);
        if(!info)
        {
            GameLog.PrintError("ReboundLogic error info == null");
            return;
        }

        GameLog.PrintDebug("发生碰撞{", aPlayerID, ",", bPlayerID, '}---', Date.now());

        if(PlayerMgr.IsSelf(aPlayerID))
        {
            this.isCanInput = false;
        }

        let aNode: cc.Node = this.carManager.getCarByID(aPlayerID);
        if(!aNode)return;

        var selfBody = aNode.getComponent(cc.RigidBody);

        var vel = selfBody.linearVelocity;
        var desiredVel = info.impluse;// the vector speed you set
        var velChange = desiredVel.sub(vel);
        var impluse = velChange; //impluse = mv

        var carCom: CarCom = aNode.getComponent("CarCom");
        if(carCom)
        {
            carCom.rebound(impluse, info.type);
        }
    }

    //两车碰撞反弹的逻辑
    ReboundLogicForPVE(collidePoint: cc.Vec2, aPlayerID: number, bPlayerID: number, ts: number = 0): void
    {
        if(GlobalVar.gameModel !== GameModel.GM_PracticeMode)
        {
            return;
        }
                
        let otherPlayerData = PlayerMgr.GetPlayer(bPlayerID);

        let power = otherPlayerData.getImpactForce();
        let info = CarUtil.CalcReboundImpulse(collidePoint, power, aPlayerID, bPlayerID);
        if(!info)
        {
            GameLog.PrintError("ReboundLogic error info == null");
            return;
        }

        GameLog.PrintDebug("发生碰撞{", aPlayerID, ",", bPlayerID, '}---', Date.now());

        if(PlayerMgr.IsSelf(aPlayerID))
        {
            this.isCanInput = false;
        }

        let args = [];

        args.push(info.type);
        args.push(info.impluse.x);
        args.push(info.impluse.y);

        this.onVkey(aPlayerID, UserCmdType.UCT_Collide, args, ts);
    }

    //两车碰撞反弹的逻辑
    ReboundLogicForPVP(collidePoint: cc.Vec2, aPlayerID: number, bPlayerID: number, ts: number = 0): void
    {
        if(GlobalVar.gameModel === GameModel.GM_PracticeMode)
        {
            GameLog.PrintError("ReboundLogic error GlobalVar.gameModel === GameModel.GM_PracticeMode");
            return;
        }
                
        let selfPlayerData = PlayerMgr.GetPlayer(aPlayerID);
        let otherPlayerData = PlayerMgr.GetPlayer(bPlayerID);

        let aPower: number = otherPlayerData.getImpactForce();
        let aInfo = CarUtil.CalcReboundImpulse(collidePoint, aPower, aPlayerID, bPlayerID);
        if(!aInfo)
        {
            GameLog.PrintError("ReboundLogic error aInfo == null");
            return;
        }

        let bPower: number = selfPlayerData.getImpactForce();
        let bInfo = CarUtil.CalcReboundImpulse(collidePoint, bPower, bPlayerID, aPlayerID);
        if(!bInfo)
        {
            GameLog.PrintError("ReboundLogic error bInfo == null");
            return;
        }

        if( PlayerMgr.IsSelf(aPlayerID) ||  PlayerMgr.IsSelf(bPlayerID))
        {
            GameLog.PrintDebug("发生碰撞{", aPlayerID, ",", bPlayerID, '}---', Date.now());

            this.isCanInput = false;

            let args = [];

            args.push(aInfo.type);
            args.push(aInfo.impluse.x);
            args.push(aInfo.impluse.y);

            args.push(bPlayerID);

            args.push(bInfo.type);
            args.push(bInfo.impluse.x);
            args.push(bInfo.impluse.y);

            this.onVkey(aPlayerID, UserCmdType.UCT_Collide, args, ts);
        }
    }

    //碰撞前的逻辑
    BeforeColliderLogic(worldManifold, selfNode, otherNode): void
    {
        //碰撞点和法向量
        let points = worldManifold.points;

        let curTime = (new Date()).valueOf();
        let isFind = false;
        let isFirst = false;
        for(let i = 0;i < this.colliderTimeVec.length;i ++){
            let colliderTime = this.colliderTimeVec[i];
            if( (colliderTime.aNodeName == selfNode.uuid || colliderTime.aNodeName == otherNode.uuid) && (colliderTime.bNodeName == selfNode.uuid || colliderTime.bNodeName == otherNode.uuid) ){
                if(curTime - colliderTime.time >= 100){
                    isFirst = true;
                }
                // GameLog.PrintLog(curTime - colliderTime.time);
                isFind = true;
                colliderTime.time = curTime;
                break;
            }
        }

        if(isFind == false){
            this.colliderTimeVec.push({
                aNodeName: selfNode.uuid,
                bNodeName: otherNode.uuid,
                time: curTime
            });
        }

        if(!isFind || isFirst){
            //首次碰撞
            if(points.length > 0){
                this.FirstCollider(points[0], selfNode, otherNode);
            }
        }
    }

    //碰撞后的逻辑
    AfterColliderLogic(selfID: number, otherID: number): void
    {
        let selfPlayerData = PlayerMgr.GetPlayer(selfID);
        let otherPlayerData = PlayerMgr.GetPlayer(otherID);

        if(selfPlayerData && otherPlayerData)
        {
            selfPlayerData.setLastColliderCarID(otherPlayerData.getUserID());
            otherPlayerData.setLastColliderCarID(selfPlayerData.getUserID());
        }
    };

    //两车首次碰撞
    FirstCollider(worldPos: cc.Vec2, aNode: cc.Node, bNode: cc.Node): void
    {
        let aPlayerData: PlayerData = PlayerMgr.GetPlayer(aNode.userID);
        let bPlayerData: PlayerData = PlayerMgr.GetPlayer(bNode.userID);

        if(this.carManager.isSelf(aPlayerData.getUserID()) || this.carManager.isSelf(bPlayerData.getUserID())){
            WXHelper.Vibrate();
            AudioManager.PlaySoundEffect(SoundType.ST_Collider);
        }
        if(worldPos){
            EventCenter.DispatchEvent(MessageType.First_Collider, worldPos);
        }
    };

    //有车辆离开地图
    LeaveGameMap(userID: number): void
    {

        let playerData = PlayerMgr.GetPlayer(userID);

        if(!playerData)
        {
            return; 
        }

        //有人死了
        EventCenter.DispatchEvent(MessageType.Player_Death, userID);
    };

    //击杀别人之后的逻辑
    //killerID: 击杀者
    //decedentID: 死者
    KillLogic(killerID: number, decedentID: number, addGoldNum: number = 0, scoreNum: number = 0): number
    {

        let gameTempData: GameTempData = GameData.GetTempData();
        if(!gameTempData)return 0;

        //连杀记录
        if(gameTempData.GetContinueKillNum() == 0){
            gameTempData.SetContinueKillNum(1);
        }else{
            if(this.isContinueKill(killerID, decedentID)){
                gameTempData.AddContinueKillNum(1);
            }else{
                gameTempData.SetContinueKillNum(1);
            }
        }

        let continueKill = gameTempData.GetContinueKillNum();

        //添加金币
        if(addGoldNum === 0)
        {
            let bei = CarTable.ContinueKill[continueKill];

            if(bei){
                addGoldNum = bei * CarTable.KillGold;
            }else{
                addGoldNum = CarTable.KillGold;
            }
        }

        var playerData: PlayerData = PlayerMgr.GetPlayer(decedentID);

        gameTempData.AddCurGoldNum(addGoldNum);
        this.recordKillTime = 0;

        EventCenter.DispatchEvent(MessageType.Kill_Player);

        return addGoldNum;
    }

    //同步等级
    public DoSynLevel(playerID: number, freamID: number)
    {
        let playerData:PlayerData = PlayerMgr.GetPlayer(playerID);

        if(playerData)
        {
            let temp: number = NumberUtil.NumberAdd(1, CarTable.LevelVolume);
            let curLevel: number = playerData.getLevel();
            let initScale: number = playerData.getInitScale();
            let initRadius: number = playerData.getInitRadius();

            for(let i: number = 0; i < curLevel; i ++)
            {
                initScale = NumberUtil.NumberMul(initScale, temp);
            }

            let curRadius: number = NumberUtil.NumberMul(initRadius, initScale);

            playerData.setCurScale(initScale);
            playerData.setCurRadius(curRadius);

            GameLog.PrintLog("CarCom:synLevel:", curLevel);
        }
    }

    //移动逻辑
    public DoMove(dt: number, playerID: number, frameID: number, cmd: UserCmdType): void
    {
        let playerData: PlayerData = PlayerMgr.GetPlayer(playerID);

        if(playerData)
        {
            let distance: cc.Vec2 = cc.v2();
            let speed: number = playerData.getSpeed();
    
            if(cmd === UserCmdType.UCT_Move || cmd === UserCmdType.UCT_Null)
            {
                let linearSpeed: cc.Vec2 = playerData.getCurLinearVeloctity();

                distance.x = NumberUtil.NumberMul(linearSpeed.x, speed);
                distance.y = NumberUtil.NumberMul(linearSpeed.y, speed);
            }
            else if(cmd === UserCmdType.UCT_Rebound)
            {
                playerData.getColliderVec(distance);
            }
    
            playerData.doMove(distance, frameID);
        }
    }

    //只检测自己与其他车辆的碰撞
    CheckCollideForSelf()
    {
        let selfID: number = GlobalVar.userID;

        if(PlayerMgr.IsValid(selfID))
        {
            this.CheckCollideForPlayerID(selfID);
        }
    }

    //只检测指定玩家与其他车辆的碰撞
    CheckCollideForPlayerID(playerID: number, ts: number = 0, isQuickRun = false)
    {
        let playerList = PlayerMgr.GetPlayerUserIDList();
        if(playerList.length > 0 && PlayerMgr.IsActive(playerID))
        {
            let aPlayerData: PlayerData = PlayerMgr.GetPlayer(playerID);

            GameLog.PrintDebug(playerID, ":检测碰撞: aPlayerData.m_CollidePlayer = ", aPlayerData.m_CollidePlayer);

            let aRadius = aPlayerData.getCurRadius();
            let aPosition = aPlayerData.getPosition();

            for( let i: number = 0, len: number = playerList.length; i < len; i ++)
            {
                let bPlayerID: number = playerList[i];

                if( playerID !== bPlayerID && PlayerMgr.IsActive(bPlayerID) )
                {
                    let bPlayerData: PlayerData = PlayerMgr.GetPlayer(bPlayerID);

                    if( aPlayerData.m_CollidePlayer !== bPlayerID /* && bPlayerData.m_CollidePlayer != playerID*/ )
                    {
                        let bRadius = bPlayerData.getCurRadius();
                        let bPosition = bPlayerData.getPosition();

                        if( Util.GetDistance(aPosition, bPosition) <= (aRadius + bRadius) )
                        {
                            GameLog.PrintDebug(playerID, "和", bPlayerID, "发生碰撞");

                            let colliderPoint: cc.Vec2 = CarUtil.CalcCollidePoint(aPosition, aRadius, bPosition, bRadius);
                            colliderPoint = this.convertToWorldPos(colliderPoint);

                            aPlayerData.m_Colliding = GameConfig.ReboundFrameCount;
                            aPlayerData.m_CollidePlayer = bPlayerID;

                            bPlayerData.m_Colliding = GameConfig.ReboundFrameCount;
                            bPlayerData.m_CollidePlayer = playerID;

                            this.AfterColliderLogic(playerID, bPlayerID);

                            if(GlobalVar.gameModel === GameModel.GM_PracticeMode)
                            {
                                //播放碰撞音效
                                AudioManager.PlaySoundEffect(SoundType.ST_Collider);

                                this.ReboundLogicForPVE(colliderPoint, playerID, bPlayerID, ts);
                                this.ReboundLogicForPVE(colliderPoint, bPlayerID, playerID, ts);
                            }
                            else
                            {
                                if(isQuickRun === false)
                                {
                                    if(PlayerMgr.IsSelf(playerID))
                                    {
                                        //播放碰撞音效
                                        AudioManager.PlaySoundEffect(SoundType.ST_Collider);
                                        
                                        this.ReboundLogicForPVP(colliderPoint, playerID, bPlayerID, ts);
                                    }
                                    else if(PlayerMgr.IsSelf(bPlayerID))
                                    {
                                        //播放碰撞音效
                                        AudioManager.PlaySoundEffect(SoundType.ST_Collider);
    
                                        this.ReboundLogicForPVP(colliderPoint, bPlayerID, playerID, ts);
                                    }
                                }
                            }
                            EventCenter.DispatchEvent(MessageType.Collider_Event, colliderPoint);
                        }
                    }
                }
            }
        }
    }

    public CheckCollide(playerID: number = null, ts: number = 0, isQuickRun = false)
    {
        if( GameConfig.PhysicsSwitch && this.IsPracticeMode() )
        {
            return;
        }

        if(playerID)
        {
            this.CheckCollideForPlayerID(playerID, ts, isQuickRun);
        }
    }

    IsNeedShowOverlord(): boolean
    {
        return GlobalVar.gameModel === GameModel.GM_DLDMode;
    };

    SetState(state: GameState)
    {
        this.gameState = state;
    };

    IsRunning(): boolean
    {
        return this.gameState == GameState.GS_Running;
    };

    IsWaitOver(): boolean
    {
        return this.gameState == GameState.GS_WaitOver;
    };

    IsOver(): boolean
    {
        return this.gameState == GameState.GS_Over;
    };

    GetState(): GameState
    {
        return this.gameState;
    };

    //是否为练习模式
    IsPracticeMode(): boolean
    {
        return GlobalVar.gameModel === GameModel.GM_PracticeMode;
    }

    GetCarManager(): CarManager
    {
        return this.carManager;
    };

    SetIsHandleCollider(value: boolean): void
    {
    };

    FrameOnMove (dt: number) :void
    {
        this.recordKillTime += dt;
    }

    //如果当前处于无敌状态则退出无敌状态
    public ExitInvincible(playerID: number)
    {
        let playerData: PlayerData = PlayerMgr.GetPlayer(playerID);
        if(playerData)
        {
            //如果当前处于无敌状态则退出无敌状态
            if(playerData.getState() == PlayerState.PS_Invincible)
            {
                playerData.setState(PlayerState.PS_WaitRelive);

                let carNode: cc.Node = this.carManager.getCarByID(playerID);
                if(carNode)
                {
                    let carCom: CarCom = carNode.getComponent("CarCom");
                    if(carCom)
                    {
                        carCom.exitInvincible();
                    }
                }
            }
        }
    }

    /**
     * 是否满足连杀条件
     * @param killerID 击杀者
     * @param decedentID 死者
     */
    private isContinueKill(killerID: number, decedentID: number): boolean
    {
        if(this.recordKillTime <= GameConfig.ContinueKillTime){
            if(GlobalVar.gameModel === GameModel.GM_DQZMode){
                var killerPlayerData: PlayerData = PlayerMgr.GetPlayer(killerID);
                var decedentPlayerData: PlayerData = PlayerMgr.GetPlayer(decedentID);
                if(!killerPlayerData || !decedentPlayerData || killerPlayerData.getTeamType() === decedentPlayerData.getTeamType()){
                    return false;
                }
            }
            return true;
        }
        return false;
    }

    private convertToWorldPos(gameMapPos: cc.Vec2)
    {
        if(this.convertNode)
        {
            this.convertNode.setPosition(gameMapPos);

            return this.convertNode.convertToWorldSpaceAR(cc.v2(0, 0));
        }
        return null;
    }

    public Destroy()
    {
        if(this.carManager)
        {
            this.carManager.clearAll();
            this.carManager = null;
        }
        if(this.colliderTimeVec)
        {
            this.colliderTimeVec.length = 0;
            this.colliderTimeVec = null;
        }

        this.physicsManager = null;
        this.colliderManager = null;
        this.convertNode = null;
        this.onVkey = null;

    }

    public static GetInstance(): GameLogic
    {
        if(this.instance == null){
            this.instance = new GameLogic();
        }
        return this.instance;
    }

    public static Destroy()
    {
        if(this.instance)
        {
            this.instance.Destroy();
            this.instance = null;
        }
    }
}
