//////////////////////////////////////////////////////////////////////
//文件名称：Config.h
//功能描述：配置文件相关
//版本说明：Config API
//			
//编写作者：2017.08.21 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#ifndef __CONFIG_H__
#define __CONFIG_H__

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include "Type.h"
#include "Common.h"

//Server
struct _SERVER_DATA
{
	ID_t			m_ServerID;
	ID_t			m_MachineID;
	CHAR			m_IP0[IP_SIZE];
	UINT			m_Port0;
	CHAR			m_IP1[IP_SIZE];
	UINT			m_Port1;
	INT				m_Type;
	BOOL			m_EnableShareMem;


	_SERVER_DATA()
	{
		Init();
	};
	VOID Init()
	{
		m_ServerID = INVALID_ID;
		m_MachineID = INVALID_ID;
		memset(m_IP0, 0, IP_SIZE);
		m_Port0 = 0;
		memset(m_IP1, 0, IP_SIZE);
		m_Port1 = 0;
		m_Type = -1;
		m_EnableShareMem = FALSE;
	}
};
struct _SERVER_WORLD
{
	CHAR			m_IP[IP_SIZE];
	UINT			m_Port;

	_SERVER_WORLD()
	{
		Init();
	}
	VOID Init()
	{
		memset(m_IP, 0, IP_SIZE);
		m_Port = 0;
	}
};



#define OVER_MAX_SERVER 256
struct _SERVER_INFO
{
	_SERVER_DATA*	m_pServer;	
	UINT			m_ServerCount;
	INT				m_HashServer[OVER_MAX_SERVER];
	_SERVER_WORLD	m_World;

	_SERVER_INFO()
	{
		m_pServer = NULL;
		m_ServerCount = 0;
		for(INT i=0; i<OVER_MAX_SERVER; i++ )
		{
			m_HashServer[i] = -1;
		}
	}
	~_SERVER_INFO()
	{
		SAFE_DELETE_ARRAY(m_pServer);
		m_ServerCount = 0; 
	}
};

//Client
struct _CLIENT_DATA
{
	ID_t			m_ServerID;
	ID_t			m_MachineID;
	CHAR			m_IP0[IP_SIZE];
	UINT			m_Port0;
	CHAR			m_IP1[IP_SIZE];
	UINT			m_Port1;
	INT				m_Type;
	BOOL			m_EnableShareMem;


	_CLIENT_DATA()
	{
		Init();
	};
	VOID Init()
	{
		m_ServerID = INVALID_ID;
		m_MachineID = INVALID_ID;
		memset(m_IP0, 0, IP_SIZE);
		m_Port0 = 0;
		memset(m_IP1, 0, IP_SIZE);
		m_Port1 = 0;
		m_Type = -1;
		m_EnableShareMem = FALSE;
	}
};


//////////////////////////////////////////////////////////////////////
struct ST_CONFIG{
	CHAR 	ONLINE_SERVER_NAME[_MAX_NAMESIZE];
	CHAR	ONLINE_IP[IPSTR_SIZE];
	INT		ONLINE_PORT;
	INT		ONLINE_DEFAULT_INPUT_BUFFER_SIZE;
	INT		ONLINE_DISCONNECT_INPUT_BUFFER_SIZE;
	INT		ONLINE_DEFAULT_OUTPUT_BUFFER_SIZE;
	INT		ONLINE_DISCONNECT_OUTPUT_BUFFER_SIZE;
	INT 	ONLINE_SND_SIZE;
	INT 	ONLINE_RCV_SIZE;
	INT 	ONLINE_REGIST_INDEX;
	INT 	ONLINE_ULIMIT_N;
	INT 	ONLINE_LOG_OPEN;
	INT 	ONLINE_BIND_PORT_CONNECT_ACC;
	INT 	ONLINE_BIND_PORT_CONNECT_MTS;
	INT 	ONLINE_BIND_PORT_CONNECT_PW;
	INT     ONLINE_HEART_BEAT_DETECTION_USER;
	INT     ONLINE_HEART_BEAT_DETECTION;
	INT     ONLINE_REPORT_INTERVAL_TIME;
	INT     ONLINE_RECONNECT_INTERVAL_TIME_TO_ACC;
	INT     ONLINE_RECONNECT_INTERVAL_TIME_TO_MTS;
	INT     ONLINE_RECONNECT_INTERVAL_TIME_TO_PW;
	INT 	ONLINE_MAPGROUP_SIZE;
	CHAR	ONLINE_SOCK_LISTEN_ADDR[IPSTR_SIZE];
	INT 	ONLINE_LISTEN_IPV_MODULE;
	INT 	ONLINE_BROADCAST_FRAME_MSG_CACHE;
	INT 	ONLINE_BROADCAST_FRAME_MSG_INTERVAL_TIME;
	INT     ONLINE_BROADCAST_COLLIDE_SYN_MSG_COUNT;
	INT     ONLINE_BROADCAST_COLLIDE_MSG_INTERVAL_TIME;
	INT     ONLINE_MAP_COLLIDE_SIZE;
	INT     ONLINE_MAP_DEAD_SIZE;
	INT     ONLINE_MAX_FRIEND_AMOUNT;

	INT 	IO_MODEL_SELECT;
	INT 	IO_MODEL_POLL;
	INT 	IO_MODEL_EPOLL;
	CHAR	PW_IP[IPSTR_SIZE];
	INT		PW_PORT;
	INT 	PW_SND_SIZE;
	INT 	PW_RCV_SIZE;
	INT 	PW_LOG_OPEN;
	INT 	PW_CONNECT_OPEN;
	CHAR	MTS_IP[IPSTR_SIZE];
	INT		MTS_PORT;
	INT 	MTS_SND_SIZE;
	INT 	MTS_RCV_SIZE;
	INT 	MTS_LOG_OPEN;
	INT 	MTS_CONNECT_OPEN;
	CHAR	ACC_IP[IPSTR_SIZE];
	INT		ACC_PORT;
	INT 	ACC_SND_SIZE;
	INT 	ACC_RCV_SIZE;
	INT 	ACC_LOG_OPEN;
	INT 	ACC_CONNECT_OPEN;
    //adding connection of mysql information, added by fliu, 2018-10-29
    CHAR	MYSQL_IP[IPSTR_SIZE];
    INT     MYSQL_PORT_;

	CHAR	MYSQL_USERNAME[NAME_SIZE];
	CHAR	MYSQL_USERPASSWD[NAME_SIZE];
    CHAR	MYSQL_DBNAME[NAME_SIZE];	
    //adding connection of mysql information end.
	
	ST_CONFIG()
	{
		Init();
	};
	VOID Init()
	{
		ONLINE_PORT = 0;
		ONLINE_SND_SIZE = 64*1024;
		ONLINE_RCV_SIZE = 16*1024;
		ONLINE_DEFAULT_INPUT_BUFFER_SIZE = 16*1024;
		ONLINE_DISCONNECT_INPUT_BUFFER_SIZE = 32*1024;
		ONLINE_DEFAULT_OUTPUT_BUFFER_SIZE = 16*1024;
		ONLINE_DISCONNECT_OUTPUT_BUFFER_SIZE = 32*1024;
		ONLINE_REGIST_INDEX = 0;
		ONLINE_ULIMIT_N = 0;
		ONLINE_LOG_OPEN = 0;
		ONLINE_BIND_PORT_CONNECT_ACC = 0;
		ONLINE_BIND_PORT_CONNECT_MTS = 0;
		ONLINE_BIND_PORT_CONNECT_PW = 0;
		ONLINE_HEART_BEAT_DETECTION_USER = 30;
		ONLINE_HEART_BEAT_DETECTION = 30;
		ONLINE_REPORT_INTERVAL_TIME = 10;
		ONLINE_RECONNECT_INTERVAL_TIME_TO_ACC = 30;
		ONLINE_RECONNECT_INTERVAL_TIME_TO_MTS = 30;
		ONLINE_RECONNECT_INTERVAL_TIME_TO_PW = 30;
		ONLINE_MAPGROUP_SIZE = 1;
		memset(ONLINE_IP, 0, IPSTR_SIZE);
		memset(ONLINE_SOCK_LISTEN_ADDR, 0, IPSTR_SIZE);
		memset(ONLINE_SERVER_NAME, 0, _MAX_NAMESIZE);

		ONLINE_LISTEN_IPV_MODULE = 0;
		ONLINE_BROADCAST_FRAME_MSG_CACHE = 0;
		ONLINE_BROADCAST_FRAME_MSG_INTERVAL_TIME = 66;
		ONLINE_BROADCAST_COLLIDE_SYN_MSG_COUNT = 5;
	    ONLINE_BROADCAST_COLLIDE_MSG_INTERVAL_TIME = 66;
	    ONLINE_MAP_COLLIDE_SIZE = 0;
	    ONLINE_MAP_DEAD_SIZE = 0;
	    ONLINE_MAX_FRIEND_AMOUNT = 100;

		IO_MODEL_SELECT = 0;
		IO_MODEL_POLL = 0;
		IO_MODEL_EPOLL = 0;	

		PW_PORT = 0;
		PW_SND_SIZE = 64*1024;
		PW_RCV_SIZE = 16*1024;
		PW_LOG_OPEN = 0;
		memset(PW_IP, 0, IPSTR_SIZE);
		PW_CONNECT_OPEN = 0;

		MTS_PORT = 0;
		MTS_SND_SIZE = 64*1024;
		MTS_RCV_SIZE = 16*1024;
		MTS_LOG_OPEN = 0;
		memset(MTS_IP, 0, IPSTR_SIZE);
		MTS_CONNECT_OPEN = 0;
		
		ACC_PORT = 0;
		ACC_SND_SIZE = 64*1024;
		ACC_RCV_SIZE = 16*1024;
		memset(ACC_IP, 0, IPSTR_SIZE);
		ACC_LOG_OPEN = 0;
		ACC_CONNECT_OPEN = 0;
	
		//for (int i = 0; i < IP_SIZE; ++i)
		//{
			//LOGDEBUG("-%c-\n", PW_IP[i]);
		//}

		//LOGDEBUG("-PW_IP=%s-MTS_IP=%s-\n", PW_IP,MTS_IP);

        //adding connection of mysql information, added by fliu, 2018-10-29
           memset(MYSQL_IP, 0, IPSTR_SIZE);
           MYSQL_PORT_ = 0;
           memset(MYSQL_USERNAME, 0, NAME_SIZE);
           memset(MYSQL_USERPASSWD, 0, NAME_SIZE);
           memset(MYSQL_DBNAME, 0, NAME_SIZE);
        //adding connection of mysql information end.


        
	}
};
extern  ST_CONFIG	g_config;

struct RobotConf
{
	char		name[NAME_SIZE];
	UInt16		id;
	UInt16 		name_id;
	UInt16 		type;
	UInt16 		age;
	UInt16 		gender;
	UInt16 		intellectual;
	UInt16 		beautiful;
	UInt16 		lovely;
	UInt16 		skin_state;
	UInt16 		long_physical;
	UInt16 		short_physical;
	UInt16 		energy;
	UInt16 		thirsty;
	UInt16 		hunger;
	UInt16 		clean;
	UInt16 		surface_temperature;
	UInt32 		account;
    UInt16      bornArea;

    RobotConf() {};
};

extern RobotConf g_robotConf_NICO;


#endif//__CONFIG_H__
