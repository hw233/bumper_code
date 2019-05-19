import CommandBase from './CommandBase';
import GameLog from '../Log/LogTool';
import GameLogic from '../GameLogic';
import PlayerData from '../data/PlayerData';
import PlayerMgr from '../manager/PlayerMgr';
import PlayerOpData from '../PlayerOpData';
import { UserCmdType } from '../GameDefine';

export default class CommandSynLevel extends CommandBase
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
            GameLog.PrintDebug(playerID, "-", "执行-等级同步-", frameID);

            let playerData:PlayerData = PlayerMgr.GetPlayer(playerID);

            if(playerData)
            {
                let level: number = parseInt(data.getArg(0));
                playerData.setLevel(level);

                GameLogic.GetInstance().DoSynLevel(playerID, frameID);
            }
        }
        else
        {
            GameLog.PrintDebug(playerID, "-", "忽略-等级同步-", frameID);
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