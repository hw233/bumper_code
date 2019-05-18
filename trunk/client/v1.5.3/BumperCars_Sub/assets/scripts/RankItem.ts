import Common from './Common';
const {ccclass, property} = cc._decorator;

@ccclass
export default class RankItem extends cc.Component
{
    @property(cc.Node)
    private bgImg: cc.Node = null;      //背景图

    @property(cc.Node)
    private cupImg: cc.Node = null;     //奖杯

    @property(cc.Node)
    private headImg: cc.Node = null;    //头像

    @property(cc.Node)
    private playerName: cc.Node = null; //昵称

    @property(cc.Node)
    private rankNum: cc.Node = null;    //排名

    @property(cc.Node)
    private killRank: cc.Node = null;    //击杀排名

    @property(cc.Node)
    private gradingRank: cc.Node = null;    //段位排名

    start () 
    {

    }

    initView(rank: number, data: any, type: number, isme: boolean = false)
    {
        if(rank <= 3 || data.is_rank === false)
        {
            this.cupImg.active = true;
            this.rankNum.active = false;
            if(data.is_rank === false){
                Common.SetNodeTexture(this.cupImg, "bw");
            }else{
                Common.SetNodeTexture(this.cupImg, "No" + rank);
            }
        }else
        {
            this.rankNum.active = true;
            this.cupImg.active = false;
            Common.SetNodeText(this.rankNum, rank.toString());
        }

        if(isme){
            Common.SetNodeTexture(this.bgImg, "paimingtiao-B");
        }


        if(type == 1)
        {
            //击杀排名
            this.gradingRank.active = false;
            this.killRank.active = true;

            Common.SetNodeText(this.killRank.getChildByName("Num"), data.score);
        }else if(type == 2){
            //段位排名
            this.gradingRank.active = true;
            this.killRank.active = false;

            var curGradingID = Math.floor(data.score / 25) + 1;

            var fileName = 'Grading_' + curGradingID;

            Common.SetNodeTexture(this.gradingRank.getChildByName("Icon"), fileName);
            Common.SetNodeText(this.gradingRank.getChildByName("Num"), (data.score % 25).toString() );
        }

        
        //限制昵称最大为10个字符
        var nickname = Common.GetByteVal(data.nickname, 10);

        Common.CreateImage(this.headImg, data.avatarUrl);
        Common.SetNodeText(this.playerName, nickname);
    }
}