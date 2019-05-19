import  Util  from "./common/Util";

const {ccclass, property} = cc._decorator;

@ccclass
export default class RandomMatch extends cc.Component{

    public startMatch (matchNum: number, minTime: number, maxTime: number, callback: Function, complate: Function):void {
        let self = this;

        let sTime: number = Util.RandomNum(minTime, maxTime);
        let mTime: number = sTime * 1000;

        let timePointVec: Array<number> = [];

        for(let i:number = 0; i < matchNum; i ++){
            let timePoint:number = Util.RandomNum(1, mTime);
            timePointVec.push(timePoint);
        }

        Util.Sort(timePointVec, true);

        let tempCount: number = 0;
        this.schedule(function(dt){
            tempCount += (dt * 1000);
            if(timePointVec[0] <= tempCount){
                timePointVec.splice(0, 1);
                callback(matchNum - timePointVec.length);
            }
            if(tempCount >= mTime){
                complate();
                self.unscheduleAllCallbacks();
            }
        }, 0.016);
    }
};