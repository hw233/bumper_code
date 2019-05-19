import GlobalVar from '../GlobalVar';
import GameLogic from '../GameLogic';
import Util from '../common/Util';
import PlayerMgr from './PlayerMgr';
import GameData from '../data/GameData';
import EngineUtil from '../common/EngineUtil';
import { AtlasType } from '../common/EngineUtil';

const {ccclass, property} = cc._decorator;

@ccclass
export default class BeaconManager extends cc.Component{

    @property(cc.Prefab)
    private beaconPrefab: cc.Prefab = null;

    private beacons: Array<cc.Node> = null;

    private beaconParent: cc.Node = null;

    init () 
    {
        this.beacons = [];
        this.beaconParent = cc.find("GameMap/OtherNode");
    }

    doStart()
    {
    }

    addBeacon(userID: number)
    {
        if(this.beaconPrefab)
        {
            let beaconNode: cc.Node = cc.instantiate(this.beaconPrefab);
            beaconNode.userID = userID;
            beaconNode.x = 9999;
            this.beacons.push(beaconNode);
            this.beaconParent.addChild(beaconNode);
        }
    }

    removeBeacon(userID: number){
        if(this.beacons)
        {
            for(let i = 0, len = this.beacons.length; i < len; i ++)
            {
                if(this.beacons[i] && this.beacons[i].userID == userID)
                {
                    this.beacons[i].destroy();
                    this.beacons.splice(i, 1);
                    break;
                }
            }
        }
    }

    public frameOnMove(dt: number): void
    {
        if(this.beacons)
        {
            let winSize: cc.Size = cc.director.getWinSize();

            let width = winSize.width - 58;
            let height = winSize.height - 50;

            let selfCar: cc.Node = GameLogic.GetInstance().GetCarManager().getCarByID(GlobalVar.userID);
            if(selfCar)
            {
                for(let i = 0, len = this.beacons.length; i < len; i ++)
                {
                    let beacon: cc.Node = this.beacons[i];
                    if(beacon)
                    {
                        this.updateTexure(beacon);

                        let otherCar: cc.Node = GameLogic.GetInstance().GetCarManager().getCarByID(beacon.userID);
                        if(otherCar && PlayerMgr.IsAlive(beacon.userID))
                        {
                            let startPos: cc.Vec2 = cc.v2();
                            let endPos: cc.Vec2 = cc.v2();

                            //左边
                            startPos.x = selfCar.x + width * 0.5;
                            startPos.y = selfCar.y + height * 0.5;

                            endPos.x = selfCar.x + width * 0.5;
                            endPos.y = selfCar.y - height * 0.5;

                            if(cc.pSegmentIntersect(selfCar.position, otherCar.position, startPos, endPos))
                            {
                                this.updatePos(beacon, selfCar.position, otherCar.position, startPos, endPos);
                                continue;
                            }

                            //右边
                            startPos.x = selfCar.x - width * 0.5;
                            startPos.y = selfCar.y + height * 0.5;

                            endPos.x = selfCar.x - width * 0.5;
                            endPos.y = selfCar.y - height * 0.5;
                            if(cc.pSegmentIntersect(selfCar.position, otherCar.position, startPos, endPos))
                            {
                                this.updatePos(beacon, selfCar.position, otherCar.position, startPos, endPos);
                                continue;
                            }

                            //上边
                            startPos.x = selfCar.x - width * 0.5;
                            startPos.y = selfCar.y + height * 0.5; 

                            endPos.x = selfCar.x + width * 0.5;
                            endPos.y = selfCar.y + height * 0.5;
                            if(cc.pSegmentIntersect(selfCar.position, otherCar.position, startPos, endPos))
                            {
                                this.updatePos(beacon, selfCar.position, otherCar.position, startPos, endPos);
                                continue;
                            }

                            //下边
                            startPos.x = selfCar.x - width * 0.5;
                            startPos.y = selfCar.y - height * 0.5;

                            endPos.x = selfCar.x + width * 0.5;
                            endPos.y = selfCar.y - height * 0.5;
                            if(cc.pSegmentIntersect(selfCar.position, otherCar.position, startPos, endPos))
                            {
                                this.updatePos(beacon, selfCar.position, otherCar.position, startPos, endPos);
                                continue;
                            }

                            beacon.x = 9999;
                        }
                        else
                        {
                            beacon.x = 9999;
                        }
                    }
                }
            }
        }
    }

    private updatePos(beacon:cc.Node, a: cc.Vec2, b: cc.Vec2, c: cc.Vec2, d: cc.Vec2): void
    {
        if(beacon)
        {
            let intersectPos: cc.Vec2 = cc.pIntersectPoint(a, b, c, d);
            beacon.x = intersectPos.x;
            beacon.y = intersectPos.y;

            let rotate = Util.GetRotation(a, b);
            beacon.rotation = rotate;
        }
    }

    private updateTexure(beacon: cc.Node): void
    {
        if(beacon)
        {
            let overlordID = GameData.GetBettleData().getOverlordUserID();

            if( overlordID === beacon.userID )
            {
                EngineUtil.SetNodeTextureByType(AtlasType.AT_GameMap, beacon, "overloadBeacon")
            }
            else
            {
                EngineUtil.SetNodeTextureByType(AtlasType.AT_GameMap, beacon, "beacon")
            }
        }
    }

    public clearAll(): void
    {
        if(this.beacons)
        {
            this.beacons.length = 0;
            this.beacons = null;
        }
    }

    onDestroy()
    {
        this.clearAll();
    }
}