// import FileObject from "./FileObject"

class FileUtil
{
    private static instance: FileUtil = null;

    private static GameRootPath: string = "C:\\Users\\User\\AppData\\Local\\Bumper2";

    private m_LogFilePath: string = null;

    private m_FileObject: FileObject = null;

    constructor()
    {
        // this.m_FileObject = new FileObject();
    }

    public static GetInstance()
    {
        if(!this.instance)
        {
            this.instance = new FileUtil();
        }
        return this.instance;
    }

    public SaveLog(logInfo: string)
    {
        // if(this.m_LogFilePath === null)
        // {
        //     let fileName: string = "GameLog---" + (new Date()).toLocaleString();

        //     this.m_LogFilePath = FileUtil.GameRootPath + "\\" + fileName;

        //     this.m_FileObject.CreateFolder(FileUtil.GameRootPath);

        //     this.m_FileObject.CreateFile(this.m_LogFilePath);
        // }

        // if(this.m_LogFilePath)
        // {
        //     this.m_FileObject.OpenFile(this.m_LogFilePath);
        //     this.m_FileObject.SaveData("", logInfo);
        //     this.m_FileObject.CloseFile();
        // }
    }
}