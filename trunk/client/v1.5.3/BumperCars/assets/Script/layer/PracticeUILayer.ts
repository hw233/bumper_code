import GameData from "../data/GameData";
import EngineUtil from "../common/EngineUtil";

const {ccclass, property} = cc._decorator;

@ccclass
export default class PracticeUILayer extends cc.Component{
    @property({type: cc.Node})
    private topNode = null;

        //剩下的人数
    @property({type: cc.Node})
    private remainingNumNode = null;

        //击杀的人数
    @property({type: cc.Node})
    private killNumNode = null;

        //金币数量
    @property({type: cc.Node})
    private goldNumNode = null;

        //匹配节点
    @property({type: cc.Node})
    private matchNode = null;

        //匹配的时间
    @property({type: cc.Node})
    private matchTimeNode = null;

        //倒计时UI
    @property({type: cc.Node})
    private countDownNode = null;

    matchTime =  null;

    ctor(){
        this.matchTime = 0;
    }

    onLoad(){
        this.countDownNode.active = false;

        if(cc.director.getWinSize().height / cc.director.getWinSize().width - 16 / 9 > 0.1){ 
            this.topNode.getComponent(cc.Widget).top = 50;
        }
    }

    init(){
        this.refreshUI()
    }

    startMatch(){
        this.matchNode.active = true;

        this.schedule(this.refreshMatchUI, 1, cc.macro.REPEAT_FOREVER, 0);
        this.schedule(this.refreshRemainingNum, 0.016, cc.macro.REPEAT_FOREVER, 0);
    }

    matchFinish(){
        this.unschedule(this.refreshMatchUI);
        this.unschedule(this.refreshRemainingNum);
        
        // this.unscheduleAllCallbacks();

        this.matchTimeNode.active = false;
        this.matchNode.active = false;
        this.matchTime = 0;
    }

    start () {

    }

    refreshUI(){
        EngineUtil.SetNodeText(this.remainingNumNode, GameData.GetTempData().GetRemainingNum());
        EngineUtil.SetNodeText(this.killNumNode, GameData.GetTempData().GetCurKillNum());
        EngineUtil.SetNodeText(this.goldNumNode, GameData.GetTempData().GetCurGoldNum());
    }

    //刷新剩余人数
    refreshRemainingNum()
    {
        EngineUtil.SetNodeText(this.remainingNumNode, GameData.GetTempData().GetRemainingNum());
    }

    //刷新匹配事件UI
    refreshMatchUI()
    {
        this.matchTime ++;
        EngineUtil.SetNodeText(this.matchTimeNode, this.matchTime);
    }

    //显示倒计时UI
    showCountDown(isShow){
        if(this.countDownNode){
            this.countDownNode.active = isShow;
        }
    }
}
