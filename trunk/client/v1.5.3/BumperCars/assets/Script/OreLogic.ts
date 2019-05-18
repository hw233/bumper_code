import ComponentBase from './common/ComponentBase';
import { MessageType } from './common/EventCenter';
import MapUtil from './MapUtil';
import GameLog from './Log/LogTool';
import Socket_SendMgr from './network/socket/Socket_SendManager';
import BettleData from './data/net/BettleData';
import GameData from './data/GameData';
import PlayerMgr from './manager/PlayerMgr';
import AudioManager from './common/AudioManager';
import { SoundType } from './common/AudioManager';
import OrePool from './OrePool';
import OreManager from './manager/OreManager';
import ItemCom from './component/ItemCom';
import GameLogic from './GameLogic';
import GlobalVar from './GlobalVar';
import OreScorePool from './OreScorePool';
import ItemScoreView from './layer/ItemScoreView';

const {ccclass, property} = cc._decorator;

@ccclass
export default class OreLogic extends ComponentBase {

    @property(OreScorePool)
    private oreScorePool: OreScorePool = null; //道具分数对象池

    @property(OrePool)
    private orePool: OrePool = null; //道具对象池

    @property(OreManager)
    private oreMgr: OreManager = null; //道具管理类

    private oreScoreParent: cc.Node = null;

    private oreParent: cc.Node = null;

    onLoad () 
    {
        //接受到随机一个矿石位置的消息
        this.registerEvent(MessageType.DLD_Receive_Random_Point, this.randomPosResp.bind(this) );
        //接受到生成矿石的消息
        this.registerEvent(MessageType.DLD_Recv_Generate_Item, this.generateItemResp.bind(this) );
        //接受到采集矿石的消息
        this.registerEvent(MessageType.DLD_Recv_Pick_Item, this.pickItemResp.bind(this) );
    }
    
    public init()
    {
        this.oreParent = cc.find("GameMap/OtherNode");
        this.oreScoreParent = cc.find("Canvas/UILayer");

        this.orePool.init(this.oreParent);
        this.oreScorePool.init(this.oreScoreParent);

        this.oreMgr.setOrePool(this.orePool);
    }

    start () {

    }

    //收到随机一个位置的消息
    private  randomPosResp() 
    {
        let randomPoint: any = null;

        while (true) 
        {
            //在地图的活动区域内随机一个点
            randomPoint = MapUtil.RandomPointByActiveArea();
            if (randomPoint) 
            {
                break;
            } else 
            {
                GameLog.PrintLog("没找到生成矿石的点");
            }
        }

        //发送给服务器
        Socket_SendMgr.GetItemSend().SendGenerateItem(randomPoint.x, randomPoint.y, randomPoint.tag);
    }
 
    //收到生成矿石的消息
    private generateItemResp() {
        let bettleData: BettleData = GameData.GetBettleData();

        if (this.orePool && bettleData) 
        {
            let oreNode = this.orePool.GetNode();
            if (oreNode) 
            {
                let itemPos = bettleData.getLocation();
                let posTag = bettleData.getLocationTag();
                let id = bettleData.getID();
                let score = bettleData.getItemScore();

                oreNode.itemID = id;
                oreNode.state = 1;
                oreNode.tag = posTag;
                oreNode.score = score;

                oreNode.setPosition(itemPos);

                // if(!oreNode.parent)
                // {
                //     this.oreParent.addChild(oreNode);
                // }

                let itemCom: ItemCom = oreNode.getComponent("ItemCom");
                if (itemCom) 
                {
                    itemCom.init();
                    itemCom.setType(bettleData.getTypeID());
                    this.oreMgr.addOre(id, posTag, oreNode);
                }
            }
        }
    }
 
    //收到采集矿石的消息
    private pickItemResp() {
        let self = this;

        let pickerID = GameData.GetBettleData().getPickerID();
        let itemID = GameData.GetBettleData().getPickItemID();

        if(PlayerMgr.IsSelf(pickerID))
        {
            AudioManager.PlaySoundEffect(SoundType.ST_Pick_Item);

            let oreNode: cc.Node = this.oreMgr.getOreByID(itemID);
            if(oreNode)
            {
                //播放特效
                let itemScoreNode = this.oreScorePool.GetNode();
                if(itemScoreNode)
                {
                    itemScoreNode.setPosition(0, 0);
                    // this.oreScoreParent.addChild(itemScoreNode);
    
                    let itemScoreCom: ItemScoreView = itemScoreNode.getComponent("ItemScoreView");
                    if(itemScoreCom)
                    {
                        itemScoreCom.init();
                        itemScoreCom.playEffect(oreNode.score.toString(), function()
                        {
                            self.oreScorePool.PutNode(itemScoreNode);
                        });
                    }
                }
            }
        }

        this.oreMgr.removeOre(itemID);
    }

    // 检测是否吃到道具
    checkPickItem() 
    {
        let carNode = GameLogic.GetInstance().GetCarManager().getCarByID(GlobalVar.userID);
        if (!carNode) return;

        let CarCom = carNode.getComponent("CarCom");
        let colliderArea = CarCom.getCollideArea();

        let collideTag = carNode.collideTag;
        if(collideTag)
        {
            for(let i = 0; i < collideTag.length; i ++)
            {
                let tag = collideTag[i];
                let oreList = this.oreMgr.getOreListByTag(tag);
                if(oreList)
                {
                    for(let j = 0, len = oreList.length; j < len; j ++)
                    {
                        let itemID = oreList[j];
                        let itemNode = this.oreMgr.getOreByID(itemID);
                        if (itemNode && itemNode.state === 1) 
                        {
                            if (MapUtil.PointIsContactInCircle(itemNode.getPosition(), colliderArea.x, colliderArea.y, colliderArea.radius)) 
                            {
                                itemNode.state = 2;
                                Socket_SendMgr.GetItemSend().SendPickMessage(itemNode.itemID);
                            }
                        }
                    }
                }
            }
        }
    }

    public frameOnMove(dt: number)
    {
        //检测是否可以采集道具
        this.checkPickItem();
    }

    public clear()
    {
    }

    // update (dt) {}
}
