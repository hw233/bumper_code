import GameLog from "../Log/LogTool";

export default class Util{

    //查找数组里是否包含某个元素
    public static IsContainElement(arr: Array<any>, item: any): boolean{
        if(arr){
            for(let i:number = 0;i < arr.length; i++){
                if(arr[i] == item){
                    return true;
                }
            }
        }
        return false;
    }

    //深拷贝数组
    public static CopyArray(arr: Array<any>, arr2: Array<any> = null): Array<any>{
        return arr.concat();
    }

    public static CloneObj(obj: any){
        let str, newobj = obj.constructor === Array ? [] : {};
        if(typeof obj !== 'object'){
            return;
        } else if(JSON){
            str = JSON.stringify(obj), //序列化对象
            newobj = JSON.parse(str); //还原
        } else {
            for(let i in obj){
                newobj[i] = typeof obj[i] === 'object' ? this.CloneObj(obj[i]) : obj[i]; 
            }
        }
        return newobj;
    }

    //是否为数字 包括字符串数字
    public static IsNumber( val:any ): boolean{

        let regPos = /^\d+(\.\d+)?$/; //非负浮点数
        let regNeg = /^(-(([0-9]+\.[0-9]*[1-9][0-9]*)|([0-9]*[1-9][0-9]*\.[0-9]+)|([0-9]*[1-9][0-9]*)))$/; //负浮点数
        if(regPos.test(val) || regNeg.test(val)){
            return true;
        }else{
            return false;
        }
    }

    //根据传入的最大字符个数截取字符串
    public static GetByteVal(val: string, max: number) :string{
        let returnValue:string = '';
        let byteValLen:number = 0;

        if(Util.IsString(val))
        {
            for (let i = 0; i < val.length; i++) {
                if (val[i].match(/[^\x00-\xff]/ig) != null)
                    byteValLen += 2;
                else
                    byteValLen += 1;
                if (byteValLen > max)
                    break;
                returnValue += val[i];
            }
        }

        return returnValue;
    }

    public static IsString(val): boolean{
        return typeof(val) == "string";
    }

    public static IsObject(val): boolean{
        return (typeof(val) == "object") && ((val instanceof Array) == false);
    }

    //从数组里移除一个元素
    public static RemoveItem(arr: Array<any>, item: any): void{
        if(this.IsArray(arr)){
            for(let i:number = 0; i < arr.length; i ++){
                if(arr[i] == item){
                    arr.splice(i, 1);
                    return;
                }
            }
        }
    }

    //排序 order：false 从大到小 order: true 从小到大
    public static Sort(arr: Array<number>, order){
        let compare = function(x, y) {
            if (x < y) {
                if(order){
                    return -1;
                }
                return 1;
            } else if (x > y) {
                if(order){
                    return 1;
                }
                return -1;
            } else {
                return 0;
            }
        };

        arr.sort(compare);
    }

    public static RandomNum(min: number, max: number) : number{
        return parseInt( (Math.random()*(max-min+1)+min) + "", 10);
    }

    public static GetDistance(p1: cc.Vec2, p2: cc.Vec2): number{
        return Math.sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
    }

    public static IsArray(obj) {
        return typeof obj == 'object' && obj.constructor == Array;
    }

    //去重
    public static RemoveDuplicate(arr: Array<any>){
        if(this.IsArray(arr)){
            let newArr = [];
            for(let i = 0;i < arr.length;i ++){
                if(newArr.indexOf(arr[i]) < 0){
                    newArr.push(arr[i]);
                }
            }
            return newArr;
        }
        return arr;
    }

    //去掉包含在arr2中的元素
    public static RemoveDuplicateArray(arr1: Array<any>, arr2: Array<any>){
        if(this.IsArray(arr1) && this.IsArray(arr2)){
            let newArr = [];
            for(let j = 0; j < arr2.length; j ++){
                let index = arr1.indexOf(arr2[j]);
                if(index >= 0){
                    arr1.splice(index, 1);
                }
            }
            return newArr;
        }
    }

    //根据指定的概率返回true
    public static RandomPercentage(percentile: number){
        let value = this.RandomNum(1, 100);

        if(value <= percentile){
            return true;
        }
        return false;
    }

    //从一个数组里随机取一个元素
    public static RandomElement(arr: Array<any>){
        if(arr){
            let len = arr.length;

            let minIndex = 0;
            let maxIndex = len - 1;

            let index = this.RandomNum(minIndex, maxIndex);

            return arr[index];
        }

        return null;
    }

    //打乱数组
    public static Shuffle(arr: Array<any>){
        for(let j, x, i = arr.length; i; j =parseInt(Math.random() * i + ""), x = arr[--i], arr[i] = arr[j], arr[j] = x);
    }

    //随机生成guid
    public static GUID(): string {
        return 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function(c) {
            let r = Math.random()*16|0, v = c == 'x' ? r : (r&0x3|0x8);
            return v.toString(16);
        });
    }

    public static Dot(v1, v2): number{
        return v1.x * v2.x + v1.y * v2.y;
    }

    public static Sub(v1, v2, out) :number{
        if(v1 && v2 && out){
            out.x = v1.x - v2.x;
            out.y = v1.y - v2.y;
        }else{
            GameLog.PrintLog("Sub fail!");
        }
        return out;
    }

    public static AngleSigned(a, b): number{

        let POINT_EPSILON = parseFloat("1.192092896e-07F");

        let a2 = a.normalize();
        let b2 = b.normalize();
        let angle = Math.atan2(a2.x * b2.y - a2.y * b2.x, this.Dot(a2, b2));
        if (Math.abs(angle) < POINT_EPSILON)
            return 0.0;
        return angle;
    }

    //必须是同一个坐标系内的两个点
    public static GetRotation(selfPos, targetPos = null): number{
        let newVec: cc.Vec2 = null;
        
        if(targetPos == null)
        {
            newVec = selfPos;
        }else{
            newVec = cc.v2();
            this.Sub(targetPos, selfPos, newVec);
        }

        let angle = this.AngleSigned(newVec, cc.v2(0, 1));

        return this.RadiansToDegrees(angle);
    }

    //给定一个圆心，在圆上的平均取几个点
    public static SurroundPoints(x: number, y: number, dis: number, count: number): Array<cc.Vec2>
    {
        let points = [];//结果在这里

        let temp: number = 360 / count;

        for (let i: number = 0; i < count; i++)
        {
            let angle: number = i * temp;
            let radian: number = this.AngleToRadian(angle);
            let newPos: cc.Vec2 = cc.v2();
            newPos.x = x + dis * Math.sin(radian);
            newPos.y = y + dis * Math.cos(radian);
            points.push(newPos);
        }
        return points;
    }

    //角度转弧度
    public static AngleToRadian(angle): number{
        return cc.macro.RAD * angle;
    }

    //弧度转角度
    public static RadiansToDegrees(angle): number{
        return cc.macro.DEG * angle;
    }

    public static Clamp(p, min_inclusive, max_inclusive, out) {
        if(out && p && min_inclusive && max_inclusive){
            out.x = cc.clampf(p.x, min_inclusive.x, max_inclusive.x);
            out.y = cc.clampf(p.y, min_inclusive.y, max_inclusive.y);
        }
        return out;
    }

    //判断一个向量是否为0
    public static IsZeroVec(v): boolean{
        return v && v.x === 0 && v.y === 0;
    }

    //将一个秒数转换成“00:00:00”的格式 (不能大于一天的秒数)
    public static ConvertTimeString(secondTime: number): string{
        let minute:any = Math.floor(secondTime / 60);
        let second:any = secondTime  % 60;
        let hour:any = Math.floor(minute / 60);

        if(minute >= 60)
            minute = Math.floor(minute % 60);

        if(minute.toString().length == 1)
            minute = "0" + minute;
        if(second.toString().length == 1)
            second = "0" + second;
        if(hour > 0 && hour.toString().length == 1)
            hour = "0" + hour;

        return hour > 0 ? hour + ":" + minute + ":" + second : minute + ":" + second;
    }

    //去一个数转换成二进制数后第几位的值(0或1)
    public static ValueAtBit(value: number, bit: number): number
    {
        return ( value >> (bit - 1) ) & 1;
    }

    //将以逗号分隔的字符串转换成数组
    public static StringConvertToArray(str: string): Array<any>
    {
        if(str)
        {
            let arr = str.split(",");
            let numarr = [];
            for(let i = 0;i < arr.length;i ++)
            {
                numarr.push(Number(arr[i]));
            }
            return numarr;
        }
        return [];
    }
};