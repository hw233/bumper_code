const {ccclass, property} = cc._decorator;

@ccclass
export default class MotionStreakBase extends cc.Component
{
    protected motionID: number          = null;

    protected myscale: number           = null;

    protected updateMotion()
    {

    }

    public setMyScale(scale: number)
    {
        // this.myscale = scale;
    }

    public setMotionID(motionID: number)
    {
        this.motionID = motionID;

        this.updateMotion();
    }

    public reset()
    {

    }

    public show()
    {

    }

    public hide()
    {
        
    }
}