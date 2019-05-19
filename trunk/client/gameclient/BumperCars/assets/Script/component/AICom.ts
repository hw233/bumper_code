import CarUtil from "../CarUtil";


import Util from "../common/Util";
import BevNodeNonePrioritySelector from "../AI/BevNodeNonePrioritySelector";
import BeSillyTerminal from "../AI/BeSillyTerminal";
import BeSillyPrecondition from "../AI/BeSillyPrecondition";
import ChasePrecondition from "../AI/ChasePrecondition";
import BevInputData from "../AI/BevInputData";
import BevOutputData from "../AI/BevOutputData";
import ChaseTerminal from "../AI/ChaseTerminal";

import PlayerMgr from "../manager/PlayerMgr";
import PlayerData from "../data/PlayerData";
import TimeCondition from './TimeCondition';
import GameConfig from '../GameConfig';
import AccountData from '../manager/AccountData';
import AITableInfo from '../table/AITableInfo';
import GameTable from '../manager/TableManger';

import { TableType } from "../table/TableDefine";
import { UserCmdType } from '../GameDefine';

const {ccclass, property} = cc._decorator;

@ccclass
export default class AICom extends cc.Component
{
    private m_UID: number                   = null;

    //地图根节点
    private m_GameMapNode: cc.Node          = null;

    //为调用者抛出AI操作的事件
    private m_OnVkey: Function              = null;

    private m_TimeCondition: TimeCondition  = null;

    //行为树输入数据
    private m_InputData: BevInputData       = null;

    //行为树输出数据
    private m_OutputData: BevOutputData     = null;

    //行为树根节点
    private m_BevTreeRoot: BevNodeNonePrioritySelector     = null;

    private m_IsStart: boolean = null;

    protected onLoad () 
    {
        this.m_IsStart = false;
        this.m_GameMapNode = cc.find('GameMap');

        this.m_TimeCondition = new TimeCondition();
        this.m_TimeCondition.SetConditionTime(GameConfig.SLogicFPS);

        this.init();
    }

    public setID(id: number): void
    {
        this.m_UID = id;
    }

    public init()
    {
        //构建AI行为树
        this.m_BevTreeRoot = new BevNodeNonePrioritySelector();

        //犯傻行为
        let beSillyTerminal = new BeSillyTerminal("BeSilly");
        //犯傻前提
        let beSillyPrecondition = new BeSillyPrecondition();
        //追赶行为
        let chaseTerminal = new ChaseTerminal("Chase");
        //追赶前提
        let chasePrecondition = new ChasePrecondition();

        beSillyTerminal.setPrecondition(beSillyPrecondition);
        chaseTerminal.setPrecondition(chasePrecondition);

        //先添加追赶行为(防止识别圈内没有目标的话就尴尬了)
        this.m_BevTreeRoot.addChild(chaseTerminal);
        this.m_BevTreeRoot.addChild(beSillyTerminal);

        let curGradingID: number = AccountData.GetInstance().GetCurLevelID();

        let aiInfo: AITableInfo = GameTable.GetInstance().GetDataInfo(TableType.TTD_AIType, curGradingID);
        if(aiInfo)
        {
            let playerData: PlayerData = PlayerMgr.GetPlayer(this.node.userID);
    
            let speed: number = CarUtil.CalcCarSpeeed(playerData.getCarID());
    
            let radius: number = aiInfo.GetAiradius();
    
            this.m_InputData = new BevInputData();
            this.m_InputData.setSelfLocation(this.node.getPosition());
            this.m_InputData.setSelfSize(this.getSelfSize());
            this.m_InputData.setSelfID(playerData.getUserID());
            this.m_InputData.setDelayTime(0);
            this.m_InputData.setMapNode(this.m_GameMapNode);
            this.m_InputData.setRandomNum(Util.RandomNum(1, 100));
            this.m_InputData.setIsBeSilly(false);
            this.m_InputData.setSillyValue(aiInfo.GetAis());
            this.m_InputData.setRadius(radius);
            this.m_InputData.setSpeed(speed);
    
            this.m_OutputData = new BevOutputData();
            this.m_OutputData.setLinearVelocity(playerData.getCurLinearVeloctity());
        }
    }

    public doStart()
    {
        this.m_IsStart = true;

        this.sendFirstWalkMessage();
    }

    //触发AI的移动
    public sendFirstWalkMessage()
    {
        this.m_OnVkey(this.m_UID, UserCmdType.UCT_Move, [0, 1]);
    }

    //遍历AI行为树
    private traverseBevTree(dt)
    {
        if(this.m_InputData){
            this.m_InputData.setSelfLocation(this.node.getPosition());
            this.m_InputData.setSelfSize(this.node.getContentSize());
            this.m_InputData.setDelayTime(dt);

            if( this.m_BevTreeRoot && this.m_BevTreeRoot.evaluate( this.m_InputData ) )
			{
				this.m_BevTreeRoot.tick( this.m_InputData, this.m_OutputData );
			}
        }
    }
    
    public setListener(listener: Function): void
    {
        this.m_OnVkey  = listener;
    }

    //获得自身的大小
    public getSelfSize()
    {
        return cc.size(this.node.width * this.node.scaleX, this.node.height * this.node.scaleY);
    }

    public frameOnMove(dt)
    {
        if(!this.m_IsStart)
        {
            return;
        }

        if(this.m_TimeCondition.IsMeetCondition() === false)
        {
            return;
        }
        
        let playerData = PlayerMgr.GetPlayer(this.node.userID);

        //反弹过程中不允许改变速度向量
        if(!playerData || playerData.getIsRebound())
        {
            return;
        }

        this.traverseBevTree(dt);

        let linearVelocity = this.m_OutputData.getLinearVelocity();

        this.m_OnVkey(this.node.userID, UserCmdType.UCT_Move, [linearVelocity.x, linearVelocity.y]);

        // // TODO
        // if(this.m_UID == GlobalVar.userID + 1)
        // {
        // this.m_OnVkey(this.node.userID, UserCmdType.UCT_Move, [1, 0]);

        // }
        // else if(this.m_UID == GlobalVar.userID + 2)
        // {
        // this.m_OnVkey(this.node.userID, UserCmdType.UCT_Move, [-1, 0]);
        // }
    }

    protected onDestroy()
    {
        this.m_GameMapNode = null;
        this.m_OnVkey = null;
        this.m_TimeCondition = null;

        if(this.m_InputData)
        {
            this.m_InputData.destroy();
            this.m_InputData = null;
        }

        if(this.m_OutputData)
        {
            this.m_OutputData.destroy();
            this.m_OutputData = null;
        }

        if(this.m_BevTreeRoot)
        {
            this.m_BevTreeRoot.destroy();
            this.m_BevTreeRoot = null;
        }

    }
}
