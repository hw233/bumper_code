import GameConfig from '../GameConfig';

export class ShareConfig
{
    public Title: string        = null;
    public Img: string          = null;
    public Description: string  = null;

    public Deserialization(data: any)
    {
        if(data)
        {
            this.Title = data.info.share_title;
            this.Img = data.info.share_img;
            this.Description = data.description;
        }
    }
}

export default class ConfigData
{
    //服务器ip
    public static ServerIP: string                = null;

    //服务器端口
    public static ServerPort: number              = null;

    //分享配置
    public  static ShareConfig: object            = null;

    //显示免费视频开关
    public static Switch_TimeLimitAd: boolean     = null;

    //保留复活广告开关
    public static Switch_ReliveAD: boolean        = null;

    //在线奖励加速开关
    public static Switch_AddSpeedAd: boolean      = null;

    public static IsInited: boolean               = false;

    public  static Init(): void
    {
        this.ShareConfig = {};
        this.ServerIP = GameConfig.ServerIP;
        this.ServerPort = GameConfig.ServerPort;
        this.Switch_TimeLimitAd = true;
        this.Switch_ReliveAD = false;
        this.Switch_AddSpeedAd = true;
    }

    public static Deserialization(data: any): void
    {
        this.IsInited = true;

        this.Switch_TimeLimitAd = Boolean(data.limit_time_award);
        this.Switch_ReliveAD = Boolean(data.new_gift);
        this.Switch_AddSpeedAd = Boolean(data.online_quick);

        if(GameConfig.GameVersion)
        {
            if(data.version == GameConfig.GameVersion)
            {
                this.ServerIP = data.wx_test_domain;
                this.ServerPort = data.wx_test_port;
            }
        }

        this.InitShareConfig(data);
    }

    public static InitShareConfig(data)
    {
        if(data.share)
        {
            for(var key in data.share)
            {
                let share = data.share[key];

                let shareConfig: ShareConfig = new ShareConfig();
                shareConfig.Deserialization(share);

                this.ShareConfig[key] = shareConfig;
            }
        }
    }
}