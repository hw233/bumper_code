export default class GameConfig
{
    public static readonly GameID: number            = 91;

    public static readonly GameVersion: string       = "1.5.3";

    public static readonly GameListID: number        = 9101;

    public static readonly RemoteAddr: string        = "https://img.51weiwan.com/pengpengche/v1.5.3";

    //是否为调试模式
    public static readonly DebugModeSwitch: boolean  = true;    

    //服务器开关(关闭的话，则使用客户端模拟服务器消息用于测试)
    public static readonly ServerEnable: boolean     = true;

    public static readonly ServerIP:string           = "ws://192.168.2.179";  //内网ip
    // public static readonly ServerIP:string           = "ws://192.168.2.118";  //内网ip
    // public static readonly ServerIP:string           = "wss://gather.51weiwan.com"; //外网ip
    // public static readonly ServerPort:any            = "5505/test";
    public static readonly ServerPort:any           = "5505";

    public static readonly AppID: string             = "wxd844e5868051fbd1";

    //logo停留的时间
    public static readonly JWLogoDuration:number     = 1.5;

    //是否开启物理引擎(只支持练习模式)
    public static readonly PhysicsSwitch: boolean    = false; 

    //是否补帧
    public static readonly FillFrameSwitch: boolean  = true;

    //客户端逻辑帧时间
    public static readonly CLogicFPS: number          = 16;  

    //服务器逻辑帧时间
    public static readonly SLogicFPS: number          = 32;  

    public static readonly CSFPSMultiple: number      = 2;

    //反弹帧数
    public static readonly ReboundFrameCount: number  = GameConfig.CSFPSMultiple * 5; 

    //网络连接超时时间(毫秒数)
    public static readonly ConnectTimeout: number     = 10000;     

    public static readonly PingInvatal: number        = 1;

    //声音开关
    public static readonly SoundSwitch: boolean       = true;     

    public static readonly DebugNode: boolean         = false;

    //快速匹配时间
    public static readonly QuickMatchTime:number     = 15;     

    //计算多杀的有效时间
    public static readonly ContinueKillTime:number   = 8;  //单位 =秒


    //是否保存log(暂时没用)
    public static readonly SaveLogSwitch: boolean    = false;        

    public static readonly LogSwitch: boolean        = true;        //log开关
    public static readonly DebugLogSwitch: boolean   = CC_WECHATGAME ? false : false;
    public static readonly ErrorLogSwitch: boolean   = CC_WECHATGAME ? true : true;
    public static readonly WalkLogSwitch: boolean    = CC_WECHATGAME ? false : false;       //位置同步log开关
    public static readonly NetLogSwitch: boolean     = CC_WECHATGAME ? false : true;       //网络消息log
    public static readonly Timestemp: boolean        = CC_WECHATGAME ? false : false;        //是否打印时间戳


    //广告
    public static readonly GetGameADAddr:string      = 'https://gather.51weiwan.com/hz/general/plan1';
    //登录接口
    public static readonly LoginAddr:string          =  "https://gather.51weiwan.com/api/login/index";
    //获取配置的接口
    public static readonly GetConfigAddr:string      = "https://gather.51weiwan.com/api/app/getConfig";
    //获取跳转游戏列表
    public static readonly GetGameList:string        = 'https://gather.51weiwan.com/api/app/redirectlist';
    //发送匹配玩家的接口
    public static readonly SendMatchPlayerAddr:string= "https://gather.51weiwan.com/xxl/robot/rand";














    //首页推荐游戏切换时间
    public static HomePageGameTime: number  = 10;

    //复活开关
    public static Switch_Relive: boolean    = true;

    //加油礼包金币数量
    public static ComeGiftGoldNum: number   = 500;

    //加油礼包星星数量
    public static ComeGiftStrNum: number    = 1;

    //保星的次数
    public  static KeepStarCount: number    = 5;

    //福星降临的几率
    public  static FXJLProbability: number  = 70;

    //弹窗打开的时间
    public static PopUpDuration:number      = 0.1;

    //相机初始大小
    public static CameraZoomRatio: number   = 1;      

    //相机缩小后的大小
    public static CameraMinZoomRation: number= 0.6;    

    //试玩任务界面的推荐游戏列表
    public static RecommendGames            = "";

    public static Init(data: any)
    {
        if(!CC_WECHATGAME)
        {
            data = data.json;
        }

        if(data)
        {
            if(this.IsValid(data.relive_switch))
            {
                this.Switch_Relive          = data.relive_switch ? true : false;
            }
            if(this.IsValid(data.come_gift_gold_num))
            {
                this.ComeGiftGoldNum        = Number(data.come_gift_gold_num);
            }
            if(this.IsValid(data.come_star_star_num))
            {
                this.ComeGiftStrNum        = Number(data.come_star_star_num);
            }
            if(this.IsValid(data.keep_star_count))
            {
                this.KeepStarCount        = Number(data.keep_star_count);
            }
            if(this.IsValid(data.jxjl_probability))
            {
                this.FXJLProbability        = Number(data.jxjl_probability);
            }
            if(this.IsValid(data.popup_duration))
            {
                this.PopUpDuration        = Number(data.popup_duration);
            }
            if(this.IsValid(data.camera_init_ratio))
            {
                this.CameraZoomRatio        = Number(data.camera_init_ratio);
            }
            if(this.IsValid(data.camera_min_ratio))
            {
                this.CameraMinZoomRation    = Number(data.camera_min_ratio);
            }
            if(this.IsValid(data.homepage_gametime))
            {
                this.HomePageGameTime = Number(data.homepage_gametime);
            }
            if(this.IsValid(data.recommend_games))
            {
                this.RecommendGames = data.recommend_games;
            }
        }
    }

    public static IsValid(value: any)
    {
        if(value === undefined || value === null)
        {
            return false;
        }
        return true;
    }
};
