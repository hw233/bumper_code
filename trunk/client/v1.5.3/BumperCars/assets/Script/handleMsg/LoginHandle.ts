
import {MsgID} from "../network/socket/MsgID";
import GameLog from "../Log/LogTool";
import GlobalVar from '../GlobalVar';
import EventCenter from '../common/EventCenter';
import { MessageType } from '../common/EventCenter';
import AccountData from '../manager/AccountData';
import HandleBase from './HandleBase';
import OnlineRewardTime from '../OnlineRewardTime';
import ItemSystem from '../Item/ItemSystem';

export default class LoginHandle extends HandleBase
{
    public constructor()
    {
        super();
    }

    public RegisterAllHandle()
    {
        //登陆的消息
        this.RegisterHandle(MsgID.MSG_LOGIN, null, this.OnLoginItem.bind(this));
        //角色信息
        this.RegisterHandle(MsgID.MSG_USERINFO, null, this.OnPlayerInfo.bind(this));

    }

    //登陆的消息
    private OnLoginItem(msg)
    {
        if(msg.result){
        }else{
            GameLog.PrintLog("登陆失败", msg.result);
        }
    }

    //角色信息
    private OnPlayerInfo(msg)
    {
        GlobalVar.IsLoggedIn = true;

        let curCarItemID: number = ItemSystem.GetInstance().GetItemIDBySoleID(msg.use_car);
        let curMotionItemID: number = ItemSystem.GetInstance().GetItemIDBySoleID(msg.tail);

        AccountData.GetInstance().SetUserID(Number(msg.uid) || 0);
        AccountData.GetInstance().SetNickName(msg.nick_name);
        AccountData.GetInstance().SetHeadImg(msg.head_picurl);
        AccountData.GetInstance().SetCurLevelID(msg.granding_id || 1);
        AccountData.GetInstance().SetKillNum(msg.kill_sum);
        // AccountData.GetInstance().SetGoldNum(msg.gold_sum);
        AccountData.GetInstance().SetCurStarNum(msg.cur_star_sum);
        // AccountData.GetInstance().SetUnlockCarID(this.convertUnlockCarData(msg.unlock_cars));
        AccountData.GetInstance().SetUseCarID(curCarItemID);
        AccountData.GetInstance().SetUseMotionID(curMotionItemID);
        AccountData.GetInstance().SetKeepStarCount(msg.star_sum);
        AccountData.GetInstance().SetReceiveGiftCount(msg.gift_sum);
        AccountData.GetInstance().SetBZLPNum(msg.king_sum);
        AccountData.GetInstance().SetAddGold(msg.add_gold);
        AccountData.GetInstance().SetAddScore(msg.add_score);
        AccountData.GetInstance().SetReceiveGiftCount(msg.get_gift || 0);
        AccountData.GetInstance().SetKeepStarCount(msg.keep_star || 0);
        AccountData.GetInstance().SetCurLevelNum(msg.granding_num || 0);
        AccountData.GetInstance().SetSignInData(Number(msg.weektask_mask) || 0);
        AccountData.GetInstance().SetTaskData(Number(msg.daytask_mask) || 0);
        AccountData.GetInstance().SetOnlineRewardTime(Number(msg.online_remain_ts) || 0);
        AccountData.GetInstance().SetDayIndex(Number(msg.cur_tm_wday));
        AccountData.GetInstance().SetTimeLimitItemID(Number(msg.show_item_type));
        AccountData.GetInstance().SetTimeLimitRemainTime(Number(msg.show_remain_ts));

        if(!CC_WECHATGAME)
        {
            AccountData.GetInstance().SetNickName(msg.uid);
        }

        EventCenter.DispatchEvent(MessageType.Login_Success);

        if(GlobalVar.IsReconnected)
        {
            GlobalVar.IsReconnected = false;
            
            EventCenter.DispatchEvent(MessageType.DLD_Reconnect_Suc);
        }

        //登陆成功 开始在线奖励的计时
        OnlineRewardTime.StartTime();
    }

    private convertUnlockCarData(unlock: string): Array<number>
    {
        let cars = unlock.split(",");
        let carArr: Array<number> = [];
        for(let i = 0, len = cars.length; i < len; i ++)
        {
            carArr.push(parseInt(cars[i]));
        }

        return carArr;
    }
}