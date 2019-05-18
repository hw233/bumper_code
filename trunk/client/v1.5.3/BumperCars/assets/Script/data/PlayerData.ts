import GameLog from '../Log/LogTool';
import NumberUtil from '../common/NumberUtil';

//房间内玩家的数据
export default class PlayerData{
    private m_UserID: number                 = null;     //用户ID
    private m_Nickname: string               = null;     //用户昵称
    private m_AvatarUrl: string              = null;     //用户头像
    private m_CarID: number                  = null;     //使用中的车辆id
    private m_MotionID: number               = null;     //使用中的拖尾id
    private m_Level: number                  = null;     //等级
    private m_IsMaster: boolean              = null;     //是否为房主
    private m_GrandingID: number             = null;     //段位id
    private m_CurStarNum: number             = null;     //当前星星数量
    private m_KillNum: number                = null;     //击杀数
    private m_Score: number                  = null;     //积分
    private m_TeamID: number                 = null;     //队伍ID
    private m_TeamType: number               = null;     //队伍类型(红方或者蓝方)
    private m_BirthPointID: number           = null;     //出生点id
    private m_BirthDirection: cc.Vec2        = null;     //出生方向
    private m_Position: cc.Vec2              = null;     //位置
    private m_OpStartPoint: cc.Vec2          = null;     //玩家的操作起点
    private m_OpEndPoint: cc.Vec2            = null;     //玩家的操作终点
    private m_SynCurLocation: cc.Vec2        = null;     //同步当前位置
    private m_OpDirty: boolean               = null;     //操作脏标记
    private m_State: number                  = null;     //玩家状态 (0: 待死亡 1: 活着 2: 2：已死亡 3：无敌)
    private m_ImpactForce: number            = null;     //撞击力
    private m_Volume: number                 = null;     //体积
    private m_Speed: number                  = null;     //速度
    private m_LastColliderCarID: number      = null;     //被撞出地图的车的id
    private m_CurLinearVelocity: cc.Vec2     = null;     //当前速度向量(标准化的)
    private m_Rotate: number                 = null;     //旋转角度
    private m_InitScale: number              = null;     //初始缩放值
    private m_CurScale: number               = null;     //当前缩放值
    private m_InitRadius: number             = null;     //初始半径
    private m_CurRadius: number              = null;     //当前半径
    private m_IsRebound: boolean             = null;     //是否正在反弹
    private m_IsJoinGame: boolean            = null;     //是否已经加入游戏
    private m_IsOverlord: boolean            = null;     //是否为霸主
    private m_IsEnemy: boolean               = null;     //是否为我的仇人
    private m_CollideVec: cc.Vec2            = null;

    public  m_isHandleRebound = null;
    public  m_Colliding: number = null;
    public  m_CollidePlayer: number = null; //当前碰撞的玩家

    constructor()
    {
        this.m_State = -1;
        this.m_UserID = 0;
        this.m_Nickname = "";
        this.m_AvatarUrl = "";
        this.m_Level = 1;
        this.m_IsMaster = false;
        this.m_KillNum = 0;
        this.m_Score = 0;
        this.m_ImpactForce = 0;
        this.m_Volume = 0;
        this.m_LastColliderCarID = 0;
        this.m_CurLinearVelocity = cc.v2(0, 0);
        this.m_BirthDirection = cc.v2(0, 0);
        this.m_SynCurLocation = cc.v2(0, 0);
        this.m_CollideVec = cc.v2(0, 0);
        this.m_BirthPointID = 0;
        this.m_IsRebound = false;
        this.m_Rotate = 0;
        this.m_TeamType = 0;
        this.m_CarID = 0;
        this.m_OpStartPoint = cc.v2(0, 0);
        this.m_OpEndPoint = cc.v2(0, 0);
        this.m_OpDirty = false;
        this.m_TeamID = 0;
        this.m_IsJoinGame = false;
        this.m_GrandingID = 0;
        this.m_CurStarNum = 0;
        this.m_Position = cc.v2(0, 0);
        this.m_InitScale = 0;
        this.m_CurScale = 0;
        this.m_InitRadius = 0;
        this.m_CurRadius = 0;
        this.m_MotionID = 1;
        this.m_IsOverlord = false;
        this.m_IsEnemy = false;

        this.m_isHandleRebound = false;
        this.m_Colliding = 0;
    }

    public setUserID(uid: number )
    {
        this.m_UserID = uid;
    }

    public getUserID(): number 
    {
        return this.m_UserID;
    }

    public setAvatarUrl(avatarUrl: string)
    {
        this.m_AvatarUrl = avatarUrl;
    }

    public getAvatarUrl(): string
    {
        return this.m_AvatarUrl;
    }

    public setState(state: number )
    {
        GameLog.PrintDebug(this.m_UserID, ", PlayerData:setState:", state);

        this.m_State = state;
    }

    public getState(): number 
    {
        return this.m_State;
    }

    public setNickname(nickname: string)
    {
        this.m_Nickname = nickname;
    }

    public getNickName(): string
    {
        return this.m_Nickname;
    }

    public setCarID(id: number )
    {
        this.m_CarID = id;
    }

    public getCarID(): number 
    {
        return this.m_CarID;
    }

    public setMotionID(id: number)
    {
        this.m_MotionID = id;
    }

    public getMotionID(): number
    {
        return this.m_MotionID;
    }

    public setGrandingID(id: number )
    {
        this.m_GrandingID = id;
    }

    public getGrandingID(): number 
    {
        return this.m_GrandingID;
    }

    public setCurStarNum(num: number )
    {
        this.m_CurStarNum = num;
    }

    public getCurStarNum(): number 
    {
        return this.m_CurStarNum;
    }

    public setTeamID(teamID: number )
    {
        this.m_TeamID = teamID;
    }

    public getTeamID(): number 
    {
        return this.m_TeamID;
    }

    public setTeamType(type: number )
    {
        this.m_TeamType = type;
    }

    public getTeamType(): number 
    {
        return this.m_TeamType;
    }

    public setIsMaster(isMaster: boolean)
    {
        this.m_IsMaster = isMaster;
    }

    public getIsMaster(): boolean
    {
        return this.m_IsMaster;
    }

    public setIsOverlord(isOverlord: boolean)
    {
        this.m_IsOverlord = isOverlord;
    }

    public getIsOverlord(): boolean
    {
        return this.m_IsOverlord;
    }

    public setIsEnemy(isEnemy: boolean)
    {
        this.m_IsEnemy = isEnemy;
    }

    public getIsEnemy(): boolean
    {
        return this.m_IsEnemy;
    }

    public setKillNum(num: number )
    {
        this.m_KillNum = num;
    }

    public getKillNum(): number 
    {
        return this.m_KillNum;
    }

    public setScore(score: number )
    {
        this.m_Score = score;
    }

    public getScore(): number 
    {
        return this.m_Score;
    }

    public setRotate(rotate: number )
    {
        this.m_Rotate = rotate;
    }

    public getRotate(): number 
    {
        return this.m_Rotate;
    }

    public setLevel(level: number )
    {
        this.m_Level = level;
    }

    public getLevel(): number 
    {
        return this.m_Level;
    }

    public setIsJoinGame(isJoin: boolean)
    {
        this.m_IsJoinGame = isJoin;
    }

    public getIsJoinGame(): boolean
    {
        return this.m_IsJoinGame;
    }

    public setBirthPointID(pointID: number )
    {
        this.m_BirthPointID = pointID;
    }

    public getBirthPointID(): number 
    {
        return this.m_BirthPointID;
    }

    public setBirthDirection(directionX, directionY)
    {
        this.m_BirthDirection.x = directionX;
        this.m_BirthDirection.y = directionY;
    }

    public getBirthDirection(): cc.Vec2
    {
        return this.m_BirthDirection;
    }

    public setInitScale(scale: number)
    {
        this.m_InitScale = scale;
    }

    public getInitScale(): number
    {
        return this.m_InitScale;
    }

    public setCurScale(scale: number): void
    {
        this.m_CurScale = scale;
    }
    
    public getCurScale(): number
    {
        return this.m_CurScale;
    }

    public setInitRadius(radius: number): void
    {
        this.m_InitRadius = radius;
    }

    public getInitRadius(): number
    {
        return this.m_InitRadius;
    }

    public setCurRadius(radius: number): void
    {
        this.m_CurRadius = radius;
    }

    public getCurRadius(): number
    {
        return this.m_CurRadius;
    }

    public setPosition(pos: any)
    {
        this.m_Position.x = pos.x;
        this.m_Position.y = pos.y;
    }

    public getPosition(): cc.Vec2
    {
        return this.m_Position;
    }

    public doMove(distance: cc.Vec2, frameID: number = 0)
    {
        this.m_Position.x = NumberUtil.NumberAdd(this.m_Position.x, distance.x);
        this.m_Position.y = NumberUtil.NumberAdd(this.m_Position.y, distance.y);

        GameLog.PrintDebug(frameID, ":", this.m_UserID, "-", Date.now(), "-{", this.m_Position.x, "," + this.m_Position.y, "}" );
    }

    public setSynCurLocation(x, y)
    {
        this.m_SynCurLocation.x = x;
        this.m_SynCurLocation.y = y;
    }

    public getSynCurLocation(): cc.Vec2
    {
        return this.m_SynCurLocation;
    }

    public setImpactForce(impactForce: number )
    {
        this.m_ImpactForce = impactForce;
    }

    public getImpactForce(): number 
    {
        return this.m_ImpactForce;
    }

    public setSpeed(speed: number)
    {
        this.m_Speed = speed;
    }

    public getSpeed(): number
    {
        return this.m_Speed;
    }

    public setVolume(volume: number )
    {
        this.m_Volume = volume;
    }

    public getVolume(): number 
    {
        return this.m_Volume;
    }

    public setLastColliderCarID(id: number )
    {
        this.m_LastColliderCarID = id;
    }

    public getLastColliderCarID(): number 
    {
        return this.m_LastColliderCarID;
    }

    public setCurLinearVeloctity(curLinearVelocity)
    {
        this.m_CurLinearVelocity.x = curLinearVelocity.x;
        this.m_CurLinearVelocity.y = curLinearVelocity.y;
    }

    public getCurLinearVeloctity(): cc.Vec2
    {
        return this.m_CurLinearVelocity;
    }
    
    public setColliderVec(x: number, y: number): void
    {
        this.m_CollideVec.x = x;
        this.m_CollideVec.y = y;
    }

    public getColliderVec(out: cc.Vec2): cc.Vec2
    {
        if(out)
        {
            out.set(this.m_CollideVec);
        }
        return this.m_CollideVec;
    }

    public setIsRebound(isRebound: boolean)
    {
        this.m_IsRebound = isRebound;
    }

    public getIsRebound(): boolean
    {
        return this.m_IsRebound;
    }

    public setOpStartPoint(x: number , y: number )
    {
        this.m_OpStartPoint.x = x;
        this.m_OpStartPoint.y = y;
    }

    public getOpStartPoint(): cc.Vec2
    {
        return this.m_OpStartPoint;
    }

    public setOpEndPoint(x: number , y: number )
    {
        this.m_OpEndPoint.x = x;
        this.m_OpEndPoint.y = y;
    }

    public getOpEndPoint(): cc.Vec2
    {
        return this.m_OpEndPoint;
    }

    public setOpDirty(dirty: boolean)
    {
        this.m_OpDirty = dirty;
    }

    public getOpDirty(): boolean
    {
        return this.m_OpDirty;
    }
}
