#include "MsgUserInfo.h"

#include "../Base/logger.h"
#include "../Main/ServerManager.h"


//////////////////////////////////////////////////// CMsgUserInfo ////////////////////////////////////////////////////
//构造函数
CMsgUserInfo::CMsgUserInfo() 
{ 
}

//析构函数
CMsgUserInfo::~CMsgUserInfo() 
{ 	
}

//读取数据
BOOL CMsgUserInfo::Read(SocketInputStream& iStream, UINT nLen)
{
	CNetMsg::Read(iStream, nLen);
	return TRUE;
}

BOOL CMsgUserInfo::Write(SocketOutputStream& oStream) const
{
	return TRUE;
}

//创建消息
BOOL CMsgUserInfo::CreateMsg(UINT nRet)
{
	ASSERT(!"CMsgUserInfo::CreateMsg()");
	return TRUE;
}
//创建消息
BOOL CMsgUserInfo::CreateMsg(Player* player)
{
	CHECKF(player);
	if (m_pMsg)
	{
		// std::stringstream ss;
		// ss << pAccID->valueint;
		// std::string str = ss.str();
		
		CUserData* pUserData = player->QueryData();
		if (!pUserData)
		{
			LOGERROR("[CMsgUserInfo::Process] pUserData=NULL");
			return FALSE;
		}

		cJSON_AddNumberToObject(m_pMsg, "uid", pUserData->GetUid());

		cJSON_AddStringToObject(m_pMsg, "head_picurl", pUserData->GetHeadPicurl().c_str());
		cJSON_AddStringToObject(m_pMsg, "nick_name", pUserData->GetNickName().c_str());

		cJSON_AddNumberToObject(m_pMsg, "gold_sum", pUserData->GetGoldSum());
		cJSON_AddNumberToObject(m_pMsg, "kill_sum", pUserData->GetKillSum());
		cJSON_AddNumberToObject(m_pMsg, "granding_id", pUserData->GetGrandingId());
		cJSON_AddNumberToObject(m_pMsg, "cur_star_sum", pUserData->GetCurStarSum());

		cJSON_AddStringToObject(m_pMsg, "unlock_cars", pUserData->GetUnlockSum().c_str());

		cJSON_AddNumberToObject(m_pMsg, "use_car", pUserData->GetUseCar());
		cJSON_AddNumberToObject(m_pMsg, "king_sum", pUserData->GetKingSum());

		cJSON_AddStringToObject(m_pMsg, "gift_day", pUserData->GetGiftDay().c_str());

		cJSON_AddNumberToObject(m_pMsg, "gift_sum", pUserData->GetGiftSum());

		cJSON_AddStringToObject(m_pMsg, "star_day", pUserData->GetStarDay().c_str());

		cJSON_AddNumberToObject(m_pMsg, "star_sum", pUserData->GetStarSum());
		cJSON_AddNumberToObject(m_pMsg, "fight_kill_sum", pUserData->GetFightKillSum());
		cJSON_AddNumberToObject(m_pMsg, "fight_score", pUserData->GetFightScore());
		cJSON_AddNumberToObject(m_pMsg, "fight_level", pUserData->GetFightLevel());
		cJSON_AddNumberToObject(m_pMsg, "add_score", pUserData->GetAddScore());
		cJSON_AddNumberToObject(m_pMsg, "add_gold", pUserData->GetAddGold());
		cJSON_AddNumberToObject(m_pMsg, "ratio", pUserData->GetRatio());
		//cJSON_AddNumberToObject(m_pMsg, "car_level", pUserData->GetCarLevel());
		//cJSON_AddNumberToObject(m_pMsg, "status", pUserData->GetStatus());
		//cJSON_AddNumberToObject(m_pMsg, "buff", pUserData->GetBuff());
		cJSON_AddNumberToObject(m_pMsg, "enemy_id", pUserData->GetEnemyId());

        cJSON_AddNumberToObject(m_pMsg, "get_gift", pUserData->GetGetGift());
        cJSON_AddNumberToObject(m_pMsg, "keep_star", pUserData->GetKeepStar());

        //add by huyf 2018.12.27:玩家信息包增加以下字段
        cJSON_AddNumberToObject(m_pMsg, "task_mask", pUserData->GetTaskMask());					//终生任务掩码位（32位 uint）
        cJSON_AddNumberToObject(m_pMsg, "weektask_mask", pUserData->GetWeekTaskMask());			//每周任务掩码位（32位 uint）
        cJSON_AddNumberToObject(m_pMsg, "daytask_mask", pUserData->GetDayTaskMask());			//每日任务掩码位（32位 uint）
        cJSON_AddNumberToObject(m_pMsg, "cur_tm_wday", pUserData->GetCurTmWday());				//当前星期[0-6]:星期天-星期六（uint）
        cJSON_AddNumberToObject(m_pMsg, "show_item_type", pUserData->GetShowItemType());		//限时免费试用-展示的道具itemtype （uint）
        cJSON_AddNumberToObject(m_pMsg, "show_remain_ts", pUserData->GetShowRemainTs());		//限时免费试用-展示的道具的剩余时间戳(秒)（uint）
        cJSON_AddNumberToObject(m_pMsg, "online_item_type", pUserData->GetOnLineItemType());	//在线领奖索引展示的道具itemtype （uint）
        cJSON_AddNumberToObject(m_pMsg, "online_remain_ts", pUserData->GetOnLineRemainTs());	//在线领奖索引展示的道具的剩余时间戳(秒)（uint）
        cJSON_AddNumberToObject(m_pMsg, "tail", pUserData->GetTail());							//uint 当前使用的拖尾道具唯一id
        //cJSON_AddNumberToObject(m_pMsg, "online_idx", pUserData->GetOnLineIdx());				//在线领奖索引
        //add end:玩家信息包增加以下字段

        //!alter by huyf 2018.12.15:修改此处不要数据库查询太耗时
        int grandingNum = Random(500);
        // if(pUserData->GetGrandingId() > 0)
        // {
        //   string sql = "SELECT COUNT(DISTINCT user_id) FROM g_bumper_info WHERE granding_id ="
        //                + std::to_string(pUserData->GetGrandingId());
        //   MYSQL_RES* result = Database()->Query(sql);
        //   MYSQL_ROW row;
        //   if(row = mysql_fetch_row(result))
        //   {
        //       grandingNum = SafeAtoi(row[0]);
        //   }
        // }
        //!alter end:修改此处不要数据库查询太耗时
        cJSON_AddNumberToObject(m_pMsg, "granding_num", grandingNum);
	
		//packetid
		
		m_pData = cJSON_Print(m_pMsg);
		string strMsg(m_pData);
    	size_t nLen = strMsg.size();
		LOGDEBUG("[CMsgUserInfo::CreateMsg]:\n m_pData=[%s] nLen=[%d]\n", m_pData, nLen);
	}		

	return TRUE;
}

//发送json数据
BOOL CMsgUserInfo::SendMsg(CNetMsg* pMsg)
{
	m_pPlayer->SendMsgToClient(pMsg);
	return TRUE;	
}

//
UINT CMsgUserInfo::Process(VOID *pInfo)
{	
	ASSERT(!"CMsgUserInfo::Process()");
	return PACKET_EXE_CONTINUE;
}
