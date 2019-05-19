//////////////////////////////////////////////////////////////////////
//文件名称：UserData.h
//功能描述：
//版本说明：UserData API
//          
//编写作者：2018.10.10 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#ifndef __USERDATA_H__
#define __USERDATA_H__

#include <string>

#include "../Common/Type.h"
#include "../Common/BaseType.h"
//#include "Player.h"

#include "../DataBase/DBMySql.h"

enum _STATUS
{
    _STATUS_ALIVE,
    _STATUS_PREPARE_ALIVE,
    _STATUS_DEAD,
};

enum _GAME_STEP
{
    _GAME_STEP_NONE,
    _GAME_STEP_START,
    _GAME_STEP_LOADING_COMPLETE,
};

/////////////////////////////////////////////////////////////////////
//玩家表修改如下：
struct  PlayerInfoStruct
{
    UInt32      m_nId;
    UInt32      m_nUid;                 //uint  用户id
    std::string m_HeadPicurl;           //string 头像
    std::string m_NickName;             //string 昵称
    UInt32      m_nGoldSum;             //uint 金币数量  
    UInt32      m_nKillSum;             //uint 击杀数量
    UInt32      m_nGrandingId;          //uint 段位id
    UInt32      m_nCurStarSum;          //uint 当前星星数量
    UInt32      m_nCurUseCar;           //uint 当前使用中的车辆
    std::string m_UnlockSum;            //string 已解锁车辆

    UINT32      m_nKeepStar;
    UINT32      m_nGetGift;
    string      m_strLastDay;
    
    UInt32      m_nKingSum;             //uint 霸主令牌数量
    std::string m_GiftDay;              //string 当天获取礼包的次数
    UInt32      m_nGiftSum;             //uint 当天获取礼包的次数
    std::string m_StarDay;              //string 当天保留星星的次数
    UInt32      m_nStarSum;             //uint 当天保留星星的次数
    UInt32      m_nBuff;

    UINT32      m_nAddScore;            //uint  初始积分加成 3000
    UINT32      m_nAddGold;             //uint  战斗结束后金币加成百分比 100%
    UINT32      m_nRatio;               //uint  大乱斗结束时候奖励的倍率，默认1倍，看视频后3倍率  
    UINT32      m_nCarLevel;            //uint  大乱斗车辆当前等级
    UInt32      m_nKingRank;            //uint  霸主标识：100：霸主
    UINT32      m_nEnemyId;             //uint  仇人id  不用持久化

    //
    UInt32      m_nFightKillSum;        //uint  本局击杀数 初始0
    UInt32      m_nFightScore;          //uint  本局积分 初始1000
    UInt32      m_nFightLevel;          //uint  本局等级 初始1

    UInt32      m_nDeadScore;           //uint  死亡时积分
    UInt32      m_nFightTeamResult;     //uint  红蓝对战结果
    UInt32      m_nDeadStatus;          //uint  死亡状体
    UInt32      m_nGameStep;            //uint  游戏步骤     //saveinfo中没有保存

    UInt32      m_i32TaskMask;          //uint  永久任务掩码位32
    UInt32      m_i32WeekTaskMask;      //uint  每周任务掩码32
    UInt32      m_i32DayTaskMask;       //uint  每天任务掩码32    
    UInt32      m_i32DayTaskMask_1;     //uint 天任务掩码64
    UInt32      m_nLastResetTaskTime;   //uint 日常任务reset时间
    UInt32      m_nCurWeekNum;          //uint 当前是一年中第几周
    UInt32      m_nCurTmWday;           //uint 当前星期[0-6]:星期天-星期六
    UINT32      m_nShowIdx;             //uint 展示索引
    UINT32      m_nShowItemType;        //uint 展示的道具itemtype
    UINT32      m_nShowRemainTs;        //uint 展示的道具的剩余时间戳(秒)
    UINT32      m_nOnLineIdx;           //uint 在线领奖索引
    UINT32      m_nOnLineItemType;      //uint 在线领奖索引展示的道具itemtype
    UINT32      m_nOnLineRemainTs;      //uint 在线领奖索引展示的道具的剩余时间戳(秒)
    UINT32      m_nIsGoldTransformItem; //uint 金币转换道具 0 : 未转换 1 : 已经转换过
    UINT32      m_nTail;                //uint 当前使用的拖尾道具唯一id

    //add by gxx 2018.11.12:添加运营需求相关字段
    UInt32      m_nSignDays;            //签到天数
    //std::string m_LastSignDay;          //最新签到日期
    UInt32      m_nDay1;                 //第1天
    UInt32      m_nDay2;                 //第2天
    UInt32      m_nDay3;                 //第3天
    UInt32      m_nDay4;                 //第4天
    UInt32      m_nDay5;                 //第5天
    UInt32      m_nDay6;                 //第6天
    UInt32      m_nDay7;                 //第7天
    UInt32      m_nWeek;                //第几周
    UInt32      m_nDebrisSum;            //碎片
    UInt32      m_nExpSkin;              //皮肤体验权限天数
    UInt32      m_nBox1;                 //第1宝箱
    UInt32      m_nBox2;                 //第2宝箱
    UInt32      m_nBox3;                 //第3宝箱
    UInt32      m_nBox4;                 //第4宝箱
    //add end:添加运营需求相关字段
    
    //---------------不需要要存储数据库的数据------------------------------//
    // 玩家状态 0：下线 1:在线 2：游戏中
    UInt32      m_nGameStatus;
};

enum PLAYER_INFO_STRUCT{

    _PLAYER_INFO_STRUCT_ID = 0, 
    _PLAYER_INFO_STRUCT_UID,
    _PLAYER_INFO_STRUCT_HEAD_PICURL,
    _PLAYER_INFO_STRUCT_NICK_NAME,
    _PLAYER_INFO_STRUCT_GOLD_SUM,
    _PLAYER_INFO_STRUCT_KILL_SUM,
    _PLAYER_INFO_STRUCT_GRANDING_ID,
    _PLAYER_INFO_STRUCT_CUR_STAR_SUM,

    _PLAYER_INFO_STRUCT_CUR_USE_CAR,   

    _PLAYER_INFO_STRUCT_UNLOCK_SUM,
    

    _PLAYER_INFO_STRUCT_KEEP_STAR,
    _PLAYER_INFO_STRUCT_GET_GIFT,
    _PLAYER_INFO_STRUCT_LAST_DAY,
  
    _PLAYER_INFO_STRUCT_KING_SUM,
    _PLAYER_INFO_STRUCT_GIFT_DAY,
    _PLAYER_INFO_STRUCT_GIFT_SUM,
    _PLAYER_INFO_STRUCT_STAR_DAY,
    _PLAYER_INFO_STRUCT_STAR_SUM,
    _PLAYER_INFO_STRUCT_BUFF,

    _PLAYER_INFO_STRUCT_ADD_SCORE,
    _PLAYER_INFO_STRUCT_ADD_GOLD,
    _PLAYER_INFO_STRUCT_RATIO,
    _PLAYER_INFO_STRUCT_CUR_LEVEL,
    _PLAYER_INFO_STRUCT_KING_RANK,
    _PLAYER_INFO_STRUCT_ENEMY_ID,

    _PLAYER_INFO_STRUCT_FIGHT_KILL_SUM,
    _PLAYER_INFO_STRUCT_FIGHT_SCORE,
    _PLAYER_INFO_STRUCT_FIGHT_LEVEL,
 

    _PLAYER_INFO_STRUCT_DEAD_SCORE,    
    _PLAYER_INFO_STRUCT_FIGHT_TEAM_RESULT,
    _PLAYER_INFO_STRUCT_STATUS,

    _PLAYER_INFO_I32TASKMASK,
    _PLAYER_INFO_I32WEEKTASKMASK,
    _PLAYER_INFO_I32DAYTASKMASK,
    _PLAYER_INFO_I32DAYTASKMASK_1,
    _PLAYER_INFO_LASTRESETTASKTIME,
    _PLAYER_INFO_CURWEEKNUM,
    _PLAYER_INFO_CURTMWDAY,

    _PLAYER_INFO_SHOW_IDX,
    _PLAYER_INFO_SHOW_ITEMTYPE,
    _PLAYER_INFO_SHOW_REMAIN_TS,

    _PLAYER_INFO_ONLINE_IDX,
    _PLAYER_INFO_ONLINE_ITEMTYPE,
    _PLAYER_INFO_ONLINE_REMAIN_TS,
    _PLAYER_INFO_IS_GOLD_TRANSFORM_ITEM,
    _PLAYER_INFO_TAIL,

    _PLAYER_INFO_STRUCT_SIGN_DAYS,
    _PLAYER_INFO_STRUCT_DAY1,
    _PLAYER_INFO_STRUCT_DAY2,
    _PLAYER_INFO_STRUCT_DAY3,
    _PLAYER_INFO_STRUCT_DAY4,
    _PLAYER_INFO_STRUCT_DAY5,
    _PLAYER_INFO_STRUCT_DAY6,
    _PLAYER_INFO_STRUCT_DAY7,
    _PLAYER_INFO_STRUCT_WEEK,
    _PLAYER_INFO_STRUCT_DEBRIS,
    _PLAYER_INFO_STRUCT_EXP_SKIN,
    _PLAYER_INFO_STRUCT_BOX1,
    _PLAYER_INFO_STRUCT_BOX2,
    _PLAYER_INFO_STRUCT_BOX3,
    _PLAYER_INFO_STRUCT_BOX4,
    
};


/////////////////////////////////////////////////////////////////////
///clase
class CUserData
{

public:
    CUserData();
    ~CUserData();
    //static DBMySql* m_pRes;

protected:
#if 0
    BOOL    Create(IDatabase* pDb, UInt32 strUid, std::string& strHeadPicurl, std::string& strNickName);
#endif
public:
    BOOL    LoadInfo   (int nUid);
    BOOL    SaveInfo   ();
    BOOL    CleanUp    ();
    BOOL    InsertRecord(int nUid, std::string const& strHeadPicurl, std::string const& strNickName);
    //设置默认属性
    VOID    SetDefaultData();

 
public:
      
    //int  用户id
    VOID             SetUid(int nUid, BOOL bUpdate = TRUE);
    UInt32           GetUid() {       return m_Info.m_nUid;     }

    //string 头像
    VOID             SetHeadPicurl(std::string const& strHeadPicurl, BOOL bUpdate = TRUE);
    std::string&     GetHeadPicurl() {         return m_Info.m_HeadPicurl;      }

    //string 昵称
    VOID             SetNickName(std::string const& strNickName, BOOL bUpdate = TRUE);
    std::string&     GetNickName() {           return m_Info.m_NickName;     }

    //int 金币数量
    VOID             SetGoldSum(UInt32 nGoldSum, BOOL bUpdate = TRUE);
    UInt32           GetGoldSum() {           return m_Info.m_nGoldSum; }

    //int 击杀数量
    VOID             SetKillSum(UInt32 nKillSum, BOOL bUpdate = TRUE);
    UInt32           GetKillSum(){          return m_Info.m_nKillSum;    }

    //int 段位id
    VOID             SetGrandingId(UInt32 nGrandingId, BOOL bUpdate = TRUE);
    UInt32           GetGrandingId()      {    return m_Info.m_nGrandingId;  }

    //int 当前星星数量
    VOID             SetCurStarSum(UInt32 nCurStarSum, BOOL bUpdate = TRUE);
    UInt32           GetCurStarSum()        {    return m_Info.m_nCurStarSum;   }

    //string 已解锁车辆
    VOID             SetUnlockSum(std::string const& strUnlockSum, BOOL bUpdate = TRUE);
    std::string&     GetUnlockSum() {           return m_Info.m_UnlockSum;       }

    //int 当前使用中的车辆
    VOID             SetUseCar(UInt32 nUseCar, BOOL bUpdate = TRUE);
    UInt32           GetUseCar()      {    return m_Info.m_nCurUseCar;      }

    VOID             SetKeepStar(UInt32 nKeepStar, BOOL bUpdate = TRUE);
    UInt32           GetKeepStar() {         return m_Info.m_nKeepStar;     }
        
    VOID             SetGetGift(UInt32 nGetGift, BOOL bUpdate = TRUE);
    UInt32           GetGetGift() {        return m_Info.m_nGetGift;    }
           
    VOID             SetLastDay(std::string& strLastDay, BOOL bUpdate = TRUE);
    std::string&     GetLastDay() {         return m_Info.m_strLastDay;  }

    //int 霸主令牌数量
    VOID             SetKingSum(UInt32 nKingSum, BOOL bUpdate = TRUE);
    UInt32           GetKingSum() {         return m_Info.m_nKingSum;    }

    //string 当天获取礼包的次数
    VOID             SetGiftDay(std::string const& strGiftDay, BOOL bUpdate = TRUE);
    std::string&     GetGiftDay()     {   return m_Info.m_GiftDay;      }

    //int 当天获取礼包的次数
    VOID             SetGiftSum(UInt32 nGiftSum, BOOL bUpdate = TRUE);
    UInt32           GetGiftSum() {         return m_Info.m_nGiftSum;   }               

    //string 当天保留星星的次数
    VOID             SetStarDay(std::string const& strStarDay, BOOL bUpdate = TRUE);
    std::string&     GetStarDay() {          return m_Info.m_StarDay;    }

    //int 当天保留星星的次数
    VOID             SetStarSum(UInt32 nStarSum, BOOL bUpdate = TRUE);
    UInt32           GetStarSum()       {    return m_Info.m_nStarSum;   }

    //int  本局击杀数 初始0
    VOID             SetFightKillSum(UInt32 nFightKillSum, BOOL bUpdate = TRUE);
    UInt32           GetFightKillSum() {          return m_Info.m_nFightKillSum;    }

    //int  本局积分 初始1000
    VOID             SetFightScore(UInt32 nFightScore, BOOL bUpdate = TRUE);
    UInt32           GetFightScore() {          return m_Info.m_nFightScore;        }

    //int  本局等级 初始1
    VOID             SetFightLevel(UInt32 nFightLevel, BOOL bUpdate = TRUE);
    UInt32           GetFightLevel() {      return m_Info.m_nFightLevel;   }  

    //int  初始积分加成 3000
    VOID             SetAddScore(UInt32 nAddScore, BOOL bUpdate = TRUE);
    UInt32           GetAddScore() {          return m_Info.m_nAddScore;        }

    //int  战斗结束后金币加成百分比 100%
    VOID             SetAddGold(UInt32 nAddGold, BOOL bUpdate = TRUE);
    UInt32           GetAddGold() {         return m_Info.m_nAddGold;       }
          
    //int  大乱斗结束时候奖励的倍率，默认1倍，看视频后3倍率   
    VOID             SetRatio(UInt32 nRatio, BOOL bUpdate = TRUE);
    UInt32           GetRatio()    {    return m_Info.m_nRatio;       }            

    //int  大乱斗chelaingdangqiandengji  
    VOID             SetCarLevel(UInt32 nCarLevel, BOOL bUpdate = TRUE);
    UInt32           GetCarLevel() {        return m_Info.m_nCarLevel;     }   
    
    //int  死亡状体 
    VOID             SetDeadStatus(UInt32 nDeadStatus, BOOL bUpdate = TRUE);
    UInt32           GetDeadStatus() {      return m_Info.m_nDeadStatus;      } 
    
    //int  游戏步骤
    VOID             SetGameStep(UInt32 nGameStep, BOOL bUpdate = TRUE);
    UInt32           GetGameStep() {      return m_Info.m_nGameStep;      } 

    //int  BUFF 
    VOID             SetBuff(UInt32 nBuff, BOOL bUpdate = TRUE);
    UInt32           GetBuff() {        return m_Info.m_nBuff;        } 

    //int  霸主标识 
    VOID             SetKingRank(UInt32 nKingRank, BOOL bUpdate = TRUE);
    UInt32           GetKingRank()       {    return m_Info.m_nKingRank;    }  

    //int  仇人id  
    VOID             SetEnemyId(UInt32 nEnemyId, BOOL bUpdate = TRUE);
    UInt32           GetEnemyId() {        return m_Info.m_nEnemyId;      }   

    //int  死亡时积分
    VOID             SetDeadScore(UInt32 nDeadScore, BOOL bUpdate = TRUE);
    UInt32           GetDeadScore() {           return m_Info.m_nDeadScore;      }  

    ////int 红蓝组对时战斗结果
    VOID             SetFightTeamResult(UInt32 nFightTeamResult, BOOL bUpdate = TRUE);
    UInt32           GetFightTeamResult() { return m_Info.m_nFightTeamResult; }

    //永久任务掩码位
    VOID            SetTaskMask(UInt32 i32Mask, BOOL bUpdate = TRUE);   
    UInt32          GetTaskMask()  { return m_Info.m_i32TaskMask; }

    //每周任务掩码位1
    VOID             SetWeekTaskMask(UInt32 i32WeekMask, BOOL bUpdate = TRUE);   
    UInt32           GetWeekTaskMask()  { return m_Info.m_i32WeekTaskMask; }

    //每日任务掩码位1
    VOID             SetDayTaskMask(UInt32 i32DayMask, BOOL bUpdate = TRUE);   
    UInt32           GetDayTaskMask()  { return m_Info.m_i32DayTaskMask; }

    //每日任务掩码位2
    VOID             SetDayTaskMask1(UInt32 i32DayMask_1, BOOL bUpdate = TRUE); 
    UInt32           GetDayTaskMask1()  { return m_Info.m_i32DayTaskMask_1; }
   
    //每日任务掩码位reset时间
    VOID             SetLastResetTaskTime(BOOL bUpdate = TRUE); 
    UInt32           GetLastResetTaskTime()  { return m_Info.m_nLastResetTaskTime; }

    //当前是一年中的第几周
    VOID             SetCurWeekNum(BOOL bUpdate = TRUE); 
    UInt32           GetCurWeekNum()  { return m_Info.m_nCurWeekNum; } 

    //当前星期[0-6]:星期天-星期六
    VOID             SetCurTmWday(UInt32 nCurTmWday, BOOL bUpdate = TRUE); 
    UInt32           GetCurTmWday()  { return m_Info.m_nCurTmWday; }          

    //展示索引
    VOID             SetShowIdx(UInt32 nShowIdx, BOOL bUpdate = TRUE); 
    UInt32           GetShowIdx()  { return m_Info.m_nShowIdx; }  

    //展示的道具
    VOID             SetShowItemType(UInt32 nShowItemType, BOOL bUpdate = TRUE); 
    UInt32           GetShowItemType()  { return m_Info.m_nShowItemType; }  

    //展示的道具的剩余时间戳(秒)
    VOID             SetShowRemainTs(UInt32 nShowRemainTs, BOOL bUpdate = TRUE); 
    UInt32           GetShowRemainTs()  { return m_Info.m_nShowRemainTs; }

    //在线领奖索引
    VOID             SetOnLineIdx(UInt32 nOnLineIdx, BOOL bUpdate = TRUE); 
    UInt32           GetOnLineIdx()  { return m_Info.m_nOnLineIdx; }  

    //在线领奖索引展示的道具itemtype
    VOID             SetOnLineItemType(UInt32 nOnLineItemType, BOOL bUpdate = TRUE); 
    UInt32           GetOnLineItemType()  { return m_Info.m_nOnLineItemType; }  

    //在线领奖索引展示的道具的剩余时间戳(秒)
    VOID             SetOnLineRemainTs(UInt32 nOnLineRemainTs, BOOL bUpdate = TRUE); 
    UInt32           GetOnLineRemainTs()  { return m_Info.m_nOnLineRemainTs; }

    //uint 金币转换道具 0 : 未转换 1 : 已经转换过
    VOID             SetIsGoldTransformItem(UInt32 nIsGoldTransformItem, BOOL bUpdate = TRUE); 
    UInt32           GetIsGoldTransformItem()  { return m_Info.m_nIsGoldTransformItem; }

    //uint 当前使用的拖尾道具唯一id
    VOID             SetTail(UInt32 nTail, BOOL bUpdate = TRUE); 
    UInt32           GetTail()  { return m_Info.m_nTail; }

    //int 签到天数
    VOID             SetSignDays(UInt32 nSignDays, BOOL bUpdate = TRUE);
    UInt32           GetSignDays(){    return m_Info.m_nSignDays;             }
    
    /*//string 最后签到日期
    VOID             SetLastSignDay(std::string const& strLastSignDay, BOOL bUpdate = TRUE);
    std::string&     GetLastSignDay(){   return m_Info.m_LastSignDay;   }*/

    //int 签到7天标志 第几周
    VOID             SetWeek(UInt32 nWeek, BOOL bUpdate = TRUE);
    UInt32           GetWeek(){    return m_Info.m_nWeek; }

    //int 第1天
    VOID             SetDay1(UInt32 nDay1, BOOL bUpdate = TRUE);
    UInt32           GetDay1(){    return m_Info.m_nDay1; }

     //int 第2天
    VOID             SetDay2(UInt32 nDay2, BOOL bUpdate = TRUE);
    UInt32           GetDay2(){    return m_Info.m_nDay2; }

     //int 第3天
    VOID             SetDay3(UInt32 nDay3, BOOL bUpdate = TRUE);
    UInt32           GetDay3(){    return m_Info.m_nDay3; }

     //int 第4天
    VOID             SetDay4(UInt32 nDay4, BOOL bUpdate = TRUE);
    UInt32           GetDay4(){    return m_Info.m_nDay4; }

    //int 第5天
    VOID             SetDay5(UInt32 nDay5, BOOL bUpdate = TRUE);
    UInt32           GetDay5(){    return m_Info.m_nDay5; }

    //int 第6天
    VOID             SetDay6(UInt32 nDay6, BOOL bUpdate = TRUE);
    UInt32           GetDay6(){    return m_Info.m_nDay6; }

    //int 第7天
    VOID             SetDay7(UInt32 nDay7, BOOL bUpdate = TRUE);
    UInt32           GetDay7(){    return m_Info.m_nDay7; }

    //int 碎片数量
    VOID             SetDebris(UInt32 nDebrisSum, BOOL bUpdate = TRUE);
    UInt32           GetDebris(){    return m_Info.m_nDebrisSum; }

    //int 皮肤体验天数
    VOID             SetExpSkin(UInt32 m_nExpSkin, BOOL bUpdate = TRUE);
    UInt32           GetExpSkin(){    return m_Info.m_nExpSkin; }

    //int 第1宝箱
    VOID             SetBox1(UInt32 nBox1, BOOL bUpdate = TRUE);
    UInt32           GetBox1(){    return m_Info.m_nBox1; }

    //int 第2宝箱
    VOID             SetBox2(UInt32 nBox2, BOOL bUpdate = TRUE);
    UInt32           GetBox2(){    return m_Info.m_nBox2; }

    //int 第3宝箱
    VOID             SetBox3(UInt32 nBox3, BOOL bUpdate = TRUE);
    UInt32           GetBox3(){    return m_Info.m_nBox3; }

    //int 第4宝箱
    VOID             SetBox4(UInt32 nBox4, BOOL bUpdate = TRUE);
    UInt32           GetBox4(){    return m_Info.m_nBox4; }


    //---------------不需要要存储数据库的数据------------------------------//
    // 玩家状态 0：下线 1:在线 2：游戏中
    VOID             SetGameStatus(UInt32 GameStatus, BOOL bUpdate = TRUE);
    UInt32           GetGameStatus(){  return m_Info.m_nGameStatus; }

    PlayerInfoStruct&       GetInfo(){   return m_Info;      }
protected:
    PlayerInfoStruct        m_Info;         //结构size:344 Now;
//    IRecord*                m_pRes;
//    DBMySql*                m_pRes;
    
   

    //friend class Player;     // 所有内容由Player操作
};

char* getCurrentYearMonDay();


#endif // __USERDATA_H__
