import EngineUtil, { AtlasType } from '../common/EngineUtil';
import GlobalVar from '../GlobalVar';
import PopUpBase from '../UI/PopUpBase';
import ScrollView from '../common/ScrollView';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import WXHelper from '../common/WXHelper';
import { MessageType } from '../common/EventCenter';
import RankItem from '../component/RankItem';
import GameLog from '../Log/LogTool';

export enum RankList
{
    WorldRankList = 1,
    GroupRankList = 2,
    FriendRankList = 3,
};

export enum RankType
{
    KillType = 1,        //击杀排名
    GradingType = 2,     //段位排名
};


const {ccclass, property} = cc._decorator;

@ccclass
export default class RanklistLayer extends PopUpBase
{
    @property({type: cc.Prefab})
    private rankItemPrefab: cc.Prefab = null;

    @property({type: cc.Node})
    private scrollViewContent: cc.Node = null;

    @property({type: cc.Node})
    private groupRankList: cc.Node = null;

    @property({type: cc.Node})
    private titleNode: cc.Node = null;

    @property({type: cc.Node})
    private worldImg: cc.Node = null;

    @property({type: cc.Node})
    private friendImg: cc.Node = null;

    @property({type: cc.Node})
    private killRankNode: cc.Node = null;

    @property({type: cc.Node})
    private gradingRankNode: cc.Node = null;

    @property(ScrollView)
    private m_ScrollView: ScrollView = null;

    @property({type: cc.Sprite})
    private rankingScrollView: cc.Sprite = null;

    private selfItemNode: cc.Node = null;

    //世界排行榜数据
    private worldRankListData: any = null;

    //自己的数据
    private selfData: any = null;

    private tex: cc.Texture2D = null;

    private type: number = null; //排行榜类型 1：世界 2：群

    private rankType: number = null; //排名类型 1: 击杀 2: 段位
    
    public OnInit () 
    {
        super.OnInit();

        this.rankType = RankType.KillType;
        this.worldRankListData = [];

        this.rankingScrollView.spriteFrame = new cc.SpriteFrame();

        this.tex = new cc.Texture2D();

        this.registerClickEvent(this.groupRankList, this.groupRankListBtnClicked.bind(this));
        this.registerClickEvent(this.closeBtn, this.closeBtnClicked.bind(this));
        this.registerClickEvent(this.killRankNode, this.killBtnClicked.bind(this));
        this.registerClickEvent(this.gradingRankNode, this.gradingBtnClicked.bind(this));

        this.registerTouchEndEvent(this.worldImg, this.switchWorldRanklist.bind(this));
        this.registerTouchEndEvent(this.friendImg, this.switchFriendRanklist.bind(this));
        
        this.registerEvent(MessageType.StartScene_GetRankList_Suc, this.getRanklistResp.bind(this) );
    }

    public OnEnter () 
    {
        super.OnEnter();

        this.dispatchEvent(MessageType.GameLoopBtn_Hide_Event);
    }

    public OnExit()
    {
        super.OnExit();

        if(this.worldRankListData)
        {
            this.worldRankListData.length = 0;
            this.worldRankListData = null;
        }

        this.selfData = null;

        WXHelper.ClearSharedCanvas();

        this.dispatchEvent(MessageType.GameLoopBtn_Show_Event);
    }

    //移除
    public OnDestroy()
    {
        super.OnDestroy();

        this.worldRankListData.length = 0;
        this.selfData = null;
        this.tex = null;

        WXHelper.ClearSharedCanvas();
    }

    public FrameOnMove(dt: number)
    {
        super.FrameOnMove(dt);

        this.updateSubDomainCanvas();
    }

    //显示 type:1 世界排行 type:2群排行 type: 3 好友排行
    protected OnShowFinished()
    {
        this.type = this.param;

        //显示排行榜
        if(this.type === RankList.WorldRankList)
        {
            this.switchWorldRanklist();
        }else if(this.type === RankList.GroupRankList)
        {
            this.showGroupRankList();
        }else if(this.type === RankList.FriendRankList)
        {
            this.switchFriendRanklist();
        }
    }

    //查看群排行按钮点击
    private groupRankListBtnClicked()
    {
        WXHelper.GroupRankListShare();
    }

    //击杀排名按钮点击
    private killBtnClicked()
    {
        if(this.rankType === RankType.KillType)
        {
            return;
        }

        this.worldRankListData.length = 0;
        this.selfData = null;

        this.rankType = RankType.KillType;

        if(this.type === RankList.WorldRankList)
        {
            this.showWorldRankList();
        }else if(this.type === RankList.GroupRankList)
        {
            this.showGroupRankList();
        }else if(this.type === RankList.FriendRankList)
        {
            this.showFriendRankList();
        }
    }

    //段位排名按钮点击
    private gradingBtnClicked()
    {
        if(this.rankType === RankType.GradingType)
        {
            return;
        }

        this.worldRankListData.length = 0;
        this.selfData = null;

        this.rankType = RankType.GradingType;

        if(this.type == RankList.WorldRankList)
        {
            this.showWorldRankList();
        }else if(this.type == RankList.GroupRankList)
        {
            this.showGroupRankList();
        }else if(this.type == RankList.FriendRankList)
        {
            this.showFriendRankList();
        }
    }

    //切换世界排行
    private switchWorldRanklist()
    {
        this.type = RankList.WorldRankList;

        this.showWorldRankList();

        EngineUtil.SetNodeTextureByType(AtlasType.AT_RankListLayer, this.worldImg, "worldImg1");
        EngineUtil.SetNodeTextureByType(AtlasType.AT_RankListLayer, this.friendImg, "friendImg2");
        EngineUtil.SetNodeTextureByType(AtlasType.AT_RankListLayer, this.titleNode, "worldTitle");
    }

    //切换好友排行
    private switchFriendRanklist()
    {
        this.type = RankList.FriendRankList;

        this.showFriendRankList();

        EngineUtil.SetNodeTextureByType(AtlasType.AT_RankListLayer, this.worldImg, "worldImg2");
        EngineUtil.SetNodeTextureByType(AtlasType.AT_RankListLayer, this.friendImg, "friendImg1");
        EngineUtil.SetNodeTextureByType(AtlasType.AT_RankListLayer, this.titleNode, "friendTitle");
    }
    
    //展示好友排行榜
    private showFriendRankList()
    {
        this.clearRankList();

        if(this.rankingScrollView)
        {
            this.rankingScrollView.node.active = true;
        }

        WXHelper.FetchFriendRankList(this.rankType);
    }

    //展示群排行榜
    private showGroupRankList(){
        if(this.rankingScrollView)
        {
            this.rankingScrollView.node.active = true;
        }
        
        this.worldImg.active = false;
        this.friendImg.active = false;

        EngineUtil.SetNodeTextureByType(AtlasType.AT_RankListLayer, this.titleNode, "groupTitle");

        WXHelper.GroupRankList(this.rankType);
    }

    //展示世界排行榜
    private showWorldRankList()
    {
        this.clearRankList();

        this.scrollViewContent.active = true;

        if(this.worldRankListData && this.selfData)
        {
            this.refreshRankList();
        }
        else
        {
            //获取排行榜数据
            Socket_SendMgr.GetCommonSend().SendGetRankListMessage(this.rankType);
        }
    }

    private refreshRankList()
    {
        GlobalVar.rankType = this.rankType;

        if(this.worldRankListData && this.worldRankListData.length >= 30)
        {
            this.m_ScrollView.init(this.worldRankListData, "RankItem");
        }

        //自己的排名
        if(this.selfItemNode === null)
        {
            if(this.selfData && this.selfData.rank !== undefined)
            {
                let userItem = cc.instantiate(this.rankItemPrefab);
                let rankItem: RankItem = userItem.getComponent('RankItem');
                if(rankItem)
                {
                    rankItem.initView(this.selfData, true);
                    userItem.y = -320;
                    userItem.tag = 1000;
                    this.popupLayer.addChild(userItem);
                }
                this.selfItemNode = userItem;
            }
        }
    }

    private clearRankList()
    {
        if(this.popupLayer.getChildByTag(1000))
        {
            this.popupLayer.getChildByTag(1000).destroy();
        }

        this.selfItemNode = null;

        if(this.scrollViewContent)
        {
            this.scrollViewContent.removeAllChildren();

            this.scrollViewContent.active = false;
        }

        if(this.rankingScrollView)
        {
            this.rankingScrollView.node.active = false;
        }

    }

    // 刷新子域的纹理
    private updateSubDomainCanvas() 
    {
        if (window["sharedCanvas"] != undefined && this.tex && this.rankingScrollView.node.active) 
        {
            this.tex.initWithElement(window["sharedCanvas"]);
            this.tex.handleLoadedTexture();

            this.rankingScrollView.spriteFrame.setTexture(this.tex);
        }
    }

    private getRanklistResp(data: any): void
    {
        this.worldRankListData = this.worldRankListData.concat(data.data);

        if(data.user_info)
        {
            this.selfData = data.user_info;
        }
        
        this.refreshRankList();
    }
}