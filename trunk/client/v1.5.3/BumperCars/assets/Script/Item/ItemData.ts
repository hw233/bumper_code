export default class ItemData
{
    //道具唯一ID
    private     m_nSoleID: number       = null; 

    //道具ID
    private     m_nItemID: number       = null; 

    //道具类型
    private     m_nItemType: number     = null; 

    //道具有效时长
    private     m_nLiftTime: number     = null;

    //道具叠加数量
    private     m_nAmount: number       = null;

    public constructor()
    {
        this.m_nSoleID      = 0;
        this.m_nItemID      = 0;
        this.m_nItemType    = 0;
        this.m_nAmount      = 0;
        this.m_nLiftTime    = 0;
    }

    public SetSoleID(value: number): void
    {
        this.m_nSoleID = value;
    }

    public GetSoleID(): number
    {
        return this.m_nSoleID;
    }

    public SetItemID(value: number): void
    {
        this.m_nItemID = value;
    }

    public GetItemID(): number
    {
        return this.m_nItemID;
    }

    public SetItemType(value: number): void
    {
        this.m_nItemType = value;        
    }

    public GetItemType(): number
    {
        return this.m_nItemType;
    }

    public SetAmount(value: number): void
    {
        this.m_nAmount = value;
    }

    public GetAmount(): number
    {
        return this.m_nAmount;
    }

    public SetLiftTime(value: number): void
    {
        this.m_nLiftTime = value;
    }

    public GetLiftTime(): number
    {
        return this.m_nLiftTime;
    }

}