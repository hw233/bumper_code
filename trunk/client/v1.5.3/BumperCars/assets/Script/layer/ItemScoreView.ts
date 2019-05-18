const {ccclass, property} = cc._decorator;

@ccclass
export default class ItemScoreView extends cc.Component
{
    @property(cc.Label)
    private label: cc.Label = null;

    onLoad () 
    {
    }

    public init()
    {
        this.node.x = 0;
        this.node.y = 0;

        this.node.opacity = 255;

        this.label.string = "";
    }

    public playEffect(num:string, callback: Function): void
    {
        this.setString(num);

        let moveAction: cc.FiniteTimeAction = cc.moveBy(0.5, 0, 200);
        let fadeoutAction: cc.FiniteTimeAction = cc.fadeOut(0.5);
        let callfunc: cc.ActionInstant = cc.callFunc(callback);

        let sequence: cc.Action = cc.sequence(moveAction, fadeoutAction, callfunc);

        this.node.runAction(sequence);
    }

    public setString(num: string): void
    {
        this.label.string = num;
    }
}