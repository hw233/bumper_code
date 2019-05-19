import SceneMgr from '../manager/SceneMgr';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import WXHelper from '../common/WXHelper';

import { GameModel } from '../GameDefine';
import { MessageType } from '../common/EventCenter';
import { SceneType } from '../manager/SceneMgr';
import PopUpBase from './PopUpBase';
import GlobalVar from '../GlobalVar';

const {ccclass, property} = cc._decorator;

@ccclass
export default class CreateLayer extends PopUpBase
{
    @property({type: cc.Node})
    private createBtn: cc.Node = null

    @property({type: cc.Node})
    private joinBtn: cc.Node = null

    public OnInit () 
    {
        super.OnInit();

        this.registerTouchEndEvent(this.createBtn, this.createBtnClicked.bind(this));
        this.registerTouchEndEvent(this.joinBtn, this.joinBtnClicked.bind(this));

        this.registerEvent(MessageType.DLD_Receive_Create_Room, this.onCreateRoomResp.bind(this));
        this.registerEvent(MessageType.DLD_Receive_Quick_Join, this.onQuickJoinResp.bind(this));
    }

    public OnEnter()
    {
        super.OnEnter();

        GlobalVar.gameModel = <GameModel>(this.param);
    }

    public OnExit()
    {
        super.OnExit();
    }

    //点击创建按钮
    private createBtnClicked()
    {
        //发送创建房间的消息
        Socket_SendMgr.GetBettleSend().SendCreateRoomMessage();
    }

    //点击加入按钮
    private joinBtnClicked()
    {
        //发送快速加入的消息
        Socket_SendMgr.GetBettleSend().SendQuickJoinMessage();
    }
    
    //接受到创建房间的消息
    private onCreateRoomResp(msg)
    {
        if(msg.model == GameModel.GM_DLDMode)
        {
            //大乱斗
            SceneMgr.SwitchScene(SceneType.DLDRoomScene);
        }else if(msg.model == GameModel.GM_DQZMode)
        {
            //组队战
            SceneMgr.SwitchScene(SceneType.ZDZRoomScene);
        }
    }

    //接受到快速进入的消息
    private onQuickJoinResp(msg){
        if(msg.model == GameModel.GM_DLDMode){
            //大乱斗
            SceneMgr.SwitchScene(SceneType.DLDRoomScene);
        }else if(msg.model == GameModel.GM_DQZMode){
            //组队战
            SceneMgr.SwitchScene(SceneType.ZDZRoomScene);
        }
    }
}