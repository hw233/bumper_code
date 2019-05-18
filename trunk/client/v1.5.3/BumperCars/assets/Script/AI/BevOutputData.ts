export default class BevOutputData{

	protected _rotate: number 	= null;
	protected _linearVelocity: cc.Vec2 	= null;

	public constructor()
	{
		this._rotate = 0;
		this._linearVelocity = cc.v2(0, 0);
	}

	public setRotate(rotate: number)
	{
		this._rotate = rotate;
	}

	public setLinearVelocity(linearVelocity: cc.Vec2)
	{
		this._linearVelocity.x = linearVelocity.x;
		this._linearVelocity.y = linearVelocity.y;
	}

	public getRotate(): number
	{
		return this._rotate;
	}

	public getLinearVelocity(): cc.Vec2
	{
		return this._linearVelocity;
	}

	public destroy()
	{
		this._linearVelocity = null;
	}
};
