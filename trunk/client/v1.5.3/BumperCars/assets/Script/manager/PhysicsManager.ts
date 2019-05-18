import GameConfig from '../GameConfig';
const {ccclass, property} = cc._decorator;

@ccclass
export default class PhysicsManager extends cc.Component{
    init () 
    {
        if(!GameConfig.PhysicsSwitch)
        {
            return;
        }
        
        let manager = cc.director.getPhysicsManager();

        //开启物理系统
        manager.enabled = true;

        // // //绘制物理调试信息
        // manager.debugDrawFlags = 
        //     cc.PhysicsManager.DrawBits.e_aabbBit |
        //     cc.PhysicsManager.DrawBits.e_pairBit |
        //     cc.PhysicsManager.DrawBits.e_centerOfMassBit |
        //     cc.PhysicsManager.DrawBits.e_jointBit |
        //     cc.PhysicsManager.DrawBits.e_shapeBit;

        //设置物理重力
        manager.gravity = cc.v2(0, 0);

        // 开启物理步长的设置
        manager.enabledAccumulator = true;

        // // 物理步长，默认 FIXED_TIME_STEP 是 1/60
        // manager.FIXED_TIME_STEP = 1/60;

        // // 每次更新物理系统处理速度的迭代次数，默认为 10
        // manager.VELOCITY_ITERATIONS = 10;

        // // 每次更新物理系统处理位置的迭代次数，默认为 10
    }    
}