import { MsgID, Msg_SignIn_Type, Msg_Action_Type, Msg_Task_Type, Msg_UserAttrib_Type, Msg_Common_Type } from './MsgID';
import { Msg_Bettle_Type } from "./MsgID";
import { Msg_Item_Type } from "./MsgID";
import { Msg_Vas_Type } from "./MsgID";
import SocketNetManager from "./SocketNetManager";
import GlobalVar from "../../GlobalVar";
import PlayerMgr from "../../manager/PlayerMgr";
import TaskTableInfo from '../../table/TaskTableInfo';
import GameTable from '../../manager/TableManger';
import { TableType } from '../../table/TableDefine';

//模拟服务器消息
export default class TestMessage{

    private static id = null;

    public static HandleMessage(msgID, data){
        if(this.id == null){
            this.id = 0;
        }

        switch(msgID){
            case MsgID.MSG_LOGIN:       //登陆数据
            {
                let msgData = this.fillData(msgID, null, {
                    result: 1,
                });

                SocketNetManager.GetInstance().onMessage({data: msgData});

                let day1 = 1 << 0;
                let day2 = 1 << 1;
                let day3 = 1 << 2;
                let day4 = 0 << 3;
                let day5 = 0 << 4;
                let day6 = 0 << 5;
                let day7 = 0 << 6;
                let box1 = 1 << 7;
                let box2 = 0 << 8;
                let box3 = 0 << 9;
                let box4 = 0 << 10;

                let signIn_mask = day1 | day2 | day3 | day4 | day5 | day6 | day7 | box1 | box2 | box3 | box4;

                let msgData2 = this.fillData(MsgID.MSG_USERINFO, null, {
                    result: 1,
                    granding_id: 15,
                    kill_sum: 1000,
                    gold_sum: 100000,
                    cur_star_sum: 3,
                    unlock_cars: "1-2-3",
                    granding_num: 19951214,
                    nick_name: "水门",
                    head_picurl: "",
                    use_car: 1,
                    star_sum: 2,
                    gift_sum: 0,
                    king_sum: 1,
                    signIn_mask: signIn_mask,
                    online_remain_ts: 18000,
                });

                SocketNetManager.GetInstance().onMessage({data: msgData2});
                break;
            }

            case MsgID.MSG_BATTLE:      //战斗数据
            {
                switch(data.action){
                    case Msg_Bettle_Type._BATTLE_ACTION_CREATE_ROOM:   //创建房间
                    {
                        let msgData = this.fillData(msgID, data.action, {
                            action: Msg_Bettle_Type._BATTLE_ACTION_CREATE_ROOM_SUC,
                            model: data.model,
                            result: 1,
                        });

                        SocketNetManager.GetInstance().onMessage({data: msgData});
                        break;
                    }
                    case Msg_Bettle_Type._BATTLE_ACTION_QUICK_JOIN:   //快速加入
                    {
                        let msgData = this.fillData(msgID, data.action, {
                            action: Msg_Bettle_Type._BATTLE_ACTION_QUICK_JOIN_SUC,
                            result: 1,
                        });

                        SocketNetManager.GetInstance().onMessage({data: msgData});
                        break;
                    }
                    case Msg_Bettle_Type._BATTLE_ACTION_QUICK_MATCH:              //快速匹配
                    {

                        let msgData = this.fillData(msgID, data.action, {
                            action: Msg_Bettle_Type._BATTLE_ACTION_QUICK_MATCH_SUC,
                            result: 1,
                        });

                        SocketNetManager.GetInstance().onMessage({data: msgData});
                        break;
                    }
                    case Msg_Bettle_Type._BATTLE_ACTION_BATTLE_KILL:   //有人死了
                    {
                        let msgData = this.fillData(msgID, data.action, {
                            action: Msg_Bettle_Type._BATTLE_ACTION_BATTLE_KILL_SUC,
                            power: 579232,
                            killer_id: 579233,
                            result: 1,
                        });

                        SocketNetManager.GetInstance().onMessage({data: msgData});
                        break;
                    }
                    case Msg_Bettle_Type._BATTLE_ACTION_BATTLE_RELIVE:   //有人复活
                    {
                        let msgData = this.fillData(msgID, data.action, {
                            action: Msg_Bettle_Type._BATTLE_ACTION_BATTLE_RELIVE_SUC,
                            reliver_id: 579232,
                            result: 1,
                        });

                        SocketNetManager.GetInstance().onMessage({data: msgData});
                        break;
                    }
                }
                break;
            }

            case MsgID.MSG_VAS:         //buff商城
            {
                switch(data.action){
                    case Msg_Vas_Type.BUY:
                    {
                        let msgData = this.fillData(msgID, data.action, {
                            action: data.action,
                            result: 1,
                            type: data.type,
                        });

                        SocketNetManager.GetInstance().onMessage({data: msgData});
                        break;
                    }
                }
                break;
            }

            case MsgID.MSG_ITEM_ATTR:   //
            {
                switch(data.action){
                    case Msg_Item_Type._ITEM_ACTION_CREATE_RANDOM_ITEM:
                    {
                        this.id ++;
                        let msgData = this.fillData(msgID, data.action, {
                            action: data.action,
                            result: 1,
                            point_x: data.pos_x,
                            point_y: data.pos_y,
                            type: 1,
                            id: this.id,
                        });

                        SocketNetManager.GetInstance().onMessage({data: msgData});
                        break;
                    }
                    case Msg_Item_Type._ITEM_ACTION_PICKUP_RANDOM_ITEM:       //采集矿石
                    {
                        let msgData1 = this.fillData(msgID, data.action, {
                            action: data.action,
                            result: 1,
                            id: data.id,
                        });

                        SocketNetManager.GetInstance().onMessage({data: msgData1});

                        let playerData = PlayerMgr.GetPlayer(GlobalVar.userID);

                        let msgData2 = this.fillData(MsgID.MSG_BATTLE, null, {
                            action: Msg_Bettle_Type._BATTLE_ACTION_BATTLE_SYN_SCORE,
                            result: 1,
                            score: playerData.getScore() + 1000,
                            level: playerData.getLevel() + 1,
                            kill_num: playerData.getKillNum() + 1,
                            target_id: GlobalVar.userID,
                            overlord_id: 579232,
                        });

                        SocketNetManager.GetInstance().onMessage({data: msgData2});
                        break;
                    }
                }
                break;
            }

            case MsgID.MSG_WALK:       //同步位置
            {
                let msgData = this.fillData(msgID, null, {
                    target_id: GlobalVar.userID,

                    point_x: data.point_x,
                    point_y: data.point_y,

                    point_start_x: data.point_start_x,
                    point_start_y: data.point_start_y,

                    point_end_x: data.point_end_x,
                    point_end_y: data.point_end_y,
                });

                setTimeout(function(){
                    SocketNetManager.GetInstance().onMessage({data: msgData});
                }, 100);

                break;
            }

            case MsgID.MSG_SIGNIN:
            {
                switch(data.action){
                    case Msg_SignIn_Type._SIGNIN_ACTION_SIGNIN:
                    {
                        let action = Msg_SignIn_Type._SIGNIN_ACTION_SIGNIN_SUC;
                        let msgData = this.fillData(msgID, data.action, {
                            action: action,
                            dayindex: data.dayindex,
                        });

                        SocketNetManager.GetInstance().onMessage({data: msgData});
                        break;
                    }
                }
                break;
            }

            case MsgID.MSG_ACTION:
            {
                switch(data.action){
                    case Msg_Action_Type._ACTION_SIGNIN_BOX_RECEIVE:
                    {
                        let action = Msg_Action_Type._ACTION_SIGNIN_BOX_RECEIVE_SUC;
                        let msgData = this.fillData(msgID, data.action, {
                            action: action,
                            idx: data.idx,
                        });

                        SocketNetManager.GetInstance().onMessage({data: msgData});
                        break;
                    }
                    case Msg_Action_Type._ACTION_ONLINE_AWARD:
                    {
                        let action1 = Msg_UserAttrib_Type._USERATTRIB_ONLINE_REMAIN_TIME;
                        let msgData1 = this.fillData(MsgID.MSG_USERATTRIB, data.action, {
                            action: action1,
                            attrib_data: 18000,
                        });
                        SocketNetManager.GetInstance().onMessage({data: msgData1});

                        let action = Msg_Action_Type._ACTION_ONLINE_AWARD_SUC;
                        let msgData = this.fillData(msgID, data.action, {
                            action: action,
                        });

                        SocketNetManager.GetInstance().onMessage({data: msgData});
                        break;
                    }
                    case Msg_Action_Type._ACTION_ONLINE_SPEED_UP:
                    {
                        let action1 = Msg_UserAttrib_Type._USERATTRIB_ONLINE_REMAIN_TIME;
                        let msgData1 = this.fillData(MsgID.MSG_USERATTRIB, data.action, {
                            action: action1,
                            attrib_data: 0,
                        });
                        SocketNetManager.GetInstance().onMessage({data: msgData1});


                        let action = Msg_Action_Type._ACTION_ONLINE_SPEED_UP_SUC;
                        let msgData = this.fillData(msgID, data.action, {
                            action: action,
                        });

                        SocketNetManager.GetInstance().onMessage({data: msgData});
                        break;
                    }
                }
                break;
            }

            case MsgID.MSG_TASKDETAIL:
            {
                switch(data.action){
                    case Msg_Task_Type._TASK_DETAIL_LIST:
                    {
                        let len: number = GameTable.GetInstance().GetDataCount(TableType.TTD_TaskType);
                        let soleID: number = 1111;
                        for(let i = 0; i < len;i ++)
                        {
                            let taskinfo: TaskTableInfo = GameTable.GetInstance().GetDataInfoByIndex(TableType.TTD_TaskType, i);
                            let action1 = Msg_Task_Type._TASK_DETAIL_ADD;
                            let msgData1 = this.fillData(msgID, action1, {
                                action: action1,
                                id: soleID ++,
                                task_id: taskinfo.GetID(),
                                complete_num: 1,
                                state: 2,
                                type: 1,
                            });
                            SocketNetManager.GetInstance().onMessage({data: msgData1});
                        }
                        let len2: number = GameTable.GetInstance().GetDataCount(TableType.TTD_TryTaskType);

                        for(let i = 0; i < len2;i ++)
                        {
                            let taskinfo: TaskTableInfo = GameTable.GetInstance().GetDataInfoByIndex(TableType.TTD_TryTaskType, i);
                            let action2 = Msg_Task_Type._TASK_DETAIL_ADD;
                            let msgData2 = this.fillData(msgID, action2, {
                                action: action2,
                                id: soleID ++,
                                task_id: taskinfo.GetID(),
                                complete_num: 1,
                                state: 1,
                                type: 2,
                            });
                            SocketNetManager.GetInstance().onMessage({data: msgData2});
                        }
                        let action3 = Msg_Task_Type._TASK_DETAIL_LIST_SUC;
                        let msgData3 = this.fillData(msgID, action3, {
                            action: action3,
                        });
                        SocketNetManager.GetInstance().onMessage({data: msgData3});

                        break;
                    }

                    case Msg_Task_Type._TASK_DETAIL_AWARD:
                    {
                        let action = Msg_Task_Type._TASK_DETAIL_AWARD_SUC;
                        // let type = 0;
                        // if(data.type == 2)
                        // {
                        //     type = 3;
                        // }
                        // else if(data.type == 3)
                        // {
                        //     type = 4;
                        // }
                        let msgData = this.fillData(msgID, action, {
                            action: action,
                            type: data.type,
                            id: data.id,
                        });
                        SocketNetManager.GetInstance().onMessage({data: msgData});
                        break;
                    }
                }
                break;
            }

            case MsgID.MSG_BUMPER_TRANSFER:
            {
                switch(data.action)
                {
                    case Msg_Common_Type._BUMPER_TRANSFER_UNLOCK_CAR:
                    {
                        let action = Msg_Common_Type._BUMPER_TRANSFER_UNLOCK_CAR_SUC;
                        let msgData = this.fillData(msgID, action, {
                            action: action,
                            data:{
                                car_id: data.car_id,
                                status: -1,
                                gold_num: 1000,
                            }
                        });
                        SocketNetManager.GetInstance().onMessage({data: msgData});
                        break;
                    }
                }
                break;
            }
        }
    }

    private static fillData(msgID, msgType, data){
        let strData = '';

        strData += msgID;
        strData += "0000";
        
        strData += JSON.stringify(data);

        return strData;
    }
};