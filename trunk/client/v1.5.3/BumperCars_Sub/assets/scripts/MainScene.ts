import { MessagetType } from './MessageType';
import GlobalVar from './GlobalVar';
import WXHelper from './WXHelper';
const {ccclass, property} = cc._decorator;

@ccclass
export default class MainScene extends cc.Component
{
    @property(cc.Node)
    groupRankList: cc.Node = null

    @property(cc.Node)
    friendRankList: cc.Node = null

    protected onLoad () 
    {
        var that = cc.view;

        cc.view._convertPointWithScale = function(point) {
            var viewport = that._viewPortRect;
            point.x = (point.x - viewport.x) / (that._scaleX / 2);
            point.y = (point.y - viewport.y) / (that._scaleY / 2);
        }

        cc.view._convertTouchesWithScale = function(touches) {
            var viewport = that._viewPortRect, scaleX = that._scaleX / 2, scaleY = that._scaleY / 2, selTouch, selPoint, selPrePoint;
            for (var i = 0; i < touches.length; i++) {
                selTouch = touches[i];
                selPoint = selTouch._point;
                selPrePoint = selTouch._prevPoint;
                selPoint.x = (selPoint.x - viewport.x) / scaleX;
                selPoint.y = (selPoint.y - viewport.y) / scaleY;
                selPrePoint.x = (selPrePoint.x - viewport.x) / scaleX;
                selPrePoint.y = (selPrePoint.y - viewport.y) / scaleY;
            }
        }
    }

    start () {
        var self = this;

        cc.log("子域场景运行");

        this.loadRes();

        this.hideAllLayer();

        wx.onMessage(data => {
            cc.log("接收主域发来消息：", data);
            
            if(data.messageType == MessagetType.MT_FriendRankList)
            {
                //好友排行榜
                GlobalVar.rankType = parseInt(data.rankType);
                self.showFetchFriendRankList();
            }else if(data.messageType == MessagetType.MT_OverRankList)
            {
                //游戏结束界面排行榜
            }else if(data.messageType == MessagetType.MT_JJCYPlayer)
            {
                //即将超越的玩家
            }else if(data.messageType == MessagetType.MT_GroupRankList)
            {
                //群排行榜
                GlobalVar.rankType = data.rankType;
                GlobalVar.groupShareTicket = data.groupShareTicket;
                self.showGroupRankList();
            }else if(data.messageType == MessagetType.MT_UpdateJJCYPlayer)
            {
            }
            else if(data.messageType == MessagetType.MT_FriendRankList)
            {
                //清理
                self.clear();
            }
        });

        //获取自己的用户信息成功
        var getUserInfoSuccess = function(res){
            cc.log("获取用户信息成功--->>>", res);

            GlobalVar.avatarUrl = res.data[0].avatarUrl;
        };

        WXHelper.GetUseInfo(getUserInfoSuccess);
    }

    //显示好友排行榜
    private showFetchFriendRankList()
    {
        this.hideAllLayer();

        var friendRankListCom = this.friendRankList.getComponent("FriendRankList");
        if(friendRankListCom){
            friendRankListCom.show();
        }

        this.friendRankList.active = true;
    }

    //显示群排行榜
    private showGroupRankList()
    {
        this.hideAllLayer();

        var groupRankListCom = this.groupRankList.getComponent("GroupRankList");
        if(groupRankListCom)
        {
            groupRankListCom.show();
        }

        this.groupRankList.active = true;
    }

    private loadRes()
    {
        cc.loader.loadRes("subdomain", cc.SpriteAtlas, function (err, texture) {
            cc.log("子域资源加载完成");
        });
    }

    private hideAllLayer(){
        if(this.groupRankList)
        {
            this.groupRankList.active = false;
        }

        if(this.friendRankList)
        {
            this.friendRankList.active = false;
        }
    }

    private clear(){
        var groupRankListCom = this.groupRankList.getComponent("GroupRankList");
        if(groupRankListCom)
        {
            groupRankListCom.clear();
        }

        var friendRankListCom = this.friendRankList.getComponent("FriendRankList");
        if(friendRankListCom)
        {
            friendRankListCom.clear();
        }
    }
}