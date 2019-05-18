import {BevState} from "./BevDefine";

import BevNode from "./BevNode";
import BevInputData from './BevInputData';
import BevOutputData from './BevOutputData';

export default class BevNodeLoop extends BevNode{

	protected _loopCount: number 		= null;
	protected _currentLoop: number	 	= null;

	constructor(debugName = ""){
		super(debugName);
		this._loopCount = -1;
		this._currentLoop = 0;
	};

	setLoopCount( n: number )
	{
		this._loopCount = n;
		return this;
	};
	
	doEvaluate(input: BevInputData): boolean
	{
		let checkLoop: boolean = (this._loopCount == -1) || (this._currentLoop < this._loopCount);
		
		if(!checkLoop)
			return false;
		
		if( this.checkIndex(0) )
			if( this._children[0].evaluate( input ) )
				return true;
		
		return false;
	};
	
	doTick(input: BevInputData, output: BevOutputData): BevState
	{
		let isFinish: BevState = BevState.BRS_Finish;
		
		if( this.checkIndex(0) )
		{
			isFinish = this._children[0].tick( input, output );
			
			if( isFinish == BevState.BRS_Finish )
			{
				if( this._loopCount == -1 )
					isFinish = BevState.BRS_Executing;
				else
				{
					++ this._currentLoop;
					if( this._currentLoop < this._loopCount )
						isFinish = BevState.BRS_Executing;
				}
			}
		}
		
		if( isFinish == BevState.BRS_Finish )
			this._currentLoop = 0;
		
		return isFinish;
	};
	
	doTransition(input: BevInputData)
	{
		if( this.checkIndex(0) )
			this._children[0].transition( input );
		
		this._currentLoop = 0;
	}
};