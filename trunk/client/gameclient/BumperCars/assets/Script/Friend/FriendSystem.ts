import EventCenter from '../common/EventCenter';
import { MessageType } from '../common/EventCenter';
import EngineUtil from '../common/EngineUtil';
import GameUI from '../manager/UIManager';
import FriendManager from './FriendManager';
import SceneMgr, { SceneType } from '../manager/SceneMgr';
import FriendApply from './FriendApply';
import GameLocalDataMgr from '../manager/GameLocalDataMgr';
import GameInviteView from '../UI/GameInviteView';
import FriendApplyView from '../UI/FriendApplyView';
import { UIType } from '../UI/UIDefine';

//好友系统
export default class FriendSystem
{
    private static instance: FriendSystem              = null;

    private static EventTag: string                    = "FriendSystem";

    constructor()
    {
        
    }

    public static GetInstance(): FriendSystem
    {
        if(this.instance === null)
        {
            this.instance = new FriendSystem();
        }
        return this.instance;
    }

    public Init()
    {
        FriendManager.GetInstance().Init();

        EventCenter.RegisterEvent(FriendSystem.EventTag, MessageType.Game_Invite_Event, this.GameInvite.bind(this) );
        EventCenter.RegisterEvent(FriendSystem.EventTag, MessageType.Friend_Apply_Event, this.FriendApply.bind(this) );
        EventCenter.RegisterEvent(FriendSystem.EventTag, MessageType.Add_Friend_Event, this.AddFriend.bind(this) );
        EventCenter.RegisterEvent(FriendSystem.EventTag, MessageType.Get_Friend_List_Event, this.GetFriendList.bind(this) );
        EventCenter.RegisterEvent(FriendSystem.EventTag, MessageType.Refresh_Friend_Info_Event, this.RefreshFriendInfo.bind(this) );
    }

    public static Destroy()
    {
        EventCenter.RemoveEvent(FriendSystem.EventTag);

        if(this.instance)
        {
            this.instance.Destroy();
            this.instance = null;
        }
    }

    //获取最近的玩家列表
    public GetRecentlyPlayerList()
    {
        let datas: Array<Object> = GameLocalDataMgr.GetInstance().GetRecentlyPlayer();
        if(datas)
        {
            FriendManager.GetInstance().InitRecentlyPlayerList(datas);
        }
    }

    //处理缓存的好友申请
    public HandleCacheFriendApply()
    {
        let friendApply: FriendApply = FriendManager.GetInstance().PopFriendApply();
        while(friendApply)
        {
            this.FriendApply(friendApply);
            friendApply = FriendManager.GetInstance().PopFriendApply();
        }
    }

    //收到游戏邀请
    private GameInvite(data)
    {
        if(data == null)
        {
            return;
        }

        GameUI.ShowRepeatableUI(UIType.UT_GameInviteView, data);
    }

    //收到好友申请
    private FriendApply(data: FriendApply)
    {
        if(data == null)
        {
            return;
        }

        if(
            SceneMgr.GetCurSceneType() === SceneType.LoadingScene ||
            SceneMgr.GetCurSceneType() === SceneType.LogoScene    ||
            SceneMgr.GetCurSceneType() === SceneType.DLDGameScene || 
            SceneMgr.GetCurSceneType() === SceneType.ZDZGameScene ||
            SceneMgr.GetCurSceneType() === SceneType.GameScene
        )
        {
            FriendManager.GetInstance().AddFriendApplyToCache(data);
            return;
        }

        GameUI.ShowRepeatableUI(UIType.UT_FriendApplyView, {
            uid: data.getUserID(),
            nickName: data.getNickName(),
            headUrl: data.getAvatarUrl(),
        });
    }

    //添加好友
    private AddFriend(friendID: number)
    {
        EventCenter.DispatchEvent(MessageType.Refresh_Friend_View);
        EventCenter.DispatchEvent(MessageType.Refresh_Friend_Item, friendID);
    }

    //刷新好友信息
    private RefreshFriendInfo(friendID: number)
    {
        EventCenter.DispatchEvent(MessageType.Refresh_Friend_View);
    }

    //获取好友列表成功
    private GetFriendList()
    {
        EventCenter.DispatchEvent(MessageType.Refresh_Friend_View);
    }

    public Destroy()
    {

    }
}