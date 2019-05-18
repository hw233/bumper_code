import GameConfig from "../GameConfig";

//声音类型
export enum SoundType{
    ST_BGMusic          = 1,  //背景音乐

    ST_CommonBtn        = 2,    
    ST_CountDown        = 3,
    ST_AddStar          = 4,
    ST_ReduceStar       = 5,
    ST_Collider         = 6,
    ST_DoubleKill       = 7,
    ST_ThreeKill        = 8,
    ST_FourKill         = 9,
    ST_FiveKill         = 10,
    ST_LuoShui          = 11,
    ST_UpLevel          = 12,
    ST_Nomber_One       = 13,
    ST_Ready            = 14,
    ST_Victory          = 15,
    ST_Dogfall          = 16,
    ST_Defeat           = 17,
    ST_Pick_Item        = 18,
};

//声音文件路径
let SoundFile = {
};

SoundFile[SoundType.ST_BGMusic]        = "bgm";
SoundFile[SoundType.ST_CommonBtn]      = "btnClick";
SoundFile[SoundType.ST_CountDown]      = "countdown_0";
SoundFile[SoundType.ST_AddStar]        = "jiax";
SoundFile[SoundType.ST_ReduceStar]     = "jianx";
SoundFile[SoundType.ST_Collider]       = "pengz";
SoundFile[SoundType.ST_DoubleKill]     = "shuangs";
SoundFile[SoundType.ST_ThreeKill]      = "sans";
SoundFile[SoundType.ST_FourKill]       = "fengkuang";
SoundFile[SoundType.ST_FiveKill]       = "baozou";
SoundFile[SoundType.ST_LuoShui]        = "luos";
SoundFile[SoundType.ST_UpLevel]        = "upLevel";
SoundFile[SoundType.ST_Nomber_One]     = "rank_1";
SoundFile[SoundType.ST_Ready]          = "countdown";
SoundFile[SoundType.ST_Victory]        = "victory";
SoundFile[SoundType.ST_Dogfall]        = "dogfall";
SoundFile[SoundType.ST_Defeat]         = "defeat";
SoundFile[SoundType.ST_Pick_Item]      = "pickItem";

export default class AudioManager
{
    //是否打开声音
    private static IsOpenSound:boolean = true;

    //是否打开音效
    public static IsOpenEffect:boolean = true;

    private static curBgm = null;

    //播放音效
    public static PlaySoundEffect(type: SoundType){
        let soundFile = SoundFile[type];
        if(soundFile){
            this.playEffect(soundFile);
        }
    }

    //通用按钮音效
    public static PlayBtnSound(){
        this.PlaySoundEffect(SoundType.ST_CommonBtn);
    }

    //播放-背景-音乐
    public static PlayBackgroundSound(){
        if(this.curBgm != null){
            return;
        }

        let soundFile = SoundFile[SoundType.ST_BGMusic];
        if(soundFile){
            this.curBgm = this.playBackgroundMusic(soundFile);
        }
    }

    //停止-背景-音乐
    public static StopBackgroundSound(){
        if(this.curBgm != null){
            cc.audioEngine.stop(this.curBgm);
            this.curBgm = null;
        }
    }

    //暂停-背景-音乐
    public static PauseBackgroundSound(){
        if(this.curBgm != null)
            cc.audioEngine.pause(this.curBgm);
    }

    //恢复-背景-音乐
    public static ResumeBackgroundSound(){
        if(this.curBgm != null)
            cc.audioEngine.resume(this.curBgm);
    }

    //打开声音
    public static OpenSound(){
        this.IsOpenSound = true;
        // this.resumeBackgroundSound();
    }

    //关闭声音
    public static CloseSound(){
        this.IsOpenSound = false;
        // this.pauseBackgroundSound();
    }

    //打开音效
    public static OpenEffect(){
        this.IsOpenEffect = true;
    }

    //关闭音效
    public static CloseEffect(){
        this.IsOpenEffect = false;
    }

    private static playEffect(audio){
        if(!GameConfig.SoundSwitch)
            return;
        if(!this.IsOpenEffect)
            return;
        if(!audio){
            return;
        }

        let url = "music/" + audio;

        cc.loader.loadRes(url, cc.AudioClip, function (err, clip) {
            let audioID = cc.audioEngine.play(clip, false, 1);
        });
    }

    private static playBackgroundMusic(audio){
        if(!GameConfig.SoundSwitch)
            return;
        if(!this.IsOpenEffect)
            return null;
        if(!audio){
            return;
        }

        let self = this;

        let url = "music/" + audio;

        cc.loader.loadRes(url, cc.AudioClip, function (err, clip) {
            self.curBgm = cc.audioEngine.play(clip, true, 1);
        });
    }
};