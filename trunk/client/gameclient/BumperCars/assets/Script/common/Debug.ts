export default class Debug
{
    public static ShowNode(parent: cc.Node, pos: cc.Vec2)
    {
        let node: cc.Node = new cc.Node();
        node.setPosition(pos);
        let sprite: cc.Sprite = node.addComponent(cc.Sprite);
        sprite.sizeMode = cc.Sprite.SizeMode.CUSTOM;
        node.setContentSize(50, 50);
        sprite.spriteFrame = new cc.SpriteFrame(cc.url.raw("resources/debug.png"));
        parent.addChild(node);
    }
}