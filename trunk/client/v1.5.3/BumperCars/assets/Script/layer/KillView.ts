import PlayerData from '../data/PlayerData';
import PlayerMgr from '../manager/PlayerMgr';
import EngineUtil from '../common/EngineUtil';
import KillViewManager from '../KillViewManager';
import Util from '../common/Util';
import { AtlasType } from '../common/EngineUtil';
const {ccclass, property} = cc._decorator;

@ccclass
export default class KillView extends cc.Component 
{
    @property(cc.Node)
    private m_pKillerImg: cc.Node = null

    @property(cc.Node)
    private m_pDecedentImg: cc.Node = null

    @property(cc.Node)
    private m_pKillerName: cc.Node = null

    @property(cc.Node)
    private m_pDecedentName: cc.Node = null

    @property(cc.Node)
    private m_pScoreLable: cc.Node = null;

    @property(cc.Node)
    private m_pGoldLabel: cc.Node = null

    @property(cc.Node)
    private m_pKillImg: cc.Node = null

    protected onLoad () 
    {
		
	}

    protected start () 
    {

    }

    public initView(killerID: number, decedentID: number, gold: number, score: number, killTimes: number)
    {
        let killerData: PlayerData = PlayerMgr.GetPlayer(killerID);
        if(killerData)
        {
            //限制昵称最大为12个字符
            let nickname = Util.GetByteVal(killerData.getNickName(), 12);
            EngineUtil.LoadRemoteImg(this.m_pKillerImg, killerData.getAvatarUrl());
            EngineUtil.SetNodeText(this.m_pKillerName, nickname);
        }

        let decedentData: PlayerData = PlayerMgr.GetPlayer(decedentID);
        if(decedentData)
        {
            //限制昵称最大为12个字符
            let nickname = Util.GetByteVal(decedentData.getNickName(), 12);
            EngineUtil.LoadRemoteImg(this.m_pDecedentImg, decedentData.getAvatarUrl());
            EngineUtil.SetNodeText(this.m_pDecedentName, nickname);
        }

        EngineUtil.SetNodeText(this.m_pScoreLable, score.toString());
        EngineUtil.SetNodeText(this.m_pGoldLabel, gold.toString());

        switch(killTimes)
        {
            case 1:
            {
                EngineUtil.SetNodeTextureByType(AtlasType.AT_GameScene, this.m_pKillImg, "FirstBlood");
                break;
            }
            case 2:
            {
                EngineUtil.SetNodeTextureByType(AtlasType.AT_GameScene, this.m_pKillImg, "DoubleKill");
                break;
            }
            case 3:
            {
                EngineUtil.SetNodeTextureByType(AtlasType.AT_GameScene, this.m_pKillImg, "TripleKill");
                break;
            }
            case 4:
            {
                EngineUtil.SetNodeTextureByType(AtlasType.AT_GameScene, this.m_pKillImg, "KillingSpree");
                break;
            }
            case 5:
            {
                EngineUtil.SetNodeTextureByType(AtlasType.AT_GameScene, this.m_pKillImg, "Outbreak");
                break;
            }
            default:
            {
                EngineUtil.SetNodeTextureByType(AtlasType.AT_GameScene, this.m_pKillImg, "FirstBlood");
                break;
            }
        }
    }

    public playAnimate(): void
    {
        let self = this;

        let winSize: cc.Size = cc.director.getWinSize();

        let moveAction1 = cc.moveBy(0.5, winSize.width, 0);
        let delayAction = cc.delayTime(1);
        let moveAction2 = cc.moveBy(0.5, winSize.width, 0);
        let callback = cc.callFunc(function()
        {
            self.node.x = -winSize.width;
            KillViewManager.GetInstance().PutKillNode(this);
        });

        this.node.runAction(cc.sequence(moveAction1, delayAction, moveAction2, callback));
    }
    // update (dt) {}
}
