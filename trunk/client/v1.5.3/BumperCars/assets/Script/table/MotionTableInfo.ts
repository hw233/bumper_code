import BaseTableInfo from './BaseTableInfo';
import BaseTableReader from './BaseTableReader';
import Util from '../common/Util';

export default class MotionTableInfo extends BaseTableInfo
{
    private m_Name: string      = null;

    //解锁条件
    private m_nConditionID: number          = null;

    //解锁状态  
    private m_nConditionState: number       = null;

    private m_nValue: number                = null;

    //速度  
    private m_nSpeed: number                = null;

    //力量  
    private m_nPower: number                = null;

    //尾焰图片ID
    private m_nImgID: number                = null;

    //拖尾类型(1：这样的 2：那样的)
    private m_nType: number                 = null;

    //拖尾数量
    private m_nNum: number                  = null;

    //生成方式
    private m_nGenerateModel: number        = null;

    //生成时间  
    private m_nGenerateTime: number         = null;

    //x偏移 
    private m_nXOffect: Array<number>       = null;

    //y偏移 
    private m_nYOffect: Array<number>       = null;

    //初始缩放值    
    private m_nInitScale: number            = null;

    //最终缩放值    
    private m_nFinalScale: number           = null;

    //生存时间  
    private m_nLiveTime: number             = null;

    //初始透明度    
    private m_nInitTransparency: number     = null;

    //最终透明度
    private m_nFinalTransparency: number    = null;

    public constructor(
        motionID: number, 
        name: string,
        conID: number,
        conState: number,
        value: number,
        speed: number,
        power: number,
        imgID: number,
        type: number,
        num: number,
        generateMode: number,
        generateTiem: number,
        xOffect: string,
        yOffect: string,
        initScale: number,
        finalScale: number,
        liveTime: number,
        initTrans: number,
        finalTrans: number)
    {
        super(motionID);

        this.m_Name                 = name;
        this.m_nConditionID         = conID;
        this.m_nConditionState      = conState;
        this.m_nValue               = value;
        this.m_nSpeed               = 0;
        this.m_nPower               = 0;
        this.m_nImgID               = imgID;
        this.m_nType                = type;
        this.m_nNum                 = num;
        this.m_nGenerateModel       = generateMode;
        this.m_nGenerateTime        = generateTiem;
        this.m_nXOffect             = Util.StringConvertToArray(xOffect);
        this.m_nYOffect             = Util.StringConvertToArray(yOffect);
        this.m_nInitScale           = initScale;
        this.m_nFinalScale          = finalScale;
        this.m_nLiveTime            = liveTime;
        this.m_nInitTransparency    = initTrans;
        this.m_nFinalTransparency   = finalTrans;
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
        return this.m_nImgID;
    }

    public GetType(): number
    {
        return this.m_nType;
    }

    public GetNum(): number
    {
        return this.m_nNum;
    }

    public GetGenerateModel(): number
    {
        return this.m_nGenerateModel;
    }

    public GetGenerateTime(): number
    {
        return this.m_nGenerateTime;
    }

    public GetXOffect(): Array<number>
    {
        return this.m_nXOffect;
    }

    public GetYOffect(): Array<number>
    {
        return this.m_nYOffect;
    }

    public GetInitScale(): number
    {
        return this.m_nInitScale;
    }

    public GetFinalScale(): number
    {
        return this.m_nFinalScale;
    }

    public GetLiveTime(): number
    {
        return this.m_nLiveTime;
    }

    public GetInitTransparency(): number
    {
        return this.m_nInitTransparency;
    }

    public GetFinalTransparency(): number
    {
        return this.m_nFinalTransparency;
    }
}

export class MotionTableReader extends BaseTableReader
{
    public constructor(filename: string)
    {
        super(filename);
    }

    public ReadOneData(id: number): BaseTableInfo
    {
        this.SetID(id);
        
        return new MotionTableInfo(
            this.GetNumber("ID"),
            this.GetString("Name"),
            this.GetNumber("ConditionID"),
            this.GetNumber("ConditionState"),
            this.GetNumber("Money"),
            this.GetNumber("Speed"),
            this.GetNumber("Power"),
            this.GetNumber("ImgID"),
            this.GetNumber("Type"),
            this.GetNumber("Num"),
            this.GetNumber("Model"),
            this.GetNumber("time"),
            this.GetString("XOffect"),
            this.GetString("YOffect"),
            this.GetNumber("InitScale"),
            this.GetNumber("FinalScale"),
            this.GetNumber("LiveTime"),
            this.GetNumber("InitTransparency"),
            this.GetNumber("FinalTransparency"),
        );
    }
}