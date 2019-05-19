import {BevState} from "./BevDefine";
import {MAX_CHILDREN} from "./BevDefine";
import BevNodePrecondition from './BevNodePrecondition';
import BevInputData from './BevInputData';
import BevOutputData from './BevOutputData';

export default class BevNode
{

	protected _debugName: string 					= null;
	protected _precondition: BevNodePrecondition 	= null;
	protected _children: Array<BevNode> 			= null;
	protected _parent: BevNode				 		= null;

	constructor(debugName = ""){
		// ----------------------------------------------------------------
		// :: Private Members
		this._precondition = null;
		this._children = null;
		this._parent = null;
		this._debugName = arguments[0] || "";
	};

	getDebugName(): string
	{
		return this._debugName;
	};

	/**
	 * add a child of bevhaior node 
	 * @param node
	 * @return this
	 */
	addChild(node: BevNode)
	{
		if(!this._children)
			this._children = [];
		
		if(this._children.length == MAX_CHILDREN)
			throw new Error( this + " overflow, max children number is " + MAX_CHILDREN);
		
		this._children.push( node );
		node._parent = this;
		return this;
	};

	/**
	 * insert a child at the specified index 
	 * @param node
	 * @param index 
	 * @return this
	 */
	addChildAt( node: BevNode, index: number )
	{
		this.addChild( node );
		
		if(index < 0)
			index = 0;
		else if(index > this._children.length - 1)
			index = this._children.length;
		
		for(let i = this._children.length - 1; i > index; --i)
			this._children[i] = this._children[i-1];
		
		this._children[index] = node;
		
		return this;
	};

	/**
	 * set the precondition 
	 * @param precondition
	 * @return this 
	 */
	setPrecondition( precondition: BevNodePrecondition )
	{
		this._precondition = precondition;
		return this;
	};

	evaluate( input: BevInputData ): boolean
	{
		let ret = !this._precondition || this._precondition.evaluate( input );
		return ret && this.doEvaluate( input );
	};

	transition( input: BevInputData )
	{
		this.doTransition( input );
	};

	tick( input: BevInputData, output: BevOutputData ) : BevState
	{
		return this.doTick( input, output );
	};

	doEvaluate( input: BevInputData ): boolean
	{
		return true;
	};

	doTransition( input: BevInputData )
	{
		// nothing to do ... implement yourself
	};

	doTick( input: BevInputData, output: BevOutputData ) : BevState
	{
		return BevState.BRS_Finish;
	};

	checkIndex( i: number ) : boolean
	{
		return i > -1 && i < this._children.length;
	}

	destroy()
	{
		if(this._children)
		{
			for(let i: number = 0, len = this._children.length; i < len;i ++)
			{
				let child = this._children[i];
				if(child)
				{
					child.destroy();
				}
			}
			this._children.length = 0;
			this._children = null;
		}

		if(this._parent)
		{
			this._parent = null;
		}

		if(this._precondition)
		{
			this._precondition = null;
		}
	}
};
