import BevNodePrioritySelector from "./BevNodePrioritySelector";
import BevInputData from './BevInputData';

export default class BevNodeNonePrioritySelector extends BevNodePrioritySelector{

	constructor(debugName = ""){
		super(debugName);
	};
	
	doEvaluate(input: BevInputData): boolean
	{
		if( this.checkIndex( this._currentSelectedIndex ) )
		{
			if( this._children[ this._currentSelectedIndex ].evaluate( input ) )
			{
				return true;
			}
		}
		
		return super.doEvaluate(input);
	}
};
