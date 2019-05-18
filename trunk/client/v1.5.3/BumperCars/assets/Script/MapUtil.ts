import Util from "./common/Util";
import MapInfo from "./MapInfo";

import { ColliderType } from "./GameDefine";

export default class MapUtil{

    //判断一个点是否包含在一个圆内(与道具做碰撞检测使用)
    public static PointIsContactInCircle(point: cc.Vec2, x: number, y: number, radius: number): boolean
    {
        if(Util.GetDistance(point, cc.v2(x, y)) < radius)
        {
            return true;
        }
        return false;
    }

    //判断一个圆是否包含在一个圆内
    public static CircleInCircle(radius1: number, x1: number, y1: number, radius2: number, x2: number, y2: number): boolean
    {
        let distance = Util.GetDistance(cc.v2(x1, y1), cc.v2(x2, y2));
        if( (radius1 - distance) >= radius2)
        {
            return true;
        }
        return false;
    }

    //判断一个圆和一个矩形是否相交
    public static CircleRectIntersect(cRadius: number, cX: number, cY: number, rect: cc.Rect): boolean
    {
        if( 
            (cX - cRadius) < (rect.x + rect.width) && 
            (cY - cRadius) < (rect.y + rect.height) && 
            (cX + cRadius) > (rect.x) &&
            (cY + cRadius) > (rect.y) )
            {
                return true;
            }
        return false;
    }

    //判断一个圆和一个圆是否相交
    public static CircleCircleIntersect(cradius1: number, x1: number, y1: number, radius2: number, x2: number, y2: number): boolean
    {
        return true;
    }

    //找一个在地图活动区域内随机一个点
    public static RandomPointByActiveArea(): any
    {
        //获得地图的活动区域
        let activeAreaVec = MapInfo.GetActiveAreaVec();

        let activeArea = Util.RandomElement(activeAreaVec);

        let data: any = null;

        if(activeArea.type == ColliderType.CT_Circle)
        {
            data = this.RandomByCircle(activeArea.x, activeArea.y, activeArea.radius);
            data.tag = activeArea.tag;
        }else if(activeArea.type == ColliderType.CT_Box)
        {
            data = this.RandomByRect(activeArea.x, activeArea.y, activeArea.width, activeArea.height);
            data.tag = activeArea.tag;
        }
        return data;
    }

    //在一个矩形区域内随机一个点
    public static RandomByRect(x: number, y: number, width: number, height: number): cc.Vec2
    {
        let randomX = Util.RandomNum(x, x + width);
        let randomY = Util.RandomNum(y, y + height);

        return cc.v2(randomX, randomY);
    }

    //在一个圆内随机取一个点
    public static RandomByCircle(x: number, y: number, radius: number): cc.Vec2
    {
        let randomP = cc.v2(0, 0);

        //随机一个半径
        let randomR = Util.RandomNum(0, radius);
        //随机一个角度
        let randomA = Util.RandomNum(0, 360);

        randomP.x = Math.sin(  Util.AngleToRadian(randomA) ) * randomR;
        randomP.y = Math.cos(  Util.AngleToRadian(randomA) ) * randomR;

        randomP.x += x;
        randomP.y += y;
        
        return randomP;
    }

    //转换地图坐标为世界坐标
    public static ConvertMapToWorld(mapPos: cc.Vec2): cc.Vec2
    {
        let mapSize = MapInfo.GetMapSize();

        let size = cc.size(750, 1334);
        let worldX = mapPos.x - (mapSize.width - size.width) * 0.5;
        let worldY = (mapSize.height - size.height) * 0.5 + size.height - mapPos.y;

        return cc.v2(worldX, worldY);
    }

    public static ConvertMapYToCanvasY(mapY: number): number
    {
        let mapSize = MapInfo.GetMapSize();

        if(!mapSize)
        {
            return 0;
        }

        return mapSize.height - mapY;
    }
};