import EngineUtil from '../common/EngineUtil';
import PopUpBase from '../UI/PopUpBase';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import { GameModel } from '../GameDefine';
import GameUI from '../manager/UIManager';

const {ccclass, property} = cc._decorator;

@ccclass
export default class GameInviteView extends PopUpBase
{
    @property({type: cc.Node})
    private m_AgreeBtn: cc.Node = null;

    @property({type: cc.Node})
    private m_TimeNode: cc.Node = null;

    @property({type: cc.Node})
    private m_NickName: cc.Node = null;

    @property({type: cc.Node})
    private m_HeadImg: cc.Node = null;

    @property({type: cc.Node})
    private m_InviteText_1: cc.Node = null;

    @property({type: cc.Node})
    private m_InviteText_2: cc.Node = null;

    private m_InviteTime: number = null

    private m_TeamID: number = null;
    private m_Mode: number = null;

    public OnInit () 
    {
        super.OnInit();

        this.m_Mode = 0;
        this.m_TeamID = 0;
        this.m_InviteTime = 15;

        this.m_InviteText_1.active = false;
        this.m_InviteText_2.active = false;

        EngineUtil.RegBtnClickEvent(this.m_AgreeBtn, this.agreeBtnClicked.bind(this));
    }

    public OnEnter () 
    {
        super.OnEnter();

        this.schedule(this.refreshTime, 1);
    }

    public OnExit()
    {
        super.OnExit();

        this.unschedule(this.refreshTime);
    }

    public RefreshView()
    {
        super.RefreshView();

        let uid: number = this.param.friend_id;
        let teamID: number = this.param.team_id;
        let nickName: string = this.param.nick_name;
        let headUrl: string = this.param.head_picurl;
        let model: number = this.param.model;

        this.m_TeamID = teamID;
        this.m_Mode = model;

        EngineUtil.SetNodeText(this.m_NickName, nickName);
        EngineUtil.LoadRemoteImg(this.m_HeadImg, headUrl);

        this.m_InviteText_1.active = model == GameModel.GM_DLDMode;
        this.m_InviteText_2.active = model == GameModel.GM_DQZMode;
    }

    private refreshTime()
    {
        this.m_InviteTime --;

        if(this.m_InviteTime < 0)
        {
            GameUI.HideUI(this.GetType());
        }

        EngineUtil.SetNodeText(this.m_TimeNode, this.m_InviteTime.toString());
    }

    private agreeBtnClicked()
    {
        Socket_SendMgr.GetBettleSend().SendInviteJoinRoomMessage(this.m_TeamID, this.m_Mode);

        GameUI.HideUI(this.GetType());
    }
}