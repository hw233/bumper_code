import GameLog from "../Log/LogTool";
import Util from './Util';

export enum AtlasType
{
    AT_None     = 0,

    AT_Common,
    AT_FriendLayer,
    AT_RankListLayer,
    AT_StartScene,
    AT_RoomScene,
    AT_GameMap,
    AT_GameCar,
    AT_GameScene,
    AT_GameOver,

    AT_TaskLayer,
    AT_CarLibLayer,

    AT_Max,
}

export default class EngineUtil{
    public static GetPrefab(name: string): any
    {
        return cc.loader.getRes("prefab/" + name, cc.Prefab);
    }

    public static SetNodeTextureByType(type: AtlasType, node: cc.Node, filename: string)
    {
        if(type <= AtlasType.AT_None || type >= AtlasType.AT_Max)
        {
            GameLog.PrintError("EngineUtil:SetNodeTextureByType:type is invaild:", type);
            return;
        }

        if(node && filename)
        {
            let atlas: cc.SpriteAtlas = null;
            switch(type)
            {
                case AtlasType.AT_Common:
                {
                    atlas = cc.loader.getRes('common', cc.SpriteAtlas);
                    break;
                }
                case AtlasType.AT_FriendLayer:
                {
                    atlas = cc.loader.getRes('friendlayer', cc.SpriteAtlas);
                    break;
                }
                case AtlasType.AT_RankListLayer:
                {
                    atlas = cc.loader.getRes('ranklist', cc.SpriteAtlas);
                    break;
                }
                case AtlasType.AT_StartScene:
                {
                    atlas = cc.loader.getRes('startscene', cc.SpriteAtlas);
                    break;
                }
                case AtlasType.AT_RoomScene:
                {
                    atlas = cc.loader.getRes('roomscene', cc.SpriteAtlas);
                    break;
                }
                case AtlasType.AT_GameMap:
                {
                    atlas = cc.loader.getRes('gamemap', cc.SpriteAtlas);
                    break;
                }
                case AtlasType.AT_GameCar:
                {
                    atlas = cc.loader.getRes('gamecar', cc.SpriteAtlas);
                    break;
                }
                case AtlasType.AT_GameScene:
                {
                    atlas = cc.loader.getRes('gamescene', cc.SpriteAtlas);
                    break;
                }
                case AtlasType.AT_GameOver:
                {
                    atlas = cc.loader.getRes('gameover', cc.SpriteAtlas);
                    break;
                }
                case AtlasType.AT_TaskLayer:
                {
                    atlas = cc.loader.getRes('tasklayer', cc.SpriteAtlas);
                    break;
                }
                case AtlasType.AT_CarLibLayer:
                {
                    atlas = cc.loader.getRes('carliblayer', cc.SpriteAtlas);
                    break;
                }
            }

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

    //example "gamescene/GameResult_1"
    //filename: resources下的文件
    //设置精灵节点的图片
    public static SetNodeTexture(node: cc.Node, filename: string): any{
        if(node && filename){
            let tmpTexture = cc.loader.getRes(filename);
            if(tmpTexture){
                let sprite = node.getComponent(cc.Sprite);
                if(sprite){
                    sprite.spriteFrame = new cc.SpriteFrame(tmpTexture);
                }
            }else{
                cc.loader.loadRes(filename, function(err, texture){
                    if(err){
                        GameLog.PrintLog(err.message);
                    }else{
                        let sprite = node.getComponent(cc.Sprite);
                        if(sprite){
                            sprite.spriteFrame = new cc.SpriteFrame(texture);
                        }
                    }
                });
            }
        }
    }

    //
    public static SetNodeSprteFrame(node: cc.Node, spriteFrame: cc.SpriteFrame): void{
        let sprite = node.getComponent(cc.Sprite);
        if(sprite){
            sprite.spriteFrame = spriteFrame;
        }
    }

    //设置文本节点的内容
    public static SetNodeText(node: cc.Node, text: string): void{
        if(node){
            if(Util.IsNumber(text) ||Util.IsString(text)){
                let label = node.getComponent(cc.Label);
                if(label){
                    label.string = text;
                }
            }
        }
    }

    public static CreateSprite(name: string, texture: cc.Texture2D): cc.Node{
        let node = new cc.Node(name);
        if(node){
            let sprite = node.addComponent(cc.Sprite);
            sprite.spriteFrame = new cc.SpriteFrame(texture);
        }
        return node;
    }

    public static CreateSpriteWithFrame(name: string, spriteFrame: cc.SpriteFrame): cc.Node{
        let node = new cc.Node(name);
        if(node){
            let sprite = node.addComponent(cc.Sprite);
            sprite.spriteFrame = spriteFrame;
            return node;
        }
        return null;
    }

    //获取指定节点的指定组件
    public static GetComponent<T extends cc.Component>(node: cc.Node, component): T
    {
        if(node && component){
            return node.getComponent(component);
        }
        return null;
    }

    //添加组件
    public static AddComponent(node: cc.Node, component): cc.Component{
        if(node && component){
            return node.addComponent(component);
        }
        return null;
    }

    //移除组件
    public static RemoveComponent(node: cc.Node, component): void{
        if(node && component){
            node.removeComponent(component);
        }
    }

    public static SetWidgetTarget(node: cc.Node, target): void{
        if(node && target){
            let widgetCom = node.getComponent(cc.Widget);
            if(widgetCom){
                widgetCom.target = target;
            }
        }
    }

    //注册按钮点击事件
    public static RegBtnClickEvent(button: cc.Node, callback){
        // cc.assert(button, "button is null");
        // cc.assert(callback, "callback is null");

        if(button && callback){
            button.on("click", callback);
        }
    }

    //取消按钮的点击事件
    public static CancelClickEvent(button: cc.Node){
        if(button){
            button.off("click");
        }
    }

    //注册按钮点击事件
    public static RegTouchEndEvent(button: cc.Node, callback){
        if(button && callback){
            button.on(cc.Node.EventType.TOUCH_END, callback);
        }
    }

    public static SetButtonInteractable(btn: cc.Node, interactable: boolean){
        if(btn){
            let com = btn.getComponent(cc.Button);
            if(com){
                com.interactable = interactable;
            }
        }
    }

    //注册键盘按下事件
    public static RegKeyUpEvent(callback){
        if(callback){
            cc.systemEvent.on(cc.SystemEvent.EventType.KEY_UP, callback);
        }
    }

    public static RegBtnTouchBeginEvent(button, callback){
        if(button && callback){
            button.on(cc.Node.EventType.TOUCH_START, callback);
        }
    }

     //加载远程图片
     public static LoadRemoteImg(node: cc.Node, url: string){
        if(!url){
            return;
        }
        cc.loader.load({
            url: url, type: 'jpg'
        }, (err, texture) => {
            if(node.isValid)
            {
                node.getComponent(cc.Sprite).spriteFrame = new cc.SpriteFrame(texture);
            }
        });
    }
}