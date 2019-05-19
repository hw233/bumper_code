export enum UserCmdType{
    UCT_Move            = 1,
    UCT_Null            = 2,
    UCT_Collide         = 3,
    UCT_Rebound         = 4,
    UCT_DLD_Relive      = 5,
    UCT_DQZ_Relive      = 6,
    UCT_DLD_Syn_Level   = 7,
    UCT_DQZ_Syn_Level   = 8,
    UCT_Death           = 9,
}

export enum CarUnlockCon{
    CUC_Invite          = 1,
    CUC_ZuanShi5        = 2,
    CUC_Gold            = 3,
    CUC_Video           = 4,
    CUC_Debris          = 5,
    CUC_TryPlay         = 6,
    CUC_Activity        = 7,
};

export enum MotionType{
    MT_Common           = 1,
    MT_Generate         = 2,
}

//车辆解锁状态
export enum CarUnlockState{
    CUS_Using           = 1,//使用中
    CUS_AlreadyUnlock   = 2,//已解锁
    CUS_NotUnlock       = 3,//未解锁
}

//房间状态
export enum RoomState{
    RS_Null         = 0,    //没有房间状态
    RS_Invite       = 1,    //邀请状态
    RS_Match        = 2,    //匹配状态
};

//游戏状态
export enum GameState{
    GS_Init         = 1 << 0,     //初始化阶段
    GS_Running      = 1 << 1,     //运行阶段
    GS_WaitOver     = 1 << 2,     //待结束阶段    
    GS_Over         = 1 << 3,     //已结束阶段    
};

//玩家状态
export enum PlayerState {
    PS_Alive        = 1 << 0,   //活着的状态
    PS_WaitDeath    = 1 << 1,   //待死亡状态(下一帧将不再渲染)
    PS_HaveDied     = 1 << 2,   //已经死亡的状态
    PS_Invincible   = 1 << 3,   //无敌状态(可以和活着的状态叠加)
    PS_WaitRelive   = 1 << 4,   //介于无敌和退出无敌之间的状态(下一帧将退出无敌状态)
};

//碰撞体类型
export enum ColliderType {
    CT_Circle       = 1,
    CT_Box          = 2,
    CT_Polygon      = 3,
};

//游戏模式
export enum GameModel{
    GM_PracticeMode     = 0,
    GM_DLDMode          = 1,
    GM_DQZMode          = 2,
};

//队伍类型
export enum TeamType{
    TT_Red              = 1,    //红方
    TT_Blue             = 2,    //蓝方
};

//游戏结果
export enum GameResult{
    GR_Victory          = 1,    //胜利
    GR_Dogfall          = 2,    //平局
    GR_Defeat           = 3,    //失败
};