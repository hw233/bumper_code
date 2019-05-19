import EngineUtil from '../common/EngineUtil';
import PlayerMgr from '../manager/PlayerMgr';
import PlayerData from '../data/PlayerData';

//霸主光环
export default class OverlordHaloView
{
    private m_OverlordHalo: cc.Node = null;     //霸主光环
    private m_OverlordPointer: cc.Node = null;  //霸主指针

    private m_OwnerID: number = null;           //拥有者的id

    constructor()
    {
        this.m_OwnerID = 0;
    }

    public initView(parent: cc.Node)
    {
        this.m_OverlordHalo = cc.instantiate(EngineUtil.GetPrefab("OverlordHalo"));
        if(parent && this.m_OverlordHalo)
        {
            parent.addChild(this.m_OverlordHalo);
            this.m_OverlordHalo.x = 9999;
        }

        this.m_OverlordPointer = cc.instantiate(EngineUtil.GetPrefab("OverlordPointer"));
        if(parent && this.m_OverlordPointer)
        {
            parent.addChild(this.m_OverlordPointer);
            this.m_OverlordPointer.x = 9999;
        }
    }

    public refreshOwnerID(ownerID: number)
    {
        if(this.m_OwnerID === ownerID)
        {
            return;
        }

        this.m_OwnerID = ownerID;
    }

    public frameOnMove(dt)
    {
        if(PlayerMgr.IsAlive(this.m_OwnerID))
        {
            let playerData: PlayerData = PlayerMgr.GetPlayer(this.m_OwnerID);

            let position : cc.Vec2  = playerData.getPosition();
            let rotate: number      = playerData.getRotate();
            let curScale: number    = playerData.getCurScale();

            if(this.m_OverlordHalo)
            {
                this.m_OverlordHalo.position = position;
                this.m_OverlordHalo.rotation = rotate;
                this.m_OverlordHalo.scale = curScale;
            }

            if(this.m_OverlordPointer)
            {
                if(PlayerMgr.IsSelf(this.m_OwnerID))
                {
                    this.m_OverlordPointer.position = position;
                    this.m_OverlordPointer.rotation = rotate;
                    this.m_OverlordPointer.scale = curScale;
                }
                else
                {
                    this.m_OverlordPointer.x = 9999;
                }
            }
        }
        else
        {
            if(this.m_OverlordHalo)
            {
                this.m_OverlordHalo.x = 9999;
            }
            if(this.m_OverlordPointer)
            {
                this.m_OverlordPointer.x = 9999;
            }
        }
    }

    public destory()
    {
        this.m_OverlordHalo = null;
        this.m_OverlordPointer = null;
    }
}