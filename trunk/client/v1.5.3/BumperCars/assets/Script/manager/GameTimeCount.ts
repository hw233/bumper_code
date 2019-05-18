import EventCenter from '../common/EventCenter';
import { MessageType } from '../common/EventCenter';
import GameData from '../data/GameData';

//游戏倒计时
export default class GameTimeCount
{
    private static EventTag: string             = "GameTimeCount";
    private static gameTime: number             = null;
    private static startTimestamp: number       = null;

    private static interval                     = null;

    public static Init () 
    {
        this.gameTime = 0;
        this.startTimestamp = 0;

        EventCenter.RegisterEvent(this.EventTag, MessageType.Start_Game_Time_Count, this.StartGameTimeCount.bind(this));
        EventCenter.RegisterEvent(this.EventTag, MessageType.Stop_Game_Time_Count, this.StopGameTimeCount.bind(this));
	}

    //启动游戏时间计时
    private static StartGameTimeCount(): void
    {
        this.startTimestamp = GameData.GetGameStartTime();

        this.gameTime = GameData.GetGameTime();

        this.interval = setInterval(this.RefreshGameTime.bind(this), 1000);
    }

    private static StopGameTimeCount(): void
    {
        clearInterval(this.interval);
        this.interval = null;
    }

    private static RefreshGameTime(): void
    {
        this.gameTime = GameData.GetGameTime() - Math.round( (Date.now() - this.startTimestamp) / 1000 );
        if(this.gameTime >= 0)
        {
            EventCenter.DispatchEvent(MessageType.Refresh_Game_Time, this.gameTime);
        }
    }

    public static Destroy(): void
    {
        EventCenter.RemoveEvent(this.EventTag);
    }

}
