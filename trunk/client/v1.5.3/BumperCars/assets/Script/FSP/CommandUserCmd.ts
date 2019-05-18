import CommandBase from './CommandBase';
import GameLog from '../Log/LogTool';
import GameLogic from '../GameLogic';
import PlayerData from '../data/PlayerData';
import PlayerMgr from '../manager/PlayerMgr';
import PlayerOpData from '../PlayerOpData';
import Util from '../common/Util';
import { PlayerState, UserCmdType } from '../GameDefine';

export default class CommandUserCmd extends CommandBase
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
            GameLog.PrintDebug(playerID, "-", "执行-操作-", frameID);

            let playerData: PlayerData = PlayerMgr.GetPlayer(playerID);

            let direction: cc.Vec2 = cc.v2(data.getArg(0), data.getArg(1));
    
            let rotate: number = Util.GetRotation(direction);
    
            playerData.setRotate(rotate);
            playerData.setCurLinearVeloctity(direction);

            GameLogic.GetInstance().DoMove(dt, playerID, frameID, cmd);

            //退出无敌
            GameLogic.GetInstance().ExitInvincible(playerID);

            GameLogic.GetInstance().CheckCollide(playerID, data.getTimeStamp(), isQuickRun);
        }
        else
        {
            GameLog.PrintDebug(playerID, "-", "忽略-操作-", frameID);
        }
    }

    protected IsCanHandle(playerID: number, cmd: UserCmdType, frameID: number): boolean
    {
        if(PlayerMgr.IsValid(playerID))
        {
            let playerData: PlayerData = PlayerMgr.GetPlayer(playerID);

            //只有当前是无敌状态或者是存活状态下才可以执行
            if(playerData.getState() == PlayerState.PS_Invincible || playerData.getState() == PlayerState.PS_Alive)
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