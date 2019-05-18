import {BevState} from "./BevDefine";
import {MAX_CHILDREN} from "./BevDefine";

import BevNode from "./BevNode";
import BevInputData from './BevInputData';
import BevOutputData from './BevOutputData';

export default class BevNodeParallel extends BevNode{

	protected _childrenStatus: Array<BevState> = null;
	protected _finishCondition: number = null;

	constructor(debugName = ""){
		super(debugName);

		this._childrenStatus = [];
		this.resetChildrenStatus();
	};

	setFinishCondition( condition )
	{
		this._finishCondition = condition;
		return this;
	};
	
	doEvaluate(input) : boolean
	{
		let len = this._children.length;
		for(let i = 0; i < len; ++i)
		{
			if( this._childrenStatus[i] == BevState.BRS_Executing )
			{
				if( !this._children[i].evaluate( input ) )
				{
					return false;
				}
			}
		}
		
		return true;
	};
	
	doTick(input: BevInputData, output: BevOutputData): BevState
	{
		let i;
		let len = this._children.length;
		
		if( this._finishCondition == 0 )
		{
			for( i = 0; i < len; ++i )
			{
				if( this._childrenStatus[i] == BevState.BRS_Executing )
					this._childrenStatus[i] = this._children[i].tick( input, output );
				
				if( this._childrenStatus[i] != BevState.BRS_Executing )
				{
					this.resetChildrenStatus();
					return BevState.BRS_Finish;
				}
					
			}
		}
		else if( this._finishCondition == 1 )
		{
			let finishedCount = 0;
			
			for( i = 0; i < len; ++i )
			{
				if( this._childrenStatus[i] == BevState.BRS_Executing )
					this._childrenStatus[i] = this._children[i].tick( input, output );
				
				if( this._childrenStatus[i] != BevState.BRS_Executing )
					++ finishedCount;
			}
			
			if( finishedCount == len )
			{
				this.resetChildrenStatus();
				return BevState.BRS_Finish;
			}
		}
		else
		{
			throw new Error("Unknown finish condition :" + this._finishCondition);
		}
		
		return BevState.BRS_Executing;
	};
	
	doTransition( input: BevInputData )
	{
		this.resetChildrenStatus();
		
		let len = this._children.length;
		for(let i = 0; i < len; ++i )
			this._children[i].transition( input );
	};
	
	// ----------------------------------------------------------------
	// :: Private Methods
	resetChildrenStatus()
	{
		for(let i = 0; i < MAX_CHILDREN; ++i )
			this._childrenStatus[i] = BevState.BRS_Executing;
	}

	public destroy()
	{
		super.destroy();
		
		if(this._childrenStatus)
		{
			this._childrenStatus.length = 0;
			this._childrenStatus = null;
		}
	}
}

