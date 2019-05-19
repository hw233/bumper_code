import NumberUtil from './NumberUtil';

export default class Vector 
{
    public x: number = null;
    public y: number = null;

    private static _pool: Array<Vector> = null;

    private constructor(x: number = 0, y: number = 0)
    {
        this.x = x;
        this.y = y;
    }

    public init()
    {
        this.x = 0;
        this.y = 0;
    }

    public div(num: number)
    {
        NumberUtil.NumberDiv(this.x, num);
        NumberUtil.NumberDiv(this.y, num);
    }

    public mul(num: number)
    {
        NumberUtil.NumberMul(this.x, num);
        NumberUtil.NumberMul(this.y, num);
    }

    public add(num: number)
    {
        NumberUtil.NumberAdd(this.x, num);
        NumberUtil.NumberAdd(this.y, num);
    }

    public sub(num: number)
    {
        NumberUtil.NumberSub(this.x, num);
        NumberUtil.NumberSub(this.y, num);
    }

    public static MulNumber(vec :Vector, num: number): Vector
    {
        let out: Vector = Vector.GetVector();

        out.x = NumberUtil.NumberMul(vec.x, num);
        out.y = NumberUtil.NumberMul(vec.y, num);
        
        return out;
    }

    public static DivNumber(vec: Vector, num: number): Vector
    {
        let out: Vector = Vector.GetVector();

        out.x = NumberUtil.NumberDiv(vec.x, num);
        out.y = NumberUtil.NumberDiv(vec.y, num);
        
        return out;
    }

    public static AddNumber(vec: Vector, num: number): Vector
    {
        let out: Vector = Vector.GetVector();

        out.x = NumberUtil.NumberAdd(vec.x, num);
        out.y = NumberUtil.NumberAdd(vec.y, num);
        
        return out;
    }

    public static SubNumber(vec: Vector, num: number): Vector
    {
        let out: Vector = Vector.GetVector();

        out.x = NumberUtil.NumberSub(vec.x, num);
        out.y = NumberUtil.NumberSub(vec.y, num);
        
        return out;
    }

    public static Init()
    {
        this._pool = [];
    }

    public static Destroy()
    {
        this._pool.length = 0;
        this._pool = null;
    }

    public static GetVector()
    {
        let last = this._pool.length - 1;
        if (last < 0) 
        {
            return new Vector();
        }
        else 
        {
            // Pop the last object in pool
            let data = this._pool[last];
            this._pool.length = last;

            data.init();

            return data;
        }
    }

    public static PutVector(data: Vector)
    {
        if(this._pool)
        {
            this._pool.push(data);
        }
    }
}