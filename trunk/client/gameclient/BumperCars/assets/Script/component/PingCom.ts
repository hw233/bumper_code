import ComponentBase from '../common/ComponentBase';
import { MessageType } from '../common/EventCenter';
import GameConfig from '../GameConfig';
import EngineUtil from '../common/EngineUtil';

const {ccclass, property} = cc._decorator;

@ccclass
export default class PingCom extends ComponentBase
{
    @property(cc.Node)
    private pingValueNode: cc.Node = null

    protected onLoad()
    {
        this.schedule(this.sendPing, GameConfig.PingInvatal);

        this.registerEvent(MessageType.PING_VALUE_CHANGE_EVENT, this.onPingChanged.bind(this));
    }

    private sendPing()
    {

    }

    private onPingChanged()
    {
        let pingValue: number = 100;

        let pingIconID: number = 1;

        if(pingValue <= 100)
        {
            pingIconID = 1;
        }else if(pingValue <= 200)
        {
            pingIconID = 2;
        }else
        {
            pingIconID = 3;
        }

        EngineUtil.SetNodeText(this.pingValueNode, pingValue.toString());
        EngineUtil.SetNodeTexture(this.node, "gamescene/PingIcon_" + pingIconID);
    }
}
