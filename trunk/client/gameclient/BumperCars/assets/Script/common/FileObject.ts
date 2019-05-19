class FileObject
{
    private m_FileOp: any = null;
    private m_FileHandle: any = null;

    constructor()
    {
        if(CC_WECHATGAME)
        {

        }
        else
        {
            // this.m_FileOp = new ActiveXObject("Scripting.FileSystemObject");
        }
    }

    /**
     * 创建一个目录
     * @param dirName 目录路径
     */
    public CreateFolder(dirName: string)
    {
        if(CC_WECHATGAME)
        {

        }else
        {
            this.m_FileOp.CreateFolder(dirName);
        }
    }

    /**
     * 创建一个文件
     * @param fileName 文件名称
     */
    public CreateFile(fileName: string)
    {
        this.m_FileOp.CreateTextFile(fileName);
    }

    /**
     * 打开一个文件
     * @param fileName 文件名称
     */
    public OpenFile(fileName:string)
    {
        this.m_FileHandle = this.m_FileOp.OpenTextFile(fileName, 2, true);
    }

    /**
     * 保存数据到一个文件
     * @param fileName 文件名称
     * @param data 数据
     */
    public SaveData(fileName: string, data: string)
    {
        if(this.m_FileHandle)
        {
            this.m_FileHandle.WriteLine(data);
        }
    }

    /**
     * 关闭文件
     */
    public CloseFile()
    {
        if(this.m_FileHandle)
        {
            this.m_FileHandle.Close();
        }
    }
}