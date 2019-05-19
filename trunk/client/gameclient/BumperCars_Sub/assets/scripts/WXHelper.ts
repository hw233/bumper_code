import GlobalVar from './GlobalVar';
export default class WXHelper
{
    public static IsWXContext()
    {
        if (CC_WECHATGAME) 
        {
            return true;
        }
        return false;
    }

    public static GetUseInfo(successCb)
    {
        wx.getUserInfo({
            withCredentials: false,
            openIdList: ['selfOpenId'],
            lang: "zh_CN",
            success: successCb,
            fail: function(){
                cc.log("获取用户信息失败:");
            }
        });
    }

    //获取群同玩成员的游戏数据
    public static GetGroupCloudData(keyArray, success)
    {
        console.log("shareTicket:", GlobalVar.groupShareTicket);

        wx.getGroupCloudStorage({
            shareTicket: GlobalVar.groupShareTicket,
            keyList: keyArray,
            success: success,
            fail: function(res){
                console.log("Rank:获取群的信息失败:", res);
            }
        });
    }

    //获取好友的游戏数据
    public static GetFriendCloudData(keyArray, success)
    {
        wx.getFriendCloudStorage({
            keyList: keyArray,
            success: res => {
                console.log("wx.getFriendCloudStorage success", res);
                success(res);
            },
            fail: res => {
                console.log("wx.getFriendCloudStorage fail", res);
            },
        });
    }
}