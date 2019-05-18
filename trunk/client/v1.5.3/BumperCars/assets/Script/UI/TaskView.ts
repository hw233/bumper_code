import EngineUtil from '../common/EngineUtil';
import PopUpBase from './PopUpBase';
import TaskData from '../Task/TaskData';
import TaskItem from '../component/TaskItem';
import TaskManager from '../Task/TaskManager';
import { HashMap } from '../common/HashMap';
import TaskSystem from '../Task/TaskSystem';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import { MessageType } from '../common/EventCenter';
import { TaskType } from '../Task/TaskDefine';
import { AtlasType } from '../common/EngineUtil';
import AudioManager from '../common/AudioManager';
import GameUI from '../manager/UIManager';
import Util from '../common/Util';

const {ccclass, property} = cc._decorator;

@ccclass
export default class TaskView extends PopUpBase
{
    @property({type: cc.Node})
    private title: cc.Node = null;

    @property({type: cc.Node})
    private dailyTaskNode: cc.Node = null;

    @property({type: cc.Node})
    private tryPlayTaskNode: cc.Node = null;

    @property({type: cc.Node})
    private dailyTaskBtn: cc.Node = null;

    @property({type: cc.Node})
    private tryPlayTaskBtn: cc.Node = null;

    @property({type: cc.Node})
    private dailyTaskContent: cc.Node = null;

    @property({type: cc.Node})
    private tryPlayTaskContent: cc.Node = null;

    @property(cc.Prefab)
    private dailyTaskPrefab: cc.Prefab = null;

    @property(cc.Prefab)
    private tryPlayTaskPrefab: cc.Prefab = null;
    
    private taskType = null;

    private dailyTaskUIlist: HashMap<number, TaskItem> = null;
    private tryplayTaskUIlist: HashMap<number, TaskItem> = null;

    public OnInit () 
    {
        super.OnInit();

        this.taskType = TaskType.TT_DailyTask;

        this.dailyTaskUIlist = new HashMap<number, TaskItem>();
        this.tryplayTaskUIlist = new HashMap<number, TaskItem>();

        EngineUtil.RegTouchEndEvent(this.dailyTaskBtn, this.dailyTaskClicked.bind(this));
        EngineUtil.RegTouchEndEvent(this.tryPlayTaskBtn, this.tryPlayTaskClicked.bind(this));

        this.registerEvent(MessageType.Task_Get_TaskList_Event, this.OnGetTaskList.bind(this));
        this.registerEvent(MessageType.TaskView_RefreshView, this.refreshView.bind(this));
    }

    public InitView()
    {
        super.InitView();

        if(TaskSystem.GetInstance().IsGetTask())
        {
            this.initDailyTaskView();
            this.initTryPlayTaskView();
        }
        else
        {
            Socket_SendMgr.GetTaskSend().SendGetTaskListMessage();
        }
    }

    private initDailyTaskView()
    {
        let taskList: number[] = Util.CopyArray(TaskManager.GetInstance().GetDailyTaskList());

        this.sortTaskList(taskList);

        if(this.dailyTaskPrefab && this.dailyTaskContent)
        {
            let dailyTaskCount: number = taskList.length;
        
            for(let i = 0; i < dailyTaskCount; i ++)
            {
                let taskID: number = taskList[i];
                let taskData: TaskData = TaskManager.GetInstance().GetTaskData(taskID);
                if(taskData)
                {
                    let dailyTaskNode = cc.instantiate(this.dailyTaskPrefab);
                    if(dailyTaskNode)
                    {
                        this.dailyTaskContent.addChild(dailyTaskNode);
                        let taskItem: TaskItem = dailyTaskNode.getComponent("TaskItem");
                        if(taskItem)
                        {
                            taskItem.initView(TaskType.TT_DailyTask, taskData);
                            this.dailyTaskUIlist.Add(taskData.GetSoleID(), taskItem);
                        }
                    }   
                }
            }
        }
    }

    private initTryPlayTaskView()
    {
        let taskList: number[] = Util.CopyArray(TaskManager.GetInstance().GetTryPlayTaskList());

        this.sortTaskList(taskList);

        if(this.tryPlayTaskPrefab && this.tryPlayTaskContent)
        {
            let tryPlayCount: number = taskList.length;
        
            for(let i = 0; i < tryPlayCount; i ++)
            {
                let taskID: number = taskList[i];
                let taskData: TaskData = TaskManager.GetInstance().GetTaskData(taskID);
                if(taskData)
                {
                    let tryPlayTaskNode = cc.instantiate(this.tryPlayTaskPrefab);
                    if(tryPlayTaskNode)
                    {
                        this.tryPlayTaskContent.addChild(tryPlayTaskNode);
                        let taskItem: TaskItem = tryPlayTaskNode.getComponent("TaskItem");
                        if(taskItem)
                        {
                            taskItem.initView(TaskType.TT_TryPlayTask, taskData);
                            this.tryplayTaskUIlist.Add(taskData.GetSoleID(), taskItem);
                        }
                    }
                }
            }
        }
    }

    public RefreshView()
    {
        super.RefreshView();

        this.taskType = <TaskType>(this.param);

        this.refreshView();
    }

    private refreshView()
    {
        if(this.taskType === TaskType.TT_DailyTask)
        {
            this.dailyTaskBtn.getComponent(cc.Toggle).check();
            this.tryPlayTaskBtn.getComponent(cc.Toggle).uncheck();

            this.showDailyTask();
        }
        else
        {
            this.dailyTaskBtn.getComponent(cc.Toggle).uncheck();
            this.tryPlayTaskBtn.getComponent(cc.Toggle).check();

            this.showTryPlayTask();
        }
    }

    private OnGetTaskList(): void
    {
        this.initDailyTaskView();
        this.initTryPlayTaskView();

        this.RefreshView();
    }

    public OnDestroy()
    {
        super.OnDestroy();

        this.dailyTaskUIlist.Destroy();
        this.tryplayTaskUIlist.Destroy();
        this.dailyTaskUIlist = null;
        this.tryplayTaskUIlist = null;
    }

    private showDailyTask()
    {
        let len: number = this.dailyTaskUIlist.Size()
        for(let i = len - 1; i >= 0; i --)
        {
            let taskItem: TaskItem = this.dailyTaskUIlist.IndexOf(i);
            if(taskItem)
            {
                let taskData: TaskData = TaskManager.GetInstance().GetTaskData(taskItem.soleID);
                if(taskData)
                {
                    taskItem.refreshView(taskData);
                }
                else
                {
                    taskItem.node.destroy();
                    this.dailyTaskUIlist.Remove(taskItem.soleID);
                }
            }
        }
        this.dailyTaskNode.active = true;
        this.tryPlayTaskNode.active = false;

        EngineUtil.SetNodeTextureByType(AtlasType.AT_TaskLayer, this.title, "Task_MRRWTitle");
    }

    private showTryPlayTask()
    {
        let len: number = this.tryplayTaskUIlist.Size()
        for(let i = len - 1; i >= 0; i --)
        {
            let taskItem: TaskItem = this.tryplayTaskUIlist.IndexOf(i);
            if(taskItem)
            {
                let taskData: TaskData = TaskManager.GetInstance().GetTaskData(taskItem.soleID);
                if(taskData)
                {
                    taskItem.refreshView(taskData);
                }
                else
                {
                    taskItem.node.destroy();
                    this.tryplayTaskUIlist.Remove(taskItem.soleID);
                }
            }
        }
        this.dailyTaskNode.active = false;
        this.tryPlayTaskNode.active = true;

        EngineUtil.SetNodeTextureByType(AtlasType.AT_TaskLayer, this.title, "Task_SWYLTitle");
    }

    private sortTaskList(taskList: Array<number>)
    {
        if(taskList)
        {
            taskList.sort(function(value1: number, value2: number)
            {
                let taskData1: TaskData = TaskManager.GetInstance().GetTaskData(value1);
                let taskData2: TaskData = TaskManager.GetInstance().GetTaskData(value2);
                if(taskData1 && taskData2)
                {
                    let temp1: number = taskData1.GetTaskState() === 3 ? -1 : taskData1.GetTaskState();
                    let temp2: number = taskData2.GetTaskState() === 3 ? -1 : taskData2.GetTaskState();
                    return temp2 - temp1;
                }
                return 0;
            });
        }
    }

    private dailyTaskClicked()
    {
        this.taskType = TaskType.TT_DailyTask;

        this.showDailyTask();
    }

    private tryPlayTaskClicked()
    {
        this.taskType = TaskType.TT_TryPlayTask;

        this.showTryPlayTask();
    }

    protected closeBtnClicked()
    {
        AudioManager.PlayBtnSound();

        GameUI.RemoveUI(this.GetType());
    }
}
