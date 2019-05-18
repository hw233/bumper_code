import { TableType } from './table/TableDefine';
import GameTable from './manager/TableManger';
import MiniGameTableInfo from './table/MiniGameTableInfo';
import WXHelper from './common/WXHelper';
import GameLog from './Log/LogTool';
import EventCenter from './common/EventCenter';
import { MessageType } from './common/EventCenter';
export default class GameJump
{
    private static hideTime: number     = null;

    private static curGameID: number    = null;

    private static isJump: boolean = null;

    public static Init()
    {
        this.isJump = false;

        EventCenter.RegisterEvent("GameJump", MessageType.Game_On_Hide_Event, this.onHide.bind(this));
        EventCenter.RegisterEvent("GameJump", MessageType.Game_On_Show_Event, this.onShow.bind(this));
    }

    public static JupmGame(gameID: number, callback: Function = null)
    {
        let self = this;

        this.curGameID = gameID;

        let successFunc = function()
        {
            if(callback)
            {
                callback();
            }
            GameLog.PrintLog("跳转成功");

            self.isJump = true;

            EventCenter.DispatchEvent(MessageType.MiniProgramToSuc, gameTableInfo.GetID());
        };

        let gameTableInfo: MiniGameTableInfo = GameTable.GetInstance().GetDataInfo(TableType.TTD_MiniGameType, gameID);
        if(gameTableInfo)
        {
            if (WXHelper.IsWXContext()) 
            {
                wx.navigateToMiniProgram({
                    envVersion: "release", //'develop',//'trial',
                    appId: gameTableInfo.GetAppID(),
                    path: gameTableInfo.GetPath(),
                    success: successFunc,
                    fail: function (res) 
                    {
                        GameLog.PrintLog("跳转失败:", gameTableInfo.GetAppID());
                    },
                });
            }
            else
            {
                successFunc();
            }
        }
    }

    private static onHide()
    {
        GameLog.PrintLog("onHide");
        this.hideTime = Date.now();
    }

    private static onShow()
    {
        GameLog.PrintLog("GameJump: onShow");

        let curTime: number = Date.now();
        if(this.hideTime)
        {
            let time: number = (curTime - this.hideTime) / 1000;

            GameLog.PrintLog("试玩时间：", time);
            if(this.isJump && time >= 5)
            {
                EventCenter.DispatchEvent(MessageType.MiniProgramTryPlaySuc, this.curGameID);
            }
        }

        this.isJump = false;
    }
}