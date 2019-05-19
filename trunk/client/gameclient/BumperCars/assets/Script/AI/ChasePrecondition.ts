import BevNodePrecondition from "./BevNodePrecondition";
import AICommon from "./AICommon";
import BevInputData from './BevInputData';

//追赶前提节点类
export default class ChasePrecondition extends BevNodePrecondition{

	constructor(debugName: string = "")
	{
		super(debugName);
	};

	evaluate(input: BevInputData): boolean
	{
		let isChase = input.getRandomNum() > input.getSillyValue();

		isChase = isChase && AICommon.IsHasCar(input.getRadius(), input.getSelfID(), input.getSelfLocation());

		if(isChase){
			input.setIsBeSilly(false);
		}else{
			input.setIsBeSilly(true);
		}

		return isChase;
	};
};