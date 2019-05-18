import ComponentBase from '../common/ComponentBase';
import GameUI from '../manager/UIManager';

export default class SceneBase extends ComponentBase
{
    //初始化时调用
    public OnInit(): void
    {

    }

    //进入场景调用
    public OnEnter(): void
    {

    }

    //每帧调用
    protected FrameOnMove(dt: number): void
    {

    }

    //退出场景调用
    protected OnExit(): void
    {
    }



    
//引擎回调函数
    protected onLoad()
    {
        this.OnInit();
    }

    protected start()
    {
        this.OnEnter();
    }

    protected update(dt: number)
    {
        this.FrameOnMove(dt);
    }

    protected onDestroy()
    {
        super.onDestroy();

        this.OnExit();
    }
}