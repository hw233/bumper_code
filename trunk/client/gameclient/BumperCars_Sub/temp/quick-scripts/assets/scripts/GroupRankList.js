(function() {"use strict";var __module = CC_EDITOR ? module : {exports:{}};var __filename = 'preview-scripts/assets/scripts/GroupRankList.js';var __require = CC_EDITOR ? function (request) {return cc.require(request, require);} : function (request) {return cc.require(request, __filename);};function __define (exports, require, module) {"use strict";
cc._RF.push(module, '85613yjyVlEYK0fnJdowJoS', 'GroupRankList', __filename);
// scripts/GroupRankList.ts

Object.defineProperty(exports, "__esModule", { value: true });
var GlobalVar_1 = require("./GlobalVar");
var WXHelper_1 = require("./WXHelper");
var RankListBase_1 = require("./RankListBase");
var _a = cc._decorator, ccclass = _a.ccclass, property = _a.property;
var GroupRankList = /** @class */ (function (_super) {
    __extends(GroupRankList, _super);
    function GroupRankList() {
        return _super !== null && _super.apply(this, arguments) || this;
    }
    //显示排行榜
    GroupRankList.prototype.showRankList = function () {
        var self = this;
        // var userData = null;
        //获取群同玩用户的托管数据成功
        var getGroupDataSuccess = function (res) {
            if (res && res.data) {
                console.log("获取群同玩用户的托管数据:", res);
                //排序
                self.dataSort(res.data);
                for (var i = 0; i < res.data.length; i++) {
                    var item = cc.instantiate(self.prefabRankItem); //生成node节点
                    item.parent = self.scrollViewContent;
                    item.getComponent("RankItem").initView(i + 1, res.data[i], GlobalVar_1.default.rankType);
                    item.y = -(i * 100);
                    if (res.data[i].avatarUrl == GlobalVar_1.default.avatarUrl) {
                        var myitem = cc.instantiate(self.prefabRankItem); //生成node节点
                        res.data[i].is_rank = true;
                        myitem.parent = self.node;
                        myitem.getComponent("RankItem").initView(i + 1, res.data[i], GlobalVar_1.default.rankType, true);
                        myitem.y = -320;
                    }
                }
                self.rankingScrollView.node.active = true;
            }
        };
        if (GlobalVar_1.default.rankType == 1) {
            WXHelper_1.default.GetGroupCloudData(["kill_num"], getGroupDataSuccess);
        }
        else if (GlobalVar_1.default.rankType == 2) {
            WXHelper_1.default.GetGroupCloudData(["star_num"], getGroupDataSuccess);
        }
    };
    //排序(ListData：res.data)
    GroupRankList.prototype.dataSort = function (ListData) {
        for (var i = ListData.length - 1; i >= 0; i--) {
            var item = ListData[i];
            if (item) {
                if (item.KVDataList.length <= 0) {
                    ListData.splice(i, 1);
                    // item.KVDataList.push({key:"highest_score", value: i});
                }
            }
        }
        ListData.sort(function (a, b) {
            var aScore = parseInt(a.KVDataList[0].value);
            var bScore = parseInt(b.KVDataList[0].value);
            a.score = aScore;
            b.score = bScore;
            return bScore - aScore;
        });
    };
    GroupRankList = __decorate([
        ccclass
    ], GroupRankList);
    return GroupRankList;
}(RankListBase_1.default));
exports.default = GroupRankList;

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
        //# sourceMappingURL=GroupRankList.js.map
        