const {ccclass, property} = cc._decorator;

@ccclass
export default class NotRemveNodePool extends cc.NodePool
{
    private _pool: Array<cc.Node> = null;

    constructor()
    {
        super();
        this._pool = [];
    }

   /**
     * !#en The current available size in the pool
     * !#zh 获取当前缓冲池的可用对象数量
     * @method size
     * @return {Number}
     */
    public size(): number
    {
        return this._pool.length;
    }

    /**
     * !#en Destroy all cached nodes in the pool
     * !#zh 销毁对象池中缓存的所有节点
     * @method clear
     */
    public clear(): void
    {
        var count = this._pool.length;

        for (let i: number = 0; i < count; ++i) 
        {
            this._pool[i].destroy();
        }
        this._pool.length = 0;
    }

    /**
     * !#en Put a new Node into the pool.
     * It will automatically remove the node from its parent without cleanup.
     * It will also invoke unuse method of the poolHandlerComp if exist.
     * !#zh 向缓冲池中存入一个不再需要的节点对象。
     * 这个函数不会将目标节点从父节点上移除, 只会改变其位置到一个看不到的地方 防止重新绘制vivisScene.
     * @method put
     * @param {Node} obj
     * @example
     *   let myNode = cc.instantiate(this.template);
     *   this.myPool.put(myNode);
     */
    public put(obj: cc.Node): void
    {
        if (obj && this._pool.indexOf(obj) === -1) 
        {
            obj.x += 9999;

            this._pool.push(obj);
        }
    }

    /**
     * !#en Get a obj from pool, if no available object in pool, null will be returned.
     * This function will invoke the reuse function of poolHandlerComp if exist.
     * !#zh 获取对象池中的对象，如果对象池没有可用对象，则返回空。
     * @method get
     * @param {any} ...params - !#en Params to pass to 'reuse' method in poolHandlerComp !#zh 向 poolHandlerComp 中的 'reuse' 函数传递的参数
     * @return {Node|null}
     * @example
     *   let newNode = this.myPool.get();
     */
    public get() :cc.Node
    {
        var last = this._pool.length - 1;
        if (last < 0) 
        {
            return null;
        }
        else 
        {
            // Pop the last object in pool
            var obj = this._pool[last];
            this._pool.length = last;

            return obj;
        }
    }
};
