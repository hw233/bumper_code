//////////////////////////////////////////////////////////////////////
//文件名称：CBattleConfusionSingle.h
//功能描述：
//版本说明：CBattleConfusionSingle API
//			
//编写作者：2018.10.11 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#ifndef __BATTLECONFUSION_TEAM_H__
#define __BATTLECONFUSION_TEAM_H__

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include "time.h"
#include "../Common/TimeOut.h"
#include "../MsgHandler/NetMsg.h"
//#include "../MsgHandler/MsgBattle.h"
#include <map>
using namespace std;

//#include "Player.h"
class Player;




//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////
//战斗类型
enum //_BATTLE_MODEL_TYPE
{	
	_BATTLE_MODEL_NONE							= 0,		//练习模式
	_BATTLE_MODEL_CONFUSION						= 1,		//大乱斗模式
	_BATTLE_MODEL_RB_FIGHT_TEAM					= 2,		//红蓝队对战模式
};

//复活类型
enum //_RELIVE_TYPE
{	
	_RELIVE_TYPE_NORMAL 						= 0,		//普通复活
	_RELIVE_TYPE_VIEW							= 1,		//看视频复活
};


enum _FIGHT_MODEL_TYPE
{
	_FIGHT_MODEL_TYPE_NONE					= 0,		//xxx模式
	_FIGHT_MODEL_TYPE_CONFUSION 			= 1,		//大乱斗模式	
	_FIGHT_MODEL_TYPE_RB_FIGHT_TEAM 		= 2,		//红蓝战队模式
};

///队伍状态
enum _TEAM_STATUS
{
	_TEAM_STATUS_CREATE =	0,
	_TEAM_STATUS_QIUCK_MATCH =	1,
	//_TEAM_STATUS_PREPARE =	2,				//5V5的队伍人员已经满了，可以进行匹配战斗了。
	_TEAM_STATUS_START =	3,
	_TEAM_STATUS_END =	4,					//战争已经结束
};

//排行榜数据结构
struct BattleConfusionTeamTopInfo
{
	INT nUid;		//用户id
	INT nSorce;		//积分
	INT nKillSum;	//击杀
	std::string strNickName;	//玩家昵称
	std::string strHeadPicurl;	//玩家头像

	BattleConfusionTeamTopInfo()
	{
		nUid = 0;
		nSorce = 0;
		nKillSum = 0;
		strNickName = "";
		strHeadPicurl = "";
	}	

	// //排序使用
	// bool operator < (const BattleConfusionTeamTopInfo& obj) const
	// {
	// 	return nSorce < obj.nSorce;
	// }

	// bool operator > (const BattleConfusionTeamTopInfo& obj) const
	// {
	// 	return nSorce > obj.nSorce;
	// }
};
typedef std::vector<BattleConfusionTeamTopInfo> VT_BATTLECONFUSIONTEAM_TOP_LIST;

//对积分进行排序
bool less_(const BattleConfusionTeamTopInfo & obj1, const BattleConfusionTeamTopInfo & obj2);
//////////////////////////////////////////////////////////////////////
///道具
struct ItemInfo
{
	INT nId;				//道具id
	INT nItemType;			//道具type
	INT nSorce;				//道具积分
	//INT nTag;				//碰撞检测使用
};

typedef std::vector<ItemInfo> VT_ITEM_LIST;
enum 
{
	EM_ITEM_SORCE_1 = 200,
	EM_ITEM_SORCE_2 = 500,
	EM_ITEM_SORCE_3 = 1000,
};

const int _ITEM_SORCE[3/*_CONFUSION_ITEMTYPE_AMOUNT*/] 							= {200, 500, 1000};
//现阶段只有矿石一种，大小不同积分不同
const int _ITEM_TYPE[3/*_CONFUSION_ITEMTYPE_AMOUNT*/] 							= {110011, 110012, 110013};

typedef      long    time_t;    											/* 时间值time_t为长整型的别名*/
const int _MAX_CONFUSION_TIME 						= 5*60+2;				//战斗时间300秒
const int _RANDOM_CREATE_ITEM_INTERVAL_SECS 		= 5;					//15秒随机生成道具
const int _RANDOM_CREATE_ITEM_SUM 					= 15;					//15秒随机生成道具
const int _MAX_TEAMAMOUNT 							= 15;					//队伍人数上限
const int _ENOUTH_TEAMAMOUNT 						= 2;					//满足开战队伍人数（等待时间已经耗尽）
const int _MAX_QUICK_MATCH_WAITTIME 				= 10;					//QUICK_MATCH状态下等待时间长度
				
const int _MAX_FIGHTTEAM_WIN 						= 30;					//红蓝战队活动胜利点总积分
const int _MAX_FIGHTTEAM_TIME 						= 5*60+2; 				//红蓝战队活动每场战斗的总时间
const int _MAX_RB_TEAMAMOUNT 						= 3;					//红蓝战队队伍人数上限
/////////////////////////////////////////////////////////////////////
///战队归属方
enum _FIGHT_OWNER_COLOUR
{
	_FIGHT_OWNER_COLOUR_NONE 	= 0,		//无方战队
	_FIGHT_OWNER_COLOUR_RED 	= 1,		//红方战队	
	_FIGHT_OWNER_COLOUR_BLUE 	= 2,		//蓝方战队
};

///战队结果
enum _FIGHT_RESULT
{
	_FIGHT_RESULT_NONE 			= 0,		//无状态
	_FIGHT_RESULT_WIN 			= 1,		//获胜	
	_FIGHT_RESULT_DRAW	 		= 2,		//平局
	_FIGHT_RESULT_DEFEAT 		= 3,		//战败
};

///66ms收集每个队伍的网络数据包
struct TeamNetMsg
{
	INT nModel;				
	//消息
	INT nAction;
	INT nTargetId;

	INT nPointX;
	INT nPointY;
	INT nPointStartX;
	INT nPointStartY;
	INT nPointEndX;
	INT nPointEndY;

	INT nBeCollideId;
	INT nTType;
	INT nTPowerX;
	INT nTPowerY;
};
typedef std::multimap<UINT, TeamNetMsg> TEAM_NET_MSG_SET;
typedef TEAM_NET_MSG_SET::const_iterator CIT;
typedef pair<CIT, CIT> Range;

//队伍玩家容器
typedef std::vector< Player* > TEAMPLAYERSET;

//////////////////////////////////////////////////////////////////////
//class
//////////////////////////////////////////////////////////////////////
class CBattleTeam
{
public:
	CBattleTeam();
	virtual ~CBattleTeam();
	VOID	Destroy();
	VOID 	Init();
	VOID 	Release() {		delete this;	}

public:
	//获取队伍id
	VOID 	SetTeamId(UINT _id) 									{	m_id = _id;		}
	UINT 	GetTeamId() 											{	return m_id;	}
	//获取队伍状态
	VOID 	SetTeamStatus(UINT nStatus) 							{	m_nTeamStatus = nStatus;	}
	UINT 	GetTeamStatus() 										{	return m_nTeamStatus;		}
	//战斗开始时间戳
	VOID 	SetTimeStamp(UINT nTimeStamp) 							{	m_nTimeStamp = nTimeStamp;	}
	time_t 	GetTimeStamp() 											{	return m_nTimeStamp;		}	
	//准备开始时间戳
	VOID 	SetQuickMatchTimeStamp(UINT nQuickMatchTimeStamp) 		{	m_nQuickMatchTimeStamp = nQuickMatchTimeStamp;	}
	time_t 	GetQuickMatchTimeStamp() 								{	return m_nQuickMatchTimeStamp;		}						
	//获取霸主指针
	Player* GetKingPlayer()											{	return (m_pKingPlayer == NULL) ? NULL : m_pKingPlayer;	}
	//获取对象指针
   	BOOL 	AddMember(Player* pPlayer, INT nModelType);
   	Player* GetMemberByIndex(size_t nIndex);
   	BOOL 	DelMember(Player* pPlayer);
   	UINT 	GetMemberAmount();
   	BOOL 	IsTeamMember(Player* pPlayer);
   	BOOL 	IsAliveMember();
   	Player* GetMember(UINT nPlayerId);
	//清理队伍中所有队员
	BOOL 	ClearMember();
	//设置队伍成员战斗结果
	BOOL 	SetFightTeamResultToMember(INT nResult);
	//战斗开始
	BOOL    FightStart();
	//准备快速匹配阶段
	BOOL 	FightQuickMatch();
	//是否快速匹配超时
	BOOL    IsQuickMatchTimeout();
	//是否已经战斗结束-大乱斗模式
	BOOL    IsFightConfusionFinsh();
	//是否已经战斗结束-红蓝对战模式
	BOOL    IsFightTeamFinsh();
	//广播某个消息给全队成员
	VOID 	BroadcastTeamMsg(CNetMsg* pMsg, Player* pSender=NULL);
	//广播66ms网络数据帧包-[大乱斗]
	VOID 	BroadcastConfusionFrameMsg();
	//广播66ms网络数据帧包-[红蓝战队]
	VOID 	BroadcastTeamFrameMsg();
	//广播队伍信息(CMsgSynRoomInfo)给某位队友
	VOID 	BroadcastTeamMsgToMember(Player* pSender=NULL);
	//广播战斗开始信息(CMsgBattle)玩家数据 大乱斗模式
	VOID 	BroadcastConfusionStartMsgToMember();
	//广播战斗开始信息(CMsgBattle)玩家数据 红蓝战队模式
	VOID    BroadcastFightTeamStartMsgToMember(CBattleTeam* pEnemyTeam);
	//广播战斗结束信息(CMsgBattle)玩家数据,带排行榜，*****仅用于-zhandui
	VOID 	BroadcastTopListMsgToMember();
	//广播战斗结束数据-大乱斗
	VOID 	BroadcastConfusionEndMsgToMember();
	//广播战斗结束数据-红蓝战队
	VOID 	BroadcastFightTeamEndMsgToMember();
	//发送生成道具数据包给队伍中的某个玩家，由他来生成像素坐标点
	VOID 	CreateFightItemPos();
	//生成对战道具并广播给队伍成员
	BOOL 	CreateFightItem(ItemInfo& iteminfo);	
	//成员拾取战斗道具增加积分
	BOOL 	PickUpFightItem(Player* pPlayer, INT nItemId);
	//发送大乱斗战斗结束1倍奖励
	VOID    Award();
	//发放大乱斗战斗结束后，观看视频3倍奖励
	//VOID    AwardThreeMulti();
	//发放战队结束奖励
	VOID    AwardFightTeam();
	//更新队伍总积分数
	VOID 	UpdateFightScore(INT nFightScore);
	//广播战斗结束玩家数据,生成区域、击杀、积分等信息
	//VOID 	BroadcastFightEndMsgToMember();
	//更新霸主玩家指针
	VOID 	UpdateKingPlayer();
	// 用于定时触发的连续时钟。每次触发时为true。
	BOOL 	ToNextTime();	
	//插入服务器队伍的消息队列缓存
	VOID 	InsertFrameMsgCache(INT nModel=1/*_BATTLE_MODEL_CONFUSION*/, INT nAction=1/*_WALK_ACTION_USER_CMD*/, INT nTargetId=0, INT nPointX=0, INT nPointY=0, INT nPointStartX=0, INT nPointStartY=0,
								INT nPointEndX=0, INT nPointEndY=0, INT nBeCollideId=0, INT nTType=0, INT nTPowerX=0, INT nTPowerY=0);
	//整合A-B队伍
	BOOL 	MergeOtherTeam(INT nModelType, CBattleTeam* pOtherTeam);

	///////////////////////////////////////////////////////////////
	///红蓝战斗模式
	//设置战队归属颜色
	VOID 				SetFightTeamColour(INT nColour)					{	m_nFightTeamColour = nColour;			}
	INT 				GetFightTeamColour()							{	return m_nFightTeamColour;				}
	//设置战队击杀数
	VOID 				SetFightTeamKillSum(INT nKillSum)				{	m_nFightTeamKillSum = nKillSum;			}
	INT 				GetFightTeamKillSum()							{	return m_nFightTeamKillSum;				}
	//战斗积分
	VOID 				SetFightTotalScore(INT nFightTotalScore)		{	m_nFightTotalScore = nFightTotalScore;	}
	INT 				GetFightTotalScore()							{	return m_nFightTotalScore;				}
	//战斗结果
	VOID 				SetFightResult(INT nFightResult)				{	m_nFightResult = nFightResult;			}
	INT 				GetFightResult()								{	return m_nFightResult;					}
	//地方队伍指针
	VOID 				SetEnemyTeam(CBattleTeam* pEnemyTeam)			{	m_pEnemyTeam = pEnemyTeam;				}
	CBattleTeam* 		GetEnemyTeam()									{	return m_pEnemyTeam;					}
	//战斗时间间隔 没隔多久出发一次	
	CTimeOut& 			GetFightTimeInterval()							{	return m_tFightTimeInterval;			}	
    BOOL   				IsFightFinish()                                 {   return FALSE;                           }    
    //时间戳
    VOID 				SetRecvMsgTimeStamp(long long llTimeStamp)		{	m_llRecvMsgTimeStamp = llTimeStamp;		}
	long long 			GetRecvMsgTimeStamp()							{	return m_llRecvMsgTimeStamp;			}
	//队长ID
    VOID 				SetTeamLeaderID(UINT32 nTeamLeaderID)			{	m_nTeamLeaderID = nTeamLeaderID;		}
	long long 			GetTeamLeaderID()								{	return m_nTeamLeaderID;					}	
				
 //    //排序使用
	// bool operator < (const CBattleTeam& obj) const
	// {
	// 	return m_StampTime < obj.GetStampTime();
	// }

	// bool operator > (const CBattleTeam& obj) const
	// {
	// 	return m_StampTime > obj.GetStampTime();
	// }

	TEAMPLAYERSET& 		GetTeamPlayerS()							{	return m_vtTeamPlayerS;	}						//报名玩家
	UINT     			GetTeamAmount()								{ 	return m_vtTeamPlayerS.size();	}
	TEAM_NET_MSG_SET& 	GetTeamNetMsgSet()							{	return m_mapTeamNetMsg;	}				//66ms内收集的队伍操作数据包

private:
	////////////////////////////////////////////////////////////////
	///公共属性
			
	VT_ITEM_LIST 		m_vItemList;								//一局战斗中item列表
	TEAMPLAYERSET 		m_vtTeamPlayerS;							//队伍列表
	UINT 				m_id;										//队伍id		
	time_t 				m_nTimeStamp;								//战斗开始时间戳
	time_t 				m_nQuickMatchTimeStamp;						//准备开始时间戳
	UINT   				m_nTeamStatus;								//队伍的状态
	CTimeOut 			m_tFightTimeInterval;						//战斗时间间隔 没隔多久出发一次	
	TEAM_NET_MSG_SET 	m_mapTeamNetMsg;							//66ms内收集的队伍操作数据包
	long long 			m_llRecvMsgTimeStamp;						//时间戳
	UINT32          	m_nTeamLeaderID;							//队长ID

	///////////////////////////////////////////////////////////////
	///大乱斗队伍所需数据
	Player* 			m_pKingPlayer;								//霸主玩家指针
	INT 				m_nKingSorce;								//霸主积分

	///////////////////////////////////////////////////////////////
	///红蓝战斗战队数据
	INT 	        	m_nFightTeamColour;							//战队所属（红队、蓝队）
	INT 				m_nFightTeamKillSum;						//战队的击杀数量
	INT 				m_nFightTotalScore;							//战斗积分
	INT 				m_nFightResult;								//战斗结果
	CBattleTeam* 		m_pEnemyTeam;								//地方队伍指针			
	
	

};
#endif //__BATTLECONFUSION_SINGLE_H__