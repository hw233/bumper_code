
//单个玩家的排行榜数据
export class PlayerRankData{
    public userID     = null;            //用户ID
    public rankNum    = null;           //排名
    public nickName   = null;         //昵称   
    public killNum    = null;           //击杀数 
    public scoreNum   = null;          //积分

    constructor(){
        this.userID = 0;            //用户ID
        this.rankNum = 0;           //排名
        this.nickName = "";         //昵称   
        this.killNum = 0;           //击杀数 
        this.scoreNum = 0;          //积分
    }
};

//游戏中的排行榜数据
export default class GameRankData{

    private playerRankData = null;

    constructor(){
        this.playerRankData = [];
    };

    public InitPlayerRankData(players){
        if(this.playerRankData && players){
            for(let i = 0;i < players.length; i ++){
                let playerData = players[i];
                if(playerData){
                    let playerRankData = new PlayerRankData();
                    playerRankData.userID = playerData.uid;
                    playerRankData.rankNum = playerData.rank;
                    playerRankData.nickName = playerData.name;
                    playerRankData.killNum = playerData.killNum;
                    playerRankData.scoreNum = playerData.scoreNum;
        
                    this.playerRankData.push(playerRankData);
                }
            }
        }
    };

    public SyncPlayerRankData(player){
        if(this.playerRankData && player){
            for(let i = 0;i < this.playerRankData.length; i ++){
                let tempPlayer = this.playerRankData[i];
                if(tempPlayer && tempPlayer.userID == player.uid){
                    tempPlayer.rankNum = player.rank;
                    tempPlayer.nickName = player.name;
                    tempPlayer.killNum = player.killNum;
                    tempPlayer.scoreNum = player.scoreNum;
                    break;
                }
            }
        }
    };

    public GetAllPlayerData(){
        return this.playerRankData;
    };
};
