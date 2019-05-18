import MotionStreakBase from './MotionBase';
import MotionTableInfo from '../table/MotionTableInfo';
import ItemSystem from '../Item/ItemSystem';
const {ccclass, property} = cc._decorator;

@ccclass
export default class MyMotionStreak extends MotionStreakBase{

    @property({type: cc.Texture2D})
    protected texture: cc.Texture2D     = null;

    @property
    private fadeTime = 1;

    @property
    private minSeg = 2;

    @property
    private color = cc.Color.WHITE;

    @property
    private stroke = 100;

    private motionStreak = null;

    private isShow: boolean = null;

    onLoad(){
        let self = this;

        this.isShow = true;

        this.stroke = 70;

        // this.stroke *= this.node.scale;
        this.myscale            = 1;

        this.motionStreak = new _ccsg.MotionStreak();
        this.node.parent._sgNode.addChild(this.motionStreak, this.node.zIndex - 1);
        this.node.on("position-changed", this.onNodePositionChanged.bind(this))

        if(this.texture)
        {
            this.motionStreak.initWithFade(this.fadeTime, this.minSeg, this.stroke, this.color, this.texture);
        }
    }

    hide(){
        this.isShow = false;

        if(this.motionStreak)
        {
            this.motionStreak.reset();

            // this.motionStreak.setStartingPositionInitialized(false);
    
            this.motionStreak.setVisible(false);
        }
    }

    show(){
        this.isShow = true;

        if(this.motionStreak)
        {
            this.motionStreak.reset();

            // this.motionStreak.setStartingPositionInitialized(false);
    
            this.motionStreak.setVisible(true);
    
            this.motionStreak.setPosition(this.node.x, this.node.y);
        }
    }

    reset(){
        if(this.motionStreak){
            this.motionStreak.reset();
        }
    }

    updateStroke()
    {
        if(this.motionStreak)
        {
            this.stroke = (this.node.width - 170) * this.node.scale;
            this.motionStreak.setStroke(this.stroke);
        }
    }

    updateMotion()
    {
        if(this.motionStreak)
        {
            let motionInfo: MotionTableInfo = ItemSystem.GetInstance().GetTableInfoByItemID(this.motionID);
            if(motionInfo)
            {
                this.texture = cc.loader.getRes("motion/Motion_" + motionInfo.GetImgID(), cc.Texture2D);

                if(this.texture)
                {
                    this.motionStreak.initWithFade(this.fadeTime, this.minSeg, this.stroke, this.color, this.texture);
                    this.motionStreak.reset();
                }
            }
        }
    }

    onNodePositionChanged()
    {
        // if(this.motionStreak && this.isShow)
        // {
        //     this.motionStreak.setPosition(this.node.x, this.node.y);
        // }
    }

    update()
    {
        if(this.motionStreak)
        {
            this.motionStreak.setPosition(this.node.x, this.node.y);
        }
    }

    onDestroy()
    {
        this.reset();
    }
}