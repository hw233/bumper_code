import Util from "../common/Util";
import {GradingSystem} from "../Grading/GradingSystem";
import EngineUtil from "../common/EngineUtil";
import { AtlasType } from '../common/EngineUtil';
import GlobalVar from '../GlobalVar';

const {ccclass, property} = cc._decorator;

@ccclass
export default class RankItem extends cc.Component{
    @property({type: cc.Node})
    private bgImg = null;

    @property({type: cc.Node})
    private cupImg = null;

    @property({type: cc.Node})
    private headImg = null;

    @property({type: cc.Node})
    private playerName = null;

    @property({type: cc.Node})
    private rankNum = null;

    @property({type: cc.Node})
    private killRank = null;

    @property({type: cc.Node})
    private gradingRank = null;

    initView(data: any, isme: boolean = false)
    {
        let type: number = GlobalVar.rankType;
        let rank: number = data.rank;
        let is_rank: boolean = (rank <= 30);

        if(rank <= 3 || is_rank === false)
        {
            this.cupImg.active = true;
            this.rankNum.active = false;
            if(is_rank === false)
            {
                EngineUtil.SetNodeTextureByType(AtlasType.AT_RankListLayer, this.cupImg, "bw");
            }else
            {
                EngineUtil.SetNodeTextureByType(AtlasType.AT_RankListLayer, this.cupImg, "No" + rank);
            }
        }else
        {
            this.rankNum.active = true;
            this.cupImg.active = false;
            EngineUtil.SetNodeText(this.rankNum, rank.toString());
        }

        if(isme)
        {
            EngineUtil.SetNodeTextureByType(AtlasType.AT_RankListLayer, this.bgImg, "paimingtiao-B");
        }else
        {
            EngineUtil.SetNodeTextureByType(AtlasType.AT_RankListLayer, this.bgImg, "paimingtiao-A");
        }

        //限制昵称最大为10个字符
        let nickname = Util.GetByteVal(data.nickname, 10);

        EngineUtil.LoadRemoteImg(this.headImg, data.avatar_url);
        EngineUtil.SetNodeText(this.playerName, nickname);
        
        if(type == 1)
        {
            //击杀排名
            this.gradingRank.active = false;
            this.killRank.active = true;

            EngineUtil.SetNodeText(this.killRank.getChildByName("Num"), data.kill_num);
        }else if(type == 2)
        {
            //段位排名
            this.gradingRank.active = true;
            this.killRank.active = false;

            let starNum = data.star_num;
            let grandingID = data.granding_id;
            if(grandingID != 31)
            {
                if(grandingID <= 0)
                {
                    grandingID = 1;
                }
                starNum = (grandingID - 1) % 5 * 5 + data.star_num;
            }

            EngineUtil.SetNodeText(this.gradingRank.getChildByName("Num"), starNum);
            EngineUtil.SetNodeTextureByType(AtlasType.AT_Common, this.gradingRank.getChildByName("Icon"), GradingSystem.GetGradingFileName(grandingID));
        }
    }
}
