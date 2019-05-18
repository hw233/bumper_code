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

import FSPClient from './FSPClient';
import GameConfig from '../GameConfig';
import GameLog from '../Log/LogTool';
import PlayerMgr from '../manager/PlayerMgr';
import PlayerOpData from '../PlayerOpData';
import { FSPFrame } from './FSPData';
import { Msg_Walk_Type } from '../network/socket/MsgID';
import { UserCmdType } from '../GameDefine';
import MessageUtil from '../network/socket/MessageUtil';
import CommandMgr from './CommandMgr';
import CommandBase from './CommandBase';
import GlobalVar from '../GlobalVar';

/**
 * 存储本地对战是发的网络包 用于本地对战兼容
 */
class SendFrameManager
{
    private m_SendFrameList: object               = null;

    constructor()
    {
        this.init();
    }

    public init()
    {
        this.m_SendFrameList = {};
    }

    public addSendFrame(playerID: number, frame: FSPFrame)
    {
        if(!this.m_SendFrameList[playerID])
        {
            this.m_SendFrameList[playerID] = [];
        }

        this.m_SendFrameList[playerID].push(frame);
    }

    public forEach(callback: Function)
    {
        for(var key in this.m_SendFrameList)
        {
            callback && callback(key, this.m_SendFrameList[key]);
        }
    }

    public clearAll()
    {
        for(var key in this.m_SendFrameList)
        {
            if(this.m_SendFrameList[key])
            {
                this.m_SendFrameList[key].length = 0;
            }
        }
    }

    public destroy()
    {
        this.clearAll();
        this.m_SendFrameList = null;
    }
}

class FrameManager
{
    private m_FrameList: Array<any> = null;

    private m_IsHandledRelive: boolean = null;

    constructor()
    {
        this.init();
    }

    public init()
    {
        if(!this.m_FrameList)
        {
            this.m_FrameList = [];
        }

        this.m_FrameList.length = 0;
        this.m_IsHandledRelive = false;
    }

    public addRecvFrame(frame: FSPFrame): void
    {
        this.m_FrameList.push(frame);
    }

    //清理碰撞的后续操作帧
    public clearCollideAfterFrame(playerID: number)
    {
        for(let i = 0, len =  this.m_FrameList.length; i < len; i ++)
        {
            let frame: FSPFrame = this.m_FrameList[i];

            if(frame.getPlayerID() == playerID && (frame.getCmd() == Msg_Walk_Type._WALK_ACTION_USER_CMD || frame.getCmd() == Msg_Walk_Type._WALK_ACTION_CONTINUE_PREVIOUS_CMD) )
            {
                GameLog.PrintLog(frame.getPlayerID(), "-", "清理-", frame.getFrameID());
                frame.setIsDeleted(true);
            }
        }
    }

    
    //处理特殊情况(晚进去的玩家进入游戏后有人死亡)
    public HandleReliveFrame()
    {
        // if(this.m_IsHandledRelive)
        // {
        //     return;
        // }

        // let flags = {};
        // this.forEach( (key: number, frame: FSPFrame)=>
        // {
        //     if(flags[key] === true)
        //     {
        //         GameLog.PrintLog(frame.getPlayerID(), "-", "清理-", frame.getFrameID());
        //         frame.setIsDeleted(true);
        //     }

        //     if(frame.getCmd() == Msg_Walk_Type._WALK_ACTION_CONFUSION_RELIVE_CMD || frame.getCmd() == Msg_Walk_Type._WALK_ACTION_TEAM_RELIVE_CMD)
        //     {
        //         flags[key] = true;
        //     }
        // } );

        // this.m_IsHandledRelive = true;
    }
    
    //倒序遍历
    public forEach(callback: Function)
    {
        for(let i = this.m_FrameList.length - 1; i >= 0; i --)
        {
            let frame: FSPFrame = this.m_FrameList[i];
            callback(frame.getPlayerID(), frame);
        }
    }

    //遍历并且移除
    public forPop(callback: Function)
    {

        let stopRun: boolean = false;

        //遍历
        for( let i: number = 0, len = this.m_FrameList.length; i < len;  i ++ )
        {
            let frame: FSPFrame = this.m_FrameList[i];
            if(frame.getIsCanRun() && stopRun === false)
            {
                if(frame.getIsDeleted() === false)
                {
                    callback(frame.getPlayerID(), frame);
                }

                frame.setIsDeleted(true);
            }
            else
            {
                stopRun = true;
                frame.setIsCanRun(true);
            }
        }

        //删除
        for( let i = 0; i < this.m_FrameList.length; )
        {
            let frame: FSPFrame = this.m_FrameList[i];

            if(frame.getIsDeleted())
            {
                this.m_FrameList.splice(i, 1);

                FSPFramePool.PutFrame(frame);
            }
            else
            {
                i ++;
            }
        }
    }

    public destroy()
    {
        this.m_FrameList.length = 0;
        this.m_FrameList = null;
    }
}
 
export class FSPFramePool
{
    private static _pool: Array<FSPFrame> = null;

    private static initCount: number = 30;

    public static Init()
    {
        this._pool = [];

        for(let i = 0; i < this.initCount; i ++)
        {
            this._pool.push( new FSPFrame() );
        }
    }

    public static GetFrame()
    {
        let last = this._pool.length - 1;
        if (last < 0) 
        {
            return new FSPFrame();
        }
        else 
        {
            // Pop the last object in pool
            let frame = this._pool[last];
            this._pool.length = last;

            frame.init();

            return frame;
        }
    }

    public static PutFrame(frame: FSPFrame)
    {
        if(this._pool)
        {
            this._pool.push(frame);
        }
    }

    public static Destroy()
    {
        if(this._pool)
        {
            for(let key in this._pool)
            {
                this._pool[key].destroy();
            }
            this._pool.length = 0;
            this._pool = null;
        }
        FSPFrame.CtorCount = 0;
    }
}

export default class FSPManager{

    //唯一实例
    private static instance: FSPManager     = null;

    private m_FSPClient: FSPClient          = null;
    private m_FrameMgr: FrameManager        = null;

    private m_SendMgr: SendFrameManager     = null;

    private m_Running: boolean              = null;

    //是否为PVP模式
    private isPVP: Boolean                  = null;

    private m_Accumulator: number           = null;
    private m_MaxAccumulator: number        = null;
    private m_Fixed_Time_Step: number       = null;

    private m_ReboundFrameCount: number     = null;
    private m_ReboundFrameList: Object     = null;

    private m_IsOpen: boolean = null;

    private m_IsQuickRun: boolean = null;

    private m_LocalInterval = null;

    constructor()
    {
        this.m_Running          = false;
        this.isPVP              = false;
        this.m_IsQuickRun       = true;
        this.m_Accumulator      = 0;
        this.m_MaxAccumulator   = 1 / 5;
        
        this.m_Fixed_Time_Step  = GameConfig.CLogicFPS;

        this.m_ReboundFrameList = {};
        this.m_ReboundFrameCount = 5;

        this.m_FrameMgr         = new FrameManager();
        this.m_FSPClient        = new FSPClient();

        this.m_SendMgr          = new SendFrameManager();

        this.m_IsOpen = false;
    }

    public static GetInstance(): FSPManager
    {
        if(this.instance === null){
            this.instance = new FSPManager();
        }

        return this.instance;
    }

    public static Destroy()
    {
        this.instance.Destroy();

        this.instance = null;
    }

    //初始化
    public Init(): void
    {
        FSPFramePool.Init();

        this.m_FrameMgr.init();
        this.m_SendMgr.init();

        this.m_FSPClient.SetFrameListener(this.OnFrameListener.bind(this));
    }

    //进入游戏调用
    public Start(ispvp: boolean): void
    {
        this.m_Running = true;
        this.m_IsQuickRun = false;
        this.isPVP = ispvp;
        // this.m_FrameMgr.HandleReliveFrame();
    }

    //游戏结束调用
    public Over(): void
    {
        this.m_Running = false;
        this.m_IsQuickRun = true;

        clearInterval(this.m_LocalInterval);

        this.m_LocalInterval = null;
    }

    public Destroy()
    {
        this.m_FSPClient.Destroy();
        this.m_FrameMgr.destroy();
        this.m_SendMgr.destroy();

        this.m_FSPClient = null;
        this.m_FrameMgr = null;
        this.m_SendMgr = null;
        this.m_ReboundFrameList = null;

        CommandMgr.Destroy();

        FSPFramePool.Destroy();
    }

    //发送用户操作
    public SendPlayerOp(cmd: UserCmdType, data: PlayerOpData): void
    {
        GameLog.PrintDebug("FSPManager: 发送: CMD = ", cmd, ", PlayerID = ", data.getPlayerID());

        if(this.IsRunning() === false)
        {
            return;
        }

        if(this.isPVP)
        {
            this.m_FSPClient.SendFSP(cmd, data);
        }
        else
        {
            this.SendPlayerOPForPVE(cmd, data);
        }
    }

    //监听帧
    public OnFrameListener(frameID: number, frame: FSPFrame): void
    {
        frame.setIsQuickRun(this.m_IsQuickRun);
        this.AddServerFrame(frame);
    }

    public AddServerFrame(frame: FSPFrame): void
    {
        frame.Print();

        this.m_FrameMgr.addRecvFrame(frame);
    }

    //用于本地兼容
    private SendPlayerOPForPVE(cmd: UserCmdType, data: PlayerOpData)
    {
        let playerID: number = data.getPlayerID();

        let frame: FSPFrame = FSPFramePool.GetFrame();
        frame.copoyArgs(data.getArgs());
        frame.setCmd(cmd);
        frame.setCustom(data.getCustom());
        frame.setPlayerID(playerID);

        this.m_SendMgr.addSendFrame(playerID, frame);

        if(cmd === UserCmdType.UCT_Collide)
        {
            this.m_ReboundFrameList[playerID] = this.m_ReboundFrameCount;
        }

        if(this.m_IsOpen === false)
        {
            this.m_IsOpen = true;
            this.m_LocalInterval = setInterval(this.PVELogic.bind(this), GameConfig.SLogicFPS);
        }
    }

    private EnterFrame(dt: number): void
    {
        this.m_FrameMgr.forPop( (key: number, frame: FSPFrame) =>
        {
            this.ExecuteFrame(dt, frame.getFrameID(), frame);
        } );
    }

    //由外界驱动
    public FrameOnMove(dt: number): void
    {
        this.m_Accumulator += (dt * 1000);

        while (this.m_Accumulator >= this.m_Fixed_Time_Step) 
        {
            this.EnterFrame(this.m_Fixed_Time_Step);

            this.m_Accumulator -= this.m_Fixed_Time_Step;
        }        
    }

    public ClearCollideAfterFrameForPlayer(playerID: number)
    {
        // this.m_FrameMgr.clearCollideAfterFrame(playerID);
    }

    //执行每一帧
    public ExecuteFrame(dt: number, frameID: number, frame: FSPFrame): boolean
    {
        if(this.IsRunning() === false)
        {
            return false;
        }

        if(!frame)
        {
            return false;  
        }

        let cmd: number = frame.getCmd();

        let playerOpData: PlayerOpData = PlayerOpData.Get();
        playerOpData.setCustom(frame.getCustom());
        playerOpData.setCMD(cmd);
        playerOpData.setNullOp(frame.getIsNull());
        playerOpData.setPlayerID(frame.getPlayerID());
        playerOpData.setTimeStamp(frame.getTimeStamp());
        playerOpData.copyArgs(frame.getArgs());

        GlobalVar.LatestFrameTimeStamp = frame.getTimeStamp();

        let command: CommandBase = CommandMgr.GetInstance().GetCommand(cmd);
        if(command)
        {
            command.DoExecute(dt, frameID, cmd, playerOpData, frame.getIsQuickRun());
        }

        PlayerOpData.Put(playerOpData);

        return true;
    }

    //模拟服务器操作 用于本地对战的兼容
    private PVELogic()
    {
        if(this.m_Running === false)
        {
            return;
        }

        let self = this;
        let index: number = 0;
        let data: object = {};

        //遍历所有玩家,如果玩家没有操作,添加空桢
        this.m_SendMgr.forEach(function(key: number, list: Array<FSPFrame>){
            
        });

        //把所有玩家的操作打包发给客户端
        this.m_SendMgr.forEach(function(key: number, list: Array<FSPFrame>){
            if(!list)
            {
                return;
            }
            if(GameConfig.PhysicsSwitch === false)
            {
                if(list.length <= 0 && PlayerMgr.IsActive(key))
                {
                    let frame: FSPFrame = null;
    
                    if(self.m_ReboundFrameList[key])
                    {
                        frame = self.BuildReboundFrame(key);
                    }
                    else
                    {
                        frame = self.BuildNullFrame(key);
                    }
    
                    list.push(frame);
                }
            }

            if(list.length > 0)
            {
                for(let i = 0, len = list.length; i < len; i ++)
                {
                    let frame: FSPFrame = list[i];
                    if(frame)
                    {
                        let mkey: string = "member_" + index ++;

                        data[mkey] = {};
                        data[mkey].time_stamp = 0;
                        data[mkey].target_id = key;
                        data[mkey].action = frame.getCmd();

                        switch(frame.getCmd())
                        {
                            case Msg_Walk_Type._WALK_ACTION_USER_CMD:   //操作
                            {
                                data[mkey].point_start_x = MessageUtil.ConvertToInt(frame.getArg(0));
                                data[mkey].point_start_y = MessageUtil.ConvertToInt(frame.getArg(1));
                                break;
                            }
                            case Msg_Walk_Type._WALK_ACTION_CONTINUE_PREVIOUS_CMD: //空桢
                            {
                                break;
                            }
                            case Msg_Walk_Type._WALK_ACTION_USER_COLLIDE_CMD:   //碰撞
                            {
                                data[mkey].power_x = MessageUtil.ConvertToInt(frame.getArg(1));
                                data[mkey].power_y = MessageUtil.ConvertToInt(frame.getArg(2));
                                break;
                            }
                            case Msg_Walk_Type._WALK_ACTION_REBOUND_CMD: //反弹
                            {
                                break;
                            }
                            case Msg_Walk_Type._WALK_ACTION_CONFUSION_CAR_LEV_CHANGE: //玩家车辆等级升级-[大乱斗]
                            {
                                data[mkey].fight_lev = frame.getArg(0);
                                break;
                            }
                            case Msg_Walk_Type._WALK_ACTION_TEAM_CAR_LEV_CHANGE: //玩家车辆等级升级-[红蓝战队]
                            {
                                data[mkey].car_lev = frame.getArg(0);
                                break;
                            }
                        }
                    }
                    FSPFramePool.PutFrame(frame);
                }
            }
        });

        //清空帧
        this.m_SendMgr.clearAll()

        this.m_FSPClient.OnFSPListener(data);
    }

    public IsRunning(): boolean
    {
        return this.m_Running;
    }

    public setIsQuickRun(isQuickRun: boolean)
    {
        this.m_IsQuickRun = isQuickRun;
    }

    //构建一个空桢
    private BuildNullFrame(playerID: number): FSPFrame
    {
        GameLog.PrintDebug("FSPManager:BuildNullFrame");

        let frame: FSPFrame = FSPFramePool.GetFrame();
        frame.setCmd(Msg_Walk_Type._WALK_ACTION_CONTINUE_PREVIOUS_CMD);
        frame.setPlayerID(playerID);

        return frame;
    }

    //构建一个反弹帧
    private BuildReboundFrame(playerID: number): FSPFrame
    {
        GameLog.PrintDebug("FSPManager:BuildReboundFrame");

        if(this.m_ReboundFrameList[playerID] && this.m_ReboundFrameList[playerID] > 0)
        {
            this.m_ReboundFrameList[playerID] -= 1;

            let frame: FSPFrame = FSPFramePool.GetFrame();
            frame.setCmd(Msg_Walk_Type._WALK_ACTION_REBOUND_CMD);
            frame.setPlayerID(playerID);
    
            return frame;
        }
        else
        {
            GameLog.PrintLog("BuildReboundFrame error: ", "this.m_ReboundFrameList[playerID] = ", this.m_ReboundFrameList[playerID]);
        }
        return null;
    }
}