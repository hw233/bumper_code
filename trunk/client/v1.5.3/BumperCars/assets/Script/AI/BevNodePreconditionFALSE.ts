import BevNodePrecondition from "./BevNodePrecondition";
import BevInputData from './BevInputData';

export default class BevNodePreconditionFALSE extends BevNodePrecondition{
	constructor(debugName: string = "")
	{
		super(debugName);
	};

	evaluate( input: BevInputData )
	{
		return false;
	}
}
