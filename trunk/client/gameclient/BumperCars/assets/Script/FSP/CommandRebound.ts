import CommandBase from './CommandBase';
import GameLog from '../Log/LogTool';
import GameLogic from '../GameLogic';
import PlayerData from '../data/PlayerData';
import PlayerMgr from '../manager/PlayerMgr';
import PlayerOpData from '../PlayerOpData';
import { UserCmdType } from '../GameDefine';

export default class CommandRebound extends CommandBase
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
            GameLog.PrintDebug(playerID, "-", "执行-反弹-", frameID);

            let playerData: PlayerData = PlayerMgr.GetPlayer(playerID);

            playerData.m_Colliding -= 1;

            if(playerData.m_Colliding === 0)
            {
                GameLog.PrintDebug(playerID, ":反弹结束");

                playerData.m_CollidePlayer = -1;
                playerData.setIsRebound(false);

                //如果自己反弹结束，则可以继续操作
                if(PlayerMgr.IsSelf(playerID))
                {
                    if(PlayerMgr.IsActive(playerID))
                    {
                        GameLogic.GetInstance().isCanInput = true;
                    }
                }
            }
    
            GameLogic.GetInstance().DoMove(dt, playerID, frameID, cmd);
    
            GameLogic.GetInstance().CheckCollide(playerID, data.getTimeStamp(), isQuickRun);
        }
        else
        {
            GameLog.PrintDebug(playerID, "-", "忽略-反弹-", frameID);
        }
    }

    protected IsCanHandle(playerID: number, cmd: UserCmdType, frameID: number): boolean
    {
        if(PlayerMgr.IsValid(playerID))
        {
            let playerData: PlayerData = PlayerMgr.GetPlayer(playerID);

            if(playerData)
            {
                if(PlayerMgr.IsSelf(playerID))
                {
                    if(GameLogic.GetInstance().isSendDeath === false)
                    {
                        return true;
                    }
                }
                else
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