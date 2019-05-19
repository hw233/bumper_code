import GameLogic from '../GameLogic';
import PlayerMgr from '../manager/PlayerMgr';
import SocketNetManager from '../network/socket/SocketNetManager';
import Util from '../common/Util';
import { Msg_Bettle_Type } from '../network/socket/MsgID';
import { Msg_Item_Type } from '../network/socket/MsgID';
import { Msg_Syn_Room_Type } from '../network/socket/MsgID';
import { MsgID } from '../network/socket/MsgID';


const {ccclass, property} = cc._decorator;

@ccclass

/**
 * 网络消息模拟界面
 */
export default class TestMsgLayer extends cc.Component
{
    @property(cc.Node)
    private testMsgLayer: cc.Node = null;

    @property(cc.Node)
    private content: cc.Node = null;

    private id: number = null;

    onLoad () {
        this.testMsgLayer.active = false;
        this.id = 579232;
        
        this.initTestMsgLayer();
    }

    initTestMsgLayer(){
        let self = this;

        let teamID = 1214;
        let teamType = 1;

        this.addTestMsg("玩家加入", function(){
            if(self.id == 579237){
                teamID = 1215;
                teamType = 2;
            }
            let data = {
                action: Msg_Syn_Room_Type.SYNROOMINFO_ACTION_ADD_MEMBER,
                uid: self.id,
                head_picurl: "",
                nick_name: "水门_" + self.id,
                use_car: 1,
                team_id: teamID,
                team_type: teamType,
            };
            self.id ++;
            let msgData = self.fillData(MsgID.MSG_SYN_ROOM, null, data);

            SocketNetManager.GetInstance().onMessage({data: msgData});
        });

        this.addTestMsg("玩家退出", function(){
            let data = {
                uid: 579233,
                result: 1,
                type: 2,
            };
            let msgData = self.fillData(MsgID.MSG_SYN_ROOM, null, data);

            SocketNetManager.GetInstance().onMessage({data: msgData});
        });

        this.addTestMsg("战斗开始的消息", function(){
            let data = {
                interval_time: 300,
                members: {
                    target_id: 579232,
                    region_id: 0,
                    kill_sum: 0,
                    fight_lev: 1,
                    direction_x: Math.random() * 10000,
                    direction_y: Math.random() * 10000,
                    fight_score: 1000,
                },
                members_1: {
                    target_id: 579233,
                    region_id: 1,
                    kill_sum: 0,
                    fight_lev: 1,
                    direction_x: Math.random() * 10000,
                    direction_y: Math.random() * 10000,
                    fight_score: 1000,
                },
                action: Msg_Bettle_Type._BATTLE_ACTION_BATTLE_START,
            };

            let msgData = self.fillData(MsgID.MSG_BATTLE, null, data);

            SocketNetManager.GetInstance().onMessage({data: msgData});
        });


        this.addTestMsg("模拟其他玩家操作的消息", function(){
            let count = PlayerMgr.GetPlayerCount();
            for(let i = 0; i < count; i ++){
                let player = PlayerMgr.GetPlayerByIndex(i);
                if(PlayerMgr.IsSelf(player.getUserID())){
                    continue;
                }
                self.schedule(function(){
                    self.simulatePlayerOp(player.getUserID());
                }, 0.5 + i / 10);
            }
        });

        this.addTestMsg("战斗结束的消息", function(){
            let data = {
                result: 1,
                action: Msg_Bettle_Type._BATTLE_ACTION_BATTLE_END_SUC,
                goldNum: 100,
                bzlpNum: 99,
                players: [
                ]
            };

            for(let i = 0;i < 15;i ++){
                data.players.push({
                    rank: i + 1,
                    head: "",
                    name: "水门_" + i,
                    killNum: 33,
                    scoreNum: 44,
                });
            }

            let msgData = self.fillData(MsgID.MSG_BATTLE, null, data);

            SocketNetManager.GetInstance().onMessage({data: msgData});
        });

        this.addTestMsg("生成矿石", function(){
            let data = {
                result: 1,
                action: Msg_Item_Type._ITEM_ACTION_CREATE_RANDOM_ITEM_POS,
            };

            self.schedule(function(){
                if(GameLogic.GetInstance().IsRunning() == false)return;

                let msgData = self.fillData(MsgID.MSG_ITEM_ATTR, null, data);
                SocketNetManager.GetInstance().onMessage({data: msgData});
            }, 1);
        });
    }

    fillData(msgID, msgType, data){
        let strData = '';

        strData += msgID;
        strData += '0000';

        strData += JSON.stringify(data);

        return strData;
    }

    //添加测试消息
    addTestMsg(title, callback){
        let node = new cc.Node();
        node.addComponent(cc.Label).string = title;
        node.on(cc.Node.EventType.TOUCH_END, callback);

        this.content.addChild(node);
    }

    //显示测试消息层
    ShowTestMsgLayer(){
        this.testMsgLayer.active = true;
    }

    //隐藏测试消息层
    HideTestMsgLayer(){
        this.testMsgLayer.active = false;
    }

    //模拟一个用户得操作
    simulatePlayerOp(userID){
        if(GameLogic.GetInstance().IsRunning() == false)return;

        let prePoint = this.getRandomPoint();
        let curPoint = this.getPoint(prePoint);

        let msgData = this.fillData(MsgID.MSG_WALK, null, {
            userID: userID,
            point_x: 0,
            point_y: 0,
            point_start_x: prePoint.x,
            point_start_y: prePoint.y,
            point_end_x: curPoint.x,
            point_end_y: curPoint.y,
        });

        SocketNetManager.GetInstance().onMessage({data: msgData});
    }

    //在一个区域内随机取得一个点
    getRandomPoint(){
        let minX = 200;
        let maxX = 500;
        let minY = 200;
        let maxY = 900;

        let x = Util.RandomNum(minX, maxX);
        let y = Util.RandomNum(minY, maxY);

        return cc.v2(x, y);
    }

    //在一个点周围随机取一个点
    getPoint(point){
        let points = Util.SurroundPoints(point.x, point.y, 100, 10);
        return Util.RandomElement(points)
    }

};


