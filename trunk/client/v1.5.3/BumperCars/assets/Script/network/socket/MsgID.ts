
/////////////////////大消息id---Start////////////////////////
export enum MsgID
{
    MSG_LOGIN       		= 2001, 
	MSG_USERINFO			= 2002,
	MSG_SYN_ROOM			= 2003,
    MSG_BATTLE      		= 2004, 
    MSG_VAS         		= 2005,
    MSG_ACTION      		= 2006,
    MSG_ITEM_ATTR   		= 2007,
	MSG_EXCHANGE    		= 2008,
	MSG_WALK	    		= 2009,
	MSG_TASKDETAIL			= 2010,		//任务包
	MSG_SIGNIN				= 2011,		//签到包

	MSG_BUMPER_TRANSFER    	= 2012,     //Bumper迁移PHP代码至C++

	MSG_FRIEND    			= 2013,     //好友系统
	MSG_FRIEND_INFO			= 2014,		//好友信息


	MSG_USERATTRIB			= 2016,		//用户属性包
	MSG_ITEMTTRIB			= 2017,		//物品属性包

	_MSG_TICK				= 2018,		//tick包

};
/////////////////////大消息id---End////////////////////////




/////////////////////小消息id---Start////////////////////////

//签到
export enum Msg_SignIn_Type
{
	//签到
	_SIGNIN_ACTION_SIGNIN							= 1,
	_SIGNIN_ACTION_SIGNIN_SUC						= 2,
	_SIGNIN_ACTION_SIGNIN_FAIL						= 3,
}

//任务
export enum Msg_Task_Type
{
	_TASK_DETAIL_LIST							= 1,		//获取任务列表
	_TASK_DETAIL_ADD							= 2,		//添加任务
	_TASK_DETAIL_LIST_SUC						= 3,		//获取任务列表成功
	_TASK_DETAIL_LIST_FAIL						= 4,		//获取任务列表失败

	_TASK_DETAIL_UPDATE							= 5,		//更新任务状态

	_TASK_DETAIL_AWARD							= 6,		//领取奖励
	_TASK_DETAIL_AWARD_SUC						= 7,		//领取奖励成功
	_TASK_DETAIL_AWARD_FAIL						= 8,		//领取奖励失败

	_TASK_DETAIL_TRY_GAME						= 9,		//试玩任务上报
	_TASK_DETAIL_TRY_GAME_FAIL					= 10,		//试玩任务上报失败

};

//PING
export enum Msg_Ping_Type
{
	_PING_										= 1,
}

//道具属性包
export enum Msg_ItemAttrib_Type 
{
	_ITEMATTRIB_ID 								= 0,					// 道具唯一ID(数据库自增量)
	_ITEMATTRIB_USER_ID 						= 1,					// 用户ID
	_ITEMATTRIB_ITEMTYPE 						= 2,					// 道具类型typeID
	_ITEMATTRIB_TYPE 							= 3,					// 道具类型type
	_ITEMATTRIB_LIFE_TIME 						= 4,					// 试用时间（剩余）
	_ITEMATTRIB_AMOUNT							= 5,					// 叠加数量(当前)
};

//用户属性更新包
export enum Msg_UserAttrib_Type
{
	_USERATTRI_HEAD_PICURL							= 1, 	//【预留不做更新】
	_USERATTRI_NICK_NAME							= 2,	//【预留不做更新】
	_USERATTRI_GOLD_SUM								= 3,	//更新金币
	_USERATTRI_KILL_SUM								= 4,	//更新击杀数
	_USERATTRI_GRANDING_ID							= 5,	//更新段位
	_USERATTRI_CUR_STAR_SUM							= 6,	//更新星星数
	_USERATTRI_UNLOCK_CARS							= 7,	//更新已解锁车辆
	_USERATTRI_USE_CAR								= 8,	//更新使用中的车辆
	_USERATTRI_KING_SUM								= 9,	//更新霸主令牌
	_USERATTRI_GIFT_DAY								= 10,	//更新GIFT_DAY
	_USERATTRI_GIFT_SUM								= 11,	//更新领取礼包的次数
	_USERATTRI_STAR_SUM								= 12,	//更新保留星星的次数
	_USERATTRI_FIGHT_KILL_SUM						= 13,	//更新FIGHT_KILL_SUM
	_USERATTRI_FIGHT_SCORE							= 14,	//更新FIGHT_SCORE
	_USERATTRI_ADD_SCORE							= 15,	//更新ADD_SCORE
	_USERATTRI_ADD_GOLD								= 16,	//更新ADD_GOLD
	_USERATTRI_RATIO								= 17,	//更新RATIO
	_USERATTRI_ENEMY_ID								= 18,	//更新ENEMY_ID	
	_USERATTRI_GET_GIFT								= 19,	//GET_GIFT	
	_USERATTRIB_DAYTASKMASK_FIR						= 21,	//签到 任务 掩码位
	_USERATTRIB_SIGNINMASK_FIR						= 22,	//星期几

	_USERATTRIB_SHOW_IDX							= 23,	//展示索引【预留后期扩展前期不适用】
	_USERATTRIB_SHOW_ITEM_TYPE						= 24,	//显示皮肤使用奖励道具的ItemType
	_USERATTRIB_SHOW_REMAIN_TIME					= 25,	//展示的道具的剩余时间(秒)

	_USERATTRIB_ONLINE_IDX							= 26,	//在线领奖索引
	_USERATTRIB_ONLINE_ITEM_TYPE					= 27,	//在线领奖索引展示的道具itemtype
	_USERATTRIB_ONLINE_REMAIN_TIME					= 28,	//在线领奖索引展示的道具的剩余时间

	_USERATTRIB_TASKMASK							= 29,	//永久任务掩码位
	_USERATTRIB_WEEKTASKMASK						= 30,	//每周任务掩码位
}

//战斗包
export enum Msg_Bettle_Type
{
	_BATTLE_ACTION_CREATE_ROOM 						= 1,		//创建房间
	_BATTLE_ACTION_CREATE_ROOM_SUC 					= 2,		//创建房间
	_BATTLE_ACTION_CREATE_ROOM_FAIL 				= 3,		//创建房间

	_BATTLE_ACTION_QUICK_MATCH 						= 4,		//快速匹配
	_BATTLE_ACTION_QUICK_MATCH_SUC 					= 5,		//快速匹配
	_BATTLE_ACTION_QUICK_MATCH_FAIL 				= 6,		//快速匹配

	_BATTLE_ACTION_QUICK_JOIN	 					= 7,		//快速加入
	_BATTLE_ACTION_QUICK_JOIN_SUC	 				= 8,		//快速加入
	_BATTLE_ACTION_QUICK_JOIN_FAIL	 				= 9,		//快速加入

	_BATTLE_ACTION_INVITE_JOIN	 					= 10,		//邀请好友加入
	_BATTLE_ACTION_INVITE_JOIN_SUC	 				= 11,		//邀请好友加入
	_BATTLE_ACTION_INVITE_JOIN_FAIL	 				= 12,		//邀请好友加入

	_BATTLE_ACTION_BATTLE_START						= 13,		//战斗开始
	_BATTLE_ACTION_BATTLE_START_SUC					= 14,		//战斗开始
	_BATTLE_ACTION_BATTLE_START_FAIL				= 15,		//战斗开始

	_BATTLE_ACTION_BATTLE_COLLIDE 					= 16,		//战斗中碰撞
	_BATTLE_ACTION_BATTLE_COLLIDE_SUC 				= 17,		//战斗中碰撞
	_BATTLE_ACTION_BATTLE_COLLIDE_FAIL 				= 18,		//战斗中碰撞

	_BATTLE_ACTION_BATTLE_KILL 						= 19,		//战斗中击杀
	_BATTLE_ACTION_BATTLE_KILL_SUC 					= 20,		//战斗中击杀
	_BATTLE_ACTION_BATTLE_KILL_FAIL 				= 21,		//战斗中击杀

	_BATTLE_ACTION_BATTLE_SYN_SCORE					= 22,		//战斗中同步积分
	_BATTLE_ACTION_BATTLE_SYN_SCORE_SUC				= 23,		//战斗中同步积分
	_BATTLE_ACTION_BATTLE_SYN_SCORE_FAIL			= 24,		//战斗中同步积分

	_BATTLE_ACTION_BATTLE_RELIVE					= 25, 		//战斗中复活
	_BATTLE_ACTION_BATTLE_RELIVE_SUC				= 26, 		//战斗中复活
	_BATTLE_ACTION_BATTLE_RELIVE_FAIL				= 27, 		//战斗中复活

	_BATTLE_ACTION_BATTLE_END						= 28,		//战斗结束
	_BATTLE_ACTION_BATTLE_END_SUC					= 29,		//战斗结束
	_BATTLE_ACTION_BATTLE_END_FAIL					=30,		//战斗结束

	_BATTLE_ACTION_VIEW_AWARD						=31,		//战斗结束界面 看视频三倍奖励
	_BATTLE_ACTION_VIEW_AWARD_SUC					=32,		//战斗结束界面 看视频三倍奖励
	_BATTLE_ACTION_VIEW_AWARD_FAIL					=33,		//战斗结束界面 看视频三倍奖励

	_BATTLE_ACTION_TEAM_START						=34,		//红蓝对战中战斗开始
	_BATTLE_ACTION_TEAM_START_SUC					=35,		//红蓝对战中战斗开始
	_BATTLE_ACTION_TEAM_START_FAIL					=36,		//红蓝对战中战斗开始

	_BATTLE_ACTION_TEAM_END							=37,		//红蓝对战中战斗结束
	_BATTLE_ACTION_TEAM_END_SUC						=38,		//红蓝对战中战斗结束
	_BATTLE_ACTION_TEAM_END_FAIL					=39,		//红蓝对战中战斗结束

	_BATTLE_ACTION_TEAM_SYN_SCORE					=40,		//红蓝对战中同步队伍积分、击杀方车辆等级
	_BATTLE_ACTION_TEAM_SYN_SCORE_SUC				=41,		//红蓝对战中同步队伍积分、击杀方车辆等级
	_BATTLE_ACTION_TEAM_SYN_SCORE_FAIL				=42,		//红蓝对战中同步队伍积分、击杀方车辆等级

	_BATTLE_ACTION_LEAVE_TEAM						=43,		//离开队伍
	_BATTLE_ACTION_LEAVE_TEAM_SUC					=44,		//离开队伍
	_BATTLE_ACTION_LEAVE_TEAM_FAIL					=45,		//离开队伍

	_BATTLE_ACTION_END_AWARD						=46,		//战斗结束同步前端霸主令牌和金币奖励
	_BATTLE_ACTION_END_AWARD_SUC					=47,		//战斗结束同步前端霸主令牌和金币奖励
	_BATTLE_ACTION_END_AWARD_FAIL					=48,		//战斗结束同步前端霸主令牌和金币奖励

	_BATTLE_ACTION_TEAM_RELIVE						=49, 		//战斗中复活-[红蓝战队]
	_BATTLE_ACTION_TEAM_RELIVE_SUC					=50, 		//战斗中复活-[红蓝战队]
	_BATTLE_ACTION_TEAM_RELIVE_FAIL					=51, 		//战斗中复活-[红蓝战队]

	_BATTLE_ACTION_LOADING_COMPLETE     			= 52,       //登陆-[大乱斗]
    _BATTLE_ACTION_LOADING_COMPLETE_SUC 			= 53,       //登陆成功-[大乱斗]
    _BATTLE_ACTION_LOADING_COMPLETE_FAIL			= 54,       //登陆失败-[大乱斗]

    _BATTLE_ACTION_TEAM_LOADING_COMPLETE        	= 55,       //登陆-[红蓝战队]
    _BATTLE_ACTION_TEAM_LOADING_COMPLETE_SUC    	= 56,       //登陆成功-[红蓝战队]
	_BATTLE_ACTION_TEAM_LOADING_COMPLETE_FAIL   	= 57,       //登陆失败-[红蓝战队]
	
	_BATTLE_ACTION_TEAM_VIEW_AWARD					= 58,		//战斗结束界面 看视频三倍奖励-[红蓝对战]
    _BATTLE_ACTION_TEAM_VIEW_AWARD_SUC				= 59,		//战斗结束界面 看视频三倍奖励成功-[红蓝对战]
	_BATTLE_ACTION_TEAM_VIEW_AWARD_FAIL				= 60,		//战斗结束界面 看视频三倍奖励失败-[红蓝对战]
	
	_BATTLE_ACTION_CHANGE_TEAM_LEADER			= 61,		//队长变更-[大乱斗]-[红蓝对战]
    _BATTLE_ACTION_CHANGE_TEAM_LEADER_SUC		= 62,		//队长变更-[大乱斗]-[红蓝对战]
	_BATTLE_ACTION_CHANGE_TEAM_LEADER_FAIL		= 63,		//队长变更-[大乱斗]-[红蓝对战]
	
	_BATTLE_ACTION_INVITE_FRIEND_JOIN			= 64,		//邀请好友加入队伍-[大乱斗]-[红蓝对战]
};

export enum Msg_Walk_Type
{
	_WALK_ACTION_USER_CMD	 				= 1,		//用户操作
	_WALK_ACTION_CONTINUE_PREVIOUS_CMD		= 2,		//空桢继续上一个操作命令，服务器66ms = 前端33ms * 2
	_WALK_ACTION_USER_COLLIDE_CMD			= 3,		//碰撞
	_WALK_ACTION_REBOUND_CMD				= 4,		//反弹
	_WALK_ACTION_CONFUSION_RELIVE_CMD		= 5,		//继续战斗复活-[大乱斗]
	_WALK_ACTION_TEAM_RELIVE_CMD			= 6,		//继续战斗复活-[红蓝战队]
	_WALK_ACTION_CONFUSION_CAR_LEV_CHANGE	= 7,		//玩家车辆等级升级-[大乱斗]
	_WALK_ACTION_TEAM_CAR_LEV_CHANGE		= 8,		//玩家车辆等级升级-[红蓝战队]
	_WALK_ACTION_KILL						= 9,		//击杀-[大乱斗]-[红蓝战队]
};


//道具包
export enum Msg_Item_Type
{
	_ITEM_ACTION_CREATE_RANDOM_ITEM_POS 			= 1,		//获取随机点
	_ITEM_ACTION_CREATE_RANDOM_ITEM					= 2,		//生成随机道具	C->S
	_ITEM_ACTION_PICKUP_RANDOM_ITEM					= 3,		//删除道具	C->S

	_ITEM_ACTION__ADD								= 4,		// 添加道具
	_ITEM_ACTION__DEL								= 5,		// 删除道具
	_ITEM_ACTION__UPDATE							= 6,		// 更新道具(全量)
	_ITEM_ACTION__USE								= 7,		// 使用某个道具
};		

//同步房间信息		
export enum Msg_Syn_Room_Type
{		
	SYNROOMINFO_ACTION_ADD_MEMBER 					= 1,		//添加成员
	SYNROOMINFO_ACTION_DEL_MEMBER 					= 2,		//删除成员
};		

//商城包		
export enum Msg_Vas_Type
{
	BUY         									= 1, 		//购买道具
	BUY_SUCCESS										= 2,		//购买成功
	BUY_FAIL										= 3,		//购买失败

	_VAS_SHOP_BUY									= 4,		//商城买道具-------商城
	_VAS_SHOP_BUY_SUC								= 5,		//商城买道具成功---商城
	_VAS_SHOP_BUY_FAIL								= 6,		//商城买道具失败---商城
};									

//兑换包		
export enum Msg_Exchange_Type
{		
    EXCHANGE_ITEM 	= 1,		//兑换道具		
};		

//action请求		
export enum Msg_Action_Type
{		
	_ACTION_GO_BACK_HOME_SCREEN						= 1,		//大乱斗战斗结束后返回游戏主界面
	_ACTION_GO_BACK_HOME_SCREEN_SUC					= 2,		//大乱斗战斗结束后返回游戏主界面成功
	_ACTION_GO_BACK_HOME_SCREEN_FAIL				= 3,		//大乱斗战斗结束后返回游戏主界面失败

	_ACTION_SIGNIN_BOX_RECEIVE						= 4,		//领取签到宝箱
	_ACTION_SIGNIN_BOX_RECEIVE_SUC					= 5,		//领取签到宝箱成功
	_ACTION_SIGNIN_BOX_RECEIVE_FAIL					= 6,		//领取签到宝箱失败

	_ACTION_LIMIT_TIME_AWARD						= 7, 		//限时领奖
	_ACTION_LIMIT_TIME_AWARD_SUC					= 8,		//限时领奖成功
	_ACTION_LIMIT_TIME_AWARD_FAIL					= 9,		//限时领奖失败

	_ACTION_ONLINE_AWARD							= 10,		//在线领奖
	_ACTION_ONLINE_AWARD_SUC						= 11,		//在线领奖成功
	_ACTION_ONLINE_AWARD_FAIL						= 12,		//在线领奖失败

	_ACTION_ONLINE_SPEED_UP							= 13,		//在线加速
	_ACTION_ONLINE_SPEED_UP_SUC						= 14,		//在线加速成功
	_ACTION_ONLINE_SPEED_UP_FAIL					= 15,		//在线加速失败

	_ACTIOIN_LIMIT_TIME_CONTINUE_TRY				= 16,		//显示免费皮肤继续使用
	_ACTIOIN_LIMIT_TIME_CONTINUE_TRY_SUC			= 17,		//显示免费皮肤继续使用成功
	_ACTIOIN_LIMIT_TIME_CONTINUE_TRY_FAIL			= 18,		//显示免费皮肤继续使用失败



	_ACTION_ADD_APP_LIST							= 25,		//添加到小程序列表
	_ACTION_ADD_APP_LIST_SUC						= 26,		//添加到小程序列表成功
	_ACTION_ADD_APP_LIST_FAIL						= 27,		//添加到小程序列表失败
};

//好友系统
export enum Msg_Friend_Type
{
	_FRIEND_APPLY									= 1,		//申请好友
	_FRIEND_ACCEPT									= 2,		//接受请求
	_FRIEND_GETLIST									= 4,		//获取好友信息
};

//好友信息
export enum Msg_Friend_Info_Type
{
	_FRIEND_INFO_ADD					= 1,		//添加好友
	_FRIEND_INFO_DEL					= 2,		//删除请求
	_FRIEND_INFO_UPDATE					= 3,		//更新好友数据
	_FRIEND_INFO_ONLINE					= 4,		//在线
	_FRIEND_INFO_OFFLINE				= 5,		//离线
	_FRIEND_INFO_GETLIST				= 6,		//好友列表信息
};

//零碎消息包
export enum Msg_Common_Type
{
	//废弃
	_BUMPER_TRANSFER_GET_USER_INFO                  = 1,    //获取用户数据 
    _BUMPER_TRANSFER_GET_USER_INFO_SUC              = 2,
    _BUMPER_TRANSFER_GET_USER_INFO_FAIL             = 3,
    
	//废弃
    _BUMPER_TRANSFER_UPLOAD_USER_DATA_GAMEOVER      = 4,    //上传游戏结束数据
    _BUMPER_TRANSFER_UPLOAD_USER_DATA_GAMEOVER_SUC  = 5,
    _BUMPER_TRANSFER_UPLOAD_USER_DATA_GAMEOVER_FAIL = 6,

	//废弃
    _BUMPER_TRANSFER_UNLOCK_CAR                     = 7,    //解锁车辆
    _BUMPER_TRANSFER_UNLOCK_CAR_SUC                 = 8,
    _BUMPER_TRANSFER_UNLOCK_CAR_FAIL                = 9,

    _BUMPER_TRANSFER_INVITE_USER                    = 10,   //邀请用户
    _BUMPER_TRANSFER_INVITE_USER_SUC                = 11,
    _BUMPER_TRANSFER_INVITE_USER_FAIL               = 12,

    _BUMPER_TRANSFER_IS_FRINED_JOIN                 = 13,   //是否有好友加入
    _BUMPER_TRANSFER_IS_FRINED_JOIN_SUC             = 14,
    _BUMPER_TRANSFER_IS_FRINED_JOIN_FAIL            = 15,

    _BUMPER_TRANSFER_USE_CAR                        = 16,   //记录使用中的车辆
    _BUMPER_TRANSFER_USE_CAR_SUC                    = 17,
    _BUMPER_TRANSFER_USE_CAR_FAIL                   = 18,

    _BUMPER_TRANSFER_RANK_GET_GRANDING_STARNUM      = 19,   //根据名次获取当前段位和等级
    _BUMPER_TRANSFER_RANK_GET_GRANDING_STARNUM_SUC  = 20,
    _BUMPER_TRANSFER_RANK_GET_GRANDING_STARNUM_FAIL = 21,

    _BUMPER_TRANSFER_UPLOAD_GOLDNUM_KILLNUM         = 22,   //金币和击杀数据上传
    _BUMPER_TRANSFER_UPLOAD_GOLDNUM_KILLNUM_SUC     = 23, 
    _BUMPER_TRANSFER_UPLOAD_GOLDNUM_KILLNUM_FAIL    = 24, 

    _BUMPER_TRANSFER_GET_NOT_DECREASE_STAR_GIFT     = 25,   //领取不减星礼包
    _BUMPER_TRANSFER_GET_NOT_DECREASE_STAR_GIFT_SUC = 26,
    _BUMPER_TRANSFER_GET_NOT_DECREASE_STAR_GIFT_FAIL= 27,

    _BUMPER_TRANSFER_GET_BIG_GIFT                   = 28,   //领取大礼包
    _BUMPER_TRANSFER_GET_BIG_GIFT_SUC               = 29,
    _BUMPER_TRANSFER_GET_BIG_GIFT_FAIL              = 30,

	_BUMPER_TRANSFER_RANKING_LIST_BY_KILLNUM        = 31,   //排行榜 by 击杀数
    _BUMPER_TRANSFER_RANKING_LIST_BY_KILLNUM_SUC    = 32,
    _BUMPER_TRANSFER_RANKING_LIST_BY_KILLNUM_FAIL   = 33,
    
    _BUMPER_TRANSFER_RANKING_LIST_BY_RANK           = 34,   //排行榜 by 段位
    _BUMPER_TRANSFER_RANKING_LIST_BY_RANK_SUC       = 35,
	_BUMPER_TRANSFER_RANKING_LIST_BY_RANK_FAIL      = 36,
	

	_BUMPER_TRANSFER_TAIL                           = 37,   //设置拖尾
    _BUMPER_TRANSFER_TAIL_SUC                       = 38,
    _BUMPER_TRANSFER_TAIL_FAIL                      = 39,
};
/////////////////////小消息id---End////////////////////////