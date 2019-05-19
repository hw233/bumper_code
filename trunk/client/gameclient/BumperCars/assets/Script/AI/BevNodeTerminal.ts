import {BevState} from "./BevDefine";
import {BevTerminalState} from "./BevDefine";
import {BRS_ERROR_TRANSITION} from "./BevDefine";

import BevNode from "./BevNode";
import Util from "../common/Util";
import BevInputData from './BevInputData';
import BevOutputData from './BevOutputData';

export default class BevNodeTerminal extends BevNode{

	protected _status: BevTerminalState = null;
	protected _needExit: boolean = null;

	constructor(debugName: string = "")
	{
		super(debugName);

		this._status = BevTerminalState.BTS_READY;
		this._needExit = null;
	};
	
	// ----------------------------------------------------------------
	// :: Override Methods
	
	doTick(input: BevInputData, output: BevOutputData): BevState
	{
		let isFinish = BevState.BRS_Finish;
		
		if( this._status == BevTerminalState.BTS_READY )
		{
			this.doEnter( input );
			this._needExit = true;
			this._status = BevTerminalState.BTS_RUNNING;
		}
		
		if( this._status == BevTerminalState.BTS_RUNNING )
		{
			isFinish = this.doExecute( input, output );
			if( isFinish == BevState.BRS_Finish || isFinish < 0 )
				this._status = BevTerminalState.BTS_FINISH;
		}
		
		if( this._status == BevTerminalState.BTS_FINISH )
		{
			if(this._needExit)
				this.doExit( input, isFinish );
			
			this._status = BevTerminalState.BTS_READY;
			this._needExit = false;
		}
		
		return isFinish;
	};
	
	doTransition( input: BevInputData )
	{
		if( this._needExit )
			this.doExit( input, BRS_ERROR_TRANSITION );
		
		this._status = BevTerminalState.BTS_READY;
		this._needExit = false;
	};
	
	// ----------------------------------------------------------------
	// :: Private Methods
	doEnter( input: BevInputData )
	{
		// nothing to do...implement yourself
	};
	
	doExecute( input: BevInputData, output: BevOutputData ): BevState
	{
		return BevState.BRS_Finish;
	};
	
	doExit( input: BevInputData, exitID: number )
	{
        input.setRandomNum(Util.RandomNum(1, 100));
	}
};