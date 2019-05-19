(function() {"use strict";var __module = CC_EDITOR ? module : {exports:{}};var __filename = 'preview-scripts/assets/scripts/FriendRankList.js';var __require = CC_EDITOR ? function (request) {return cc.require(request, require);} : function (request) {return cc.require(request, __filename);};function __define (exports, require, module) {"use strict";
cc._RF.push(module, 'c2499akU9VHPLW+LYRIuWlO', 'FriendRankList', __filename);
// scripts/FriendRankList.ts

Object.defineProperty(exports, "__esModule", { value: true });
var RankListBase_1 = require("./RankListBase");
var GlobalVar_1 = require("./GlobalVar");
var WXHelper_1 = require("./WXHelper");
var _a = cc._decorator, ccclass = _a.ccclass, property = _a.property;
//好友排行榜
var FriendRankList = /** @class */ (function (_super) {
    __extends(FriendRankList, _super);
    function FriendRankList() {
        return _super !== null && _super.apply(this, arguments) || this;
    }
    //显示排行榜
    FriendRankList.prototype.showRankList = function () {
        var self = this;
        //获取用户好友的托管数据成功
        var getFriendDataSuccess = function (res) {
            if (res && res.data) {
                console.log("获取用户好友的托管数据:", res);
                //排序
                self.dataSort(res.data);
                var selfIndex = -1;
                for (var i = 0; i < 30; i++) {
                    if (res.data.length <= i) {
                        break;
                    }
                    var item = cc.instantiate(self.prefabRankItem); //生成node节点
                    item.parent = self.scrollViewContent;
                    item.getComponent("RankItem").initView(i + 1, res.data[i], GlobalVar_1.default.rankType);
                    item.y = -(i * 100);
                    if (res.data[i].avatarUrl == GlobalVar_1.default.avatarUrl) {
                        selfIndex = i;
                    }
                }
                var selfData = self.getSelfData(res.data);
                if (selfData) {
                    selfData.is_rank = (selfIndex != -1);
                    var myitem = cc.instantiate(self.prefabRankItem); //生成node节点
                    myitem.parent = self.node;
                    myitem.getComponent("RankItem").initView(selfIndex + 1, selfData, GlobalVar_1.default.rankType, true);
                    myitem.y = -320;
                }
                self.rankingScrollView.node.active = true;
            }
        };
        if (GlobalVar_1.default.rankType == 1) {
            WXHelper_1.default.GetFriendCloudData(["kill_num"], getFriendDataSuccess);
        }
        else if (GlobalVar_1.default.rankType == 2) {
            WXHelper_1.default.GetFriendCloudData(["star_num"], getFriendDataSuccess);
        }
    };
    FriendRankList.prototype.getSelfData = function (data) {
        for (var i = 0; i < data.length; i++) {
            if (data[i].avatarUrl == GlobalVar_1.default.avatarUrl) {
                return data[i];
            }
        }
        return null;
    };
    //排序(ListData：res.data)
    FriendRankList.prototype.dataSort = function (ListData) {
        ListData.sort(function (a, b) {
            var aScore = parseInt(a.KVDataList[0].value);
            var bScore = parseInt(b.KVDataList[0].value);
            a.score = aScore;
            b.score = bScore;
            return bScore - aScore;
        });
    };
    FriendRankList = __decorate([
        ccclass
    ], FriendRankList);
    return FriendRankList;
}(RankListBase_1.default));
exports.default = FriendRankList;

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
        //# sourceMappingURL=FriendRankList.js.map
        