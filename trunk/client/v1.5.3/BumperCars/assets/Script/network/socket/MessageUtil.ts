import NumberUtil from '../../common/NumberUtil';

//用于处理网络数据的工具类
export default class MessageUtil{

    public static ConvertToFloat(value): number
    {
        let fValue: number = NumberUtil.NumberDiv(value, 10000);
        return fValue;
    }

    public static ConvertToInt(value): number
    {
        let fValue = NumberUtil.NumberMul(value, 10000);
        let nValue = NumberUtil.DoubleToInt(fValue);

        return nValue;
    }
};