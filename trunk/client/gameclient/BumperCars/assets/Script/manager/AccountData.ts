//管理用户数据--与服务器保持同步
export default class AccountData
{
    private static instance: AccountData        = null;

    private _uid:number                         = null;     //用户id
    private _nickName:string                    = null;     //用户昵称
    private _headImg:string                     = null;     //头像地址

    // private _goldNum:number                     = null;     //金币数量
    private _killNum:number                     = null;     //击杀数量
    private _curLevelID:number                  = null;     //当前段位id
    private _curStarNum:number                  = null;     //当前星星数量
    // private _unlockCarID:Array<number>          = null;     //已解锁车辆ID
    private _usedCarID:number                   = null;     //使用中的车辆id
    private _usedMotionID: number               = null;     //使用中的拖尾id
    private _bzlpNum:number                     = null;     //霸主令牌的数量

    private _curLevelNum:number                 = null;     //当前段位的人数
    private _keepStarCount:number               = null;     //保留星星的次数
    private _receiveGiftCount:number            = null;     //领取礼包的次数

    private _addScore:number                    = null;     //积分加成
    private _addGold:number                     = null;     //金币加成

    private _signInData: number                 = null;     //签到数据
    private _taskData: number                   = null;     //任务数据

    private _onlineRewardTime: number           = null;     //在线领奖剩余时间

    private _timeLimitItemID: number            = null;     //限时免费车辆id
    private _timeLimitRemainTime: number        = null;     //限时免费车辆剩余时间

    private _dayIndex: number                   = null;     //周几
    
    constructor()
    {
        this._uid                       = 0;
        this._curLevelID                = 1;
        this._curStarNum                = 0;
        this._killNum                   = 0;
        // this._goldNum                   = 0;
        // this._unlockCarID               = [];
        this._curLevelNum               = 0;
        this._nickName                  = "";
        this._headImg                   = "";
        this._usedCarID                 = 0;
        this._usedMotionID              = 0;
        this._keepStarCount             = 0;
        this._receiveGiftCount          = 0;
        this._bzlpNum                   = 0;
        this._addScore                  = 0;
        this._addGold                   = 0;
        this._signInData                = 0;
        this._taskData                  = 0;
        this._onlineRewardTime          = 0;
        this._dayIndex                  = 0;
        this._timeLimitItemID           = 0;
        this._timeLimitRemainTime       = 0;
    }

    public static GetInstance(): AccountData
    {
        if(!this.instance)
        {
            this.instance = new AccountData();
        }
        return this.instance;
    }
    
    public Init()
    {
    }

    public SetUserID(uid: number)
    {
        this._uid = uid;
    }

    public GetUserID(): number
    {
        return this._uid;
    }

    public SetAddScore(addScore: number)
    {
        this._addScore = addScore;
    }

    public SetAddGold(addGold: number)
    {
        this._addGold = addGold;
    }

    public GetAddScore(): number
    {
        return this._addScore;
    }

    public GetAddGold(): number
    {
        return this._addGold;
    }

    public SetBZLPNum(num: number)
    {
        this._bzlpNum = num;
    }

    public GetBZLPNum(): number
    {
        return this._bzlpNum;
    }

    public GetKeepStaroCount(): number
    {
        return this._keepStarCount;
    }

    public SetKeepStarCount(count: number)
    {
        this._keepStarCount = count;
    }

    public SetReceiveGiftCount(count: number)
    {
        this._receiveGiftCount = count;
    }

    public GetReceiveGiftCount(): number
    {
        return this._receiveGiftCount;
    }

    public SetCurLevelID(value: number)
    {
        this._curLevelID = value;
    }

    public SetKillNum(value: number)
    {
        this._killNum = value;
    }

    // public SetGoldNum(value: number)
    // {
    //     this._goldNum = value;
    // }

    public SetCurStarNum(value: number)
    {
        this._curStarNum = value;
    }

    // public SetUnlockCarID(cars: Array<number>)
    // {
    //     this._unlockCarID = cars;
    // }

    public SetNickName(nickname: string)
    {
        this._nickName = nickname;
    }

    public SetHeadImg(_headImg: string)
    {
        this._headImg = _headImg;
    }

    public SetCurLevelNum(num: number)
    {
        this._curLevelNum = num;
    }

    public SetUseCarID(id: number)
    {
        this._usedCarID = id;
    }

    public SetUseMotionID(id: number)
    {
        this._usedMotionID = id;
    }

    public GetUsedMotionID(): number
    {
        return this._usedMotionID;
    }

    // public AddGoldNum(value: number)
    // {
    //     this._goldNum += value;
    // }

    public AddCurStarNum(value: number)
    {
        this._curStarNum += value;
    }

    // public AddUnlockCarID(value: number)
    // {
    //     this._unlockCarID.push(value);
    // }

    public AddCurLevelID(value: number)
    {
        this._curLevelID += value;
    }

    public GetCurLevelID(): number
    {
        return this._curLevelID;
    }

    public GetKillNum(): number
    {
        return this._killNum;
    }

    // public GetGoldNum(): number
    // {
    //     return this._goldNum;
    // }

    // public GetUnlockCarID(): Array<number>
    // {
    //     return this._unlockCarID;
    // }

    public GetCurStarNum(): number
    {
        return this._curStarNum;
    }

    public GetNickName(): string
    {
        return this._nickName;
    }

    public GetHeadImg(): string
    {
        return this._headImg;
    }

    public GetCurLevelNum(): number
    {
        return this._curLevelNum;
    }

    public GetUseCarID(): number
    {
        return this._usedCarID;
    }

    public SetSignInData(signInData: number): void
    {
        this._signInData = signInData;
    }

    public GetSignInData(): number
    {
        return this._signInData;
    }

    public SetTaskData(taskData: number): void
    {
        this._taskData = taskData;
    }

    public GetTaskData(): number
    {
        return this._taskData;
    }

    public SetOnlineRewardTime(value: number): void
    {
        this._onlineRewardTime = value;
    }

    public GetOnlineRewardTime(): number
    {
        return this._onlineRewardTime;
    }

    public SetDayIndex(day: number): void
    {
        this._dayIndex = day;
    }

    public GetDayIndex(): number
    {
        return this._dayIndex;
    }

    public SetTimeLimitItemID(itemID: number): void
    {
        this._timeLimitItemID = itemID;
    }

    public GetTimeLimitItemID(): number
    {
        return this._timeLimitItemID;
    }

    public SetTimeLimitRemainTime(time: number): void
    {
        this._timeLimitRemainTime = time;
    }

    public GetTimeLimitRemainTime(): number
    {
        return this._timeLimitRemainTime;
    }
} 