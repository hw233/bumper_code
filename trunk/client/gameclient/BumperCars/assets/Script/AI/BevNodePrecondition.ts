import BevInputData from './BevInputData';
export default class BevNodePrecondition
{
	constructor(debugName: string = "")
	{

	};
	
	evaluate( input: BevInputData )
	{
		throw new Error("This is an abstract method. You need to implement yourself.");
	}
	
	// ----------------------------------------------------------------
	// :: Override Methods
	
	// ----------------------------------------------------------------
	// :: Private Methods
	
	// ----------------------------------------------------------------
	// :: Private Members
}
