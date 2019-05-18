import ComponentBase from '../common/ComponentBase';

const {ccclass, property} = cc._decorator;

//新消息
@ccclass
export default class NewMsgCom extends ComponentBase
{
    @property(cc.Node)
    private newMsgImg: cc.Node = null;

    @property(cc.String)
    private newMsgType: string = "";

    protected onLoad()
    {
        this.onMessage(false);

        this.registerEvent(this.newMsgType, this.onMessage.bind(this));
    }

    private onMessage(isHasNesMsg: boolean)
    {
        if(this.newMsgImg)
        {
            this.newMsgImg.active = isHasNesMsg;
        }
    }
}