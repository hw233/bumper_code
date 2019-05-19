#include "MsgBumperTransfer.h"
#include "../Base/logger.h"
#include "../Main/ServerManager.h"
#include "../DataBase/DBMySql.h"
#include "../Common/Common.h"

#include <string.h>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <map>

int atoi_safe(const char* str);

void splitString(string str, const char* sep, vector<int>& vec);

char* getCurrentYearMonDay()
{

    time_t timer;
    time(&timer);
    
    tm* t_tm = localtime(&timer);
    
    static char strYearMonDay[32];
    memset(strYearMonDay, 0, sizeof(strYearMonDay));
    strftime(strYearMonDay, sizeof(strYearMonDay), "%Y-%m-%d", t_tm);
    
    return strYearMonDay;
}



void insStar(map<string, int>& data, int paramStarNum)
{

    int starNum = data["star_num"] + paramStarNum;
    if(starNum >= 5)
    {
        if(data["granding_id"] < 31)
        {
            data["star_num"] = starNum - 5;
            data["granding_id"]++;
            (paramStarNum == 1)? data["status"]= 1: data["status"] = 2;
        }
        else
        {
            data["star_num"] = starNum;
            data["granding_id"] = 31;
            (paramStarNum > 0)? data["status"] = 1: data["status"] = -1; 
        }

    }
    else if(starNum <= 0)
    {
        if(data["granding_id"] > 1)
        {
            if(starNum == 0)
            {
                data["star_num"] = 0;
            }
            else
            {
                data["star_num"] = 4;
                data["granding_id"]--;
            }
            data["status"] = -1;
        }
        else
        {
            data["star_num"] = 0;
            data["granding_id"] = 1;
            data["status"] = 0;
        }

   }
   else 
   {
        data["star_num"] = starNum;
        if(paramStarNum < 0)
        {
            data["status"] = -1;
        }
        else if(paramStarNum == 1)
        {
            data["status"] = 1;
        }
        else
        {
            data["status"] = 2;
        }
   }

}

CMsgBumperTransfer::CMsgBumperTransfer() 
{

}

CMsgBumperTransfer::~CMsgBumperTransfer() 
{

}

BOOL CMsgBumperTransfer::Read(SocketInputStream& iStream, UINT nLen) 
{
    CNetMsg::Read(iStream, nLen);
    return TRUE;
}

BOOL CMsgBumperTransfer::Write(SocketOutputStream& oStream) const 
{
    return TRUE;
}

BOOL CMsgBumperTransfer::CreateMsg(UINT nAction, VT_RANK_TOP_LIST & vt_tmp, RankTopListStruct* pUserInfo)
{

       if(m_pMsg) 
       {
            cJSON_AddNumberToObject(m_pMsg, "action", nAction);
    
            if(nAction == _BUMPER_TRANSFER_RANKING_LIST_BY_RANK_SUC)
            {
                cJSON_AddNumberToObject(m_pMsg, "action", nAction);
            
                cJSON_AddNumberToObject(m_pMsg, "errno", 0);
                cJSON_AddStringToObject(m_pMsg, "errmsg", "");
                cJSON* jData;
                cJSON_AddItemToObject(m_pMsg, "data", jData = cJSON_CreateObject());
                if(!vt_tmp.empty())
                {
                    cJSON* elemArray = NULL;
                    cJSON* elem = NULL;
                    cJSON_AddItemToObject(jData, "data", elemArray = cJSON_CreateArray());

                    for(auto iter = vt_tmp.begin(); iter != vt_tmp.end(); ++iter)
                    {
                       cJSON_AddItemToArray(elemArray, elem = cJSON_CreateObject());
            		       cJSON_AddNumberToObject(elem, "granding_id", (*iter)->m_nGrandingId);
            		       cJSON_AddNumberToObject(elem, "star_num", (*iter)->m_nStarNum);
            		       cJSON_AddStringToObject(elem, "nickname", ((*iter)->m_NickName).c_str());               
            		       cJSON_AddStringToObject(elem, "avatar_url", ((*iter)->m_HeadPicurl).c_str());   
                       cJSON_AddNumberToObject(elem, "rank", (*iter)->m_nRank);
                    }
                }

               if(pUserInfo)
               {
                    cJSON* jUserInfo;
                    cJSON_AddItemToObject(jData, "user_info", jUserInfo = cJSON_CreateObject());
                    cJSON_AddStringToObject(jUserInfo, "nickname", (pUserInfo->m_NickName).c_str());
                    cJSON_AddStringToObject(jUserInfo, "avatar_url", (pUserInfo->m_HeadPicurl).c_str());
                    cJSON_AddNumberToObject(jUserInfo, "granding_id", pUserInfo->m_nGrandingId);
                    cJSON_AddNumberToObject(jUserInfo, "star_num", pUserInfo->m_nStarNum);
                    cJSON_AddNumberToObject(jUserInfo, "rank", pUserInfo->m_nRank);
               }
            }

            m_pData = cJSON_Print(m_pMsg);
            string strMsg(m_pData);
            INT nLen = strMsg.size();
            LOGDEBUG("[CMsgBumperTransfer::CreateMsg]:\n m_pData=[%s] nLen=[%d]\n", m_pData, nLen);
      }

       return true;
}



BOOL CMsgBumperTransfer::CreateMsg(UINT nAction, VT_KILLNUM_TOP_LIST & vt_tmp, KillNumTopListStruct* pUserInfo)
{

       if(m_pMsg) 
       {
            cJSON_AddNumberToObject(m_pMsg, "action", nAction);
    
                if(nAction == _BUMPER_TRANSFER_RANKING_LIST_BY_KILLNUM_SUC)
                {
                    
                cJSON_AddNumberToObject(m_pMsg, "action", nAction);
            
                cJSON_AddNumberToObject(m_pMsg, "errno", 0);
                cJSON_AddStringToObject(m_pMsg, "errmsg", "");
                cJSON* jData;
                cJSON_AddItemToObject(m_pMsg, "data", jData = cJSON_CreateObject());
                if(!vt_tmp.empty())
                {

                    cJSON* elemArray = NULL;
                    cJSON* elem = NULL;
                    cJSON_AddItemToObject(jData, "data", elemArray = cJSON_CreateArray());

                    for(auto iter = vt_tmp.begin(); iter != vt_tmp.end(); ++iter)
                    {
                       cJSON_AddItemToArray(elemArray, elem = cJSON_CreateObject());
                          
        		       cJSON_AddNumberToObject(elem, "kill_num", (*iter)->m_nKillNum);
        		       cJSON_AddNumberToObject(elem, "star_num", (*iter)->m_nStarNum);
        		       cJSON_AddStringToObject(elem, "nickname", ((*iter)->m_NickName).c_str());               
        		       cJSON_AddStringToObject(elem, "avatar_url", ((*iter)->m_HeadPicurl).c_str());   
                       cJSON_AddNumberToObject(elem, "rank", (*iter)->m_nRank);
                       
                    }
                }

               if(pUserInfo)
               {

                       
                      cJSON* jUserInfo;
                      cJSON_AddItemToObject(jData, "user_info", jUserInfo = cJSON_CreateObject());
                      cJSON_AddStringToObject(jUserInfo, "nickname", (pUserInfo->m_NickName).c_str());
                      cJSON_AddStringToObject(jUserInfo, "avatar_url", (pUserInfo->m_HeadPicurl).c_str());
                      cJSON_AddNumberToObject(jUserInfo, "kill_num", pUserInfo->m_nKillNum);
                      cJSON_AddNumberToObject(jUserInfo, "star_num", pUserInfo->m_nStarNum);
                      cJSON_AddNumberToObject(jUserInfo, "rank", pUserInfo->m_nRank);

               }
          }



                  m_pData = cJSON_Print(m_pMsg);
                  string strMsg(m_pData);
                  INT nLen = strMsg.size();
                  LOGDEBUG("[CMsgBumperTransfer::CreateMsg]:\n m_pData=[%s] nLen=[%d]\n", m_pData, nLen);


      }

       return true;
}






BOOL CMsgBumperTransfer::CreateMsg(UINT nAction, vector<map<string, string>*>& data, map<string, string> & user_info)
{
    if(m_pMsg) 
    {
         cJSON_AddNumberToObject(m_pMsg, "action", nAction);

        if(nAction == _BUMPER_TRANSFER_RANKING_LIST_BY_RANK_SUC)
        {
            cJSON_AddNumberToObject(m_pMsg, "action", nAction);
        
            cJSON_AddNumberToObject(m_pMsg, "errno", 0);
            cJSON_AddStringToObject(m_pMsg, "errmsg", "");
            cJSON* jData;
            cJSON_AddItemToObject(m_pMsg, "data", jData = cJSON_CreateObject());
             
            if(!data.empty())
            {
                cJSON* elemArray = NULL;
                cJSON* elem = NULL;
                cJSON_AddItemToObject(jData, "data", elemArray = cJSON_CreateArray());
                for(auto iter = data.begin(); iter != data.end(); ++iter)
                {
                   cJSON_AddItemToArray(elemArray, elem = cJSON_CreateObject());
                      
    		       cJSON_AddNumberToObject(elem, "granding_id", SafeAtoi((**iter)["granding_id"].c_str()));
    		       cJSON_AddNumberToObject(elem, "star_num", SafeAtoi((**iter)["star_num"].c_str()));
    		       cJSON_AddStringToObject(elem, "nickname", (**iter)["nickname"].c_str());               
    		       cJSON_AddStringToObject(elem, "avatar_url", (**iter)["avatar_url"].c_str());   
                   cJSON_AddNumberToObject(elem, "rank", SafeAtoi((**iter)["rank"].c_str()));
                }
             }
            if(!user_info.empty())
            {
               cJSON* jUserInfo;
               cJSON_AddItemToObject(jData, "user_info", jUserInfo = cJSON_CreateObject());
               cJSON_AddStringToObject(jUserInfo, "nickname", user_info["nickname"].c_str());
               cJSON_AddStringToObject(jUserInfo, "avatar_url", user_info["avatar_url"].c_str());
               cJSON_AddNumberToObject(jUserInfo, "granding_id", SafeAtoi(user_info["granding_id"].c_str()));
               cJSON_AddNumberToObject(jUserInfo, "star_num", SafeAtoi(user_info["star_num"].c_str()));
               cJSON_AddNumberToObject(jUserInfo, "rank", SafeAtoi(user_info["rank"].c_str()));
               cJSON_AddNumberToObject(jUserInfo, "is_rank", SafeAtoi(user_info["is_rank"].c_str()));
            }
        }
         else
         if(nAction == _BUMPER_TRANSFER_RANKING_LIST_BY_KILLNUM_SUC)
         {
            cJSON_AddNumberToObject(m_pMsg, "action", nAction);
        
            cJSON_AddNumberToObject(m_pMsg, "errno", 0);
            cJSON_AddStringToObject(m_pMsg, "errmsg", "");
            cJSON* jData;
            cJSON_AddItemToObject(m_pMsg, "data", jData = cJSON_CreateObject());
             
            if(!data.empty())
            {
                cJSON* elemArray = NULL;
                cJSON* elem = NULL;
                cJSON_AddItemToObject(jData, "data", elemArray = cJSON_CreateArray());
                for(auto iter = data.begin(); iter != data.end(); ++iter)
                {
                   cJSON_AddItemToArray(elemArray, elem = cJSON_CreateObject());
                      
    		       cJSON_AddNumberToObject(elem, "kill_num", SafeAtoi((**iter)["kill_num"].c_str()));
    		       cJSON_AddNumberToObject(elem, "star_num", SafeAtoi((**iter)["star_num"].c_str()));
    		       cJSON_AddStringToObject(elem, "nickname", (**iter)["nickname"].c_str());               
    		       cJSON_AddStringToObject(elem, "avatar_url", (**iter)["avatar_url"].c_str()); 
                    cJSON_AddNumberToObject(elem, "rank", SafeAtoi((**iter)["rank"].c_str()));
                }
             }

            if(!user_info.empty())
            {

                cJSON* jUserInfo;
                cJSON_AddItemToObject(jData, "user_info", jUserInfo = cJSON_CreateObject());
                cJSON_AddStringToObject(jUserInfo, "nickname", user_info["nickname"].c_str());
                cJSON_AddStringToObject(jUserInfo, "avatar_url", user_info["avatar_url"].c_str());
                cJSON_AddNumberToObject(jUserInfo, "kill_num", SafeAtoi(user_info["kill_num"].c_str()));
                cJSON_AddNumberToObject(jUserInfo, "star_num", SafeAtoi(user_info["star_num"].c_str()));
                cJSON_AddNumberToObject(jUserInfo, "rank", SafeAtoi(user_info["rank"].c_str()));
                cJSON_AddNumberToObject(jUserInfo, "is_rank", SafeAtoi(user_info["is_rank"].c_str()));
            }
        }

        m_pData = cJSON_Print(m_pMsg);
        string strMsg(m_pData);
        INT nLen = strMsg.size();
        LOGDEBUG("[CMsgBumperTransfer::CreateMsg]:\n m_pData=[%s] nLen=[%d]\n", m_pData, nLen);

    }

    return TRUE;
}



BOOL CMsgBumperTransfer::CreateMsg(UINT nAction, map<string, int> & data)
{
    if(m_pMsg) 
    {
        cJSON_AddNumberToObject(m_pMsg, "action", nAction);

        if(nAction == _BUMPER_TRANSFER_GET_BIG_GIFT_SUC)
        {
            cJSON_AddNumberToObject(m_pMsg, "errno", 0);
            cJSON_AddStringToObject(m_pMsg, "errmsg", "");
            cJSON* jdata;
            cJSON_AddItemToObject(m_pMsg, "data", jdata=cJSON_CreateObject());
            //!alter by huyf 2019.01.12:前端说把金币去掉其他都要
            //cJSON_AddNumberToObject(jdata, "gold_num", data["gold_num"]);
            //!alter end:前端说把金币去掉其他都要
            cJSON_AddNumberToObject(jdata, "star_num", data["star_num"]);
            cJSON_AddNumberToObject(jdata, "granding_id", data["granding_id"]);
            cJSON_AddNumberToObject(jdata, "granding_num", data["granding_num"]);
            cJSON_AddNumberToObject(jdata, "get_gift", data["get_gift"]);
        }
        else 
        if(nAction == _BUMPER_TRANSFER_GET_NOT_DECREASE_STAR_GIFT_SUC)
        {
            cJSON_AddNumberToObject(m_pMsg, "errno", 0);
            cJSON_AddStringToObject(m_pMsg, "errmsg", "");
            cJSON* jdata;
            cJSON_AddItemToObject(m_pMsg, "data", jdata=cJSON_CreateObject());
            cJSON_AddNumberToObject(jdata, "star_num", data["star_num"]);
            cJSON_AddNumberToObject(jdata, "granding_id", data["granding_id"]);
            cJSON_AddNumberToObject(jdata, "keep_star", data["keep_star"]);
        }
        else
        if(nAction == _BUMPER_TRANSFER_RANK_GET_GRANDING_STARNUM_SUC)
        {
            cJSON_AddNumberToObject(m_pMsg, "errno", 0);
            cJSON_AddStringToObject(m_pMsg, "errmsg", "");
            cJSON* jdata;
            cJSON_AddItemToObject(m_pMsg, "data", jdata=cJSON_CreateObject());
            cJSON_AddNumberToObject(jdata, "star_num", data["star_num"]);
            cJSON_AddNumberToObject(jdata, "granding_id", data["granding_id"]);
            cJSON_AddNumberToObject(jdata, "status", data["status"]);
        }

        m_pData = cJSON_Print(m_pMsg);
        string strMsg(m_pData);
        INT nLen = strMsg.size();
        LOGDEBUG("[CMsgBumperTransfer::CreateMsg]:\n m_pData=[%s] nLen=[%d]\n", m_pData, nLen);
       
    }
     return TRUE;
}
BOOL CMsgBumperTransfer::CreateMsg(UINT nAction, map<string, string> & data, UINT32 nData/*=0*/)
{
    if(m_pMsg) 
    {
        cJSON_AddNumberToObject(m_pMsg, "action", nAction);
        
        //!alter by huyf 2019.01.12:此action 已经不再使用
        // if(nAction == _BUMPER_TRANSFER_GET_USER_INFO_SUC) 
        // {
        //     cJSON_AddNumberToObject(m_pMsg, "errno", 0);
        //     cJSON_AddStringToObject(m_pMsg, "errmsg", "");
        
        //     cJSON* jData = NULL;
        //     cJSON_AddItemToObject(m_pMsg, "data", jData=cJSON_CreateObject());

        //     cJSON_AddStringToObject(jData, "nickname", data["nickname"].c_str());
        //     cJSON_AddStringToObject(jData, "avatar_url", data["avatar_url"].c_str());
        //     cJSON_AddNumberToObject(jData, "gold_num", SafeAtoi(data["gold_num"].c_str()));
        //     cJSON_AddNumberToObject(jData, "kill_num", SafeAtoi(data["kill_num"].c_str()));

        //     cJSON_AddNumberToObject(jData, "granding_id", SafeAtoi(data["granding_id"].c_str()));
        //     cJSON_AddNumberToObject(jData, "star_num", SafeAtoi(data["star_num"].c_str()));

        //     cJSON_AddNumberToObject(jData, "granding_num", SafeAtoi(data["nickname"].c_str()));
            
        //     cJSON_AddNumberToObject(jData, "keep_star", SafeAtoi(data["keep_star"].c_str()));
        //     cJSON_AddNumberToObject(jData, "get_gift", SafeAtoi(data["get_gift"].c_str()));
        //     cJSON_AddStringToObject(jData, "use_car", data["use_car"].c_str());


        //     vector<int> vec;
        //     splitString(data["unlock_cars"], ",", vec);
        //     cJSON* arrayJson = cJSON_CreateArray();
        //     for(vector<int>::iterator iter = vec.begin(); iter != vec.end(); ++iter)
        //     {
        //         LOGDEBUG("elem of vector vec %d \n", *iter);
        //         cJSON_AddItemToArray(arrayJson, cJSON_CreateNumber(*iter));
        //     }
        //     cJSON_AddItemToObject(jData, "unlock_cars", arrayJson);
             
        // }
        // else 
        //!alter end:此action 已经不再使用
        if(/*(nAction == _BUMPER_TRANSFER_UPLOAD_USER_DATA_GAMEOVER_SUC) || */(nAction ==  _BUMPER_TRANSFER_INVITE_USER_SUC)|| (nAction == _BUMPER_TRANSFER_IS_FRINED_JOIN_SUC) || (nAction == _BUMPER_TRANSFER_USE_CAR_SUC))
        {
            cJSON_AddNumberToObject(m_pMsg, "errno", 0);
            cJSON_AddStringToObject(m_pMsg, "errmsg", "");
            cJSON_AddStringToObject(m_pMsg, "data", "");
            cJSON_AddNumberToObject(m_pMsg, "use_car", nData);
        }
        //add by huyf 2019.01.09:设置拖尾
        else 
        if(nAction == _BUMPER_TRANSFER_TAIL_SUC)
        {
            cJSON_AddNumberToObject(m_pMsg, "tail_id", nData);
        }
        else 
        if(nAction == _BUMPER_TRANSFER_TAIL_FAIL)
        {
        }
        //add end:设置拖尾
        else if(/*(nAction == _BUMPER_TRANSFER_GET_USER_INFO_FAIL) || (nAction == _BUMPER_TRANSFER_UPLOAD_USER_DATA_GAMEOVER_FAIL)  || */(nAction == _BUMPER_TRANSFER_UPLOAD_GOLDNUM_KILLNUM_FAIL) ||(nAction == _BUMPER_TRANSFER_IS_FRINED_JOIN_FAIL) || (nAction == _BUMPER_TRANSFER_GET_NOT_DECREASE_STAR_GIFT_FAIL)|| (nAction == _BUMPER_TRANSFER_GET_BIG_GIFT_FAIL))
            
        {
            cJSON_AddNumberToObject(m_pMsg, "errno", SafeAtoi(data["errno"].c_str()));
            cJSON_AddStringToObject(m_pMsg, "errmsg", data["errmsg"].c_str());
        }
        //!alter by huyf 2019.01.12:此action 已经不再使用
//         else if(nAction == _BUMPER_TRANSFER_UNLOCK_CAR_SUC)
//         {
//             cJSON_AddNumberToObject(m_pMsg, "errno", 0);
//             cJSON_AddStringToObject(m_pMsg, "errmsg", "");
        
//             cJSON* jData = NULL;
//             cJSON_AddItemToObject(m_pMsg, "data", jData=cJSON_CreateObject());

//             // msg json: { "gold_num" : pUserData->GetGoldSum(),  "unlock_cars" : vec, "status" : 1}
//             LOGDEBUG("data[gold_num]: %s\n", data["gold_num"].c_str());
//             cJSON_AddNumberToObject(jData, "gold_num", SafeAtoi(data["gold_num"].c_str()));
//             cJSON_AddNumberToObject(jData, "status", SafeAtoi(data["status"].c_str()));
//             cJSON_AddNumberToObject(jData, "car_id", SafeAtoi(data["car_id"].c_str()));
// #if 0
//             vector<int> vec;
//             splitString(data["unlock_cars"], ",", vec);
//             cJSON* arrayJson = cJSON_CreateArray();
//             for(vector<int>::iterator iter = vec.begin(); iter != vec.end(); ++iter)
//             {
//                 LOGDEBUG("elem of vector vec %d \n", *iter);
//                 cJSON_AddItemToArray(arrayJson, cJSON_CreateNumber(*iter));
//             }
//             cJSON_AddItemToObject(jData, "unlock_cars", arrayJson);

// #endif
//         }
        // else if(nAction == _BUMPER_TRANSFER_UNLOCK_CAR_FAIL)
        // {
        //     cJSON_AddNumberToObject(m_pMsg, "errno", 0);
        //     cJSON_AddStringToObject(m_pMsg, "errmsg", "");
        
        //     cJSON* jData = NULL;
        //     cJSON_AddItemToObject(m_pMsg, "data", jData=cJSON_CreateObject());
        //     cJSON_AddNumberToObject(jData, "status", SafeAtoi(data["status"].c_str()));
        //     cJSON_AddNumberToObject(jData, "car_id", SafeAtoi(data["car_id"].c_str()));
            
        // }
        //!alter end:此action 已经不再使用

        m_pData = cJSON_Print(m_pMsg);
        string strMsg(m_pData);
        INT nLen = strMsg.size();
        LOGDEBUG("[CMsgBumperTransfer::CreateMsg]:\n m_pData=[%s] nLen=[%d]\n", m_pData, nLen);
    }

    return TRUE;
}

BOOL CMsgBumperTransfer::SendMsg(CNetMsg* pMsg) 
{
    m_pPlayer->SendMsgToClient(pMsg);
    return TRUE;
}

UINT CMsgBumperTransfer::Process(VOID* pInfo) 
{

    LOGDEBUG("[CMsgBumperTransfer::Process] begin...");
    
    DEBUG_TRY

    m_pPlayer = static_cast<Player*>(pInfo);
    CHECKF(m_pPlayer);

    m_pMsg = cJSON_Parse(this->m_pData);
    cJSON* pJsonMsg = m_pMsg;
    if(pJsonMsg) 
    {

        cJSON* pAction = cJSON_GetObjectItem(pJsonMsg, "action");
        CHECKF(pAction);
        LOGDEBUG("[CMsgBumperTransfer::Process] action=[%d] pPlayer=[%p]\n", pAction->valueint, m_pPlayer);

        switch(pAction->valueint) 
        {

            //!alter by huyf 2019.01.12:此action 已经不再使用
            //获取用户数据         input param: user_id,  output param:  errno, errmsg, data:nickname, avatar_url, gold_num, 
            // kill_num, granding_id, star_num, unlock_cars, granding_num, use_car
            /*
            case _BUMPER_TRANSFER_GET_USER_INFO:
            {

                cJSON* pUserId = cJSON_GetObjectItem(pJsonMsg, "user_id");
                CHECKF(pUserId);
                
                CUserData* pUserData = m_pPlayer->QueryData();
                
                pUserData->SetUid(pUserId->valueint);
                pUserData->LoadInfo(pUserData->GetUid());

                //!alter by huyf 2019.01.02:优化不再操作数据库,玩家在线过凌晨的话不会自动更新,不修改了就这样吧
                if(pUserData->GetNickName() != "default_initial")
                {
                        string currentDay = getCurrentYearMonDay();
                        if(pUserData->GetLastDay() != currentDay)
                        {
                            //json:   data{ 'last_day': currentDay, 'keep_star': 0, 'get_gift': 0 }
                            string last_day = currentDay;
                            int keep_star = 0;
                            int get_gift = 0;

                            char szSql[512];
                            memset(szSql, 0, sizeof(szSql)/sizeof(*szSql));
                            sprintf(szSql, "UPDATE g_bumper_info SET \`last_day\`=\"%s\", keep_star=%d, get_gift=%d where user_id=%d",currentDay.c_str(), keep_star, get_gift, pUserId->valueint);
                            LOGDEBUG("sql statement szSql: %s\n", szSql);
                            Database()->UpdateRecord(szSql);

                            // m_pPlayer->SetLastDay(currentDay);
                            // m_pPlayer->SetKeepStar(0);
                            // m_pPlayer->SetGetGift(0);
                        }
                 }      
                 //!alter end:优化不再操作数据库,玩家在线过凌晨的话不会自动更新,不修改了就这样吧

                string sql = "SELECT u.nickname, u.avatar_url, b.gold_num, b.kill_num, b.granding_id, b.star_num, " 
                                     "b.use_car, b.unlock_cars, b.keep_star, b.get_gift FROM g_wechat_user u, g_bumper_info b" 
                                     " WHERE u.id=b.user_id AND b.user_id=" + std::to_string(pUserId->valueint);
 
                MYSQL_RES* result = Database()->Query(sql);
                BOOL bExist = FALSE;
                MYSQL_ROW row;
                enum
                {
                    _NICKNAME       = 0,
                    _AVATAR_URL,
                    _GOLD_NUM,
                    _KILL_NUM,
                    _GRANDING_ID,
                    _STAR_NUM,
                    _USE_CAR,
                    _UNLOCK_CARS,
                    _KEEP_STAR,
                    _GET_GIFT,

                };

                map<string, string> data;    
                while((row = mysql_fetch_row(result)))
                {
                    bExist= TRUE;
                    
                    pUserData->SetNickName(row[_NICKNAME]);
                    data["nickname"] = row[_NICKNAME];
                    
                    pUserData->SetHeadPicurl(row[_AVATAR_URL]);
                    data["avatar_url"] = row[_AVATAR_URL];
                    //!alter by huyf 2019.01.12:金币调用修改
                    //pUserData->SetGoldSum(SafeAtoi(row[_GOLD_NUM]));
                    m_pPlayer->SetGoldSum(SafeAtoi(row[_GOLD_NUM]));
                    //!alter end:金币调用修改
                    data["gold_num"] = row[_GOLD_NUM];

                    pUserData->SetKillSum(SafeAtoi(row[_KILL_NUM]));
                    data["kill_num"] = row[_KILL_NUM];
                    
                    pUserData->SetGrandingId(SafeAtoi(row[_GRANDING_ID]));
                    data["granding_id"] = row[_GRANDING_ID];
                    
                    pUserData->SetCurStarSum(SafeAtoi(row[_STAR_NUM]));
                    data["star_num"] = row[_STAR_NUM];

                    pUserData->SetUseCar(SafeAtoi(row[_USE_CAR]));
                    data["use_car"] = row[_USE_CAR];
                    
                    pUserData->SetUnlockSum(row[_UNLOCK_CARS]);
                    data["unlock_cars"] = row[_UNLOCK_CARS];
                    
                    pUserData->SetKeepStar(SafeAtoi(row[_KEEP_STAR]));
                    data["keep_star"] = row[_KEEP_STAR];
                    
                    pUserData->SetGetGift(SafeAtoi(row[_GET_GIFT]));
                    data["get_gift"] = row[_GET_GIFT];
                }

                if(!bExist)
                {
                    CMsgBumperTransfer msg;
                    
                    map<string, string> data;
                    data["errno"] = std::to_string(321);
                    data["errmsg"] = "没有该用户信息";
                    
                    IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_GET_USER_INFO_FAIL, data)) 
                    {
                        SendMsg(&msg);
                    }
                    return PACKET_EXE_CONTINUE;

                }

                //!alter by huyf 2018.12.15:修改此处不要数据库查询太耗时
                data["granding_num"] = "100";
                // if(pUserData->GetGrandingId() > 0)
                // {
                //     string sql = "SELECT COUNT(DISTINCT user_id) FROM g_bumper_info WHERE granding_id =" + std::to_string(pUserData->GetGrandingId());
                //     MYSQL_RES* result = Database()->Query(sql);
                //     MYSQL_ROW row;
                //     if(row = mysql_fetch_row(result))
                //     {
                //         data["granding_num"] = row[0];
                //     }
                // }
                //!alter end:修改此处不要数据库查询太耗时
                
                CMsgBumperTransfer msg;
                IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_GET_USER_INFO_SUC, data)) 
                {
                    SendMsg(&msg);
                }
                return PACKET_EXE_CONTINUE;
            }
            break; 
            */
            //!alter end:此action 已经不再使用

            //!alter by huyf 2019.01.12:此action 已经不再使用
            /*
            case _BUMPER_TRANSFER_UPLOAD_USER_DATA_GAMEOVER:
            {

                cJSON* pUserId  = cJSON_GetObjectItem(pJsonMsg, "user_id");
                cJSON* pGoldNum = cJSON_GetObjectItem(pJsonMsg, "gold_num");
                cJSON* pKillNum = cJSON_GetObjectItem(pJsonMsg, "kill_num");
                cJSON* pGrandingId = cJSON_GetObjectItem(pJsonMsg, "granding_id");
                cJSON* pStarNum    = cJSON_GetObjectItem(pJsonMsg, "star_num");
                
                CHECKF(pUserId && pGoldNum && pKillNum && pGrandingId && pStarNum);

                // if user_id用户不存在于tbl g_bumper_info;
                #if 0
                {
                    CMsgBumperTransfer msg;
                    
                    map<string, string> data;
                    data["errno"] = std::to_string(321);
                    data["errmsg"] = "没有该用户信息";
                    
                    IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_UPLOAD_USER_DATA_GAMEOVER_FAIL, data)) 
                    {
                        SendMsg(&msg);
                    }
                    return PACKET_EXE_CONTINUE;
                }
                #endif

                
                CUserData* pUserData = m_pPlayer->QueryData();
                
                    pUserData->SetUid(pUserId->valueint);
                    //!alter by huyf 2019.01.12:金币调用修改
                    //pUserData->SetGoldSum(pUserData->GetGoldSum() + pGoldNum->valueint);
                    m_pPlayer->SetGoldSum(m_pPlayer->GetGoldSum() + pGoldNum->valueint);
                    //!alter end:金币调用修改
                    pUserData->SetKillSum(pUserData->GetKillSum() + pKillNum->valueint); 
                    
                    pUserData->SetGrandingId(pGrandingId->valueint);
                    pUserData->SetCurStarSum(pStarNum->valueint);

                    CMsgBumperTransfer msg;
                    map<string,string> mss;
                    IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_UPLOAD_USER_DATA_GAMEOVER_SUC, mss)) 
                    {
                        SendMsg(&msg);
                    }
                    return PACKET_EXE_CONTINUE;
            }
            break; 
            */
            //!alter end:此action 已经不再使用

            //!alter by huyf 2019.01.12:此action 已经不再使用
            //!转到商城点卷信息包
            /*
            case _BUMPER_TRANSFER_UNLOCK_CAR:
            {
                cJSON* pUserId = cJSON_GetObjectItem(pJsonMsg, "user_id");
                cJSON* pCarId = cJSON_GetObjectItem(pJsonMsg, "car_id");
                CHECKF(pUserId && pCarId);

                CUserData* pUserData = m_pPlayer->QueryData();

                pUserData->SetUid(pUserId->valueint);

//                pUserData->LoadInfo(pUserData->GetUid());

                bool bExist = FALSE;
                if(pUserData->GetHeadPicurl() !="default_initial") 
                {
                    bExist = TRUE;
                }

               if(bExist)
               {
               //        vector<string> vecShare = { "默认", "好友邀请", "段位 21", "1000金币", "5000金币", "30000金币", "50000金币"};
                       
                       string unlock_cars = pUserData->GetUnlockSum();
                       vector<int> vec;
                       splitString(unlock_cars, ",", vec);

                       vector<int>::iterator iter = find(vec.begin(), vec.end(), pCarId->valueint);

                       //没有找到元素car_id
                       if(iter == vec.end())
                       {
                           int condition_id = 0;
                           int value = 0;
                           
                           string sql = "select condition_id, value from g_bumper_cars where id=" + std::to_string(pCarId->valueint);
                           MYSQL_RES* result = Database()->Query(sql);
			               MYSQL_ROW row;

                           enum
                           {
                                _CONDITION_ID   = 0,
                                    _VALUE      = 1,
                           };

                           while(row = mysql_fetch_row(result))
                           {
                                condition_id = SafeAtoi(row[_CONDITION_ID]);
                                value = SafeAtoi(row[_VALUE]);
                           }
                           
                           switch(condition_id) 
                           {
                                case 0:
                                {
                                    map<string, string> data;
                                    data["car_id"] = std::to_string(pCarId->valueint);
                                    data["status"] = "2";
                            
                                    CMsgBumperTransfer msg;
                                    IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_UNLOCK_CAR_SUC, data)) 
                                    {
                                        SendMsg(&msg);
                                    }
                                    return PACKET_EXE_CONTINUE;
                                    
                                }
                                break;

                                case 1:
                                {
                                    sql = "select parent_id from g_bumper_master where parent_id=" + std::to_string(pUserId->valueint);
                                    MYSQL_RES* result = Database()->Query(sql);
                                    if(!(row = mysql_fetch_row(result)))
                                    {
                                        map<string, string> data;
                                        data["status"] = "-1";
                                        data["car_id"] = std::to_string(pCarId->valueint);
                                
                                        CMsgBumperTransfer msg;
                                        IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_UNLOCK_CAR_FAIL, data)) 
                                        {
                                            SendMsg(&msg);
                                        }
                                        return PACKET_EXE_CONTINUE;
                                    }
                                    #if 1

                                   if(!unlock_cars.empty()) 
                                   {
                                       unlock_cars.append(",");
                                       unlock_cars.append(std::to_string(pCarId->valueint));

                                   }
                                   else
                                   {
                                       unlock_cars = std::to_string(pCarId->valueint);
                                   }
                                   
                                   pUserData->SetUnlockSum(unlock_cars);
                                   
                                   cJSON* pParentId = cJSON_GetObjectItem(pJsonMsg, "parent_id");
                                   if(pParentId) 
                                   {
                                       char szSql[512];
                                       memset(szSql, 0, sizeof(szSql)/sizeof(*szSql));
                                       sprintf(szSql, "insert into g_bumper_invite(user_id, parent_id, car_id) values(%d,%d,%d)", \
                                                      pUserId->valueint, pParentId->valueint, std::to_string(pCarId->valueint).c_str());
                                       Database()->UpdateRecord(szSql);
                                   }
                                   
                                   vec.push_back(pCarId->valueint);

                                   // msg json: { "gold_num" : pUserData->GetGoldSum(),  "unlock_cars" : vec, "status" : 1}

                                   map<string, string> data;
                                   //!alter by huyf 2019.01.12:金币调用修改
                                   //data["gold_num"] = std::to_string(pUserData->GetGoldSum());
                                   data["gold_num"] = std::to_string(m_pPlayer->GetGoldSum());
                                   //!alter end:金币调用修改
                                   data["status"] = std::to_string(1);
                                   data["car_id"] = std::to_string(pCarId->valueint);
                                   for(auto iter = vec.begin(); iter != vec.end(); ++iter)
                                   {
                                      data["unlock_cars"].append(std::to_string(*iter));
                                   }
                                   
                                   CMsgBumperTransfer msg;
                                   IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_UNLOCK_CAR_SUC, data))
                                   {
                                      SendMsg(&msg);
                                   }
      							               return PACKET_EXE_CONTINUE;

                                    #endif
                                }
                                break;
                                
                                case 2:
                                {
                                    if(pUserData->GetGrandingId() < value) 
                                    {
                                        map<string, string> data;
                                         data["car_id"] = std::to_string(pCarId->valueint);
                                        data["status"] = "-1";
                                
                                        CMsgBumperTransfer msg;
                                        IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_UNLOCK_CAR_FAIL, data)) 
                                        {
                                            SendMsg(&msg);
                                        }
                                         return PACKET_EXE_CONTINUE;
                                    }
                                }
                                break;

                                case 3:
                                {
                                    //!alter by huyf 2019.01.12:金币调用修改
                                    //if(pUserData->GetGoldSum() < value)
                                    if(m_pPlayer->GetGoldSum() < value)
                                    //!alter end:金币调用修改
                                    {
                                        map<string, string> data;
                                        data["car_id"] = std::to_string(pCarId->valueint);
                                        data["status"] ="-1";
                                
                                        CMsgBumperTransfer msg;
                                        IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_UNLOCK_CAR_SUC, data)) 
                                        {
                                            SendMsg(&msg);
                                        }
                                         return PACKET_EXE_CONTINUE;
                                    } 
                                    else 
                                    {
                                        //!alter by huyf 2019.01.12:金币调用修改
                                        //pUserData->SetGoldSum(pUserData->GetGoldSum() - value);
                                        m_pPlayer->SetGoldSum(m_pPlayer->GetGoldSum() - value);
                                        //!alter end:金币调用修改
                                    }
                                }
                                break;

                                case 4:
                                {
      									           if(!unlock_cars.empty()) 
                                   {
                                       unlock_cars.append(",");
                                       unlock_cars.append(std::to_string(pCarId->valueint));

                                   }
                                   else
                                   {
                                       unlock_cars = std::to_string(pCarId->valueint);
                                   }
                                   
                                   pUserData->SetUnlockSum(unlock_cars);
                                   
                                   cJSON* pParentId = cJSON_GetObjectItem(pJsonMsg, "parent_id");
                                   if(pParentId) 
                                   {
                                       char szSql[512];
                                       memset(szSql, 0, sizeof(szSql)/sizeof(*szSql));
                                       sprintf(szSql, "insert into g_bumper_invite(user_id, parent_id, car_id) values(%d,%d,%d)", \
                                                      pUserId->valueint, pParentId->valueint, pCarId->valuestring);
                                       Database()->UpdateRecord(szSql);
                                   }
                                   
                                   vec.push_back(pCarId->valueint);

                                   // msg json: { "gold_num" : pUserData->GetGoldSum(),  "unlock_cars" : vec, "status" : 1}

                                   map<string, string> data;
                                   //!alter by huyf 2019.01.12:金币调用修改
                                   //data["gold_num"] = std::to_string(pUserData->GetGoldSum());
                                   data["gold_num"] = std::to_string(m_pPlayer->GetGoldSum());
                                   //!alter end:金币调用修改
                                   data["status"] = std::to_string(1);
                                   data["car_id"] = std::to_string(pCarId->valueint);
                                   for(auto iter = vec.begin(); iter != vec.end(); ++iter)
                                   {
                                      data["unlock_cars"].append(std::to_string(*iter));
                                   }
                                   
                                   CMsgBumperTransfer msg;
                                   IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_UNLOCK_CAR_SUC, data)) 
                                   {
                                      SendMsg(&msg);
                                   }
      							               return PACKET_EXE_CONTINUE;
                                } 
                                break;

#if 0
                                default:
                                {
                                    map<string, string> data;
                                    data["status"] = "-1";
                            
                                    CMsgBumperTransfer msg;
                                    IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_UNLOCK_CAR_SUC, data)) 
                                    {
                                        SendMsg(&msg);
                                    }
                                     return PACKET_EXE_CONTINUE;
                                }
                                break;
#endif
                             }
                             
                             if(!unlock_cars.empty()) 
                             {
                                 unlock_cars.append(",");
                                 unlock_cars.append(std::to_string(pCarId->valueint));

                             }
                             else
                             {
                                 unlock_cars = std::to_string(pCarId->valueint);
                             }
                             
                             pUserData->SetUnlockSum(unlock_cars);
                             
                             cJSON* pParentId = cJSON_GetObjectItem(pJsonMsg, "parent_id");
                             if(pParentId) 
                             {
                                 char szSql[512];
                                 memset(szSql, 0, sizeof(szSql)/sizeof(*szSql));
                                 sprintf(szSql, "insert into g_bumper_invite(user_id, parent_id, car_id) values(%d,%d,%d)", \
                                                pUserId->valueint, pParentId->valueint, pCarId->valuestring);
                                 Database()->UpdateRecord(szSql);
                             }
                             
                             vec.push_back(pCarId->valueint);

                             // msg json: { "gold_num" : pUserData->GetGoldSum(),  "unlock_cars" : vec, "status" : 1}

                             map<string, string> data;
                             //!alter by huyf 2019.01.12:金币调用修改
                             //data["gold_num"] = std::to_string(pUserData->GetGoldSum());
                             data["gold_num"] = std::to_string(m_pPlayer->GetGoldSum());
                             //!alter end:金币调用修改
                             data["status"] = std::to_string(1);
                             data["car_id"] = std::to_string(pCarId->valueint);
                             for(auto iter = vec.begin(); iter != vec.end(); ++iter)
                             {
                                data["unlock_cars"].append(std::to_string(*iter));
                             }
                             
                             CMsgBumperTransfer msg;
                             IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_UNLOCK_CAR_SUC, data)) 
                             {
                                SendMsg(&msg);
                             }
                             
                       }
                       else 
                       {   
                            //找到元素
                            map<string, string> data;
                            //!alter by huyf 2019.01.12:金币调用修改
                            //data["gold_num"] = std::to_string(pUserData->GetGoldSum());
                            data["gold_num"] = std::to_string(m_pPlayer->GetGoldSum());
                            //!alter end:金币调用修改
                             data["status"] = std::to_string(2);
                             data["car_id"] = std::to_string(pCarId->valueint);
                            
                            CMsgBumperTransfer msg;
                            IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_UNLOCK_CAR_SUC, data)) 
                            {
                                SendMsg(&msg);
                            }
                       }
                   }
                   else
                   {
                        char szSql[512];
                        memset(szSql, 0, sizeof(szSql)/sizeof(*szSql));
                        sprintf(szSql, "insert into g_bumper_info(user_id, unlock_cars) values(%d,%s)", \
                                       pUserId->valueint, std::to_string(pCarId->valueint).c_str());
                        Database()->UpdateRecord(szSql);

#if 0
                        map<string, string> data;
                        CMsgBumperTransfer msg;
                        IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_UNLOCK_CAR_SUC, data)) 
                        {
                            SendMsg(&msg);
                        }
#endif
                   }

                    return PACKET_EXE_CONTINUE;
            }
            break; 
            */
            //!alter end:此action 已经不再使用

            case _BUMPER_TRANSFER_INVITE_USER:
            {
                cJSON* pUserId = cJSON_GetObjectItem(pJsonMsg, "user_id");
                cJSON* pParentId = cJSON_GetObjectItem(pJsonMsg, "parent_id");
                CHECKF(pUserId && pParentId);
                
                CUserData* pUserData = m_pPlayer->QueryData();
                pUserData->SetUid(pUserId->valueint);
                char szSql[512];
                memset(szSql, 0, sizeof(szSql)/sizeof(*szSql));
                sprintf(szSql, "insert into g_bumper_master(parent_id, user_id) values(%d,%d)",\
                               pParentId->valueint, pUserId->valueint);

                Database()->UpdateRecord(szSql);

                CMsgBumperTransfer msg;
                map<string, string> mss;
                IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_INVITE_USER_SUC, mss)) 
                {
                    SendMsg(&msg);
                }
               
                return PACKET_EXE_CONTINUE;
            }
            break; 

            case _BUMPER_TRANSFER_IS_FRINED_JOIN:
            {

                cJSON* pUserId = cJSON_GetObjectItem(pJsonMsg, "user_id");
                CHECKF(pUserId);
                CUserData* pUserData = m_pPlayer->QueryData();

                pUserData->SetUid(pUserId->valueint);
                
                string sql = "SELECT * FROM g_bumper_master WHERE parent_id=" + std::to_string(pUserId->valueint);
                MYSQL_RES* result = Database()->Query(sql);
                MYSQL_ROW row;
                BOOL bFind = FALSE;
                //if(row = mysql_fetch_row(result))
                while((row = mysql_fetch_row(result)))
                {
                     CMsgBumperTransfer msg;
                      map<string, string> mss;
                     IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_IS_FRINED_JOIN_SUC, mss))
                     {
                        SendMsg(&msg);
                     }

                     bFind = TRUE;

                     //!alter by huyf 2019.01.12:修改内存泄漏
                      mysql_free_result(result);
                      result = NULL;
                      //!alter end:修改内存泄漏
                      return PACKET_EXE_CONTINUE;
                }
                if(!bFind)
                {
                     CMsgBumperTransfer msg;
                     map<string, string> data;
                     data["errno"] = "417";
                     data["errmsg"] = "没有邀请到好友"; 
                     IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_IS_FRINED_JOIN_FAIL, data))
                     {
                        SendMsg(&msg);
                     }
                }
                //!alter by huyf 2019.01.12:修改内存泄漏
                mysql_free_result(result);
                result = NULL;
                //!alter end:修改内存泄漏
                return PACKET_EXE_CONTINUE;
            }
            break; 

            case _BUMPER_TRANSFER_USE_CAR:
            {
                cJSON* pUserId = cJSON_GetObjectItem(pJsonMsg, "user_id");
                cJSON* pCarId  = cJSON_GetObjectItem(pJsonMsg, "car_id");         //道具唯一id
                CHECKF(pUserId && pCarId);
                
                CUserData* pUserData = m_pPlayer->QueryData();
                //玩家道具管理器
                CItemDataManager* pItemDataManager = m_pPlayer->QueryItemDataManager();
                if (pItemDataManager && pUserData)
                {
                  CItemData* pItemData = pItemDataManager->QueryItemData(pCarId->valueint);
                  if (pItemData)
                  {
                    //pUserData->SetUid(pUserId->valueint);
                    pUserData->SetUseCar(pCarId->valueint);
                    CMsgBumperTransfer msg;
                    map<string, string> mss;
                    IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_USE_CAR_SUC, mss, pCarId->valueint))
                    {
                        SendMsg(&msg);
                    }
                  }
                  else
                  {
                    //pUserData->SetUid(pUserId->valueint);
                    pUserData->SetUseCar(0);

                    CMsgBumperTransfer msg;
                    map<string, string> mss;
                    IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_USE_CAR_SUC, mss, 0))
                    {
                        SendMsg(&msg);
                    }
                  }
                }
                return PACKET_EXE_CONTINUE;
            }
            break; 
            //add by huyf 2019.01.09:设置拖尾
            case _BUMPER_TRANSFER_TAIL:
            {
                cJSON* pTailId  = cJSON_GetObjectItem(pJsonMsg, "tail_id");         //道具唯一id
                CHECKF(pTailId);
                
                CUserData* pUserData = m_pPlayer->QueryData();
                //玩家道具管理器
                CItemDataManager* pItemDataManager = m_pPlayer->QueryItemDataManager();
                if (pItemDataManager && pUserData)
                {
                    CItemData* pItemData = pItemDataManager->QueryItemData(pTailId->valueint);
                    if (pItemData)
                    {
                        //检查是否为拖尾道具
                        if (pItemData->GetType() == ITEM_TYPE_CAR_TAIL) ////拖尾类道具
                        {
                            pUserData->SetTail(pTailId->valueint);
                            CMsgBumperTransfer msg;
                            map<string, string> mss;
                            IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_TAIL_SUC, mss, pTailId->valueint))
                            {
                                SendMsg(&msg);
                            }
                        }
                        else
                        {
                            //pUserData->SetTail(0, TRUE);

                            CMsgBumperTransfer msg;
                            map<string, string> mss;
                            IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_TAIL_FAIL, mss, 0))
                            {
                                SendMsg(&msg);
                            }
                        }
                    }
                    else
                    {
                        //pUserData->SetTail(0, TRUE);

                        CMsgBumperTransfer msg;
                        map<string, string> mss;
                        IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_TAIL_FAIL, mss, 0))
                        {
                            SendMsg(&msg);
                        }
                    }
                }
                return PACKET_EXE_CONTINUE;
            }
            break; 
            //add end:设置拖尾

            case _BUMPER_TRANSFER_RANK_GET_GRANDING_STARNUM:
            {
                cJSON* pUserId = cJSON_GetObjectItem(pJsonMsg, "user_id");
                cJSON* pRank = cJSON_GetObjectItem(pJsonMsg, "rank");
                CHECKF(pUserId && pRank);
                
                CUserData* pUserData = m_pPlayer->QueryData();
                if(pUserData)
                {
                    pUserData->SetUid(pUserId->valueint);
                    pUserData->LoadInfo(pUserData->GetUid());
                    UINT starNum = pUserData->GetCurStarSum();
                    UINT grandingId = pUserData->GetGrandingId();
                    map<string, int> data;
                    data["granding_id"] = grandingId;
                    data["star_num"] = starNum;
                    
                   // pUserData->SetStatus(0);
                    data["status"] = 0;
                    
                    if((1 <= grandingId) && (grandingId <= 31))
                    {
                        int rank = pRank->valueint;
                        if((1 <= grandingId) && (grandingId <= 5)) 
                        {
                            if(rank == 1)
                            {
                                insStar(data, 2);
                            }
                            else if(rank >= 2 && rank <= 5)
                            {
                                insStar(data, 1);
                            }
                        }
                        else 
                        if((6 <= grandingId) && (grandingId <= 10)) 
                        {
                            if(rank == 1)
                            {
                                insStar(data, 2);
                            }
                            else if(rank >= 2 && rank <= 4)
                            {
                                insStar(data, 1);
                            }
                        }
                        else 
                        if((11 <= grandingId) && (grandingId <= 15)) 
                        {
                            if(rank == 1)
                            {
                                insStar(data, 2);
                            }
                            else if(rank >= 2 && rank <= 3)
                            {
                                insStar(data, 1);
                            }
                            else if(rank >= 8 && rank <= 10)
                            {
                                insStar(data, -1);
                            }

                        }
                        else 
                        if((16 <= grandingId) && (grandingId <= 20)) 
                        {
                             if(rank == 1)
                            {
                                insStar(data, 2);
                            }
                            else if(rank == 2)
                            {
                                insStar(data, 1);
                            }
                            else if(rank >= 5 && rank <= 10)
                            {
                                insStar(data, -1);
                            }

                        }
                        else 
                        if((21 <= grandingId) && (grandingId <= 30)) 
                        {
                             if(rank == 1)
                            {
                                insStar(data, 2);
                            }
                            else if(rank == 2)
                            {
                                insStar(data, 1);
                            }
                            else if(rank >= 4 && rank <= 10)
                            {
                                insStar(data, -1);
                            }

                        }
                        else 
                        {
                            if(rank == 1)
                            {
                                insStar(data, 1);
                            }
                            else if(rank >= 3 && rank <= 12)
                            {
                                insStar(data, -1);
                            }
                        }
                    }
                    else 
                    {
                        CMsgBumperTransfer msg;
                        map<string, string> mss;
                        IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_RANK_GET_GRANDING_STARNUM_FAIL, mss))
                        {
                              SendMsg(&msg);
                        }
                    }

                   pUserData->SetCurStarSum(data["star_num"]);
                   pUserData->SetGrandingId(data["granding_id"]);
                   //pUserData->SetStatus("status");
                   //data["status"] = 0;

                   CMsgBumperTransfer msg;
                   IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_RANK_GET_GRANDING_STARNUM_SUC, data))
                   {
                        SendMsg(&msg);
                   }
                   return PACKET_EXE_CONTINUE;
                }
                else 
                {
                    CMsgBumperTransfer msg;
                    map<string, string> mss;
                    IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_RANK_GET_GRANDING_STARNUM_FAIL, mss))
                    {
                          SendMsg(&msg);
                    }
                    return PACKET_EXE_CONTINUE;
                }
            }
            break; 

            case _BUMPER_TRANSFER_UPLOAD_GOLDNUM_KILLNUM:
            {
                cJSON* pUserId  = cJSON_GetObjectItem(pJsonMsg, "user_id");
                cJSON* pGoldNum = cJSON_GetObjectItem(pJsonMsg, "gold_num");
                cJSON* pKillNum = cJSON_GetObjectItem(pJsonMsg, "kill_num");
                CHECKF(pUserId && pGoldNum && pKillNum);

                //  if param pUserId->valueint 不存在于tbl g_bumper_info中，返回信息 msg: errno 321, "no user 
                // info of user_id"
                #if 0
                {
                    CMsgBumperTransfer msg;
                    map<string, string> data;
                    data["errno"] = std::to_string(321);
                    data["errmsg"] = "没有该用户信息";
                    IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_UPLOAD_GOLDNUM_KILLNUM_FAIL, data)) 
                    {
                        SendMsg(&msg);
                    }
                    
                    return PACKET_EXE_CONTINUE;
                }
                #endif
                
                CUserData* pUserData = m_pPlayer->QueryData();
            //  pUserData->SetUid(pUserId->valueint);
            //  //!alter by huyf 2019.01.12:金币调用修改
                //pUserData->SetGoldSum(pUserData->GetGoldSum() + pGoldNum->valueint);
                //m_pPlayer->SetGoldSum(m_pPlayer->GetGoldSum() + pGoldNum->valueint);
                m_pPlayer->AwardItem(130010, pGoldNum->valueint);
                //!alter end:金币调用修改
                pUserData->SetKillSum(pUserData->GetKillSum() + pKillNum->valueint);   

                CMsgBumperTransfer msg;
                map<string, string> mss;
                IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_UPLOAD_GOLDNUM_KILLNUM_SUC, mss))
                {
                    SendMsg(&msg);
                }
                 return PACKET_EXE_CONTINUE;
                
            }
            break; 

            case _BUMPER_TRANSFER_GET_NOT_DECREASE_STAR_GIFT:
            {
                cJSON* pUserId = cJSON_GetObjectItem(pJsonMsg, "user_id");
                CHECKF(pUserId);
                CUserData* pUserData = m_pPlayer->QueryData();

                pUserData->SetUid(pUserId->valueint);
                pUserData->LoadInfo(pUserData->GetUid());
                
                if(pUserData->GetKeepStar() >= 5)
                {
                    CMsgBumperTransfer msg;
                    map<string, string> data;
                    data["errno"] = std::to_string(202);
                    data["errmsg"] = "不能继续加星";
                    IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_GET_NOT_DECREASE_STAR_GIFT_FAIL, data)) 
                    {
                         SendMsg(&msg);
                    }
                    return PACKET_EXE_CONTINUE;
                }

                map<string, int> data;

                int starNum = pUserData->GetCurStarSum() + 1;
                
                if(starNum >= 5)
                {
                    if(pUserData->GetGrandingId() < 31)
                    {
                        data["star_num"] = starNum - 5;
                        data["granding_id"] = pUserData->GetGrandingId() + 1;
                    }
                    else 
                    {
                        data["star_num"] = starNum;
                        data["granding_id"] = 31;
                    }
                }
                else 
                {
                    data["star_num"] = starNum;
                    data["granding_id"] = pUserData->GetGrandingId();
                }

                if((strcmp(pUserData->GetLastDay().c_str(), getCurrentYearMonDay())==0) && (pUserData->GetKeepStar() < 5))
                {
                    data["keep_star"] = pUserData->GetKeepStar() + 1;
                }
                
                pUserData->SetCurStarSum(data["star_num"]);
                pUserData->SetGrandingId(data["granding_id"]);
                pUserData->SetKeepStar(data["keep_star"]);

                CMsgBumperTransfer msg;
                IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_GET_NOT_DECREASE_STAR_GIFT_SUC, data))
                {
                    SendMsg(&msg);
                }
                return PACKET_EXE_CONTINUE;
            }
            break; 

            case _BUMPER_TRANSFER_GET_BIG_GIFT:
            {
                cJSON* pUserId = cJSON_GetObjectItem(pJsonMsg, "user_id");
                CHECKF(pUserId);
                CUserData* pUserData = m_pPlayer->QueryData();

                pUserData->SetUid(pUserId->valueint);
                pUserData->LoadInfo(pUserData->GetUid());

                if(pUserData->GetGetGift() >= 4)
                {
                    CMsgBumperTransfer msg;
                    map<string, string> data;
                    data["errno"] = std::to_string(202);
                    data["errmsg"] = "不能继续领礼包";
                    IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_GET_BIG_GIFT_FAIL, data)) 
                    {
                         SendMsg(&msg);
                    }
                    return PACKET_EXE_CONTINUE;
                }

                map<string, int> data;
                int starNum = pUserData->GetCurStarSum() + 1;
                
                if(starNum >= 5)
                {
                    if(pUserData->GetGrandingId() < 31)
                    {
                        data["star_num"] = starNum - 5;
                        data["granding_id"] = pUserData->GetGrandingId() + 1;
                    }
                    else 
                    {
                        data["star_num"] = starNum;
                        data["granding_id"] = 31;
                    }
                }
                else 
                {
                    data["star_num"] = starNum;
                    data["granding_id"] = pUserData->GetGrandingId();
                }

                //!alter by huyf 2019.01.12:金币调用修改
                //data["gold_num"] = pUserData->GetGoldSum() + 500;
                data["gold_num"] = m_pPlayer->GetGoldSum() + 500;
                //!alter end:金币调用修改

                if((strcmp(pUserData->GetLastDay().c_str(), getCurrentYearMonDay())==0) && (pUserData->GetGetGift() < 4))
                {
                    data["get_gift"] = pUserData->GetGetGift() + 1;
                    LOGDEBUG("data[get_gift]=%d\n", data["get_gift"]);
                }
                LOGDEBUG("111, data[get_gift]=%d\n", data["get_gift"]);

                pUserData->SetCurStarSum(data["star_num"]);
                pUserData->SetGrandingId(data["granding_id"]);
                //!alter by huyf 2019.01.12:金币调用修改
                //pUserData->SetGoldSum(data["gold_num"]);
                m_pPlayer->SetGoldSum(data["gold_num"]);
                //!alter end:金币调用修改
                pUserData->SetGetGift(data["get_gift"]);

                //!alter by huyf 2018.12.15:修改此处不要数据库查询太耗时
                data["granding_num"]= Random(500);
                // string sql = "SELECT COUNT(DISTINCT user_id) FROM g_bumper_info WHERE granding_id =" + std::to_string(pUserData->GetGrandingId());
                // MYSQL_RES* result = Database()->Query(sql);
                // MYSQL_ROW row;
                // if(row = mysql_fetch_row(result))
                // {
                //     data["granding_num"] = SafeAtoi(row[0]);
                // }
                //!alter end:修改此处不要数据库查询太耗时
                CMsgBumperTransfer msg;
                IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_GET_BIG_GIFT_SUC, data))
                {
                    SendMsg(&msg);
                }
                return PACKET_EXE_CONTINUE;
            }
            break; 

            case _BUMPER_TRANSFER_RANKING_LIST_BY_RANK:
            {
                RankTopListStruct* pUserInfo = new RankTopListStruct;
                pUserInfo->m_HeadPicurl = m_pPlayer-> GetHeadPicurl();
                pUserInfo->m_NickName = m_pPlayer->GetNickName();
                pUserInfo->m_nGrandingId = m_pPlayer->GetGrandingId();
                pUserInfo->m_nStarNum = m_pPlayer->GetCurStarSum();
                pUserInfo->m_nRank = 91;
                
                UINT cnt = 0;
                VT_RANK_TOP_LIST& vt_rank_top_list =  GetTopListManager()->GetVtRankTopList();

                VT_RANK_TOP_LIST vt_tmp_001;
                VT_RANK_TOP_LIST vt_tmp_002;
                VT_RANK_TOP_LIST vt_tmp_003;

                for(auto iter = vt_rank_top_list.begin(); iter != vt_rank_top_list.end(); ++iter)
                {
                     if((*iter)->m_nUserId == m_pPlayer->GetUid())
                     {
                        pUserInfo->m_nIsRank = true;
                     }
                }
                
                for(auto iter = vt_rank_top_list.begin(); iter != vt_rank_top_list.end(); ++iter)
                {
                     cnt++;
                     if(cnt<=10)
                     {
                        vt_tmp_001.push_back(*iter);
                        if(cnt==10)
                        {
                            CMsgBumperTransfer msg;
                            IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_RANKING_LIST_BY_RANK_SUC, vt_tmp_001, pUserInfo))
                            {
                                 this->SendMsg(&msg);
                            }
                        }
                     }
                     else if(cnt<=20)
                     {
                        vt_tmp_002.push_back(*iter);
                        if(cnt==20)
                        {
                           CMsgBumperTransfer msg;
                           IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_RANKING_LIST_BY_RANK_SUC, vt_tmp_002, NULL))
                           {
                                SendMsg(&msg);
                           }
                         }
                     }
                     else
                     {
                        vt_tmp_003.push_back(*iter);
                        if(cnt==30)
                        {
                            CMsgBumperTransfer msg;
                            IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_RANKING_LIST_BY_RANK_SUC, vt_tmp_003, NULL))
                            {
                                 SendMsg(&msg);
                            }
                        
                        }
                     }
                     
                }

                //!alter by huyf 2019.01.12:修改内存泄漏
                SAFE_DELETE(pUserInfo);
                //!alter end:修改内存泄漏

                return PACKET_EXE_CONTINUE;
           }
           break; 

           case _BUMPER_TRANSFER_RANKING_LIST_BY_KILLNUM:
           {
              KillNumTopListStruct* pUserInfo = new KillNumTopListStruct;
              pUserInfo->m_HeadPicurl = m_pPlayer-> GetHeadPicurl();
              pUserInfo->m_NickName = m_pPlayer->GetNickName();
              pUserInfo->m_nKillNum = m_pPlayer->GetKillSum();
              pUserInfo->m_nStarNum = m_pPlayer->GetCurStarSum();
              pUserInfo->m_nRank = 193;

              UINT cnt = 0;
              VT_KILLNUM_TOP_LIST& vt_kill_top_list =  GetTopListManager()->GetVtKillNumTopList();

              VT_KILLNUM_TOP_LIST vt_tmp_001;
              VT_KILLNUM_TOP_LIST vt_tmp_002;
              VT_KILLNUM_TOP_LIST vt_tmp_003;

              for(auto iter = vt_kill_top_list.begin(); iter != vt_kill_top_list.end(); ++iter)
              {
                  if((*iter)->m_nUserId == m_pPlayer->GetUid())
                  {
                      pUserInfo->m_nIsRank = true;
                  }
              }

              for(auto iter = vt_kill_top_list.begin(); iter != vt_kill_top_list.end(); ++iter)
              {
                  cnt++;
                  if(cnt<=10)
                  {
                      vt_tmp_001.push_back(*iter);
                      if(cnt==10)
                      {
                          CMsgBumperTransfer msg;
                          IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_RANKING_LIST_BY_KILLNUM_SUC, vt_tmp_001, pUserInfo))
                          {
                              this->SendMsg(&msg);
                          }
                      }
                  }
                  else if(cnt<=20)
                  {
                      vt_tmp_002.push_back(*iter);
                      if(cnt==20)
                      {
                          CMsgBumperTransfer msg;
                          IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_RANKING_LIST_BY_KILLNUM_SUC, vt_tmp_002, NULL))
                          {
                              SendMsg(&msg);
                          }
                      }
                  }
                  else
                  {
                      vt_tmp_003.push_back(*iter);
                      if(cnt==30)
                      {
                          CMsgBumperTransfer msg;
                          IF_OK(msg.CreateMsg(_BUMPER_TRANSFER_RANKING_LIST_BY_KILLNUM_SUC, vt_tmp_003, NULL))
                          {
                              SendMsg(&msg);
                          }
                      }
                  }
              }

              //!alter by huyf 2019.01.12:修改内存泄漏
              SAFE_DELETE(pUserInfo);
              //!alter end:修改内存泄漏
              return PACKET_EXE_CONTINUE;
           }
           break;

           default:
           {
                 return PACKET_EXE_CONTINUE;
           }
           break;
       }

    }

    return PACKET_EXE_ERROR;
    DEBUG_CATCH0("[CMsgBumperTransfer::Process] Is Catch...");

}


