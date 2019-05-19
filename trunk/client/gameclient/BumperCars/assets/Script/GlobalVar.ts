export default class GlobalVar{
    public static isGameOver = false;

    public static comingSwitchScene = null;

    public static shareTicket = null;

    public static IsReconnected = false;

    public static IsLoggedIn = false;

    public static LatestFrameTimeStamp = 0;

    public static OpenVibration = true;  //是否打开振动

    public static userID: number = null;   //user id

    public static SelectCarID: number = null;

    //是否打开群排行
    public static isOpenGroupRank = false;

    //排行榜类型
    public static rankType = 0;

    //游戏模式 1：练习模式 2：大乱斗 3：夺旗战
    public static gameModel = 0;

    //是否为房主
    public static isMaster = false;

    //是否加载过配置
    public static IsLoadConfig = false;

    public static GameAdGID = null;

    public static GameAdInfo = null;
}

