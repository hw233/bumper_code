import BaseTableInfo from '../table/BaseTableInfo';
import EventCenter from '../common/EventCenter';
import GameTable from '../manager/TableManger';
import GameUI from '../manager/UIManager';
import ItemData from './ItemData';
import ItemManager from './ItemManager';
import SceneMgr, { SceneType } from '../manager/SceneMgr';
import { ItemDefine, ItemType } from './ItemDefine';
import { MessageType } from '../common/EventCenter';
import { TableType } from '../table/TableDefine';
import { UIType } from '../UI/UIDefine';

//道具系统
export default class ItemSystem
{
    private static EventTag: string     = "ItemSystem";
    private static instance: ItemSystem = null;

    private constructor()
    {
    }

    public Init()
    {
        EventCenter.RegisterEvent(ItemSystem.EventTag, MessageType.Item_Add_Item_Event, this.OnAddItem.bind(this));
        EventCenter.RegisterEvent(ItemSystem.EventTag, MessageType.Item_Delete_Item_Event, this.OnDeleteItem.bind(this));
        EventCenter.RegisterEvent(ItemSystem.EventTag, MessageType.Item_Update_Item_Event, this.OnUpdateItem.bind(this));
        EventCenter.RegisterEvent(ItemSystem.EventTag, MessageType.Item_Used_Item_Event, this.OnUsedItem.bind(this));
        EventCenter.RegisterEvent(ItemSystem.EventTag, MessageType.Item_Item_Expire_Event, this.OnItemExpire.bind(this));
        EventCenter.RegisterEvent(ItemSystem.EventTag, MessageType.Item_Try_Suc_Event, this.OnTrialItem.bind(this));
    }

    //根据道具ID判断是否为限时道具
    public IsTimeLimtiByItemID(itemID: number): boolean
    {
        return (itemID % 10) > 0;
    }

    //判断是否为限时道具
    public IsTimeLimitItem(itemID: number)
    {
        let itemData: ItemData = ItemManager.GetInstance().GetItemByItemID(itemID);
        if(itemData)
        {
            return this.IsTimeLimtiByItemID(itemData.GetItemID());
        }
    }

    //根据道具ID判断是否拥有某个道具
    public IsHasItemByItemID(itemID: number): boolean
    {
        if(ItemManager.GetInstance().GetItemByItemID(itemID))
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    //根据道具ID获取全局ID
    public GetSoleIDByItemID(itemID: number): number
    {
        let itemData: ItemData = ItemManager.GetInstance().GetItemByItemID(itemID);
        if(itemData)
        {
            return itemData.GetSoleID();
        }
        return -1;
    }

    //根据道具ID获取道具数量
    public GetItemCountByItemID(itemID: number): number
    {
        let itemData: ItemData = ItemManager.GetInstance().GetItemByItemID(itemID);
        if(itemData)
        {
            return itemData.GetAmount();
        }
        return 0;
    }

    //获取金币数量
    public GetGoldNum()
    {
        return this.GetItemCountByItemID(ItemDefine.GoldItemID);   
    }

    //获取碎片数量
    public GetDebrisNum()
    {
        return this.GetItemCountByItemID(ItemDefine.DebrisItemID);   
    }

    //道具唯一ID转道具id
    public GetItemIDBySoleID(soleID: number)
    {
        let itemData: ItemData = ItemManager.GetInstance().GetItemByID(soleID);
        if(itemData)
        {
            return this.GetItemIDByItemTypeID(itemData.GetItemID());
        }
        return 0;
    }

    //道具类型转道具ID
    public GetItemIDByItemTypeID(itemID: number)
    {
        return Math.floor(itemID / 10) * 10;
    } 

    //根据道具ID获取道具类型
    public GetTypeByItemID(itemID: number): ItemType
    {
        return Math.floor(itemID / 10000);
    }

    //根据道具ID获取表数据
    public GetTableInfoByItemID<T extends BaseTableInfo>(itemID: number): T
    {
        let itemType: ItemType = this.GetTypeByItemID(itemID);

        let tempItemID: number = this.GetItemIDByItemTypeID(itemID);

        let tableInfo: T = null;

        switch(itemType)
        {
            case ItemType.IT_CarType:           //车
            {
                tableInfo = GameTable.GetInstance().GetDataInfo(TableType.TTD_CarType, tempItemID);
                break;
            }
            case ItemType.IT_MotionType:        //拖尾
            {
                tableInfo = GameTable.GetInstance().GetDataInfo(TableType.TTD_MotionType, tempItemID);
                break;
            }
            case ItemType.IT_GoldType:          //金币
            case ItemType.IT_DebrisType:        //碎片
            {
                tableInfo = GameTable.GetInstance().GetDataInfo(TableType.TTD_ItemType, tempItemID);
                break;
            }
        }

        return tableInfo;
    }

    //展示过期的道具
    public ShowExpireItem()
    {
        let expireItems = [];

        let itemCount: number = ItemManager.GetInstance().GetItemCount();

        for(let i = 0; i < itemCount;i ++)
        {
            let itemData: ItemData = ItemManager.GetInstance().GetItemByIndex(i);
            if(itemData && this.IsTimeLimtiByItemID(itemData.GetItemID()))
            {
                if(itemData.GetLiftTime() <= 0)
                {
                    //到期了
                    expireItems.push(itemData.GetSoleID());
                }
            }
        }

        if(expireItems.length > 1)
        {
            GameUI.ShowUI(UIType.UT_TryUseFinishView, expireItems);
        }
        else if(expireItems.length > 0)
        {
            GameUI.ShowUI(UIType.UT_TryUseFinishView, expireItems[0]);
        }else
        {
            GameUI.RemoveUI(UIType.UT_TryUseFinishView);
        }
    }

    //更新试用时间
    public UpdateLiftTime()
    {
        let itemCount: number = ItemManager.GetInstance().GetItemCount();

        for(let i = 0; i < itemCount;i ++)
        {
            let itemData: ItemData = ItemManager.GetInstance().GetItemByIndex(i);
            if(itemData && this.IsTimeLimtiByItemID(itemData.GetItemID()))
            {
                if(itemData.GetLiftTime() > 0)
                {
                    itemData.SetLiftTime(itemData.GetLiftTime() - 1);
                }
            }
        }
    }

    private OnItemExpire(soleID: number)
    {
        if(
                SceneMgr.GetCurSceneType() === SceneType.StartScene 
            // ||  SceneMgr.GetCurSceneType() === SceneType.DLDRoomScene   
            // ||  SceneMgr.GetCurSceneType() === SceneType.ZDZRoomScene
        )   
        {
            //直接弹出到期窗口
            GameUI.ShowUI(UIType.UT_TryUseFinishView, soleID);
        }
        else
        {
            //缓存起来进入 返回首页在弹出到期窗口
        }
    }

    private OnAddItem(item: ItemAddParams)
    {
        if(item)
        {
            if(ItemManager.GetInstance().GetItemByID(item.soleID) === null)
            {
                let itemData: ItemData = new ItemData();
                itemData.SetSoleID(item.soleID);
                itemData.SetItemID(item.itemID);
                itemData.SetItemType(item.type);
                itemData.SetAmount(item.amount);
                itemData.SetLiftTime(item.liftTime);
    
                ItemManager.GetInstance().AddItem(itemData);
            }
        }
    }

    private OnDeleteItem(data: any)
    {
        ItemManager.GetInstance().RemoveItemByID(data.soleID);

        EventCenter.DispatchEvent(MessageType.CarLib_RefreshView);
        EventCenter.DispatchEvent(MessageType.StartScene_RefreshCarList);
    }

    private OnUpdateItem(item: ItemUpdateParams)
    {
        let itemData: ItemData = ItemManager.GetInstance().GetItemByID(item.soleID);
        if(itemData)
        {
            itemData.SetAmount(item.amount);
            itemData.SetLiftTime(item.liftTime);
        }
    }

    private OnUsedItem(item: ItemTrialParams)
    {

    }

    private OnTrialItem(item: ItemTrialParams)
    {
        let liftTime: number = 0;

        if(item.liftTime)
        {
            liftTime = item.liftTime;
        }
        else
        {
            let itemData: ItemData = ItemManager.GetInstance().GetItemByID(item.itemID);
            if(itemData)
            {
                liftTime = itemData.GetLiftTime();
            }
        }

        //免费使用
        GameUI.ShowUI(UIType.UT_TryUseSucView, liftTime);

        EventCenter.DispatchEvent(MessageType.CarLib_RefreshView);
        EventCenter.DispatchEvent(MessageType.StartScene_RefreshCarList);
        EventCenter.DispatchEvent(MessageType.StartScene_RefreshTopView);
    }

    private Destroy()
    {
    }

    public static GetInstance(): ItemSystem
    {
        if(!this.instance)
        {
            this.instance = new ItemSystem();
        }
        return this.instance;
    }

    public static Destroy()
    {
        if(this.instance)
        {
            this.instance = null;
            this.instance.Destroy();
        }
    }
}

type ItemAddParams = 
{
    soleID: number,
    itemID: number,
    type: number,
    liftTime: number,
    amount: number,
}

type ItemUpdateParams = 
{
    soleID: number,
    liftTime: number,
    amount: number,
}

type ItemTrialParams = 
{
    itemID: number,
    liftTime: number,
}