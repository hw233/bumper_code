import BaseTableInfo from './BaseTableInfo';
import BaseTableReader from './BaseTableReader';

export default class AITableInfo extends BaseTableInfo
{
    //识别半径
    private m_nAiradius: number     = null;

    //犯傻几率
    private m_nAis: number          = null;

    public constructor(id: number, airadius: number, ais: number)
    {
        super(id);

        this.m_nAiradius = airadius;
        this.m_nAis = ais;
    }

    public GetAiradius(): number
    {
        return this.m_nAiradius;
    }

    public GetAis(): number
    {
        return this.m_nAis;
    }
}

export class AITableReader extends BaseTableReader
{
    public constructor(filename: string)
    {
        super(filename);
    }

    public ReadOneData(id: number): BaseTableInfo
    {
        this.SetID(id);

        return new AITableInfo(
            this.GetNumber("ID"),
            this.GetNumber("Airadius"),
            this.GetNumber("Ais"),
        );
    }
}