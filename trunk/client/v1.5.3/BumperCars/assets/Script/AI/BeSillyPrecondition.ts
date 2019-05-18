import BevNodePrecondition from "./BevNodePrecondition";
import BevInputData from './BevInputData';

//犯傻前提节点类
export default class BeSillyPrecondition extends BevNodePrecondition{

	constructor(debugName = ""){
		super(debugName);
	};

	evaluate(input: BevInputData)
	{
		return input.getIsBeSilly()
	}
	
}
