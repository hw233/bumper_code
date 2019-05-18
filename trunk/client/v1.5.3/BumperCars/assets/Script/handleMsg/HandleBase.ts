import HandleMgr from '../network/socket/HandleMgr';

//消息管理者基类(每一个派生类需在RegisterHandler类里注册一下)
export default class HandleBase
{
    constructor()
    {
        // HandleMgr.RegisterHandler(this);

        this.RegisterAllHandle();
    }

    //注册消息处理函数
    protected RegisterHandle(msgID, msgType, callback)
    {
        HandleMgr.RigisterMessageHandle(msgID, msgType, callback);
    }

    //需要派生类实现
    public RegisterAllHandle()
    {

    }
}
