(function() {"use strict";var __module = CC_EDITOR ? module : {exports:{}};var __filename = 'preview-scripts/assets/scripts/MainScene.js';var __require = CC_EDITOR ? function (request) {return cc.require(request, require);} : function (request) {return cc.require(request, __filename);};function __define (exports, require, module) {"use strict";
cc._RF.push(module, '3904cQPHqBLu71IZ1DptBX+', 'MainScene', __filename);
// scripts/MainScene.ts

Object.defineProperty(exports, "__esModule", { value: true });
var MessageType_1 = require("./MessageType");
var GlobalVar_1 = require("./GlobalVar");
var WXHelper_1 = require("./WXHelper");
var _a = cc._decorator, ccclass = _a.ccclass, property = _a.property;
var MainScene = /** @class */ (function (_super) {
    __extends(MainScene, _super);
    function MainScene() {
        var _this = _super !== null && _super.apply(this, arguments) || this;
        _this.groupRankList = null;
        _this.friendRankList = null;
        return _this;
    }
    MainScene.prototype.onLoad = function () {
        var that = cc.view;
        cc.view._convertPointWithScale = function (point) {
            var viewport = that._viewPortRect;
            point.x = (point.x - viewport.x) / (that._scaleX / 2);
            point.y = (point.y - viewport.y) / (that._scaleY / 2);
        };
        cc.view._convertTouchesWithScale = function (touches) {
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
        };
    };
    MainScene.prototype.start = function () {
        var self = this;
        cc.log("子域场景运行");
        this.loadRes();
        this.hideAllLayer();
        wx.onMessage(function (data) {
            cc.log("接收主域发来消息：", data);
            if (data.messageType == MessageType_1.MessagetType.MT_FriendRankList) {
                //好友排行榜
                GlobalVar_1.default.rankType = parseInt(data.rankType);
                self.showFetchFriendRankList();
            }
            else if (data.messageType == MessageType_1.MessagetType.MT_OverRankList) {
                //游戏结束界面排行榜
            }
            else if (data.messageType == MessageType_1.MessagetType.MT_JJCYPlayer) {
                //即将超越的玩家
            }
            else if (data.messageType == MessageType_1.MessagetType.MT_GroupRankList) {
                //群排行榜
                GlobalVar_1.default.rankType = data.rankType;
                GlobalVar_1.default.groupShareTicket = data.groupShareTicket;
                self.showGroupRankList();
            }
            else if (data.messageType == MessageType_1.MessagetType.MT_UpdateJJCYPlayer) {
            }
            else if (data.messageType == MessageType_1.MessagetType.MT_FriendRankList) {
                //清理
                self.clear();
            }
        });
        //获取自己的用户信息成功
        var getUserInfoSuccess = function (res) {
            cc.log("获取用户信息成功--->>>", res);
            GlobalVar_1.default.avatarUrl = res.data[0].avatarUrl;
        };
        WXHelper_1.default.GetUseInfo(getUserInfoSuccess);
    };
    //显示好友排行榜
    MainScene.prototype.showFetchFriendRankList = function () {
        this.hideAllLayer();
        var friendRankListCom = this.friendRankList.getComponent("FriendRankList");
        if (friendRankListCom) {
            friendRankListCom.show();
        }
        this.friendRankList.active = true;
    };
    //显示群排行榜
    MainScene.prototype.showGroupRankList = function () {
        this.hideAllLayer();
        var groupRankListCom = this.groupRankList.getComponent("GroupRankList");
        if (groupRankListCom) {
            groupRankListCom.show();
        }
        this.groupRankList.active = true;
    };
    MainScene.prototype.loadRes = function () {
        cc.loader.loadRes("subdomain", cc.SpriteAtlas, function (err, texture) {
            cc.log("子域资源加载完成");
        });
    };
    MainScene.prototype.hideAllLayer = function () {
        if (this.groupRankList) {
            this.groupRankList.active = false;
        }
        if (this.friendRankList) {
            this.friendRankList.active = false;
        }
    };
    MainScene.prototype.clear = function () {
        var groupRankListCom = this.groupRankList.getComponent("GroupRankList");
        if (groupRankListCom) {
            groupRankListCom.clear();
        }
        var friendRankListCom = this.friendRankList.getComponent("FriendRankList");
        if (friendRankListCom) {
            friendRankListCom.clear();
        }
    };
    __decorate([
        property(cc.Node)
    ], MainScene.prototype, "groupRankList", void 0);
    __decorate([
        property(cc.Node)
    ], MainScene.prototype, "friendRankList", void 0);
    MainScene = __decorate([
        ccclass
    ], MainScene);
    return MainScene;
}(cc.Component));
exports.default = MainScene;

cc._RF.pop();
        }
        if (CC_EDITOR) {
            __define(__module.exports, __require, __module);
        }
        else {
            cc.registerModuleFunc(__filename, function () {
                __define(__module.exports, __require, __module);
            });
        }
        })();
        //# sourceMappingURL=MainScene.js.map
        