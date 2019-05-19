import EventCenter from "../common/EventCenter";
import {MessageType} from "../common/EventCenter";

///////////////游戏临时数据---Start//////////////////////

export default class GameTempData{
    private curKillNum          = null;     //当前局击杀数
    private curGoldNum          = null;     //当前局金币数
    private remainingNum        = null;     //剩余人数
    private continueKillNum     = null;     //连续击杀数
    private rankNum             = null;     //本局排名
    private maxContinueKillNum  = null;     //最大连续击杀数
    private isDoubleGold        = null;

    public InitTempData(){
        this.curKillNum = 0;
        this.curGoldNum = 0;
        this.remainingNum = 0;
        this.continueKillNum = 0;
        this.rankNum = 0;
        this.maxContinueKillNum = 0;
        this.isDoubleGold = false;
    };

    public SetRankNum(num){
        this.rankNum = num;
        EventCenter.DispatchEvent(MessageType.Refresh_UI);
    };

    public SetCurKillNum(num){
        this.curKillNum = num;
        EventCenter.DispatchEvent(MessageType.Refresh_UI);
    };

    public SetIsDoubleGold(value){
        this.isDoubleGold = value;
    };

    public AddCurKillNum(num){
        this.curKillNum += num;
        EventCenter.DispatchEvent(MessageType.Refresh_UI);
    };

    public SetCurGoldNum(num){
        this.curGoldNum = num;
        EventCenter.DispatchEvent(MessageType.Refresh_UI);
    };

    public AddCurGoldNum(num){
        this.curGoldNum += num;
        EventCenter.DispatchEvent(MessageType.Refresh_UI);
    };

    public SetRemainingNum(num){
        this.remainingNum = num;
        EventCenter.DispatchEvent(MessageType.Refresh_UI);
    };

    public AddtRemainingNum(num){
        this.remainingNum += num;
        EventCenter.DispatchEvent(MessageType.Refresh_UI);
    };

    public GetRankNum(){
        return this.rankNum;
    };

    public GetCurKillNum(){
        return this.curKillNum;
    };

    public GetCurGoldNum(){
        return this.curGoldNum;
    };

    public GetRemainingNum(){
        return this.remainingNum;
    };

    public SetContinueKillNum(num){
        this.continueKillNum = num;
        if(this.continueKillNum > this.maxContinueKillNum){
            this.maxContinueKillNum = this.continueKillNum;
        }
    };

    public AddContinueKillNum(num){
        this.continueKillNum += num;

        if(this.continueKillNum > this.maxContinueKillNum){
            this.maxContinueKillNum = this.continueKillNum;
        }
    };

    public GetContinueKillNum(){
        return this.continueKillNum;
    };

    public GetMaxContinueKillNum(){
        return this.maxContinueKillNum;
    };

    public GetIsDoubleGold(){
        return this.isDoubleGold;
    };
}


///////////////游戏临时数据---End//////////////////////