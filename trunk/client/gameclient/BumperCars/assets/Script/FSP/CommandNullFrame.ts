import CommandBase from './CommandBase';
import GameLog from '../Log/LogTool';
import GameLogic from '../GameLogic';
import PlayerData from '../data/PlayerData';
import PlayerMgr from '../manager/PlayerMgr';
import PlayerOpData from '../PlayerOpData';
import { PlayerState, UserCmdType } from '../GameDefine';

export default class CommandNullFrame extends CommandBase
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
            GameLog.PrintDebug(playerID, "-", "执行-空桢-", frameID);

            GameLogic.GetInstance().DoMove(dt, playerID, frameID, cmd);

            GameLogic.GetInstance().CheckCollide(playerID, data.getTimeStamp(), isQuickRun);
        }
        else
        {
            GameLog.PrintDebug(playerID, "-", "忽略-空桢-", frameID);
        }
    }

    protected IsCanHandle(playerID: number, cmd: UserCmdType, frameID: number): boolean
    {
        if(PlayerMgr.IsValid(playerID))
        {
            let playerData: PlayerData = PlayerMgr.GetPlayer(playerID);

            //活着的时候才能执行空桢
            if(playerData.getState() == PlayerState.PS_Alive)
            {
                if(playerData.m_Colliding <= 0)
                {
                    return true;
                }
            }
            else
            {
                return false;
            }
        }
        return false;
    }
}