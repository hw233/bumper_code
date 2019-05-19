import { ColliderType, GameModel } from './GameDefine';
import GlobalVar from './GlobalVar';
import Util from './common/Util';
import { PracticeCarPosTable } from './table/PracticeCarPosTable';
import { DLDCarPosTable } from './table/DLDCarPosTable';
import { DQZCarPosTable } from './table/DQZCarPosTable';

export default class MapInfo{
    
    //地图节点
    private static MapNode: cc.Node = null;

    //地图的尺寸
    private static MapSize: cc.Size = null;

    //地图的活动区域
    private static ActiveAreaVec: Array<any> = null;

    //车的位置
    private static CarPosVec: Array<any> = null;

    //死亡区域
    private static DeaAreaVec: Array<any> = null;

    public static Init(gameMap: cc.Node){
        this.ActiveAreaVec = [];
        this.DeaAreaVec = [];

        this.MapNode = gameMap;
        this.MapSize = cc.size(gameMap.width, gameMap.height);

        this.InitActiveAreaVec();
        this.InitCarPosVec();
        this.InitDeaAreaVec();

        this.InitEnd();
    }

    public static InitActiveAreaVec(){
        if(!this.MapNode)return;

        //活动区域
        let ActiveAreaNode = this.MapNode.getChildByName("ActiveAreaNode");
        if(ActiveAreaNode){

            let circleColliderVec = ActiveAreaNode.getComponents(cc.CircleCollider);
            for(let i = 0;i < circleColliderVec.length;i ++){
                let circleCollider = circleColliderVec[i];
                if(circleCollider.enabled){
                    this.ActiveAreaVec.push({
                        x: circleCollider.offset.x,
                        y: circleCollider.offset.y,
                        radius: circleCollider.radius,
                        type: ColliderType.CT_Circle,
                        tag: circleCollider.tag,
                    });
                }
            }

            let boxColliderVec = ActiveAreaNode.getComponents(cc.BoxCollider);
            for(let i = 0;i < boxColliderVec.length;i ++){
                let boxCollider = boxColliderVec[i];
                if(boxCollider.enabled){
                    let offset = boxCollider.offset;
                    let size = boxCollider.size;
                    let x = offset.x - size.width / 2;
                    let y = offset.y - size.height / 2;
                    this.ActiveAreaVec.push({
                        x: x,
                        y: y,
                        width: size.width,
                        height: size.height,
                        type: ColliderType.CT_Box,
                        tag: boxCollider.tag,
                    });
                }
            }

            // let polygonColliderVec = ActiveAreaNode.getComponents(cc.BoxCollider);
            // for(let i = 0;i < polygonColliderVec.length;i ++){
            //     let polygonCollider = polygonColliderVec[i];
            //     this.ActiveAreaVec.push({
            //         offsetX: boxCollider.offset.x,
            //         offsetY: boxCollider.offset.y,
            //         width: boxCollider.size.width,
            //         height: boxCollider.size.height,
            //         type: ColliderType.CT_Box,
            //     });
            // }
        }
    }

    public static InitCarPosVec(){
        if(!this.MapNode)return;

        //车的出生位置
        switch(GlobalVar.gameModel)
        {
            case GameModel.GM_PracticeMode:
            {
                this.CarPosVec = Util.CopyArray(PracticeCarPosTable);
                break;
            }
            case GameModel.GM_DLDMode:
            {
                this.CarPosVec = Util.CopyArray(DLDCarPosTable);
                break;
            }
            case GameModel.GM_DQZMode:
            {
                this.CarPosVec = Util.CopyArray(DQZCarPosTable);
                break;
            }
            default:
            {
                this.CarPosVec = [];
                break;
            }
        }
    }

    public static InitDeaAreaVec(){

        if(!this.MapNode)return;

        //死亡区域(暂时只支持圆形)
        let DeathAreaNode = this.MapNode.getChildByName("DeaAreaNode");
        if(DeathAreaNode){

            let circleColliderVec = DeathAreaNode.getComponents(cc.CircleCollider);
            for(let i = 0;i < circleColliderVec.length;i ++){
                let circleCollider = circleColliderVec[i];
                this.DeaAreaVec.push({
                    x: circleCollider.offset.x,
                    y: circleCollider.offset.y,
                    radius: circleCollider.radius,
                    type: ColliderType.CT_Circle,
                });
            }
        }
    }

    public static InitEnd(){
        this.MapNode = null;
    }

    public static Reset(){
        this.MapSize = null;
        this.MapNode = null;

        this.ActiveAreaVec.length = 0;
        this.CarPosVec.length = 0;
        this.DeaAreaVec.length = 0;
    }

    public static GetMapSize(){
        return this.MapSize;
    }

    public static GetActiveAreaVec(){
        return this.ActiveAreaVec;
    }

    public static GetCarPosVec(){
        return this.CarPosVec;
    }

    public static GetDeaAreaVec(){
        return this.DeaAreaVec;
    }

    public static GetProperty(objname, propertyname){
        return this.getProperty(objname, propertyname);
    };

    private static getProperty(obj, propertyname){
        if(obj){
            if(obj.getProperty){
                return obj.getProperty(propertyname);
            }
            if(obj[propertyname]){
                return obj[propertyname];
            }
        }
        return null;
    }
}