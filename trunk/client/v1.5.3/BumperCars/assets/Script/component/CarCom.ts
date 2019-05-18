import CarUtil from '../CarUtil';
import ComponentBase from '../common/ComponentBase';
import CustomMotion from '../MotionStreak/CustomMotion';
import EngineUtil, { AtlasType } from '../common/EngineUtil';
import GameConfig from '../GameConfig';
import GameLog from '../Log/LogTool';
import GameLogic from '../GameLogic';
import MotionStreakBase from '../MotionStreak/MotionBase';
import MyMotionStreak from '../MotionStreak/MyMotionStreak';
import NumberUtil from '../common/NumberUtil';
import PlayerData from '../data/PlayerData';
import PlayerMgr from '../manager/PlayerMgr';
import Util from '../common/Util';
import { CarTable } from '../table/CarTable';
import { MessageType } from '../common/EventCenter';
import { MotionType, PlayerState } from '../GameDefine';
import MotionTableInfo from '../table/MotionTableInfo';
import ItemSystem from '../Item/ItemSystem';
import CarTableInfo from '../table/CarTableInfo';
import GlobalVar from '../GlobalVar';

const {ccclass, property} = cc._decorator;

@ccclass
export default class CarCom extends ComponentBase{
    private m_UID:number                  = null;       //唯一标识
    private m_ReliveAnim:cc.Animation     = null;       //复活动画
    private m_PhysicsCollider             = null;       //物理碰撞体
    private m_Body: cc.RigidBody          = null;       //刚体

    private m_Streak: MotionStreakBase      = null;       //拖尾

    private m_LevelNode: cc.Node          = null;       //等级展示节点

    private m_Time: number                = null;
    private m_CurLevel: number            = null;

    private m_IsStart: boolean            = null;

    private m_NameParent: cc.Node         = null;       //昵称父节点

    private m_NickNode: cc.Node           = null;       //昵称节点

    constructor()
    {
        super();

        this.m_UID = 0;
        this.m_Time = 1.0 / 60.0;
        this.m_IsStart = false;
        this.m_CurLevel = 1;
    }

    public setID(id: number): void
    {
        this.m_UID = id;
    }
    
    public getID(): number
    {
        return this.m_UID;
    }

    public initCarUI(): void
    {
        this.node.isCheck = false;
        this.node.active = true;

        let playerData: PlayerData = PlayerMgr.GetPlayer(this.m_UID);
        
        let itemID: number = playerData.getCarID();

        this.setSkin(itemID);

        let initRadius = this.getComponent(cc.CircleCollider).radius;

        let initScale: number = CarUtil.CalcScaleValue();
        let impact: number = CarUtil.CalcCarPower(itemID);
        let speed: number = CarUtil.CalcCarSpeeed(itemID);
        playerData.setSpeed(speed);
        playerData.setInitScale(initScale);
        playerData.setCurScale(initScale);
        playerData.setInitRadius(initRadius);
        playerData.setCurRadius(NumberUtil.NumberMul(initRadius, initScale));
        playerData.setImpactForce(impact);

        this.node.scale = initScale;

        let level: number = playerData.getLevel();

        this.m_LevelNode = this.node.getChildByName("level");
        this.m_PhysicsCollider = this.getComponent("cc.PhysicsCircleCollider");
        this.m_Body = this.getComponent(cc.RigidBody);
        this.m_ReliveAnim = this.getComponent(cc.Animation);

        this.m_LevelNode.getComponent(cc.Label).setIsUseCostom(true);

        EngineUtil.SetNodeText(this.m_LevelNode, level.toString());

        //玩家昵称节点的父节点(玩家昵称统一放到另一个节点渲染,为了不打断合批)
        this.m_NameParent = cc.find("GameMap/PlayerName");

        //初始化出生时的移动向量
        this.initVeloctity();

        //初始化出生时的角度
        this.initRotate();

        this.addNickNode();

        this.addMotion(playerData.getMotionID());

        if(playerData.getState() == -1)
        {
            //进入无敌
            this.enterInvncible();
        }
    }

    public doStart(): void
    {
        this.node.zIndex = 10;

        this.m_IsStart = true;
    }

    //初始化出生方向
    public initVeloctity(): void
    {
        let playerData = PlayerMgr.GetPlayer(this.m_UID);
        if(!playerData)return;
        
        //设置默认出生方向
        playerData.setCurLinearVeloctity(cc.v2(0, 1));
    }

    //根据出生方向设置车的角度
    public initRotate(): void
    {
        let playerData = PlayerMgr.GetPlayer(this.m_UID);
        if(!playerData)return;

        let direction = cc.v2(0, 1);

        let rotate = Util.GetRotation(cc.v2(0, 0), direction);

        playerData.setRotate(rotate);

        this.node.rotation = rotate;
    }

    //添加昵称节点
    private addNickNode()
    {
        if(this.m_NameParent)
        {
            let carNamePrefab: cc.Prefab = EngineUtil.GetPrefab("CarName");
            if(carNamePrefab)
            {
                let carNameNode: cc.Node = cc.instantiate(carNamePrefab);
                if(carNameNode)
                {
                    this.m_NickNode = carNameNode;
                    this.m_NameParent.addChild(carNameNode);

                    let playerData: PlayerData = PlayerMgr.GetPlayer(this.m_UID);

                    let nickname: string = Util.GetByteVal(playerData.getNickName().toString(), 12);
                    EngineUtil.SetNodeText(carNameNode.getChildByName("NickName"), nickname);
                }
            }
        }
    }

    //添加拖尾
    public addMotion(motionID: number)
    {
        let motionInfo: MotionTableInfo = ItemSystem.GetInstance().GetTableInfoByItemID(motionID);
        if(motionInfo)
        {
            let motionCom: MotionStreakBase = null;

            if(motionInfo.GetType() === MotionType.MT_Common)
            {
                motionCom = this.node.addComponent(MyMotionStreak);
            }
            else if(motionInfo.GetType() === MotionType.MT_Generate)
            {
                motionCom = this.node.addComponent(CustomMotion);
            }

            if(motionCom)
            {
                motionCom.setMotionID(motionID);
                motionCom.hide();

                this.m_Streak = motionCom;
            }
        }
    }

    public initPos(pos: cc.Vec2): void
    {
        this.node.setPosition(pos);
        this.doSynNicknameNode();
    }

    //设置皮肤
    private setSkin(itemID: number): void
    {
        let carInfo: CarTableInfo = ItemSystem.GetInstance().GetTableInfoByItemID(itemID);
        if(carInfo)
        {
            let filename = 'Car_' + carInfo.GetImgID();
            EngineUtil.SetNodeTextureByType(AtlasType.AT_GameCar, this.node, filename);
        }
    }

    //死亡
    public death(): void
    {
        GameLog.PrintLog("CarCom:death", this.m_UID);

        this.node.active = false;

        //隐藏拖尾
        if(this.m_Streak)
        {
            this.m_Streak.hide();
        }

        if(this.m_NickNode)
        {
            this.m_NickNode.active = false;
        }

        //初始化角度
        this.initRotate();

        //初始化向量
        this.initVeloctity()

        let playerData: PlayerData = PlayerMgr.GetPlayer(this.m_UID);
        if(!playerData)return;

        playerData.setOpDirty(false);

        //改变状态为已死亡
        playerData.setState(PlayerState.PS_HaveDied);
    }

    //复活
    public relive(): void
    {
        GameLog.PrintLog("CarCom:relive", this.m_UID);

        this.node.active = true;
        this.node.isCheck = false;

        if(this.m_NickNode)
        {
            this.m_NickNode.active = true;
        }

        let playerData: PlayerData = PlayerMgr.GetPlayer(this.m_UID);
        if(!playerData)return;

        if(this.m_ReliveAnim)
        {
            this.m_ReliveAnim.play();
        }

        //显示拖尾
        if(this.m_Streak)
        {
            this.m_Streak.reset();
        }

        //初始化角度
        this.initRotate();

        //初始化向量
        this.initVeloctity()

        //清除击杀者的标记
        playerData.setLastColliderCarID(0);

        playerData.setOpDirty(false);

        //复活后进入无敌
        this.enterInvncible();
    }

    //进入无敌状态
    public enterInvncible()
    {
        GameLog.PrintLog("CarCom:enterInvncible：", this.m_UID);

        let playerData: PlayerData = PlayerMgr.GetPlayer(this.m_UID);
        if(!playerData)return;

        if(this.m_ReliveAnim)
        {
            this.m_ReliveAnim.play();
        }
        
        //设为无敌状态(不参与碰撞)
        playerData.setState(PlayerState.PS_Invincible);
    }

    //如果当前处于无敌状态则退出无敌状态
    public exitInvincible()
    {
        let playerData: PlayerData = PlayerMgr.GetPlayer(this.m_UID);
        if(playerData.getState() === PlayerState.PS_WaitRelive)
        {
            this.invincibleFinished();
        }
    }

    //无敌时间结束
    public invincibleFinished(): void
    {
        GameLog.PrintLog("CarCom:invincibleFinished：", this.m_UID);

        //显示拖尾
        if(this.m_Streak)
        {
            this.m_Streak.show();
        }

        let playerData: PlayerData = PlayerMgr.GetPlayer(this.m_UID);
        if(!playerData)return;

        if(this.m_PhysicsCollider)
        {
            this.m_PhysicsCollider.enabled = true;
        }

        if(this.m_ReliveAnim)
        {
            this.m_ReliveAnim.stop();
            this.node.opacity = 255;
        }

        playerData.setState(PlayerState.PS_Alive);
    }

    //碰撞反弹
    public rebound(impluse: cc.Vec2, type: number): void
    {
        GameLog.PrintLog("CarCom:rebound:", this.m_UID);

        let playerData: PlayerData = PlayerMgr.GetPlayer(this.m_UID);

        if(playerData){
            playerData.setIsRebound(true);
            playerData.setColliderVec(impluse.x, impluse.y);
        }

        this.unschedule(this.afterRebound);

        var time = CarTable.ColliderTime;
        if(type == 1){
            time = CarTable.ReboundTime;
        }

        this.scheduleOnce(this.afterRebound, time);

        if(this.m_Body)
        {
            this.m_Body.applyLinearImpulse( impluse, this.m_Body.getWorldCenter(), true );
        }
    }

    //获得圆形的碰撞区域
    public getCollideArea(): any
    {
        let colliderCom = this.getComponent(cc.CircleCollider);
        if(!colliderCom)return;

        let colliderAreaOffect = colliderCom.offset;
        let x = this.node.x + colliderAreaOffect.x;
        let y = this.node.y + colliderAreaOffect.y;

        let radius = colliderCom.radius * this.node.scaleX;

        return {x: x, y: y, radius: radius};
    }

    public frameOnMove (dt: number) : void
    {
        if(GameConfig.PhysicsSwitch && GameLogic.GetInstance().IsPracticeMode())
        {
            this.doMoveForPVE(dt);
        }
        else
        {
            this.doMoveForPVP(dt);
        }

        this.doSynNicknameNode();
        
        this.doSynLevel();
    }

    //刷新昵称的位置
    public doSynNicknameNode()
    {
        if(this.m_NickNode)
        {
            this.m_NickNode.x = this.node.x;
            this.m_NickNode.y = this.node.y;

            this.m_NickNode.scale = this.node.scale;
            this.m_NickNode.rotation = this.node.rotation;
        }
    }

    //同步等级
    public doSynLevel(): void
    {
        let playerData:PlayerData = PlayerMgr.GetPlayer(this.m_UID);

        if(playerData && this.m_CurLevel !== playerData.getLevel())
        {
            let curScale: number = playerData.getCurScale();
            let curLevel: number = playerData.getLevel();

            this.node.scale = curScale;

            this.m_CurLevel = curLevel;
            EngineUtil.SetNodeText( this.m_LevelNode, curLevel.toString() );

            if(PlayerMgr.IsSelf(this.m_UID))
            {
                this.dispatchEvent(MessageType.Self_Level_Changed, curLevel);
            }

            if(this.m_Streak)
            {
                this.m_Streak.setMyScale(this.m_CurLevel * 0.08 + 1);
            }

            GameLog.PrintDebug(this.m_UID, "-CarCom:doSynLevel:", curLevel);
        }
    }

    public doMoveForPVP(dt: number): void
    {
        let playerData: PlayerData = PlayerMgr.GetPlayer(this.m_UID);

        if(playerData)
        {

            let position : cc.Vec2 = playerData.getPosition();
            let rotate: number = playerData.getRotate();
    
            this.node.rotation = rotate;

            this.node.x = position.x;
            this.node.y = position.y;

            // GameLog.PrintDebug("RenderPosUpdate:", this.m_UID, "-", Date.now(), "-{", this.node.x, "," + this.node.y, "}" );
        }
    }

    public doMoveForPVE(dt: number)
    {
        if(!this.m_IsStart)
        {
            return;
        }

        let playerData: PlayerData = PlayerMgr.GetPlayer(this.m_UID);

        //反弹过程中不允许改变速度向量
        if(!playerData || playerData.getIsRebound())
        {
            return;
        }

        if(this.m_Body)
        {
            var vel = this.m_Body.linearVelocity;
            var curLinear = playerData.getCurLinearVeloctity();// the vector speed you set
            var desiredVel = curLinear.mul(playerData.getSpeed());

            if(cc.pFuzzyEqual(vel, desiredVel, 1) === false)
            {
                this.node.rotation = playerData.getRotate();

                let velChange: cc.Vec2 = cc.v2();
                let force: cc.Vec2 = cc.v2();

                Util.Sub(desiredVel, vel, velChange);
                Util.Clamp(velChange, cc.v2(-30, -30), cc.v2(30, 30), velChange);

                velChange.div(this.m_Time, force); //f = mv/t

                // Game.GameLog.PrintLog(force);

                this.m_Body.applyForceToCenter(force, true);
            }
        }
    }

    //反弹后的逻辑
    private afterRebound(): void
    {
        let playerData = PlayerMgr.GetPlayer(this.m_UID);

        if(playerData)
        {
            playerData.setIsRebound(false);
        }

        if(PlayerMgr.IsSelf(this.m_UID))
        {
            GameLogic.GetInstance().isCanInput = true;
        }
        GameLogic.GetInstance().SetIsHandleCollider(false);
    }
    
    protected onDestroy()
    {
        super.onDestroy();

        this.m_LevelNode = null;
        this.m_NickNode = null;
        this.m_NameParent = null;
        this.m_ReliveAnim = null;
        this.m_PhysicsCollider = null;
        this.m_Body = null;
    }
}
