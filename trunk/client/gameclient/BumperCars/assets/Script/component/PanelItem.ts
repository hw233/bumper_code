import EngineUtil from '../common/EngineUtil';
import MiniGameTableInfo from '../table/MiniGameTableInfo';
import GameTable from '../manager/TableManger';
import { TableType } from '../table/TableDefine';
import GameJump from '../GameJump';
import Socket_SendMgr from '../network/socket/Socket_SendManager';
import ComponentBase from '../common/ComponentBase';
import { MessageType } from '../common/EventCenter';
import GameLog from '../Log/LogTool';
const {ccclass, property} = cc._decorator;

@ccclass
export default class PanelItem extends ComponentBase
{
    @property(Boolean)
    private isCutName: boolean = true

    @property(cc.Node)
    private imgSp: cc.Node = null

    private gameID: number = null;
    private taskSoleID: number = null;

    private isRegister: boolean = null;

    protected onLoad() 
    {
        this.isRegister = false;
        EngineUtil.RegTouchEndEvent(this.imgSp, this.onBtnClickItem.bind(this));
    }

    public initView(gameID: number, taskSoleID: number = null) 
    {
        this.gameID = gameID;

        this.taskSoleID = taskSoleID;

        let gameTableInfo: MiniGameTableInfo = GameTable.GetInstance().GetDataInfo(TableType.TTD_MiniGameType, gameID);
        if(gameTableInfo)
        {
            this.setGameName(gameTableInfo.GetName());
            if(CC_WECHATGAME)
            {
                EngineUtil.LoadRemoteImg(this.imgSp, gameTableInfo.GetImgUrl());
            }
        }

        if(this.taskSoleID !== null)
        {
            if(this.isRegister === false)
            {
                this.isRegister = true;
                this.registerEvent(MessageType.MiniProgramTryPlaySuc, this.tryPlaySuc.bind(this));
            }
        }
    }

    private setGameName(name) 
    {
        let nameNode = this.node.getChildByName("name");
        if(nameNode)
        {
            let newName = name;
            if(this.isCutName)
            {
                if(name.length > 4) 
                {
                    newName = `${name.substring(0 , 4)}...`;
                }
            }
            nameNode.getComponent(cc.Label).string = newName;
        }
    }

    //执行跳转逻辑
    public onBtnClickItem() 
    {
        let self = this;

        GameJump.JupmGame(this.gameID);
    }

    //试玩成功
    private tryPlaySuc(gameID: number)
    {
        GameLog.PrintLog("tryPlaySuc");

        if(this.gameID === gameID)
        {
            setTimeout(() => {
                Socket_SendMgr.GetTaskSend().SendTryGameMessage(this.taskSoleID);
            }, 1000);
        }
    }
}
