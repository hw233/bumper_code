import BaseTableInfo from './BaseTableInfo';
import BaseTableReader from './BaseTableReader';

export default class OtherGamesTableInfo extends BaseTableInfo
{
    //icon图片
    private m_nIconImg: string             = null;

    //游戏名字
    private m_GameName: string             = null;

    //落地页图片
    private m_GameImg: string              = null;

    public constructor(gameID: number, name: string, iconImg: string, gameImg: string)
    {
        super(gameID);

        this.m_GameName = name;
        this.m_nIconImg = iconImg;
        this.m_GameImg = gameImg;
    }

    public GetIconImg(): string
    {
        return this.m_nIconImg;
    }

    public GetGameImg(): string
    {
        return this.m_GameImg;
    }

    public GetGameName(): string
    {
        return this.m_GameName;
    }
}

export class OtherGamesTableReader extends BaseTableReader
{
    public constructor(filename: string)
    {
        super(filename);
    }

    public ReadOneData(id: number): BaseTableInfo
    {
        this.SetID(id);

        return new OtherGamesTableInfo(
            this.GetNumber("game_id"),
            this.GetString("name"),
            this.GetString("iconfile"),
            this.GetString("imgfile"),
        );
    }
}