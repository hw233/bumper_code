import PanelItem from './PanelItem';
import TaskData from '../Task/TaskData';
import RewardItem from './RewardItem';
import EngineUtil from '../common/EngineUtil';
import TaskManager from '../Task/TaskManager';
import GameUI from '../manager/UIManager';
import { UIType } from '../UI/UIDefine';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import { RewardData } from '../data/RewardData';
import { TaskType, TaskState } from '../Task/TaskDefine';
import { AtlasType } from '../common/EngineUtil';
import AdvertiseMgr from '../Advertise/AdvertiseMgr';
import { AdvertiseType } from '../Advertise/AdvertiseMgr';

const {ccclass, property} = cc._decorator;

@ccclass
export default class TaskItem extends cc.Component
{
    @property({type: PanelItem})
    private gameItem: PanelItem  = null;

    @property({type: cc.Node})
    private rewardNode: cc.Node = null;

    //任务按钮(去完成，领取奖励，再领一次，已完成四种展现方式)
    @property({type: cc.Node})
    private btn: cc.Node = null;

    @property(cc.Node)
    private taskName: cc.Node = null;

    @property(cc.Prefab)
    private rewardPrefab: cc.Prefab = null;

    private taskType: TaskType = null;

    public soleID: number = null;

    protected onLoad()
    {
        EngineUtil.RegBtnClickEvent(this.btn, this.btnClicked.bind(this));
    }

    //type = 1 试玩任务 type = 2 每日任务
    public initView(type: TaskType, data: TaskData)
    {
        this.taskType = type;
        this.soleID = data.GetSoleID();

        this.initRewardList(data.GetRewardList());
        this.refreshBtnState(data.GetTaskState());

        this.refreshView(data);
    }

    public refreshView(data: TaskData)
    {

        if(this.taskType === TaskType.TT_DailyTask)
        {
            let taskName = data.GetTaskDes() + "(" + data.GetTaskProgress() + "/" + data.GetTaskTarget() + ")";
            EngineUtil.SetNodeText(this.taskName, taskName);
        }
        else if(this.taskType === TaskType.TT_TryPlayTask)
        {
            if(this.gameItem)
            {
                this.gameItem.initView(data.GetCustom(), data.GetSoleID());
            }
        }

        this.refreshBtnState(data.GetTaskState());
    }

    //初始化奖励列表
    private initRewardList(rewardList: Array<RewardData>)
    {
        if(rewardList && this.rewardPrefab)
        {
            for(let i = 0, len = rewardList.length; i < len; i ++)
            {
                let reward: RewardData = rewardList[i];
                if(reward)
                {
                    let rewardView = cc.instantiate(this.rewardPrefab);
                    if(rewardView)
                    {
                        let rewardItem: RewardItem = rewardView.getComponent("RewardItem");
                        if(rewardItem)
                        {
                            rewardItem.initView(reward.ItemID, reward.ItemNum);
                        }
                        this.rewardNode.addChild(rewardView);
                    }
                }
            }
        }
    }

    private refreshBtnState(state: number)
    {
        if(this.taskType === TaskType.TT_TryPlayTask && state === TaskState.TS_ToFinish)
        {
            EngineUtil.SetNodeTextureByType(AtlasType.AT_TaskLayer, this.btn, "Task_SWBtn");
            return;
        }
        EngineUtil.SetNodeTextureByType(AtlasType.AT_TaskLayer, this.btn, "Task_BtnState_" + state);


        if(state === TaskState.Ts_Finished)
        {
            EngineUtil.SetButtonInteractable(this.btn, false);
        }
    }

    private btnClicked()
    {
        let taskData: TaskData = TaskManager.GetInstance().GetTaskData(this.soleID);
        if(taskData)
        {
            switch(taskData.GetTaskState())
            {
                case TaskState.TS_ToFinish:
                {
                    this.toFinish(taskData);
                    break;
                }
                case TaskState.TS_Receive:
                {
                    this.receive(taskData);
                    break;
                }
                case TaskState.TS_AgainReceive:
                {
                    this.againReceive(taskData);
                    break;
                }
            }
        }
    }

    //去完成
    private toFinish(taskData: TaskData)
    {
        if(this.taskType === TaskType.TT_DailyTask)
        {
            if(taskData.GetSkipViewID() === UIType.UT_TaskView)
            {
                GameUI.RefreshUI(UIType.UT_TaskView, taskData.GetSkipViewType());
            }
            else
            {
                GameUI.HideUI(UIType.UT_TaskView);
                GameUI.ShowUI(taskData.GetSkipViewID(), taskData.GetSkipViewType());
            }
        }
        else if(this.taskType === TaskType.TT_TryPlayTask)
        {
            // if(!CC_WECHATGAME)
            {
                if(this.gameItem)
                {
                    this.gameItem.onBtnClickItem();
                }
            }
            // else
            // {
            //     Socket_SendMgr.GetTaskSend().SendTryGameMessage(taskData.GetSoleID());
            // }
        }
    }

    //领取奖励
    private receive(taskData: TaskData)
    {
        Socket_SendMgr.GetTaskSend().SendRecvRewardMessage(taskData.GetSoleID(), 0);
    }

    //再领一次
    private againReceive(taskData: TaskData)
    {
        AdvertiseMgr.CreateVideoAd(AdvertiseType.ADConfig_Video, (isFinish)=>
        {
            if(isFinish)
            {
                Socket_SendMgr.GetTaskSend().SendRecvRewardMessage(taskData.GetSoleID(), 1);
            }
        });
    }
}
