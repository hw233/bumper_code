////////////////////////////////////////////////////////////////////
//                            _ooOoo_                             //
//                           o8888888o                            //
//                           88" . "88                            //
//                           (| ^_^ |)                            //
//                           O\  =  /O                            //
//                        ____/`---'\____                         //
//                      .'  \\|     |//  `.                       //
//                     /  \\|||  :  |||//  \                      //
//                    /  _||||| -:- |||||-  \                     //
//                    |   | \\\  -  /// |   |                     //
//                    | \_|  ''\---/''  |   |                     //
//                    \  .-\__  `-`  ___/-. /                     //
//                  ___`. .'  /--.--\  `. . ___                   //
//                ."" '<  `.___\_<|>_/___.'  >'"".                //
//              | | :  `- \`.;`\ _ /`;.`/ - ` : | |               //
//              \  \ `-.   \_ __\ /__ _/   .-` /  /               //
//        ========`-.____`-.___\_____/___.-`____.-'========       //
//                             `=---='                            //
//        ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^      //
//            佛祖保佑       无BUG        不修改                   //
////////////////////////////////////////////////////////////////////

import PlayerData from '../data/PlayerData';
import GlobalVar from "../GlobalVar";
import { TeamType, PlayerState } from '../GameDefine';

export default class PlayerMgr 
{
    //玩家列表
    private static playerList: Object = null;

    //玩家userid列表
    private static playerUserIDList: Array<number> = null;

    //队伍列表
    private static teamList: Object =  null;

    public static InitMgr(){
        this.teamList = null;
        this.playerList = null;
        if(this.playerUserIDList)
        {
            this.playerUserIDList.length = 0;
        }

        this.teamList = {}
        this.playerList = {}
        this.playerUserIDList = [];
    }

    //根据uid、昵称和头像添加一个玩家(练习模式使用)
    public static AddPlayer(userID: number, nickName: string, avatarUrl: string): void
    {
        let playerData = new PlayerData();

        playerData.setUserID(userID);
        playerData.setNickname(nickName);
        playerData.setAvatarUrl(avatarUrl);

        if(!CC_WECHATGAME)
        {
            playerData.setNickname(userID.toString());
        }

        this.playerList[userID] = playerData;
        this.playerUserIDList.push(userID);
    }  

    public static AddPlayerByInfo(info: any): void
    {
        if(info && this.playerList && this.playerUserIDList)
        {
            let playerData = new PlayerData();

            let userID: number = parseInt(info.uid || info.target_id);

            playerData.setUserID( userID );
            playerData.setNickname( info.nick_name );
            playerData.setAvatarUrl( info.head_picurl );
            playerData.setCarID( parseInt(info.use_car) );
            playerData.setMotionID( parseInt(info.use_motion) );
            playerData.setTeamID( parseInt(info.team_id) );
            playerData.setTeamType( parseInt(info.team_colour) );
            playerData.setBirthPointID( parseInt(info.region_id || 0 ) );
            
            if(!CC_WECHATGAME)
            {
                playerData.setNickname(userID.toString());
            }

            this.playerList[userID] = playerData;
            this.playerUserIDList.push(userID);
        }
    }

    public static RemovePlayer(userID: number): void
    {
        if(this.IsValid(userID))
        {
            if(this.playerUserIDList && userID != null && userID != undefined)
            {
                for(let i = 0;i < this.playerUserIDList.length;i ++)
                {
                    if(this.playerUserIDList[i] == userID)
                    {
                        this.playerUserIDList.splice(i, 1);
                        break;
                    }
                }
            }
    
            if(this.playerList && this.playerList[userID])
            {
                delete this.playerList[userID];
            }
        }
    }

    public static GetPlayerUserIDList(): Array<number>
    {
        return this.playerUserIDList;
    }


    public static GetPlayer(userID: number): PlayerData
    {
        if(this.IsValid(userID))
        {
            if(this.playerList && this.playerList[userID]){
                return this.playerList[userID];
            }
        }
        return null;
    }

    public static GetPlayerByIndex(index: number): PlayerData
    {
        if(this.playerUserIDList)
        {
            let userID = this.playerUserIDList[index];
            if(this.IsValid(userID))
            {
                return this.playerList[userID];
            }
        }
        return null;
    }

    public static GetPlayerIDByIndex(index: number): number
    {
        if(this.playerUserIDList)
        {
            let userID = this.playerUserIDList[index];
            if(this.IsValid(userID))
            {
                return userID;
            }
        }
        return null;
    }

    public static GetTeamTypeByTeamID(teamID: number): TeamType
    {
        return this.teamList[teamID];
    }

    public static AddTeamTypeByTeamID(teamID: number, teamType: number): void{
        this.teamList[teamID] = teamType;
    }

    //获得房间内的玩家数量
    public static GetPlayerCount(): number
    {
        return this.playerUserIDList.length;
    }

    //同步房间内的玩家列表
    public static SynPlayerList(roomInfo): void 
    {
        this.playerUserIDList.length = 0;
        this.playerList = null;

        if(roomInfo){
            for(let i = 0; i < roomInfo.length;i ++){
                let info = roomInfo[i];
                this.AddPlayerByInfo(info);
            }
        }
    }

    //获得活动中的玩家数量
    public static GetAcitivePlayer(): number
    {
        let count = 0;
        for(let key in this.playerList) {
            let player = this.playerList[key];
            if(player && player.getState() == 1){
                count ++;
            }
        }
        return count;
    }

    //获得活动中的玩家列表
    public static GetActivePlayerList(): Array<number>
    {
        let playerList: Array<number> = [];
        for(let key in this.playerList) {
            let player = this.playerList[key];
            if(player && player.getState() == 1){
                playerList.push(player.getUserID());
            }
        }
        return playerList;
    }

    //获得自己的队伍类型
    public static GetSelfTeamType(): TeamType
    {
        let player: PlayerData = this.GetPlayer(GlobalVar.userID);
        if(player){
            return player.getTeamType();
        }

        return null;
    }

    //判断一个玩家是否存在
    public static IsExistPlayer(uid: number): boolean
    {
        if(this.playerList)
        {
            if(this.playerList[uid])
            {
                return true;
            }
        }
        return false;
    }

    public static IsValid(userid: number): boolean
    {
        if(this.IsExistPlayer(userid))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    public static IsActive(userid: number): boolean
    {
        if(this.IsValid(userid))
        {
            let playerData: PlayerData = this.playerList[userid];

            if(playerData.getState() == PlayerState.PS_Alive)
            {
                return true;
            }
        }

        return false;
    }

    public static IsAlive(userid: number): boolean
    {
        if(this.IsValid(userid))
        {
            let playerData: PlayerData = this.playerList[userid];

            if(playerData.getState() === PlayerState.PS_Alive || playerData.getState() === PlayerState.PS_Invincible || playerData.getState() === PlayerState.PS_WaitRelive)
            {
                return true;
            }
        }

        return false;
    }

    public static IsSelf(userID: number): boolean
    {
        return GlobalVar.userID == userID;
    }

    public static Clear(): void 
    {
        this.teamList = null;
        this.playerList = null;

        if(this.playerUserIDList)
        {
            this.playerUserIDList.length = 0;
            this.playerUserIDList = null;
        }
    }
}
