
const {ccclass, property} = cc._decorator;

//矿石管理器
@ccclass
export default class OreManager extends cc.Component{

    private allOre = null;

    private oreMap = null;

    private orePool = null;

    onLoad()
    {
        this.allOre = {};
        this.oreMap = {};
    }

    //添加一个矿石
    public addOre(id, tag, item)
    {
        if(item)
        {
            this.allOre[id] = item;

            if(!this.oreMap[tag])
            {
                this.oreMap[tag] = [];
            }
            this.oreMap[tag].push(id);
        }
    }

    //移除一个矿石
    public removeOre(id)
    {
        if(this.allOre[id])
        {
            let tag = this.allOre[id].tag;
            if(this.oreMap[tag])
            {
                for(let i = 0, len = this.oreMap[tag].length; i < len; i ++)
                {
                    if(this.oreMap[tag][i] == id)
                    {
                        this.oreMap[tag].splice(i, 1);
                        break;
                    }
                }
            }

            this.orePool.PutNode(this.allOre[id]);
            delete this.allOre[id];
        }
    }

    public getOreListByTag(tag: number)
    {
        if(this.oreMap[tag])
        {
            return this.oreMap[tag];
        }
        return null;
    }

    public getOreByID(id: number)
    {
        if(this.allOre[id])
        {
            return this.allOre[id];
        }
        return null;
    }

    public setOrePool(orePool)
    {
        this.orePool = orePool;
    }

    onDestroy()
    {
        this.allOre = null;

        for(var key in this.oreMap)
        {
            if(this.oreMap[key])
            {
                this.oreMap[key].length = 0;
            }
        }
        this.oreMap = null;
        this.orePool = null;
    }
}
