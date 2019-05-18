import PlayerOpData from '../PlayerOpData';
import { UserCmdType } from '../GameDefine';

export default class CommandBase
{
    public constructor()
    {

    }

    //由派生类实现
    public DoExecute(dt: number, frameID: number, cmd: number, data: PlayerOpData, isQuickRun: boolean = false)
    {

    }

    //由派生类实现
    protected IsCanHandle(playerID: number, cmd: UserCmdType, frameID: number): boolean
    {
        return true;
    }
}