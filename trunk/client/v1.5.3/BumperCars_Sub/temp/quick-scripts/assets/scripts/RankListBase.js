(function() {"use strict";var __module = CC_EDITOR ? module : {exports:{}};var __filename = 'preview-scripts/assets/scripts/RankListBase.js';var __require = CC_EDITOR ? function (request) {return cc.require(request, require);} : function (request) {return cc.require(request, __filename);};function __define (exports, require, module) {"use strict";
cc._RF.push(module, '3020c3nFJpNCaEYA6RNrzUq', 'RankListBase', __filename);
// scripts/RankListBase.ts

Object.defineProperty(exports, "__esModule", { value: true });
var _a = cc._decorator, ccclass = _a.ccclass, property = _a.property;
var RankListBase = /** @class */ (function (_super) {
    __extends(RankListBase, _super);
    function RankListBase() {
        var _this = _super !== null && _super.apply(this, arguments) || this;
        _this.rankingScrollView = null;
        _this.scrollViewContent = null;
        _this.prefabRankItem = null;
        return _this;
    }
    RankListBase.prototype.onLoad = function () {
        if (cc.director.getWinSize().height / cc.director.getWinSize().width - 16 / 9 > 0.1) {
            this.node.scaleY = cc.director.getWinSize().height / 1334;
        }
    };
    RankListBase.prototype.show = function () {
        this.removeChild();
        this.showRankList();
    };
    RankListBase.prototype.clear = function () {
        this.removeChild();
    };
    RankListBase.prototype.removeChild = function () {
        this.node.removeChildByTag(1000);
        this.scrollViewContent.removeAllChildren();
        this.rankingScrollView.node.active = false;
    };
    //显示排行榜
    RankListBase.prototype.showRankList = function () {
    };
    //排序(ListData：res.data)
    RankListBase.prototype.dataSort = function (ListData) {
        for (var i = ListData.length - 1; i >= 0; i--) {
            var item = ListData[i];
            if (item) {
                if (item.KVDataList.length <= 0) {
                    ListData.splice(i, 1);
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
    __decorate([
        property(cc.ScrollView)
    ], RankListBase.prototype, "rankingScrollView", void 0);
    __decorate([
        property(cc.Node)
    ], RankListBase.prototype, "scrollViewContent", void 0);
    __decorate([
        property(cc.Prefab)
    ], RankListBase.prototype, "prefabRankItem", void 0);
    RankListBase = __decorate([
        ccclass
    ], RankListBase);
    return RankListBase;
}(cc.Component));
exports.default = RankListBase;

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
        //# sourceMappingURL=RankListBase.js.map
        