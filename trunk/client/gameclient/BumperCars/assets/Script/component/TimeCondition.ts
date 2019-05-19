/**
 * 限制某个函数一段时间内不能重复执行
 */

export default class TimeCondition
{
    private m_PreTime: number = null;

    private m_ConditionTime: number = null;

    constructor()
    {
    }

    public SetConditionTime(time: number)
    {
        this.m_ConditionTime = time;
    }

    public IsMeetCondition(): boolean
    {
        if(this.m_PreTime === null){
            this.m_PreTime = Date.now();
        }

        let cur = Date.now();

        if( (cur - this.m_PreTime) < this.m_ConditionTime )
        {
            return false;
        }
        else
        {
            this.m_PreTime = cur;
            return true;
        }
    }
}