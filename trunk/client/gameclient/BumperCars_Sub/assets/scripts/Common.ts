export default class Common
{
    constructor()
    {

    }

    public static CreateImage(node: cc.Node, avatarUrl: string) 
    {
        if(!node || !avatarUrl){
            return;
        }

        if (CC_WECHATGAME) 
        {
            try {
                let image = wx.createImage();
                image.onload = () => {
                    try {
                        let texture = new cc.Texture2D();
                        texture.initWithElement(image);
                        texture.handleLoadedTexture();
                        node.getComponent(cc.Sprite).spriteFrame = new cc.SpriteFrame(texture);
                    } catch (e) {
                        cc.log(e);
                        node.active = false;
                    }
                };
                image.src = avatarUrl;
            }catch (e) {
                cc.log(e);
                node.active = false;
            }
        } else {
            cc.loader.load({
                url: avatarUrl, type: 'jpg'
            }, (err, texture) => {
                node.getComponent(cc.Sprite).spriteFrame = new cc.SpriteFrame(texture);
            });
        }
    }
    
    public static SetNodeText(node: cc.Node, text: string)
    {
        if(node)
        {
            var label = node.getComponent(cc.Label);
            if(label)
            {
                label.string = text;
            }
        }
    }

    public static SetNodeTexture(node: cc.Node, filename: string)
    {
        if(node && filename)
        {
            let atlas: cc.SpriteAtlas = cc.loader.getRes('subdomain', cc.SpriteAtlas);

            if(atlas)
            {
                let spriteFrame = atlas.getSpriteFrame(filename);
                if(spriteFrame)
                {
                    let sprite = node.getComponent(cc.Sprite);
                    if(sprite)
                    {
                        sprite.spriteFrame = spriteFrame;
                    }
                }
            }
        }
    }

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
    public static GetByteVal(val, max) 
    {
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
    }
}