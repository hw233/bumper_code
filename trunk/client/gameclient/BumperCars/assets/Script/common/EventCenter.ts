
//事件类型定义
export const MessageType =
{
    Logic_FPS_End               : "Logic_FPS_End",                  //逻辑帧更新结束

    Login_Success               : "Login_Success",                  //登陆成功

    CarBtnClicked               : "CarBtnClicked",
    CarLibBtnClicked            : "CarLibBtnClicked",
    CarLibMotionClicked         : "CarLibMotionClicked",

    Game_On_Hide_Event          : "Game_On_Hide_Event",
    Game_On_Show_Event          : "Game_On_Show_Event",

    GameLoopBtn_Show_Event      : "GameLoopBtn_Show_Event",         //显示游戏圈按钮
    GameLoopBtn_Hide_Event      : "GameLoopBtn_Hide_Event",         //隐藏游戏圈按钮

    PING_VALUE_CHANGE_EVENT     : "PING_VALUE_CHANGE_EVENT",

    StartScene_RefreshCarList   : "StartScene_RefreshCarList",
    StartScene_RefreshTopView   : "StartScene_RefreshTopView",
    StartScene_RefreshTimeLimit : "StartScene_RefreshTimeLimit",

    CarLib_RefreshView          : "CarLib_RefreshView",

    NewMessage_Task             : "NewMessage_Task",                //任务新消息
    NewMessage_SignIn           : "NewMessage_SignIn",              //签到新消息

    MiniProgramToSuc            : "MiniProgramToSuc",               //小游戏跳转成功
    MiniProgramTryPlaySuc       : "MiniProgramTryPlaySuc",          //小游戏试玩成功

    TimeLimit_Refresh_Event     : "TimeLimit_Refresh_Event",        //限时免费车辆刷新

    Online_Reward_Time_Update   : "Online_Reward_Time_Update",      //在线奖励时间更新
    Online_Recv_Reward_Event    : "Online_Recv_Reward_Event",       //在线奖励领取
    Online_Speed_Up_Event       : "Online_Speed_Up_Event",          //在线奖励时间加速
    Online_Can_Receive_Event    : "Online_Can_Receive_Event",       //在线奖励可以领取


    Skin_TryUse_Suc_Event       : "Skin_TryUse_Suc_Event",          //皮肤试用成功


    Receive_User_Info           : "Receive_User_Info",              //接受到用户得信息
    Update_Camera_Pos           : "Update_Camera_Pos",              //更新相机位置
    Start_Game_Time_Count       : "Start_Game_Time_Count",          //开始游戏计时
    Refresh_Game_Time           : "Refresh_Game_Time",              //更新游戏时间
    Stop_Game_Time_Count        : "Stop_Game_Time_Count",           //停止更新游戏时间

    Get_Friend_List_Event       : "Get_Friend_List_Event",          //获取好友列表成功的消息
    Friend_Apply_Event          : "Friend_Apply_Event",             //好友申请的消息
    Game_Invite_Event           : "Game_Invite_Event",              //游戏邀请的消息
    Add_Friend_Event            : "Add_Friend_Event",               //添加好友的消息
    Refresh_Friend_Info_Event   : "Refresh_Friend_Info_Event",      //刷新好友信息




    /***************道具系统事件定义---Start********************/
    Item_Add_Item_Event         : "Item_Add_Item_Event",            //添加道具
    Item_Delete_Item_Event      : "Item_Delete_Item_Event",         //删除道具
    Item_Update_Item_Event      : "Item_Update_Item_Event",         //更新道具
    Item_Used_Item_Event        : "Item_Update_Used_Event",         //使用道具
    Item_Item_Expire_Event      : "Item_Item_Expire_Event",         //道具过期
    Item_Try_Suc_Event          : "Item_Try_Suc_Event",             //道具试用成功
    Item_Update_LiftTime_Event  : "Item_Update_LiftTime_Event",     //道具试用时间更新
    /***************道具系统事件定义---End********************/




    /***************任务系统事件定义---Start********************/
    Task_Add_Task_Event         : "Task_Add_Task_Event",            //添加一条任务
    Task_Get_TaskList_Event     : "Task_Get_TaskList_Event",        //任务列表获取成功
    Task_Update_Task_Event      : "Task_Update_Task_Event",         //更新任务
    Task_Receive_Reward_Event   : "Task_Receive_Reward_Event",      //领取任务
    Task_Try_Game_Event         : "Task_Try_Game_Event",            //试玩任务上报

    TaskView_RefreshView        : "TaskView_RefreshView",           //刷新任务界面
    /***************任务系统事件定义---End********************/


    /***************签到系统事件定义---Start********************/
    SignIn_Refresh_SignIn_View     : "SignIn_Refresh_SignIn_View",      //刷新签到项
    SignIn_Refresh_SignIn_Item     : "SignIn_Refresh_SignIn_Item",      //刷新签到项
    SignIn_Refresh_Box_Item        : "SignIn_Refresh_Box_Item",         //刷新签到项
    SignIn_Receive_Suc_Event       : "SignIn_Receive_Suc_Event",        //签到成功
    SignIn_Receive_Fail_Event      : "SignIn_Receive_Fail_Event",       //签到失败
    SignIn_Retroactive_Suc_Event   : "SignIn_Retroactive_Suc_Event",    //补签成功
    SignIn_Retroactive_Fail_Event  : "SignIn_Retroactive_Fail_Event",   //补签失败
    SignIn_Receive_Box_Suc_Event   : "SignIn_Receive_Box_Suc_Event",    //宝箱领取成功
    /***************签到系统事件定义---End********************/


    /***************好友系统事件定义---Start********************/
    Open_Friend_View        : "Open_Friend_View",                       //打开好友界面
    Refresh_Friend_Item     : "Refresh_Friend_Item",                    //刷新指定玩家的UI
    Refresh_Friend_View     : "Refresh_Friend_View",                    //刷新好友界面
    /***************好友系统事件定义---End********************/




    /***************网络消息事件定义---Start********************/
    CarLib_Unlock_Item_Suc           : "CarLib_Unlock_Item_Suc",            //解锁道具成功
    CarLib_Unlock_Item_Fail          : "CarLib_Unlock_Item_Fail",           //解锁道具失败

    Use_Car_Suc                     : "Use_Car_Suc",                        //使用车辆成功
    Use_Motion_Suc                  : "Use_Motion_Suc",                     //使用拖尾成功

    StartScene_GetRankList_Suc      : "StartScene_GetRankList_Suc",         //排行榜数据获取成功
    StartScene_GetRankList_Fail     : "StartScene_GetRankList_Fail",        //排行榜数据获取失败

    OverLayer_Get_Grading_Suc       : "OverLayer_Get_Grading_Suc",          //获取段位信息成功
    OverLayer_Get_Grading_Fail      : "OverLayer_Get_Grading_Fail",         //获取段位信息失败

    OverLayer_Keep_Star_Suc         : "OverLayer_Keep_Star_Suc",            //保星成功
    OverLayer_Keep_Star_Fail        : "OverLayer_Keep_Star_Fail",           //保星失败

    ComeOnView_Receive_Suc          : "ComeOnView_Receive_Suc",              //加油礼包领取成功
    ComeOnView_Receive_Fail         : "ComeOnView_Receive_Fail",             //加油礼包领取失败

    Player_Relive_Suc               : "Player_Relive_Suc",                  //有玩家复活成功

    DLD_Reconnect_Suc               : "DLD_Reconnect_Suc",          //重连成功
    DLD_Receive_Buy_Buff            : "DLD_Receive_Buy_Buff",       //接受到购买buff的消息
    DLD_Receive_Create_Room         : "DLD_Receive_Create_Room",    //接受到创建房间的消息
    DLD_Receive_Quick_Join          : "DLD_Receive_Quick_Join",     //接受到快速加入的消息
    DLD_Receive_Quick_Match         : "DLD_Receive_Quick_Match",    //接受到快速匹配的消息
    DLD_Player_Join_Game            : "DLD_Player_Join_Game",       //有玩家进入游戏的消息
    DLD_Join_Player                 : "DLD_Join_Player",            //加入一个玩家
    DLD_Leave_Player                : "DLD_Leave_Player",           //离开一个玩家
    DLD_Leader_Changed              : "DLD_Leader_Changed",         //接受到队长变更的消息
    DLD_Exit_Room                   : "DLD_Exit_Room",              //退出房间
    DLD_Syn_Player_List             : "DLD_Syn_Player_List",        //同步房间内的玩家列表
    DLD_Player_Exit_Game            : "DLD_Player_Exit_Game",       //DLD_Player_Exit_Game
    DLD_Bettle_Start                : "DLD_Bettle_Start",           //接受到战斗开始的消息
    DLD_Receive_Join                : "DLD_Receive_Join",           //接受到从邀请加入的链接进入房间的消息 -成功
    DLD_Receive_Join_Fail           : "DLD_Receive_Join_Fail",      //接受到从邀请加入的链接进入房间的消息 -失败
    DLD_Receive_Syn_Score           : "DLD_Receive_Syn_Score",      //接受到同步积分的消息
    DLD_Receive_Random_Point        : "DLD_Receive_Random_Point",   //随机一个点
    DLD_Recv_Generate_Item          : "DLD_Recv_Generate_Item",     //接受生成矿石的消息
    DLD_Recv_Pick_Item              : "DLD_Recv_Pick_Item",         //接受采集道具的消息
    DLD_Recv_Collider               : "DLD_Recv_Collider",          //接受到碰撞的消息
    DLD_Receive_Death               : "DLD_Receive_Death",          //接受到有人死亡的消息
    DLD_Receive_Relive              : "DLD_Receive_Relive",         //接受到有人复活的消息
    DLD_Receive_Bettle_End          : "DLD_Receive_Bettle_End",     //接受到战斗结束的消息
    DLD_Receive_Three_Reward        : "DLD_Receive_Three_Reward",   //接受到3倍奖励的消息
    DLD_Receive_Cancel_Quick_Join   : "DLD_Receive_Cancel_Quick_Join",//接受取消快速加入的消息
    /***************网络消息事件定义---End********************/





    /***************游戏中的事件定义---Start********************/
    Collider_Event          : "Collider_Event",
    Leave_Map               : "Leave_Map",
    Game_Over               : "Game_Over",
    Refresh_UI              : "Refresh_UI",
    Kill_Player             : "Kill_Player",
    Player_Death            : "Player_Death",
    Reset_Game              : "Reset_Game",
    HitWeaknesses           : "playWeaknesses",
    First_Collider          : "First_Collider",
    Drop_Map                : "Drop_Map",
    Return_HomePage         : "Return_HomePage",
    Keep_Star               : "Keep_Star",
    Self_Level_Changed      : "Self_Level_Changed", //自身等级改变
    /***************游戏中的事件定义---End********************/
};


export default class EventCenter
{
    private static eventTargetList = null;
    private static dispatchList = null;

    public static RegisterEvent(uuid: string, key: string, callback: Function){
        if(!uuid){
            return;
        }
        if(!key){
            return;
        }
        if(!callback){
            return;
        }
        if(!this.eventTargetList){
            this.eventTargetList = {};
        }
        if(!this.eventTargetList[uuid]){
            this.eventTargetList[uuid] = {};
        }
        if(!this.eventTargetList[uuid][key]){
            this.eventTargetList[uuid][key] = [];
        }

        this.eventTargetList[uuid][key].push(callback);
    }

    public static DispatchEvent(key: string, data = null){
        if(!key){
            return;
        }
        if(this.eventTargetList){

            for(let target in this.eventTargetList){
                if(this.eventTargetList[target]){
                    let eventList = this.eventTargetList[target]
                    if(eventList[key]){
                        let callbackArr = eventList[key];
                        if(callbackArr){
                            for(let i = 0;i < callbackArr.length;i ++){
                                callbackArr[i](data);   
                            }
                        }
                    }
                }
            }
        }
    }

    public static DispatchEventAsync(key: string)
    {
        if(!this.dispatchList){
            this.dispatchList = [];
        }

        if(!key){
            return;
        }

        this.dispatchList.push(key);
    }

    public static FrameOnMove(dt: number)
    {
        if(this.dispatchList && this.dispatchList.length > 0)
        {
            let len = this.dispatchList.length;
            for(let i = 0;i < len;i ++)
            {
                let key = this.dispatchList[i];
                this.DispatchEvent(key, null);
            }
            this.dispatchList.length = 0;
        }
    }

    public static RemoveEvent(uuid: string){
        if(!uuid){
            return;
        }
        if(this.eventTargetList && this.eventTargetList[uuid]){
            delete this.eventTargetList[uuid];
        }
    }

    public static RemoveEventByKey(uuid: string, key: string){
        if(!uuid){
            return;
        }
        if(!key)
        {
            return;
        }
        if(this.eventTargetList && this.eventTargetList[uuid]){
            if(this.eventTargetList[uuid][key])
            {
                delete this.eventTargetList[uuid][key];
            }
        }
    }

    public static ClearAll(){
        if(this.eventTargetList)
        {
            this.eventTargetList = null;
        }
        if(this.dispatchList)
        {
            this.dispatchList.length = 0;
            this.dispatchList = null;
        }
    }
};