import GameLog from '../Log/LogTool';
export default class NumberUtil
{
    public static NumberAdd(num1, num2) 
    {
        if(num1 === undefined || num1 === null || num2 === undefined || num2 === null )
        {
            GameLog.PrintError("NumberUtil:NumberAdd:num1 or num2 = null");
            return 0;
        }
        num1 = this.DoubleFiexd(num1);
        num2 = this.DoubleFiexd(num2);

        return this.DoubleFiexd(num1 + num2);
    }

    public static NumberSub(num1, num2) 
    {
        if(num1 === undefined || num1 === null || num2 === undefined || num2 === null )
        {
            GameLog.PrintError("NumberUtil:NumberSub:num1 or num2 = null");
            return 0;
        }
        return this.NumberAdd(num1, -num2)
    }

    public static NumberDiv(num1, num2) 
    {
        if(num1 === undefined || num1 === null || num2 === undefined || num2 === null )
        {
            GameLog.PrintError("NumberUtil:NumberDiv:num1 or num2 = null");
            return 0;
        }
        num1 = this.DoubleFiexd(num1);
        num2 = this.DoubleFiexd(num2);

        return this.DoubleFiexd(num1 / num2);
    }

    public static NumberMul(num1, num2) 
    {
        if(num1 === undefined || num1 === null || num2 === undefined || num2 === null )
        {
            GameLog.PrintError("NumberUtil:NumberMul:num1 or num2 = null");
            return 0;
        }
        num1 = this.DoubleFiexd(num1);
        num2 = this.DoubleFiexd(num2);

        return this.DoubleFiexd(num1 * num2);
    }

    // public static NumberAdd(num1, num2) 
    // {
    //     num1 = Number(num1);
    //     num2 = Number(num2);
    //     var dec1, dec2, times;
    //     try { dec1 = this.CountDecimals(num1)+1; } catch (e) { dec1 = 0; }
    //     try { dec2 = this.CountDecimals(num2)+1; } catch (e) { dec2 = 0; }
    //     times = Math.pow(10, Math.max(dec1, dec2));
    //     // var result = (num1 * times + num2 * times) / times;
    //     var result = (this.NumberMul(num1, times) + this.NumberMul(num2, times)) / times;
    //     return this.GetCorrectResult("add", num1, num2, result);
    //     // return result;
    // }
     
    // public static NumberSub(num1, num2) 
    // {
    //     num1 = Number(num1);
    //     num2 = Number(num2);
    //     var dec1, dec2, times;
    //     try { dec1 = this.CountDecimals(num1)+1; } catch (e) { dec1 = 0; }
    //     try { dec2 = this.CountDecimals(num2)+1; } catch (e) { dec2 = 0; }
    //     times = Math.pow(10, Math.max(dec1, dec2));
    //     // var result = Number(((num1 * times - num2 * times) / times);
    //     var result = Number((this.NumberMul(num1, times) - this.NumberMul(num2, times)) / times);
    //     return this.GetCorrectResult("sub", num1, num2, result);
    //     // return result;
    // }
     
    // public static NumberDiv(num1, num2) 
    // {
    //     num1 = Number(num1);
    //     num2 = Number(num2);
    //     var t1 = 0, t2 = 0, dec1, dec2;
    //     try { t1 = this.CountDecimals(num1); } catch (e) { }
    //     try { t2 = this.CountDecimals(num2); } catch (e) { }
    //     dec1 = this.ConvertToInt(num1);
    //     dec2 = this.ConvertToInt(num2);
    //     var result = this.NumberMul((dec1 / dec2), Math.pow(10, t2 - t1));
    //     return this.GetCorrectResult("div", num1, num2, result);
    //     // return result;
    // }
    
    // public static NumberMul(num1, num2){
    //     num1 = Number(num1);
    //     num2 = Number(num2);
    //     var times = 0, s1 = num1.toString(), s2 = num2.toString();
    //     try { times += this.CountDecimals(s1); } catch (e) { }
    //     try { times += this.CountDecimals(s2); } catch (e) { }
    //     var result = this.ConvertToInt(s1) * this.ConvertToInt(s2) / Math.pow(10, times);
    //     return this.GetCorrectResult("mul", num1, num2, result);
    //     // return result;
    // }

    // public static CountDecimals(num) 
    // {
    //     var len = 0;
    //     try 
    //     {
    //         num = Number(num);
    //         var str = num.toString().toUpperCase();
    //         if (str.split('E').length === 2) 
    //         { // scientific notation
    //             var isDecimal = false;
    //             if (str.split('.').length === 2) 
    //             {
    //                 str = str.split('.')[1];
    //                 if (parseInt(str.split('E')[0]) !== 0) 
    //                 {
    //                     isDecimal = true;
    //                 }
    //             }
    //             let x = str.split('E');
    //             if (isDecimal) 
    //             {
    //                 len = x[0].length;
    //             }
    //             len -= parseInt(x[1]);
    //         } else if (str.split('.').length === 2) 
    //         { // decimal
    //             if (parseInt(str.split('.')[1]) !== 0) 
    //             {
    //                 len = str.split('.')[1].length;
    //             }
    //         }
    //     } catch(e) 
    //     {
    //         throw e;
    //     } finally 
    //     {
    //         if (isNaN(len) || len < 0) 
    //         {
    //             len = 0;
    //         }
    //         return len;
    //     }
    // }

    // private static ConvertToInt(num) {
    //     num = Number(num);
    //     var newNum = num;
    //     var times = this.CountDecimals(num);
    //     var temp_num = num.toString().toUpperCase();
    //     if (temp_num.split('E').length === 2) 
    //     {
    //         newNum = Math.round(num * Math.pow(10, times));
    //     } else 
    //     {
    //         newNum = Number(temp_num.replace(".", ""));
    //     }
    //     return newNum;
    // }

    // private static GetCorrectResult(type, num1, num2, result) {
    //     var temp_result = 0;
    //     switch (type) 
    //     {
    //         case "add":
    //             temp_result = num1 + num2;
    //             break;
    //         case "sub":
    //             temp_result = num1 - num2;
    //             break;
    //         case "div":
    //             temp_result = num1 / num2;
    //             break;
    //         case "mul":
    //             temp_result = num1 * num2;
    //             break;
    //     }
    //     if (Math.abs(result - temp_result) > 1) {
    //         return temp_result;
    //     }
    //     return result;
    // }

    private static DoubleFiexd(dValue: number): number
    {
        return Number(dValue.toFixed(4));
    }

    public static DoubleToInt(dValue: number)
    {
        return Math.round(dValue);
    }
}