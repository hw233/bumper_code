import GameLog from '../Log/LogTool';

export class FSPFrame
{
    private m_FrameID: number          = null;

    private m_PlayerID: number         = null;

    private m_isDeleted: boolean       = null;

    private m_Cmd: number              = null;

    private m_Args: Array<any>         = null;

    private m_Custom: any              = null;

    private m_IsNull: boolean          = null;

    private m_IsQuickRun: boolean      = null;

    private m_TimeStamp: number        = null;

    private m_IsCanRun: boolean        = null;  //补帧需要下一帧才可以执行

    public static CtorCount        = 0;

    constructor()
    {
        this.init();

        FSPFrame.CtorCount ++;
        // cc.log("FSPFrame.CtorCount:", FSPFrame.CtorCount);
    }

    public init()
    {
        if(this.m_Args)
        {
            this.m_Args.length = 0;
        }

        this.m_FrameID = 0;
        this.m_isDeleted = false;
        this.m_PlayerID = 0;
        this.m_Cmd = 0;
        this.m_Args = [];
        this.m_IsNull = false;
        this.m_IsQuickRun = false;
        this.m_TimeStamp = 0;
        this.m_IsCanRun = true;
    }

    public setFrameID(m_FrameID: number)
    {
        this.m_FrameID = m_FrameID;
    }

    public getFrameID(): number
    {
        return this.m_FrameID;
    }

    public setPlayerID(playerID: number): void
    {
        this.m_PlayerID = playerID;
    }

    public getPlayerID(): number
    {
        return this.m_PlayerID;
    }

    public setCmd(cmd: number): void
    {
        this.m_Cmd = cmd;
    }

    public getCmd(): number
    {
        return this.m_Cmd;
    }

    public setIsCanRun(isCanRun: boolean): void
    {
        this.m_IsCanRun = isCanRun;
    }

    public getIsCanRun(): boolean
    {
        return this.m_IsCanRun;
    }

    public setTimeStamp(ts: number): void
    {
        this.m_TimeStamp = ts;
    }

    public getTimeStamp(): number
    {
        return this.m_TimeStamp;
    }

    public addArg(arg: any): void
    {
        this.m_Args.push(arg);
    }

    public getArg(index: number): any
    {
        if(index >=0 && index < this.m_Args.length)
        {
            return this.m_Args[index];
        }
        return null;
    }

    public copoyArgs(args: Array<any>): void
    {
        if(args)
        {
            for(let i = 0, len = args.length; i < len; i ++)
            {
                this.m_Args.push(args[i]);
            }
        }
    }

    public getArgs(): Array<any>
    {
        return this.m_Args;
    }

    public hasArg(): boolean
    {
        return this.m_Args.length >= 2;
    }

    public setIsNull(isnull: boolean): void
    {
        this.m_IsNull = isnull;
    }

    public getIsNull(): boolean
    {
        return this.m_IsNull;
    }

    public setCustom(custom: any): void
    {
        this.m_Custom = custom;
    }

    public getCustom(): any
    {
        return this.m_Custom;
    }

    public setIsDeleted(isdeleted: boolean): void
    {
        this.m_isDeleted = isdeleted;
    }

    public getIsDeleted(): boolean
    {
        return this.m_isDeleted;
    }

    public setIsQuickRun(isQuickRun: boolean): void
    {
        this.m_IsQuickRun = isQuickRun;
    }

    public getIsQuickRun(): boolean
    {
        return this.m_IsQuickRun;
    }

    public destroy()
    {
        this.m_Args.length = 0;
        this.m_Args = null;
    }

    public Print()
    {
        let str: string = "";

        str += "FSPManager: 接受: {";
        str += "FrameID=";
        str += this.m_FrameID;
        str += "-";
        str += "PlayerID=";
        str += this.m_PlayerID;
        str += "-";
        str += "CMD=";
        str += this.m_Cmd;
        str += "-";
        str += "[";

        if(this.m_Args.length > 0)
        {
            for(let i: number = 0; i < this.m_Args.length; i ++)
            {
                str += this.m_Args[i];
                str += "-";
            }
        }

        str += "]";
        str += "}";

        GameLog.PrintDebug(str);
    }
}