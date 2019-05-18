import EngineUtil from '../common/EngineUtil';
import PlayerMgr from '../manager/PlayerMgr';
import PlayerData from '../data/PlayerData';
import ComponentBase from '../common/ComponentBase';
import { MessageType } from '../common/EventCenter';
import { AtlasType } from '../common/EngineUtil';
import GameRankItem from './GameRankItem';

const {ccclass, property} = cc._decorator;

@ccclass
export default class GameRankView extends ComponentBase{
    @property({type: cc.Node})
    private otherRankContent: cc.Node = null;

    @property({type: cc.Node})
    private selfRankNode: cc.Node = null;

    @property({type: cc.Prefab})
    private rankItemPrefab: cc.Prefab = null;

    @property({type: cc.Node})
    private btnNode: cc.Node = null;

    @property
    private initCount: number = 5;       //初始个数

    @property
    private maxCount: number = 9;      //最大展示的个数

    @property
    private spacingY: number = 0;

    private state: number = null;   //1：收回状态 2：展开状态

    //一项的高度
    private itemHeigth:number = null;

    //当前显示的个数
    private curShowCount: number = null;

    //所有的ui项
    private itemList: Array<cc.Node> = null;

    onLoad () {
        this.state = 1; //默认为收回的状态

        //有玩家退出游戏
        this.registerEvent(MessageType.DLD_Player_Exit_Game, this.playerExitGameResp.bind(this));

        EngineUtil.SetNodeTextureByType(AtlasType.AT_GameScene, this.btnNode, "xiala");

        EngineUtil.RegTouchEndEvent(this.btnNode, this.btnNodeClicked.bind(this));
    }

    public initLayer()
    {
        this.itemList = [];

        let playerCount: number = PlayerMgr.GetPlayerCount();
        //需要创建的个数
        let createCount: number = this.maxCount;

        if(playerCount < this.maxCount)
        {
            createCount = playerCount;
        }

        this.curShowCount = this.initCount;
        if(playerCount < this.initCount)
        {
            this.curShowCount = playerCount;
        }

        for(let i = 0; i < createCount; i ++)
        {
            let rank = i + 1;
            
            let playerData = PlayerMgr.GetPlayerByIndex(i);
            if(playerData)
            {
                let itemNode = this.addItemNode();
                if(itemNode)
                {
                    let rankItem: GameRankItem = itemNode.getComponent('GameRankItem');
                    if(rankItem)
                    {
                        if(PlayerMgr.IsSelf(playerData.getUserID()))
                        {
                            //初始化自己的数据
                            this.refreshSelfItem(rank, playerData);
                        }
                        rankItem.initView(rank, playerData);
                    }

                    itemNode.userID = playerData.getUserID();
                    itemNode.y = -20 - i * this.itemHeigth;

                    if( (i + 1) > this.curShowCount )
                    {
                        itemNode.x = 9999;
                    }
                    else
                    {
                        itemNode.x = 0;
                    }
                }
            }
        }

        this.refreshSelfItemPos();
    }

    //刷新自己那一项的位置
    private refreshSelfItemPos()
    {
        if(this.selfRankNode)
        {
            this.selfRankNode.y = (-23 - this.curShowCount * this.itemHeigth);
        }
    }

    //根据当前需要显示的个数 来更新排行榜位置
    private refreshRankItemPos()
    {
        if(this.itemList)
        {
            for(let i: number = 0, len = this.itemList.length; i < len; i ++)
            {
                let itemNode: cc.Node = this.itemList[i];
                if(itemNode)
                {
                    if( (i + 1) > this.curShowCount )
                    {
                        //不需要展示
                        itemNode.x = 9999;
                    }
                    else
                    {
                        itemNode.x = 0;
                    }
                    itemNode.y = - 20 - i * this.itemHeigth;
                }
            }
        }
    }

    //点击
    btnNodeClicked()
    {
        if( this.state === 1 )
        {
            this.unfoldRanklist();
        }else if(this.state === 2)
        {
            this.withDrawRanklist();
        }
    }

    //展开排行榜
    unfoldRanklist()
    {
        this.state = 2;

        let playerCount: number = PlayerMgr.GetPlayerCount();

        this.curShowCount = this.maxCount;
        if(playerCount < this.maxCount)
        {
            this.curShowCount = playerCount;
        }

        this.refreshRankItemPos();

        this.refreshSelfItemPos();

        EngineUtil.SetNodeTextureByType(AtlasType.AT_GameScene, this.btnNode, "shouqi");
    }

    //收回排行榜
    withDrawRanklist()
    {
        this.state = 1;

        let playerCount: number = PlayerMgr.GetPlayerCount();

        this.curShowCount = this.initCount;
        if(playerCount < this.initCount)
        {
            this.curShowCount = playerCount;
        }

        this.refreshRankItemPos();

        this.refreshSelfItemPos();

        EngineUtil.SetNodeTextureByType(AtlasType.AT_GameScene, this.btnNode, "xiala");
    }

    //根据玩家数据对所有item进行排序
    itemSort()
    {
        if(this.itemList && this.itemList.length > 0)
        {
            this.itemList.sort(function(aItem, bItem) 
            {
                let aData = PlayerMgr.GetPlayer(aItem.userID);
                let bData = PlayerMgr.GetPlayer(bItem.userID);
    
                if(aData && bData)
                {
                    return (bData.getScore() - aData.getScore());
                }
            })
        };
    }

    //刷新排行榜
    refreshRankView(){

        //数据更新 先排序
        this.itemSort();

        this.refreshRankItemPos();

        this.refreshSelfItemPos();
        
        if(this.itemList)
        {
            for(let i: number = 0, len = this.itemList.length; i < len; i ++)
            {
                let rank: number = i + 1;
                let itemNode: cc.Node = this.itemList[i];
                if(itemNode)
                {
                    let playerData: PlayerData = PlayerMgr.GetPlayer(itemNode.userID);
                    if(playerData)
                    {
                        if(PlayerMgr.IsSelf(itemNode.userID))
                        {
                            //刷新自己的数据
                            this.refreshSelfItem(rank, playerData);
                        }

                        if( (i + 1) <= this.curShowCount )
                        {
                            itemNode.getComponent("GameRankItem").refreshView(rank, playerData);
                        }
                    }
                }
            }
        }
    }


    private refreshSelfItem(rank: number, playerData: PlayerData)
    {
        //初始化自己的数据
        let selfItem = this.selfRankNode.getComponent("GameRankItem");
        if(selfItem)
        {
            selfItem.initView(rank, playerData);
        }
    }

    private removeItemNode(userID: number)
    {
        if(this.itemList)
        {
            for(let i: number = 0, len = this.itemList.length; i < len; i ++)
            {
                if(this.itemList[i].userID === userID)
                {
                    this.itemList[i].destroy();
                    this.itemList.splice(i, 1);

                    this.refreshRankItemPos();
                    this.refreshSelfItemPos();

                    break;
                }
            }
        }
    }

    private addItemNode(): cc.Node
    {
        let itemNode: cc.Node = this.createItemNode();
        if(itemNode)
        {
            if(this.itemHeigth === null)
            {
                this.itemHeigth = itemNode.height;
            }
            this.otherRankContent.addChild(itemNode);
            this.itemList.push(itemNode);
            return itemNode;
        }
        return null;
    }

    private createItemNode(): cc.Node
    {
        if(this.rankItemPrefab)
        {
            let rankItemNode = cc.instantiate(this.rankItemPrefab);
            if(rankItemNode)
            {
               return rankItemNode;
            }
        } 
        return null;
    }

    //收到玩家退出游戏的消息(按下返回按钮触发)
    private playerExitGameResp(playerID: number)
    {
        this.removeItemNode(playerID);
    }
}