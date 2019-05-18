import PlayerOpData from '../PlayerOpData';
import CommandBase from './CommandBase';
import PlayerMgr from '../manager/PlayerMgr';
import PlayerData from '../data/PlayerData';
import { UserCmdType } from '../GameDefine';
import GameLog from '../Log/LogTool';

export default class CommandCollide extends CommandBase
{
    public constructor()
    {
        super();
    }

    public DoExecute(dt: number, frameID: number, cmd: number, data: PlayerOpData, isQuickRun: boolean = false)
    {
        let playerID: number = data.getPlayerID();

        if(this.IsCanHandle(playerID, cmd, frameID) )
        {
            GameLog.PrintDebug(playerID, "-", "执行-碰撞-", frameID);

            let playerData: PlayerData = PlayerMgr.GetPlayer(playerID);

            let impluse: cc.Vec2 = cc.v2(data.getArg(1), data.getArg(2));

            playerData.setIsRebound(true);
            playerData.setColliderVec(impluse.x, impluse.y);

            // let bPlayerID: number = data.getArg(3);
            // if( PlayerMgr.IsValid(bPlayerID))
            // {
            //     GameLog.PrintDebug(bPlayerID, "-", "执行-碰撞-", frameID);

            //     let bPlayerData: PlayerData = PlayerMgr.GetPlayer(bPlayerID);

            //     let impluse: cc.Vec2 = cc.v2(data.getArg(5), data.getArg(6));

            //     bPlayerData.setIsRebound(true);
            //     bPlayerData.setColliderVec(impluse.x, impluse.y);
            // }
        }
        else
        {
            GameLog.PrintDebug(playerID, "-", "忽略-碰撞-", frameID);
        }
    }

    protected IsCanHandle(playerID: number, cmd: UserCmdType, frameID: number): boolean
    {
        if(PlayerMgr.IsValid(playerID))
        {
            return true;
        }
        return false;
    }
}