import BaseTableInfo from './BaseTableInfo';
import BaseTableReader from './BaseTableReader';

export default class MiniGameTableInfo extends BaseTableInfo
{
    private m_Name: string      = null;

    private m_Path: string      = null;

    private m_AppID: string     = null;

    private m_ImgUrl: string     = null;

    public constructor(ID: number, name: string, path: string, appID: string, imgUrl: string)
    {
        super(ID);

        this.m_Name = name;
        this.m_Path = path;
        this.m_AppID = appID;
        this.m_ImgUrl = imgUrl;
    }

    public GetName(): string
    {
        return this.m_Name;
    }
    
    public GetPath(): string
    {
        return this.m_Path;
    }

    public GetAppID(): string
    {
        return this.m_AppID;
    }

    public GetImgUrl(): string
    {
        return this.m_ImgUrl;
    }
}

export class MiniGameTableReader extends BaseTableReader
{
    public constructor(filename: string)
    {
        super(filename);
    }

    public ReadOneData(id: number): BaseTableInfo
    {
        this.SetID(id);
        
        return new MiniGameTableInfo(
            this.GetNumber("game_id"),
            this.GetString("name"),
            this.GetString("path"),
            this.GetString("app_id"),
            this.GetString("img_url"),
        );
    }
}