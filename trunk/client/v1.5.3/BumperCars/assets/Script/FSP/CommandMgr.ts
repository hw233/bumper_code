import CommandBase from "./CommandBase";
import CommandUserCmd from './CommandUserCmd';
import CommandNullFrame from "./CommandNullFrame";
import CommandCollide from "./CommandCollide";
import CommandRebound from "./CommandRebound";
import CommandRelive from "./CommandRelive";
import CommandSynLevel from './CommandSynLevel';
import CommandDeath from './CommandDeath';
import { UserCmdType } from '../GameDefine';

export default class CommandMgr
{
    private static instance: CommandMgr = null;

    private m_UserOpCmd: CommandBase    = null;
    private m_NullFrameCmd: CommandBase = null;
    private m_CollideCmd: CommandBase   = null;
    private m_ReboundCmd: CommandBase   = null;
    private m_ReliveCmd: CommandBase    = null;
    private m_SynLevelCmd: CommandBase  = null;
    private m_DeathCmd: CommandBase     = null;

    private constructor()
    {
        this.m_UserOpCmd = new CommandUserCmd();
        this.m_NullFrameCmd = new CommandNullFrame();
        this.m_CollideCmd = new CommandCollide();
        this.m_ReboundCmd = new CommandRebound();
        this.m_ReliveCmd = new CommandRelive();
        this.m_SynLevelCmd = new CommandSynLevel();
        this.m_DeathCmd = new CommandDeath();
    }

    public static GetInstance()
    {
        if(this.instance === null)
        {
            this.instance = new CommandMgr();
        }
        return this.instance;
    }

    public static Destroy()
    {
        if(this.instance)
        {
            this.instance.Destroy();
            this.instance = null;
        }
    }

    public Destroy()
    {
        this.m_UserOpCmd = null;
        this.m_NullFrameCmd = null;
        this.m_CollideCmd = null;
        this.m_ReboundCmd = null;
        this.m_ReliveCmd = null;
        this.m_SynLevelCmd = null;
        this.m_DeathCmd = null;
    }

    public GetCommand(cmdID: number): CommandBase
    {
        let command: CommandBase = null;
        switch(cmdID)
        {
            case UserCmdType.UCT_Move:   //操作
            {
                command = this.m_UserOpCmd;
                break;
            }
            case UserCmdType.UCT_Null: //空桢
            {
                command = this.m_NullFrameCmd;
                break;
            }
            case UserCmdType.UCT_Collide:   //碰撞
            {
                command = this.m_CollideCmd;
                break;
            }
            case UserCmdType.UCT_Rebound: //反弹
            {
                command = this.m_ReboundCmd;
                break;
            }
            case UserCmdType.UCT_DLD_Relive: //大乱斗复活
            {
                command = this.m_ReliveCmd;
                break;
            }
            case UserCmdType.UCT_DQZ_Relive: //夺旗战复活
            {
                command = this.m_ReliveCmd;
                break;
            }
            case UserCmdType.UCT_DLD_Syn_Level: //同步等级
            {
                command = this.m_SynLevelCmd;
                break;
            }
            case UserCmdType.UCT_DQZ_Syn_Level: //同步等级
            {
                command = this.m_SynLevelCmd;
                break;
            }
            case UserCmdType.UCT_Death:         //死亡
            {
                command = this.m_DeathCmd;
                break;
            }
            default:
            {
                break;
            }
        }
        return command;
    }
}