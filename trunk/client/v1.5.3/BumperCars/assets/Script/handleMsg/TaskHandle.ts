import HandleBase from './HandleBase';
import { MsgID, Msg_Task_Type } from '../network/socket/MsgID';
import EventCenter from '../common/EventCenter';
import { MessageType } from '../common/EventCenter';
import GameLog from '../Log/LogTool';
import TaskSystem from '../Task/TaskSystem';

export default class TaskHandle extends HandleBase
{
    constructor()
    {
        super();
    }

    public RegisterAllHandle()
    {
        this.RegisterHandle(MsgID.MSG_TASKDETAIL, Msg_Task_Type._TASK_DETAIL_ADD, this.OnAddTaskMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_TASKDETAIL, Msg_Task_Type._TASK_DETAIL_LIST_SUC, this.OnGetTaskListSucMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_TASKDETAIL, Msg_Task_Type._TASK_DETAIL_LIST_FAIL, this.OnGetTaskListFailMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_TASKDETAIL, Msg_Task_Type._TASK_DETAIL_UPDATE, this.OnUpdateTaskMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_TASKDETAIL, Msg_Task_Type._TASK_DETAIL_AWARD_SUC, this.OnReceiveSucMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_TASKDETAIL, Msg_Task_Type._TASK_DETAIL_AWARD_FAIL, this.OnReceiveFailMessage.bind(this));
        this.RegisterHandle(MsgID.MSG_TASKDETAIL, Msg_Task_Type._TASK_DETAIL_TRY_GAME_FAIL, this.OnTryGameFailMessage.bind(this));
    }

    //添加任务
    private OnAddTaskMessage(msg)
    {
        EventCenter.DispatchEvent(MessageType.Task_Add_Task_Event, {
            soleID: Number(msg.id),
            taskID: Number(msg.task_id),
            progress: Number(msg.complete_num),
            state: Number(msg.status),
            type: Number(msg.type),
        });
    }

    //获取好友列表成功
    private OnGetTaskListSucMessage()
    {
        TaskSystem.GetInstance().SetIsGetTask(true);

        EventCenter.DispatchEvent(MessageType.Task_Get_TaskList_Event);
    }

    //获取任务列表失败
    private OnGetTaskListFailMessage()
    {
        GameLog.PrintLog("获取任务列表失败");
    }

    //更新任务
    private OnUpdateTaskMessage(msg)
    {
        EventCenter.DispatchEvent(MessageType.Task_Update_Task_Event, {
            soleID: Number(msg.id),
            progress: Number(msg.complete_num),
            state: Number(msg.status),
        });
    }

    //领取奖励成功
    private OnReceiveSucMessage(msg)
    {
        EventCenter.DispatchEvent(MessageType.Task_Receive_Reward_Event, {
            soleID: Number(msg.id),
            type: Number(msg.type),
        });
    }

    //领取奖励失败
    private OnReceiveFailMessage()
    {
        GameLog.PrintLog("领取奖励失败");
    }

    //试玩任务上报失败
    private OnTryGameFailMessage()
    {
        GameLog.PrintLog("试玩任务上报失败");
    }
}