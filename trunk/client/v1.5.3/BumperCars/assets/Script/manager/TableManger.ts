import { TableType } from '../table/TableDefine';
import BaseTableReader from '../table/BaseTableReader';
import { HashMap } from '../common/HashMap';
import BaseTableInfo from '../table/BaseTableInfo';
import { TaskTableReader } from '../table/TaskTableInfo';
import { ConfigTableReader } from '../table/ConfigTableInfo';
import { ItemTableReader } from '../table/ItemTableInfo';
import { AITableReader } from '../table/AITableInfo';
import { TryPlayTaskTableReader } from '../table/TryPlayTaskTableInfo';
import { CarTableReader } from '../table/CarTableInfo';
import { MotionTableReader } from '../table/MotionTableInfo';
import { MiniGameTableReader } from '../table/MiniGameTableInfo';
import { SignInTableReader } from '../table/SignInTableInfo';
import { BoxTableReader } from '../table/BoxTableInfo';
import { OnlineRewardTableReader } from '../table/OnlineRewardTableInfo';
import Util from '../common/Util';
import { OtherGamesTableReader } from '../table/OtherGamesTableInfo';

export default class GameTable
{
    private static instance: GameTable                                      = null;

    //存放所有数据
    private m_AllData: HashMap<TableType, Array<BaseTableInfo> >      = null;

    private constructor()
    {
        this.m_AllData = new HashMap();
    }

    public ReadAllTable(): void
    {
        this.ReadTable(TableType.TTD_ConfigType, new ConfigTableReader('config'));
        this.ReadTable(TableType.TTD_ItemType, new ItemTableReader('item'));
        this.ReadTable(TableType.TTD_AIType, new AITableReader('ai'));
        this.ReadTable(TableType.TTD_CarType, new CarTableReader('car'));
        this.ReadTable(TableType.TTD_MotionType, new MotionTableReader('motion'));
        this.ReadTable(TableType.TTD_TaskType, new TaskTableReader('dailytask'));
        this.ReadTable(TableType.TTD_TryTaskType, new TryPlayTaskTableReader('tryplaytask'));
        this.ReadTable(TableType.TTD_MiniGameType, new MiniGameTableReader('minigame'));
        this.ReadTable(TableType.TTD_SignInType, new SignInTableReader('signin'));
        this.ReadTable(TableType.TTD_BoxType, new BoxTableReader('box'));
        this.ReadTable(TableType.TTD_OnlineRewardType, new OnlineRewardTableReader('onlinereward'));
        this.ReadTable(TableType.TTD_OtherGames, new OtherGamesTableReader('othergames'));
    }

    public GetDataInfo<T extends BaseTableInfo>(type: TableType, id: number): T
    {
        if(this.IsValid(type))
        {
            let datas = this.m_AllData.Get(type);
            let count: number = datas.length;

            for(let i = 0;i < count; i ++)
            {
                let data: BaseTableInfo = datas[i];
                if(data && data.GetID() == id)
                {
                    return <T>(data);
                }
            }
        }
        return null;
    }

    public GetDataInfoByIndex<T extends BaseTableInfo>(type: TableType, index: number): T
    {
        if(this.IsValid(type))
        {
            let datas = this.m_AllData.Get(type);
            if(datas[index])
            {
                return <T>(datas[index]); 
            }
        }
        return null;
    }

    public GetDataInfoByRandom<T extends BaseTableInfo>(type: TableType): T
    {
        if(this.IsValid(type))
        {
            let datas = this.m_AllData.Get(type);
            let length = datas.length;

            let random: number = Util.RandomNum(0, length - 1);

            if(datas[random])
            {
                return <T>(datas[random]); 
            }
        }
        return null;
    }

    public GetDataCount(type: TableType)
    {
        if(this.IsValid(type))
        {
            return this.m_AllData.Get(type).length;
        }
        return 0;
    }

    private ReadTable(type: TableType, tableReader: BaseTableReader)
    {
        if(this.IsValid(type))
        {
            if(tableReader)
            {
                let count: number = tableReader.GetDataCount();
                let datas: Array<BaseTableInfo> = [];
                for(let i = 0;i < count; i ++)
                {
                    let info: BaseTableInfo = tableReader.ReadOneData(i);
                    if(info)
                    {
                        datas.push(info);
                    }
                }
                this.m_AllData.Add(type, datas);
            }
        }
    }

    private IsValid(type: TableType)
    {
        if(type > TableType.TTD_None && type < TableType.TTD_Max)
        {
            return true;
        }
        return false;
    }

    public static GetInstance(): GameTable
    {
        if(!this.instance)
        {
            this.instance = new GameTable;
        }
        return this.instance;
    }
}