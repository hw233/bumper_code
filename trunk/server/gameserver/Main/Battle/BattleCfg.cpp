//////////////////////////////////////////////////////////////////////
//文件名称：CBattleCfg.cpp
//功能描述：
//版本说明：CBattleCfg API
//			
//编写作者：2018.11.29 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#include "BattleCfg.h"
#include "../../DataBase/DBMySql.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//全局变量
CBattleCfg* g_pBattleCfg = NULL;


CBattleCfg::CBattleCfg()
{
	//InitDefault();
}

CBattleCfg::~CBattleCfg()
{
}



BOOL CBattleCfg::LoadConfigInfo()
{
	if (!Database())
    { 
    	LOGERROR("[CBattleCfg::LoadConfigInfo] Database==NULL");
        return FALSE;
    }

    std::string sql = string("SELECT * FROM g_bumper_config_1")+string(" LIMIT 999");
    MYSQL_RES* result = Database()->Query(sql);
    if(result == NULL)
    {
        LOGERROR("[CBattleCfg::LoadConfigInfo] sql error: %s\n", sql.c_str());
        return FALSE;
    }

    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)))
    {    
    	m_stBattleCfgInfo.nId = SafeAtoi(row[_BC_ID]);
    	//通用配置
    	m_stBattleCfgInfo.nBcBroadcastFrameMsgCache = SafeAtoi(row[_BC_BROADCAST_FRAME_MSG_CACHE]);
    	m_stBattleCfgInfo.nBcBroadcastFrameMsgIntervalTime = (SafeAtoi(row[_BC_BROADCAST_FRAME_MSG_INTERVAL_TIME]) <= 0) ? 33 : SafeAtoi(row[_BC_BROADCAST_FRAME_MSG_INTERVAL_TIME]);
    	m_stBattleCfgInfo.nBcBroadcastCollideSynMsgCount = (SafeAtoi(row[_BC_BROADCAST_COLLIDE_SYN_MSG_COUNT]) <= 0) ? 100 : SafeAtoi(row[_BC_BROADCAST_COLLIDE_SYN_MSG_COUNT]);
    	m_stBattleCfgInfo.nBcBroadcastCollideMsgIntervalTime = m_stBattleCfgInfo.nBcBroadcastFrameMsgIntervalTime;
    	m_stBattleCfgInfo.nBcMapCollideSize = (SafeAtoi(row[_BC_MAP_COLLIDE_SIZE]) <= 0) ? 100 : SafeAtoi(row[_BC_MAP_COLLIDE_SIZE]);
    	m_stBattleCfgInfo.nBcMapDeadSize = (SafeAtoi(row[_BC_MAP_DEAD_SIZE]) <= 0) ? 100 : SafeAtoi(row[_BC_MAP_DEAD_SIZE]);
    	m_stBattleCfgInfo.nBcMaxFriendAmount = (SafeAtoi(row[_BC_MAX_FRIEND_AMOUNT]) <= 0) ? 100 : SafeAtoi(row[_BC_MAX_FRIEND_AMOUNT]);
    	//大乱斗相关
    	m_stBattleCfgInfo.nBccMaxTime = (SafeAtoi(row[_BCC_MAX_TIME]) <= 0) ? 300 : SafeAtoi(row[_BCC_MAX_TIME]);
    	m_stBattleCfgInfo.nBccRandomCreateItemIntervalSecs = (SafeAtoi(row[_BCC_RANDOM_CREATE_ITEM_INTERVAL_SECS]) <= 0) ? 1 : SafeAtoi(row[_BCC_RANDOM_CREATE_ITEM_INTERVAL_SECS]);
    	m_stBattleCfgInfo.nBccRandomCreateItemSum = (SafeAtoi(row[_BCC_RANDOM_CREATE_ITEM_SUM]) <= 0) ? 20 : SafeAtoi(row[_BCC_RANDOM_CREATE_ITEM_SUM]);
    	m_stBattleCfgInfo.nBccMaxTeamAmount = (SafeAtoi(row[_BCC_MAX_TEAMAMOUNT]) <= 0) ? 15 : SafeAtoi(row[_BCC_MAX_TEAMAMOUNT]);
    	m_stBattleCfgInfo.nBccEnouthTeamAmount = (SafeAtoi(row[_BCC_ENOUTH_TEAMAMOUNT]) <= 0) ? 10 : SafeAtoi(row[_BCC_ENOUTH_TEAMAMOUNT]);
    	m_stBattleCfgInfo.nBccMaxQuickMatchWaitTime = (SafeAtoi(row[_BCC_MAX_QUICK_MATCH_WAITTIME]) <= 0) ? 15 : SafeAtoi(row[_BCC_MAX_QUICK_MATCH_WAITTIME]);
    	m_stBattleCfgInfo.nBccItemType1 = (SafeAtoi(row[_BCC_ITEM_TYPE1]) <= 0) ? 110011 : SafeAtoi(row[_BCC_ITEM_TYPE1]);
    	m_stBattleCfgInfo.nBccItemScore1 = (SafeAtoi(row[_BCC_ITEM_SCORE1]) <= 0) ? 200 : SafeAtoi(row[_BCC_ITEM_SCORE1]);
    	m_stBattleCfgInfo.nBccItemType2 = (SafeAtoi(row[_BCC_ITEM_TYPE2]) <= 0) ? 110012 : SafeAtoi(row[_BCC_ITEM_TYPE2]);
    	m_stBattleCfgInfo.nBccItemScore2 = (SafeAtoi(row[_BCC_ITEM_SCORE2]) <= 0) ? 500 : SafeAtoi(row[_BCC_ITEM_SCORE2]);
    	m_stBattleCfgInfo.nBccItemType3 = (SafeAtoi(row[_BCC_ITEM_TYPE3]) <= 0) ? 110013 : SafeAtoi(row[_BCC_ITEM_TYPE3]);
    	m_stBattleCfgInfo.nBccItemScore3 = (SafeAtoi(row[_BCC_ITEM_SCORE3]) <= 0) ? 1000 : SafeAtoi(row[_BCC_ITEM_SCORE3]);
    	m_stBattleCfgInfo.nBccPerKillAwardGold = (SafeAtoi(row[_BCC_PER_KILL_AWARD_GOLD]) <= 0) ? 10 : SafeAtoi(row[_BCC_PER_KILL_AWARD_GOLD]);
    	m_stBattleCfgInfo.nBccBaseFightScore = (SafeAtoi(row[_BCC_BASE_FIGHT_SCORE]) <= 0) ? 1000 : SafeAtoi(row[_BCC_BASE_FIGHT_SCORE]);
    	m_stBattleCfgInfo.nBccReliveRegionRandom = (SafeAtoi(row[_BCC_REILVE_REGION_RANGDOM]) <= 0) ? 15 : SafeAtoi(row[_BCC_REILVE_REGION_RANGDOM]);
    	m_stBattleCfgInfo.nBccDirectionXRandom = (SafeAtoi(row[_BCC_DIRECTION_X_RANGDOM]) != 10000) ? 10000 : SafeAtoi(row[_BCC_DIRECTION_X_RANGDOM]);
    	m_stBattleCfgInfo.nBccDirectionYRandom = (SafeAtoi(row[_BCC_DIRECTION_Y_RANGDOM]) != 10000) ? 10000 : SafeAtoi(row[_BCC_DIRECTION_Y_RANGDOM]);
    	m_stBattleCfgInfo.nBccItemTypeAmount = (SafeAtoi(row[_BCC_ITEMTYPE_AMOUNT]) <= 0) ? 3 : SafeAtoi(row[_BCC_ITEMTYPE_AMOUNT]);
        m_stBattleCfgInfo.nBccAwardBaseRatio = (SafeAtoi(row[_BCC_AWARD_BASE_RATIO]) <= 0) ? 1 : SafeAtoi(row[_BCC_AWARD_BASE_RATIO]);
        m_stBattleCfgInfo.nBccAwardStartViewRatio = (SafeAtoi(row[_BCC_AWARD_START_VIEW_RATIO]) <= 0) ? 1 : SafeAtoi(row[_BCC_AWARD_START_VIEW_RATIO]);
        m_stBattleCfgInfo.nBccAwardEndViewRatio = (SafeAtoi(row[_BCC_AWARD_END_VIEW_RATIO]) <= 0) ? 3 : SafeAtoi(row[_BCC_AWARD_END_VIEW_RATIO]);
        m_stBattleCfgInfo.nBccAwardKingAmount = (SafeAtoi(row[_BCC_AWARD_KING_AMOUNT]) <= 0) ? 1 : SafeAtoi(row[_BCC_AWARD_KING_AMOUNT]);
        m_stBattleCfgInfo.nBccAwardKingBaseRatio = (SafeAtoi(row[_BCC_AWARD_KING_BASE_RATIO]) <= 0) ? 1 : SafeAtoi(row[_BCC_AWARD_KING_BASE_RATIO]);
        m_stBattleCfgInfo.nBccAwardKingViewRatio = (SafeAtoi(row[_BCC_AWARD_KING_VIEW_RATIO]) <= 0) ? 3 : SafeAtoi(row[_BCC_AWARD_KING_VIEW_RATIO]);
    	//红蓝对战相关
    	m_stBattleCfgInfo.nBcftMaxFightTeamFinishScore = (SafeAtoi(row[_BCFT_MAX_FIGHTTEAM_FINISH_SCORE]) <= 0) ? 30 : SafeAtoi(row[_BCFT_MAX_FIGHTTEAM_FINISH_SCORE]);
    	m_stBattleCfgInfo.nBcftMaxFightTeamCostTime = (SafeAtoi(row[_BCFT_MAX_FIGHTTEAM_COST_TIME]) <= 0) ? 300 : SafeAtoi(row[_BCFT_MAX_FIGHTTEAM_COST_TIME]);
    	m_stBattleCfgInfo.nBcftMaxRBTeamAmount = (SafeAtoi(row[_BCFT_MAX_RB_TEAMAMOUNT]) <= 0) ? 5 : SafeAtoi(row[_BCFT_MAX_RB_TEAMAMOUNT]);
    	m_stBattleCfgInfo.nBcftDefaultCarLevel = (SafeAtoi(row[_BCFT_DEFAULT_CAR_LEVEL]) <= 0) ? 1 : SafeAtoi(row[_BCFT_DEFAULT_CAR_LEVEL]);
    	m_stBattleCfgInfo.nBcftMaxCarLevel = (SafeAtoi(row[_BCFT_MAX_CAR_LEVEL]) <= 0) ? 10 : SafeAtoi(row[_BCFT_MAX_CAR_LEVEL]);
    	m_stBattleCfgInfo.nBcftKillAwardRatio = (SafeAtoi(row[_BCFT_KILL_AWARD_RATIO_]) <= 0) ? 1 : SafeAtoi(row[_BCFT_KILL_AWARD_RATIO_]);
    	m_stBattleCfgInfo.nBcftReliveRegionRandom = (SafeAtoi(row[_BCFT_REILVE_REGION_RANGDOM]) != 5) ? 5 : SafeAtoi(row[_BCFT_REILVE_REGION_RANGDOM]);
    	m_stBattleCfgInfo.nBcftWinAwardGold = (SafeAtoi(row[_BCFT_WIN_AWARD_GOLD]) <= 0) ? 150 : SafeAtoi(row[_BCFT_WIN_AWARD_GOLD]);
    	m_stBattleCfgInfo.nBcftDrawAwardGold = (SafeAtoi(row[_BCFT_DRAW_AWARD_GOLD]) <= 0) ? 100 : SafeAtoi(row[_BCFT_DRAW_AWARD_GOLD]);
    	m_stBattleCfgInfo.nBcftDefaultAwardGold = (SafeAtoi(row[_BCFT_DEFEAT_AWARD_GOLD]) <= 0) ? 50 : SafeAtoi(row[_BCFT_DEFEAT_AWARD_GOLD]);
        m_stBattleCfgInfo.nBcftAwardBaseRatio = (SafeAtoi(row[_BCFT_AWARD_BASE_RATIO]) <= 0) ? 1 : SafeAtoi(row[_BCFT_AWARD_BASE_RATIO]);
        m_stBattleCfgInfo.nBcftAwardViewRatio = (SafeAtoi(row[_BCFT_AWARD_VIEW_RATIO]) <= 0) ? 3 : SafeAtoi(row[_BCFT_AWARD_VIEW_RATIO]);

        //大乱斗相关
        m_stBattleCfgInfo.nBccKillAwardScoreRatio = (SafeAtoi(row[_BCC_KILL_AWARD_SCORE_RATIO]) <= 0) ? 5 : SafeAtoi(row[_BCC_KILL_AWARD_SCORE_RATIO]);
        m_stBattleCfgInfo.nBccRevengeAwardKillScoreRatio = (SafeAtoi(row[_BCC_REVENGE_AWARD_KILL_SCORE_RATIO]) <= 0) ? 7 : SafeAtoi(row[_BCC_REVENGE_AWARD_KILL_SCORE_RATIO]);
        m_stBattleCfgInfo.nBccNormalReliveRemainScoreRatio = (SafeAtoi(row[_BCC_NORMAL_RELIVE_REMAIN_SCORE_RATIO]) <= 0) ? 10 : SafeAtoi(row[_BCC_NORMAL_RELIVE_REMAIN_SCORE_RATIO]);
        m_stBattleCfgInfo.nBccViewReliveRemainScoreRatio = (SafeAtoi(row[_BCC_VIEW_RELIVE_REMAIN_SCORE_RATIO]) <= 0) ? 5 : SafeAtoi(row[_BCC_VIEW_RELIVE_REMAIN_SCORE_RATIO]);

        m_stBattleCfgInfo.nBcMegerOtherTeamS = (SafeAtoi(row[_BC_MEGER_OTHER_TEAMS]) <= 0) ? 0 : SafeAtoi(row[_BC_MEGER_OTHER_TEAMS]);
	
        LOGDEBUG("[CBattleCfg::LoadConfigInfo] nId=[%d] nBcBroadcastFrameMsgCache=[%d] nBcBroadcastFrameMsgIntervalTime=[%d] nBcBroadcastCollideSynMsgCount=[%d] nBcBroadcastCollideMsgIntervalTime=[%d] \
        	nBcMapCollideSize=[%d] nBcMapDeadSize=[%d] nBcMaxFriendAmount=[%d] nBccMaxTime=[%d] nBccRandomCreateItemIntervalSecs=[%d] nBccRandomCreateItemSum=[%d] nBccMaxTeamAmount=[%d]\
        	nBccEnouthTeamAmount=[%d] nBccMaxQuickMatchWaitTime=[%d] nBccItemType1=[%d] nBccItemScore1=[%d] nBccItemType2=[%d] nBccItemScore2=[%d] nBccItemType3=[%d]\
        	nBccItemScore3=[%d] nBccPerKillAwardGold=[%d] nBccBaseFightScore=[%d] nBccReliveRegionRandom=[%d] nBccDirectionXRandom=[%d] nBccDirectionYRandom=[%d] nBccItemTypeAmount=[%d]\
            nBccAwardBaseRatio=[%d] nBccAwardStartViewRatio=[%d] nBccAwardEndViewRatio=[%d] nBccAwardKingAmount=[%d] nBccAwardKingBaseRatio=[%d] nBccAwardKingViewRatio=[%d]\
        	nBcftMaxFightTeamFinishScore=[%d] nBcftMaxFightTeamCostTime=[%d] nBcftMaxRBTeamAmount=[%d] nBcftDefaultCarLevel=[%d] nBcftMaxCarLevel=[%d] nBcftKillAwardRatio=[%d] nBcftReliveRegionRandom=[%d]\
        	nBcftWinAwardGold=[%d] nBcftDrawAwardGold=[%d] nBcftDefaultAwardGold=[%d] nBcftAwardBaseRatio=[%d] nBcftAwardViewRatio=[%d] nBccKillAwardScoreRatio=[%d] nBccRevengeAwardKillScoreRatio=[%d]\
            nBccNormalReliveRemainScoreRatio=[%d] nBccViewReliveRemainScoreRatio=[%d] nBcMegerOtherTeamS=[%d]", 
        	m_stBattleCfgInfo.nId,m_stBattleCfgInfo.nBcBroadcastFrameMsgCache,m_stBattleCfgInfo.nBcBroadcastFrameMsgIntervalTime,m_stBattleCfgInfo.nBcBroadcastCollideSynMsgCount,m_stBattleCfgInfo.nBcBroadcastCollideMsgIntervalTime,
        	m_stBattleCfgInfo.nBcMapCollideSize,m_stBattleCfgInfo.nBcMapDeadSize,m_stBattleCfgInfo.nBcMaxFriendAmount,m_stBattleCfgInfo.nBccMaxTime,m_stBattleCfgInfo.nBccRandomCreateItemIntervalSecs,
        	m_stBattleCfgInfo.nBccRandomCreateItemSum,m_stBattleCfgInfo.nBccMaxTeamAmount,m_stBattleCfgInfo.nBccEnouthTeamAmount,m_stBattleCfgInfo.nBccMaxQuickMatchWaitTime,m_stBattleCfgInfo.nBccItemType1,
        	m_stBattleCfgInfo.nBccItemScore1,m_stBattleCfgInfo.nBccItemType2,m_stBattleCfgInfo.nBccItemScore2,m_stBattleCfgInfo.nBccItemType3,m_stBattleCfgInfo.nBccItemScore3,m_stBattleCfgInfo.nBccPerKillAwardGold,
        	m_stBattleCfgInfo.nBccBaseFightScore,m_stBattleCfgInfo.nBccReliveRegionRandom,m_stBattleCfgInfo.nBccDirectionXRandom,m_stBattleCfgInfo.nBccDirectionYRandom,m_stBattleCfgInfo.nBccItemTypeAmount,
            m_stBattleCfgInfo.nBccAwardBaseRatio,m_stBattleCfgInfo.nBccAwardStartViewRatio,m_stBattleCfgInfo.nBccAwardEndViewRatio,m_stBattleCfgInfo.nBccAwardKingAmount,m_stBattleCfgInfo.nBccAwardKingBaseRatio,m_stBattleCfgInfo.nBccAwardKingViewRatio,
        	m_stBattleCfgInfo.nBcftMaxFightTeamFinishScore,m_stBattleCfgInfo.nBcftMaxFightTeamCostTime,m_stBattleCfgInfo.nBcftMaxRBTeamAmount,m_stBattleCfgInfo.nBcftDefaultCarLevel,m_stBattleCfgInfo.nBcftMaxCarLevel,
        	m_stBattleCfgInfo.nBcftKillAwardRatio,m_stBattleCfgInfo.nBcftReliveRegionRandom,m_stBattleCfgInfo.nBcftWinAwardGold,m_stBattleCfgInfo.nBcftDrawAwardGold,m_stBattleCfgInfo.nBcftDefaultAwardGold,
            m_stBattleCfgInfo.nBcftAwardBaseRatio,m_stBattleCfgInfo.nBcftAwardViewRatio,m_stBattleCfgInfo.nBccKillAwardScoreRatio,m_stBattleCfgInfo.nBccRevengeAwardKillScoreRatio,m_stBattleCfgInfo.nBccNormalReliveRemainScoreRatio,
            m_stBattleCfgInfo.nBccViewReliveRemainScoreRatio,m_stBattleCfgInfo.nBcMegerOtherTeamS);
    }
    mysql_free_result(result);
    result = NULL;
    LOGDEBUG("[CBattleCfg::LoadConfigInfo] Loading table g_battle_config end");
    return TRUE;
}

BOOL CBattleCfg::SaveConfigInfo()	
{
    return TRUE;
}

BOOL CBattleCfg::SetBcBroadcastFrameMsgCache(UINT nBcBFMsgCache, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBcBroadcastFrameMsgCache = nBcBFMsgCache;
    return TRUE;
}

BOOL CBattleCfg::SetBcBroadcastFrameMsgIntervalTime(UINT nBcBFMsgIntervalTime, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBcBroadcastFrameMsgIntervalTime = nBcBFMsgIntervalTime;
    return TRUE;
}
BOOL CBattleCfg::SetBcBroadcastCollideSynMsgCount(UINT nBcBCSMsgCount, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBcBroadcastCollideSynMsgCount = nBcBCSMsgCount;
    return TRUE;
}
BOOL CBattleCfg::SetBcBroadcastCollideMsgIntervalTime(UINT nBcBCMsgIntervalTime, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBcBroadcastCollideMsgIntervalTime = nBcBCMsgIntervalTime;
    return TRUE;
}
BOOL CBattleCfg::SetBcMapCollideSize(UINT nBcMapCollideSize, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBcMapCollideSize = nBcMapCollideSize;
    return TRUE;
}
BOOL CBattleCfg::SetBcMapDeadSize(UINT nBcMapDeadSize, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBcMapDeadSize = nBcMapDeadSize;
    return TRUE;
}
BOOL CBattleCfg::SetBcMaxFriendAmount(UINT nBcMaxFriendAmount, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBcMaxFriendAmount = nBcMaxFriendAmount;
    return TRUE;
}

//大乱斗相关
BOOL CBattleCfg::SetBccMaxTime(UINT nBccMaxTime, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBccMaxTime = nBccMaxTime;
    return TRUE;
}
BOOL CBattleCfg::SetBccRandomCreateItemIntervalSecs(UINT nBccRCIIntervalSecs, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBccRandomCreateItemIntervalSecs = nBccRCIIntervalSecs;
    return TRUE;
}
BOOL CBattleCfg::SetnBccRandomCreateItemSum(UINT nBccRCISum, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBccRandomCreateItemSum = nBccRCISum;
    return TRUE;
}
BOOL CBattleCfg::SetBccMaxTeamAmount(UINT nBccMaxTeamAmount, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBccMaxTeamAmount = nBccMaxTeamAmount;
    return TRUE;
}
BOOL CBattleCfg::SetBccEnouthTeamAmount(UINT nBccEnouthTeamAmount, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBccEnouthTeamAmount = nBccEnouthTeamAmount;
    return TRUE;
}
BOOL CBattleCfg::SetBccMaxQuickMatchWaitTime(UINT nBccMaxQuickMatchWaitTime, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBccMaxQuickMatchWaitTime = nBccMaxQuickMatchWaitTime;
    return TRUE;
}
BOOL CBattleCfg::SetBccItemType1(UINT nBccItemType1, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBccItemType1 = nBccItemType1;
    return TRUE;
}
BOOL CBattleCfg::SetBccItemScore1(UINT nBccItemScore1, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBccItemScore1 = nBccItemScore1;
    return TRUE;
}
BOOL CBattleCfg::SetBccItemType2(UINT nBccItemType2, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBccItemType2 = nBccItemType2;
    return TRUE;
}
BOOL CBattleCfg::SetBccItemScore2(UINT nBccItemScore2, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBccItemScore2 = nBccItemScore2;
    return TRUE;
}
BOOL CBattleCfg::SetBccItemType3(UINT nBccItemType3, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBccItemType3 = nBccItemType3;
    return TRUE;
}
BOOL CBattleCfg::SetBccItemScore3(UINT nBccItemScore3, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBccItemScore3 = nBccItemScore3;
    return TRUE;
}
BOOL CBattleCfg::SetBccPerKillAwardGold(UINT nBccPerKillAwardGold, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBccPerKillAwardGold = nBccPerKillAwardGold;
    return TRUE;
}
BOOL CBattleCfg::SetBccBaseFightScore(UINT nBccBaseFightScore, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBccBaseFightScore = nBccBaseFightScore;
    return TRUE;
}
BOOL CBattleCfg::SetBccReliveRegionRandom(UINT nBccReliveRegionRandom, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBccReliveRegionRandom = nBccReliveRegionRandom;
    return TRUE;
}
BOOL CBattleCfg::SetBccDirectionXRandom(UINT nBccDirectionXRandom, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBccDirectionXRandom = nBccDirectionXRandom;
    return TRUE;
}
BOOL CBattleCfg::SetBccDirectionYRandom(UINT nBccDirectionYRandom, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBccDirectionYRandom = nBccDirectionYRandom;
    return TRUE;
}
BOOL CBattleCfg::SetBccItemTypeAmount(UINT nBccItemTypeAmount, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBccItemTypeAmount = nBccItemTypeAmount;
    return TRUE;
}
BOOL CBattleCfg::SetBccAwardBaseRatio(UINT nBccAwardBaseRatio, BOOL bUpdate /*= TRUE*/)
{
    m_stBattleCfgInfo.nBccAwardBaseRatio = nBccAwardBaseRatio;
    return TRUE;
}
BOOL CBattleCfg::SetBccAwardStartViewRatio(UINT nBccAwardStartViewRatio, BOOL bUpdate /*= TRUE*/)
{
    m_stBattleCfgInfo.nBccAwardStartViewRatio = nBccAwardStartViewRatio;
    return TRUE;
}
BOOL CBattleCfg::SetBccAwardEndViewRatio(UINT nBccAwardEndViewRatio, BOOL bUpdate /*= TRUE*/)
{
    m_stBattleCfgInfo.nBccAwardEndViewRatio = nBccAwardEndViewRatio;
    return TRUE;
}
BOOL CBattleCfg::SetBccAwardKingAmount(UINT nBccAwardKingAmount, BOOL bUpdate /*= TRUE*/)
{
    m_stBattleCfgInfo.nBccAwardKingAmount = nBccAwardKingAmount;
    return TRUE;
}
BOOL CBattleCfg::SetBccAwardKingBaseRatio(UINT nBccAwardKingBaseRatio, BOOL bUpdate /*= TRUE*/)
{
    m_stBattleCfgInfo.nBccAwardKingBaseRatio = nBccAwardKingBaseRatio;
    return TRUE;
}
BOOL CBattleCfg::SetBccAwardKingViewRatio(UINT nBccAwardKingViewRatio, BOOL bUpdate /*= TRUE*/)
{
    m_stBattleCfgInfo.nBccAwardKingViewRatio = nBccAwardKingViewRatio;
    return TRUE;
}

//红蓝对战相关
BOOL CBattleCfg::SetBcftMaxFightTeamFinishScore(UINT nBcftMFTFinishScore, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBcftMaxFightTeamFinishScore = nBcftMFTFinishScore;
    return TRUE;
}
BOOL CBattleCfg::SetBcftMaxFightTeamCostTime(UINT nBcftMFTCostTime, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBcftMaxFightTeamCostTime = nBcftMFTCostTime;
    return TRUE;
}
BOOL CBattleCfg::SetBcftMaxRBTeamAmount(UINT nBcftMaxRBTeamAmount, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBcftMaxRBTeamAmount = nBcftMaxRBTeamAmount;
    return TRUE;
}
BOOL CBattleCfg::SetBcftDefaultCarLevel(UINT nBcftDefaultCarLevel, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBcftDefaultCarLevel = nBcftDefaultCarLevel;
    return TRUE;
}	
BOOL CBattleCfg::SetBcftMaxCarLevel(UINT nBcftMaxCarLevel, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBcftMaxCarLevel = nBcftMaxCarLevel;
    return TRUE;
}
BOOL CBattleCfg::SetBcftKillAwardRatio(UINT nBcftKillAwardRatio, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBcftKillAwardRatio = nBcftKillAwardRatio;
    return TRUE;
}
BOOL CBattleCfg::SetBcftReliveRegionRandom(UINT nBcftReliveRegionRandom, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBcftReliveRegionRandom = nBcftReliveRegionRandom;
    return TRUE;
}
BOOL CBattleCfg::SetBcftWinAwardGold(UINT nBcftWinAwardGold, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBcftWinAwardGold = nBcftWinAwardGold;
    return TRUE;
}
BOOL CBattleCfg::SetBcftDrawAwardGold(UINT nBcftDrawAwardGold, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBcftDrawAwardGold = nBcftDrawAwardGold;
    return TRUE;
}
BOOL CBattleCfg::SetBcftDefaultAwardGold(UINT nBcftDefaultAwardGold, BOOL bUpdate /*= TRUE*/)
{
	m_stBattleCfgInfo.nBcftDefaultAwardGold = nBcftDefaultAwardGold;
    return TRUE;
}

BOOL CBattleCfg::SetBcftAwardBaseRatio(UINT nBcftAwardBaseRatio, BOOL bUpdate /*= TRUE*/)
{
    m_stBattleCfgInfo.nBcftAwardBaseRatio = nBcftAwardBaseRatio;
    return TRUE;
}
BOOL CBattleCfg::SetBcftAwardViewRatio(UINT nBcftAwardViewRatio, BOOL bUpdate /*= TRUE*/)
{
    m_stBattleCfgInfo.nBcftAwardViewRatio = nBcftAwardViewRatio;
    return TRUE;
}

//大乱斗相关 
BOOL CBattleCfg::SetBccKillAwardScoreRatio(UINT nBccKillAwardScoreRatio, BOOL bUpdate/* = TRUE*/)
{
    m_stBattleCfgInfo.nBccKillAwardScoreRatio = nBccKillAwardScoreRatio;
    return TRUE;
}
BOOL CBattleCfg::SetBccRevengeAwardKillScoreRatio(UINT nBccRevengeAwardKillScoreRatio, BOOL bUpdate/* = TRUE*/)
{
    m_stBattleCfgInfo.nBccRevengeAwardKillScoreRatio = nBccRevengeAwardKillScoreRatio;
    return TRUE;
}
BOOL CBattleCfg::SetBccNormalReliveRemainScoreRatio(UINT nBccNormalReliveRemainScoreRatio, BOOL bUpdate/* = TRUE*/)
{
    m_stBattleCfgInfo.nBccNormalReliveRemainScoreRatio = nBccNormalReliveRemainScoreRatio;
    return TRUE;
}
BOOL CBattleCfg::SetBccViewReliveRemainScoreRatio(UINT nBccViewReliveRemainScoreRatio, BOOL bUpdate/* = TRUE*/)
{
    m_stBattleCfgInfo.nBccViewReliveRemainScoreRatio = nBccViewReliveRemainScoreRatio;
    return TRUE;
}
BOOL CBattleCfg::SetBcMegerOtherTeamS(UINT nBcMegerOtherTeamS, BOOL bUpdate/* = TRUE*/)
{
    m_stBattleCfgInfo.nBcMegerOtherTeamS = nBcMegerOtherTeamS;
    return TRUE;
}