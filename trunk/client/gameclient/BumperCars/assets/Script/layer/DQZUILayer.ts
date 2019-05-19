import Util from "../common/Util";
import GameData from "../data/GameData";
import EngineUtil from "../common/EngineUtil";
import { MessageType } from '../common/EventCenter';
import ComponentBase from '../common/ComponentBase';
import GameUI from '../manager/UIManager';
import { UIType } from '../UI/UIDefine';

const {ccclass, property} = cc._decorator;

@ccclass
export default class DQZUILayer extends ComponentBase{
    @property({type: cc.Node})
    private topNode: cc.Node = null;

    //游戏时间框
    @property({type: cc.Node})
    private gameTimeFrame: cc.Node = null;

    @property({type: cc.Node})
    private gameTimeNode = null;

    //返回按钮
    @property({type: cc.Node})
    private returnBtn: cc.Node = null;

    @property({type: cc.Node})
    private countDownNode: cc.Node = null;

    @property({type: cc.Node})
    private redScoreNode: cc.Node = null;

    @property({type: cc.Node})
    private blueScoreNode: cc.Node = null;

    onLoad(){
        this.countDownNode.active = false;

        this.registerEvent(MessageType.Refresh_Game_Time, this.refreshGameTime.bind(this));

        EngineUtil.RegBtnClickEvent(this.returnBtn, this.returnBtnClicked.bind(this) );

        if(cc.director.getWinSize().height / cc.director.getWinSize().width - 16 / 9 > 0.1)
        { 
            this.gameTimeFrame.active = false;
            this.gameTimeNode.x = -300;
        }

        this.gameTimeNode.getComponent(cc.Label).setIsUseCostom(true);
    }

    start () 
    {
        this.initLayer();
    }

    initLayer(){
        let timeStr = Util.ConvertTimeString(0);
        EngineUtil.SetNodeText(this.gameTimeNode, timeStr);
    }

    doStart(){
    }

    doOver(){
    }

    refreshUI(){
        let teamID: number = GameData.GetBettleData().getTeamID();

        let redScore: number = GameData.GetBettleData().getRedScore();
        let blueScore: number = GameData.GetBettleData().getBlueScore();

        EngineUtil.SetNodeText(this.redScoreNode, redScore.toString());
        EngineUtil.SetNodeText(this.blueScoreNode, blueScore.toString());
    }

    //刷新游戏时间
    private refreshGameTime(time: number){
        let timeStr: string = Util.ConvertTimeString(time);
        EngineUtil.SetNodeText(this.gameTimeNode, timeStr);
    }

    //返回按钮点击
    private returnBtnClicked()
    {
        GameUI.ShowUI(UIType.UT_ReturnView);
    }

    //显示倒计时UI
    showCountDown(isShow)
    {
        if(this.countDownNode)
        {
            this.countDownNode.active = isShow;
        }
    }
}
