import AccountData from './manager/AccountData';
import EventCenter from './common/EventCenter';
import { MessageType } from './common/EventCenter';
import ItemSystem from './Item/ItemSystem';

export default class OnlineRewardTime
{
    private static tempTime = 0;

    private static isStart = false;

    public static StartTime()
    {
        this.isStart = true;
    }

    public static FrameOnMove(dt: number)
    {
        if(this.isStart === false)
        {
            return;
        }

        this.tempTime += dt;

        if(this.tempTime >= 1)
        {
            this.tempTime -= 1;

            this.updateTime();
        }
    }

    private static updateTime()
    {
        let time: number = AccountData.GetInstance().GetOnlineRewardTime() - 1;

        if(time >= 0)
        {
            AccountData.GetInstance().SetOnlineRewardTime(time);

            EventCenter.DispatchEvent(MessageType.Online_Reward_Time_Update);
        }

        let time2: number = AccountData.GetInstance().GetTimeLimitRemainTime() - 1;
        if(time2 >= 0)
        {
            AccountData.GetInstance().SetTimeLimitRemainTime(time2);

            EventCenter.DispatchEvent(MessageType.Item_Update_LiftTime_Event);
        }

        ItemSystem.GetInstance().UpdateLiftTime();
    }
}