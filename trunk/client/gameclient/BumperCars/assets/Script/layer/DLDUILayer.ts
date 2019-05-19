import Util from "../common/Util";
import GameRankView from "./GameRankView";
const {ccclass, property} = cc._decorator;
import EngineUtil from "../common/EngineUtil";
import ComponentBase from '../common/ComponentBase';
import { MessageType } from "../common/EventCenter";
import GameUI from '../manager/UIManager';
import { UIType } from '../UI/UIDefine';

@ccclass
export default class DLDUILayer extends ComponentBase{

    @property({type: cc.Node})
    private topNode: cc.Node = null;

    //游戏时间
    @property({type: cc.Node})
    private gameTimeNode = null;

    //返回按钮
    @property({type: cc.Node})
    private returnBtn: cc.Node = null;
    
    //游戏排行榜
    @property({type: GameRankView})
    private gameRankList: GameRankView = null;

    //倒计时UI
    @property({type: cc.Node})
    private countDownNode: cc.Node = null;

    constructor(){
        super();
    }

    protected onLoad(){
        this.countDownNode.active = false;

        this.registerEvent(MessageType.Refresh_Game_Time, this.refreshGameTime.bind(this));

        EngineUtil.RegBtnClickEvent(this.returnBtn, this.returnBtnClicked.bind(this) );

        if(cc.director.getWinSize().height / cc.director.getWinSize().width - 16 / 9 > 0.1)
        { 
            this.gameTimeNode.y = -10;
        }

        this.gameTimeNode.getComponent(cc.Label).setIsUseCostom(true);
    }

    protected start () {
        this.initLayer();
    }

    private initLayer(){
        let timeStr = Util.ConvertTimeString(0);
        EngineUtil.SetNodeText(this.gameTimeNode, timeStr);

        this.gameRankList.initLayer();
    }

    //刷新游戏时间
    private refreshGameTime(time: number){
        let timeStr: string = Util.ConvertTimeString(time);
        EngineUtil.SetNodeText(this.gameTimeNode, timeStr);
    }

    public doStart(){
    }
    
    public doOver(){
    }

    public refreshUI(){
        this.gameRankList.refreshRankView();
    }

    //返回按钮点击
    private returnBtnClicked()
    {
        GameUI.ShowUI(UIType.UT_ReturnView);
    }
    

    //显示倒计时UI
    public showCountDown(isShow)
    {
        if(this.countDownNode)
        {
            this.countDownNode.active = isShow;
        }
    }
}
