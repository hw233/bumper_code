import EngineUtil from '../common/EngineUtil';
import PlayerMgr from '../manager/PlayerMgr';
import PlayerData from '../data/PlayerData';

//仇人光环
export default class EnemyHaloView
{
    private m_EnemyHalo: cc.Node = null;     //仇人光环

    private m_EnemyID: number = null;           //仇人的id

    constructor()
    {
        this.m_EnemyID = 0;
    }

    public initView(parent: cc.Node)
    {
        this.m_EnemyHalo = cc.instantiate(EngineUtil.GetPrefab("EnemyNode"));
        if(parent && this.m_EnemyHalo)
        {
            parent.addChild(this.m_EnemyHalo);
            this.m_EnemyHalo.x = 9999;
        }
    }

    public refreshEnemyID(id: number)
    {
        if(this.m_EnemyID === id)
        {
            return;
        }

        this.m_EnemyID = id;
    }

    public frameOnMove(dt: number)
    {
        if(PlayerMgr.IsAlive(this.m_EnemyID))
        {
            let playerData: PlayerData = PlayerMgr.GetPlayer(this.m_EnemyID);

            let position : cc.Vec2  = playerData.getPosition();
            let rotate: number      = playerData.getRotate();
            let curScale: number    = playerData.getCurScale();

            if(this.m_EnemyHalo)
            {
                this.m_EnemyHalo.position = position;
                this.m_EnemyHalo.rotation = rotate;
                this.m_EnemyHalo.scale = curScale;
            }
        }
        else
        {
            if(this.m_EnemyHalo)
            {
                this.m_EnemyHalo.x = 9999;
            }
        }
    }

    public destory()
    {
        this.m_EnemyHalo = null;
    }
}