import CommandBase from './CommandBase';
import GameLog from '../Log/LogTool';
import PlayerData from '../data/PlayerData';
import PlayerMgr from '../manager/PlayerMgr';
import PlayerOpData from '../PlayerOpData';
import { UserCmdType, PlayerState } from '../GameDefine';
import GameData from '../data/GameData';
import EventCenter from '../common/EventCenter';
import { MessageType } from '../common/EventCenter';

export default class CommandDeath extends CommandBase
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
            GameLog.PrintDebug(playerID, "-", "执行-死亡-", frameID);

            let playerData:PlayerData = PlayerMgr.GetPlayer(playerID);

            if(playerData)
            {
                let killerID: number = parseInt(data.getArg(0)) || 0;   //击杀者
                let goldNum: number = parseInt(data.getArg(1)) || 0;
                let scoreNum: number = parseInt(data.getArg(2)) || 0;
        
                if(PlayerMgr.IsSelf(playerID))
                {
                    //如果死者是我,并且不是自杀的话， 记录击杀者
                    GameData.GetBettleData().setEnemyUserID(killerID);
                }
        
                if(PlayerMgr.IsSelf(killerID) && GameData.GetBettleData().getEnemyUserID() === playerID)
                {
                    //击杀者是我,死者是我的仇人，复仇成功,清除仇人ID
                    GameData.GetBettleData().setEnemyUserID(0);
                }
        
                if(PlayerMgr.IsValid(playerID))
                {
                    //改变状态为死亡状态
                    let playerData = PlayerMgr.GetPlayer(playerID);
                    playerData.setState(PlayerState.PS_WaitDeath);
                }
        
                EventCenter.DispatchEvent(MessageType.DLD_Receive_Death, {
                    power: playerID,
                    killerID: killerID,
                    goldNum: goldNum,
                    scoreNum: scoreNum,
                });
            }
        }
        else
        {
            GameLog.PrintDebug(playerID, "-", "忽略-死亡-", frameID);
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