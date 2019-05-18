import PopUpBase from './PopUpBase';
import EngineUtil from '../common/EngineUtil';
import GameUI from '../manager/UIManager';
import { UIType } from './UIDefine';
import { RewardData } from '../data/RewardData';
import { AtlasType } from '../common/EngineUtil';

const {ccclass, property} = cc._decorator;

@ccclass
export default class SignInBoxRewardView extends PopUpBase 
{
    @property(cc.Node)
    private boxImg: cc.Node             = null

    @property(cc.Animation)
    private animate: cc.Animation       = null;

    private boxID: number               = null;

    public OnInit()
    {
        super.OnInit();

        this.boxID = 0;

        if(this.animate)
        {
            this.animate.on('finished', this.playFinish.bind(this));
        }
    }

    public RefreshView()
    {
        super.RefreshView();
        
        let boxID: number = this.param.boxID;
        let rewardList: Array<RewardData> = this.param.rewardList;

        this.boxID = boxID;

        if(rewardList && rewardList.length > 0)
        {
            for(let i = 0, len = rewardList.length; i < len; i ++)
            {
                let reward = rewardList[i];
                if(reward)
                {
                    GameUI.AddViewToQuene(UIType.UT_RewardView, {
                        itemID: reward.ItemID,
                        itemNum: reward.ItemNum,
                    })
                }
            }
        }
        EngineUtil.SetNodeTextureByType(AtlasType.AT_TaskLayer, this.boxImg, "SignIn_Box_" + boxID + "_1");
    }

    protected OnShowFinished()
    {
        super.OnShowFinished();

        if(this.animate)
        {
            this.animate.play();
        }
    }

    private playFinish()
    {
        EngineUtil.SetNodeTextureByType(AtlasType.AT_TaskLayer, this.boxImg, "SignIn_Box_" + this.boxID + "_2");

        this.scheduleOnce(()=>{
            GameUI.HideUI(this.GetType());
            GameUI.ShowViewByQuene();
        }, 0.5);
    }
}
