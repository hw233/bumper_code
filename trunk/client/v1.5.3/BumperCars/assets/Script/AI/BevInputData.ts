export default class BevInputData{

	private _targetLocation: cc.Vec2 		= null;
	private _selfLocation: cc.Vec2 			= null;
	private _selfSize: cc.Size 				= null;
	private _selfID: number 				= null;
	private _targetCarID: number 			= null;
	private _delayTime: number 				= null;
	private _mapNode: cc.Node 				= null;
	private _randomNum: number				= null;
	private _isBeSilly: boolean 			= null;
	private _beSillyValue: number 			= null;
	private _radius: number 				= null;
	private _speed: number 					= null;

	public constructor(){
		this._targetLocation = cc.v2(0, 0);
		this._selfLocation = cc.v2(0, 0);
		this._selfSize = cc.size(0, 0);
		this._selfID = 0;
		this._targetCarID = 0;
		this._delayTime = 0;
		this._mapNode = null;
		this._randomNum = 0;
		this._isBeSilly = false;
		this._beSillyValue = 0;		//犯傻几率
		this._radius = 0;		//识别半径
		this._speed = 0;
	};

	public setTargetLocation(tpoint: cc.Vec2)
	{
		if(tpoint){
			this._targetLocation.x = tpoint.x;
			this._targetLocation.y = tpoint.y;
		}
	};

	public setSelfLocation(spoint: cc.Vec2)
	{
		if(spoint){
			this._selfLocation.x = spoint.x;
			this._selfLocation.y = spoint.y;
		}
	};

	public setSelfSize(size)
	{
		if(size){
			this._selfSize.width = size.width;
			this._selfSize.height = size.height;
		}
	};

	public setSelfID(selfID: number)
	{
		this._selfID = selfID;
	};

	public setTargetCarID(carID: number)
	{
		this._targetCarID = carID;
	};

	public setDelayTime(dt: number)
	{
		this._delayTime = dt;
	};

	public setMapNode(node: cc.Node)
	{
		this._mapNode = node;
	};

	public setRandomNum(random: number)
	{
		this._randomNum = random;
	};

	public setIsBeSilly(value)
	{
		this._isBeSilly = value;
	};

	public setSillyValue(value)
	{
		this._beSillyValue = value;
	};

	public setRadius(radius: number)
	{
		this._radius = radius;
	};

	public setSpeed(speed: number)
	{
		this._speed = speed;
	};

	public getSpeed(): number
	{
		return this._speed;
	};

	public getSelfLocation()
	{
		return this._selfLocation;
	};

	public getSelfSize()
	{
		return this._selfSize;
	};

	public getTargetLocation(): cc.Vec2
	{
		return this._targetLocation;
	};

	public getDelayTime(): number
	{
		return this._delayTime;
	};

	public getMapNode(): cc.Node
	{
		return this._mapNode;
	};

	public getRandomNum(): number
	{
		return this._randomNum;
	};

	public getIsBeSilly()
	{
		return this._isBeSilly;
	};

	public getSillyValue(): number
	{
		return this._beSillyValue;
	};

	public getRadius(): number
	{
		return this._radius;
	};

	public getSelfID(): number
	{
		return this._selfID;
	}

	public destroy()
	{
		this._targetLocation = null;
		this._selfLocation = null;
		this._selfSize = null;
		this._mapNode = null;
	}
};