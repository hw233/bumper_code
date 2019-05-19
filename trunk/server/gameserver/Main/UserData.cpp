#include "../MsgHandler/NetMsg.h"
#include "../Common/Config.h"
#include "../Common/Common.h"
#include "../Main/UserData.h"
#include "ClientManager.h"
#include "../DataBase/DBMySql.h"
#include "../Protocol/Command.h"
#include "../Protocol/cJSON.h"
#include <stdio.h>                      //sprintf

////////////////////////////////////////////////////////////////////
CUserData::CUserData()
{
    //memset(&m_Info, 0, sizeof(PlayerInfoStruct));  
    SetDefaultData();  
}

//////////////////////////////////////////////////////////////////////
CUserData::~CUserData()
{
}

BOOL CUserData::CleanUp()
{
    //memset(&m_Info, 0, sizeof(PlayerInfoStruct));    
    SetDefaultData();     
    return TRUE;
}

BOOL CUserData::InsertRecord(int nUid, std::string const& strHeadPicurl, std::string const& strNickName)
{
    char szSql[1024];
    memset(szSql, 0, 1024);
    sprintf(szSql, "INSERT INTO g_bumper_info(user_id, head_picurl, nick_name) VALUES (%d,\"%s\",\"%s\")", nUid, strHeadPicurl.c_str(), strNickName.c_str());
    if(Database()->UpdateRecord(szSql)) 
    {
        LOGDEBUG("[CUserData::InsertRecord] Is Suc... sql=[%s]\n", szSql);
    }
    else 
    {
        LOGERROR("[CUserData::InsertRecord] Is Error... sql=[%s]\n", szSql);
        return FALSE;
    }
    return TRUE;
}



//////////////////////////////////////////////////////////////////////

BOOL CUserData::LoadInfo(int nUid) 
{
    if (!Database())
    { 
         return FALSE;
    }

    std::string sql = string("SELECT * FROM g_bumper_info WHERE user_id=")+std::to_string(m_Info.m_nUid);      
    MYSQL_RES* result = Database()->Query(sql);
    if(result == NULL)
    {
        LOGERROR("[CUserData::LoadInfo] sql error: %s\n", sql.c_str());
        return FALSE;
    }

    BOOL bExist = FALSE;        
    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)))
    {    
        // m_Info.m_nUid = SafeAtoi(row[_PLAYER_INFO_STRUCT_UID]);

        if(row[_PLAYER_INFO_STRUCT_HEAD_PICURL] && strlen(row[_PLAYER_INFO_STRUCT_HEAD_PICURL]))
        {
            m_Info.m_HeadPicurl = row[_PLAYER_INFO_STRUCT_HEAD_PICURL];
        }
        else 
        {
            //LOGERROR("[CUserData::LoadInfo] _PLAYER_INFO_STRUCT_HEAD_PICURL: %s\n", row[_PLAYER_INFO_STRUCT_HEAD_PICURL]);
            m_Info.m_HeadPicurl = "default";
        }
        
        if(row[_PLAYER_INFO_STRUCT_NICK_NAME] && strlen(row[_PLAYER_INFO_STRUCT_NICK_NAME]))
        {
            m_Info.m_NickName = row[_PLAYER_INFO_STRUCT_NICK_NAME];
        }
        else 
        {
            //LOGERROR("[CUserData::LoadInfo] _PLAYER_INFO_STRUCT_NICK_NAME: %s\n", row[_PLAYER_INFO_STRUCT_HEAD_PICURL]);
            m_Info.m_NickName = "default";
        }
        
        m_Info.m_nGoldSum = SafeAtoi(row[_PLAYER_INFO_STRUCT_GOLD_SUM]);
        m_Info.m_nKillSum = SafeAtoi(row[_PLAYER_INFO_STRUCT_KILL_SUM]);
        m_Info.m_nGrandingId = SafeAtoi(row[_PLAYER_INFO_STRUCT_GRANDING_ID]);
        m_Info.m_nCurStarSum = SafeAtoi(row[_PLAYER_INFO_STRUCT_CUR_STAR_SUM]);

        if(row[_PLAYER_INFO_STRUCT_UNLOCK_SUM] && strlen(row[_PLAYER_INFO_STRUCT_UNLOCK_SUM]))
        {
            m_Info.m_UnlockSum = row[_PLAYER_INFO_STRUCT_UNLOCK_SUM];
        }
        else 
        {
            m_Info.m_UnlockSum = "";
        }

        m_Info.m_nGetGift = SafeAtoi(row[_PLAYER_INFO_STRUCT_GET_GIFT]);
        m_Info.m_nKeepStar = SafeAtoi(row[_PLAYER_INFO_STRUCT_KEEP_STAR]);

#if 0
        if(row[_PLAYER_INFO_STRUCT_KEEP_STAR] && strlen(row[_PLAYER_INFO_STRUCT_KEEP_STAR]))
        {
            m_Info.m_strLastDay = row[_PLAYER_INFO_STRUCT_KEEP_STAR];
        }
#endif

        m_Info.m_strLastDay = row[_PLAYER_INFO_STRUCT_LAST_DAY];
        
        m_Info.m_nCurUseCar = SafeAtoi(row[_PLAYER_INFO_STRUCT_CUR_USE_CAR]);
        m_Info.m_nKingSum = SafeAtoi(row[_PLAYER_INFO_STRUCT_KING_SUM]);
        
        if(row[_PLAYER_INFO_STRUCT_GIFT_DAY] && strlen(row[_PLAYER_INFO_STRUCT_GIFT_DAY]))
        {
            m_Info.m_GiftDay = row[_PLAYER_INFO_STRUCT_GIFT_DAY];
        }
        else 
        {
            m_Info.m_GiftDay = "0";
        }
            
        m_Info.m_nGiftSum = SafeAtoi(row[_PLAYER_INFO_STRUCT_GIFT_SUM]);
        
        if(row[_PLAYER_INFO_STRUCT_STAR_DAY] && strlen(row[_PLAYER_INFO_STRUCT_STAR_DAY]))
        {
            m_Info.m_StarDay = row[_PLAYER_INFO_STRUCT_STAR_DAY];
        } 
        else 
        {
            m_Info.m_StarDay = "0";
        }
        
        m_Info.m_nStarSum = SafeAtoi(row[_PLAYER_INFO_STRUCT_STAR_SUM]);
        m_Info.m_nBuff = SafeAtoi(row[_PLAYER_INFO_STRUCT_BUFF]);

        m_Info.m_nAddScore = SafeAtoi(row[_PLAYER_INFO_STRUCT_ADD_SCORE]);
        m_Info.m_nAddGold = SafeAtoi(row[_PLAYER_INFO_STRUCT_ADD_GOLD]);
        m_Info.m_nRatio = SafeAtoi(row[_PLAYER_INFO_STRUCT_RATIO]);
        m_Info.m_nCarLevel = SafeAtoi(row[_PLAYER_INFO_STRUCT_CUR_LEVEL]);       
        m_Info.m_nKingRank = SafeAtoi(row[_PLAYER_INFO_STRUCT_KING_RANK]);
        m_Info.m_nEnemyId = SafeAtoi(row[_PLAYER_INFO_STRUCT_ENEMY_ID]);

        m_Info.m_nFightKillSum = SafeAtoi(row[_PLAYER_INFO_STRUCT_FIGHT_KILL_SUM]);
        m_Info.m_nFightScore = SafeAtoi(row[_PLAYER_INFO_STRUCT_FIGHT_SCORE]);
        m_Info.m_nFightLevel = SafeAtoi(row[_PLAYER_INFO_STRUCT_FIGHT_LEVEL]);
        m_Info.m_nDeadStatus = SafeAtoi(row[_PLAYER_INFO_STRUCT_STATUS]);

        m_Info.m_i32TaskMask = SafeAtoi(row[_PLAYER_INFO_I32TASKMASK]);          //永久任务掩码位32
        m_Info.m_i32WeekTaskMask = SafeAtoi(row[_PLAYER_INFO_I32WEEKTASKMASK]);       //每周任务掩码32
        m_Info.m_i32DayTaskMask = SafeAtoi(row[_PLAYER_INFO_I32DAYTASKMASK]);       //天任务掩码32
        m_Info.m_i32DayTaskMask_1 = SafeAtoi(row[_PLAYER_INFO_I32DAYTASKMASK_1]);     //天任务掩码64
        m_Info.m_nLastResetTaskTime = SafeAtoi(row[_PLAYER_INFO_LASTRESETTASKTIME]);   //日常任务reset时间
        m_Info.m_nCurWeekNum = SafeAtoi(row[_PLAYER_INFO_CURWEEKNUM]);                   //当前是一年中第几周 
        m_Info.m_nCurTmWday = SafeAtoi(row[_PLAYER_INFO_CURTMWDAY]);                   //当前星期[0-6]:星期天-星期六
        m_Info.m_nShowIdx = SafeAtoi(row[_PLAYER_INFO_SHOW_IDX]);                      //展示索引
        m_Info.m_nShowItemType = SafeAtoi(row[_PLAYER_INFO_SHOW_ITEMTYPE]);            //展示的道具itemtype
        m_Info.m_nShowRemainTs = SafeAtoi(row[_PLAYER_INFO_SHOW_REMAIN_TS]);           //展示的道具的剩余时间戳(秒)
        m_Info.m_nOnLineIdx = SafeAtoi(row[_PLAYER_INFO_ONLINE_IDX]);                      //展示索引
        m_Info.m_nOnLineItemType = SafeAtoi(row[_PLAYER_INFO_ONLINE_ITEMTYPE]);            //展示的道具itemtype
        m_Info.m_nOnLineRemainTs = SafeAtoi(row[_PLAYER_INFO_ONLINE_REMAIN_TS]);           //展示的道具的剩余时间戳(秒)
        m_Info.m_nIsGoldTransformItem = SafeAtoi(row[_PLAYER_INFO_IS_GOLD_TRANSFORM_ITEM]); //uint 金币转换道具 0 : 未转换 1 : 已经转换过
        m_Info.m_nTail = SafeAtoi(row[_PLAYER_INFO_TAIL]); //uint 当前使用的拖尾道具唯一id

        // /*...xiaoxiao...*/
        // m_Info.m_nSignDays = SafeAtoi(row[_PLAYER_INFO_STRUCT_SIGN_DAYS]);
        // /*if(row[_PLAYER_INFO_STRUCT_LAST_SIGN_DAY] && strlen(row[_PLAYER_INFO_STRUCT_LAST_SIGN_DAY]))
        // {
        //     m_Info.m_LastSignDay = row[_PLAYER_INFO_STRUCT_LAST_SIGN_DAY];
        // }*/
        // m_Info.m_nDay1 = SafeAtoi(row[_PLAYER_INFO_STRUCT_DAY1]);
        // m_Info.m_nDay2 = SafeAtoi(row[_PLAYER_INFO_STRUCT_DAY2]); 
        // m_Info.m_nDay3 = SafeAtoi(row[_PLAYER_INFO_STRUCT_DAY3]); 
        // m_Info.m_nDay4 = SafeAtoi(row[_PLAYER_INFO_STRUCT_DAY4]); 
        // m_Info.m_nDay5 = SafeAtoi(row[_PLAYER_INFO_STRUCT_DAY5]); 
        // m_Info.m_nDay6 = SafeAtoi(row[_PLAYER_INFO_STRUCT_DAY6]); 
        // m_Info.m_nDay7 = SafeAtoi(row[_PLAYER_INFO_STRUCT_DAY7]);   
        // m_Info.m_nWeek = SafeAtoi(row[_PLAYER_INFO_STRUCT_WEEK]);

        LOGDEBUG("[CUserData::LoadInfo] uid=%d, head_picurl=%s, nick_name=%s, gold_sum=%d, kill_sum=%d, granding_id=%d, \
                cur_star_sum=%d, cur_use_car=%d,unlock_cars=%s, keep_star=%d, get_gift=%d, last_day=%s, king_sum=%d, gift_day=%s, gift_sum=%d, star_day=%s, \
                star_sum=%d, buff=%d, add_score=%d, add_gold=%d, ratio=%d, car_level=%d, fight_kill_sum=%d, fight_score=%d, dead_score=[%d]\
                fight_level=%d, status=%d, king_rank=%d, enemy_id=%d task_mask=%d week_task_mask=[%d] day_task_mask=[%d] day_task_mask1=[%d] last_reset_task_time=[%d] cur_week_num=[%d] cur_tm_wday=[%d] show_idx=[%d] show_item_type=[%d] show_remain_ts=[%d]\
                online_idx=[%d] online_item_type=[%d] online_remain_ts=[%d] is_gold_transform_item=[%d] tail=[%d]", 
                m_Info.m_nUid, m_Info.m_HeadPicurl.c_str(), m_Info.m_NickName.c_str(), m_Info.m_nGoldSum, m_Info.m_nKillSum, 
                m_Info.m_nGrandingId, m_Info.m_nCurStarSum, m_Info.m_nCurUseCar, m_Info.m_UnlockSum.c_str(), 
                m_Info.m_nKeepStar, m_Info.m_nGetGift, m_Info.m_strLastDay.c_str(), m_Info.m_nKingSum, 
                m_Info.m_GiftDay.c_str(), m_Info.m_nGiftSum, m_Info.m_StarDay.c_str(), m_Info.m_nStarSum, m_Info.m_nBuff, m_Info.m_nAddScore, 
                m_Info.m_nAddGold, m_Info.m_nRatio, m_Info.m_nCarLevel, m_Info.m_nFightKillSum, m_Info.m_nFightScore, m_Info.m_nDeadScore, 
                m_Info.m_nFightLevel, m_Info.m_nDeadStatus, m_Info.m_nKingRank, m_Info.m_nEnemyId,m_Info.m_i32TaskMask,m_Info.m_i32WeekTaskMask,m_Info.m_i32DayTaskMask,m_Info.m_i32DayTaskMask_1,
                m_Info.m_nLastResetTaskTime,m_Info.m_nCurWeekNum,m_Info.m_nCurTmWday,m_Info.m_nShowIdx,m_Info.m_nShowItemType,m_Info.m_nShowRemainTs,
                m_Info.m_nOnLineIdx,m_Info.m_nOnLineItemType,m_Info.m_nOnLineRemainTs,m_Info.m_nIsGoldTransformItem,m_Info.m_nTail);

        bExist  = TRUE;
    }

    if(!bExist)
    {
        SetDefaultData();
        m_Info.m_nUid=nUid;       
        InsertRecord(nUid, m_Info.m_HeadPicurl, m_Info.m_NickName);        
    }
    mysql_free_result(result);
    result = NULL;
    LOGDEBUG("[CUserData::LoadInfo] Loading table g_bumper_info end");
    return TRUE;
    
}

BOOL CUserData::SaveInfo()
{
    if (!Database())
        return FALSE;

    char szSql[4096];
    memset(szSql, 0, 4096);
    sprintf(szSql, "UPDATE g_bumper_info SET head_picurl=\"%s\",nick_name=\"%s\",gold_num=%d,kill_num=%d,granding_id=%d,star_num=%d,use_car=%d,\
        unlock_cars=\"%s\",keep_star=%d,get_gift=%d,last_day=%s,king_num=%d,gift_day=\"%s\",gift_num=%d,star_day=\"%s\",star_sum=%d,add_score=%d,add_gold=%d,fight_team_result=%d,\
        fight_kill_sum=%d,fight_score=%d,fight_level=%d,add_score=%d,add_gold=%d,ratio=%d,car_level=%d,\`status\`=%d,buff=%d,king_rank=%d,\
        enemy_id=%d,dead_score=%d,task_mask=%d,day_task_mask=%d,week_task_mask=%d,day_task_mask1=%d,last_reset_task_time=%d,cur_week_num=%d,cur_tm_wday=%d,show_idx=%d,show_item_type=%d,show_remain_ts=%d,\
        online_idx=%d,online_item_type=%d,online_remain_ts=%d,is_gold_transform_item=%d,tail=%d WHERE user_id=%d", 
        m_Info.m_HeadPicurl.c_str(),m_Info.m_NickName.c_str(), m_Info.m_nGoldSum, m_Info.m_nKillSum, m_Info.m_nGrandingId, m_Info.m_nCurStarSum, m_Info.m_nCurUseCar, 
        m_Info.m_UnlockSum.c_str(), m_Info.m_nKeepStar, m_Info.m_nGetGift, m_Info.m_strLastDay.c_str(), m_Info.m_nKingSum,m_Info.m_GiftDay.c_str(),m_Info.m_nGiftSum,m_Info.m_StarDay.c_str(),m_Info.m_nStarSum, m_Info.m_nAddScore, m_Info.m_nAddGold, m_Info.m_nFightTeamResult, 
        m_Info.m_nFightKillSum,m_Info.m_nFightScore, m_Info.m_nFightLevel,m_Info.m_nAddScore,m_Info.m_nAddGold,m_Info.m_nRatio,m_Info.m_nCarLevel,0/*m_Info.m_nDeadStatus*/,m_Info.m_nBuff,m_Info.m_nKingRank, 
        m_Info.m_nEnemyId,0/*m_Info.m_nDeadScore*/,m_Info.m_i32TaskMask,m_Info.m_i32WeekTaskMask,m_Info.m_i32DayTaskMask,m_Info.m_i32DayTaskMask_1,m_Info.m_nLastResetTaskTime,m_Info.m_nCurWeekNum,m_Info.m_nCurTmWday,m_Info.m_nShowIdx,m_Info.m_nShowItemType,m_Info.m_nShowRemainTs,
        m_Info.m_nOnLineIdx,m_Info.m_nOnLineItemType,m_Info.m_nOnLineRemainTs,m_Info.m_nIsGoldTransformItem,m_Info.m_nTail,m_Info.m_nUid);

    LOGDEBUG("[CUserData::SaveInfo] szSql = [%s]", szSql);

    if(Database()->UpdateRecord(szSql))
    {
        LOGDEBUG("[CUserData::SaveInfo] Is Suc... sql=[%s]\n", szSql);
    }
    else
    {
        LOGERROR("[CUserData::SaveInfo] Is Error... sql=[%s]\n", szSql);
        return FALSE;
    }

    return TRUE;
}

//设置默认属性
VOID CUserData::SetDefaultData()
{
    m_Info.m_nUid = 0;
    m_Info.m_HeadPicurl = "default";           //string 头像
    m_Info.m_NickName = "default";             //string 昵称
    m_Info.m_nGoldSum = 100;                   //int 金币数量  
    m_Info.m_nKillSum = 0;                     //int 击杀数量
    m_Info.m_nGrandingId = 1;                  //int 段位id
    m_Info.m_nCurStarSum = 0;                  //int 当前星星数量
    m_Info.m_UnlockSum = "1";                    //string 已解锁车辆
    m_Info.m_nCurUseCar = 1;                 //int 当前使用中的车辆

    m_Info.m_nGetGift = 0;
    m_Info.m_nKeepStar = 0;
    m_Info.m_strLastDay = "1970-01-01";
    
    m_Info.m_nKingSum = 0;                     //int 霸主令牌数量
    m_Info.m_GiftDay = "2018-11-02";           //string 当天获取礼包的次数
    m_Info.m_nGiftSum = 0;                     //int 当天获取礼包的次数
    m_Info.m_StarDay = "2018-11-02";           //string 当天保留星星的次数
    m_Info.m_nStarSum = 0;                     //int 当天保留星星的次数
    m_Info.m_nBuff = 0;

    m_Info.m_nAddScore = 0;                    //int  初始积分加成 3000
    m_Info.m_nAddGold = 0;                     //int  战斗结束后金币加成百分比 100%
    m_Info.m_nRatio = 0;                       //int  大乱斗结束时候奖励的倍率，默认1倍，看视频后3倍率  
    m_Info.m_nCarLevel = 1;                    //int  大乱斗车辆当前等级
    m_Info.m_nKingRank = 0;                    //int  霸主标识：100：霸主
    m_Info.m_nEnemyId = 0;                     //int  仇人id  不用持久化

    //
    m_Info.m_nFightKillSum = 0;                //int  本局击杀数 初始0
    m_Info.m_nFightScore = 1000;               //int  本局积分 初始1000
    m_Info.m_nFightLevel = 1;                  //int  本局等级 初始1
    m_Info.m_nDeadStatus = 0;                      //int  死亡状体

    m_Info.m_nDeadScore = 0;                   //int  死亡时积分
    m_Info.m_nFightTeamResult = 0;             //int  红蓝对战结果

    m_Info.m_nDeadStatus = _STATUS_ALIVE;
    m_Info.m_nGameStep   = _GAME_STEP_NONE;

    m_Info.m_i32TaskMask = 0;                   //永久任务掩码位32
    m_Info.m_i32WeekTaskMask = 0;               //天任务掩码32
    m_Info.m_i32DayTaskMask = 0;                //天任务掩码32
    m_Info.m_i32DayTaskMask_1 = 0;              //天任务掩码64
    m_Info.m_nLastResetTaskTime = 0;            //日常任务reset时间
    m_Info.m_nCurTmWday = 0;                    //当前星期[0-6]:星期天-星期六
    m_Info.m_nShowIdx = 0;                      //展示索引
    m_Info.m_nShowItemType = 0;                 //展示的道具itemtype
    m_Info.m_nShowRemainTs = 0;                 //展示的道具的剩余时间戳(秒)
    m_Info.m_nOnLineIdx = 0;                    //uint 在线领奖索引(秒)
    m_Info.m_nOnLineItemType = 0;               //uint 在线领奖索引展示的道具itemtype(秒)
    m_Info.m_nOnLineRemainTs = 0;               //uint 在线领奖索引展示的道具的剩余时间戳(秒)
    m_Info.m_nIsGoldTransformItem = 0;          //uint 金币转换道具 0 : 未转换 1 : 已经转换过
    m_Info.m_nTail = 0;                         //uint 当前使用的拖尾道具唯一id

    return;
}
//int  用户id
VOID CUserData::SetUid(int nUid, BOOL bUpdate/* = TRUE*/)
{
    m_Info.m_nUid = nUid;
    return;
}
//string 头像
VOID CUserData::SetHeadPicurl(std::string const& strHeadPicurl, BOOL bUpdate/* = TRUE*/)
{
    m_Info.m_HeadPicurl   = strHeadPicurl;
    if (!Database())
        return;

    if (bUpdate)
    {    
        std::string sql=string("UPDATE g_bumper_info SET head_picurl=\"")+strHeadPicurl+"\" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    } 
    return;
}

//string 昵称
VOID CUserData::SetNickName(std::string const& strNickName, BOOL bUpdate/* = TRUE*/)
{  
    m_Info.m_NickName   = strNickName;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET nick_name=\"")+strNickName+"\" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//int 金币数量
VOID CUserData::SetGoldSum(UInt32 nGoldSum, BOOL bUpdate/* = TRUE*/)
{
    m_Info.m_nGoldSum   = nGoldSum;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET gold_num=")+std::to_string(nGoldSum)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    } 
    return;   
}

//int 击杀数量
VOID CUserData::SetKillSum(UInt32 nKillSum, BOOL bUpdate/* = TRUE*/)
{
    m_Info.m_nKillSum   = nKillSum;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET kill_num=")+std::to_string(nKillSum)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}
    
//int 段位id
VOID CUserData::SetGrandingId(UInt32 nGrandingId, BOOL bUpdate/* = TRUE*/)
{
    m_Info.m_nGrandingId   = nGrandingId;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET granding_id=")+std::to_string(nGrandingId)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}


//int 当前星星数量
VOID CUserData::SetCurStarSum(UInt32 nCurStarSum, BOOL bUpdate/* = TRUE*/)
{
    m_Info.m_nCurStarSum   = nCurStarSum;
    if (!Database())
      return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET star_num=")+std::to_string(nCurStarSum)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//string 已解锁车辆
VOID CUserData::SetUnlockSum(std::string const& strUnlockSum, BOOL bUpdate/* = TRUE*/)
{
    m_Info.m_UnlockSum   = strUnlockSum;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET unlock_cars=\"")+strUnlockSum+"\" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//int 当前使用中的车辆
VOID CUserData::SetUseCar(UInt32 nCurUseCar, BOOL bUpdate/* = TRUE*/)
{
    m_Info.m_nCurUseCar   = nCurUseCar;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET use_car=")+std::to_string(nCurUseCar)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//int 霸主令牌数量
VOID CUserData::SetKingSum(UInt32 nKingSum, BOOL bUpdate/* = TRUE*/)
{
    m_Info.m_nKingSum   = nKingSum;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET king_num=")+std::to_string(nKingSum)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//string 当天获取礼包的次数
VOID CUserData::SetGiftDay(std::string const& strGiftDay, BOOL bUpdate/* = TRUE*/)
{
    m_Info.m_GiftDay   = strGiftDay;
    if (!Database())
      return;

    if (bUpdate)
    {
      std::string sql=string("UPDATE g_bumper_info SET gift_day=\"")+strGiftDay+"\" WHERE user_id="+std::to_string(m_Info.m_nUid);
      Database()->UpdateRecord(sql);
    }
    return;
}

//int 当天获取礼包的次数
VOID CUserData::SetGiftSum(UInt32 nGiftSum, BOOL bUpdate/* = TRUE*/)
{
    m_Info.m_nGiftSum   = nGiftSum;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET gift_num=")+std::to_string(nGiftSum)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}             

//string 当天保留星星的次数
VOID CUserData::SetStarDay(std::string const& strStarDay, BOOL bUpdate/* = TRUE*/)
{
    m_Info.m_StarDay   = strStarDay;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET star_day=\"")+strStarDay+"\" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//int 当天保留星星的次数
VOID CUserData::SetStarSum(UInt32 nStarSum, BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_nStarSum   = nStarSum;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET star_num=")+std::to_string(nStarSum)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//int  本局击杀数 初始0
VOID CUserData::SetFightKillSum(UInt32 nFightKillSum, BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_nFightKillSum   = nFightKillSum; 
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET fight_kill_sum=")+std::to_string(nFightKillSum)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//int  本局积分 初始1000
VOID CUserData::SetFightScore(UInt32 nFightScore, BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_nFightScore   = nFightScore;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET fight_score=")+std::to_string(nFightScore)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//int  本局等级 初始1
VOID CUserData::SetFightLevel(UInt32 nFightLevel, BOOL bUpdate /*= TRUE*/) 
{
    m_Info.m_nFightLevel   = nFightLevel;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET fight_level=")+std::to_string(nFightLevel)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

  //int  初始积分加成 3000
VOID CUserData::SetAddScore(UInt32 nAddScore, BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_nAddScore   = nAddScore;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET add_score=")+std::to_string(nAddScore)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}


//int  战斗结束后金币加成百分比 100%
VOID  CUserData::SetAddGold(UInt32 nAddGold, BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_nAddGold   = nAddGold;
    if (!Database())
      return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET add_gold=")+std::to_string(nAddGold)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}
      
//int  大乱斗结束时候奖励的倍率，默认1倍，看视频后3倍率   
VOID CUserData::SetRatio(UInt32 nRatio, BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_nRatio   = nRatio;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET ratio=")+std::to_string(nRatio)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}  

//int  大乱斗chelaingdangqiandengji  
VOID CUserData::SetCarLevel(UInt32 nCarLevel, BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_nCarLevel   = nCarLevel;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET car_level=")+std::to_string(nCarLevel)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//int  死亡状体 
VOID CUserData::SetDeadStatus(UInt32 nDeadStatus, BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_nDeadStatus   = nDeadStatus;
    if (!Database())
        return;

    // if (bUpdate)
    // {
    //     std::string sql=string("UPDATE g_bumper_info SET game_step=")+std::to_string(nDeadStatus)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
    //     Database()->UpdateRecord(sql);
    // }
    return;
}


//int  死亡状体 
VOID CUserData::SetGameStep(UInt32 nGameStep, BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_nGameStep   = nGameStep;
    if (!Database())
        return;

    //保持在内存不写数据库
    // if (bUpdate)
    // {
    //     std::string sql=string("UPDATE g_bumper_info SET status=")+std::to_string(nDeadStatus)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
    //     Database()->UpdateRecord(sql);
    // }
    return;
}



//int  BUFF 
VOID CUserData::SetBuff(UInt32 nBuff, BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_nBuff  = nBuff;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET buff=")+std::to_string(nBuff)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//int  霸主标识 
VOID CUserData::SetKingRank(UInt32 nKingRank, BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_nKingRank  = nKingRank;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET king_rank=")+std::to_string(nKingRank)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
}

//int  仇人id  不用持久化  
VOID CUserData::SetEnemyId(UInt32 nEnemyId, BOOL bUpdate/* = TRUE*/)
{
    m_Info.m_nEnemyId  = nEnemyId;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET enemy_id=")+std::to_string(nEnemyId)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//int  死亡时积分
VOID CUserData::SetDeadScore(UInt32 nDeadScore, BOOL bUpdate/* = TRUE*/)
{
    m_Info.m_nDeadScore  = nDeadScore;
    if (!Database())
        return;

    if (bUpdate)
    {
        //!alter by huyf 2018.12.25:每局保留，不需要保存到数据库
        //std::string sql=string("UPDATE g_bumper_info SET dead_score=")+std::to_string(nDeadScore)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        //Database()->UpdateRecord(sql);
        //!alter end:每局保留，不需要保存到数据库
    }
    return;
}

//int 红蓝组对时战斗结果
VOID  CUserData::SetFightTeamResult(UInt32 nFightTeamResult, BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_nFightTeamResult  = nFightTeamResult;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET fight_team_result=")+std::to_string(nFightTeamResult)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;

}

VOID CUserData::SetKeepStar(UInt32 nKeepStar, BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_nKeepStar = nKeepStar;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET keep_star=")+std::to_string(nKeepStar)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

VOID CUserData::SetGetGift(UInt32 nGetGift, BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_nGetGift = nGetGift;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET get_gift=")+std::to_string(nGetGift)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

VOID CUserData::SetLastDay(std::string& strLastDay, BOOL bUpdate/* = TRUE*/)
{
    m_Info.m_strLastDay = strLastDay;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET last_day=\"")+strLastDay+"\" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//永久任务掩码位
VOID CUserData::SetTaskMask(UInt32 i32Task, BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_i32TaskMask  = i32Task;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET task_mask=")+std::to_string(i32Task)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//每周任务任务掩码位64
VOID CUserData::SetWeekTaskMask(UInt32 i32WeekMask, BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_i32WeekTaskMask  = i32WeekMask;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET week_task_mask=")+std::to_string(i32WeekMask)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//每天任务任务掩码位64
VOID CUserData::SetDayTaskMask(UInt32 i32DayMask, BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_i32DayTaskMask  = i32DayMask;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET day_task_mask=")+std::to_string(i32DayMask)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//天任务任务掩码位128
VOID CUserData::SetDayTaskMask1(UInt32 i32DayMask, BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_i32DayTaskMask_1  = i32DayMask;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET day_task_mask1=")+std::to_string(i32DayMask)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}
//日常任务reset时间
VOID CUserData::SetLastResetTaskTime(BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_nLastResetTaskTime  = ::DateStamp();
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET last_reset_task_time=")+std::to_string(m_Info.m_nLastResetTaskTime)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//当前是一年中的第几周
VOID CUserData::SetCurWeekNum(BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_nCurWeekNum  = ::WeekNum();
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET cur_week_num=")+std::to_string(m_Info.m_nCurWeekNum)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//当前星期[0-6]:星期天-星期六
VOID CUserData::SetCurTmWday(UInt32 nCurTmWday, BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_nCurTmWday  = nCurTmWday;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET cur_tm_wday=")+std::to_string(m_Info.m_nCurTmWday)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//展示索引
VOID CUserData::SetShowIdx(UInt32 nShowIdx, BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_nShowIdx  = nShowIdx;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET show_idx=")+std::to_string(m_Info.m_nShowIdx)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
} 

//展示的道具
VOID CUserData::SetShowItemType(UInt32 nShowItemType, BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_nShowItemType  = nShowItemType;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET show_item_type=")+std::to_string(m_Info.m_nShowItemType)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//展示的道具的剩余时间戳(秒)
VOID CUserData::SetShowRemainTs(UInt32 nShowRemainTs, BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_nShowRemainTs  = nShowRemainTs;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET show_remain_ts=")+std::to_string(m_Info.m_nShowRemainTs)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//在线领奖索引
VOID CUserData::SetOnLineIdx(UInt32 nOnLineIdx, BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_nOnLineIdx  = nOnLineIdx;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET online_idx=")+std::to_string(m_Info.m_nOnLineIdx)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
} 

//在线领奖索引展示的道具itemtype
VOID CUserData::SetOnLineItemType(UInt32 nOnLineItemType, BOOL bUpdate/*= TRUE*/)
{
    m_Info.m_nOnLineItemType  = nOnLineItemType;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET online_item_type=")+std::to_string(m_Info.m_nOnLineItemType)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//在线领奖索引展示的道具的剩余时间戳(秒)
VOID CUserData::SetOnLineRemainTs(UInt32 nOnLineRemainTs, BOOL bUpdate/*= TRUE*/)
{
    m_Info.m_nOnLineRemainTs  = nOnLineRemainTs;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET online_remain_ts=")+std::to_string(m_Info.m_nOnLineRemainTs)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//uint 金币转换道具 0 : 未转换 1 : 已经转换过
VOID CUserData::SetIsGoldTransformItem(UInt32 nIsGoldTransformItem, BOOL bUpdate/*= TRUE*/)
{
    m_Info.m_nIsGoldTransformItem  = nIsGoldTransformItem;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET is_gold_transform_item=")+std::to_string(m_Info.m_nIsGoldTransformItem)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//uint 当前使用的拖尾道具唯一id
VOID CUserData::SetTail(UInt32 nTail, BOOL bUpdate/*= TRUE*/)
{
    m_Info.m_nTail  = nTail;
    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("UPDATE g_bumper_info SET tail=")+std::to_string(m_Info.m_nTail)+" WHERE user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

// add by gxx 2018.11.13
//int 签到日期
VOID CUserData::SetSignDays(UInt32 nSignDays, BOOL bUpdate /*= TRUE*/)
{

    m_Info.m_nSignDays  = nSignDays;

    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("update g_bumper_info set sign_days=")+std::to_string(nSignDays)+" where user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

/*
//string 签到最后日期
VOID CUserData::SetLastSignDay(std::string const& strLastSignDay, BOOL bUpdate)
{

    m_Info.m_LastSignDay   = strLastSignDay;

    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("update g_bumper_info set last_sign_day=\"")+strStarDay+"\" where user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
}
*/

//int 第1天
VOID CUserData::SetDay1(UInt32 nDay1, BOOL bUpdate /*= TRUE*/)
{

    m_Info.m_nDay1  = nDay1;

    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("update g_bumper_info set day1=")+std::to_string(nDay1)+" where user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//int 第2天
VOID CUserData::SetDay2(UInt32 nDay2, BOOL bUpdate /*= TRUE*/)
{

    m_Info.m_nDay2  = nDay2;

    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("update g_bumper_info set day2=")+std::to_string(nDay2)+" where user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}
//int 第3天
VOID CUserData::SetDay3(UInt32 nDay3, BOOL bUpdate /*= TRUE*/)
{

    m_Info.m_nDay3  = nDay3;

    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("update g_bumper_info set day3=")+std::to_string(nDay3)+" where user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//int 第4天
VOID CUserData::SetDay4(UInt32 nDay4, BOOL bUpdate /*= TRUE*/)
{

    m_Info.m_nDay4  = nDay4;

    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("update g_bumper_info set day4=")+std::to_string(nDay4)+" where user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//int 第5天
VOID CUserData::SetDay5(UInt32 nDay5, BOOL bUpdate /*= TRUE*/)
{

    m_Info.m_nDay5  = nDay5;

    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("update g_bumper_info set day5=")+std::to_string(nDay5)+" where user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//int 第6天
VOID CUserData::SetDay6(UInt32 nDay6, BOOL bUpdate /*= TRUE*/)
{

    m_Info.m_nDay6  = nDay6;

    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("update g_bumper_info set day6=")+std::to_string(nDay6)+" where user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}
//int 第7天
VOID CUserData::SetDay7(UInt32 nDay7, BOOL bUpdate /*= TRUE*/)
{

    m_Info.m_nDay7  = nDay7;

    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("update g_bumper_info set day7=")+std::to_string(nDay7)+" where user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//UINT32 7天签到标志
VOID CUserData::SetWeek(UInt32 nWeek, BOOL bUpdate)
{
    m_Info.m_nWeek = nWeek;

    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("update g_bumper_info set week=")+std::to_string(nWeek)+" where user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//皮肤碎片
VOID CUserData::SetDebris(UInt32 nDebrisSum, BOOL bUpdate)
{
     m_Info.m_nDebrisSum = nDebrisSum;

    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("update g_bumper_info set debirs=")+std::to_string(nDebrisSum)+" where user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//皮肤体验天数
VOID CUserData::SetExpSkin(UInt32 nExpSkin, BOOL bUpdate)
{
     m_Info.m_nExpSkin = nExpSkin;

    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("update g_bumper_info set exp_skin=")+std::to_string(nExpSkin)+" where user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//int 第1宝箱
VOID CUserData::SetBox1(UInt32 nBox1, BOOL bUpdate /*= TRUE*/)
{

    m_Info.m_nBox1  = nBox1;

    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("update g_bumper_info set box1=")+std::to_string(nBox1)+" where user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//int 第2宝箱
VOID CUserData::SetBox2(UInt32 nBox2, BOOL bUpdate /*= TRUE*/)
{

    m_Info.m_nBox2  = nBox2;

    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("update g_bumper_info set box2=")+std::to_string(nBox2)+" where user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//int 第3宝箱
VOID CUserData::SetBox3(UInt32 nBox3, BOOL bUpdate /*= TRUE*/)
{

    m_Info.m_nBox3  = nBox3;

    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("update g_bumper_info set box3=")+std::to_string(nBox3)+" where user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

//int 第4宝箱
VOID CUserData::SetBox4(UInt32 nBox4, BOOL bUpdate /*= TRUE*/)
{

    m_Info.m_nBox4  = nBox4;

    if (!Database())
        return;

    if (bUpdate)
    {
        std::string sql=string("update g_bumper_info set box4=")+std::to_string(nBox4)+" where user_id="+std::to_string(m_Info.m_nUid);
        Database()->UpdateRecord(sql);
    }
    return;
}

// end by gxx

  //---------------不需要要存储数据库的数据------------------------------//
    // 玩家状态 0：下线 1:在线 2：游戏中
VOID CUserData::SetGameStatus(UInt32 GameStatus, BOOL bUpdate /*= TRUE*/)
{
    m_Info.m_nGameStatus  = GameStatus;

    // if (!Database())
    //     return;

    // if (bUpdate)
    // {
    //     std::string sql=string("update g_bumper_info set box4=")+std::to_string(nBox4)+" where user_id="+std::to_string(m_Info.m_nUid);
    //     Database()->UpdateRecord(sql);
    // }
    return;
}