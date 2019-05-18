import GameLog from '../Log/LogTool';
import ComponentBase from '../common/ComponentBase';
import { MessageType } from '../common/EventCenter';
const {ccclass, property} = cc._decorator;

@ccclass
export default class GameLoopCom extends ComponentBase
{
    private static BtnUrl: string = 'http://gather.51weiwan.com/uploads/file/20190110/3df40cc811485ef11aef0182937bef28.png';

    private clubBtn: any    = null;

    protected onLoad()
    {
        this.registerEvent(MessageType.GameLoopBtn_Show_Event, this.showClubBtn.bind(this));
        this.registerEvent(MessageType.GameLoopBtn_Hide_Event, this.hideClubBtn.bind(this));

        this.createGameLoopBtn();
    }

    private showClubBtn()
    {
        if(this.clubBtn)
        {
            this.clubBtn.show();
        }
    }

    private hideClubBtn()
    {
        if(this.clubBtn)
        {
            this.clubBtn.hide();
        }
    }

    private createGameLoopBtn()
    {
        if(!CC_WECHATGAME)
        {
            return;
        }

        GameLog.PrintLog("CreateClubButton");

        if(window["SystemInfo"])
        {
            let pos: cc.Vec2 = this.node.convertToWorldSpaceAR(cc.v2(0, 0));

            let left: number = pos.x / cc.winSize.width;
            let top: number = (cc.winSize.height - pos.y) / cc.winSize.height;

            GameLog.PrintLog("left = ", left, "top = ", top);

            let modelWidth = window["SystemInfo"].windowWidth;
            let modelHeight = window["SystemInfo"].windowHeight;
            let buttonWidth = 70 * (modelWidth / (cc.winSize.width));
            let buttonHeight = 80 * (modelHeight / (cc.winSize.height));
            let buttonLeft = modelWidth * left - buttonWidth * 0.5;
            let buttonTop = modelHeight * top - buttonHeight * 0.5;
            // let buttonTop =  (cc.winSize.height - 450) / (cc.winSize.height / modelHeight);

            // if(cc.director.getWinSize().height / cc.director.getWinSize().width - 16 / 9 > 0.1){ 
            //     tempTop += 20;
            // }

            this.clubBtn = wx.createGameClubButton({
                type: 'image',
                image: GameLoopCom.BtnUrl,
                style: {
                    left: buttonLeft,
                    top: buttonTop,
                    width: buttonWidth,
                    height: buttonHeight,
                }
            });


            this.hideClubBtn();
        }
    }

    onDestroy()
    {
        if(this.clubBtn)
        {
            this.clubBtn.destroy();
            this.clubBtn = null;
        }
    }
}
