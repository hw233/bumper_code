#ifndef _COMMAND_H_
#define _COMMAND_H_

//////////////////////////////////////////////////// Command ////////////////////////////////////////////////////
enum Command
{
	// req to Account 		[0 - 199]
    ACC_ADDUSER_REQ			=	0,
    ACC_CHECKUSER_REQ		=	1,
	ACC_LOGIN_REQ			=	2,
	ACC_CHECKUNAME_REQ		=	3,
	// Acc and GameServer
	ACC_LOGOUT_REQ			=	4,
	ACC_KICKOFF_REQ			=	5,
	ACC_CHECKTOKEN_REQ		=	6,

	// resp from Accout
    ACC_ADDUSER_RESP		=	100,
    ACC_CHECKUSER_RESP		=	101,
	ACC_LOGIN_RESP 			= 	102,
	ACC_CHECKUNAME_RESP		= 	103,
	// Acc and GameServer
	ACC_LOGOUT_RESP			= 	104,
	ACC_KICKOFF_RESP		= 	105,
	ACC_CHECKTOKEN_RESP		= 	106,


	// Client and GameServer	[200 - 399]
	// Client to GameServer
	C2GS_LOGIN				=	200,
	C2GS_LOGOUT				=	201,
	C2GS_KICK_OFF 			=	202,
	C2GS_CREATE_ROBOT		=	203,
	C2GS_ROBOT_ONLINE		=	204,
	//C2GS_GET_SCENE_ID	 	=	205,
	//C2GS_CREATE_SCENE		=	206,
	//C2GS_ENTER_SCENE		=	207,
	// GameServer to Client
	GS2C_LOGIN				=	300,
	GS2C_LOGOUT				=	301,
	GS2C_KICK_OFF 			=	302,
	GS2C_CREATE_ROBOT		=	303,
	GS2C_ROBOT_ONLINE		=	304,
	//GS2C_GET_SCENE_ID		=	305,
	//GS2C_CREATE_SCENE		=	306,
	//GS2C_ENTER_SCENE		=	307,


	// GameServer and PW 		[400 - 599]
	// GameServer to PW

	GS2PW_ROBOT_ONLINE 		= 	400,
	GS2PW_ROBOT_OFFLINE 	=	401,
	GS2PW_CREATE_ROBOT		=	402,
	GS2PW_ALL_WORLD_DATA	=	403,
	GS2PW_GET_PW_DATA		=	404,
	// PW to GameServer
	PW2GS_ROBOT_ONLINE		=	500,
	PW2GS_ROBOT_OFFLINE		=	501,
	PW2GS_CREATE_ROBOT		=	502,
	PW2GS_ALL_WORLD_DATA	=	503,
	PW2GS_GET_PW_DATA		=	504,


	// GameServer and MTS		[600 - 799]
	// GameServer to MTS
	GS2MTS_ROBOT_ONLINE		=	600,
	GS2MTS_ROBOT_OFFLINE	=	601,
	GS2MTS_GET_ROBOT_LIST	=	602, // 机器人查询
	GS2MTS_CREATE_ROBOT		=	603, // 创建机器人
//	GS2MTS_CREATE_SCENE		=	604, // 创建场景
//	GS2MTS_GET_SCENE_ITEM 	= 	605, // Get Scene Item
//	GS2MTS_ADD_SCENE_ITEM 	=	606, // 创建场景物品
//	GS2MTS_DEL_SCENE_ITEM 	=	607, // 删除场景物品
	// MTS to GameServer
	MTS2GS_GET_ROBOT_LIST	=	700, // 机器人查询回复
	MTS2GS_CREATE_ROBOT		=	701, // 创建机器人回复
//	MTS2GS_CREATE_SCENE		= 	702, // 创建场景回复
//	MTS2GS_GET_SCENE_ITEM 	= 	703, // Get Scene Item
//	MTS2GS_ADD_SCENE_ITEM	=	704, // 单独创建物品的回复


	// TES Command 				[800 - 999]
	// Req
	TES2MTS_DEAL_M_NODE 	=	800,
    C2MTS_AICHAT            =   801,  // 客户端发送的自然语言消息包
    AS2MTS_CLIENT_LOGIN		=   802,  // AIserver发送的客户端登录,主要是broker用于路由.通过robotID和套接字建立关系,用于MTS主动发送消息给客户端
    C2MTS_STATE             =   803,  // 客户端上传的机器人状态,在说,没在说等

    // Resp
	MTS2TES_EXPRESSION_NODE	=	900,
    MTS2C_AICHAT            =   901,  // MTS给客户端的自然语言数据
    MTS2AS_CLIENT_LOGIN		=   902,  // MTS回复给AIserver的消息包,

    //Client to GameServer
    _MSG_LOGIN	    		= 2001,		//登录包
    _MSG_USERINFO    		= 2002,		//角色信息包
    _MSG_SYN_ROOM    		= 2003,		//同步房间信息包
    _MSG_BATTLE      		= 2004, 	//战斗包
    _MSG_VAS         		= 2005,		//商城包
    _MSG_ACTION      		= 2006,		//action包
    _MSG_ITEM   			= 2007,		//物品包
    _MSG_EXCHANGE    		= 2008,		//兑换道具包
    _MSG_WALK	    		= 2009,		//走路包
    _MSG_TASKDETAIL			= 2010,		//任务包
    _MSG_SIGNUP	    		= 2011,		//签到系统 包   
    _MSG_BUMPER_TRANSFER    = 2012,     //Bumper迁移PHP代码至C++
    _MSG_FRIEND				= 2013,		//好友系统包
    _MSG_FRIEND_INFO		= 2014,		//好友信息包
    _MSG_TOPLIST			= 2015,		//排行榜包
    _MSG_USER_ATTRIB 		= 2016,		//用户属性包
    _MSG_ITEM_ATTRIB		= 2017,		//物品属性包
    _MSG_TICK				= 2018,		//tick包

    _MSG_TEST				= 5000,		//自测包

	// Common Protocol
	COMMON_SERVER_REG_REQ	=	9997,
	COMMON_SERVER_REG_RESP	=	9998,
	COMMON_HEART_BEAT		=	9999,

	//log
	C2Server_LOG			= 	10002, //add by huyf 2018.08.33:从客户端发送日志信息到服务器GameServer

    MTS_BROKER_SUBCRIBE     =   65533, //MTS broker程序worker订阅消息的命令.
};


//////////////////////////////////////////////////// Command ////////////////////////////////////////////////////
enum ServerType
{
	SERVER_TYPE_GC			=	0, // Game 	Client
	SERVER_TYPE_ACC 		=	1, // Acc 	Server
	SERVER_TYPE_OL			=	2, // OnLine Server
	SERVER_TYPE_MTS			=	3, // MTS 	Server
	SERVER_TYPE_PW			=	4, // PW 	Server		
};

#endif // _COMMAND_H_
