import KillView from './layer/KillView';
import EngineUtil from './common/EngineUtil';
import GlobalVar from './GlobalVar';
import { GameModel } from './GameDefine';

export default class KillViewManager
{
    private static instance: KillViewManager = null;

    private killViewPool: cc.NodePool = null;

    private constructor()
    {
        this.killViewPool = new cc.NodePool;
    }

    public static GetInstance(): KillViewManager
    {
        if(!this.instance)
        {
            this.instance = new KillViewManager();
        }
        return this.instance;
    }

    public Init()
    {
        for(let i = 0; i < 5;i ++)
        {
            let node: cc.Node = this.createNode();
            this.killViewPool.put(node);
        }
    }

    public PlayKillInfo(parent: cc.Node, killerID: number, decedentID: number, gold: number, score: number, killTimes: number)
    {
        let killNode: cc.Node = this.getKillView();
        if(killNode && parent)
        {
            parent.addChild(killNode);

            let killView: KillView = killNode.getComponent("KillView");
            if(killView)
            {
                killView.initView(killerID, decedentID, gold, score, killTimes);
                killView.playAnimate();
            }
        }
    }

    public PutKillNode(killNode: cc.Node): void
    {
        this.killViewPool.put(killNode);
    }

    private getKillView(): cc.Node
    {
        let node = null;

        if(this.killViewPool)
        {
            // 通过 size 接口判断对象池中是否有空闲的对象
            if (this.killViewPool.size() > 0) 
            { 
                node = this.killViewPool.get();
            } else 
            {
                // 如果没有空闲对象，也就是对象池中备用对象不够时，我们就用 cc.instantiate 重新创建
                node = this.createNode();
            }
        }

        return node;
    }

    private createNode()
    {
        let killViewPrefab: cc.Prefab = null;

        switch(GlobalVar.gameModel)
        {
            case GameModel.GM_DLDMode:
            {
                killViewPrefab = EngineUtil.GetPrefab("DLDKillFrameNode");
                break;
            }
            case GameModel.GM_DQZMode:
            {
                killViewPrefab = EngineUtil.GetPrefab("DQZKillFrameNode");
                break;
            }
            case GameModel.GM_PracticeMode:
            {
                killViewPrefab = EngineUtil.GetPrefab("LXSKillFrameNode");
                break;
            }
        }
        
        if(killViewPrefab)
        {
            return cc.instantiate(killViewPrefab);
        }
        return null;
    }

    public Destroy()
    {
        this.killViewPool.clear();
    }

    public static Destroy()
    {
        if(this.instance)
        {
            this.instance.Destroy();
            this.instance = null;
        }
    }
}