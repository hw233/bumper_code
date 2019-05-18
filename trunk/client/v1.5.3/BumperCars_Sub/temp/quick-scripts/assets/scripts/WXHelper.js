(function() {"use strict";var __module = CC_EDITOR ? module : {exports:{}};var __filename = 'preview-scripts/assets/scripts/WXHelper.js';var __require = CC_EDITOR ? function (request) {return cc.require(request, require);} : function (request) {return cc.require(request, __filename);};function __define (exports, require, module) {"use strict";
cc._RF.push(module, 'c9a72pLfUBOZL5A/XD+xpBl', 'WXHelper', __filename);
// scripts/WXHelper.ts

Object.defineProperty(exports, "__esModule", { value: true });
var GlobalVar_1 = require("./GlobalVar");
var WXHelper = /** @class */ (function () {
    function WXHelper() {
    }
    WXHelper.IsWXContext = function () {
        if (CC_WECHATGAME) {
            return true;
        }
        return false;
    };
    WXHelper.GetUseInfo = function (successCb) {
        wx.getUserInfo({
            withCredentials: false,
            openIdList: ['selfOpenId'],
            lang: "zh_CN",
            success: successCb,
            fail: function () {
                cc.log("获取用户信息失败:");
            }
        });
    };
    //获取群同玩成员的游戏数据
    WXHelper.GetGroupCloudData = function (keyArray, success) {
        console.log("shareTicket:", GlobalVar_1.default.groupShareTicket);
        wx.getGroupCloudStorage({
            shareTicket: GlobalVar_1.default.groupShareTicket,
            keyList: keyArray,
            success: success,
            fail: function (res) {
                console.log("Rank:获取群的信息失败:", res);
            }
        });
    };
    //获取好友的游戏数据
    WXHelper.GetFriendCloudData = function (keyArray, success) {
        wx.getFriendCloudStorage({
            keyList: keyArray,
            success: function (res) {
                console.log("wx.getFriendCloudStorage success", res);
                success(res);
            },
            fail: function (res) {
                console.log("wx.getFriendCloudStorage fail", res);
            },
        });
    };
    return WXHelper;
}());
exports.default = WXHelper;

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
        //# sourceMappingURL=WXHelper.js.map
        