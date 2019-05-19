//////////////////////////////////////////////////////////////////////
//文件名称：RobotBase.h
//功能描述：机器人接口（基类）
//版本说明：socket API
//
//编写作者：2017.08.16 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#ifndef __ROBOTBASE_H__
#define __ROBOTBASE_H__

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include "../Common/BaseType.h"
#include "../Common/Type.h"
#include "../Common/Common.h"
#include "../NetBase/Socket.h"
#include "../NetBase/SocketInputStream.h"
#include "../NetBase/SocketOutputStream.h"
#include "../MsgHandler/NetMsg.h"
#include "Robot.h"

//#include "Battle.h"
#include "BattleTeam.h"
#include "UserData.h"
#include "BattleManager.h"
//#include "BattleConfusionSingle.h"
//add by huyf 2011.22:添加好友系统
#include "../Friend/FriendManager.h" 
//add end:添加好友系统
#include "../Item/ItemType.h" 
#include "../Item/ItemTypeManager.h" 
#include "../Item/ItemData.h" 
#include "../Item/ItemDataManager.h" 
#include "../TaskDetail/TaskDetailManager.h" 
 //class CBattleTeam;
 class CBattle;
 class CBattleConfusionSingle;
//#include "Robot.h"
//#include "Pet.h"

//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////
enum SendToServer
{
	PW_SERVER = 1,
	MTS_SERVER = 2,
	ACC_SERVER = 3,
};

#define ACC_SERVER_NUM 5025//1025
#define MTS_SERVER_NUM 5026//1026
#define PW_SERVER_NUM 5027//1027
//#define MAX_CACHE_SIZE 1024
#define MAX_KICK_TIME 6000000
#define HEART_BEAT_DETECTION_USER 30+10
#define HEART_BEAT_DETECTION 30
//如果玩家3 2 1 倒计时还未结束就发送了操作包，则丢弃
#define _STATUS_PREPARE_ALIVE_INTERVAL 3 				//准备复活状态的倒计时 3 2 1 秒数
//typedef UINT PlayerID_t;			//机器人编号
//typedef UINT ID_t;

class Robot;
//class Pet;

enum PACKET_FLAG
{
	PF_NONE=0,

};

struct ASYNC_PACKET
{
	CNetMsg*		m_pMsg;
	PlayerID_t		m_PlayerID;
	UINT		m_Flag;

	ASYNC_PACKET( )
	{
		m_pMsg = NULL;
		m_PlayerID = INVALID_ID;
		m_Flag = PF_NONE;
	};
	~ASYNC_PACKET( )
	{
		SAFE_DELETE ( m_pMsg );
		m_PlayerID = INVALID_ID;
		m_Flag = PF_NONE;
	};
};

///------------------------------------强联网对战相关常量-----------------------------------------------
//大乱斗
const int _CONFUSION_PER_KILL_AWARD_GOLD 	= 10;				//击杀奖励的金币数
const int _CONFUSION_BASE_FIGHT_SCORE 		= 1000;				//基础积分
const int _CONFUSION_REILVE_REGION_RANGDOM 	= 30;				//复活随机区域数量
const int _CONFUSION_DIRECTION_X_RANGDOM 	= 10000;			//复活时候X方向随机，扩大10000倍率 (0~1)
const int _CONFUSION_DIRECTION_Y_RANGDOM 	= 10000;			//复活时候X方向随机，扩大10000倍率 (0~1)
const int _CONFUSION_ITEMTYPE_AMOUNT		= 3;				//生成随机道具的种类(3种)

//红蓝对战
const int _FIGHTTEAM_DEFAULT_CAR_LEVEL_ 	= 1;				//击杀车辆默认最低等级
const int _FIGHTTEAM_MAX_CAR_LEVEL_ 		= 10;				//击杀车辆最高等级
const int _FIGHTTEAM_KILL_AWARD_RATIO_ 		= 1;				//击杀车辆奖励的倍率
const int _FIGHTTEAM_REILVE_REGION_RANGDOM 	= 5;				//复活随机区域数量（0-4）~（5-9）

const int _FIGHTTEAM_WIN_AWARD_GOLD 		= 150;				//胜利奖励的金币	
const int _FIGHTTEAM_DRAW_AWARD_GOLD 		= 100;				//平局奖励的金币
const int _FIGHTTEAM_DEFEAT_AWARD_GOLD 		= 50;				//失败奖励的金币
///------------------------------------强联网对战相关常量-----------------------------------------------

//游戏进度
enum GAME_PROCESS
{
	_GAME_PROCESS_NONE,			//初始化
	_GAME_PROCESS_HANDSHARK,	//刚建立连接(三次握手)
	_GAME_PROCESS_LOGIN,		//已经收到login包并进行了处理
};


//ONLINE or OFFLINE or GAMEING
enum _FRIEND_STATUS
{
	_FRIEND_STATUS_OFFLINE				= 0,		//离线
	_FRIEND_STATUS_ONLINE				= 1,		//在线	
	_FRIEND_STATUS_GAMEING				= 2,		//游戏中	
};

typedef std::map<std::string, UINT> MAP_COLLIDE_SET;
typedef MAP_COLLIDE_SET::const_iterator MAP_COLLIDE_CIT;

typedef std::map<std::string, UINT> MAP_DEAD_SET;
typedef MAP_DEAD_SET::const_iterator MAP_DEAD_CIT;
////////////////////////////////////////////////////////////////////////////////////////
///
class Player
{
  public :
	Player( );
	virtual ~Player( );

  public:
	//数据接收
	virtual BOOL ProcessInput();
	//数据发送
	virtual BOOL ProcessOutput();
	//消息处理
	virtual BOOL ProcessCommand (bool Option = true);
	//清除
	virtual VOID CleanUp();
	//逻辑接口：心跳
	virtual BOOL HeartBeat(DWORD dwTime=0);
	//OnTimer函数
	virtual BOOL OnTimer();
	//kick time修正
	virtual VOID ResetKick();
	//向此服务器端程序发送消息
	//此接口不支持线程间同步，因此只能有当前线程来调用执行
	virtual BOOL SendPacket (CNetMsg* pMsg);
	//判断当前服务器端玩家是否有效
	virtual BOOL IsValid();
	//玩家断开连接
	virtual VOID Disconnect();

	//通用接口
	virtual BOOL IsServerPlayer()							=	0;
	virtual BOOL IsClientPlayer()							=	0;
	virtual BOOL IsGamePlayer()								=	0;

	VOID 				SetNowTime(DWORD tm) 							{	m_time = tm; 			}
	DWORD 				GetNowTime()									{ 	return m_time; 			}
	UInt32				GetAccountID()									{  	return m_AccountID; 	}
	VOID				SetAccountID (UInt32 accID)						{ 	m_AccountID = accID;	}
	//add by huyf 2018.04.23:添加账号信息
	string& 			GetStrAccount()									{	return m_StrAccount; }
	VOID  				SetStrAccount(const char* strAccount) 			{ m_StrAccount = strAccount; }
	//add end:添加账号信息
	ClientID_t 			GetClientID()									{ return m_PID; }
	VOID  				SetClientID(ClientID_t id) 						{	m_PID = id;	}
	PlayerID_t			GetPlayerID()									{ 	return m_PID; 	}
	VOID 				SetPlayerID(PlayerID_t id) 						{	m_PID = id;	}
	ID_t  				GetUID()										{ 	return m_UID; }
	VOID 				SetUID(ID_t id)									{ 	m_UID = id; 	}
	ID_t 				GetPlayerManagerID() 							{ 		return m_PlayerManagerID; }
	VOID  				SetPlayerManagerID (ID_t id)					{	m_PlayerManagerID = id;	}
	Socket*  			GetSocket()										{ 	return m_pSocket; 	}
	VOID 				SetSocket(Socket* sock)							{ 		m_pSocket = sock; }
	SocketInputStream* 	GetSocketInputStream()							{   		return m_pSocketInputStream; }
	VOID  				SetSocketInputStream(SocketInputStream* sis)	{ 		m_pSocketInputStream = sis; }
	SocketOutputStream*	GetSocketOutputStream()							{  		  return m_pSocketOutputStream;	}
	VOID 				SetSocketOutputStream(SocketOutputStream* sos) 	{ m_pSocketOutputStream = sos; }
	BYTE 				GetPacketIndex()								{  		return m_PacketIndex; }
	VOID  				SetPacketIndex(BYTE index)						{		m_PacketIndex = index;	}
	BOOL 				IsEmpty()										{ 	return m_IsEmpty; }
	VOID 				SetEmpty(BOOL empty=TRUE)						{ m_IsEmpty = empty;	}
	BOOL 				IsDisconnect()									{ 	 return m_IsDisconnect;	}
	VOID 				SetDisconnect(BOOL disconnect=TRUE)				{ 	m_IsDisconnect = disconnect; }
	UINT 				GetReceiveBufferSize() const 					{ 	return m_pSocket->getReceiveBufferSize(); }	
	BOOL 				SetReceiveBufferSize(UINT size)					{ 	return m_pSocket->setReceiveBufferSize ( size );	}
	UINT 				GetSendBufferSize() const						{ 		return m_pSocket->getSendBufferSize();	}
	BOOL 				SetSendBufferSize(UINT size)					{ 		return m_pSocket->setReceiveBufferSize ( size ); }
	DWORD 				UpdateNowTime();
	UInt32 				CheckTime();
	UInt32 				CheckTimeEx();
	Robot* 				GetRobot()										{  	return m_robot;	}
	BOOL 				CreateNewRobot(const char* robotID, UInt32 accID, UInt32 protoTypeID=0 );	
	BOOL 				SendMsgToClient(CNetMsg* msg);

	//奖励道具
	CItemData*				AwardItem(UINT nItemType, UINT nAmount=1);

	//add by huyf 2018.10.09:碰碰车二期
	BOOL  					SetBcs(CBattleConfusionSingle* pBcs)		{ 	CHECKF (pBcs);  m_pBcs = pBcs; return TRUE;	}
	CBattleConfusionSingle* GetBcs() 									{ 	return m_pBcs; }
	VOID  					ClearBcs() 									{	 m_pBcs = NULL; }

	BOOL  					SetBct(CBattleTeam* pBt)					{ 	CHECKF(pBt); m_pBct = pBt; return TRUE; }
	CBattleTeam* 			GetBct()									{	return m_pBct; 	}
	VOID 					ClearBct()	 								{ 	m_pBct = NULL; }

	BOOL  					SetBrbt(CBattleTeam* pBt)					{ 	CHECKF(pBt); m_pBrbt = pBt; return TRUE; }
	CBattleTeam* 			GetBrbt()									{	return m_pBrbt; 	}
	VOID 					ClearBrbt()	 								{ 	m_pBrbt = NULL; }
	CUserData* 				QueryData()									{  	return &m_UserData;	}
	MAP_COLLIDE_SET&		QueryCollideSet()							{	return m_MapCollideSet; }			// 玩家碰撞信息容器
	MAP_DEAD_SET&			QueryDeadSet()								{	return m_MapDeadSet;	}			// 玩家死亡信息容器	
	CFriendManager&			QueryFriendManager()						{	return m_FriendManager;	}			// 好友管理的对象
	//CRecentManager&		QueryRecentManager()						{	return m_RecentManager;	}			// 最近玩家管理的对象
	CItemDataManager* 		QueryItemDataManager()						{	return m_pItemDataManager;	}		// 添加道具系统
	CTaskDetailManager* 	QueryTaskDetailManager()					{	return m_pTaskDetailManager;	}	// 添加任务系统
	CTimeOut& 				QueryLimitTimeInterval()					{	return m_tLimitTimeInterval;	};	// 限时免费计时器	
	CTimeOut& 				QueryOnLineInterval()						{	return m_tOnLineInterval;	};		// 在线领奖计时器
	CTimeOut& 				QueryMultiKillTimer()						{	return m_tMultiKillTimer;	};		// 在线领奖计时器	
	
	//玩家id
	VOID 					SetUid(UInt32 nUid, BOOL bUpdate=TRUE)							{	QueryData()->SetUid ( nUid, bUpdate );   }
	UInt32 					GetUid()														{	return QueryData()->GetUid();     }

	//string 头像
	VOID 					SetHeadPicurl(std::string& strHeadPicurl, BOOL bUpdate=TRUE)	{	QueryData()->SetHeadPicurl (strHeadPicurl, bUpdate );     }
    std::string& 			GetHeadPicurl()													{	return QueryData()->GetHeadPicurl();    }

	//string 昵称
	VOID 					SetNickName(std::string& strNickName, BOOL bUpdate =TRUE)		{	QueryData()->SetNickName(strNickName,bUpdate);    }
	std::string& 			GetNickName()													{	return QueryData()->GetNickName();     }

	//int 金币数量
	//VOID 					SetGoldSum(UInt32 nGoldSum, BOOL bUpdate=TRUE)					{	QueryData()->SetGoldSum(nGoldSum,bUpdate);    }
	//UInt32 				GetGoldSum()													{	return  QueryData()->GetGoldSum();	}

	VOID 					SetGoldSum(UInt32 nGoldSum, BOOL bUpdate=TRUE)					{	SetGoldSumTransformItem(nGoldSum, bUpdate);    }
	UInt32 					GetGoldSum(BOOL bTransform = TRUE)								{	if(bTransform) return  GetGoldSumTransformItem(); else return QueryData()->GetGoldSum();	}

	//金币修改为道具形式
	VOID 					SetGoldSumTransformItem(UInt32 nGoldSum, BOOL bUpdate=TRUE);
	UInt32 					GetGoldSumTransformItem();

	//int 击杀数量
	VOID 					SetKillSum(UInt32 nKillSum, BOOL bUpdate = TRUE)				{	QueryData()->SetKillSum(nKillSum, bUpdate);	}
	UInt32 					GetKillSum()													{	return QueryData()->GetKillSum();}

	//int 段位id
	VOID 					SetGrandingId (UInt32 nGrandingId, BOOL bUpdate = TRUE)			{	QueryData()->SetGrandingId(nGrandingId, bUpdate);}
	UInt32 					GetGrandingId()													{	return QueryData()->GetGrandingId();}

	//int 当前星星数量
	VOID 					SetCurStarSum (UInt32 nCurStarSum, BOOL bUpdate = TRUE)			{	QueryData()->SetCurStarSum(nCurStarSum, bUpdate);	}
	UInt32 					GetCurStarSum()													{	return QueryData()->GetCurStarSum();	}

	//string 已解锁车辆
	VOID  					SetUnlockSum(std::string& strUnlockSum, BOOL bUpdate = TRUE)	{	QueryData()->SetUnlockSum(strUnlockSum, bUpdate);	}
	std::string& 			GetUnlockSum()													{	return QueryData()->GetUnlockSum();	}

	//int 当前使用中的车辆
	VOID  					SetUseCar(UInt32 nUseCar, BOOL bUpdate = TRUE)					{	QueryData()->SetUseCar(nUseCar, bUpdate);	}
	UInt32 					GetUseCar()														{	return QueryData()->GetUseCar();	}

	VOID             		SetKeepStar(UInt32 nKeepStar, BOOL bUpdate = TRUE)				{	QueryData()->SetKeepStar(nKeepStar, bUpdate);	}
    UInt32           		GetKeepStar()													{	return QueryData()->GetKeepStar();	}

    VOID             		SetGetGift(UInt32 nGetGift, BOOL bUpdate = TRUE)				{	QueryData()->SetGetGift(nGetGift, bUpdate);	}
    UInt32           		GetGetGift() 													{	return QueryData()->GetGetGift();	}

	//int 霸主令牌数量
	VOID  					SetKingSum(UInt32 nKingSum, BOOL bUpdate = TRUE )				{	QueryData()->SetKingSum(nKingSum, bUpdate);}
	UInt32 					GetKingSum()													{	return QueryData()->GetKingSum();}

	//string 当天获取礼包的次数
	VOID 					SetGiftDay(std::string& strGiftDay, BOOL bUpdate = TRUE )		{	QueryData()->SetGiftDay(strGiftDay, bUpdate);	}
	std::string& 			GetGiftDay()													{	return QueryData()->GetGiftDay();	}

	VOID             		SetLastDay(std::string& strLastDay, BOOL bUpdate = TRUE)		{	QueryData()->SetLastDay(strLastDay, bUpdate);	}
    std::string&     		GetLastDay()													{	return QueryData()->GetLastDay();	}

	//int 当天获取礼包的次数
	VOID 					SetGiftSum(UInt32 nGiftSum, BOOL bUpdate = true)				{	QueryData()->SetGiftSum(nGiftSum, bUpdate);	}
	UInt32 					GetGiftSum()													{	return QueryData()->GetGiftSum();	}

	//string 当天保留星星的次数
	VOID  					SetStarDay(std::string& strStarDay, BOOL bUpdate = TRUE)		{	QueryData()->SetStarDay(strStarDay, bUpdate );	}
	std::string& 			GetStarDay()													{	return QueryData()->GetStarDay();	}

	//int 当天保留星星的次数
	VOID  					SetStarSum(UInt32 nStarSum, BOOL bUpdate = TRUE)				{	QueryData()->SetStarSum(nStarSum, bUpdate);	}
	UInt32 					GetStarSum()													{	return QueryData()->GetStarSum();	}

	//int  本局击杀数 初始0
	VOID  					SetFightKillSum(UInt32 nFightKillSum, BOOL bUpdate = TRUE)		{	QueryData()->SetFightKillSum(nFightKillSum, bUpdate );	}
	UInt32  				GetFightKillSum()												{	return QueryData()->GetFightKillSum();	}

	//int  本局积分 初始1000
	VOID 					SetFightScore(UInt32 nFightScore, BOOL bUpdate = TRUE)			{	QueryData()->SetFightScore(nFightScore, bUpdate );	}
	UInt32 					GetFightScore()													{	return QueryData()->GetFightScore();	}

	//int  本局等级 初始1
	VOID 					SetFightLevel(UInt32 nFightLevel, BOOL bUpdate = TRUE)			{	QueryData()->SetFightLevel(nFightLevel, bUpdate);	}
	UInt32  				GetFightLevel()													{	return QueryData()->GetFightLevel();	}

	//int  初始积分加成 3000
    VOID             		SetAddScore(UInt32 nAddScore, BOOL bUpdate = TRUE)				{	QueryData()->SetAddScore(nAddScore, bUpdate);}
    UInt32           		GetAddScore()                              						{    return QueryData()->GetAddScore();         }

    //int  战斗结束后金币加成百分比 100%
    VOID             		SetAddGold(UInt32 nAddGold, BOOL bUpdate = TRUE)				{	QueryData()->SetAddGold(nAddGold, bUpdate);}
    UInt32           		GetAddGold()                              						{    return QueryData()->GetAddGold();         }
          
    //int  大乱斗结束时候奖励的倍率，默认1倍，看视频后3倍率   
    VOID             		SetRatio(UInt32 nRatio, BOOL bUpdate = TRUE)					{	QueryData()->SetRatio(nRatio, bUpdate);}
    UInt32           		GetRatio()                              						{    return QueryData()->GetRatio();         }  

    //int  大乱斗-车辆当前等级  
    VOID             		SetCarLevel(UInt32 nCarLevel, BOOL bUpdate = TRUE)				{	QueryData()->SetCarLevel(nCarLevel, bUpdate);}
    UInt32           		GetCarLevel()                              						{    return QueryData()->GetCarLevel();        }   
    
    //int  死亡状体 
    VOID             		SetDeadStatus(UInt32 nDeadStatus, BOOL bUpdate = TRUE)			{	QueryData()->SetDeadStatus(nDeadStatus, bUpdate);}
    UInt32           		GetDeadStatus()                              					{    return QueryData()->GetDeadStatus();         } 

    //int  游戏步骤
    VOID             		SetGameStep(UInt32 nGameStep, BOOL bUpdate = TRUE)				{	QueryData()->SetGameStep(nGameStep, bUpdate);}
    UInt32           		GetGameStep()													{    return QueryData()->GetGameStep();         }

    //int  BUFF 
    VOID             		SetBuff(UInt32 nBuff, BOOL bUpdate = TRUE)						{	QueryData()->SetBuff(nBuff, bUpdate);}
    UInt32           		GetBuff()                              							{   return QueryData()->GetBuff();         }  

    //int  霸主标识 
    VOID             		SetKingRank(UInt32 nKingRank, BOOL bUpdate = TRUE)				{	QueryData()->SetKingRank(nKingRank, bUpdate);}
    UInt32           		GetKingRank()                              						{   return QueryData()->GetKingRank();         } 

    //int  仇人id  不用持久化  
    VOID             		SetEnemyId(UInt32 nEnemyId, BOOL bUpdate = TRUE)				{	QueryData()->SetEnemyId(nEnemyId, bUpdate);}
    UInt32           		GetEnemyId()								   					{   return QueryData()->GetEnemyId();         } 

    //int  死亡时积分
    VOID             		SetDeadScore(UInt32 nDeadScore, BOOL bUpdate = TRUE)			{	QueryData()->SetDeadScore(nDeadScore, bUpdate);}
    UInt32           		GetDeadScore()													{   return QueryData()->GetDeadScore();         } 

    ////int 红蓝组对时战斗结果
    VOID                    SetFightTeamResult(UInt32 nFightTeamResult, BOOL bUpdate = TRUE){   QueryData()->SetFightTeamResult(nFightTeamResult, bUpdate); }
    UInt32                  GetFightTeamResult()                                            {   return QueryData()->GetFightTeamResult();  }

    //每日任务掩码位reset时间
    VOID             		SetLastResetTaskTime(BOOL bUpdate = TRUE) 						{   QueryData()->SetLastResetTaskTime(bUpdate); }
    UInt32             		GetLastResetTaskTime()  										{ 	return QueryData()->GetLastResetTaskTime(); }

    //当前是一年中的第几周
    VOID             		SetCurWeekNum(BOOL bUpdate = TRUE)								{   QueryData()->SetCurWeekNum(bUpdate); }
    UInt32           		GetCurWeekNum()  												{ 	return QueryData()->GetCurWeekNum(); }

    //当前星期[0-6]:星期天-星期六
    VOID             		SetCurTmWday(UInt32 nCurTmWday, BOOL bUpdate = TRUE)			{   QueryData()->SetCurTmWday(nCurTmWday, bUpdate); }
    UInt32              	GetCurTmWday()													{   return QueryData()->GetCurTmWday();  }

    //展示索引
    VOID             		SetShowIdx(UInt32 nShowIdx, BOOL bUpdate = TRUE)				{   QueryData()->SetShowIdx(nShowIdx, bUpdate); }
    UInt32           		GetShowIdx()  													{   return QueryData()->GetShowIdx();  }

    //展示的道具
    VOID             		SetShowItemType(UInt32 nShowItemType, BOOL bUpdate = TRUE)		{   QueryData()->SetShowItemType(nShowItemType, bUpdate); }
    UInt32           		GetShowItemType() 												{   return QueryData()->GetShowItemType();  }

    //展示的道具的剩余时间戳(秒)
    VOID             		SetShowRemainTs(UInt32 nShowRemainTs=3600*2, BOOL bUpdate = TRUE)		{   QueryData()->SetShowRemainTs(nShowRemainTs, bUpdate); }
    UInt32           		GetShowRemainTs() 												{   return QueryData()->GetShowRemainTs();  }

    //在线领奖索引
    VOID             		SetOnLineIdx(UInt32 nOnLineIdx, BOOL bUpdate = TRUE)			{   QueryData()->SetOnLineIdx(nOnLineIdx, bUpdate); }
    UInt32           		GetOnLineIdx()  												{   return QueryData()->GetOnLineIdx();  }

    //在线领奖索引展示的道具itemtype
    VOID             		SetOnLineItemType(UInt32 nOnLineItemType, BOOL bUpdate = TRUE){   QueryData()->SetOnLineItemType(nOnLineItemType, bUpdate); }
    UInt32           		GetOnLineItemType()											{   return QueryData()->GetOnLineItemType();  }

    //在线领奖索引展示的道具的剩余时间戳(秒)
    VOID             		SetOnLineRemainTs(UInt32 nOnLineRemainTs, BOOL bUpdate = TRUE){   QueryData()->SetOnLineRemainTs(nOnLineRemainTs, bUpdate); }
    UInt32           		GetOnLineRemainTs() 											{   return QueryData()->GetOnLineRemainTs();  }

    //uint 金币转换道具 0 : 未转换 1 : 已经转换过
    VOID             		SetIsGoldTransformItem(UInt32 nIsGoldTransformItem, BOOL bUpdate = TRUE){   QueryData()->SetIsGoldTransformItem(nIsGoldTransformItem, bUpdate); }
    UInt32           		GetIsGoldTransformItem() 												{   return QueryData()->GetIsGoldTransformItem();  }

    //uint 当前使用的拖尾道具唯一id
    VOID             		SetTail(UInt32 nTail, BOOL bUpdate = TRUE)						{   QueryData()->SetTail(nTail, bUpdate); }
    UInt32           		GetTail()														{   return QueryData()->GetTail();  }

    // add by gxx 2018.11.13
	////int 签到天数
    VOID                    SetSignDays(UInt32 nSignDays, BOOL bUpdate = TRUE)              {   QueryData()->SetSignDays(nSignDays, bUpdate); }
    UInt32                  GetSignDays()                                                   {   return QueryData()->GetSignDays();  }

    // 玩家状态	0：下线 1:在线 2：游戏中
    VOID                    SetGameStatus(UInt32 GameStatus, BOOL bUpdate = TRUE)           {   QueryData()->SetGameStatus(GameStatus, bUpdate); }
    UInt32                  GetGameStatus()                                                 {   return QueryData()->GetGameStatus();  }

    //多杀计数
    VOID                    SetMultiKillCount(UInt32 nMultiKillCount, BOOL bUpdate = TRUE)      {   m_nMultiKillCount = nMultiKillCount; }
    UInt32                  GetMultiKillCount()                                                 {   return m_nMultiKillCount;  }
			

   /* //string 最后签到日期
    VOID             SetLastSignDay(std::string const& strLastSignDay, BOOL bUpdate = TRUE);{	QueryData()->SetLastSignDay(strLastSignDay, bUpdate);	}
    std::string&     GetLastSignDay()					 									{	QueryData()->GetLastSignDay();	}*/

    //int 签到7天标志 第几周
    VOID             SetWeek(UInt32 nWeek, BOOL bUpdate = TRUE)	                            {	QueryData()->SetWeek(nWeek, bUpdate);	}
    UInt32           GetWeek()																{	return QueryData()->GetWeek();		}
	
	//int 第1天
    VOID             SetDay1(UInt32 nDay1, BOOL bUpdate = TRUE)                            	{	QueryData()->SetDay1(nDay1, bUpdate);	}
    UInt32           GetDay1()																{	return QueryData()->GetDay1();		}

   //int 第2天
    VOID             SetDay2(UInt32 nDay2, BOOL bUpdate = TRUE)                            	{	QueryData()->SetDay2(nDay2, bUpdate);	}
    UInt32           GetDay2()																{	return QueryData()->GetDay2();		}

    //int 第3天
    VOID             SetDay3(UInt32 nDay3, BOOL bUpdate = TRUE)                            	{	QueryData()->SetDay3(nDay3, bUpdate);	}
    UInt32           GetDay3()																{	return QueryData()->GetDay3();		}

    //int 第4天
    VOID             SetDay4(UInt32 nDay4, BOOL bUpdate = TRUE)                            	{	QueryData()->SetDay4(nDay4, bUpdate);	}
    UInt32           GetDay4()																{	return QueryData()->GetDay4();		}
    
    //int 第5天
    VOID             SetDay5(UInt32 nDay5, BOOL bUpdate = TRUE)                            	{	QueryData()->SetDay5(nDay5, bUpdate);	}
    UInt32           GetDay5()																{	return QueryData()->GetDay5();		}
    
    //int 第6天
    VOID             SetDay6(UInt32 nDay6, BOOL bUpdate = TRUE)                            	{	QueryData()->SetDay6(nDay6, bUpdate);	}
    UInt32           GetDay6()																{	return QueryData()->GetDay6();		}
    
    //int 第7天
    VOID             SetDay7(UInt32 nDay7, BOOL bUpdate = TRUE)                            	{	QueryData()->SetDay7(nDay7, bUpdate);	}
    UInt32           GetDay7()																{	return QueryData()->GetDay7();		}

    //int 碎片数量
    VOID             SetDebris(UInt32 nDebirsSum, BOOL bUpdate = TRUE)                      {	QueryData()->SetDebris(nDebirsSum, bUpdate);	}
    UInt32           GetDebris()															{	return QueryData()->GetDebris();		}
    //int 皮肤体验天数
    VOID             SetExpSkin(UInt32 nExpSkin, BOOL bUpdate = TRUE)                      	{	QueryData()->SetExpSkin(nExpSkin, bUpdate);	}
    UInt32           GetExpSkin()															{	return QueryData()->GetExpSkin();		}

    //int 第1宝箱
    VOID             SetBox1(UInt32 nBox1, BOOL bUpdate = TRUE)                            	{	QueryData()->SetBox1(nBox1, bUpdate);	}
    UInt32           GetBox1()																{	return QueryData()->GetBox1();		}

     //int 第2宝箱
    VOID             SetBox2(UInt32 nBox2, BOOL bUpdate = TRUE)                            	{	QueryData()->SetBox2(nBox2, bUpdate);	}
    UInt32           GetBox2()																{	return QueryData()->GetBox2();		}

     //int 第3宝箱
    VOID             SetBox3(UInt32 nBox3, BOOL bUpdate = TRUE)                            	{	QueryData()->SetBox3(nBox3, bUpdate);	}
    UInt32           GetBox3()																{	return QueryData()->GetBox3();		}

     //int 第4宝箱
    VOID             SetBox4(UInt32 nBox4, BOOL bUpdate = TRUE)                            	{	QueryData()->SetBox4(nBox4, bUpdate);	}
    UInt32           GetBox4()																{	return QueryData()->GetBox4();		}

    //end by gxx 

    //保存玩家数据
    //BOOL 					SaveInfo()														{	return QueryData()->SaveInfo();			}
    BOOL 					SaveInfo();

    //游戏进度    
    VOID             		SetGameProcess(INT nGameProcess) 								{   m_nGameProcess = nGameProcess; }
    INT              		GetGameProcess()  												{ 	return m_nGameProcess; }

    //根据积分获取配置战斗等级
	UINT  					GetFightCfgLevel (UINT nSorce);
	//拾取战斗道具
 	BOOL 					PickUpFightItem(INT nItemId);
 	//发放观看视屏3倍率奖励-[大乱斗]
	VOID 					AwardThreeMulti();	
	//发放观看视屏3倍率奖励-[红蓝对战]
	VOID 					AwardThreeMultiFightTeam();
 	
 	//大乱斗模式中被我击杀了玩家的指针
 	VOID             		SetBeKiller(Player* pBeKiller)									{	m_pBeKiller = pBeKiller;				}
    Player*           		GetBeKiller()													{   return m_pBeKiller;         			} 
	//add end:碰碰车二期
	//当前发生碰撞事件的帧
	VOID             		SetCurCollideFrame(UINT nCurCollideFrame) 						{   m_nCurCollideFrame = nCurCollideFrame; }
    UINT              		GetCurCollideFrame()  											{ 	return m_nCurCollideFrame; }
	
	//死亡计时器
	CTimeOut& 				GetDeadTimeInterval()											{   return m_tDeadTimeInterval;         	} 	

	//永久掩码位
	UInt32					GetTaskMask()													{	return QueryData()->GetTaskMask();		}
	BOOL 					AddTaskMask(INT idx, BOOL bUpdate=TRUE);
	VOID 					ClrTaskMask(INT idx, BOOL bUpdate=TRUE);
	BOOL 					ChkTaskMask(INT idx);
   //每周任务相关
    UInt32					GetWeekTaskMask()												{	return QueryData()->GetDayTaskMask();		}
	VOID 					ResetWeekTaskMask(BOOL bUpdate, BOOL bSynMsg=TRUE);
	BOOL 					ChkWeekTaskMask(INT idx);
	VOID 					ClrWeekTaskMaskAll(BOOL bUpdate, BOOL bSynMsg=TRUE);
	VOID 					ClrWeekTaskMask(INT idx, BOOL bUpdate=TRUE);
	BOOL 					AddWeekTaskMask(INT idx, BOOL bUpdate=TRUE);
    //每日任务相关
    UInt32					GetDayTaskMask()												{	return QueryData()->GetDayTaskMask();		}
	UInt32  				GetDayTaskMask1()												{	return QueryData()->GetDayTaskMask1();	}
	VOID 					ResetDayTaskMask(BOOL bUpdate, BOOL bSynMsg=TRUE);
	BOOL 					ChkDayTaskMask(INT idx);
	VOID 					ClrDayTaskMaskAll(BOOL bUpdate, BOOL bSynMsg=TRUE);
	VOID 					ClrDayTaskMask(INT idx, BOOL bUpdate=TRUE);
	BOOL 					AddDayTaskMask(INT idx, BOOL bUpdate=TRUE);
	//清理所有每日任务completenum字段
	VOID 					ResetDayTaskMaskCompltetNum(UINT nTaskType, BOOL bUpdate, BOOL bSynMsg=TRUE);
	//设置在线领奖
	BOOL 					SetOnLineAward(UINT nIdx, BOOL bSynMsg=TRUE);
	//触发玩家日常任务接口
	BOOL 					TriggerDayTaskDetail(UINT nTaskId);
	//解锁车辆改造
	BOOL 					RemakeUnlockCar();

public:
	UInt32 						m_AccountID;
	//add by huyf 2018.04.23:添加账号信息
	//CHAR                      m_StrAccount[_MAX_ACCOUNTSIZE];
	std::string                 m_StrAccount;
	//add end:添加账号信息
	PlayerID_t					m_PID;
	ID_t						m_UID;
	ID_t						m_PlayerManagerID;
	Socket*						m_pSocket;
	SocketInputStream* 			m_pSocketInputStream;
	SocketOutputStream* 		m_pSocketOutputStream;
	BYTE						m_PacketIndex;
	BOOL 						m_IsEmpty;
	BOOL 						m_IsDisconnect;
	Robot* 					 	m_robot;	

protected:
	CUserData 	     			m_UserData;
	DWORD 						m_time;
	
	CTimeOut 					m_tLimitTimeInterval;		//限时免费计时器---下线不计时
	CTimeOut 					m_tOnLineInterval;			//在线领奖计时器---下线不计时
	CTimeOut 					m_tMultiKillTimer;			//多杀计时器---下线不计时
	UINT 						m_nMultiKillCount;			//多杀计数

	//add by huyf 2018.10.09:碰碰车二期
	//大乱斗模式
	CBattleConfusionSingle* 	m_pBcs;						//单人指针
	CBattleTeam* 				m_pBct;						//队伍指针
	//红蓝战队
	CBattleTeam* 				m_pBrbt;					//队伍指针
	Player* 					m_pBeKiller;				//大乱斗模式中被我击杀了玩家的指针
	INT 						m_nGameProcess;				//游戏进度
	MAP_COLLIDE_SET				m_MapCollideSet;			//玩家碰撞信息容器
	MAP_DEAD_SET				m_MapDeadSet;				//玩家死亡信息容器
	//如果玩家3 2 1 倒计时还未结束就发送了操作包，则丢弃
	CTimeOut 					m_tDeadTimeInterval;		//死亡计时器
	UINT             			m_nCurCollideFrame;			//当前发生碰撞事件的帧
	//add end:碰碰车二期
	//add by huyf 2018.12.28:添加任务系统
	CTaskDetailManager* 		m_pTaskDetailManager;
	//add end:添加任务系统
	//add by huyf 2018.12.13:添加道具系统
	CItemDataManager* 			m_pItemDataManager;
	//add end:添加道具系统
	
	//add by huyf 2011.21:添加好友系统
protected: 
	CFriendManager				m_FriendManager;			// 好友管理的对象
	UINT 						m_nGameStatus;				// 玩家状态	0：下线 1:在线 2：游戏中
	//CRecentManager	m_RecentManager;					// 最近玩家管理的对象
	//add end:添加好友系统
};

#endif
