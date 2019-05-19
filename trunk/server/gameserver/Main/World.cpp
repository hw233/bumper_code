//////////////////////////////////////////////////////////////////////
//文件名称：World.cpp
//功能描述：世界服务器
//版本说明：World API
//
//编写作者：2017.08.22 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//include头文件
//////////////////////////////////////////////////////////////////////
#include "World.h"
#include "../Base/TimeManager.h"
#include "../Base/Log.h"
#include "../Base/logger.h"
#include "ServerManager.h"
#include "ClientManager.h"
#include "../Common/Config.h"
#include "PlayerPool.h"
#include "PacketFactoryManager.h"
#include "../Common/IniFile.h"

#include "./TopList/TopListManager.h"

//add by huyf 2018.04.03:多线程架构
//#include "../Comm/MessagePort.h"
//#include "../Thread/SocketThread/SocketThread.h"
//#include "../Share/I_Shell.h"
//add end:多线程架构

#if defined(__LINUX__)
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <errno.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#include "BattleManager.h"
#include "./Battle/BattleCfg.h"
#include "./Item/ItemAwardManager.h"
#include "./Item/ItemAwardGroupManager.h"
#include "./OnLineAward/OnLineAwardCfgManager.h"
#include "./TaskDetail/TaskDetailCfgManager.h"

//日志开关标志位
BOOL g_bLogSave = FALSE;


//////////////////////////////////////////////////////////////////////
//全局变量
//////////////////////////////////////////////////////////////////////
World g_World;
//add by huyf 2018.04.03:多线程架构
//struct STAT_STRUCT  g_stat = {0};
//long                g_nRestart      = false;
//long                g_nServerClosed = false;
//long                s_nDatabaseTimeSum;
//long g_nAllPlayers = 0;
//add end:多线程架构
//
// main函数
//
int main(int argc, char* argv[])
{
    DEBUG_TRY
    Logger::Instance().Init("online", DEBUG);

    //读取配置文件
    //==========================Title-Log=======================================
    CIniFile    ini("../Config/OnLineConfig.ini", "OnLineServer");
    g_config.ONLINE_REGIST_INDEX = ini.GetInt("REGIST_INDEX");
    g_config.ONLINE_SND_SIZE = ini.GetInt("SND_SIZE");
    g_config.ONLINE_RCV_SIZE = ini.GetInt("RCV_SIZE");
    g_config.ONLINE_DEFAULT_INPUT_BUFFER_SIZE = ini.GetInt("DEFAULT_INPUT_BUFFER_SIZE");
    g_config.ONLINE_DISCONNECT_INPUT_BUFFER_SIZE = ini.GetInt("DISCONNECT_INPUT_BUFFER_SIZE");
    g_config.ONLINE_DEFAULT_OUTPUT_BUFFER_SIZE = ini.GetInt("DEFAULT_OUTPUT_BUFFER_SIZE");
    g_config.ONLINE_DISCONNECT_OUTPUT_BUFFER_SIZE = ini.GetInt("DISCONNECT_OUTPUT_BUFFER_SIZE");
    g_config.ONLINE_HEART_BEAT_DETECTION_USER = ini.GetInt("HEART_BEAT_DETECTION_USER");
    g_config.ONLINE_HEART_BEAT_DETECTION = ini.GetInt("HEART_BEAT_DETECTION");
    g_config.ONLINE_REPORT_INTERVAL_TIME = ini.GetInt("REPORT_INTERVAL_TIME");
    g_config.ONLINE_RECONNECT_INTERVAL_TIME_TO_ACC = ini.GetInt("RECONNECT_INTERVAL_TIME_TO_ACC");
    g_config.ONLINE_RECONNECT_INTERVAL_TIME_TO_MTS = ini.GetInt("RECONNECT_INTERVAL_TIME_TO_MTS");
    g_config.ONLINE_RECONNECT_INTERVAL_TIME_TO_PW = ini.GetInt("RECONNECT_INTERVAL_TIME_TO_PW");
    //add by huyf 2018.04.03:多线程架构
	//g_config.ONLINE_MAPGROUP_SIZE = ini.GetInt("MAPGROUP_SIZE");
	//add end:多线程架构
    g_config.ONLINE_ULIMIT_N = ini.GetInt("ULIMIT_N");
    g_config.ONLINE_LOG_OPEN = ini.GetInt("LOG_OPEN");
    g_config.ONLINE_PORT = ini.GetInt("PORT");
    g_config.ONLINE_BIND_PORT_CONNECT_ACC = ini.GetInt("BIND_PORT_CONNECT_ACC");
    g_config.ONLINE_BIND_PORT_CONNECT_MTS = ini.GetInt("BIND_PORT_CONNECT_MTS");
    g_config.ONLINE_BIND_PORT_CONNECT_PW = ini.GetInt("BIND_PORT_CONNECT_PW");
	//add by huyf 2018.04.03:多线程架构
	//地址可以配置
    ini.GetString(g_config.ONLINE_IP, "IP", IPSTR_SIZE);
    //ini.GetString(g_config.ONLINE_SOCK_LISTEN_ADDR, "SOCK_LISTEN_ADDR", IPSTR_SIZE);
	//add end:多线程架构
    //!alter by huyf 2018.08.01:支持双栈支持IPV4和IPV6 
    g_config.ONLINE_LISTEN_IPV_MODULE = ini.GetInt("LISTEN_IPV_MODULE");
    //!alter end:支持双栈支持IPV4和IPV6 
    //!alter by huyf 2018.11.07:广播同步帧缓存66ms 
    g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE = ini.GetInt("BROADCAST_FRAME_MSG_CACHE");
    g_config.ONLINE_BROADCAST_FRAME_MSG_INTERVAL_TIME = ini.GetInt("BROADCAST_FRAME_MSG_INTERVAL_TIME");
    //!alter end:广播同步帧缓存66ms 
    //!//!alter by huyf 2018.11.07:;碰撞后服务器主动广播碰撞行为的消息包数,默认：5
    g_config.ONLINE_BROADCAST_COLLIDE_SYN_MSG_COUNT = ini.GetInt("BROADCAST_COLLIDE_SYN_MSG_COUNT");
    g_config.ONLINE_BROADCAST_COLLIDE_MSG_INTERVAL_TIME = ini.GetInt("BROADCAST_COLLIDE_MSG_INTERVAL_TIME");
    //!alter end:;碰撞后服务器主动广播碰撞行为的消息包数,默认：5 
    //!//!alter by huyf 2018.11.07:缓存队列的长度（碰撞信息包、死亡信息包）
    g_config.ONLINE_MAP_COLLIDE_SIZE = ini.GetInt("MAP_COLLIDE_SIZE");
    g_config.ONLINE_MAP_DEAD_SIZE = ini.GetInt("MAP_DEAD_SIZE");
    //!alter end:缓存队列的长度（碰撞信息包、死亡信息包）
    //!alter by huyf 2018.11.23:好友系统
   g_config.ONLINE_MAX_FRIEND_AMOUNT = ini.GetInt("MAX_FRIEND_AMOUNT"); 
    //!alter end:好友系统
    ini.GetString(g_config.ONLINE_SERVER_NAME, "SERVER_NAME", _MAX_NAMESIZE);    

    if(g_config.ONLINE_LOG_OPEN == 1)
    {
        g_bLogSave = TRUE;
    }
    else
    if(g_config.ONLINE_LOG_OPEN == 0)
    {
        g_bLogSave = FALSE;
    }

    char strTitle[_MAX_TITLESIZE];
    memset(strTitle, 0, _MAX_TITLESIZE);
    sprintf(strTitle, "%s - %s - ver:%d date:(%s %s)", GAME_TITLE, g_config.ONLINE_SERVER_NAME, int(_VERSION), __DATE__, __TIME__);   
    // init log file
    //CreateDirectory(LOGFILE_DIR);
    mkdir(LOGFILE_DIR, S_IRWXU);
    InitLog(strTitle, LOGFILE_FILENAME, time(NULL));    
    LOGMSG("\n\n\n");
    LOGMSG("=================================================================");
    LOGMSG(strTitle);
    LOGMSG("=================================================================");


    //add by huyf 2018.03.30:多线程架构
    //LOGDEBUG("[Main] LoadConfigIni...");
    //CHECKF(LoadConfigIni())
    //LOGDEBUG("[Main] LoadConfigIni Is Ok...");
    //add end:多线程架构

    LOGMSG("[Main] OnLineServer SERVER_NAME=【%s】 SOCK_LISTEN_ADDR=【%s】 PORT=【%d】 REGIST_INDEX=【%d】 ULIMIT_N=【%d】 LOG_OPEN=【%d】 MAPGROUP_SIZE=【%d】 LISTEN_IPV_MODULE=【%s】", 
        g_config.ONLINE_SERVER_NAME,g_config.ONLINE_IP,g_config.ONLINE_PORT,g_config.ONLINE_REGIST_INDEX,g_config.ONLINE_ULIMIT_N,g_config.ONLINE_LOG_OPEN,g_config.ONLINE_MAPGROUP_SIZE,
        g_config.ONLINE_LISTEN_IPV_MODULE > 0 ? "IPV6" : "IPV4");

    if (g_config.ONLINE_PORT <= 1024)
    {
        g_config.ONLINE_PORT = 5505;
    }
    //如果配置错误则最大缓存值为默认值的2倍
    if (g_config.ONLINE_DEFAULT_INPUT_BUFFER_SIZE == 0)
    {
        g_config.ONLINE_DEFAULT_INPUT_BUFFER_SIZE = 4*1024;
    }
    if (g_config.ONLINE_DEFAULT_OUTPUT_BUFFER_SIZE == 0)
    {
        g_config.ONLINE_DEFAULT_OUTPUT_BUFFER_SIZE = 4*1024;
    }
    if (g_config.ONLINE_DISCONNECT_INPUT_BUFFER_SIZE <= g_config.ONLINE_DEFAULT_INPUT_BUFFER_SIZE)
    {
        g_config.ONLINE_DISCONNECT_INPUT_BUFFER_SIZE = g_config.ONLINE_DEFAULT_INPUT_BUFFER_SIZE * 2;
    }
    if (g_config.ONLINE_DISCONNECT_OUTPUT_BUFFER_SIZE <= g_config.ONLINE_DEFAULT_OUTPUT_BUFFER_SIZE)
    {
        g_config.ONLINE_DISCONNECT_OUTPUT_BUFFER_SIZE = g_config.ONLINE_DEFAULT_OUTPUT_BUFFER_SIZE * 2;
    }
    if (g_config.ONLINE_HEART_BEAT_DETECTION_USER <= 0)
    {
        g_config.ONLINE_HEART_BEAT_DETECTION_USER = 30;
    }
    if (g_config.ONLINE_HEART_BEAT_DETECTION <= 0)
    {
        g_config.ONLINE_HEART_BEAT_DETECTION = 30;
    }
    if (g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE <= 0)
    {
        g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE = 0;
    }
    if (g_config.ONLINE_BROADCAST_FRAME_MSG_INTERVAL_TIME <= 0)
    {
        g_config.ONLINE_BROADCAST_FRAME_MSG_INTERVAL_TIME = 66;
    }
    if (g_config.ONLINE_BROADCAST_COLLIDE_SYN_MSG_COUNT <= 0)
    {
        g_config.ONLINE_BROADCAST_COLLIDE_SYN_MSG_COUNT = 5;
    }
    if (g_config.ONLINE_BROADCAST_COLLIDE_MSG_INTERVAL_TIME <= 0)
    {
        g_config.ONLINE_BROADCAST_COLLIDE_MSG_INTERVAL_TIME = 66;
    }
    if (g_config.ONLINE_MAP_COLLIDE_SIZE <= 0)
    {
        g_config.ONLINE_MAP_COLLIDE_SIZE = 100;
    }
    if (g_config.ONLINE_MAP_DEAD_SIZE <= 0)
    {
        g_config.ONLINE_MAP_DEAD_SIZE = 100;
    }
    
    LOGMSG("[Main] OnLineServer SND_SIZE=【%d】 RCV_SIZE=【%d】", 
        g_config.ONLINE_SND_SIZE, g_config.ONLINE_RCV_SIZE);

    LOGMSG("[Main] OnLineServer BROADCAST_FRAME_MSG_CACHE=【%d】 BROADCAST_FRAME_MSG_INTERVAL_TIME=【%d】 BROADCAST_COLLIDE_SYN_MSG_COUNT=【%d】 BROADCAST_COLLIDE_MSG_INTERVAL_TIME=【%d】", 
        g_config.ONLINE_BROADCAST_FRAME_MSG_CACHE, g_config.ONLINE_BROADCAST_FRAME_MSG_INTERVAL_TIME, g_config.ONLINE_BROADCAST_COLLIDE_SYN_MSG_COUNT, g_config.ONLINE_BROADCAST_COLLIDE_MSG_INTERVAL_TIME);

    LOGMSG("[Main] OnLineServer MAP_COLLIDE_SIZE=【%d】 MAP_DEAD_SIZE=【%d】 MAX_FRIEND_AMOUNT=【%d】", 
        g_config.ONLINE_MAP_COLLIDE_SIZE, g_config.ONLINE_MAP_DEAD_SIZE, g_config.ONLINE_MAX_FRIEND_AMOUNT);

    LOGMSG("[Main] OnLineServer BIND_PORT_CONNECT_ACC=【%d】 BIND_PORT_CONNECT_MTS=【%d】 BIND_PORT_CONNECT_PW=【%d】 HEART_BEAT_DETECTION_USER=【%d】 HEART_BEAT_DETECTION=【%d】", 
        g_config.ONLINE_BIND_PORT_CONNECT_ACC, g_config.ONLINE_BIND_PORT_CONNECT_MTS, g_config.ONLINE_BIND_PORT_CONNECT_PW, g_config.ONLINE_HEART_BEAT_DETECTION_USER, g_config.ONLINE_HEART_BEAT_DETECTION);

    LOGMSG("[Main] OnLineServer DEFAULT_INPUT_BUFFER_SIZE=【%d】 DISCONNECT_INPUT_BUFFER_SIZE=【%d】 DEFAULT_OUTPUT_BUFFER_SIZE=【%d】 DISCONNECT_OUTPUT_BUFFER_SIZE=【%d】", 
        g_config.ONLINE_DEFAULT_INPUT_BUFFER_SIZE, g_config.ONLINE_DISCONNECT_INPUT_BUFFER_SIZE, g_config.ONLINE_DEFAULT_OUTPUT_BUFFER_SIZE, g_config.ONLINE_DISCONNECT_OUTPUT_BUFFER_SIZE);

    if (g_config.ONLINE_REPORT_INTERVAL_TIME <= 0)
    {
        g_config.ONLINE_REPORT_INTERVAL_TIME = 10;
    }
    if (g_config.ONLINE_RECONNECT_INTERVAL_TIME_TO_ACC <= 0)
    {
        g_config.ONLINE_RECONNECT_INTERVAL_TIME_TO_ACC = 30;
    }
    if (g_config.ONLINE_RECONNECT_INTERVAL_TIME_TO_MTS <= 0)
    {
        g_config.ONLINE_RECONNECT_INTERVAL_TIME_TO_MTS = 30;
    }
    if (g_config.ONLINE_RECONNECT_INTERVAL_TIME_TO_PW <= 0)
    {
        g_config.ONLINE_RECONNECT_INTERVAL_TIME_TO_PW = 30;
    }
    LOGMSG("[Main] OnLineServer REPORT_INTERVAL_TIME=【%d】 RECONNECT_INTERVAL_TIME_TO_ACC=【%d】 RECONNECT_INTERVAL_TIME_TO_MTS=【%d】 RECONNECT_INTERVAL_TIME_TO_PW=【%d】", 
    g_config.ONLINE_REPORT_INTERVAL_TIME, g_config.ONLINE_RECONNECT_INTERVAL_TIME_TO_ACC, g_config.ONLINE_RECONNECT_INTERVAL_TIME_TO_MTS, g_config.ONLINE_RECONNECT_INTERVAL_TIME_TO_PW);

    //=============================CLIENT_MAX_SIZE====================================
#ifdef _SELECT_
    LOGMSG("[Main] I/O Model Start Is = 【 _SELECT_ 】 CLIENT_MAX_SIZE =【%d】", _SETSIZE_); 
#else
    #ifdef _POLL_
        LOGMSG("[Main] I/O Model Start Is = 【 _POLL_ 】 CLIENT_MAX_SIZE =【%d】", _SETSIZE_); 
    #elif _EPOLL_
        LOGMSG("[Main] I/O Model Start Is = 【 _EPOLL_ 】 CLIENT_MAX_SIZE =【%d】", _SETSIZE_); 
    #else 
        LOGMSG("[Main] I/O Model Start Is = 【 ERROR 】 CLIENT_MAX_SIZE =【%d】", _SETSIZE_);  
    #endif
#endif
    
    //=============================ULIMIT_N====================================
    struct rlimit rlim_new;
    if(getrlimit(RLIMIT_NOFILE, &rlim_new) < 0)
            ERR_EXIT("Getrlimit");
    LOGMSG("[Main] Get resource limit for per process =【%d】", (int)rlim_new.rlim_max);
    //如果要改变的话，那么需要我们的重新设置，调用函数setrlimit     
    LOGMSG("[Main] Read OnLineConfig.ini ONLINE_ULIMIT_N=【%d】", g_config.ONLINE_ULIMIT_N);
    if(0)
    {
        rlim_new.rlim_cur = _SETSIZE_;
        rlim_new.rlim_max = _SETSIZE_;
        LOGMSG("[Main] Setrlimit-RLIMIT_NOFILE rlim_new.rlim_cur=【%d】 rlim_new.rlim_max=【%d】", rlim_new.rlim_cur, rlim_new.rlim_max);
        if(setrlimit(RLIMIT_NOFILE, &rlim_new) < 0)
        {
            LOGERROR("[Main] Setrlimit-RLIMIT_NOFILE is error\n");
            ERR_EXIT("setrlimit");
        }
        //重新将修改后的结果打印出来
         LOGMSG("[Main] Getrlimit-RLIMIT_NOFILE rlim_new.rlim_cur=【%d】 rlim_new.rlim_max=【%d】", rlim_new.rlim_cur, rlim_new.rlim_max);
        if(getrlimit(RLIMIT_NOFILE, &rlim_new) < 0)
        {
            LOGERROR("[Main] Getrlimit-RLIMIT_NOFILE is error\n");
            ERR_EXIT("getrlimit");
        }
        /*设置为最大值
        if (getrlimit(RLIMIT_CORE, &rlim)==0) 
        {
            rlim_new.rlim_cur = rlim_new.rlim_max = RLIM_INFINITY;
            if (setrlimit(RLIMIT_CORE, &rlim_new)!=0) 
            {
                rlim_new.rlim_cur = rlim_new.rlim_max = rlim.rlim_max;
                (void) setrlimit(RLIMIT_CORE, &rlim_new);
            }
        }
        */
    }
    
    //=============================IoModel====================================
#if defined(__LINUX__)
    extern int g_IoModelSelect;
    extern int g_IoModelPoll;
    extern int g_IoModelEPoll;
    if(g_IoModelSelect == 0
        &&g_IoModelPoll == 0
        &&g_IoModelEPoll == 0)
    {
        LOGCATCH("[Main] I/O Model Start Is ERROR, Please Select One of I/O Model From ../Config/OnLineConfig.ini I/O_Model Node [SELECT、POLL、EPOLL]");
        return 0;
    }
#endif     

    
#if defined(__WINDOWS__)
    _CrtSetDbgFlag(_CrtSetDbgFlag(0) | _CRTDBG_LEAK_CHECK_DF);
#endif

    
    /*if(argc > 1)
      {
      for(int i=1; i<argc; i++)
      {
      if(strcmp(argv[i],"-ignoreassert")==0)
      {
      g_Command_assert=1;
      }
      if(strcmp(argv[i],"-retryassert")==0)
      {
      g_Command_assert=2;
      }
      else if(strcmp(argv[i],"-ignoremessagebox")==0)
      {
      g_Command_IgnoreMessageBox=TRUE;
      }
      }
      }*/

    
    //从配置文件读取pw-SERVER地址配置信息
    ini.SetSection("PWServer");
    ini.GetString(g_config.PW_IP, "IP", IPSTR_SIZE);
    g_config.PW_PORT = ini.GetInt("PORT");
    g_config.PW_SND_SIZE = ini.GetInt("SND_SIZE");
    g_config.PW_RCV_SIZE = ini.GetInt("RCV_SIZE");
    g_config.PW_LOG_OPEN = ini.GetInt("LOG_OPEN");
    g_config.PW_CONNECT_OPEN = ini.GetInt("CONNECT_OPEN");
    LOGMSG("[Main] Read OnLineConfig.ini OK->PWServer CONNECT_OPEN=[%d] IP=[%s]\tPORT=[%d]\tSND_SIZE=[%d]\tRCV_SIZE=[%d]\tLOG_OPEN=[%d]", g_config.PW_CONNECT_OPEN, g_config.PW_IP, g_config.PW_PORT, g_config.PW_SND_SIZE, g_config.PW_RCV_SIZE,g_config.PW_LOG_OPEN);

    //从配置文件读取mts-SERVER地址配置信息
    ini.SetSection("MTSServer");
    ini.GetString(g_config.MTS_IP, "IP", IPSTR_SIZE);
    g_config.MTS_PORT = ini.GetInt("PORT");
    g_config.MTS_SND_SIZE = ini.GetInt("SND_SIZE");
    g_config.MTS_RCV_SIZE = ini.GetInt("RCV_SIZE");
    g_config.MTS_LOG_OPEN = ini.GetInt("LOG_OPEN");
    g_config.MTS_CONNECT_OPEN = ini.GetInt("CONNECT_OPEN");
    LOGMSG("[Main] Read OnLineConfig.ini OK->MTSServer CONNECT_OPEN=[%d] IP=[%s]\tPORT=[%d]\tSND_SIZE=[%d]\tRCV_SIZE=[%d]\tLOG_OPEN=[%d]", g_config.MTS_CONNECT_OPEN, g_config.MTS_IP, g_config.MTS_PORT, g_config.MTS_SND_SIZE, g_config.MTS_RCV_SIZE,g_config.MTS_LOG_OPEN);

    //从配置文件读取acc-SERVER地址配置信息
    ini.SetSection("AccServer");
    ini.GetString(g_config.ACC_IP, "IP", IPSTR_SIZE);
    g_config.ACC_PORT = ini.GetInt("PORT");
    g_config.ACC_SND_SIZE = ini.GetInt("SND_SIZE");
    g_config.ACC_RCV_SIZE = ini.GetInt("RCV_SIZE");
    g_config.ACC_LOG_OPEN = ini.GetInt("LOG_OPEN");
    g_config.ACC_CONNECT_OPEN = ini.GetInt("CONNECT_OPEN");
    LOGMSG("[Main] Read OnLineConfig.ini OK->ACCServer CONNECT_OPEN=[%d] IP=[%s]\tPORT=[%d]\tSND_SIZE=[%d]\tRCV_SIZE=[%d]\tLOG_OPEN=[%d]", g_config.ACC_CONNECT_OPEN, g_config.ACC_IP, g_config.ACC_PORT, g_config.ACC_SND_SIZE, g_config.ACC_RCV_SIZE,g_config.ACC_LOG_OPEN);

    //从配置文件读取Mysql的配置信息, added by fliu, 2018-10-29
//  ini.GetString(g_config.ACC_IP, "IP", IPSTR_SIZE);

    ini.SetSection("Mysql");
    ini.GetString(g_config.MYSQL_IP, "MYSQL_IP", IPSTR_SIZE);
    g_config.MYSQL_PORT_ = ini.GetInt("MYSQL_PORT_");

    ini.GetString(g_config.MYSQL_USERNAME, "MYSQL_USERNAME", NAME_SIZE);
    
    ini.GetString(g_config.MYSQL_USERPASSWD, "MYSQL_USERPASSWD", NAME_SIZE);
    
    ini.GetString(g_config.MYSQL_DBNAME, "MYSQL_DBNAME", NAME_SIZE);
    
    //从配置文件读取Mysql的配置信息.    
    g_pDbMySql = new DBMySql(g_config.MYSQL_IP, g_config.MYSQL_USERNAME, g_config.MYSQL_USERPASSWD, g_config.MYSQL_DBNAME, g_config.MYSQL_PORT_);
    if (g_pDbMySql == NULL)
    {
        LOGCATCH("[Main] Connect To MySql Is Error... IP=[%s]\tUSERNAME=[%s]\tUSERPASSWD=[%s]\tDBNAME=[%s]\tPORT=[%d]", g_config.MYSQL_IP, g_config.MYSQL_USERNAME, g_config.MYSQL_USERPASSWD, g_config.MYSQL_DBNAME, g_config.MYSQL_PORT_);
    }
    LOGMSG("[Main] Connect To MySql Is Suc... IP=[%s]\tUSERNAME=[%s]\tUSERPASSWD=[%s]\tDBNAME=[%s]\tPORT=[%d]", g_config.MYSQL_IP, g_config.MYSQL_USERNAME, g_config.MYSQL_USERPASSWD, g_config.MYSQL_DBNAME, g_config.MYSQL_PORT_);

    //!alter by huyf 2018.12.04:把g_wechat_user的用户刷新到g_bumper_info表中（涉及到的字段有nickname、avatar_url）
    // std::string sql = string("SELECT id, nickname, avatar_url FROM g_wechat_user WHERE game_id=91");      
    // MYSQL_RES* result = g_pDbMySql->Query(sql);
    // if(result == NULL)
    // {
    //     LOGERROR("[CUserData::LoadInfo] sql error: %s\n", sql.c_str());
    //     return FALSE;
    // }
    // INT nAmount = 0;
    // string strNickName;
    // string strHeadPicurl;    
    // MYSQL_ROW row;
    // while((row = mysql_fetch_row(result)))
    // {    
    //     int id = SafeAtoi(row[0]);        
    //     strNickName = row[1];
    //     strHeadPicurl = row[2];
        
        
    //     sql = string("UPDATE g_bumper_info SET head_picurl=\"")+strHeadPicurl+string("\", nick_name=\"")+strNickName+ "\" WHERE user_id="+std::to_string(id);    
    //     g_pDbMySql->Query(sql);        
    //     LOGERROR("[CUserData::LoadInfo] nAmount=[%d] id=[%d] strNickName=[%s] strHeadPicurl=[%s]", nAmount++, id,strNickName.c_str(),strHeadPicurl.c_str());
    // }
    // LOGERROR("[CUserData::LoadInfo] OK");
    // return FALSE;
    //!alter end:把g_wechat_user的用户刷新到g_bumper_info表中（涉及到的字段有nickname、avatar_url）

    /*
    //Read robot conf
    CIniFile iniNICO("../Config/RobotInfoConf.ini", "RobotInfoConf_NICO");
    iniNICO.GetString(g_robotConf_NICO.name, "name", NAME_SIZE);
    LOGMSG("[Main] ini NICO name[%s]", g_robotConf_NICO.name);

    g_robotConf_NICO.id = iniNICO.GetInt("id");
    g_robotConf_NICO.name_id = iniNICO.GetInt("name_id");
    g_robotConf_NICO.type = iniNICO.GetInt("type");
    g_robotConf_NICO.age = iniNICO.GetInt("age");
    g_robotConf_NICO.gender = iniNICO.GetInt("gender");
    g_robotConf_NICO.intellectual = iniNICO.GetInt("intellectual");
    g_robotConf_NICO.beautiful = iniNICO.GetInt("beautiful");
    g_robotConf_NICO.lovely = iniNICO.GetInt("lovely");
    g_robotConf_NICO.skin_state = iniNICO.GetInt("skin_state");
    g_robotConf_NICO.long_physical = iniNICO.GetInt("long_physical");
    g_robotConf_NICO.short_physical = iniNICO.GetInt("short_physical");
    g_robotConf_NICO.energy = iniNICO.GetInt("energy");
    g_robotConf_NICO.thirsty = iniNICO.GetInt("thirsty");
    g_robotConf_NICO.hunger = iniNICO.GetInt("hunger");
    g_robotConf_NICO.clean = iniNICO.GetInt("clean");
    g_robotConf_NICO.surface_temperature = iniNICO.GetInt("surface_temperature");
    g_robotConf_NICO.account = iniNICO.GetInt("account");
    g_robotConf_NICO.bornArea = iniNICO.GetInt("bornArea");
    LOGMSG("[Main] ini NICO id[%u], bornArea[%u]", g_robotConf_NICO.id, g_robotConf_NICO.bornArea);
    */

    BOOL bRet;

    //init map
    //SceneMgr::Instance().init();

    //add by huyf 2018.03.30:添加多线程架构
    //LOGMSG("[Main] CMessagePort::InitPortSet...");
    //CHECKF(CMessagePort::InitPortSet(MSGPORT_MAPGROUP_FIRST + g_config.ONLINE_MAPGROUP_SIZE));
    //LOGMSG("[Main] CMessagePort::InitPortSet Is Ok...MSGPORT_MAPGROUP=【%d~%d】",MSGPORT_MAPGROUP_FIRST, MSGPORT_MAPGROUP_FIRST + g_config.ONLINE_MAPGROUP_SIZE);
	//add end:多线程架构

    LOGMSG("[Main] g_World.Init");
    bRet = g_World.Init();
    CHECKF(bRet);

    LOGMSG("[Main] g_World.Loop\n");
    bRet = g_World.Loop();
    CHECKF(bRet);

    LOGMSG("[Main] g_World.Exit\n");
    bRet = g_World.Exit();
    CHECKF(bRet);
    DEBUG_CATCH("main")
    return 0;
}

//////////////////////////////////////////////////////////////////////
//
// func description :   constructor
//
//-----input------
// Parameters
//      无
//------output------
// Return
//      无
World::World()
{
    SetActive(TRUE);

#if defined(__WINDOWS__)
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(2 , 2);
    err = WSAStartup(wVersionRequested, &wsaData);
#endif
}

//////////////////////////////////////////////////////////////////////
//
// func description :   destructor
//
//-----input------
// Parameters
//      无
//------output------
// Return
//      无
World::~World()
{
#if defined(__WINDOWS__)
        WSACleanup();
#endif
}


//////////////////////////////////////////////////////////////////////
//
// func description :   Init
//
//-----input------
// Parameters
//      无
//------output------
// Return
//      无
BOOL World::Init()
{
    BOOL bRet;
    LOGMSG("[World::Init] NewStaticManager");
    bRet = NewStaticManager();
    CHECKF(bRet);
    LOGMSG("[World::Init] NewStaticManager...OK!");

    LOGMSG("[World::Init] InitStaticManager");
    bRet = InitStaticManager();
    CHECKF(bRet);
    LOGMSG("[World::Init] InitStaticManager...OK!");

    return TRUE;
}

BOOL World::Loop()
{
    while(IsActive())
    {
        MySleep(1);//1秒
        g_pServerManager->Loop();
        g_pClientManager->Loop();

        //战争管理器
        g_pBattleManager->Loop();

        //排行榜管理器
        g_pTopListManager->Loop();
    }
    return TRUE;
}

BOOL World::Exit()
{
    BOOL bRet;
    LOGMSG("Start Exit...");
    bRet = DelStaticManager();
    CHECKF(bRet);
    LOGMSG("Exit...OK!");
    return TRUE;
}

BOOL World::NewStaticManager()
{
    DEBUG_TRY
    LOGMSG("[World::NewStaticManager] g_pBattleCfg new");
    g_pBattleCfg = new CBattleCfg();
    CHECKF(g_pBattleCfg);
    LOGMSG("[World::NewStaticManager] g_pBattleCfg new...OK");

    LOGMSG("[World::NewStaticManager] g_pClientManager new");
    g_pClientManager = new ClientManager();
    CHECKF(g_pClientManager);
    LOGMSG("[World::NewStaticManager] g_pClientManager new...OK");

    //时间模块需要最开始的时候设置
    LOGMSG("[World::NewStaticManager] g_pTimeManager new");
    g_pTimeManager = new TimeManager();
    CHECKF(g_pTimeManager);
    LOGMSG("[World::NewStaticManager] g_pTimeManager new...OK");
	
	//add by huyf 2018.04.03:多线程架构
    //LOGMSG("[World::InitStaticManager] new CSocketThread...");
    //m_pSocketThread = new CSocketThread(CMessagePort::GetInterface(MSGPORT_SOCKET), "SocketThread", 0);
    //CHECKF(m_pSocketThread);
    //LOGMSG("[World::InitStaticManager] new CSocketThread Is Ok...");
	//add end:多线程架构

    LOGMSG("[World::NewStaticManager] g_pServerManager new");
    g_pServerManager = new ServerManager();
    CHECKF(g_pServerManager);
    LOGMSG("[World::NewStaticManager] g_pServerManager new...OK");

    LOGMSG("[World::NewStaticManager] g_pPlayerPool new"); 
    g_pPlayerPool = new PlayerPool();
    CHECKF(g_pPlayerPool);
    LOGMSG("[World::NewStaticManager] g_pPlayerPool new...OK");

    LOGMSG("[World::NewStaticManager] g_pPacketFactoryManager new");
    g_pPacketFactoryManager = new PacketFactoryManager();
    CHECKF(g_pPacketFactoryManager);
    LOGMSG("[World::NewStaticManager] g_pPacketFactoryManager new...OK");

    LOGMSG("[World::NewStaticManager] g_pTopListManager new");
    g_pTopListManager = new CTopListManager();
    CHECKF(g_pTopListManager);
    LOGMSG("[World::NewStaticManager] g_pTopListManager new...OK");

    LOGMSG("[World::NewStaticManager] g_pItemAwardManager new");
    g_pItemAwardManager = new CItemAwardManager();
    CHECKF(g_pItemAwardManager);
    LOGMSG("[World::NewStaticManager] g_pItemAwardManager new...OK");

    LOGMSG("[World::NewStaticManager] g_pItemAwardGroupManager new");
    g_pItemAwardGroupManager = new CItemAwardGroupManager();
    CHECKF(g_pItemAwardGroupManager);
    LOGMSG("[World::NewStaticManager] g_pItemAwardGroupManager new...OK");
    
    LOGMSG("[World::NewStaticManager] g_pItemTypeManager new");
    g_pItemTypeManager = new CItemTypeManager();
    CHECKF(g_pItemTypeManager);
    LOGMSG("[World::NewStaticManager] g_pItemTypeManager new...OK");

    LOGMSG("[World::NewStaticManager] g_pBattleManager new");
    g_pBattleManager = new CBattleManager();
    CHECKF(g_pBattleManager);
    LOGMSG("[World::NewStaticManager] g_pBattleManager new...OK");

    LOGMSG("[World::NewStaticManager] g_pItemAwardManager new");
    g_pItemAwardManager = new CItemAwardManager();
    CHECKF(g_pItemAwardManager);
    LOGMSG("[World::NewStaticManager] g_pItemAwardManager new...OK");

    LOGMSG("[World::NewStaticManager] g_pOnLineAwardCfgManager new");
    g_pOnLineAwardCfgManager = new COnLineAwardCfgManager();
    CHECKF(g_pOnLineAwardCfgManager);
    LOGMSG("[World::NewStaticManager] g_pOnLineAwardCfgManager new...OK");

    LOGMSG("[World::NewStaticManager] g_pTaskDetailCfgManager new");
    g_pTaskDetailCfgManager = new CTaskDetailCfgManager();
    CHECKF(g_pTaskDetailCfgManager);
    LOGMSG("[World::NewStaticManager] g_pTaskDetailCfgManager new...OK");

    DEBUG_CATCHF("World::NewStaticManager")
    return TRUE;
}

BOOL World::InitStaticManager()
{
    DEBUG_TRY
    BOOL ret;
	//add by huyf 2018.04.03:多线程架构
    //LOGMSG("[World::InitStaticManager] m_pSocketThread->CreateThread");
    //CHECKF(m_pSocketThread->CreateThread(false));
    //LOGMSG("[World::InitStaticManager] m_pSocketThread->CreateThread  Is Ok...");
    //add end:添加多线程架构
    
    LOGMSG("[World::InitStaticManager] g_pBattleCfg Init");
    ret = g_pBattleCfg->LoadConfigInfo();
    CHECKF(ret);
    LOGMSG("[World::InitStaticManager] g_pBattleCfg Init...OK");

    LOGMSG("[World::InitStaticManager] g_pClientManager Init");
    ret = g_pClientManager->Init();
    CHECKF(ret);
    LOGMSG("[World::InitStaticManager] g_pClientManager Init...OK");
        
    LOGMSG("[World::InitStaticManager] g_pTimeManager Init");
    ret = g_pTimeManager->Init();
    CHECKF(ret);
    LOGMSG("[World::InitStaticManager] g_pTimeManager Init...OK");

    LOGMSG("[World::InitStaticManager] g_pServerManager Init");
    ret = g_pServerManager->Init();
    CHECKF(ret);
    LOGMSG("[World::InitStaticManager] g_pServerManager Init...OK");

    LOGMSG("[World::InitStaticManager] g_pPlayerPool Init");
    ret = g_pPlayerPool->Init(MAX_POOL_SIZE);
    CHECKF(ret);
    LOGMSG("[World::InitStaticManager] g_pPlayerPool Init...OK", MAX_POOL_SIZE);

    LOGMSG("[World::InitStaticManager] g_pPacketFactoryManager Init");
    ret = g_pPacketFactoryManager->Init();
    CHECKF(ret);
    LOGMSG("[World::InitStaticManager] g_pPacketFactoryManager Init...OK");

    LOGMSG("[World::InitStaticManager] g_pTopListManager Init");
    ret = g_pTopListManager->Init();
    CHECKF(ret);
    LOGMSG("[World::InitStaticManager] g_pTopListManager Init...OK");

    LOGMSG("[World::InitStaticManager] g_pItemTypeManager Init");
    ret = g_pItemTypeManager->Init();
    CHECKF(ret);
    LOGMSG("[World::InitStaticManager] g_pItemTypeManager Init...OK");

    LOGMSG("[World::InitStaticManager] g_pItemAwardManager Init");
    ret = g_pItemAwardManager->Init();
    CHECKF(ret);
    LOGMSG("[World::InitStaticManager] g_pItemAwardManager Init...OK");

    LOGMSG("[World::InitStaticManager] g_pItemAwardGroupManager Init");
    ret = g_pItemAwardGroupManager->Init();
    CHECKF(ret);
    LOGMSG("[World::InitStaticManager] g_pItemAwardGroupManager Init...OK");
    
    LOGMSG("[World::InitStaticManager] g_pOnLineAwardCfgManager Init");
    ret = g_pOnLineAwardCfgManager->Init();
    CHECKF(ret);
    LOGMSG("[World::InitStaticManager] g_pOnLineAwardCfgManager Init...OK");
    
    LOGMSG("[World::InitStaticManager] g_pTaskDetailCfgManager Init");
    ret = g_pTaskDetailCfgManager->Init();
    CHECKF(ret);
    LOGMSG("[World::InitStaticManager] g_pTaskDetailCfgManager Init...OK");

    DEBUG_CATCHF("World::InitStaticManager")
    return TRUE;
}

BOOL World::DelStaticManager()
{
    DEBUG_TRY
    SAFE_DELETE(g_pBattleCfg);
    LOGMSG("[World::DelStaticManager] SAFE_DELETE g_pBattleCfg is OK");

    SAFE_DELETE(g_pTimeManager);
    LOGMSG("[World::DelStaticManager] SAFE_DELETE g_pTimeManager is OK");

    //
    //以下模块放后面删除
    //
    //SAFE_DELETE(g_pOnlineUser);
    //LOGMSG(WORLD_LOGFILE, "g_pOnlineUser delete...OK");
	
	//add by huyf 2018.04.03:多线程架构
    //SAFE_DELETE(m_pSocketThread);
    //LOGMSG("[World::DelStaticManager] SAFE_DELETE m_pSocketThread is OK");
	//add end:多线程架构

    SAFE_DELETE(g_pClientManager);
    LOGMSG("[World::DelStaticManager] SAFE_DELETE g_pClientManager is OK");

    SAFE_DELETE(g_pPacketFactoryManager);
    LOGMSG("[World::DelStaticManager] SAFE_DELETE g_pPacketFactoryManager is OK");

    SAFE_DELETE(g_pPlayerPool);
    LOGMSG("[World::DelStaticManager] SAFE_DELETE g_pPlayerPool is OK");

    SAFE_DELETE(g_pServerManager);
    LOGMSG("[World::DelStaticManager] SAFE_DELETE g_pServerManager is OK");
    
    SAFE_DELETE(g_pTopListManager);
    LOGMSG("[World::DelStaticManager] SAFE_DELETE g_pTopListManager is OK");
    
    SAFE_DELETE(g_pItemTypeManager);
    LOGMSG("[World::DelStaticManager] SAFE_DELETE g_pItemTypeManager is OK");

    SAFE_DELETE(g_pItemAwardManager);
    LOGMSG("[World::DelStaticManager] SAFE_DELETE g_pItemAwardManager is OK");

    SAFE_DELETE(g_pItemAwardGroupManager);
    LOGMSG("[World::DelStaticManager] SAFE_DELETE g_pItemAwardGroupManager is OK");

    SAFE_DELETE(g_pBattleManager);
    LOGMSG("[World::DelStaticManager] SAFE_DELETE g_pBattleManager is OK");

    SAFE_DELETE(g_pOnLineAwardCfgManager);
    LOGMSG("[World::DelStaticManager] SAFE_DELETE g_pOnLineAwardCfgManager is OK");
    
    SAFE_DELETE(g_pTaskDetailCfgManager);
    LOGMSG("[World::DelStaticManager] SAFE_DELETE g_pOnLineAwardCfgManager is OK");

    SAFE_DELETE(g_pDbMySql);
    LOGMSG("[World::DelStaticManager] SAFE_DELETE g_pDbMySql is OK");

    DEBUG_CATCHF("World::DelStaticManager")
    return TRUE;
}
