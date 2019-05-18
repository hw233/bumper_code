import MessageUtil          from '../network/socket/MessageUtil';
import PlayerOpData         from '../PlayerOpData';
import Socket_SendMgr       from '../network/socket/Socket_SendManager';
import SocketNetManager     from '../network/socket/SocketNetManager';
import { FSPFrame }         from './FSPData';
import { Msg_Walk_Type }    from '../network/socket/MsgID';
import { UserCmdType }      from '../GameDefine';
import { FSPFramePool } from './FSPManager';
import GameLog from '../Log/LogTool';
import GameConfig from '../GameConfig';

export default class FSPClient
{
    private m_FrameID      : number     = null;
    private m_FrameListener: Function   = null;

    private m_Count: number = null;

    private m_PreTime: number = null;

    constructor()
    {
        this.m_Count = 0;
        this.init();
    }

    public init()
    {
        this.m_FrameID = 0;

        SocketNetManager.GetInstance().SendFPSListener(this.OnFSPListener.bind(this));
    }

    public SendFSP(cmd: UserCmdType, data: PlayerOpData)
    {
        if(cmd === UserCmdType.UCT_Move)
        {
            Socket_SendMgr.GetCommonSend().SendPlayerOp(data.getArgs(), data.getTimeStamp());
        }
        else if(cmd === UserCmdType.UCT_Collide)
        {
            Socket_SendMgr.GetCommonSend().SendCollideMessage(data.getArgs(), data.getTimeStamp());
        }
    }

    public OnFSPListener(data: any)
    {
        // if(this.m_PreTime == null)
        // {
        //     this.m_PreTime = Date.now();
        // }

        // let cur: number = Date.now();
        // console.debug(this.m_Count, cur, "OnFSPListener: interval = ", (cur - this.m_PreTime) );
        // this.m_Count ++;
        // this.m_PreTime = cur;


        for(let key in data)
        {
            let member = data[key];

            let frameID: number         = this.m_FrameID ++;
            let timestamp: number       = parseInt(member.ts);
            let playerID: number        = parseInt(member.target_id);
            let action: Msg_Walk_Type   = parseInt(member.action);
    
            let frame: FSPFrame = FSPFramePool.GetFrame();
            frame.setFrameID(frameID);
            frame.setPlayerID(playerID);
            frame.setCmd(action);
            frame.setTimeStamp(timestamp);

            switch(action)
            {
                case Msg_Walk_Type._WALK_ACTION_USER_CMD:               //操作
                {
                    frame.setIsNull(false);
                    frame.addArg(MessageUtil.ConvertToFloat(member.point_start_x));
                    frame.addArg(MessageUtil.ConvertToFloat(member.point_start_y));

                    break;
                }
                case Msg_Walk_Type._WALK_ACTION_CONTINUE_PREVIOUS_CMD: //空桢
                {
                    frame.setIsNull(true);

                    break;
                }
                case Msg_Walk_Type._WALK_ACTION_USER_COLLIDE_CMD:       //碰撞
                {
                    frame.setIsNull(false);

                    frame.addArg(member.type);
                    frame.addArg(MessageUtil.ConvertToFloat(member.power_x));
                    frame.addArg(MessageUtil.ConvertToFloat(member.power_y));

                    // frame.addArg(member.becollide_id);

                    // frame.addArg(member.t_type);
                    // frame.addArg(MessageUtil.ConvertToFloat(member.t_power_x));
                    // frame.addArg(MessageUtil.ConvertToFloat(member.t_power_y));

                    break;
                }
                case Msg_Walk_Type._WALK_ACTION_REBOUND_CMD:            //反弹
                {
                    frame.setIsNull(false);
                    break;
                }
                case Msg_Walk_Type._WALK_ACTION_CONFUSION_RELIVE_CMD:   //复活-[大乱斗]
                {
                    frame.setIsNull(false);
                    frame.addArg(member.region_id);
                    frame.addArg(member.car_lev);
                    break;
                }
                case Msg_Walk_Type._WALK_ACTION_TEAM_RELIVE_CMD:        //复活-[红蓝战队]
                {
                    frame.setIsNull(false);
                    frame.addArg(member.region_id);
                    frame.addArg(member.car_lev);
                    break;
                }
                case Msg_Walk_Type._WALK_ACTION_CONFUSION_CAR_LEV_CHANGE:            //玩家车辆等级升级-[大乱斗]
                {
                    frame.setIsNull(false);
                    frame.addArg(member.fight_lev);
                    break;
                }
                case Msg_Walk_Type._WALK_ACTION_TEAM_CAR_LEV_CHANGE:            //玩家车辆等级升级-[红蓝战队]
                {
                    frame.setIsNull(false);
                    frame.addArg(member.car_lev);
                    break;
                }
                case Msg_Walk_Type._WALK_ACTION_KILL:            //玩家死亡
                {
                    frame.setIsNull(false);
                    frame.addArg(member.killer_id);
                    frame.addArg(member.award_gold);
                    frame.addArg(member.award_score);
                    break;
                }
            }
            this.m_FrameListener(frameID, frame);
        }

        //补帧
        for(let key in data)
        {
            let member = data[key];

            let timestamp: number       = parseInt(member.ts);
            let playerID: number        = parseInt(member.target_id);
            let action: Msg_Walk_Type   = parseInt(member.action);

            this.fillFrame(action, playerID, timestamp);
        }
    }

    public SetFrameListener(frameListener)
    {
        this.m_FrameListener = frameListener;
    }

    public Destroy()
    {
        this.m_FrameListener = null;
        SocketNetManager.GetInstance().SendFPSListener(null);
    }

    //补帧(只对改变位置的帧 进行补帧)
    private fillFrame(cmd: Msg_Walk_Type, playerID: number, timestamp: number)
    {
        if(GameConfig.FillFrameSwitch === false)
        {
            return;
        }

        if( cmd !== Msg_Walk_Type._WALK_ACTION_USER_CMD && cmd !== Msg_Walk_Type._WALK_ACTION_CONTINUE_PREVIOUS_CMD && cmd !== Msg_Walk_Type._WALK_ACTION_REBOUND_CMD )
        {
            //不需要补帧
            return;
        }

        //补帧id
        let frameID: number         = this.m_FrameID ++;

        //补帧
        let fillFrame: FSPFrame = FSPFramePool.GetFrame();
        fillFrame.setFrameID(frameID);
        fillFrame.setPlayerID(playerID);
        fillFrame.setTimeStamp(timestamp);
        fillFrame.setIsCanRun(false);

        switch(cmd)
        {
            case Msg_Walk_Type._WALK_ACTION_USER_CMD:               //操作
            {
                //收到操作帧 补一个空桢
                fillFrame.setIsNull(true);
                fillFrame.setCmd(Msg_Walk_Type._WALK_ACTION_CONTINUE_PREVIOUS_CMD);
                break;
            }
            case Msg_Walk_Type._WALK_ACTION_CONTINUE_PREVIOUS_CMD: //空桢
            {

                //收到空桢 再补一个空桢
                fillFrame.setIsNull(true);
                fillFrame.setCmd(Msg_Walk_Type._WALK_ACTION_CONTINUE_PREVIOUS_CMD);
                break;
            }
            case Msg_Walk_Type._WALK_ACTION_REBOUND_CMD:            //反弹
            {
                //收到反弹桢 再补一个反弹桢
                fillFrame.setIsNull(false);
                fillFrame.setCmd(Msg_Walk_Type._WALK_ACTION_REBOUND_CMD);
                break;
            }
        }

        GameLog.PrintDebug(playerID, ":补帧-", "CMD=", fillFrame.getCmd(), "-frameID=", frameID);

        this.m_FrameListener(frameID, fillFrame);
    }
}