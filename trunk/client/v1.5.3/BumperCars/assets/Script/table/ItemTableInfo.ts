import BaseTableInfo from './BaseTableInfo';
import BaseTableReader from './BaseTableReader';

export default class ItemTableInfo extends BaseTableInfo
{
    private m_Name: string      = null;

    private m_ImgID: number     = null;

    public constructor(itemID: number, name: string, imgID: number)
    {
        super(itemID);

        this.m_Name = name;
        this.m_ImgID = imgID;
    }

    public GetItemName(): string
    {
        return this.m_Name;
    }
    

    public GetImgID(): number
    {
        return this.m_ImgID;
    }
}

export class ItemTableReader extends BaseTableReader
{
    public constructor(filename: string)
    {
        super(filename);
    }

    public ReadOneData(id: number): BaseTableInfo
    {
        this.SetID(id);
        
        return new ItemTableInfo(
            this.GetNumber("ID"),
            this.GetString("Name"),
            this.GetNumber("ImgID")
        );
    }
}