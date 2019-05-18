import {BevState} from "./BevDefine";

import BevNode from "./BevNode";
import BevInputData from './BevInputData';
import BevOutputData from './BevOutputData';

export default class BevNodePrioritySelector extends BevNode{

	protected _lastSelectedIndex: number 		= null;
	protected _currentSelectedIndex: number 	= null;

	constructor(debugName = ""){
		super(debugName);
		this._currentSelectedIndex = -1;
		this._lastSelectedIndex = -1;
	};

	doEvaluate(input: BevInputData): boolean
	{
		this._currentSelectedIndex = -1;
		
		let len = this._children.length;
		for(let i = 0; i < len; ++i)
		{
			if( this._children[i].evaluate( input ) )
			{
				this._currentSelectedIndex = i;
				return true;
			}
		}
		return false;
	};
	
	doTransition(input: BevInputData): void
	{
		if( this.checkIndex( this._lastSelectedIndex ) )
		{
			this._children[ this._lastSelectedIndex ].transition( input );
		}
		this._lastSelectedIndex = -1;
	};
	
	doTick(input: BevInputData, output: BevOutputData): BevState
	{
		let isFinish = BevState.BRS_Finish;
		
		if( this.checkIndex( this._currentSelectedIndex ) )
		{
			if( this._currentSelectedIndex != this._lastSelectedIndex )
			{
				if( this.checkIndex( this._lastSelectedIndex ) )
				{
					this._children[ this._lastSelectedIndex ].transition( input );
				}
				
				this._lastSelectedIndex = this._currentSelectedIndex;
			}
		}
		
		if( this.checkIndex( this._lastSelectedIndex ) )
		{
			isFinish = this._children[ this._lastSelectedIndex ].tick( input, output );
			if( isFinish == BevState.BRS_Finish ){
				this._lastSelectedIndex = -1;
				this._currentSelectedIndex = -1;
			}
		}
		
		return isFinish;
	}
}
