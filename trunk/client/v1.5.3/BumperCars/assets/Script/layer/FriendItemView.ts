import FriendData from '../Friend/FriendData';
import EngineUtil, { AtlasType } from '../common/EngineUtil';
import ComponentBase from '../common/ComponentBase';
import FriendManager from '../Friend/FriendManager';
import SceneMgr, { SceneType } from '../manager/SceneMgr';
import { MessageType } from '../common/EventCenter';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import PlayerMgr from '../manager/PlayerMgr';
import { GradingSystem } from '../Grading/GradingSystem';
import WXHelper from '../common/WXHelper';
import GameData from '../data/GameData';
import GlobalVar from '../GlobalVar';

const {ccclass, property} = cc._decorator;

@ccclass
export default class FriendItemView extends ComponentBase
{
    private static InviteIntervalTime:number = 30;

    //头像节点
    @property(cc.Node)
    private m_HeadImgNode: cc.Node = null;

    //昵称节点
    @property(cc.Node)
    private m_NickNameNode: cc.Node = null;

    //段位节点
    @property(cc.Node)
    private m_GradingNode: cc.Node = null;

    //状态标识节点
    @property(cc.Node)
    private m_StateNode: cc.Node = null;

    //邀请按钮节点
    @property(cc.Node)
    private m_InviteBtn: cc.Node = null;

    //申请按钮节点
    @property(cc.Node)
    private m_ApplyBtn: cc.Node = null;

    //邀请弹框节点
    @property(cc.Node)
    private m_InviteFrame: cc.Node = null;

    private m_UserID: number = null;

    protected onLoad()
    {
        this.m_UserID = 0;
        this.hideAllBtn();

        EngineUtil.RegBtnClickEvent( this.m_InviteBtn, this.inviteBtnClicked.bind(this) );
        EngineUtil.RegBtnClickEvent( this.m_ApplyBtn, this.applyBtnClicked.bind(this) );

        this.registerEvent(MessageType.Refresh_Friend_Item, this.refreshItem.bind(this));
    }

    protected start()
    {

    }

    public initView( friendID: number )
    {
        let friendData: FriendData = FriendManager.GetInstance().GetData(friendID);
        if(friendData)
        {
            this.m_UserID = friendData.getUserID();

            if(this.m_GradingNode)
            {
                let pGradingNameNode = this.m_GradingNode.getChildByName("GradingName");
                let pStarNumNode = this.m_GradingNode.getChildByName("StarNum");

                let nStarNum: number = friendData.getCurStarNum();
                let nGrandingID: number = friendData.getGradingID();
                // if(nGrandingID != 31)
                // {
                //     nStarNum = (nGrandingID - 1) % 5 * 5 + nStarNum;
                // }

                EngineUtil.SetNodeText(pStarNumNode, nStarNum.toString());
                EngineUtil.SetNodeText(pGradingNameNode, GradingSystem.GetGradingName(nGrandingID));
            }

            this.refreshUI( friendID );

            EngineUtil.SetNodeText(this.m_NickNameNode, friendData.getNickName());
            EngineUtil.LoadRemoteImg(this.m_HeadImgNode, friendData.getAvatarUrl());
        }
    }

    //刷新UI
    public refreshUI(friendID: number)
    {
        let friendData = FriendManager.GetInstance().GetData(friendID);
        if(friendData)
        {
            let isFriend: boolean = friendData.getIsFriend();
            let inGame: boolean = friendData.getInGame();
            let isOnline: number = friendData.getOnLineState();
    
            if(SceneMgr.GetCurSceneType() == SceneType.StartScene)
            {
                if(isFriend)
                {
                    this.refreshStateUI(inGame, isOnline === 1);
                }
                else
                {
                    this.showApplyBtn();
                }
            }
            else
            {
                if(isFriend)
                {
                    if(inGame)
                    {
                        this.refreshStateUI(inGame, true);
                    }else
                    {
                        this.showInviteBtn();
                    }
                }
                else
                {
                    this.showApplyBtn();
                }
            }
        }
    }

    private refreshItem(friendID: number)
    {
        if(friendID == this.m_UserID)
        {
            this.refreshUI(friendID);
        }
    }

    private refreshStateUI(inGame: boolean, isOnline: boolean)
    {
        this.hideAllBtn();

        this.m_StateNode.active = true;

        if(inGame)
        {
            EngineUtil.SetNodeTextureByType(AtlasType.AT_FriendLayer, this.m_StateNode, "ingame");
        }
        else
        {
            if(isOnline)
            {
                EngineUtil.SetNodeTextureByType(AtlasType.AT_FriendLayer, this.m_StateNode, "online");
            }
            else
            {
                EngineUtil.SetNodeTextureByType(AtlasType.AT_FriendLayer, this.m_StateNode, "leaveline");
            }
        }
        
    }

    //邀请按钮点击
    private inviteBtnClicked()
    {
        EngineUtil.SetButtonInteractable(this.m_InviteBtn, false);

        this.scheduleOnce(this.showInviteBtn, 15);

        if(FriendManager.GetInstance().IsOnline(this.m_UserID))
        {
            Socket_SendMgr.GetBettleSend().SendInviteFriendJoinRoomMessage(this.m_UserID);
        }
        else
        {
            WXHelper.InviteJoinShare(GameData.GetRoomID(), GlobalVar.gameModel, null);
        }

    }

    //申请按钮点击
    private applyBtnClicked()
    {
        this.showAlreadyApplyBtn();

        this.scheduleOnce(this.showApplyBtn, FriendItemView.InviteIntervalTime);

        Socket_SendMgr.GetFriendSend().SendApplyFriendMessage(this.m_UserID);
    }

    //显示申请按钮
    private showApplyBtn()
    {
        if(FriendManager.GetInstance().IsMyFriend(this.m_UserID))
        {
            return;
        }

        this.hideAllBtn();

        this.m_ApplyBtn.active = true;

        EngineUtil.SetNodeTextureByType(AtlasType.AT_FriendLayer, this.m_ApplyBtn, "shenqinghaoyou");
        EngineUtil.SetButtonInteractable(this.m_ApplyBtn, true);
    }

    //显示已申请按钮
    private showAlreadyApplyBtn()
    {
        if(FriendManager.GetInstance().IsMyFriend(this.m_UserID))
        {
            return;
        }

        EngineUtil.SetNodeTextureByType(AtlasType.AT_FriendLayer, this.m_ApplyBtn, "yishenqing");
        EngineUtil.SetButtonInteractable(this.m_ApplyBtn, false);
    }

    //显示邀请按钮
    private showInviteBtn(online: boolean = true)
    {
        this.hideAllBtn();

        if(SceneMgr.GetCurSceneType() === SceneType.StartScene)
        {
            return;
        }

        //不在房间内的玩家才可以邀请
        if(PlayerMgr.IsValid(this.m_UserID))
        {
            return;
        }
        
        this.m_InviteBtn.active = true;
        if(FriendManager.GetInstance().IsOnline(this.m_UserID))
        {
            EngineUtil.SetNodeTextureByType(AtlasType.AT_FriendLayer, this.m_InviteBtn, "youxiyaoqing");
        }
        else
        {
            EngineUtil.SetNodeTextureByType(AtlasType.AT_FriendLayer, this.m_InviteBtn, "wxInvite");
        }
        EngineUtil.SetButtonInteractable(this.m_InviteBtn, true);
    }

    private hideAllBtn()
    {
        this.m_ApplyBtn.active = false;
        this.m_InviteBtn.active = false;
        this.m_StateNode.active = false;
    }

    protected onDestroy()
    {
        super.onDestroy();
    }
}