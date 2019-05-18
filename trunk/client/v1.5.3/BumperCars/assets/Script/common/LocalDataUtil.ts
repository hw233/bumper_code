export default class LocalDataUtil
{
    constructor()
    {

    }

    public ReadData(key: string): string
    {
        return cc.sys.localStorage.getItem(key);
    }

    public SaveData(key: string, data: string)
    {
        cc.sys.localStorage.setItem(key, data);
    }

    public RemoveData(key: string)
    {
        cc.sys.localStorage.removeItem(key);
    }
}