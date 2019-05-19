import {BevState} from "./BevDefine";

import BevNode from "./BevNode";
import BevInputData from './BevInputData';
import BevOutputData from './BevOutputData';

export default class BevNodeSequence extends BevNode{

	private _currentNodeIndex: number = null;

	constructor(debugName: string = ""){
		super(debugName);

		this._currentNodeIndex = -1;
	};

	doEvaluate( input: BevInputData ): boolean
	{
		let index = this._currentNodeIndex;
		if( index == -1 )
			index = 0;
		
		if( this.checkIndex( index ) )
		{
			if( this._children[ index ].evaluate( input ) )
				return true;
		}
		
		return false;
	};
	
	doTick(input: BevInputData, output: BevOutputData): BevState
	{
		let isFinish = BevState.BRS_Finish;
		
		if( this._currentNodeIndex == -1 )
			this._currentNodeIndex = 0;
		
		isFinish = this._children[ this._currentNodeIndex ].tick( input, output );
		if( isFinish )
		{
			++ this._currentNodeIndex;
			if( this._currentNodeIndex == this._children.length )
				this._currentNodeIndex = -1;
			else
				isFinish = BevState.BRS_Executing;
		}
		
		if( isFinish < 0 ) // error
			this._currentNodeIndex = -1;
		
		return isFinish;
	};
	
	doTransition(input)
	{
		if( this.checkIndex( this._currentNodeIndex ) )
			this._children[ this._currentNodeIndex ].transition( input );
		this._currentNodeIndex = -1;
	}
}