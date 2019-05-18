import AccountData      from '../manager/AccountData';
import BettleData       from './net/BettleData';
import DLDSettleData    from './net/DLDSettleData';
import DQZSettleData    from './net/DQZSettleData';
import GameRankData     from './net/GameRankData';
import GameTempData     from './GameTempData';
import WXHelper         from '../common/WXHelper';

export default class GameData
{
    //房间id
    private static  roomID: number          = null;
    //游戏开始时间
    private static  gameStartTime: number   = null;    
    //游戏时间 单位：秒 
    private static  gameTime: number        = null;    
    //房间状态 
    private static  roomState               = null;     

    //单机游戏临时数据
    private static  gameTempData: GameTempData      = null;            
    //游戏排行数据
    private static  gameRankData: GameRankData      = null;         
    //战斗中数据
    private static  bettleData: BettleData          = null;
    //游戏结算数据
    private static  dldsettleData: DLDSettleData    = null;      
    //夺旗战结算数据   
    private static  dqzSettleData: DQZSettleData    = null;

    public static Init()
    {
    }

    //初始化游戏临时数据
    public static InitTempData()
    {
        if(!this.gameTempData)
        {
            this.gameTempData = new GameTempData();
        }

        this.gameTempData.InitTempData();
    }

    //初始化网络数据
    public static InitNetData()
    {
        this.bettleData             = new BettleData();
        this.gameRankData           = new GameRankData();

        this.dldsettleData          = new DLDSettleData();
        this.dqzSettleData          = new DQZSettleData();
    }

    //获得战斗中数据
    public static GetBettleData(): BettleData
    {
        return this.bettleData;
    }

    //获得游戏排行数据
    public static GetGameRankData(): GameRankData
    {
        return this.gameRankData;
    }

    //获得大乱斗游戏结算数据
    public static GetDLDSettleData(): DLDSettleData
    {
        return this.dldsettleData;
    }

    //获得夺旗战游戏结算数据
    public static GetDQZSettleData(): DQZSettleData
    {
        return this.dqzSettleData;
    }

    //获得游戏临时数据
    public static GetTempData(): GameTempData
    {
        return this.gameTempData;
    }

    //清除游戏临时数据
    public static ClearTempData()
    {
        if(this.gameTempData)
        {
            this.gameTempData = null;
        }
    }
    
    //清理网络通信数据
    public static ClearNetData()
    {
        if(this.gameRankData)
        {
            this.gameRankData = null;
        }
        if(this.bettleData)
        {
            this.bettleData = null;
        }
        if(this.dldsettleData)
        {
            this.dldsettleData = null;
        }

        if(this.dqzSettleData)
        {
            this.dqzSettleData = null;
        }
    }

    public static SetRoomState(state)
    {
        this.roomState = state;
    }

    public static GetRoomState()
    {
        return this.roomState;
    }

    public static SetRoomID(roomID)
    {
        this.roomID = roomID;
    }

    public static GetRoomID()
    {
        return this.roomID;
    }

    public static SetGameTime(time)
    {
        this.gameTime = time;
    }

    public static GetGameTime()
    {
        return this.gameTime;
    }

    public static SetGameStartTime(time: number)
    {
        this.gameStartTime = time;
    }

    public static GetGameStartTime(): number
    {
        return this.gameStartTime;
    }

    public static Save()
    {
        this.SaveCloudStorage();
    }

    //保存数据到微信用户托管数据
    public static SaveCloudStorage()
    {
        //上传用户托管数据
        let KVDataList = this.GenerateCloudData();
        WXHelper.UploadUserCloudStorage(KVDataList);
    }

    //生成用户托管数据
    public static GenerateCloudData()
    {
        let totalStarNum = this.GetTotalStarNum();
        let killNum = AccountData.GetInstance().GetKillNum() + this.gameTempData.GetCurKillNum();

        let killData = 
        {
            key: "kill_num",
            value: killNum.toString(),
        }

        let starData = 
        {
            key: "star_num",
            value: totalStarNum.toString(),
        }

        let keyDataList = [];

        keyDataList.push(killData);
        keyDataList.push(starData);

        return keyDataList;
    }

    //计算直到当前段位的获得的所有星星数量
    public static GetTotalStarNum()
    {
        let curLevelID = AccountData.GetInstance().GetCurLevelID();
        let curStarNum = AccountData.GetInstance().GetCurStarNum();

        return (curLevelID - 1) * 5 + curStarNum;
    }

    //根据星星总数计算段位id
    public static GetGradingIDByTotalStar(totalStarNum)
    {
        return Math.floor(totalStarNum / 5) + 1;
    }
    
}