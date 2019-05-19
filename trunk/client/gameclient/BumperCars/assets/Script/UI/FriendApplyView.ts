import EngineUtil from '../common/EngineUtil';
import PopUpBase from '../UI/PopUpBase';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import GameUI from '../manager/UIManager';

const {ccclass, property} = cc._decorator;

@ccclass
export default class FriendApplyView extends PopUpBase
{
    @property({type: cc.Node})
    private m_AgreeBtn: cc.Node = null;

    @property({type: cc.Node})
    private m_NickName: cc.Node = null;

    @property({type: cc.Node})
    private m_HeadImg: cc.Node = null;

    private m_ApplyID: number = null;

    public OnInit() 
    {
        super.OnInit();

        this.m_ApplyID = 0;

        EngineUtil.RegBtnClickEvent(this.m_AgreeBtn, this.agreeBtnClicked.bind(this));
    }

    protected refreshView()
    {
        super.RefreshView();

        let uid: number = this.param.uid;
        let nickName: string = this.param.nickName;
        let headUrl: string = this.param.headUrl;

        this.m_ApplyID = uid;

        EngineUtil.SetNodeText(this.m_NickName, nickName);
        EngineUtil.LoadRemoteImg(this.m_HeadImg, headUrl);
    }

    private agreeBtnClicked()
    {
        Socket_SendMgr.GetFriendSend().SendAgreeApplyMessage(this.m_ApplyID);

        GameUI.HideUI(this.GetType());
    }
}