(function() {"use strict";var __module = CC_EDITOR ? module : {exports:{}};var __filename = 'preview-scripts/assets/scripts/RankItem.js';var __require = CC_EDITOR ? function (request) {return cc.require(request, require);} : function (request) {return cc.require(request, __filename);};function __define (exports, require, module) {"use strict";
cc._RF.push(module, '47686h4Bn1I47AjBIzX1z8m', 'RankItem', __filename);
// scripts/RankItem.ts

Object.defineProperty(exports, "__esModule", { value: true });
var Common_1 = require("./Common");
var _a = cc._decorator, ccclass = _a.ccclass, property = _a.property;
var RankItem = /** @class */ (function (_super) {
    __extends(RankItem, _super);
    function RankItem() {
        var _this = _super !== null && _super.apply(this, arguments) || this;
        _this.bgImg = null; //背景图
        _this.cupImg = null; //奖杯
        _this.headImg = null; //头像
        _this.playerName = null; //昵称
        _this.rankNum = null; //排名
        _this.killRank = null; //击杀排名
        _this.gradingRank = null; //段位排名
        return _this;
    }
    RankItem.prototype.start = function () {
    };
    RankItem.prototype.initView = function (rank, data, type, isme) {
        if (isme === void 0) { isme = false; }
        if (rank <= 3 || data.is_rank === false) {
            this.cupImg.active = true;
            this.rankNum.active = false;
            if (data.is_rank === false) {
                Common_1.default.SetNodeTexture(this.cupImg, "bw");
            }
            else {
                Common_1.default.SetNodeTexture(this.cupImg, "No" + rank);
            }
        }
        else {
            this.rankNum.active = true;
            this.cupImg.active = false;
            Common_1.default.SetNodeText(this.rankNum, rank.toString());
        }
        if (isme) {
            Common_1.default.SetNodeTexture(this.bgImg, "paimingtiao-B");
        }
        if (type == 1) {
            //击杀排名
            this.gradingRank.active = false;
            this.killRank.active = true;
            Common_1.default.SetNodeText(this.killRank.getChildByName("Num"), data.score);
        }
        else if (type == 2) {
            //段位排名
            this.gradingRank.active = true;
            this.killRank.active = false;
            var curGradingID = Math.floor(data.score / 25) + 1;
            var fileName = 'Grading_' + curGradingID;
            Common_1.default.SetNodeTexture(this.gradingRank.getChildByName("Icon"), fileName);
            Common_1.default.SetNodeText(this.gradingRank.getChildByName("Num"), (data.score % 25).toString());
        }
        //限制昵称最大为10个字符
        var nickname = Common_1.default.GetByteVal(data.nickname, 10);
        Common_1.default.CreateImage(this.headImg, data.avatarUrl);
        Common_1.default.SetNodeText(this.playerName, nickname);
    };
    __decorate([
        property(cc.Node)
    ], RankItem.prototype, "bgImg", void 0);
    __decorate([
        property(cc.Node)
    ], RankItem.prototype, "cupImg", void 0);
    __decorate([
        property(cc.Node)
    ], RankItem.prototype, "headImg", void 0);
    __decorate([
        property(cc.Node)
    ], RankItem.prototype, "playerName", void 0);
    __decorate([
        property(cc.Node)
    ], RankItem.prototype, "rankNum", void 0);
    __decorate([
        property(cc.Node)
    ], RankItem.prototype, "killRank", void 0);
    __decorate([
        property(cc.Node)
    ], RankItem.prototype, "gradingRank", void 0);
    RankItem = __decorate([
        ccclass
    ], RankItem);
    return RankItem;
}(cc.Component));
exports.default = RankItem;

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
        //# sourceMappingURL=RankItem.js.map
        