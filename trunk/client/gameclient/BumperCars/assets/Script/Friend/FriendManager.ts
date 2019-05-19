import FriendData from './FriendData';
import FriendApply from './FriendApply';

//管理好友数据
export default class FriendManager
{
    private static instance: FriendManager              = null;

    private m_RecentlyPlayerList: Object                = null;        //最近玩过的玩家列表

    private m_FriendList: Object                        = null;        //好友列表

    private m_FriendUserIDList: Array<number>           = null;
    
    private m_RecentlyPlayerUserIDList: Array<number>   = null;

    private m_IsHaveData: boolean                       = null;

    private m_CacheFriendApplyList: Array<FriendApply>  = null; //缓存游戏过程中收到的好友申请, 游戏结束后显示出来

    constructor()
    {
        this.m_IsHaveData = false;

        this.m_RecentlyPlayerList = {};
        this.m_FriendList = {};

        this.m_FriendUserIDList = [];
        this.m_RecentlyPlayerUserIDList = [];
        this.m_CacheFriendApplyList = [];
    }

    public static GetInstance(): FriendManager
    {
        if(this.instance === null)
        {
            this.instance = new FriendManager();
        }
        return this.instance;
    }

    public static Destroy()
    {
        if(this.instance)
        {
            this.instance.Destroy();
            this.instance = null;
        }
    }

    public Init()
    {
        
    }

    public InitRecentlyPlayerList(datas: Array<Object>)
    {
        this.m_RecentlyPlayerList = null;
        this.m_RecentlyPlayerList = {};

        this.m_RecentlyPlayerUserIDList.length = 0;

        for(let i = 0, len = datas.length; i < len; i ++)
        {
            let data = datas[i];
            if(data)
            {
                let friendData: FriendData = FriendData.Deserialization(data);

                let isMyFriend: boolean = this.IsMyFriend(friendData.getUserID())

                friendData.setIsFriend(isMyFriend);

                this.m_RecentlyPlayerList[friendData.getUserID()] = friendData;
                this.m_RecentlyPlayerUserIDList.push(friendData.getUserID());
            }
        }

        // for(let key in data)
        // {
        //     if(key.indexOf("members") < 0)
        //     {
        //         continue;
        //     }

        //     let member = data[key];

        //     let friendData: FriendData = FriendData.Deserialization(member);

        //     this.m_RecentlyPlayerList[friendData.getUserID()] = friendData;
        //     this.m_RecentlyPlayerUserIDList.push(friendData.getUserID());
        // }
    }

    //添加一些好友到列表
    public AddFriendsToList(data: any)
    {
        for(let key in data)
        {
            if(key.indexOf("members") < 0)
            {
                continue;
            }

            let member = data[key];

            let friendData: FriendData = FriendData.Deserialization(member);

            this.m_FriendList[friendData.getUserID()] = friendData;
            this.m_FriendUserIDList.push(friendData.getUserID());
        }

        this.m_IsHaveData = true;
    }

    public AddFriendApplyToCache(data: FriendApply)
    {
        let friendApply: FriendApply = new FriendApply();
        if(friendApply)
        {
            friendApply.copyCtor(data);
            this.m_CacheFriendApplyList.push(friendApply);
        }
    }

    public GetCacheFriendApplyCount(): number
    {
        return this.m_CacheFriendApplyList.length;
    }

    //清空好友申请
    public ClearCacheFrinedApply(): void
    {
        if(this.m_CacheFriendApplyList)
        {
            this.m_CacheFriendApplyList.length = 0;
        }
    }

    public PopFriendApply(): FriendApply
    {
        if(this.m_CacheFriendApplyList.length > 0)
        {
            return this.m_CacheFriendApplyList.shift();
        }
        return null;
    }

    public RemoveRecentlyPlayer(userID: number)
    {
        for(let i = 0, len = this.m_RecentlyPlayerUserIDList.length; i < len; i++)
        {
            if(this.m_RecentlyPlayerUserIDList[i] == userID)
            {
                this.m_RecentlyPlayerUserIDList.splice(i, 1);
            }
        }

        if(this.m_RecentlyPlayerList[userID])
        {
            delete this.m_RecentlyPlayerList[userID];
        }
    }

    public AddFriendToMe(friendData: FriendData)
    {
        if(friendData)
        {
            let temp: FriendData = new FriendData();
            temp.copyCtor(friendData);

            this.m_FriendList[temp.getUserID()] = temp;

            this.m_FriendUserIDList.push(temp.getUserID());
        }
    }

    public GetMyFriend(userID: number): FriendData
    {
        if(this.m_FriendList[userID])
        {
            return this.m_FriendList[userID];
        }
        return null;
    }

    public GetRecentlyPlayer(userID: number)
    {
        if(this.m_RecentlyPlayerList[userID])
        {
            return this.m_RecentlyPlayerList[userID];
        }
        return null;
    }

    public GetData(userID: number)
    {
        if(this.IsMyFriend(userID))
        {
            return this.GetMyFriend(userID);
        }
        else
        {
            return this.GetRecentlyPlayer(userID);
        }
        return null;
    }

    public IsMyFriend(userID: number): boolean
    {
        if(this.m_FriendList[userID])
        {
            return true;
        }
        return false;
    }

    public GetMyFriendCount(): number
    {
        if(this.m_FriendUserIDList)
        {
            return this.m_FriendUserIDList.length;
        }
        return 0;
    }

    public GetRecentlyCount(): number
    {
        if(this.m_RecentlyPlayerUserIDList)
        {
            return this.m_RecentlyPlayerUserIDList.length;
        }

        return 0;
    }

    public GetMyFriendByIndex(index: number): FriendData
    {
        if(this.m_FriendUserIDList[index])
        {
            return this.m_FriendList[this.m_FriendUserIDList[index]];
        }
        return null;
    }

    public GetRecentlyPlayerByIndex(index: number): FriendData
    {
        if(this.m_RecentlyPlayerUserIDList[index])
        {
            return this.m_RecentlyPlayerList[this.m_RecentlyPlayerUserIDList[index]];
        }
        return null;
    }

    public GetMyFriendList()
    {
        return this.m_FriendUserIDList;
    }

    public GetRecentlyPlayerList()
    {
        return this.m_RecentlyPlayerUserIDList;
    }

    public IsHasData()
    {
        return this.m_IsHaveData;
    }

    public IsOnline(friendID: number)
    {
        let friendData: FriendData = this.GetMyFriend(friendID);
        if(friendData)
        {
            return friendData.getOnLineState();
        }
        return false;
    }

    public Destroy()
    {
        this.m_RecentlyPlayerList = null;
        this.m_FriendList = null;

        if(this.m_FriendUserIDList)
        {
            this.m_FriendUserIDList.length = 0;
            this.m_FriendUserIDList = null;
        }

        if(this.m_RecentlyPlayerUserIDList)
        {
            this.m_RecentlyPlayerUserIDList.length = 0;
            this.m_RecentlyPlayerUserIDList = null;
        }

        if(this.m_CacheFriendApplyList)
        {
            this.m_CacheFriendApplyList.length = 0;
            this.m_CacheFriendApplyList = null;
        }
    }
}