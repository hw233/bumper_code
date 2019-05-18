import ColliderArea from './component/ColliderArea';
import PlayerData from './data/PlayerData';
import PlayerMgr from './manager/PlayerMgr';
import Util from './common/Util';
import { CarTable } from './table/CarTable';
import GameConfig from './GameConfig';
import GameLogic from './GameLogic';
import NumberUtil from './common/NumberUtil';
import CarTableInfo from './table/CarTableInfo';
import ItemSystem from './Item/ItemSystem';


export default class CarUtil
{

    //计算等级为1时的缩放值
    public static CalcScaleValue(): number
    {
        let initScale = 1;
        let temp: number = NumberUtil.NumberAdd(1, CarTable.LevelVolume);

        for(let i = 0; i < CarTable.MaxLevel; i ++)
        {
            initScale = NumberUtil.NumberDiv(initScale, temp);
        }

        return parseFloat(initScale.toFixed(4));
    }

    //根据车的ID计算车的速度
    public static CalcCarSpeeed(id){
        //基础速度
        let baseSpeed = CarTable.BaseSpeed;
        if(GameConfig.PhysicsSwitch && GameLogic.GetInstance().IsPracticeMode())
        {
            baseSpeed = CarTable.PhysicsBaseSpeed;
        }

        //车辆对应的速度加成
        let bonusSpeed = 0;

        let carInfo: CarTableInfo = ItemSystem.GetInstance().GetTableInfoByItemID(id);
        if(carInfo)
        {
            if(carInfo)
            {
                bonusSpeed = carInfo.GetSpeed() / 100;
            }
    
            let finalSpeed = baseSpeed * (1 + bonusSpeed);
    
            return finalSpeed;
        }
    }

    //根据车的ID计算车的撞击力
    public static CalcCarPower(id)
    {
        //基础撞击力
        let basePower = CarTable.ColliderSpeed;
        if(GameConfig.PhysicsSwitch && GameLogic.GetInstance().IsPracticeMode())
        {
            basePower = CarTable.PhysicsColliderSpeed;
        }

        //车辆对应的速度加成
        let bonusPower = 0;

        let carInfo: CarTableInfo = ItemSystem.GetInstance().GetTableInfoByItemID(id);
        if(carInfo)
        {
            bonusPower = carInfo.GetPower() / 100;
        }

        let finalPower = basePower * (1 + bonusPower);

        return finalPower;
    }

    //计算两车相撞后的反弹冲量
    /**
     * @param {碰撞点} collidePoint 
     * @param {刚体A} selfBody
     * @param {刚体B} otherBody
     */
    public static CalcReboundImpulse(collidePoint: cc.Vec2, power: number, selfID: number, otherID: number)
    {
        let selfPlayerData: PlayerData = PlayerMgr.GetPlayer(selfID);
        if(!selfPlayerData)return null;

        let otherPlayerData: PlayerData = PlayerMgr.GetPlayer(otherID);
        if(!otherPlayerData)return null;

        let selfNode: cc.Node = GameLogic.GetInstance().GetCarManager().getCarByID(selfID);
        if(!selfNode)return null;

        let otherNode: cc.Node = GameLogic.GetInstance().GetCarManager().getCarByID(otherID);
        if(!otherNode)return null;

        let selfColliderAreaCom: ColliderArea = selfNode.getComponent("ColliderArea");
        let selfType = selfColliderAreaCom.getColliderAreaType(collidePoint);

        let otherColliderAreaCom: ColliderArea = otherNode.getComponent("ColliderArea");
        let otherType = otherColliderAreaCom.getColliderAreaType(collidePoint);

        let otherLevel = otherPlayerData.getLevel();
        
        let info = this.CalcColliderImpact(selfType, otherType);

        let colliderAreaImpact = info.value;

        //等级的撞击力
        let levelImpact: number = NumberUtil.NumberMul( (otherLevel - 1), CarTable.LevelImpact );

        let impact: number = NumberUtil.NumberMul( (levelImpact + 1), colliderAreaImpact );

        if(selfType === 3)
        {
            // EventCenter.DispatchEvent(MessageType.HitWeaknesses, selfPlayerData.getUserID());
        }

        let relativeVelocity = null;

        if(GameConfig.PhysicsSwitch && GameLogic.GetInstance().IsPracticeMode())
        {
            relativeVelocity = this.CalcRoboundAngleForPhysics(selfNode, otherNode, collidePoint);
        }
        else
        {
            relativeVelocity = this.CalcRoboundAngle(selfID, otherID);
        }

        if(relativeVelocity.x === 0.0 && relativeVelocity.y === 0.0)
        {
            return null;
        }

        let normalizeVec = relativeVelocity.normalize();

        // 施加一个冲量到刚体上指定的点上，这个点是世界坐标系下的一个点
        let Impulse = cc.v2();

        Impulse.x = NumberUtil.NumberMul( normalizeVec.x, NumberUtil.NumberMul(power, impact) );
        Impulse.y = NumberUtil.NumberMul( normalizeVec.y, NumberUtil.NumberMul(power, impact) );

        info.impluse = Impulse;

        return info;
    }

    //获取碰撞点
    public static CalcCollidePoint(a1: cc.Vec2, r1: number, a2: cc.Vec2, r2: number)
    {

        let arg: Function = function(v: cc.Vec2): number
        {
            return Math.atan2(v.y, v.x);
        }

        let polar: Function = function(a: number, r: number): cc.Vec2
        {
            return cc.v2(Math.cos(r) * a, Math.sin(r) * a);
        }

        let d: number = Util.GetDistance(a2, a1);
        let a: number = Math.acos( (r1 * r1 + d * d - r2 * r2) / ( 2* r1 * d) );
        let t: number = arg( a2.sub(a1) );

        let p1: cc.Vec2 = a1.add(polar(r1, t + a));
        let p2: cc.Vec2 = a1.add(polar(r1, t - a));

        // Debug.ShowNode(cc.find("GameMap"), p1);
        // Debug.ShowNode(cc.find("GameMap"), p2);

        return p1;
    }

    //计算碰撞反弹角度
    public static CalcRoboundAngleForPhysics(selfNode: cc.Node, otherNode: cc.Node, colliderPoint: cc.Vec2)
    {
        if(selfNode && otherNode)
        {
            let selfBody = selfNode.getComponent(cc.RigidBody);
            let otherBody = otherNode.getComponent(cc.RigidBody);

            var vel1 = selfBody.getLinearVelocityFromWorldPoint( colliderPoint, null );
            var vel2 = otherBody.getLinearVelocityFromWorldPoint( colliderPoint, null );
            var relativeVelocity = vel1.sub(vel2);

            return relativeVelocity;
        }
        return null;
    }

    //计算碰撞反弹角度
    public static CalcRoboundAngle(selfID: number, otherID: number)
    {
        let selfPlayerData: PlayerData = PlayerMgr.GetPlayer(selfID);
        if(!selfPlayerData)return null;

        let otherPlayerData: PlayerData = PlayerMgr.GetPlayer(otherID);
        if(!otherPlayerData)return null;

        let aPos = selfPlayerData.getPosition();
        let bPos = otherPlayerData.getPosition();

        let aRadius = selfPlayerData.getCurRadius();
        let bRadius = otherPlayerData.getCurRadius();

        let dx = bPos.x - aPos.x;                      //两小球圆心距对应的两条直角边

        let dy= bPos.y-aPos.y;

        var dist=Math.sqrt(dx*dx + dy*dy);       //两直角边求圆心距

        var misDist=aRadius + bRadius;   //圆心距最小值

        //假设碰撞后球会按原方向继续做一定的运动，将其定义为运动A   

        var angle=Math.atan2(dy,dx);

        //当刚好相碰，即dist=misDist时，tx=ballb.x, ty=ballb.y

         let tx = aPos.x+Math.cos(angle) * misDist; 

        let ty=aPos.y+Math.sin(angle) * misDist;

        //产生运动A后，tx > ballb.x, ty > ballb.y,所以用ax、ay记录的是运动A的值

        let ax =(tx- bPos.x);  

        let ay =(ty - bPos.y);

        return cc.v2(-ax, -ay);
    }

    public static CalcColliderImpact(aType, bType): any
    {
        let value = 0;
        let type = 0;

        switch(aType){
            case 1: 
            {
                switch(bType){
                    case 1:
                    {
                        type = 1;
                        value = CarTable.ColliderAreaRebound[bType];
                        break;
                    }
                    case 2:
                    {
                        type = 1;
                        value = CarTable.ColliderAreaRebound[bType];
                        break;
                    }
                    case 3:
                    {
                        type = 1;
                        value = CarTable.ColliderAreaRebound[bType];
                        break;
                    }
                }
                break;
            }
            case 2: 
            {
                switch(bType){
                    case 1:
                    {
                        type = 2;
                        value = CarTable.ColliderAreaImpact[aType];
                        break;
                    }
                    case 2:
                    {
                        type = 1;
                        value = CarTable.ColliderAreaRebound[bType];
                        break;
                    }
                    case 3:
                    {
                        type = 1;
                        value = CarTable.ColliderAreaRebound[bType];
                        break;
                    }
                }
                break;
            }
            case 3:
            {
                switch(bType){
                    case 1:
                    {
                        type = 2;
                        value = CarTable.ColliderAreaImpact[aType];
                        break;
                    }
                    case 2:
                    {
                        type = 1;
                        value = CarTable.ColliderAreaRebound[bType];
                        break;
                    }
                    case 3:
                    {
                        type = 1;
                        value = CarTable.ColliderAreaRebound[bType];
                        break;
                    }
                }
                break;
            }
        }

        return{
            value:value,
            type: type
        };
    }

};