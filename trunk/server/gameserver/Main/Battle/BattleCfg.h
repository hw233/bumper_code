//////////////////////////////////////////////////////////////////////
//文件名称：CBattleCfg.h
//功能描述：
//版本说明：CBattleCfg API
//			
//编写作者：2018.11.29 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#ifndef __BATTLE_CFG_H__
#define __BATTLE_CFG_H__
#include "../../Common/Type.h"
#include "../../Common/BaseType.h"
#include "../../Common/Config.h"
#include "../../Common/Common.h"

//////////////////////////////////////////////////////////////////////
///
struct stBattleCfg
{
	UINT nId;															// 自增量ID
	//通用配置
	UINT nBcBroadcastFrameMsgCache; 									// 广播同步帧缓存66ms 0:不缓存 1:缓存 默认 : 1
	UINT nBcBroadcastFrameMsgIntervalTime;				 				// 广播网络同步帧间隔ms 默认 : 66
	UINT nBcBroadcastCollideSynMsgCount;								// 碰撞后服务器主动广播碰撞行为的消息包数 默认 : 5
	UINT nBcBroadcastCollideMsgIntervalTime;			 				// 广播网络碰撞同步帧间隔ms 默认 : 66
	UINT nBcMapCollideSize;												// 碰撞缓存队列大小 默认 : 100
	UINT nBcMapDeadSize;												// 死亡缓存队列大小 默认 : 100
	UINT nBcMaxFriendAmount;											// 好友数量 默认 : 100

	//大乱斗相关
	UINT nBccMaxTime;													// 战斗时间300秒
	UINT nBccRandomCreateItemIntervalSecs;								// 随机生成道具时间间隔
	UINT nBccRandomCreateItemSum;										// 随机生成道具总数量
	UINT nBccMaxTeamAmount;												// 队伍人数上限
	UINT nBccEnouthTeamAmount;											// 满足开战队伍人数（等待时间已经耗尽）
	UINT nBccMaxQuickMatchWaitTime;										// QUICK_MATCH状态下等待时间长度
	UINT nBccItemType1;													// 随机刷新道具type1
	UINT nBccItemScore1;												// 道具积分1
	UINT nBccItemType2;													// 随机刷新道具type2
	UINT nBccItemScore2;												// 道具积分2
	UINT nBccItemType3;													// 随机刷新道具type3
	UINT nBccItemScore3;												// 道具积分3
	UINT nBccPerKillAwardGold;											// 击杀奖励的金币数
	UINT nBccBaseFightScore;											// 基础积分
	UINT nBccReliveRegionRandom;										// 复活随机区域数量
	UINT nBccDirectionXRandom;											// 复活时候X方向随机，扩大10000倍率 (0~1)
	UINT nBccDirectionYRandom;											// 复活时候X方向随机，扩大10000倍率 (0~1)
	UINT nBccItemTypeAmount;											// 生成随机道具的种类
	UINT nBccAwardBaseRatio;											// 奖励金币的基础倍率 默认 1
	UINT nBccAwardStartViewRatio;										// 奖励金币的观看视频后倍率(战前) 默认 1
	UINT nBccAwardEndViewRatio;											// 奖励金币的观看视频后倍率（战后）默认 3
	UINT nBccAwardKingAmount;											// 奖励霸主令牌的数量 默认 1
	UINT nBccAwardKingBaseRatio;										// 奖励霸主令牌的基础倍率 默认 1
	UINT nBccAwardKingViewRatio;										// 奖励霸主令牌观看视频后的倍率 默认 3

	//红蓝对战相关
	UINT nBcftMaxFightTeamFinishScore;									// 红蓝战队活动胜利点总积分	默认 : 30
	UINT nBcftMaxFightTeamCostTime;										// 红蓝战队活动每场战斗的总时间	默认 : 300
	UINT nBcftMaxRBTeamAmount;											// 红蓝战队队伍人数上限 默认 : 5
	UINT nBcftDefaultCarLevel;											// 击杀车辆默认最低等级 默认 : 1
	UINT nBcftMaxCarLevel;												// 击杀车辆最高等级 默认 : 10
	UINT nBcftKillAwardRatio;											// 击杀车辆奖励的倍率 默认 : 1
	UINT nBcftReliveRegionRandom;										// 复活随机区域数量（0-4）~（5-9）
	UINT nBcftWinAwardGold;												// 胜利奖励的金币 默认 : 150
	UINT nBcftDrawAwardGold;											// 平局奖励的金币 默认 : 100
	UINT nBcftDefaultAwardGold;											// 失败奖励的金币 默认 : 50
	UINT nBcftAwardBaseRatio;											// 奖励金币的基础倍率 默认 1
	UINT nBcftAwardViewRatio;											// 奖励金币的观看视频后倍率 默认 3

	//大乱斗相关
	UINT nBccKillAwardScoreRatio;										// 击杀奖励的积分赔率
	UINT nBccRevengeAwardKillScoreRatio;								// 复仇奖励的积分倍率
	UINT nBccNormalReliveRemainScoreRatio;								// 普通复活后保留的剩余积分比例
	UINT nBccViewReliveRemainScoreRatio;								// 观看视频后复活保留的剩余分数比例

	//通用配置
	UINT nBcMegerOtherTeamS;		 					 				// 队伍整合 默认 : 0(整合) 1 : 整合
};
enum _BATTLE_CFG
{
	_BC_ID 											= 0,				// 自增量ID
	
	//通用配置
	_BC_BROADCAST_FRAME_MSG_CACHE 					= 1, 				// 广播同步帧缓存66ms 0:不缓存 1:缓存 默认 : 1
	_BC_BROADCAST_FRAME_MSG_INTERVAL_TIME 			= 2, 				// 广播网络同步帧间隔ms 默认 : 66
	_BC_BROADCAST_COLLIDE_SYN_MSG_COUNT 			= 3,  				// 碰撞后服务器主动广播碰撞行为的消息包数 默认 : 5
	_BC_BROADCAST_COLLIDE_MSG_INTERVAL_TIME 		= 4, 				// 广播网络碰撞同步帧间隔ms 默认 : 66
	_BC_MAP_COLLIDE_SIZE 							= 5,				// 碰撞缓存队列大小 默认 : 100
	_BC_MAP_DEAD_SIZE 								= 6,				// 死亡缓存队列大小 默认 : 100
	_BC_MAX_FRIEND_AMOUNT 							= 7, 				// 好友数量 默认 : 100

	//大乱斗相关	
	_BCC_MAX_TIME 									= 8,				// 战斗时间300秒 默认 : 300
	_BCC_RANDOM_CREATE_ITEM_INTERVAL_SECS 			= 9,				// 3秒随机生成道具 默认 : 3
	_BCC_RANDOM_CREATE_ITEM_SUM 					= 10,				// 3秒随机生成道具数量 默认 : 暂时无用
	_BCC_MAX_TEAMAMOUNT								= 11,				// 队伍人数上限 默认 : 15
	_BCC_ENOUTH_TEAMAMOUNT 							= 12,				// 满足开战队伍人数（等待时间已经耗尽） 默认 : 10
	_BCC_MAX_QUICK_MATCH_WAITTIME 					= 13, 				// QUICK_MATCH状态下等待时间长度 默认 : 10	 
	_BCC_ITEM_TYPE1					 				= 14,				// 道具type1 
	_BCC_ITEM_SCORE1				 				= 15,				// 道具积分1
	_BCC_ITEM_TYPE2						 			= 16,				// 道具type2
	_BCC_ITEM_SCORE2					 			= 17,				// 道具积分2
	_BCC_ITEM_TYPE3						 			= 18,				// 道具type3
	_BCC_ITEM_SCORE3					 			= 19,				// 道具积分3
	_BCC_PER_KILL_AWARD_GOLD 						= 20,				// 击杀奖励的金币数 默认 : 2
	_BCC_BASE_FIGHT_SCORE 							= 21,				// 基础积分 默认 : 1000
	_BCC_REILVE_REGION_RANGDOM 						= 22,				// 复活随机区域数量 默认 : 30
	_BCC_DIRECTION_X_RANGDOM 						= 23,				// 复活时候X方向随机，扩大10000倍率 (0~1)
	_BCC_DIRECTION_Y_RANGDOM 						= 24,				// 复活时候X方向随机，扩大10000倍率 (0~1)
	_BCC_ITEMTYPE_AMOUNT							= 25,				// 生成随机道具的种类 暂时无用
	_BCC_AWARD_BASE_RATIO							= 26,				// 奖励金币的基础倍率
	_BCC_AWARD_START_VIEW_RATIO						= 27,				// 奖励金币的观看视频后倍率（战前）
	_BCC_AWARD_END_VIEW_RATIO						= 28,				// 奖励金币的观看视频后倍率（战后）
	_BCC_AWARD_KING_AMOUNT							= 29,				// 奖励霸主令牌的数量 默认 1
	_BCC_AWARD_KING_BASE_RATIO						= 30,				// 奖励霸主令牌的基础倍率
	_BCC_AWARD_KING_VIEW_RATIO						= 31,				// 奖励霸主令牌观看视频后的倍率							

	//红蓝对战相关
	_BCFT_MAX_FIGHTTEAM_FINISH_SCORE 				= 32, 				// 红蓝战队活动胜利点总积分	默认 : 30
	_BCFT_MAX_FIGHTTEAM_COST_TIME 					= 33, 				// 红蓝战队活动每场战斗的总时间	默认 : 300
	_BCFT_MAX_RB_TEAMAMOUNT 						= 34, 				// 红蓝战队队伍人数上限 默认 : 5
	_BCFT_DEFAULT_CAR_LEVEL 						= 35,				// 击杀车辆默认最低等级 默认 : 1
	_BCFT_MAX_CAR_LEVEL 							= 36,				// 击杀车辆最高等级 默认 : 10
	_BCFT_KILL_AWARD_RATIO_ 						= 37,				// 击杀车辆奖励的倍率 默认 : 1
	_BCFT_REILVE_REGION_RANGDOM 					= 38,				// 复活随机区域数量（0-4）~（5-9）
	_BCFT_WIN_AWARD_GOLD 							= 39,				// 胜利奖励的金币 默认 : 150	 
	_BCFT_DRAW_AWARD_GOLD 							= 40,				// 平局奖励的金币 默认 : 100
	_BCFT_DEFEAT_AWARD_GOLD 						= 41,				// 失败奖励的金币 默认 : 50
	_BCFT_AWARD_BASE_RATIO 							= 42,				// 奖励金币的基础倍率 默认 : 1
	_BCFT_AWARD_VIEW_RATIO 							= 43,				// 奖励金币的观看视频后倍率 默认 : 3

	//大乱斗相关
	_BCC_KILL_AWARD_SCORE_RATIO						= 44,			     // 击杀奖励的积分赔率 默认 : 5
	_BCC_REVENGE_AWARD_KILL_SCORE_RATIO				= 45, 				 // 复仇奖励的积分倍率 默认 : 7
	_BCC_NORMAL_RELIVE_REMAIN_SCORE_RATIO			= 46,				 // 普通复活后保留的剩余积分比例 默认 : 10
	_BCC_VIEW_RELIVE_REMAIN_SCORE_RATIO				= 47,				 // 观看视频后复活保留的剩余分数比例 默认 : 5

	//通用配置
	_BC_MEGER_OTHER_TEAMS		 					= 48, 				 // 队伍整合 默认 : 0(不整合) 1 : 整合
	
};
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
class CBattleCfg
{
public:
	CBattleCfg();
	virtual ~CBattleCfg();

public:
	BOOL 	LoadConfigInfo();
	//BOOL    InitDefault();
	BOOL	SaveConfigInfo();	

public:

	UINT	GetId()																					{	return m_stBattleCfgInfo.nId;			}
	BOOL	SetId(UINT nId)																			{	return TRUE;			}
	
	UINT	GetBcBroadcastFrameMsgCache()															{	return m_stBattleCfgInfo.nBcBroadcastFrameMsgCache;			}
	BOOL 	SetBcBroadcastFrameMsgCache(UINT nBcBFMsgCache, BOOL bUpdate = TRUE);

	UINT	GetBcBroadcastFrameMsgIntervalTime()													{	return m_stBattleCfgInfo.nBcBroadcastFrameMsgIntervalTime;	}
	BOOL	SetBcBroadcastFrameMsgIntervalTime(UINT nBcBFMsgIntervalTime, BOOL bUpdate = TRUE);

	UINT	GetBcBroadcastCollideSynMsgCount()														{	return m_stBattleCfgInfo.nBcBroadcastCollideSynMsgCount;	}
	BOOL	SetBcBroadcastCollideSynMsgCount(UINT nBcBCSMsgCount, BOOL bUpdate = TRUE);

	UINT	GetBcBroadcastCollideMsgIntervalTime()													{	return m_stBattleCfgInfo.nBcBroadcastCollideMsgIntervalTime;	}
	BOOL	SetBcBroadcastCollideMsgIntervalTime(UINT nBcBCMsgIntervalTime, BOOL bUpdate = TRUE);

	UINT	GetBcMapCollideSize()																	{ 	return m_stBattleCfgInfo.nBcMapCollideSize; 	}
	BOOL	SetBcMapCollideSize(UINT nBcMapCollideSize, BOOL bUpdate = TRUE);

	UINT	GetBcMapDeadSize()																		{ 	return m_stBattleCfgInfo.nBcMapDeadSize; 	}
	BOOL	SetBcMapDeadSize(UINT nBcMapDeadSize, BOOL bUpdate = TRUE);

	UINT	GetBcMaxFriendAmount()																	{ 	return m_stBattleCfgInfo.nBcMaxFriendAmount; 	}
	BOOL	SetBcMaxFriendAmount(UINT nBcMaxFriendAmount, BOOL bUpdate = TRUE);

	//大乱斗相关
	UINT	GetBccMaxTime()																			{ 	return m_stBattleCfgInfo.nBccMaxTime; 	}
	BOOL	SetBccMaxTime(UINT nBccMaxTime, BOOL bUpdate = TRUE);

	UINT	GetBccRandomCreateItemIntervalSecs()													{ 	return m_stBattleCfgInfo.nBccRandomCreateItemIntervalSecs; 	}
	BOOL	SetBccRandomCreateItemIntervalSecs(UINT nBccRCIIntervalSecs, BOOL bUpdate = TRUE);

	UINT	GetnBccRandomCreateItemSum()															{ 	return m_stBattleCfgInfo.nBccRandomCreateItemSum; 	}
	BOOL	SetnBccRandomCreateItemSum(UINT nBccRCISum, BOOL bUpdate = TRUE);

	UINT	GetBccMaxTeamAmount()																	{ 	return m_stBattleCfgInfo.nBccMaxTeamAmount; 	}
	BOOL	SetBccMaxTeamAmount(UINT nBccMaxTeamAmount, BOOL bUpdate = TRUE);

	UINT	GetBccEnouthTeamAmount()																{ 	return m_stBattleCfgInfo.nBccEnouthTeamAmount; 	}
	BOOL	SetBccEnouthTeamAmount(UINT nBccEnouthTeamAmount, BOOL bUpdate = TRUE);

	UINT	GetBccMaxQuickMatchWaitTime()															{ 	return m_stBattleCfgInfo.nBccMaxQuickMatchWaitTime; 	}
	BOOL	SetBccMaxQuickMatchWaitTime(UINT nBccMaxQuickMatchWaitTime, BOOL bUpdate = TRUE);

	UINT	GetBccItemType1()																		{ 	return m_stBattleCfgInfo.nBccItemType1; 	}
	BOOL	SetBccItemType1(UINT nBccItemType1, BOOL bUpdate = TRUE);

	UINT	GetBccItemScore1()																		{ 	return m_stBattleCfgInfo.nBccItemScore1; 	}
	BOOL	SetBccItemScore1(UINT nBccItemScore1, BOOL bUpdate = TRUE);

	UINT	GetBccItemType2()																		{ 	return m_stBattleCfgInfo.nBccItemType2; 	}
	BOOL	SetBccItemType2(UINT nBccItemType2, BOOL bUpdate = TRUE);

	UINT	GetBccItemScore2()																		{ 	return m_stBattleCfgInfo.nBccItemScore2; 	}
	BOOL	SetBccItemScore2(UINT nBccItemScore2, BOOL bUpdate = TRUE);

	UINT	GetBccItemType3()																		{ 	return m_stBattleCfgInfo.nBccItemType3; 	}
	BOOL	SetBccItemType3(UINT nBccItemType2, BOOL bUpdate = TRUE);

	UINT	GetBccItemScore3()																		{ 	return m_stBattleCfgInfo.nBccItemScore3; 	}
	BOOL	SetBccItemScore3(UINT nBccItemScore3, BOOL bUpdate = TRUE);

	UINT	GetBccPerKillAwardGold()																{ 	return m_stBattleCfgInfo.nBccPerKillAwardGold; 	}
	BOOL	SetBccPerKillAwardGold(UINT nBccPerKillAwardGold, BOOL bUpdate = TRUE);

	UINT	GetBccBaseFightScore()																	{ 	return m_stBattleCfgInfo.nBccBaseFightScore; 	}
	BOOL	SetBccBaseFightScore(UINT nBccBaseFightScore, BOOL bUpdate = TRUE);

	UINT	GetBccReliveRegionRandom()																{ 	return m_stBattleCfgInfo.nBccReliveRegionRandom; 	}
	BOOL	SetBccReliveRegionRandom(UINT nBccReliveRegionRandom, BOOL bUpdate = TRUE);

	UINT	GetBccDirectionXRandom()																{ 	return m_stBattleCfgInfo.nBccDirectionXRandom; 	}
	BOOL	SetBccDirectionXRandom(UINT nBccDirectionXRandom, BOOL bUpdate = TRUE);

	UINT	GetBccDirectionYRandom()																{ 	return m_stBattleCfgInfo.nBccDirectionYRandom; 	}
	BOOL	SetBccDirectionYRandom(UINT nBccDirectionYRandom, BOOL bUpdate = TRUE);

	UINT	GetBccItemTypeAmount()																	{ 	return m_stBattleCfgInfo.nBccItemTypeAmount; 	}
	BOOL	SetBccItemTypeAmount(UINT nBccItemTypeAmount, BOOL bUpdate = TRUE);

	UINT	GetBccAwardBaseRatio()																	{ 	return m_stBattleCfgInfo.nBccAwardBaseRatio; 	}
	BOOL	SetBccAwardBaseRatio(UINT nBccAwardBaseRatio, BOOL bUpdate = TRUE);

	UINT	GetBccAwardStartViewRatio()																{ 	return m_stBattleCfgInfo.nBccAwardStartViewRatio; 	}
	BOOL	SetBccAwardStartViewRatio(UINT nBccAwardStartViewRatio, BOOL bUpdate = TRUE);

	UINT	GetBccAwardEndViewRatio()																{ 	return m_stBattleCfgInfo.nBccAwardEndViewRatio; 	}
	BOOL	SetBccAwardEndViewRatio(UINT nBccAwardEndViewRatio, BOOL bUpdate = TRUE);

	UINT	GetBccAwardKingAmount()																	{ 	return m_stBattleCfgInfo.nBccAwardKingAmount; 	}
	BOOL	SetBccAwardKingAmount(UINT nBccAwardKingAmount, BOOL bUpdate = TRUE);

	UINT	GetBccAwardKingBaseRatio()																{ 	return m_stBattleCfgInfo.nBccAwardKingBaseRatio; 	}
	BOOL	SetBccAwardKingBaseRatio(UINT nBccAwardKingBaseRatio, BOOL bUpdate = TRUE);

	UINT	GetBccAwardKingViewRatio()																{ 	return m_stBattleCfgInfo.nBccAwardKingViewRatio; 	}
	BOOL	SetBccAwardKingViewRatio(UINT nBccAwardKingViewRatio, BOOL bUpdate = TRUE);

	//红蓝对战相关
	UINT	GetBcftMaxFightTeamFinishScore()														{ 	return m_stBattleCfgInfo.nBcftMaxFightTeamFinishScore; 	}
	BOOL	SetBcftMaxFightTeamFinishScore(UINT nBcftMFTFinishScore, BOOL bUpdate = TRUE);
	
	UINT	GetBcftMaxFightTeamCostTime()															{ 	return m_stBattleCfgInfo.nBcftMaxFightTeamCostTime; 	}
	BOOL	SetBcftMaxFightTeamCostTime(UINT nBcftMFTCostTime, BOOL bUpdate = TRUE);

	UINT	GetBcftMaxRBTeamAmount()																{ 	return m_stBattleCfgInfo.nBcftMaxRBTeamAmount; 	}
	BOOL	SetBcftMaxRBTeamAmount(UINT nBcftMaxRBTeamAmount, BOOL bUpdate = TRUE);

	UINT	GetBcftDefaultCarLevel()																{ 	return m_stBattleCfgInfo.nBcftDefaultCarLevel; 	}
	BOOL	SetBcftDefaultCarLevel(UINT nBcftDefaultCarLevel, BOOL bUpdate = TRUE);
	
	UINT	GetBcftMaxCarLevel()																	{ 	return m_stBattleCfgInfo.nBcftMaxCarLevel; 	}
	BOOL	SetBcftMaxCarLevel(UINT nBcftMaxCarLevel, BOOL bUpdate = TRUE);

	UINT	GetBcftKillAwardRatio()																	{ 	return m_stBattleCfgInfo.nBcftKillAwardRatio; 	}
	BOOL	SetBcftKillAwardRatio(UINT nBcftKillAwardRatio, BOOL bUpdate = TRUE);

	UINT	GetBcftReliveRegionRandom()																{ 	return m_stBattleCfgInfo.nBcftReliveRegionRandom; 	}
	BOOL	SetBcftReliveRegionRandom(UINT nBcftReliveRegionRandom, BOOL bUpdate = TRUE);

	UINT	GetBcftWinAwardGold()																	{ 	return m_stBattleCfgInfo.nBcftWinAwardGold; 	}
	BOOL	SetBcftWinAwardGold(UINT nBcftWinAwardGold, BOOL bUpdate = TRUE);

	UINT	GetBcftDrawAwardGold()																	{ 	return m_stBattleCfgInfo.nBcftDrawAwardGold; 	}
	BOOL	SetBcftDrawAwardGold(UINT nBcftDrawAwardGold, BOOL bUpdate = TRUE);

	UINT	GetBcftDefaultAwardGold()																{ 	return m_stBattleCfgInfo.nBcftDefaultAwardGold; 	}
	BOOL	SetBcftDefaultAwardGold(UINT nBcftDefaultAwardGold, BOOL bUpdate = TRUE);

	UINT	GetBcftAwardBaseRatio()																	{ 	return m_stBattleCfgInfo.nBcftAwardBaseRatio; 	}
	BOOL	SetBcftAwardBaseRatio(UINT nBcftAwardBaseRatio, BOOL bUpdate = TRUE);

	UINT	GetBcftAwardViewRatio()																	{ 	return m_stBattleCfgInfo.nBcftAwardViewRatio; 	}
	BOOL	SetBcftAwardViewRatio(UINT nBcftAwardViewRatio, BOOL bUpdate = TRUE);

	//大乱斗相关	
	UINT	GetBccKillAwardScoreRatio()																			{ 	return m_stBattleCfgInfo.nBccKillAwardScoreRatio; 	}
	BOOL	SetBccKillAwardScoreRatio(UINT nBccKillAwardScoreRatio, BOOL bUpdate = TRUE);
	UINT	GetBccRevengeAwardKillScoreRatio()																	{ 	return m_stBattleCfgInfo.nBccRevengeAwardKillScoreRatio; 	}
	BOOL	SetBccRevengeAwardKillScoreRatio(UINT nBccRevengeAwardKillScoreRatio, BOOL bUpdate = TRUE);
	UINT	GetBccNormalReliveRemainScoreRatio()																{ 	return m_stBattleCfgInfo.nBccNormalReliveRemainScoreRatio; 	}
	BOOL	SetBccNormalReliveRemainScoreRatio(UINT nBccNormalReliveRemainScoreRatio, BOOL bUpdate = TRUE);
	UINT	GetBccViewReliveRemainScoreRatio()																	{ 	return m_stBattleCfgInfo.nBccViewReliveRemainScoreRatio; 	}
	BOOL	SetBccViewReliveRemainScoreRatio(UINT nBccViewReliveRemainScoreRatio, BOOL bUpdate = TRUE);

	//通用配置
	UINT	GetBcMegerOtherTeamS()																				{ 	return m_stBattleCfgInfo.nBcMegerOtherTeamS; 	}
	BOOL	SetBcMegerOtherTeamS(UINT nBcMegerOtherTeamS, BOOL bUpdate = TRUE);

protected:
	stBattleCfg			m_stBattleCfgInfo;
};


//全局变量
extern CBattleCfg* g_pBattleCfg;
#define BattleCfg()          (g_pBattleCfg)

#endif//__FRIEND_DATA_H__
