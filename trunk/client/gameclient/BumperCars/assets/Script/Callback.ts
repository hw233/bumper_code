import Util from "./common/Util";
import GameLogic from "./GameLogic";

const {ccclass, property} = cc._decorator;

@ccclass
export default class Callback extends cc.Component{
    
    private worldManifold = null;

    start () 
    {
    }

    // 只在两个碰撞体开始接触时被调用一次
    onBeginContact(contact, selfCollider, otherCollider) 
    {

        this.worldManifold = Util.CloneObj(contact.getWorldManifold());

        GameLogic.GetInstance().BeforeColliderLogic(this.worldManifold, selfCollider.node, otherCollider.node);
    }

    // 只在两个碰撞体结束接触时被调用一次
    onEndContact(contact, selfCollider, otherCollider) 
    {

    }

    // 每次将要处理碰撞体接触逻辑时被调用 可以在这处理碰撞信息
    onPreSolve(contact, selfCollider, otherCollider) 
    {
        // GameLog.PrintLog("onPreSolve", contact.getRestitution());
        // contact.setRestitution(contact.getRestitution() + 1);

        // // 修改碰撞体间的摩擦力
        // contact.setFriction(friction);

        // // 修改碰撞体间的弹性系数
        // contact.setRestitution(restitution);

        //这些修改只会在本次物理处理步骤中生效。
    }

    // 每次处理完碰撞体接触逻辑时被调用 contact携带引擎计算出的碰撞的冲量信息
    onPostSolve(contact, selfCollider, otherCollider) 
    {
        // GameLog.PrintLog("onPostSolve");
        // let impulse = contact.getImpulse();
        // GameLog.PrintLog(impulse);

        if(this.worldManifold && this.worldManifold.points.length > 0)
        {
            let collidePoint = this.worldManifold.points[0];
            GameLogic.GetInstance().ReboundLogicForPhysics(collidePoint, selfCollider.node, otherCollider.node);
            // GameLogic.GetInstance().ReboundLogic(this.worldManifold, selfCollider.body, otherCollider.body);
        }

        GameLogic.GetInstance().AfterColliderLogic(selfCollider.node, otherCollider.node);
    }
}
