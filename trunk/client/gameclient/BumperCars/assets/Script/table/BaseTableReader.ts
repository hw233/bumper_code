import Reader from './Reader';
import BaseTableInfo from './BaseTableInfo';

export default class BaseTableReader
{
    protected m_pReader: Reader       = null;
    protected m_nID: number           = null;

    public constructor(filename: string)
    {
        this.m_pReader = new Reader(filename);
        this.m_pReader.Init();
    }

    public SetID(id: number)
    {
        this.m_nID = id;
    }

    public GetString(key: string): string
    {
        if(this.m_pReader)
        {
            return this.m_pReader.GetString(this.m_nID, key);
        }
        return "";
    }

    public GetNumber(key: string): number
    {
        if(this.m_pReader)
        {
            return this.m_pReader.GetNumber(this.m_nID, key);
        }
        return 0;
    }

    public GetBoolean(key: string): boolean
    {
        if(this.m_pReader)
        {
            return this.m_pReader.GetBoolean(this.m_nID, key);
        }
        return false;
    }

    public GetDataCount(): number
    {
        if(this.m_pReader)
        {
            return this.m_pReader.GetDataCount();
        }
        return 0;
    }

    //需要派生类重写
    public ReadOneData(id): BaseTableInfo
    {
        return null;
    }
}