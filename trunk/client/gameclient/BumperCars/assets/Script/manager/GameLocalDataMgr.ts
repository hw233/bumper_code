import LocalDataUtil from '../common/LocalDataUtil';
import PlayerMgr from './PlayerMgr';
import PlayerData from '../data/PlayerData';

//游戏本地数据管理
export default class GameLocalDataMgr
{
    private static instance: GameLocalDataMgr   = null;

    private m_pLocalDataUtil: LocalDataUtil     = null;

    private m_pCurDate: Date                    = null;

    private m_nPlayGameCount: number            = null;         //领取礼包后玩游戏的局数
    private m_nWatchVideoCount: number          = null;         //观看视频的次数

    private m_bIsSignIn: boolean                = null;         //是否弹出签到

    private m_HomePageGames: Array<number>      = null;         //首页游戏列表

    private m_bIsFirstEnter: boolean            = null;         //今天是否首次进入游戏

    private constructor()
    {
        this.m_bIsSignIn = false;
        this.m_bIsFirstEnter = false;

        this.m_HomePageGames = [];

        this.m_nPlayGameCount = 0;
        this.m_nWatchVideoCount = 0;
    }

    public static GetInstance(): GameLocalDataMgr
    {
        if(!this.instance)
        {
            this.instance = new GameLocalDataMgr();
        }
        return this.instance;
    }
    
    public Init()
    {
        this.m_pLocalDataUtil = new LocalDataUtil();
    }

    //读取user id
    public ReadUserID(): number
    {
        if(this.m_pLocalDataUtil)
        {
            let userid = this.m_pLocalDataUtil.ReadData('userid');
            if(userid !== null)
            {
                return parseInt( userid);
            }
        }
        return null;
    }

    //清除user id
    public ClearUserID()
    {
        if(this.m_pLocalDataUtil)
        {
            this.m_pLocalDataUtil.RemoveData('userid');
        }
    }

    //保存user id
    public SaveUserID(userid: number)
    {
        if(this.m_pLocalDataUtil)
        {
            this.m_pLocalDataUtil.SaveData('userid', userid.toString());
        }
    }

    //读取用户昵称
    public ReadUserNickName(): string
    {
        if(this.m_pLocalDataUtil)
        {
            let nickname = this.m_pLocalDataUtil.ReadData('nickname');
            if(nickname !== null)
            {
                return nickname;
            }
        }
        return null;
    }

    //保存用户昵称
    public SaveUserNickName(nickname: string): void
    {
        if(this.m_pLocalDataUtil)
        {
            this.m_pLocalDataUtil.SaveData('nickname', nickname);
        }
    }

    //读取用户头像
    public ReadUserHeadUrl(): string
    {
        if(this.m_pLocalDataUtil)
        {
            let headUrl = this.m_pLocalDataUtil.ReadData('headurl');
            if(headUrl !== null)
            {
                return headUrl;
            }
        }
        return null;
    }

    //保存用户头像
    public SaveUserHeadUrl(headUrl: string): void
    {
        if(this.m_pLocalDataUtil)
        {
            this.m_pLocalDataUtil.SaveData('headurl', headUrl);
        }
    }

    //读取视频剩余观看次数
    public ReadVideoCount()
    {
        this.m_pCurDate = new Date();

        let key = this.m_pCurDate.toLocaleDateString();

        let dataObj = null;
        let dataStr = this.m_pLocalDataUtil.ReadData('video_count');
        if(dataStr){
            dataObj = JSON.parse(dataStr);
        }

        if(!dataObj || dataObj[key] == undefined)
        {
            this.m_nWatchVideoCount = 0;
        }
        else
        {
            this.m_nWatchVideoCount = parseInt(dataObj[key].watch_video_count) || 0;
        }
    }

    //保存视频剩余观察次数
    public SaveVideoCount()
    {
        if(this.m_pCurDate && this.m_nWatchVideoCount !== null)
        {
            let key = this.m_pCurDate.toLocaleDateString();

            let dataObj = {};

            dataObj[key] = 
            {
                watch_video_count: this.m_nWatchVideoCount,
            }

            if(this.m_pLocalDataUtil)
            {
                this.m_pLocalDataUtil.SaveData('video_count', JSON.stringify(dataObj));
            }
        }
    }

    public GetWatchVideoCount(): number
    {
        return this.m_nWatchVideoCount;
    }

    public SetWatchVideoCount(count: number)
    {
        this.m_nWatchVideoCount = count;

        this.SaveVideoCount();
    }

    //读取领取礼包后玩游戏的局数
    public ReadPlayGameCount()
    {
        this.m_pCurDate = new Date();

        let key = this.m_pCurDate.toLocaleDateString();

        let dataObj = null;
        let dataStr = this.m_pLocalDataUtil.ReadData('playgame_count');
        if(dataStr)
        {
            dataObj = JSON.parse(dataStr);
        }

        if(!dataObj || dataObj[key] == undefined)
        {
            this.m_nPlayGameCount = 0;
        }else
        {
            this.m_nPlayGameCount = parseInt(dataObj[key].playgame_count) || 0;
        }
    }

    //保存领取礼包后玩游戏的局数
    public SavePlayGameCount(): void
    {
        if(this.m_pCurDate && this.m_nPlayGameCount !== null)
        {
            let key = this.m_pCurDate.toLocaleDateString();

            let dataObj = {};

            dataObj[key] = 
            {
                playgame_count: this.m_nPlayGameCount,
            }

            if(this.m_pLocalDataUtil)
            {
                this.m_pLocalDataUtil.SaveData('playgame_count', JSON.stringify(dataObj));
            }
        }
    }

    //设置领取礼包后玩游戏的局数
    public SetPlayGameCount(count: number): void
    {
        this.m_nPlayGameCount = count;

        this.SavePlayGameCount();
    }

    //获取领取礼包后玩游戏的局数
    public GetPlayGameCount(): number
    {
        return this.m_nPlayGameCount;
    }

    //读取今天是否弹出过签到
    public ReadSignIn()
    {
        this.m_pCurDate = new Date();

        let key = this.m_pCurDate.toLocaleDateString();

        let dataObj = null;
        let dataStr = this.m_pLocalDataUtil.ReadData('signIn');
        if(dataStr)
        {
            dataObj = JSON.parse(dataStr);
        }

        if(!dataObj || dataObj[key] == undefined)
        {
            this.m_bIsSignIn = false;
        }else
        {
            this.m_bIsSignIn = Boolean(dataObj[key].signIn);
        }
    }

    //保存今天是否弹出过签到
    public SaveSignIn(): void
    {
        if(this.m_pCurDate && this.m_bIsSignIn !== null)
        {
            let key = this.m_pCurDate.toLocaleDateString();

            let dataObj = {};

            dataObj[key] = 
            {
                signIn: this.m_bIsSignIn === true ? 1 : 0,
            }

            if(this.m_pLocalDataUtil)
            {
                this.m_pLocalDataUtil.SaveData('signIn', JSON.stringify(dataObj));
            }
        }
    }

    //设置今天是否弹出过签到
    public SetSignIn(signIn: boolean): void
    {
        this.m_bIsSignIn = signIn;

        this.SaveSignIn();
    }

    //获取今天是否弹出过签到
    public GetSignIn(): boolean
    {
        return this.m_bIsSignIn;
    }

    //读取首页游戏列表
    public ReadHomePageGames()
    {
        let dataStr = this.m_pLocalDataUtil.ReadData('homepagegames');
        if(dataStr)
        {
            this.m_HomePageGames = JSON.parse(dataStr);
        }
    }

    //保存首页游戏列表
    public SaveHomePageGames(): void
    {
        if(this.m_HomePageGames !== null)
        {
            if(this.m_pLocalDataUtil)
            {
                this.m_pLocalDataUtil.SaveData('homepagegames', JSON.stringify(this.m_HomePageGames));
            }
        }
    }

    //设置首页游戏列表
    public SetHomePageGames(games: Array<number>): void
    {
        this.m_HomePageGames = games;

        this.SaveHomePageGames();
    }

    //获取首页游戏列表
    public GetHomePageGames(): Array<number>
    {
        return this.m_HomePageGames;
    }

    public ReadIsFirstEnter()
    {
        this.m_pCurDate = new Date();

        let key = this.m_pCurDate.toLocaleDateString();

        let dataObj = null;
        let dataStr = this.m_pLocalDataUtil.ReadData('isFirstEnter');
        if(dataStr)
        {
            dataObj = JSON.parse(dataStr);
        }

        if(!dataObj || dataObj[key] == undefined)
        {
            this.m_bIsFirstEnter = false;
        }else
        {
            this.m_bIsFirstEnter = Boolean(dataObj[key].isFirstEnter);
        }
    }

    public SaveIsFirstEnter(): void
    {
        if(this.m_pCurDate && this.m_bIsFirstEnter !== null)
        {
            let key = this.m_pCurDate.toLocaleDateString();

            let dataObj = {};

            dataObj[key] = 
            {
                isFirstEnter: this.m_bIsFirstEnter === true ? 1 : 0,
            }

            if(this.m_pLocalDataUtil)
            {
                this.m_pLocalDataUtil.SaveData('isFirstEnter', JSON.stringify(dataObj));
            }
        }
    }

    public SetIsFirstEnter(isFirst: boolean): void
    {
        this.m_bIsFirstEnter = isFirst;

        this.SaveIsFirstEnter();
    }

    public GetIsFirstEnter(): boolean
    {
        return this.m_bIsFirstEnter;
    }

    //保存最近玩家
    public SaveRecentlyPlayer()
    {
        let datas: Array<Object> = [];

        let nCount: number = PlayerMgr.GetPlayerCount();

        for(let i = 0; i< nCount; i++)
        {
            let playerData: PlayerData = PlayerMgr.GetPlayerByIndex(i);
            if(playerData)
            {
                datas.push({
                    friend_id: playerData.getUserID(),
                    nick_name: playerData.getNickName(),
                    head_picurl: playerData.getAvatarUrl(),
                    granding_id: playerData.getGrandingID(),
                    cur_star_sum: playerData.getCurStarNum(),
                });
            }
        }
        if(this.m_pLocalDataUtil)
        {
            this.m_pLocalDataUtil.SaveData('recently_player', JSON.stringify(datas));
        }
    }

    //获取最近玩家
    public GetRecentlyPlayer()
    {
        if(this.m_pLocalDataUtil)
        {
            let strInfo = this.m_pLocalDataUtil.ReadData('recently_player');
            if(strInfo)
            {
                let datas: Array<any> = JSON.parse(strInfo);
                if(datas)
                {
                    return datas;
                }
            }
            return null;
        }
    }

    public Destroy(): void
    {
        if(this.m_pLocalDataUtil)
        {
            this.m_pLocalDataUtil = null;
        }
    }

    public static Destroy(): void
    {
        if(this.instance)
        {
            this.instance.Destroy();
            this.instance = null;
        }
    }
};