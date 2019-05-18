export default class BaseTableInfo
{
    private m_nID: number   = null;

    constructor(id: number)
    {
        this.m_nID = id;
    }

    public SetID(id: number)
    {
        this.m_nID = id;
    }

    public GetID(): number
    {
        return this.m_nID;
    }
}