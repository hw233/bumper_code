import NotRemveNodePool from './NotRemoveNodePool';
const {ccclass, property} = cc._decorator;

@ccclass
export default class MyNodePool extends cc.Component{

    @property({ type: cc.Prefab })
    private nodePrefab: cc.Prefab = null;

    @property
    private initCount: number = 10;

    @property
    private isRemove: boolean = true;

    private pool = null;

    private parent: cc.Node = null;

    public init(parent: cc.Node = null)
    {
        if(!this.nodePrefab)return;

        if(parent)
        {
            this.parent = parent;
        }

        if(this.isRemove)
        {
            this.pool = new cc.NodePool();
        }
        else
        {
            this.pool = new NotRemveNodePool();
        }

        for (let i: number = 0; i < this.initCount; ++i) 
        {

            let node = this.CreateNode();
            this.pool.put(node); // 通过 putInPool 接口放入对象池
        }
    }

    public GetNode(): cc.Node 
    {
        let node = null;

        if(this.pool){
            // 通过 size 接口判断对象池中是否有空闲的对象
            if (this.pool.size() > 0) { 
                node = this.pool.get();
            } else {
                // 如果没有空闲对象，也就是对象池中备用对象不够时，我们就用 cc.instantiate 重新创建
                node = this.CreateNode();
            }

            if(node && node.init)
            {
                if(this.parent && !node.parent)
                {
                    this.parent.addChild(node);
                }
                node.init();
            }

           
        }

        return node;
    }

    public PutNode(node): void
    {
        if(this.pool){
            this.pool.put(node); 
        }
    }

    public DestroyPool(){
        if(this.pool){
            this.pool.clear();
        }
        this.pool = null;
    }

    protected CreateNode()
    {
        let node = cc.instantiate(this.nodePrefab);
        if(node)
        {
            return node;
        }
        return null;
    }

    protected onDestroy()
    {
        this.DestroyPool();
    }
};
