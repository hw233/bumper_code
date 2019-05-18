import Util from "./common/Util";
import { UserCmdType, GameModel } from './GameDefine';
import GlobalVar from './GlobalVar';
import GameConfig from './GameConfig';
import TimeCondition from './component/TimeCondition';
import GameLogic from './GameLogic';

////////////////////////////////////////////////////////////////////
//                            _ooOoo_                             //
//                           o8888888o                            //
//                           88" . "88                            //
//                           (| ^_^ |)                            //
//                           O\  =  /O                            //
//                        ____/`---'\____                         //
//                      .'  \\|     |//  `.                       //
//                     /  \\|||  :  |||//  \                      //
//                    /  _||||| -:- |||||-  \                     //
//                    |   | \\\  -  /// |   |                     //
//                    | \_|  ''\---/''  |   |                     //
//                    \  .-\__  `-`  ___/-. /                     //
//                  ___`. .'  /--.--\  `. . ___                   //
//                ."" '<  `.___\_<|>_/___.'  >'"".                //
//              | | :  `- \`.;`\ _ /`;.`/ - ` : | |               //
//              \  \ `-.   \_ __\ /__ _/   .-` /  /               //
//        ========`-.____`-.___\_____/___.-`____.-'========       //
//                             `=---='                            //
//        ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^      //
//            佛祖保佑       无BUG        不修改                   //
////////////////////////////////////////////////////////////////////


export default class GameInput {

    //用于监听用户输入操作的节点
    private static eventNode: cc.Node  = null;

    //记录操作的起始位置
    private static prePoint: cc.Vec2   = null;

    //为调用者抛出用户操作的事件
    private static onVkey: Function    = null;

    private static timeCondition: TimeCondition = null;

    public static Init(): void
    {
        this.timeCondition = new TimeCondition();
        this.timeCondition.SetConditionTime(GameConfig.SLogicFPS);
    }

    public static Start(): void
    {
        this.eventNode = cc.find("Canvas");
        if(this.eventNode)
        {
            this.eventNode.on(cc.Node.EventType.TOUCH_START, this.onTouchStart, this);
            this.eventNode.on(cc.Node.EventType.TOUCH_MOVE, this.onTouchMove, this);
            this.eventNode.on(cc.Node.EventType.TOUCH_END, this.onTouchEnd, this);
        }

        //TODO
        // if(GlobalVar.userID == 820462)
        // {
        // this.onVkey(GlobalVar.userID, UserCmdType.UCT_Move, [1, 0]);

        // }else if(GlobalVar.userID == 820518)
        // {
        // this.onVkey(GlobalVar.userID, UserCmdType.UCT_Move, [-1, 0]);
        // }

        if(GlobalVar.gameModel === GameModel.GM_PracticeMode)
        {
            this.SendFirstWalkMessage();
        }
    }

    public static SendFirstWalkMessage()
    {
        this.onVkey(GlobalVar.userID, UserCmdType.UCT_Move, [0, 1]);
    }

    public static SetListener(listener: Function): void
    {
        this.onVkey = listener;
    }
    
    public static Destroy(): void
    {
        if(this.eventNode)
        {
            this.eventNode.off(cc.Node.EventType.TOUCH_START, this.onTouchStart, this);
            this.eventNode.off(cc.Node.EventType.TOUCH_MOVE, this.onTouchMove, this);
            this.eventNode.off(cc.Node.EventType.TOUCH_END, this.onTouchEnd, this);
    
            this.eventNode = null;
        }
    }

    private static onTouchStart(event): void
    {
        if(GameLogic.GetInstance().IsRunning() == false){
            return;
        }

        this.prePoint = event.getLocation();
    }

    private static onTouchMove(event): void
    {
        if(this.timeCondition.IsMeetCondition() === false)
        {
            return;
        }
        
        if(this.prePoint === null)
        {
            return;
        }

        let curPoint = event.getLocation();

        this.handleVKey(this.prePoint, curPoint);
    }

    private static onTouchEnd(event): void
    {
        this.prePoint = null;
    }

    private static handleVKey(startLocation: cc.Vec2, endLocation: cc.Vec2): void
    {   
        if(GameLogic.GetInstance().isCanInput === false)
        {
            return;
        }
        
        if(this.onVkey)
        {
            let subVec: cc.Vec2 = cc.v2();

            Util.Sub(endLocation, startLocation, subVec);

            if(subVec.x === 0.0 && subVec.y === 0.0)
            {
                return;
            }

            let linearVelocity: cc.Vec2 = subVec.normalize();

            let args = [];

            args.push(linearVelocity.x);
            args.push(linearVelocity.y);

            this.onVkey(GlobalVar.userID, UserCmdType.UCT_Move, args);
        }
    }
}
