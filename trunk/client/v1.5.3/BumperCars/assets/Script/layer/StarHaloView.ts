
import EngineUtil from '../common/EngineUtil';
import PlayerMgr from '../manager/PlayerMgr';
import PlayerData from '../data/PlayerData';
import GameData from '../data/GameData';
import GameLogic from '../GameLogic';

//主角光环
export default class StarHaloView
{
    private m_StarHalo: cc.Node = null;     //主角光环
    private m_StarPointer: cc.Node = null;  //主角指针

    private m_StarID: number = null;    //主角id

    constructor()
    {
        this.m_StarID = 0;
    }

    public initView(parent: cc.Node)
    {
        this.m_StarHalo = cc.instantiate(EngineUtil.GetPrefab("StarAura"));
        if(this.m_StarHalo)
        {
            this.m_StarHalo.zIndex = 100;
            
            if(parent && this.m_StarHalo)
            {
                parent.addChild(this.m_StarHalo);
                this.m_StarHalo.x = 9999;
            }
    
            this.m_StarPointer = cc.instantiate(EngineUtil.GetPrefab("StarPointer"));
            if(parent && this.m_StarPointer)
            {
                parent.addChild(this.m_StarPointer);
                this.m_StarPointer.x = 9999;
            }
        }
    }

    public setStarID(id: number)
    {
        this.m_StarID = id;
    }

    public frameOnMove(dt)
    {
        let playerData: PlayerData = PlayerMgr.GetPlayer(this.m_StarID);

        if( playerData && PlayerMgr.IsAlive(this.m_StarID))
        {
            if(GameLogic.GetInstance().IsPracticeMode() === false)
            {
                if(GameData.GetBettleData().getOverlordUserID() === this.m_StarID)
                {
                    this.m_StarHalo.x = 9999;
                    this.m_StarPointer.x = 9999;
                    return;
                }
            } 

            if(this.m_StarHalo && this.m_StarPointer)
            {
                let position : cc.Vec2  = playerData.getPosition();
                let rotate: number      = playerData.getRotate();
                let curScale: number    = playerData.getCurScale();

                this.m_StarHalo.position = position;
                this.m_StarHalo.rotation = rotate;
                this.m_StarHalo.scale = curScale;

                this.m_StarPointer.position = position;
                this.m_StarPointer.rotation = rotate;
                this.m_StarPointer.scale = curScale;
            }
        }
        else
        {
            this.m_StarHalo.x = 9999;
            this.m_StarPointer.x = 9999;
        }
    }

    public destory()
    {
        this.m_StarHalo = null;
        this.m_StarPointer = null;
    }
}