//好友数据
export default class FriendApply
{
    private m_UserID: number        = null; //用户id
    private m_NickName: string      = null; //用户昵称
    private m_AvatarUrl: string     = null; //用户头像

    constructor()
    {
        this.m_UserID = 0;
        this.m_NickName = "";
        this.m_AvatarUrl = "";
    }

    public copyCtor(data: FriendApply)
    {
        if(data)
        {
            this.m_UserID = data.getUserID();
            this.m_NickName = data.getNickName();
            this.m_AvatarUrl = data.getAvatarUrl();
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

    public setNickName(nickname: string)
    {
        this.m_NickName = nickname;
    }

    public getNickName(): string
    {
        return this.m_NickName;
    }

    //反序列化一个好友申请
    public static Deserialization(data: any): FriendApply
    {
        let userID: number = parseInt(data.friend_id) || 0;
        let avatarUrl: string = data.head_picurl || "";
        let nickname: string = data.nick_name || "";

        let friendData: FriendApply = new FriendApply();
        friendData.setUserID(userID);
        friendData.setAvatarUrl(avatarUrl);
        friendData.setNickName(nickname);

        return friendData;
    }
}