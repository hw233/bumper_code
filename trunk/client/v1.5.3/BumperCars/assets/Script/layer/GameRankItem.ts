import Util from "../common/Util";
import EngineUtil from "../common/EngineUtil";



const {ccclass, property} = cc._decorator;

@ccclass
export default class GameRankItem extends cc.Component
{

    @property({type: cc.Node})
    private playerName = null;

    @property({type: cc.Node})
    private rankNum = null;

    @property({type: cc.Node})
    private killNum = null;

    @property({type: cc.Node})
    private scoreNum = null;

    onLoad()
    {
        this.rankNum.getComponent(cc.Label).setIsUseCostom(true);

        this.killNum.getComponent(cc.Label).setIsUseCostom(true);

        this.scoreNum.getComponent(cc.Label).setIsUseCostom(true);
    }

    initView(rank, data){
        //限制昵称最大为10个字符
        let nickname = Util.GetByteVal(data.getNickName(), 10);

        EngineUtil.SetNodeText(this.playerName, nickname);
        EngineUtil.SetNodeText(this.killNum, data.getKillNum());
        EngineUtil.SetNodeText(this.scoreNum, data.getScore());
        EngineUtil.SetNodeText(this.rankNum, rank);
    }

    refreshView(rank, data)
    {
        EngineUtil.SetNodeText(this.killNum, data.getKillNum());
        EngineUtil.SetNodeText(this.scoreNum, data.getScore());
        EngineUtil.SetNodeText(this.rankNum, rank);
    }
}
