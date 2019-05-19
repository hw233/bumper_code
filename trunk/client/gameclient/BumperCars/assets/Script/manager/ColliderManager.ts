const {ccclass, property} = cc._decorator;

@ccclass
export default class ColliderManager extends cc.Component{
    init(){
        //获取碰撞检测系统
        let colliderManager = cc.director.getCollisionManager();
        //开启碰撞检测系统
        colliderManager.enabled = true;
        // //开启 debug 绘制
        // colliderManager.enabledDebugDraw = true;
        // //显示碰撞组件的包围盒
        // colliderManager.enabledDrawBoundingBox = true;
    }
}