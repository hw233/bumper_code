
//单个玩家的结算数据
export class PlayerSettleData{

    public rankNum: number    = null;          //排名
    public nickName: string   = null;         //昵称   
    public killNum: number    = null;          //击杀数 
    public scoreNum:number    = null;         //积分
    public userID: number     = null;           //userid

    constructor(){
        this.rankNum    = 0;           //排名
        this.nickName   = "";         //昵称   
        this.killNum    = 0;           //击杀数 
        this.scoreNum   = 0;          //积分
        this.userID     = 0;            //userid
    }
}

export default class DLDSettleData{

    private goldNum: number                         = null;  //奖励的金币数
    private bzlpNum: number                         = null;  //奖励的霸主令牌数量
    private isSettle: boolean                       = null;

    private allPlayerSettleData: Array<PlayerSettleData>   = null;

    constructor(){
        this.allPlayerSettleData = [];
        this.goldNum = 0;
        this.bzlpNum = 0;

        this.isSettle = false;
    };

    public InitPlayerSettleData(settleData){
        if(settleData){
            let playerSettleData = new PlayerSettleData();
            playerSettleData.rankNum = settleData.rank;
            playerSettleData.nickName = settleData.nick_name;
            playerSettleData.killNum = settleData.kill_sum;
            playerSettleData.scoreNum = settleData.fight_score;
            playerSettleData.userID = settleData.user_id;

            this.allPlayerSettleData.push(playerSettleData);
        }
    };

    public SetGoldNum(goldNum: number){
        this.goldNum = goldNum;
    };

    public SetBZLPNum(num: number){
        this.bzlpNum = num;
    };

    public GetAllPlayerData() : Array<PlayerSettleData>{
        return this.allPlayerSettleData;
    };

    public GetGoldNum() : number{
        return this.goldNum;
    };

    public GetBZLPNum() : number{
        return this.bzlpNum;
    };

    public SetSettleSuc(){
        this.isSettle = true;
    };

    public IsSettleSuc(): boolean{
        return this.isSettle;
    };
}
