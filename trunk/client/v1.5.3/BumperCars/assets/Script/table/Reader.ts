export default class Reader{

    // private static TablePath: string    = CC_WECHATGAME ? "https://img.51weiwan.com/pengpengche/v1.5.1/table/" : "table/";
    private static TablePath: string    = "table/";

    private m_strFileName: string       = null;
    private m_Data: any                 = null;
    
    constructor(filename: string)
    {
        this.m_strFileName = filename;
    }

    public Init()
    {
        let fullPath: string = Reader.TablePath + this.m_strFileName;

        // if(CC_WECHATGAME)
        // {
        //     fullPath += ".json";
        // }

        this.m_Data = cc.loader.getRes(fullPath);
        if(this.m_Data && this.m_Data.json)
        {
            this.m_Data = this.m_Data.json;
        }
    }

    public GetString(row: number, key: string): string
    {
        if(this.m_Data)
        {
            let value: any = this.m_Data[row][key];
            return String(value);
        }
        return "";
    }

    public GetNumber(row: number, key: string): number
    {
        if(this.m_Data)
        {
            let value: any = this.m_Data[row][key];
            return Number(value);
        }
        return 0;
    }

    public GetBoolean(row: number, key: string): boolean
    {
        if(this.m_Data)
        {
            let value: any = this.GetNumber(row, key);
            return value ? true : false;
        }
        return false;
    }

    public GetDataCount(): number
    {
        if(this.m_Data)
        {
            return this.m_Data.length;
        }
        return 0;
    }   

    public GetData(): any
    {
        return this.m_Data;
    }
}