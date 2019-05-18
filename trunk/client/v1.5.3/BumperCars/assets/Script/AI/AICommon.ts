
import Util from "../common/Util";
import PlayerMgr from "../manager/PlayerMgr";
import GameLogic from "../GameLogic";


export default class AICommon{

    //判断一个点是否在地图的可活动范围内
    public static IsInActiveRange(pos: cc.Vec2, mapNode: cc.Node): boolean{
        if(mapNode){
            let colliderNode = mapNode.getChildByName("ColiderNode");
            if(colliderNode){
                let circleColliderVec = colliderNode.getComponents(cc.CircleCollider);
                if(circleColliderVec && circleColliderVec.length > 0){
                    for(let i = 0;i < circleColliderVec.length;i ++){
                        let circleCollider = circleColliderVec[i];
                        let offset = circleCollider.offset;
                        let radius = circleCollider.radius;

                        if(Util.GetDistance(pos, offset) <= radius){
                            return true;
                        }
                    }
                }

                let boxColliderVec = colliderNode.getComponents(cc.BoxCollider);
                if(boxColliderVec && boxColliderVec.length > 0){
                    for(let i = 0;i < boxColliderVec.length;i ++){
                        let boxCollider = boxColliderVec[i];
                        let offset = boxCollider.offset;
                        let halfWidth = boxCollider.size.width * 0.5;
                        let halfHeight = boxCollider.size.height * 0.5;

                        let x1 = offset.x - halfWidth;
                        let y1 = offset.y - halfHeight;

                        let dis = 0;

                        let rect = cc.rect(x1 + dis * 0.5, y1 + dis * 0.5, boxCollider.size.width - dis, boxCollider.size.height - dis);
                        if(false/*rect.contains(pos)*/){
                            return true;
                        }
                    }
                }
            }
            // let pllygonColliderVec = colliderNode.getComponents(cc.PolygonCollider);
        }
        return false;
    };

    //判断识别圈内是否有车辆
    public static IsHasCar(radius: number, selfID: number, selfPos: cc.Vec2){
        let playerList = PlayerMgr.GetActivePlayerList();

        for(let i = 0;i < playerList.length;i ++){
            let playerID = playerList[i];
            let player = PlayerMgr.GetPlayer(playerID);
            if(player && player.getState() == 1){
                let car = GameLogic.GetInstance().GetCarManager().getCarByID(playerID);
                if(car){
                    let carCom = car.getComponent("CarCom");

                    //自己的车除外
                    if(carCom.node.userID == selfID){
                        continue;
                    }

                    let targetPos = car.getPosition();
            
                    if(Util.GetDistance(targetPos, selfPos) <= radius){
                        return true;
                    }
                }
            }
        }

        return false;
    };


    //找寻识别圈内的所有车辆
    public static FindCarByDistance(radius: number, selfID: number, selfPos: cc.Vec2){
        let cars = [];

        let playerList = PlayerMgr.GetActivePlayerList();

        for(let i: number = 0;i < playerList.length;i ++){
            let playerID = playerList[i];
            let player = PlayerMgr.GetPlayer(playerID);
            if(player && player.getState() == 1){
                let car = GameLogic.GetInstance().GetCarManager().getCarByID(playerID);
                if(car){
                    let carCom = car.getComponent("CarCom");

                    //自己的车除外
                    if(carCom.node.userID == selfID){
                        continue;
                    }

                    let targetPos = car.getPosition();
            
                    if(Util.GetDistance(targetPos, selfPos) <= radius){
                        cars.push(carCom.node.userID);
                    }
                }
            }
        }

        return cars;
    };

    //在自己的识别圈内随机寻找一个在活动范围内的点
    public static FindRandomPos(selfPos: cc.Vec2, radius: number, mapNode: cc.Node){

        let points = this.FindRandomPointByRange(300, radius, selfPos, mapNode);
        for(let i = points.length - 1; i >= 0; i --){
            let point = points[i];
            if(this.IsInDeaArea(selfPos, point, mapNode)){
                points.splice(i, 1);
            }
        }

        return Util.RandomElement(points)
    };

    //在给定的范围内随机找一些点
    public static FindRandomPointByRange(min: number, max: number, selfPos: cc.Vec2, mapNode: cc.Node){
        let randomDistance = Util.RandomNum(min, max);

        let points = Util.SurroundPoints(selfPos.x, selfPos.y, randomDistance, 10);

        //去除不在范围内的点
        for(let i = points.length - 1; i >= 0; i --){
            let point = points[i];

            if(this.IsInActiveRange(point, mapNode) == false){
                points.splice(i, 1);
            }else{
                // this.addDebugNode(point);
            }
        }

        if(points.length <= 0){
            return this.FindRandomPointByRange(min, max - 100, selfPos, mapNode);
        }

        // cc.assert(points.length > 0, "points == 0");

        return points;
    };

    //判断一条直线是否穿过地图的死亡区域
    public static IsInDeaArea(p1: cc.Vec2, p2: cc.Vec2, gameMapNode: cc.Node){
        let deaAreaNode = gameMapNode.getChildByName("DeaAreaNode");
        if(!deaAreaNode){
            return;
        }

        // let circleColliderVec = deaAreaNode.getComponents(cc.CircleCollider);
        // if(circleColliderVec && circleColliderVec.length > 0){
        //     for(let i = 0;i < circleColliderVec.length;i ++){
        //         let circleCollider = circleColliderVec[i];
        //         let offset = circleCollider.offset;
        //         let radius = circleCollider.radius;

        //         if(Util.GetDistance(pos, offset) <= radius){
        //             return true;
        //         }
        //     }
        // }

        let boxColliderVec = deaAreaNode.getComponents(cc.BoxCollider);
        if(boxColliderVec && boxColliderVec.length > 0){
            for(let i = 0;i < boxColliderVec.length;i ++){
                let boxCollider = boxColliderVec[i];
                let offset = boxCollider.offset;
                let halfWidth = boxCollider.size.width * 0.5;
                let halfHeight = boxCollider.size.height * 0.5;
                let x1 = offset.x - halfWidth;
                let x2 = offset.x + halfWidth;
                let y1 = offset.y - halfHeight;
                let y2 = offset.y + halfHeight;

                let leftDownPoint = cc.v2(x1, y1);
                let leftUpPoint = cc.v2(x1, y2);
                let rightDownPoint = cc.v2(x2, y1);
                let rightUpPoint = cc.v2(x2, y2);

                if(cc.pSegmentIntersect(p1, p2, leftDownPoint, leftUpPoint)){
                    return true;
                }else if(cc.pSegmentIntersect(p1, p2, leftUpPoint, rightUpPoint)){
                    return true;
                }else if(cc.pSegmentIntersect(p1, p2, rightUpPoint, rightDownPoint)){
                    return true;
                }else if(cc.pSegmentIntersect(p1, p2, rightDownPoint, leftDownPoint)){
                    return true;
                }
            }
        }

        return false;
    };
};
