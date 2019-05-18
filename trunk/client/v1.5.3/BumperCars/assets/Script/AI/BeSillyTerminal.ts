import AICommon from "./AICommon";
import Util from "../common/Util";

import {BevState} from "./BevDefine";
import BevNodeTerminal from "./BevNodeTerminal";
import BevInputData from './BevInputData';
import BevOutputData from './BevOutputData';

//犯傻行为节点类
export default class BeSillyTerminal extends BevNodeTerminal{

	protected recordTime: number = null;
	protected targetPos: cc.Vec2 = null;

	constructor(debugName = ""){
		super(debugName);
		this.recordTime = 0;
		this.targetPos = null;
	};

	doEnter( input: BevInputData )
	{
		super.doEnter(input);
		
		this.recordTime = 0;

		//自己的当前位置
		let selfPos: cc.Vec2 = input.getSelfLocation();

		//识别半径
		let radius: number = input.getRadius();
		
		//在识别圈内随机获得一个目标点
        this.targetPos = AICommon.FindRandomPos(selfPos, radius, input.getMapNode());
	};
	
	doExecute( input: BevInputData, output: BevOutputData )
	{
		this.recordTime += input.getDelayTime();

		if(this.recordTime >= 3){
			//犯傻时间超过3秒则结束
			return BevState.BRS_Finish;
		}
		
		//自己的当前位置
		let selfPos = input.getSelfLocation();

		let targetPos = this.targetPos;

        if(targetPos && Util.GetDistance(targetPos, selfPos) <= 50){
            //已经移动到了目的地,则结束
			return BevState.BRS_Finish;
        }

		//自己的大小尺寸
		let selfSize = input.getSelfSize();
		//地图节点
		let mapNode = input.getMapNode();

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
		this.targetPos = null;
	};


	//private mothon
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
}
