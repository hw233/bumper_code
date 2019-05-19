const {ccclass, property} = cc._decorator;

@ccclass
export default class ScrollView extends cc.Component
{
    @property(cc.Node)
    private view:cc.Node            = null;       //scrollview的view剪裁节点
    @property(cc.Node)
    private list:cc.Node            = null;       //scrollview的具体装载item的node
    @property(cc.Prefab)
    private itemPrefab:cc.Prefab    = null;       //item的间隔

    @property()
    private spacingY: number        = 0;

    private data: Array<any>          = null;
    private rowItemCounts: number     = null;
    private items: Array<cc.Node>     = null;
    private topMax: number            = null;
    private bottomMax: number         = null;
    private lastListY: number         = null;
    private itemHeight: number        = null;

    private comName: string           = null;

    protected onLoad () 
    {
        //预加载的item的数据
        this.data = [];
        //当前可视区域内部填充满需要的item数量
        this.rowItemCounts = 0;
        //创建的item节点的数组
        this.items = [];
        //顶部最大Y
        this.topMax = 0;
        //底部最小Y
        this.bottomMax = 0;
        //上一次listnode的Y坐标
        this.lastListY = 0;
        //itemprefab的高度
        this.itemHeight = 0;
    }

    //使用需调用init方法传递进data数组 
    init(data: Array<any>, comName: string)
    {
        this.data = data;
        //保存高度
        let height = 0;
        
        this.list.removeAllChildren();

        this.list.y = this.view.height * 0.5;

        this.items.length = 0;

        if(data.length <= 0)
        {
            return;
        }

        this.comName = comName;

        //创建item 
        let item = cc.instantiate(this.itemPrefab);
        height = item.height;
        this.itemHeight = height;
        //计算可视区域内部填充满需要的item数量
        this.rowItemCounts = Math.ceil(this.view.height / (height + this.spacingY));

        //加载rowitemCounts + 10个item 
        for(let i = 0 ; i < this.rowItemCounts + 10; ++ i)
        {
            //数据已经加载完毕了 说明需要加载的数据量很小
            if(typeof data[i] == 'undefined')
                break;
                
            //加入item节点到scrollview的list里面
            this.list.addChild(item);

            //data[i]为了测试方便实际上只是一个1 2 3这样的数字 具体data和initView方法的实现
            //你需要根据你自己的情况来实现 
            item.getComponent(this.comName).initView(data[i]);
            //记录一下itemid
            item.itemID = i;
            //保存item到数组
            this.items.push(item);

            //设置x坐标
            item.x = 0;
            //设置y坐标 (根据自己设置的不同的锚点这些东西来调整能跑就完事了)
            item.y = - (i * (height + this.spacingY ));
            //继续创建
            if(i < this.rowItemCounts + 9)
            {
                item = cc.instantiate(this.itemPrefab);
            }
            
        }
        //设置list的高度 不设置无法滑动
        this.list.height = 20 + (data.length) * height + (data.length) * this.spacingY;
        //计算顶部最大Y
        this.topMax = (5 * height + 4 * this.spacingY);
        //计算底部最小Y
        this.bottomMax = -(this.view.height + this.topMax);
        //保存list的当前Y坐标
        this.lastListY = this.list.y;
    }

    update()
    {
        //判断是否往下滑动
        let isDown = this.list.y > this.lastListY;
        //当前的item数量
        let countOfItems = this.items.length;
        //预显示数据的总数量
        let dataLen = this.data.length;
        //遍历所有item节点
        for (let i in this.items)
        {
            let item = this.items[i];
            //item坐标转换到对应view节点的坐标 y坐标需要减去一半item的高度...具体看你item的锚点设置
            let itemPos = this.list.convertToWorldSpaceAR(item.position);
            itemPos.y -= this.view.height / 2;
            itemPos = this.view.convertToNodeSpaceAR(itemPos);
            //如果是往下滑动
            if(isDown)
            {
                //判断当前item的坐标是否大于顶部最大Y
                if(itemPos.y > this.topMax)
                {
                    //计算新的itmeid 
                    //比如一共13个item item的索引就是0-12 那么第0个item超过y坐标之后 就需要显示第13个item
                    //那么就是将当前id + 当前item的数量即可
                    let newId = item.itemID + countOfItems;
                    //如果item已经显示完毕了就不需要刷新了
                    if(newId >= dataLen) return;
                    //保存itemid
                    item.itemID = newId;
                    //计算item的新的Y坐标 也就是当前y减去所有item加起来的高度
                    item.y = item.y - countOfItems * this.itemHeight - (countOfItems ) * this.spacingY;
                    //刷新item内容 
                    item.getComponent(this.comName).initView(this.data[item.itemID]);
                }
                //如果是往上滑动
            }else 
            { 
                //如果超过底部最小Y 和上面的一样处理一下就完事了
                if(itemPos.y < this.bottomMax)
                {
                    let newId = item.itemID - countOfItems;
                    if (newId < 0) return;
                    item.itemID = newId;
                    item.y = item.y + countOfItems * this.itemHeight + (countOfItems) * this.spacingY;
                    item.getComponent(this.comName).initView(this.data[item.itemID]);
                }
            }
        }
        //存储下当前listnode的Y坐标 
        this.lastListY = this.list.y;
    }
}