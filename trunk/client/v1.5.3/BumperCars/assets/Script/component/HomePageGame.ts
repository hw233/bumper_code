import PanelItem from './PanelItem';
import GameLocalDataMgr from '../manager/GameLocalDataMgr';
import GameTable from '../manager/TableManger';
import { TableType } from '../table/TableDefine';
import MiniGameTableInfo from '../table/MiniGameTableInfo';
import GameConfig from '../GameConfig';
import ComponentBase from '../common/ComponentBase';
import { MessageType } from '../common/EventCenter';
const {ccclass, property} = cc._decorator;

@ccclass
export default class HomePageGame extends ComponentBase{
    
    @property(PanelItem)
    private panelItem: PanelItem        = null;

    private games: Array<number>        = null;

    private curIndex: number            = null;

    private timeout: any                = null;

    protected onLoad()
    {
        this.curIndex = 0;

        this.initGames();
        
        this.showGame();

        this.registerEvent(MessageType.MiniProgramToSuc, this.jumpSuc.bind(this))
    }

    private showGame()
    {
        clearTimeout(this.timeout);

        if(this.curIndex < 0 || this.curIndex >= this.games.length)
        {
            this.curIndex = 0;
        }
        if(this.games[this.curIndex])
        {
            let gameID: number = this.games[this.curIndex];

            if(this.panelItem)
            {
                this.panelItem.initView(gameID);
            }
        }
        this.curIndex ++;


        this.timeout = setTimeout(this.showGame.bind(this), GameConfig.HomePageGameTime * 1000);
    }

    private initGames()
    {
        GameLocalDataMgr.GetInstance().ReadHomePageGames();

        this.games = GameLocalDataMgr.GetInstance().GetHomePageGames();

        if(!this.games)
        {
            this.games = [];
        }

        if(this.games && this.games.length <= 0)
        {
            let count: number = GameTable.GetInstance().GetDataCount(TableType.TTD_MiniGameType);
            for(let i = 0;i < count; i ++)
            {
                let gameTableInfo: MiniGameTableInfo = GameTable.GetInstance().GetDataInfoByIndex(TableType.TTD_MiniGameType, i);
                if(gameTableInfo)
                {
                    this.games.push(gameTableInfo.GetID());
                }
            }

            GameLocalDataMgr.GetInstance().SetHomePageGames(this.games);
        }
    }

    private jumpSuc(gameID: number)
    {
        if(this.games)
        {
            for(let i = 0;i < this.games.length;i ++)
            {
                if(this.games[i] === gameID)
                {
                    this.games.splice(i, 1);
                    break;
                }
            }
    
            this.showGame();
    
            GameLocalDataMgr.GetInstance().SetHomePageGames(this.games);
        }
    }

    onDestroy()
    {
        this.games = null;

        clearTimeout(this.timeout);
    }
}
