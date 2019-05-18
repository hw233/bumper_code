import Util from "../common/Util";
import EngineUtil from "../common/EngineUtil";
import AccountData from '../manager/AccountData';

const {ccclass, property} = cc._decorator;

@ccclass
export default class GoldEffect extends cc.Component{
    
    @property({type: cc.SpriteFrame})
    private itemImg = null

    @property
    private minX:number = -100

    @property
    private maxX:number = 100

    @property
    private minY:number = -100

    @property
    private maxY:number = 100

    @property
    private disperseTime:number = 0.5

    @property
    private itemCount:number = 10

    private isCalled: boolean = null

    playEffect (targetWorldPos: cc.Vec2, callback: Function) 
    {
        if(!targetWorldPos)return;

        let startPos: cc.Vec2 = cc.v2(cc.director.getWinSize().width * 0.5, cc.director.getWinSize().height * 0.5);

        let self = this;

        let targetPos: cc.Vec2 = this.node.convertToNodeSpaceAR(targetWorldPos);

        this.minX = startPos.x + this.minX;
        this.maxX = startPos.x + this.maxX;
        this.minY = startPos.y + this.minY;
        this.maxY = startPos.y + this.maxY;

        let delayTime = 0.2;
        let moveTime = 0.5;

        for(let i: number = 0; i < this.itemCount; i ++){
            let x = Util.RandomNum(this.minX, this.maxX);
            let y = Util.RandomNum(this.minY, this.maxY);

            let node = new cc.Node();
            let sprite = node.addComponent(cc.Sprite);
            sprite.spriteFrame = this.itemImg;
            node.setPosition(startPos);

            this.node.addChild(node);

            let action = cc.moveTo(this.disperseTime, x, y);
            let delay = cc.delayTime(delayTime);
            let action2 = cc.moveTo(moveTime, targetPos);
            let func = cc.callFunc(function(){
                if(self.isCalled == null)
                {
                    self.isCalled = true;
                    callback && callback();
                }
            });
            let removeSelf = cc.removeSelf();

            node.runAction(cc.sequence(action, delay, action2, func, removeSelf));
        }
    }
}