import BaseTableInfo from './BaseTableInfo';
import BaseTableReader from './BaseTableReader';

export default class CarTableInfo extends BaseTableInfo
{
    private m_Name: string      = null;

    //车图片ID
    private m_CarImgID: number         = null;

    //解锁条件
    private m_nConditionID: number      = null;

    //解锁状态
    private m_nConditionState: number   = null;

    private m_nValue: number            = null;

    //速度
    private m_nSpeed: number            = null;

    //力量
    private m_nPower: number            = null;

    public constructor(
        carID: number, 
        name: string,
        imgID: number,
        conID: number,
        conState: number,
        value: number,
        speed: number,
        power: number)
    {
        super(carID);

        this.m_Name = name;
        this.m_CarImgID = imgID;
        this.m_nConditionID = conID;
        this.m_nConditionState = conState;
        this.m_nValue = value;
        this.m_nSpeed = speed;
        this.m_nPower = power;
    }

    public GetName(): string
    {
        return this.m_Name;
    }

    public GetConditionID(): number
    {
        return this.m_nConditionID;
    }

    public GetConditionState(): number
    {
        return this.m_nConditionState;
    }

    public GetValue(): number
    {
        return this.m_nValue;
    }

    public GetSpeed(): number
    {
        return this.m_nSpeed;
    }

    public GetPower(): number
    {
        return this.m_nPower;
    }

    public GetImgID(): number
    {
        return this.m_CarImgID;
    }
}

export class CarTableReader extends BaseTableReader
{
    public constructor(filename: string)
    {
        super(filename);
    }

    public ReadOneData(id: number): BaseTableInfo
    {
        this.SetID(id);
        
        return new CarTableInfo(
            this.GetNumber("ID"),
            this.GetString("name"),
            this.GetNumber("ImgID"),
            this.GetNumber("ConditionID"),
            this.GetNumber("ConditionState"),
            this.GetNumber("Money"),
            this.GetNumber("speed"),
            this.GetNumber("power"),
        );
    }
}