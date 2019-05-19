import EngineUtil, { AtlasType } from '../common/EngineUtil';
import CarUtil from '../CarUtil';
import ComponentBase from '../common/ComponentBase';
import MotionStreakBase from '../MotionStreak/MotionBase';
import MyMotionStreak from '../MotionStreak/MyMotionStreak';
import CustomMotion from '../MotionStreak/CustomMotion';
import MotionTableInfo from '../table/MotionTableInfo';
import ItemSystem from '../Item/ItemSystem';
import CarTableInfo from '../table/CarTableInfo';

const {ccclass, property} = cc._decorator;

//负责展示车库车辆
@ccclass
export default class ShowCarView extends ComponentBase
{
    //地图
    @property(cc.Node)
    private mapNode: cc.Node                    = null;
    @property(cc.Node)
    private mapNode1: cc.Node                   = null;
    @property(cc.Node)
    private mapNode2: cc.Node                   = null;

    //当前展示的车辆
    @property(cc.Node)
    private showCarNode: cc.Node                = null;

    //当前选中的车辆id
    private curCarID: number                    = null;

    //当前选中的拖尾id
    private curMotionID: number                 = null;

    private mapSize: cc.Size                    = null;

    private moveSpeed: number                   = null;

    private offect: number                      = null;

    private distance: number                    = null;

    protected onLoad () 
    {
        this.curCarID       = 0;
        this.curMotionID    = 0;
        this.distance       = 0;

        this.mapSize        = this.mapNode1.getContentSize();

        this.offect         = Math.sqrt(this.mapSize.width * this.mapSize.width * 2) / 2;

        this.mapNode1.isCanActive = true;
        this.mapNode2.isCanActive = false;

        this.showCarNode.zIndex = 10;
    }

    protected update(dt: number)
    {
        if(this.showCarNode)
        {
            this.showCarNode.x += this.moveSpeed;
            this.showCarNode.y += this.moveSpeed;
        }

        if(this.mapNode)
        {
            this.mapNode.x -= this.moveSpeed;
            this.mapNode.y -= this.moveSpeed;

            this.distance += this.moveSpeed;

            if( this.distance > this.offect )
            {
                if(this.mapNode1)
                {
                    this.mapNode1.x += this.distance;
                    this.mapNode1.y += this.distance;
                }

                if(this.mapNode2)
                {
                    this.mapNode2.x = this.mapNode1.x + this.offect;
                    this.mapNode2.y = this.mapNode1.x + this.offect;
                }

                this.mapNode1.isCanActive = !this.mapNode1.isCanActive;
                this.mapNode2.isCanActive = !this.mapNode2.isCanActive;

                this.distance = 0;
            }
        }
    }

    public refreshMotion(motionID: number)
    {
        this.curMotionID = motionID;
        let motionInfo: MotionTableInfo = ItemSystem.GetInstance().GetTableInfoByItemID(motionID);
        if(motionInfo)
        {
            let motion: MotionStreakBase = null;

            let motion1: MotionStreakBase = this.showCarNode.getComponent(MyMotionStreak);
            let motion2: MotionStreakBase = this.showCarNode.getComponent(CustomMotion);

            if(motionInfo.GetType() === 1)
            {
                motion = motion1;
            }
            else if(motionInfo.GetType() === 2)
            {
                motion = motion2;
            }

            motion1.enabled = false;
            motion2.enabled = false;

            if(motion)
            {
                motion.enabled = true;
                motion.setMotionID(motionID);
            }
        }
    }

    public refreshCar(carID: number)
    {
        this.curCarID = carID;
        this.refreshMoveSpeed();
        this.refreshShowCar();
    }

    private refreshMoveSpeed()
    {
        this.moveSpeed = CarUtil.CalcCarSpeeed(this.curCarID);
    }

    private refreshShowCar()
    {
        let tableInfo: CarTableInfo = ItemSystem.GetInstance().GetTableInfoByItemID(this.curCarID);
        if(tableInfo)
        {
            EngineUtil.SetNodeTextureByType(AtlasType.AT_StartScene, this.showCarNode, "Car_" + tableInfo.GetImgID())
        }
    }
}