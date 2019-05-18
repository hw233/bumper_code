import ComponentBase from '../common/ComponentBase';
import GameConfig from '../GameConfig';
const {ccclass, property} = cc._decorator;

@ccclass
export default class CameraControl extends ComponentBase{

    private target:cc.Node = null;

    private camera: cc.Camera = null;

    // use this for initialization
    protected onLoad() 
    {
        this.camera = this.getComponent(cc.Camera);

        if(GameConfig.PhysicsSwitch)
        {
            cc.director.getPhysicsManager().on('one-step', ()=>
            {
                this.updatePos();
            });
        }
    }

    protected onEnable() 
    {
        // cc.director.getCollisionManager().attachDebugDrawToCamera(this.camera);
        // cc.director.getPhysicsManager().attachDebugDrawToCamera(this.camera);
    }

    protected onDisable() 
    {
        // cc.director.getCollisionManager().detachDebugDrawFromCamera(this.camera);
        // cc.director.getPhysicsManager().detachDebugDrawFromCamera(this.camera);
    }

    protected onDestroy()
    {
        super.onDestroy();

        this.target && this.target.off("position-changed", this.updatePos);
        this.target = null;

        if(GameConfig.PhysicsSwitch)
        {
            cc.director.getPhysicsManager().off('one-step');
        }
    }

    private updatePos()
    {
        if(this.target){
            let targetPos = this.target.convertToWorldSpaceAR(cc.Vec2.ZERO);
            this.node.position = this.node.parent.convertToNodeSpaceAR(targetPos);

            // GameLog.PrintLog("更新相机坐标", this.node.x, this.node.y);
        }
    }

    public setTarget(target: cc.Node){
        if(target){
            target.on("position-changed", this.updatePos.bind(this));
        }else{
            this.target && this.target.off("position-changed", this.updatePos);
        }

        this.target = target;

        this.updatePos();

        // this.scheduleOnce(this.updatePos.bind(this), 0.01);
    }

    // // called every frame, uncomment this function to activate update callback
    // lateUpdate(){
        // if(this.target)
        // {
        //     let targetPos = this.target.convertToWorldSpaceAR(cc.Vec2.ZERO);
        //     this.node.position = this.node.parent.convertToNodeSpaceAR(targetPos);
        //     GameLog.PrintDebug("CameraPosUpdate:");
        // }
        // let ratio = targetPos.y / cc.winSize.height;
        // this.camera.zoomRatio = 1 + (0.5 - ratio) * 0.5;
    // }
}
