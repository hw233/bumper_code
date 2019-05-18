import Util from "../common/Util";

const {ccclass, property} = cc._decorator;

@ccclass
export default class ColliderArea extends cc.Component{

    @property({type: cc.Node})
    private rectSprite1:cc.Node = null;

    @property({type: cc.Node})
    private rectSprite2:cc.Node = null;

    @property({type: cc.Node})
    private rectSprite3:cc.Node = null;

    public getColliderAreaType(worldPos): number
    {

        // let boundingBoxCenter1: cc.Vec2 = this.rectSprite1.getBoundingBoxToWorld().center;
        // let boundingBoxCenter2: cc.Vec2 = this.rectSprite2.getBoundingBoxToWorld().center;
        // let boundingBoxCenter3: cc.Vec2 = this.rectSprite3.getBoundingBoxToWorld().center;

        let boundingBoxCenter1: cc.Vec2 = this.rectSprite1.convertToWorldSpaceAR(cc.v2(0, 0));
        let boundingBoxCenter2: cc.Vec2 = this.rectSprite2.convertToWorldSpaceAR(cc.v2(0, 0));
        let boundingBoxCenter3: cc.Vec2 = this.rectSprite3.convertToWorldSpaceAR(cc.v2(0, 0));


        // let boundingBoxCenter1: cc.Vec2 = this.rectSprite1.getBoundingBox().center;
        // let boundingBoxCenter2: cc.Vec2 = this.rectSprite2.getBoundingBox().center;
        // let boundingBoxCenter3: cc.Vec2 = this.rectSprite3.getBoundingBox().center;

        let d1: number = Util.GetDistance(worldPos, boundingBoxCenter1);
        let d2: number = Util.GetDistance(worldPos, boundingBoxCenter2);
        let d3: number = Util.GetDistance(worldPos, boundingBoxCenter3);

        return (d1 < d2) ? ( (d1 < d3) ? 1 : 3 ) : ( (d2 < d3) ? 2 : 3 );

        // if(d1 < d2){
        //     if(d1 < d3){
        //         return 1;
        //     }else{
        //         return 3;
        //     }
        // }else{
        //     if(d2 < d3){
        //         return 2;
        //     }else{
        //         return 3;
        //     }
        // }
        // return 1;
    }
}
