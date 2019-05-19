import ItemData from './ItemData';
import ItemSystem from './ItemSystem';

//道具管理器
export default class ItemManager
{
    private static instance: ItemManager = null;

    private m_ItemList: Array<ItemData>   = null;

    private constructor()
    {
        this.m_ItemList = [];
    }

    public AddItem(itemData: ItemData)
    {
        if(itemData)
        {
            this.m_ItemList.push(itemData);
        }
    }

    public GetItemByItemID(itemID: number): ItemData
    {
        for(let i = 0, len = this.m_ItemList.length; i < len; i ++)
        {
            let tempItemID: number = ItemSystem.GetInstance().GetItemIDByItemTypeID(this.m_ItemList[i].GetItemID());
            if(tempItemID === itemID)
            {
                return this.m_ItemList[i];
            }   
        }
        return null;
    }

    public GetItemByID(soleID: number): ItemData
    {
        for(let i = 0, len = this.m_ItemList.length; i < len; i ++)
        {
            if(this.m_ItemList[i].GetSoleID() === soleID)
            {
                return this.m_ItemList[i];
            }   
        }
        return null;
    }

    public GetItemByIndex(index: number): ItemData
    {
        if(this.m_ItemList[index])
        {
            return this.m_ItemList[index];
        }

        return null;
    }

    public GetItemCount(): number
    {
        return this.m_ItemList.length;
    }

    public RemoveItemByID(soleID: number): void
    {
        for(let i = 0, len = this.m_ItemList.length; i < len; i ++)
        {
            if(this.m_ItemList[i].GetSoleID() === soleID)
            {
                this.m_ItemList.splice(i, 1);
                break;
            }   
        }
    }

    private Destroy()
    {
        this.m_ItemList.length = 0;
        this.m_ItemList = null;
    }

    public static GetInstance(): ItemManager
    {
        if(!this.instance)
        {
            this.instance = new ItemManager();
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