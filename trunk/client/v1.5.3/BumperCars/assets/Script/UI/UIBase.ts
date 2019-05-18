import ComponentBase from '../common/ComponentBase';
import { UIType } from './UIDefine';
//UI组件基类

export default class UIBase extends ComponentBase
{
    protected isShow: boolean = null;

    protected isInit: boolean = null;

    protected uiType: UIType = UIType.UT_None;

    protected param: any = null;

    //初始化化界面时调用
    public OnInit()
    {
        this.isInit = true;
        this.isShow = false;
    }

    //进入界面时调用
    public OnEnter()
    {
    }

    //退出界面时调用
    public OnExit(isEffect: boolean = true)
    {

    }

    //销毁界面时调用
    public OnDestroy()
    {

    }

    public FrameOnMove(dt: number)
    {

    }

    public SetParam(param: any)
    {
        this.param = param;
    }

    public SetType(type: UIType)
    {
        this.uiType = type;
    }

    public GetType(): UIType
    {
        return this.uiType;
    }
    
    public IsShow(): boolean
    {
        return this.isShow;
    }

    //显示界面
    public ShowView(isEffect: boolean = true): void
    {
    }

    //隐藏界面
    public HideView(isEffect: boolean = true)
    {
    }

    //移除界面
    public RemoveView(isEffect: boolean = true): void
    {
        this.node.destroy();
    }

    //对界面进行一些初始化操作
    public InitView(): void
    {

    }

    //刷新界面
    public RefreshView(): void
    {

    }
// //引擎回调
//     protected onLoad()
//     {
//         this.OnInit();
//     }

//     protected start()
//     {
//         this.OnEnter();
//     }

//     protected onDestroy()
//     {
//         super.onDestroy();

//         this.OnExit();
//     }

    protected update(dt: number)
    {
        this.FrameOnMove(dt);
    }
}