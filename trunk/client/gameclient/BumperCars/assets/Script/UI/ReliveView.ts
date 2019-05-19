import PopUpBase from "../UI/PopUpBase";
import GlobalVar from "../GlobalVar";
import PlayerMgr from "../manager/PlayerMgr";
import Socket_SendMgr from "../network/socket/Socket_SendManager";
import EngineUtil from "../common/EngineUtil";
import AdvertiseMgr from '../Advertise/AdvertiseMgr';
import { AdvertiseType } from '../Advertise/AdvertiseMgr';
import { ShareSceneType } from '../common/ShareSceneType';
import ConfigData from '../common/ConfigData';
import { AtlasType } from '../common/EngineUtil';
import GameConfig from '../GameConfig';

//复活类型
export enum _RELIVE_TYPE{
    _RELIVE_TYPE_NORMAL 					= 0,		//普通复活
    _RELIVE_TYPE_VIEW						= 1,		//看视频复活
};

const {ccclass, property} = cc._decorator;

@ccclass
export default class ReliveView extends PopUpBase
{
    @property({type: cc.Node})
    private enemyNickeName: cc.Node = null;

    @property({type: cc.Node})
    private enemyHeadImg: cc.Node = null;

    @property({type: cc.Node})
    private keepReliveBtn: cc.Node = null;

    @property({type: cc.Node})
    private normalReliveBtn: cc.Node = null;

    @property({type: cc.Node})
    private freeReliveFlag: cc.Node = null;

    @property({type: cc.Node})
    private freeWatchTip: cc.Node = null;

    @property({type: cc.Node})
    private watchTip: cc.Node = null;

    @property({type: cc.Node})
    private killTypeImg: cc.Node = null;
    
    public OnInit () 
    {
        super.OnInit();

        this.registerTouchEndEvent(this.keepReliveBtn, this.keepReliveBtnClicked.bind(this));
        this.registerTouchEndEvent(this.normalReliveBtn, this.normalReliveBtnClicked.bind(this));

        if(ConfigData.Switch_ReliveAD)
        {
            this.watchTip.active = true;
            this.freeWatchTip.active = false;
            this.freeReliveFlag.active = false;
        }
        else
        {
            this.watchTip.active = false;
            this.freeWatchTip.active = true;
            this.freeReliveFlag.active = true;
        }
    }

    public RefreshView()
    {
        super.RefreshView();

        let killType: number = this.param.killType;
        let nickname: string = this.param.nickName;
        let headUrl: string = this.param.headUrl;

        if(killType === 1)
        {
            //自杀
            EngineUtil.SetNodeTextureByType(AtlasType.AT_GameScene, this.killTypeImg, "Game_SelfKilled");
        }
        else
        {
            //他杀
            EngineUtil.SetNodeTextureByType(AtlasType.AT_GameScene, this.killTypeImg, "Game_HeKilled");
        }
        EngineUtil.SetNodeText(this.enemyNickeName, nickname);
        EngineUtil.LoadRemoteImg(this.enemyHeadImg, headUrl);
    }

    //保留复活
    private keepReliveBtnClicked(): void
    {
        let playerData = PlayerMgr.GetPlayer(GlobalVar.userID);
        if(playerData)
        {
            if(ConfigData.Switch_ReliveAD)
            {
                AdvertiseMgr.CreateVideoAd(AdvertiseType.ADConfig_Video_Keep_Star, function(isFinish)
                {
                    if(isFinish)
                    {
                        Socket_SendMgr.GetBettleSend().SendReliveMessage(_RELIVE_TYPE._RELIVE_TYPE_VIEW, playerData.getBirthPointID());
                    }
                }, ShareSceneType.ReceiveGift);
            }else
            {
                Socket_SendMgr.GetBettleSend().SendReliveMessage(_RELIVE_TYPE._RELIVE_TYPE_VIEW, playerData.getBirthPointID());
            }
        }
    }

    //普通复活
    private normalReliveBtnClicked(): void
    {
        let playerData = PlayerMgr.GetPlayer(GlobalVar.userID);
        
        if(playerData)
        {
            Socket_SendMgr.GetBettleSend().SendReliveMessage(_RELIVE_TYPE._RELIVE_TYPE_NORMAL, playerData.getBirthPointID());
        }
    }
}
