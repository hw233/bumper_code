import EngineUtil from '../common/EngineUtil';
import FriendManager from '../Friend/FriendManager';
import FriendData from '../Friend/FriendData';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import FriendSystem from '../Friend/FriendSystem';
import PlayerMgr from '../manager/PlayerMgr';
import ScrollView from '../common/ScrollView';
import { MessageType } from '../common/EventCenter';
import WXHelper from '../common/WXHelper';
import { AtlasType } from '../common/EngineUtil';
import UIBase from './UIBase';

const {ccclass, property} = cc._decorator;

@ccclass
export default class FriendLayer extends UIBase
{
    //半透明遮罩层
    @property({type: cc.Node})
    protected maskLayer: cc.Node = null;

    //弹窗层
    @property({type: cc.Node})
    protected popupLayer: cc.Node = null;

    //动画
    @property(cc.Animation)
    protected m_LayerAnim: cc.Animation = null;

    //标题节点
    @property(cc.Node)
    private m_TitleNode = null;

    @property(cc.Node)
    private m_FriendToggle = null;

    @property(cc.Node)
    private m_RecentlyToggle = null;

    @property(cc.ScrollView)
    private m_FriendScrollView = null;

    @property(cc.Node)
    private m_FriendContent: cc.Node = null;

    @property(cc.Node)
    private m_FriendBtn = null;

    @property(cc.Node)
    private m_OpenImg = null;

    @property(cc.Node)
    private m_CloseImg = null;

    @property(ScrollView)
    private m_ScrollView: ScrollView = null;

    private m_IsShow = false;

    private m_FriendUIList = null;

    private m_Type = null;  //类型 1：好友列表 2：最近玩家列表

    protected m_IsPlaying: boolean = null;

    protected m_IsRemove: boolean = null;

    public OnInit()
    {
        super.OnInit();

        this.m_FriendUIList = [];

        this.m_IsPlaying = false;
        this.m_IsRemove = false;

        this.maskLayer.active = false;
        this.popupLayer.active = false;

        this.m_OpenImg.active = true;
        this.m_CloseImg.active = false;

        this.registerClickEvent(this.m_FriendToggle, this.friendClicked.bind(this) );
        this.registerClickEvent(this.m_RecentlyToggle, this.recentlyClicked.bind(this) );
        this.registerClickEvent(this.m_FriendBtn, this.friendBtnClicked.bind(this) );

        this.registerEvent( MessageType.Refresh_Friend_View, this.refreshList.bind(this) );

        //阻止事件向下传递
        if(this.maskLayer.getComponent(cc.BlockInputEvents) === null)
        {
            this.maskLayer.addComponent(cc.BlockInputEvents);
        }

        this.registerTouchEndEvent(this.maskLayer, this.clickMaskLayer.bind(this) );
        this.registerTouchEndEvent(this.popupLayer, function(){});

        this.m_LayerAnim.on("finished", this.playFinished.bind(this));
    }

    public OnDestroy()
    {
        super.OnDestroy();

        this.m_FriendContent.removeAllChildren();
        this.m_FriendUIList.length = 0;
    }

    public ShowView()
    {
        super.ShowView();

        this.popupLayer.active = true;
    }

    public HideView()
    {
        super.HideView();

        this.popupLayer.active = false;
    }

    public RemoveView()
    {
        super.RemoveView();

        this.node.destroy();
    }

    private openView()
    {
        this.m_CloseImg.active = true;
        this.m_OpenImg.active = false;

        this.m_IsPlaying = true;
        this.m_IsShow = true;

        this.maskLayer.active = true;

        if(this.m_LayerAnim)
        {
            this.m_LayerAnim.play("FriendOpenAnim");
        }

        if(this.m_Type == 2)
        {
            this.showRecentlyList();
        }
        else
        {
            this.showFriendList();
        }
    }

    private closeView(isEffect: boolean = false)
    {
        this.m_IsPlaying = true;
        this.m_IsShow = false;

        this.maskLayer.active = false;

        if(this.m_LayerAnim)
        {
            this.m_LayerAnim.play("FriendCloseAnim");
        }
    }

    private friendBtnClicked()
    {
        if(this.m_IsPlaying)
        {
            return;
        }

        if(this.m_IsShow)
        {
            this.closeView();
        }
        else
        {
            //如果当前没有好友信息，先从服务器获取
            if(FriendManager.GetInstance().IsHasData() === false)
            {
                Socket_SendMgr.GetFriendSend().SendGetFriendList();
            }

            this.openView();
        }
    }

    private friendClicked()
    {
        if(this.m_Type === 1)
        {
            return;
        }

        this.showFriendList();
    }

    private recentlyClicked()
    {
        if(this.m_Type === 2)
        {
            return;
        }

        this.showRecentlyList();
    }

    //显示好友列表
    private showFriendList()
    {
        this.m_Type = 1;
        EngineUtil.SetNodeTextureByType(AtlasType.AT_FriendLayer, this.m_TitleNode, "friendTitle");

        let data = [];

        let count: number = FriendManager.GetInstance().GetMyFriendCount();

        for(let i = 0; i < count; i ++)
        {
            let friendData: FriendData = FriendManager.GetInstance().GetMyFriendByIndex(i);
            if(friendData)
            {
                let friendID: number = friendData.getUserID();

                if(PlayerMgr.IsSelf(friendID))
                {
                    continue;
                }
    
                data.push(friendID);
            }
        }

        this.friendListSort(data);

        this.m_ScrollView.init(data, 'FriendItemView');
    }

    //显示最近玩家列表
    private showRecentlyList()
    {
        this.m_Type = 2;
        EngineUtil.SetNodeTextureByType(AtlasType.AT_FriendLayer, this.m_TitleNode, "recentlyTitle");

        FriendSystem.GetInstance().GetRecentlyPlayerList();

        let data = [];

        let list = FriendManager.GetInstance().GetRecentlyPlayerList();

        for(let i = 0; i < list.length; i ++)
        {
            let playerID: number = list[i];

            if(PlayerMgr.IsSelf(playerID))
            {
                continue;
            }

            data.push(playerID);
        }

        this.m_ScrollView.init(data, 'FriendItemView');
    }


    protected playFinished()
    {
        this.m_IsPlaying = false;

        if(this.m_IsShow === false)
        {
            this.m_CloseImg.active = false;
            this.m_OpenImg.active = true;
    
            this.m_FriendScrollView.node.active = false;
        }

        if(this.m_IsRemove)
        {
            this.node.destroy();
        }
    }

    public refreshList()
    {
        if(this.m_Type == 1)
        {
            this.showFriendList();
        }
        else if(this.m_Type == 2)
        {
            this.showRecentlyList();
        }
    }

    //好友列表排序
    private friendListSort(friendList: Array<number>)
    {
        let friendSort = function(a: number, b: number)
        {
            let aData: FriendData = FriendManager.GetInstance().GetMyFriend(a);
            let bData: FriendData = FriendManager.GetInstance().GetMyFriend(b);

            if(aData && bData)
            {
                if(bData.getOnLineState() != aData.getOnLineState())
                {
                    return Number(bData.getOnLineState()) - Number(aData.getOnLineState());
                }
                else if(bData.getGradingID() != aData.getGradingID())
                {
                    return bData.getGradingID() - aData.getGradingID();
                }
                else
                {
                    return bData.getCurStarNum() - aData.getCurStarNum();
                }
            }
            return 0;
        };

        friendList.sort(friendSort);
    }

    private clickMaskLayer()
    {
        this.closeView();
    }
}
    


