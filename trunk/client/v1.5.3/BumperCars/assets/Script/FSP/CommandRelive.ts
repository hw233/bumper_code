import PlayerOpData from '../PlayerOpData';
import CommandBase from './CommandBase';
import PlayerData from '../data/PlayerData';
import PlayerMgr from '../manager/PlayerMgr';
import GameLogic from '../GameLogic';
import MapInfo from '../MapInfo';
import EventCenter, { MessageType } from '../common/EventCenter';
import { UserCmdType, PlayerState } from '../GameDefine';
import GameLog from '../Log/LogTool';

export default class CommandRelive extends CommandBase
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
            GameLog.PrintDebug(playerID, "-", "执行-复活-", frameID);

            let relivePosID: number = data.getArg(0);
            let level: number = data.getArg(1);

            let playerData: PlayerData = PlayerMgr.GetPlayer(playerID);
            if (playerData) 
            {
                if(level != null)
                {
                    playerData.setLevel(level); 
                }
                playerData.m_Colliding = 0;
                playerData.m_CollidePlayer = -1;

                playerData.setBirthPointID(relivePosID);

                let carNode = GameLogic.GetInstance().GetCarManager().getCarByID(playerID);
                if (carNode) 
                {
                    //所有的出生点
                    let BirthPointVec = MapInfo.GetCarPosVec();

                    //位置编号
                    let birthPointID: number = relivePosID;
                    let pos: any = BirthPointVec[birthPointID];

                    playerData.setPosition(pos);
                    
                    carNode.getComponent("CarCom").relive();

                    GameLogic.GetInstance().DoSynLevel(playerID, frameID);
                }

                if(PlayerMgr.IsSelf(playerID))
                {
                    GameLogic.GetInstance().isCanInput = false;
                    GameLogic.GetInstance().isSendDeath = false;
                }
            }
            EventCenter.DispatchEvent(MessageType.Player_Relive_Suc, playerID);
        }
        else
        {
            GameLog.PrintDebug(playerID, "-", "忽略-复活-", frameID);
        }
    }

    protected IsCanHandle(playerID: number, cmd: UserCmdType, frameID: number): boolean
    {
        if(PlayerMgr.IsValid(playerID))
        {
            //只有当前是死亡状态或者是无敌状态(晚进去别人死亡复活的情况)才可以进行复活
            if(true/*playerData.getState() == PlayerState.PS_Invincible || playerData.getState() == PlayerState.PS_HaveDied*/)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        return false;
    }
}