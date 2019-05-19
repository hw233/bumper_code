//好友数据
export default class FriendData
{
    private m_UserID: number        = null; //用户id
    private m_NickName: string      = null; //用户昵称
    private m_AvatarUrl: string     = null; //用户头像
    private m_OnLineState: boolean  = null; //用户在状态 0：离线 1：在线 
    private m_InGame: boolean       = null; //用户是否在游戏中
    private m_GradingID: number     = null; //用户段位id
    private m_CurStarNum: number    = null; //当前星星数量
    private m_GameTime: number      = null; //游戏时间(单位：分)
    private m_IsFriend: boolean     = null; //是否为好友

    constructor()
    {
        this.m_UserID = 0;
        this.m_NickName = "";
        this.m_AvatarUrl = "";
        this.m_OnLineState = false;
        this.m_InGame = false;
        this.m_GameTime = 0;
        this.m_CurStarNum = 0;
        this.m_IsFriend = true;
        this.m_GradingID = 0;
    }

    public copyCtor(data: FriendData)
    {
        if(data)
        {
            this.m_UserID = data.getUserID();
            this.m_NickName = data.getNickName();
            this.m_AvatarUrl = data.getAvatarUrl();
            this.m_OnLineState = data.getOnLineState();
            this.m_InGame = data.getInGame();
            this.m_GameTime = data.getGameTime();
            this.m_IsFriend = data.getIsFriend();
            this.m_CurStarNum = data.getCurStarNum();
            this.m_GradingID = data.getGradingID();
        }
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

    public setOnLineState(onLine: boolean )
    {
        this.m_OnLineState = onLine;
    }

    public getOnLineState(): boolean 
    {
        return this.m_OnLineState;
    }

    public setNickName(nickname: string)
    {
        this.m_NickName = nickname;
    }

    public getNickName(): string
    {
        return this.m_NickName;
    }

    public setInGame(inGame: boolean)
    {
        this.m_InGame = inGame;
    }

    public getInGame(): boolean
    {
        return this.m_InGame;
    }

    public setGradingID(grading: number)
    {
        this.m_GradingID = grading;
    }

    public getGradingID(): number
    {
        return this.m_GradingID;
    }

    public setCurStarNum(curStartNum: number)
    {
        this.m_CurStarNum = curStartNum;
    }

    public getCurStarNum(): number
    {
        return this.m_CurStarNum;
    }

    public setGameTime(gameTime: number)
    {
        this.m_GameTime = gameTime;
    }

    public getGameTime(): number
    {
        return this.m_GameTime;
    }

    public setIsFriend(isFriend: boolean)
    { 
        this.m_IsFriend = isFriend;
    }

    public getIsFriend(): boolean
    {
        return this.m_IsFriend;
    }

    //反序列化一个好友信息
    public static Deserialization(data: any): FriendData
    {
        let userID: number = parseInt(data.friend_id) || 0;
        let avatarUrl: string = data.head_picurl || "";
        let nickname: string = data.nick_name || "";
        let onlineState: number = parseInt(data.online) || 0;
        let gradingID: number = parseInt(data.granding_id) || 0;
        let starNum: number = parseInt(data.cur_star_sum) || 0;

        if(!CC_WECHATGAME)
        {
            nickname = userID.toString();
        }

        let friendData: FriendData = new FriendData();
        friendData.setUserID(userID);
        friendData.setAvatarUrl(avatarUrl);
        friendData.setNickName(nickname);
        friendData.setOnLineState(onlineState > 0);
        friendData.setInGame(onlineState === 2);
        friendData.setGradingID(gradingID);
        friendData.setCurStarNum(starNum);

        return friendData;
    }
}