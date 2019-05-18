export default class PlayerOpData{
    
    private playerID: number            = null;     //玩家ID
    private cmd: number                 = null;     //操作命令
    private custom: any                 = null;     //保留
    private nullOp: boolean             = null;     //空操作
    private args: Array<any>            = null;     //参数
    private ts: number                  = null;     //时间戳

    private static _pool: Array<PlayerOpData> = null;

    private static initCount: number = 30;

    private static CtorCount        = 0;
    
    private constructor()
    {
        this.init();

        PlayerOpData.CtorCount ++;
    }

    public static Init()
    {
        this._pool = [];

        for(let i = 0; i < this.initCount; i ++)
        {
            this._pool.push( new PlayerOpData() );
        }
    }

    public static Get()
    {
        let last = this._pool.length - 1;
        if (last < 0) 
        {
            return new PlayerOpData();
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

    public static Put(data: PlayerOpData)
    {
        if(this._pool)
        {
            this._pool.push(data);
        }
    }

    public static Destroy()
    {
        if(this._pool)
        {
            for(let key in this._pool)
            {
                this._pool[key].destroy();
            }
            this._pool.length = 0;
            this._pool = null;
        }

        PlayerOpData.CtorCount = 0;
    }

    public init()
    {
        this.cmd = 0;
        this.custom = null;
        this.playerID = 0;
        this.nullOp = false;
        this.args = [];
        this.ts = 0;
    }

    public setPlayerID(playerID: number): void
    {
        this.playerID = playerID;
    }

    public getPlayerID(): number
    {
        return this.playerID;
    }

    public setCMD(cmd: number): void
    {
        this.cmd = cmd;
    }

    public getCMD(): number
    {
        return this.cmd;
    }

    public setCustom(custom: any): void
    {
        this.custom = custom;
    }

    public getCustom(): any
    {
        return this.custom;
    }

    public setTimeStamp(ts: number): void
    {
        this.ts = ts;
    }

    public getTimeStamp(): number
    {
        return this.ts;
    }

    public addArg(arg: any): void
    {
        this.args.push(arg);
    }

    public getArg(index: number): any
    {
        if(index >=0 && index < this.args.length)
        {
            return this.args[index];
        }
        return null;
    }

    public copyArgs(args: Array<any>)
    {
        if(args)
        {
            for(let i = 0, len = args.length; i < len; i ++)
            {
                this.args.push(args[i]);
            }
        }
    }

    public getArgs()
    {
        return this.args;
    }

    public setNullOp(value: boolean): void
    {
        this.nullOp = value;
    }

    public isNullOp(): boolean
    {
        return this.nullOp;
    }

    public destroy(): void
    {
        this.custom = null;
        this.args.length = 0;
        this.args = null;
    }
}