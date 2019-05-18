export interface KeyValue<K,V>
{
    key: K,
    value: V
}

export class HashMap<K, V>
{
    //存儲列表
    private _list: KeyValue<K, V>[] = null;

    constructor()
    {
        this._list = [];
    }

    //通过key获取索引
    private GetIndexByKey(key: K): number
    {
        let count: number = this._list.length;
        for(let index = 0; index < count; index ++)
        {
            const element: KeyValue<K, V> = this._list[index];
            if(element.key == key)
            {
                return index;
            }
        }
        return -1;
    }

    public Add(key: K, value: V): void
    {
        var data: KeyValue<K, V> = { key: key, value: value };
        var index: number = this.GetIndexByKey(key);
        if (index != -1)
        {
            //已存在：刷新值
            this._list[index] = data;
        }
        else
        {
            //不存在：添加值
            this._list.push(data);
        }
    }

    public Remove(key: K): any
    {
        var index: number = this.GetIndexByKey(key);
        if (index != -1)
        {
            var data: KeyValue<K, V> = this._list[index];
            this._list.splice(index, 1);
            return data;
        }
        return null;
    }

    public Has(key: K): boolean
    {
        var index: number = this.GetIndexByKey(key);
        return index != -1;
    }

    public IndexOf(index: number): V
    {
        if (index != -1)
        {
            var data: KeyValue<K, V> = this._list[index];
            return data.value;
        }
        return null;
    }

    public Get(key: K): V
    {
        var index: number = this.GetIndexByKey(key);
        if (index != -1)
        {
            var data: KeyValue<K, V> = this._list[index];
            return data.value;
        }
        return null;
    }

    public Size(): number
    {
        if(this._list)
        {
            return this._list.length;
        }
        return 0;
    }

    public ForEachKeyValue(f: {(data: KeyValue<K,V>): void})
    {
        var count: number = this._list.length;
        for (let index = 0; index < count; index++)
        {
            const element: KeyValue<K, V> = this._list[index];
            f(element);
        }
    }

    public ForEach(f: {(key: K, value: V): void})
    {
        var count: number = this._list.length;
        for (let index = 0; index < count; index++)
        {
            const element: KeyValue<K, V> = this._list[index];
            f(element.key, element.value);
        }
    }

    public Sort(f)
    {
        this._list.sort(f);
    }

    public Clear()
    {
        if(this._list)
        {
            this._list.length = 0;
        }
    }

    public Destroy()
    {
        this.Clear();
        this._list = null;
    }
}