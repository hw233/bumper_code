export default class ConfigManager
{
    private static ConfigAddr: string = "";

    private static DownloadIndex: number = 0;

    private static Callback: Function = null;

    private static ConfigList: Array<string> = 
    [

    ];

    public static DownloadConfig(callback: Function = null)
    {   
        if(callback)
        {
            this.Callback = callback;
        }

        if(this.ConfigList[this.DownloadIndex])
        {
            let config = this.ConfigAddr + this.ConfigList[this.DownloadIndex];
            if(config)
            {
                cc.loader.load(config, this.DownloadFinish);
            }

            this.DownloadIndex ++;
        }
        else
        {
            if(this.Callback)
            {
                this.Callback();
            }
        }
    }

    public static DownloadFinish(error: Error, result: any)
    {
        this.DownloadConfig();
    }
}