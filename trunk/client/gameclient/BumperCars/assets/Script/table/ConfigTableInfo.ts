import BaseTableInfo from './BaseTableInfo';
import BaseTableReader from './BaseTableReader';
import GameConfig from '../GameConfig';
export default class ConfigTableInfo extends BaseTableInfo
{
    constructor()
    {
        super(0);
    }
}

export class ConfigTableReader extends BaseTableReader
{
    public constructor(filename: string)
    {
        super(filename);
    }

    public ReadOneData(id: number): BaseTableInfo
    {
        GameConfig.Init(this.m_pReader.GetData())

        return null;
    }
}