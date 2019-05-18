//战斗中的数据
export default class BettleData{

    private _curOverlordID: number      = null;

    private _colliderUserID: number     = null;
    private _impluse: cc.Vec2           = null;     //反弹冲量
    private _colliderType: number       = null;     //碰撞类型

    private _relivePosID: number        = null;     //复活位置id
    private _relivePos: cc.Vec2         = null;     //复活位置
    private _reliveDirection: cc.Vec2   = null;     //复活方向

    private _typeID: number             = null;     //生成的道具类型id
    private _id: number                 = null;     //生成的道具（世界唯一id)
    private _location: cc.Vec2          = null;     //生成的道具位置
    private _itemScore: number          = null;     //道具的分数
    private _locationTag: number        = null;     //位置对应的tag
    private _pickItemID: number         = null;     //采集的道具(世界唯一id)
    private _pickerID: number           = null;     //采集者的id

    private _enemyUserID: number        = null;     //仇人id
    private _reliveUserID: number       = null;     //复活者id
    private _overlordUserID: number     = null;     //霸主id


    private _redScore: number           = null;     //红队积分
    private _blueScore: number          = null;     //蓝队积分


    private _teamID: number             = null;     //需要同步的队伍ID
    private _targetUserID: number       = null;     //目标uid
    private _targetLevel: number        = null;     //目标等级

    constructor()
    {
        this._colliderUserID = 0;
        this._impluse = cc.v2(0, 0);
        this._colliderType = 0;

        this._redScore = 0;
        this._blueScore = 0;
        this._teamID = 0;
        this._targetUserID = 0;
        this._targetLevel = 1;

        this._curOverlordID = 0;

        this._enemyUserID = 0;
        this._pickItemID = 0;
        this._overlordUserID = 0;
        this._reliveUserID = 0;
        this._relivePosID = 0;
        this._pickerID = 0;
        this._locationTag = 0;
        this._itemScore = 0;

        this._relivePos = cc.v2(0, 0);
        this._reliveDirection = cc.v2(0, 0);

        this._typeID = 0;
        this._id = 0;
        this._location = cc.v2(0, 0);
    }

    public setType(type: number)
    {
        this._colliderType = type;
    }

    public getType(): number
    {
        return this._colliderType;
    }

    public setColliderID(id: number)
    {
        this._colliderUserID = id;
    }

    public getColliderID(): number
    {
        return this._colliderUserID;
    }

    public setImpluse(x: number, y: number)
    {
        this._impluse.x = x;
        this._impluse.y = y;
    }

    public getImpluse(): cc.Vec2
    {
        return this._impluse;
    }

    public setTypeID(type: number)
    {
        this._typeID = type;
    }

    public getTypeID(): number
    {
        return this._typeID;
    }

    public setID(id: number)
    {
        this._id = id;
    }

    public getID(): number
    {
        return this._id;
    }

    public setLocation(x: number, y: number)
    {
        this._location.x = x;
        this._location.y = y;
    }

    public getLocation(): cc.Vec2
    {
        return this._location;
    }

    public setLocationTag(tag: number)
    {
        this._locationTag = tag;
    }

    public getLocationTag(): number
    {
        return this._locationTag;
    }

    public setRelivePos(x: number, y: number)
    {
        this._relivePos.x = x;
        this._relivePos.y = y;
    }

    public setRelivePosID(posID: number)
    {
        this._relivePosID = posID;
    }

    public getRelivePosID(): number
    {
        return this._relivePosID;
    }

    public getRelivePos(): cc.Vec2
    {
        return this._relivePos;
    }

    public setReliveDirection(x: number, y: number)
    {
        this._reliveDirection.x = x;
        this._reliveDirection.y = y;
    }

    public getReliveDirection(): cc.Vec2
    {
        return this._reliveDirection;
    }

    public setReliveUserID(id: number)
    {
        this._reliveUserID = id;
    }

    public getReliveUserID(): number
    {
        return this._reliveUserID;
    }

    public setEnemyUserID(id: number)
    {
        this._enemyUserID = id;
    }

    public getEnemyUserID(): number
    {
        return this._enemyUserID;
    }

    public setOverlordUserID(id: number)
    {
        this._overlordUserID = id;
    }

    public getOverlordUserID(): number
    {
        return this._overlordUserID;
    }

    public setCurOverlordUserID(id: number)
    {
        this._curOverlordID = id;
    }

    public getCurOverlordUserID(): number
    {
        return this._curOverlordID;
    }

    public setPickItemID(id: number)
    {
        this._pickItemID = id;
    }

    public getPickItemID(): number
    {
        return this._pickItemID;
    }

    public setPickerID(id: number)
    {
        this._pickerID = id;
    }

    public getPickerID(): number
    {
        return this._pickerID;
    }

    public setItemScore(score: number)
    {
        this._itemScore = score;
    }

    public getItemScore(): number
    {
        return this._itemScore;
    }

    public setTeamID(teamID: number)
    {
        this._teamID = teamID;
    }

    public getTeamID(): number
    {
        return this._teamID;
    }

    public setTargetUserID(targetID: number)
    {
        this._targetUserID = targetID;
    }

    public getTargetUserID(): number
    {
        return this._targetUserID;
    }

    public setTargetLevel(level: number)
    {
        this._targetLevel = level;
    }

    public getTargetLevel(): number
    {
        return this._targetLevel;
    }

    public setRedScore(score: number)
    {
        this._redScore = score;
    }

    public getRedScore(): number
    {
        return this._redScore;
    }

    public setBlueScore(score: number)
    {
        this._blueScore = score;
    }

    public getBlueScore(): number
    {
        return this._blueScore;
    }
}