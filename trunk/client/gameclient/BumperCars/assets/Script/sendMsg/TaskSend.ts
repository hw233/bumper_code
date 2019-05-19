
import { MsgID, Msg_Task_Type } from '../network/socket/MsgID';
import SocketNetManager from "../network/socket/SocketNetManager";

export default class TaskSend
{
    /**
     * 获取任务列表
     */
    public SendGetTaskListMessage()
    {
        let data = 
        {
            action: Msg_Task_Type._TASK_DETAIL_LIST,
        };
        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_TASKDETAIL, data);
    }

    /**
     * 领取任务奖励
     * @param soleID 任务唯一ID
     * @param type 领取类型 0：首次领奖 1 : 再次领奖
     */
    public SendRecvRewardMessage(soleID: number, type: number)
    {
        let data = 
        {
            action: Msg_Task_Type._TASK_DETAIL_AWARD,
            id: soleID,
            type: type,
        };
        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_TASKDETAIL, data);
    }

    /**
     * 试玩任务上报
     * @param soleID 任务唯一ID
     */
    public SendTryGameMessage(soleID: number)
    {
        let data = 
        {
            action: Msg_Task_Type._TASK_DETAIL_TRY_GAME,
            id: soleID,
        };
        SocketNetManager.GetInstance().SendMsg(MsgID.MSG_TASKDETAIL, data);
    }
}
