import GameTable from '../manager/TableManger';
import { TableType } from '../table/TableDefine';
import OtherGamesTableInfo from '../table/OtherGamesTableInfo';
import EngineUtil from '../common/EngineUtil';
import GameConfig from '../GameConfig';
import GameLog from '../Log/LogTool';
import GameUI from '../manager/UIManager';

const {ccclass, property} = cc._decorator;

@ccclass
export default class RecommendGame extends cc.Component
{
    @property(cc.Node)
    private layout: cc.Node = null;

    @property(cc.Prefab)
    private itemPrefab: cc.Prefab = null;

    onLoad()
    {
        let count: number = GameTable.GetInstance().GetDataCount(TableType.TTD_OtherGames);

        for(let i = 0; i < count; i ++)
        {
            let gameInfo: OtherGamesTableInfo = GameTable.GetInstance().GetDataInfoByIndex(TableType.TTD_OtherGames, i);
            if(gameInfo)
            {
                let itemNode: cc.Node = cc.instantiate(this.itemPrefab);
                if(itemNode)
                {
                    this.layout.addChild(itemNode);

                    this.initItem(itemNode, gameInfo);
                }
            }
        }
    }

    private initItem(itemNode: cc.Node, gameInfo: OtherGamesTableInfo)
    {
        itemNode.userID = gameInfo.GetID();

        let iconNode: cc.Node = itemNode.getChildByName("icon");
        let gameNameNode: cc.Node = itemNode.getChildByName("name");

        let newName = `${gameInfo.GetGameName().substring(0 , 4)}...`;

        EngineUtil.SetNodeText(gameNameNode, newName);
        EngineUtil.SetNodeTexture(iconNode, "othergame/" + gameInfo.GetIconImg());

        itemNode.on(cc.Node.EventType.TOUCH_END, this.itemClicked.bind(this));
    }

    private itemClicked(event)
    {
        let target: cc.Node = event.target;
        if(target)
        {
            this.createImage(target.userID);
        }
        // AdvertiseMgr.DestroyBannerAd();
    }

    private createImage(gameID)
    {
        let gameInfo: OtherGamesTableInfo = GameTable.GetInstance().GetDataInfo(TableType.TTD_OtherGames, gameID);
        if(gameInfo)
        {
            let fileName: string = GameConfig.RemoteAddr + "/image/" + gameInfo.GetGameImg();

            if( CC_WECHATGAME )
            {
                wx.previewImage({
                    urls: [fileName],

                    success: function()
                    {
                        GameLog.PrintLog("RecommendGame createImage success");
                    },

                    fail: function()
                    {
                        GameLog.PrintLog("RecommendGame createImage fail");
                    }
                })
            }
        }
    }
}
