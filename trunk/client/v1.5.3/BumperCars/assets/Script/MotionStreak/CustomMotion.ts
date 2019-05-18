import MotionStreakBase from './MotionBase';
import Util from '../common/Util';
import MotionTableInfo from '../table/MotionTableInfo';
import ItemSystem from '../Item/ItemSystem';

const {ccclass, property} = cc._decorator;

@ccclass
export default class CustomMotion extends MotionStreakBase
{
    // onLoad () {},

    private nodePool: cc.NodePool           = null;

    private spriteframe: cc.SpriteFrame     = null;

    private generateTime: number            = null;

    private tempTime: number                = null;

    private motionInfo: MotionTableInfo     = null;

    private isGenerate: boolean             = false;

    protected onLoad () 
    {
        this.myscale            = 1;
        this.isGenerate         = true;
        this.tempTime           = 0;
        this.generateTime       = 0;
        this.nodePool           = new cc.NodePool();

        for(let i = 0;i < 20;i ++)
        {
            let node = new cc.Node();

            node.addComponent(cc.Sprite);

            this.nodePool.put(node);
        }
    }

    public show()
    {
        this.isGenerate = true;
    }

    public hide()
    {
        this.isGenerate = false;
    }

    private generateMotion()
    {
        let self = this;

        if(!this.spriteframe)return;

        if(!this.isGenerate)return;

        if(this.motionInfo)
        {
            let originPos: cc.Vec2     = this.node.position;
            let num: number            = this.motionInfo.GetNum();
            let xOffect: Array<number> = this.motionInfo.GetXOffect();
            let yOffect: Array<number> = this.motionInfo.GetYOffect();
            let generateModel: number  = this.motionInfo.GetGenerateModel();
            let initScale: number      = this.motionInfo.GetInitScale();
            let finalScale: number     = this.motionInfo.GetFinalScale();
            let initTrans: number      = this.motionInfo.GetInitTransparency();
            let finalTrans: number     = this.motionInfo.GetFinalTransparency();
            let liveTime: number       = this.motionInfo.GetLiveTime();

            for(let i = 0;i < num; i ++)
            {
                let node: cc.Node = this.getNode();
                if(node)
                {
                    let x = 0, y = 0;

                    if(generateModel === 1)
                    {
                        if( xOffect.length >= (i + 1) )
                        {
                            x = originPos.x + xOffect[i];
                        }
                        if( yOffect.length >= (i + 1) )
                        {
                            y = originPos.y + yOffect[i];
                        }
                    }
                    else if(generateModel === 2)
                    {
                        if(xOffect.length >= 2)
                        {
                            x = Util.RandomNum(originPos.x + xOffect[0], originPos.x + xOffect[1])
                        }
                        if(yOffect.length >= 2)
                        {
                            y = Util.RandomNum(originPos.y + yOffect[0], originPos.y + yOffect[1])
                        }
                    }
                    node.setPosition(x, y);
                    node.getComponent(cc.Sprite).spriteFrame = this.spriteframe

                    node.zIndex = this.node.zIndex - 1;
                    node.rotation = this.node.rotation;
                    node.scale = initScale * this.myscale;
                    node.opacity = 255 * initTrans;

                    let scaleAction: cc.FiniteTimeAction = cc.scaleTo(liveTime, finalScale * this.myscale);
                    let fadeAction: cc.FiniteTimeAction = cc.fadeTo(liveTime, 255 * finalTrans);
                    let callback: cc.ActionInstant = cc.callFunc(function(){
                        node.stopAllActions();
                        if(self.nodePool)
                        {
                            self.nodePool.put(node);
                        }
                    });

                    let spawnAcion = cc.sequence(cc.spawn(scaleAction, fadeAction), callback);

                    node.runAction(spawnAcion);

                    this.node.parent.addChild(node);
                }
            }
        }
    }

    updateMotion()
    {
        let motionInfo: MotionTableInfo = ItemSystem.GetInstance().GetTableInfoByItemID(this.motionID);
        if(motionInfo)
        {
            this.generateTime = motionInfo.GetGenerateTime();
            this.motionInfo = motionInfo;

            let atlas: cc.SpriteAtlas = cc.loader.getRes('motion/motion', cc.SpriteAtlas);
            if(atlas)
            {
                this.spriteframe = atlas.getSpriteFrame("Motion_" + motionInfo.GetImgID());
            }
        }
    }

    protected update(dt: number)
    {
        this.tempTime += dt;

        if(this.tempTime >= this.generateTime)
        {
            this.tempTime = 0;
            this.generateMotion();
        }
    }   

    private getNode(): cc.Node 
    {
        let node: cc.Node = null;

        if(this.nodePool)
        {
            // 通过 size 接口判断对象池中是否有空闲的对象
            if (this.nodePool.size() > 0) 
            {

                node = this.nodePool.get();
            } 
            else 
            {
                // 如果没有空闲对象，也就是对象池中备用对象不够时，我们就用 cc.instantiate 重新创建
                node = new cc.Node();
                node.addComponent(cc.Sprite);
            }

            if(node)
            {
                node.scale = 1;
                node.opacity = 255;
            }
        }
        return node;
    }

    onDestroy()
    {
        this.motionInfo = null;
        this.nodePool.clear();
        this.nodePool = null;
    }
}