
export const CarTable = {
    //撞击力
    ColliderAreaImpact: {
        1: 1.0,
        2: 1.2,
        3: 1.4,
    },

    //反弹力
    ColliderAreaRebound: {
        1: 1,
        2: 0.4,
        3: 0.2,
    },

    ColliderSpeed: 20,

    LevelImpact: 0.10,   //等级提升对应的撞击力

    LevelVolume: 0.08,  //等级提升对应的体积

    // BaseImpact: 1.5,      //基础的撞击力

    PhysicsBaseSpeed: 350,
    PhysicsColliderSpeed: 520,

    BaseSpeed: 5 ,     //移动速度

    ReboundTime: 0.2,   //反弹的时间

    ColliderTime: 0.3, //撞击的时间

    AINum: CC_WECHATGAME ? 9 : 9,  //ai数量

    KillGold: 5,        //击杀获得的金币

    //连杀对应的倍数
    ContinueKill: {
        1: 1,
        2: 2,
        3: 4,
        4: 8,
        5: 16,
    },

    MaxLevel: 15,   //暂不可改
};