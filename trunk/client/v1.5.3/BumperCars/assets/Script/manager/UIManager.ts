import EngineUtil from '../common/EngineUtil';
import UIBase from '../UI/UIBase';
import WXHelper from '../common/WXHelper';
import { HashMap } from '../common/HashMap';
import { UIType } from '../UI/UIDefine';
import Util from '../common/Util';

/**
 * 公共UI管理类
 */
export default class GameUI
{
    private static rootNode: cc.Node                = null;

    private static mainView: cc.Node                = null;

    private static loadingView: cc.Node             = null;
    private static loadingFailView: cc.Node         = null;

    private static friendView: cc.Node              = null;

    private static zIndex: number                   = null;

    private static uiList: HashMap<UIType,UIBase>   = null;     //UI列表(唯一的)

    private static uiArray: Array<UIBase>           = null;     //UI数组(可重复的)

    private static uiQuene: Array<UIBase>           = null;     //UI队列(依次展示的)

    public static Init(): void
    {
        this.zIndex = 1;
        this.uiList = new HashMap<UIType,UIBase>();
        this.uiArray = [];
        this.uiQuene = [];
    }

    public static AddView(view: cc.Node): void
    {
        if(view)
        {
            this.rootNode.addChild(view, this.zIndex ++);
        }
    }

    //设置当前主界面
    public static SetMainView(mainView: cc.Node): void
    {
        this.mainView = mainView;
    }

    //添加一个界面到队列里
    public static AddViewToQuene(uiType: UIType, param: any = null)
    {
        if(this.IsVaild(uiType) === false)
        {
            return;
        }

        let uiView: UIBase = this.CreateUI(uiType);
        if(uiView)
        {
            uiView.SetParam(param);
            
            this.uiQuene.push(uiView);
        }
    }

    //显示UI队列里队头的界面并从队列里移除
    public static ShowViewByQuene()
    {
        if(this.uiQuene.length > 0)
        {
            let uiView: UIBase = this.uiQuene.shift();
            if(uiView)
            {
                uiView.node.zIndex = this.zIndex ++;

                this.rootNode.addChild(uiView.node);
    
                uiView.OnInit();
    
                uiView.InitView();
    
                uiView.ShowView();
    
                uiView.OnEnter();
    
                uiView.RefreshView();
            }
        }
    }

    //显示一个可以同时出现多个的UI界面
    public static ShowRepeatableUI(uiType: UIType, param: any = null): void
    {
        if(this.IsVaild(uiType) === false)
        {
            return;
        }

        let uiView: UIBase = this.GetRepeatableUI(uiType);
        if(!uiView)
        {
            uiView = this.CreateUI(uiType);
            if(uiView)
            {
                this.uiArray.push(uiView);

                this.rootNode.addChild(uiView.node);

                uiView.OnInit();
                    
                uiView.InitView();
            }
        }

        if(uiView)
        {
            uiView.node.zIndex = this.zIndex ++;

            uiView.SetParam(param);

            uiView.ShowView();

            uiView.OnEnter();

            uiView.RefreshView();
        }
    }

    //显示一个UI界面
    public static ShowUI(uiType: UIType, param: any = null)
    {
        if(this.IsVaild(uiType) === false)
        {
            return;
        }

        let uiView: UIBase = this.GetUI(uiType);
        if(!uiView)
        {
            uiView = this.CreateUI(uiType);
            if(uiView)
            {
                this.uiList.Add(uiType, uiView);
                this.rootNode.addChild(uiView.node);
    
                uiView.OnInit();
    
                uiView.InitView();
            }
        }

        if(uiView)
        {
            if(param !== null)
            {
                uiView.SetParam(param);
            }

            uiView.node.zIndex = this.zIndex ++;

            uiView.ShowView();

            uiView.OnEnter();

            uiView.RefreshView();
        }
    }

    //隐藏一个UI界面
    public static HideUI(uiType: UIType)
    {
        if(this.IsVaild(uiType) === false)
        {
            return;
        }

        let uiView: UIBase = this.GetUI(uiType);
        if(uiView)
        {
            uiView.HideView();

            uiView.OnExit();
        }
    }

    //刷新一个UI界面
    public static RefreshUI(uiType: UIType, param: any = null)
    {
        if(this.IsVaild(uiType) === false)
        {
            return;
        }

        let uiView: UIBase = this.GetUI(uiType);
        if(uiView)
        {
            if(param)
            {
                uiView.SetParam(param);
            }
            uiView.RefreshView();
        }
    }

    //移除一个UI界面
    public static RemoveUI(uiType: UIType)
    {
        if(this.IsVaild(uiType) === false)
        {
            return;
        }

        let uiView: UIBase = this.GetUI(uiType);
        if(uiView)
        {
            this.uiList.Remove(uiView.GetType());

            uiView.OnExit();

            uiView.OnDestroy();

            uiView.RemoveView();
        }
    }

    //移除当前场景缓存的UI
    public static RemoveAllUI(): void
    {
        if(this.uiList)
        {
            this.uiList.ForEach(function(key: UIType, value: UIBase)
            {
                value.OnDestroy();
                value.RemoveView();
            });

            this.uiList.Clear();
        }
    }

    //打开模拟服务器消息界面
    public static ShowTestMessageView(): void
    {
        let self = this;
        cc.loader.loadRes("prefab/TestMsgLayer", cc.Prefab, function(err, prefab){
            cc.log(prefab);
            if(prefab)
            {
                let testNode: cc.Node = cc.instantiate(prefab);
                if(testNode)
                {
                    if(self.rootNode)
                    {
                        self.rootNode.addChild(testNode);
                    }
                }
            }
        });
    }

    //显示加载界面
    public static ShowLoadingView(): void
    {
        if(!this.rootNode)return;

        if(!this.loadingView)
        {
            let loadingViewPrefab = EngineUtil.GetPrefab("LoadingView");
            if(loadingViewPrefab)
            {
                this.loadingView = cc.instantiate(loadingViewPrefab);
                this.rootNode.addChild(this.loadingView, 10000);
            }
        }

        if(this.loadingView)
        {
            this.loadingView.setPosition(0, 0);
        }
    }

    //隐藏加载界面
    public static HideLoadingView(): void
    {
        if(this.loadingView)
        {
            this.loadingView.setPosition(9999, 9999);
        }
    }

    //显示加载失败界面
    public static ShowLoadingFailView(): void
    {
        if(!this.loadingFailView)
        {
            let failViewPrefab = EngineUtil.GetPrefab("LoadingFail");
            if(failViewPrefab)
            {
                this.loadingFailView = cc.instantiate(failViewPrefab);
                this.rootNode.addChild(this.loadingFailView);
            }
        }

        if(this.loadingFailView)
        {
            this.loadingFailView.setPosition(0, 0);
        }
    }

    //显示加载失败界面
    public static HideLoadingFailView(): void
    {
        if(this.loadingFailView)
        {
            this.loadingFailView.setPosition(9999, 9999);
        }
    }

    //文字提示
    public static ShowHint(hint: string): void
    {
        WXHelper.ShowToast(hint);
    }

    public static SetRootNode(rootNode: cc.Node): void
    {
        this.rootNode = rootNode;
    }

    private static GetUI(uiType: UIType): UIBase
    {
        let uiNode: UIBase = null;
        if(uiType > UIType.UT_None && uiType < UIType.UT_Max)
        {
            if(this.uiList)
            {
                uiNode = this.uiList.Get(uiType);
            }
        }
        return uiNode;
    }

    private static GetRepeatableUI(uiType: UIType): UIBase
    {
        for(let i = 0, len = this.uiArray.length;i < len; i ++)
        {
            let uiView: UIBase = this.uiArray[i];
            if(uiView)
            {
                if(uiView.GetType() === uiType && uiView.IsShow() === false)
                {
                    return uiView;
                }
            }
        }
        return null;
    }

    private static CreateUI(uiType: UIType): UIBase
    {
        let uiNode: cc.Node = null;
        let uiCom: UIBase = null;

        switch(uiType)
        {
            case UIType.UT_ComeOnView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("ComeOnView"));
                uiCom = uiNode.getComponent("ComeOnGiftView");
                break;
            }
            case UIType.UT_CreateRoomView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("CreateNode"));
                uiCom = uiNode.getComponent("CreateLayer");
                break;
            }
            case UIType.UT_FriendApplyView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("FriendApply"));
                uiCom = uiNode.getComponent("FriendApplyView");
                break;
            }
            case UIType.UT_GameInviteView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("GameInvite"));
                uiCom = uiNode.getComponent("GameInviteView");
                break;
            }
            case UIType.UT_RankListView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("RankListLayer"));
                uiCom = uiNode.getComponent("RanklistLayer");
                break;
            }
            case UIType.UT_SettingView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("SettingNode"));
                uiCom = uiNode.getComponent("SettingView");
                break;
            }
            case UIType.UT_UnlockSucView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("UnlockSucView"));
                uiCom = uiNode.getComponent("UnlockSucView");
                break;
            }
            case UIType.UT_UnlockFailView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("UnlockFailView"));
                uiCom = uiNode.getComponent("UnlockFailView");
                break;
            }
            case UIType.UT_FriendView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("FriendView"));
                uiCom = uiNode.getComponent("FriendLayer");
                break;
            }
            case UIType.UT_SignInView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("SignInView"));
                uiCom = uiNode.getComponent("SignInView");
                break;
            }
            case UIType.UT_TaskView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("TaskVeiw"));
                uiCom = uiNode.getComponent("TaskView");
                break;
            }
            case UIType.UT_MenuView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("MenuView"));
                uiCom = uiNode.getComponent("MenuView");
                break;
            }
            case UIType.UT_CarLibView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("CarLibraryView"));
                uiCom = uiNode.getComponent("CarLibView");
                break;
            }
            case UIType.UT_SignInBoxRewardView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("SignInBoxReward"));
                uiCom = uiNode.getComponent("SignInBoxRewardView");
                break;
            }
            case UIType.UT_VasView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("VasNode"));
                uiCom = uiNode.getComponent("VasLayer");
                break;
            }
            case UIType.UT_FXJLView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("FXJLNode"));
                uiCom = uiNode.getComponent("FXJLView");
                break;
            }
            case UIType.UT_ReliveView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("ReliveNode"));
                uiCom = uiNode.getComponent("ReliveView");
                break;
            }
            case UIType.UT_ReturnView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("ReturnView"));
                uiCom = uiNode.getComponent("ReturnView");
                break;
            }
            case UIType.UT_PracticeOverView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("OverLayer"));
                uiCom = uiNode.getComponent("OverLayer");
                break;
            }
            case UIType.UT_DLDOverView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("DLDOverLayer"));
                uiCom = uiNode.getComponent("DLDOverLayer");
                break;
            }
            case UIType.UT_DQZOverView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("DQZOverLayer"));
                uiCom = uiNode.getComponent("DQZOverLayer");
                break;
            }
            case UIType.UT_RewardView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("RewardView"));
                uiCom = uiNode.getComponent("RewardView");
                break;
            }
            case UIType.UT_TryUseFinishView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("CarExpireView"));
                uiCom = uiNode.getComponent("CarExpireView");
                break;
            }
            case UIType.UT_TryUseSucView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("TrialSucView"));
                uiCom = uiNode.getComponent("TrialSucView");
                break;
            }
            case UIType.UT_NormalView:
            {
                uiNode = cc.instantiate(EngineUtil.GetPrefab("NormalView"));
                uiCom = uiNode.getComponent("NormalView");
                break;
            }
        }

        if(uiCom)
        {
            uiCom.SetType(uiType);
        }

        return uiCom;
    }

    private static IsVaild(uiType: UIType)
    {
        if(uiType > UIType.UT_None && uiType < UIType.UT_Max)
        {
            return true;
        }
        return false;
    }
    
    public static Destroy(){
        if(this.loadingView)
        {
            this.loadingView.destroy();
            this.loadingView = null;
        }
        if(this.loadingFailView)
        {
            this.loadingFailView.destroy();
            this.loadingFailView = null;
        }
        if(this.friendView)
        {
            this.friendView.destroy();
            this.friendView = null;
        }

        this.mainView = null;
        this.rootNode = null;
    }
}
