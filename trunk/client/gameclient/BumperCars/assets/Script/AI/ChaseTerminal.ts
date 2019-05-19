
import AICommon from "./AICommon";
import Util from "../common/Util";

import {BevState} from "./BevDefine";
import BevNodeTerminal from "./BevNodeTerminal";
import GameLogic from "../GameLogic";
import BevInputData from './BevInputData';
import BevOutputData from './BevOutputData';

//追赶行为节点类
export default class BeSillyTerminal extends BevNodeTerminal{

    protected randomTime    = null;
    protected recordTime    = null;
    protected targetID    = null;

    constructor(debugName: string = ""){

        super(debugName);

		this.randomTime = 0;
        this.recordTime = 0;
        
        this.targetID = null;
	};

	doEnter( input:BevInputData )
	{
        super.doEnter(input);

		this.recordTime = 0;
        this.randomTime = Util.RandomNum(1, 2);
        
        //我的坐标
        let selfPos = input.getSelfLocation();
        
    	//识别圈内的所有车
        let targetCars = AICommon.FindCarByDistance(input.getRadius(), input.getSelfID(), selfPos);

        //谁死谁活，全凭天意，随机取它一辆
        this.targetID = Util.RandomElement(targetCars);
	};
	
	doExecute( input: BevInputData, output: BevOutputData ): BevState
	{
		this.recordTime += input.getDelayTime();

		if(this.recordTime >= this.randomTime){
			//追赶指定的随机时间后则结束当前行为
            return BevState.BRS_Finish;
		}

        //我的坐标
        let selfPos = input.getSelfLocation();

		let target = GameLogic.GetInstance().GetCarManager().getCarByID(this.targetID);

		if(!target){
			//目标id无效, 结束当前行为
			return BevState.BRS_Finish;
		}

		//自己的大小尺寸
        let selfSize = input.getSelfSize();
        
		//地图节点
		let mapNode = input.getMapNode();

        //获取目标车的坐标
        let targetPos = target.getPosition();

		if(this.checkTargetPos(selfPos, targetPos, selfSize, mapNode) == false){
			//下一个移动的位置不在地图的活动区域内,则结束
			return BevState.BRS_Finish;
		}

		//旋转角度
        let rotate = Util.GetRotation(selfPos, targetPos);
        output.setRotate(rotate);

        //设置移动向量
        let subVec = targetPos.sub(selfPos);
        let normalizeVec = subVec.normalize();

        output.setLinearVelocity(normalizeVec);

		return BevState.BRS_Executing;
	};
	
	doExit( input: BevInputData, exitID: number )
	{
        super.doExit(input, exitID);

		this.recordTime = 0;
		this.randomTime = 0;
        this.targetID = null;
	};

	//判读下一位置是否在活动区域内
    checkTargetPos(selfPos: cc.Vec2, targetPos: cc.Vec2, selfSize: cc.Size, mapNode: cc.Node): boolean
    {
        let subVec = targetPos.sub(selfPos);
        let normalizeVec = subVec.normalize();

        let newVec = normalizeVec.mul(selfSize.width * 0.5);

        if(AICommon.IsInActiveRange(selfPos.add(newVec), mapNode)){
            // this.addDebugNode(targetPos);
            return true;
        }else{
            // this.switchTarget();
            // this.isMoving = false;
            // //找到距离目标最近的点，作为临时位置
            // let nearestPoint = this.findNearestPoint(selfPos, targetPos, 400);
            // if(nearestPoint){
            //     this.addDebugNode(nearestPoint);
            //     this.tempTargetPos = nearestPoint;
            //     this.moveToTargetPos(nearestPoint);
            // }else{
            //     //实在找不到可移动的路径的话，也不能弃疗，重新寻找目标
            //     this.startMove();
            // }
            return false;
        }
    };
};
