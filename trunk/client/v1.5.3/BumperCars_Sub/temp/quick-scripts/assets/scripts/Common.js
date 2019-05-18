(function() {"use strict";var __module = CC_EDITOR ? module : {exports:{}};var __filename = 'preview-scripts/assets/scripts/Common.js';var __require = CC_EDITOR ? function (request) {return cc.require(request, require);} : function (request) {return cc.require(request, __filename);};function __define (exports, require, module) {"use strict";
cc._RF.push(module, '79cffzRxpVFb4mm65bZpPa8', 'Common', __filename);
// scripts/Common.ts

Object.defineProperty(exports, "__esModule", { value: true });
var Common = /** @class */ (function () {
    function Common() {
    }
    Common.CreateImage = function (node, avatarUrl) {
        if (!node || !avatarUrl) {
            return;
        }
        if (CC_WECHATGAME) {
            try {
                var image_1 = wx.createImage();
                image_1.onload = function () {
                    try {
                        var texture = new cc.Texture2D();
                        texture.initWithElement(image_1);
                        texture.handleLoadedTexture();
                        node.getComponent(cc.Sprite).spriteFrame = new cc.SpriteFrame(texture);
                    }
                    catch (e) {
                        cc.log(e);
                        node.active = false;
                    }
                };
                image_1.src = avatarUrl;
            }
            catch (e) {
                cc.log(e);
                node.active = false;
            }
        }
        else {
            cc.loader.load({
                url: avatarUrl, type: 'jpg'
            }, function (err, texture) {
                node.getComponent(cc.Sprite).spriteFrame = new cc.SpriteFrame(texture);
            });
        }
    };
    Common.SetNodeText = function (node, text) {
        if (node) {
            var label = node.getComponent(cc.Label);
            if (label) {
                label.string = text;
            }
        }
    };
    Common.SetNodeTexture = function (node, filename) {
        if (node && filename) {
            var atlas = cc.loader.getRes('subdomain', cc.SpriteAtlas);
            if (atlas) {
                var spriteFrame = atlas.getSpriteFrame(filename);
                if (spriteFrame) {
                    var sprite = node.getComponent(cc.Sprite);
                    if (sprite) {
                        sprite.spriteFrame = spriteFrame;
                    }
                }
            }
        }
    };
    // public static SetNodeTexture(node: cc.Node, filename: string)
    // {
    //     if(node && filename)
    //     {
    // filename = cc.url.raw("resources/" + filename + ".png");
    //         cc.loader.load({
    //             url: filename, type: 'png'
    //         }, (err, texture) => {
    //             if(node.getComponent(cc.Sprite)){
    //                 node.getComponent(cc.Sprite).spriteFrame = new cc.SpriteFrame(texture);
    //             }
    //         });
    //     }
    // }
    //根据传入的最大字符个数截取字符串
    Common.GetByteVal = function (val, max) {
        var returnValue = '';
        var byteValLen = 0;
        for (var i = 0; i < val.length; i++) {
            if (val[i].match(/[^\x00-\xff]/ig) != null)
                byteValLen += 2;
            else
                byteValLen += 1;
            if (byteValLen > max)
                break;
            returnValue += val[i];
        }
        return returnValue;
    };
    return Common;
}());
exports.default = Common;

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
        //# sourceMappingURL=Common.js.map
        