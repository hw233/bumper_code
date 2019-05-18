import GameUI from '../manager/UIManager';

export enum SceneType
{
    None            = 0,
    StartScene      = 1,      //主界面场景
    LoadingScene    = 2,    //加载资源场景
    GameScene       = 3,       //练习模式场景
    LogoScene       = 4,       //Logo场景
    RanklistScene   = 5,   //排行榜场景
    DLDRoomScene    = 6,    //大乱斗房间场景
    DLDGameScene    = 7,    //大乱斗游戏场景
    ZDZRoomScene    = 8,    //组队战房间场景
    ZDZGameScene    = 9,    //组队战游戏场景
};

export default class SceneMgr
{
    private static _isSwitched: boolean         = false;       //是否正在切换场景

    private static _curSceneType: SceneType     = SceneType.None;

    private static _nextSceneType: SceneType    = SceneType.None;

    private static _isLoadedStartRes: boolean   = false;

    public static Init()
    {
        this._isSwitched = false;

        this._curSceneType = 0;

        this._isLoadedStartRes = false;

        cc.director.on(cc.Director.EVENT_AFTER_SCENE_LAUNCH, this.loadSceneFinish.bind(this));
    }

    public static SwitchScene(sceneType)
    {
        let nextScene = this.GetSceneByType(sceneType);

        this._curSceneType = sceneType;

        if(sceneType == SceneType.StartScene)
        {
            if(this._isLoadedStartRes == false)
            {
                nextScene = this.GetSceneByType(SceneType.LoadingScene);
                this._curSceneType = SceneType.LoadingScene;
                this._nextSceneType = sceneType;
                this._isLoadedStartRes = true;
            }else
            {
                if(this._nextSceneType === SceneType.StartScene)
                {
                    this._nextSceneType = null;
                }
            }
        }
        else if(sceneType == SceneType.GameScene || sceneType == SceneType.DLDGameScene || sceneType == SceneType.ZDZGameScene )
        {
            nextScene = this.GetSceneByType(SceneType.LoadingScene);
            this._curSceneType = SceneType.LoadingScene;
            this._nextSceneType = sceneType;
        }
        else
        {
            this._curSceneType = sceneType;
            this._nextSceneType = null;
        }

        this._isSwitched = true;

        this.runScene(nextScene);
    }

    public static SwitchNextScene()
    {
        if(this._nextSceneType)
        {
            let nextScene = this.GetSceneByType(this._nextSceneType);
            this._curSceneType = this._nextSceneType;
            this._nextSceneType = null;

            this._isSwitched = true;

            this.runScene(nextScene);
        }
    }

    public static GetCurSceneType()
    {
        return this._curSceneType;
    };

    public static GetNextSceneType()
    {
        return this._nextSceneType;
    }

    public static IsSwitched()
    {
        return this._isSwitched;
    }

    public static IsGameScene()
    {
        return this._curSceneType === SceneType.GameScene || this._curSceneType === SceneType.DLDGameScene || this._curSceneType === SceneType.ZDZGameScene;
        
    }

    public static GetSceneByType(sceneType)
    {
        let scene = null;

        switch(sceneType)
        {
            case SceneType.StartScene:
            {
                scene = 'StartScene';
                break;
            }
            case SceneType.GameScene:
            {
                scene = 'GameScene';
                break;
            }
                
            case SceneType.LoadingScene:
            {
                scene = 'LoadingScene';
                break;
            }

            case SceneType.LogoScene:
            {
                scene = 'LogoScene';
                break;
            }
            
            case SceneType.RanklistScene:
            {
                scene = 'RanklistScene'
                break;
            }

            case SceneType.DLDRoomScene:
            {
                scene = 'DLDRoomScene'
                break;
            }
            case SceneType.DLDGameScene:
            {
                scene = 'DLDGameScene';
                break;
            }
            case SceneType.ZDZRoomScene:
            {
                scene = 'DQZRoomScene'
                break;
            }
            case SceneType.ZDZGameScene:
            {
                scene = 'DQZGameScene';
                break;
            }

        }
        return scene;
    }

    private static runScene(sceneName: string)
    {
        GameUI.ShowLoadingView();

        GameUI.RemoveAllUI();

        cc.director.preloadScene(sceneName, function()
        {
            GameUI.HideLoadingView();

            cc.director.loadScene(sceneName);
        } );
    }
    
    //场景加载结束
    private static loadSceneFinish()
    {
        this._isSwitched = false;
    }
};
