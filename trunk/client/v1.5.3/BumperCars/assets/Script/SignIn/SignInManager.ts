import { SignInDayData } from './SignInDayData';
import BoxData from './BoxData';

export default class SignInManager
{
    private static instance: SignInManager = null;

    //一周的签到数据
    private m_SignInDayDataArr: Array<SignInDayData> = null;

    //宝箱的签到数据
    private m_BoxDayDataArr: Array<BoxData> = null;

    constructor()
    {
        this.m_SignInDayDataArr = [];
        this.m_BoxDayDataArr = [];
    }

    public static GetInstance(): SignInManager
    {
        if(!this.instance)
        {
            this.instance = new SignInManager;
        }
        return this.instance;
    }

    //初始化签到数据
    public Init()
    {

    }

    public AddSignInData(data: SignInDayData): void
    {
        this.m_SignInDayDataArr.push(data);
    }

    public AddBoxData(data: BoxData): void
    {
        this.m_BoxDayDataArr.push(data);
    }

    //获取签到的周期天数
    public GetSignInDayNum(): number
    {
        return this.m_SignInDayDataArr.length;
    }

    //获取宝箱数量
    public GetBoxNum(): number
    {
        return this.m_BoxDayDataArr.length;
    }

    //根据索引获取某一天的签到数据
    public GetSiginDataByIndex(index: number): SignInDayData
    {
        if(this.m_SignInDayDataArr[index])
        {
            return this.m_SignInDayDataArr[index];
        }
        return null;
    }

    //根据id获得签到数据
    public GetSignInDataByID(id: number): SignInDayData
    {
        for(let i = 0, len = this.m_SignInDayDataArr.length; i < len; i ++)
        {
            if(this.m_SignInDayDataArr[i].GetDayID() === id)
            {
                return this.m_SignInDayDataArr[i];
            }
        }
        return null;
    }

    public GetBoxDataByIndex(index: number): BoxData
    {
        if(this.m_BoxDayDataArr[index])
        {
            return this.m_BoxDayDataArr[index];
        }
        return null;
    }

    //根据id获得宝箱数据
    public GetBoxDataByID(id: number): BoxData
    {
        for(let i = 0, len = this.m_BoxDayDataArr.length; i < len; i ++)
        {
            if(this.m_BoxDayDataArr[i].GetBoxID() === id)
            {
                return this.m_BoxDayDataArr[i];
            }
        }
        return null;
    }

    public Destroy()
    {
        this.m_BoxDayDataArr.length = 0;
        this.m_SignInDayDataArr.length = 0;

        this.m_BoxDayDataArr = null;
        this.m_SignInDayDataArr = null;
    }
    
    public static Destroy()
    {
        if(this.instance)
        {
            this.instance.Destroy();
            this.instance = null;
        }
    }
}