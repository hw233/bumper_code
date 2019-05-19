import PopUpBase from './PopUpBase';
import EngineUtil from '../common/EngineUtil';
import Util from '../common/Util';
const {ccclass, property} = cc._decorator;

@ccclass
export default class TrialSucView extends PopUpBase
{
    @property(cc.Node)
    private timeNode: cc.Node = null;

    private time: number = null;

    public RefreshView()
    {
        super.RefreshView();

        this.time = Number(this.param);

        this.refreshTime();

        this.schedule(this.refreshTime, 1);
    }

    //
    private refreshTime()
    {
        if(this.time <= 0)
        {
            return;
        }

        EngineUtil.SetNodeText(this.timeNode, Util.ConvertTimeString(this.time));

        this.time --;
    }
}
