import EngineUtil from "../common/EngineUtil";
import WXHelper from "../common/WXHelper";
import GameData from '../data/GameData';
import GlobalVar from '../GlobalVar';

/**
 * state: 1, //邀请状态
 * state: 2, //有玩家状态
 * state: 3, //匹配状态
 */

const {ccclass, property} = cc._decorator;

@ccclass
export default class MatchItem extends cc.Component{
        //头像
    @property({type: cc.Node})
    private headNode = null;

        //邀请按钮
    @property({type: cc.Node})
    private inviteBtn = null;

        //加载中节点
    @property({type: cc.Node})
    private loadingNode = null;

    onLoad () {
        this.headNode.active = false;
        this.loadingNode.active = false;
        this.inviteBtn.active = true;

        this.inviteBtn.on(cc.Node.EventType.TOUCH_END, this.inviteBtnClicked);
    }

    start () {

    }

    //邀请好友点击
    inviteBtnClicked(){
        WXHelper.InviteJoinShare(GameData.GetRoomID(), GlobalVar.gameModel, null);
    }

    initView(player){
        if(player){
            let headImg = player.getAvatarUrl();
            EngineUtil.LoadRemoteImg(this.headNode, headImg);
        }
    }

    refreshUI(){
        if(this.node.state == 1){
            this.headNode.active = false;
            this.loadingNode.active = false;

            this.inviteBtn.active = true;
        }else if(this.node.state == 2){
            this.inviteBtn.active = false;
            this.loadingNode.active = false;

            this.headNode.active = true;
        }else if(this.node.state == 3){
            this.inviteBtn.active = false;
            this.headNode.active = false;

            this.loadingNode.active = true;
        }
    }
}
