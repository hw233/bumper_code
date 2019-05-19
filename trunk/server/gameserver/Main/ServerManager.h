//////////////////////////////////////////////////////////////////////
//文件名称：ServerManager.h
//功能描述：
//版本说明：ServerManager API
//			
//编写作者：2017.08.21 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#ifndef __SERVERMANAGER_H__
#define __SERVERMANAGER_H__

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include "../NetBase/ServerSocket.h"
#include "../Common/Type.h"
#include "../Common/Thread.h"
#include "../Common/Config.h"

#include "PlayerManager.h"
#include "ClientManager.h"
#include "PlayerPool.h"

//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//class
//////////////////////////////////////////////////////////////////////


class ServerPlayer;

class ServerManager : public PlayerManager
{
public :
	ServerManager();
	virtual ~ServerManager();

public:
	//模块初始化接口
	BOOL 	Init();
	//网络监听接口
#ifdef _SELECT_
	INT     Select();
#endif        
        
    //add by huyf 2018.01.24:支持I/O多路复用poll
#ifdef _POLL_  
    INT     Poll();
#endif
    BOOL    IsAccept(Socket* pSocket);
    BOOL    IsReady(Socket* pSocket);
    BOOL    IsWrite(Socket* pSocket);
    BOOL    IsException(Socket* pSocket);
    //add end:支持I/O多路复用poll
    
    //add by huyf 2018.01.24:支持I/O多路复用epoll
#ifdef _EPOLL_  
    INT     EPoll();
#endif
    //add end:支持I/O多路复用epoll

    //add by huyf 2018.04.04:多线程架构
    BOOL 	ProcessSocket();
    void 	AcceptNewSocket();
    //add end:多线程架构    

	//数据接收接口
	BOOL    ProcessInputs();
	//数据发送接口
	BOOL    ProcessOutputs();
	//异常处理处理
	BOOL    ProcessExceptions();
	//消息处理
	BOOL    ProcessCommands();
	//新连接接收处理
	BOOL    AcceptNewConnection();
	//判断连接是否已满
	BOOL 	IsFullConnect();
	//逻辑接口
	BOOL    HeartBeat(DWORD dwTime=0);

	//LOOP 接口
	BOOL	Loop();

	//OnTimer 接口
	BOOL	OnTimer();

	//激活
	BOOL 	IsActive()						{  return m_bActive;	}
	VOID    SetActive(BOOL active)			{	m_bActive = active;	}					

public://通用接口
	//将Player数据加入系统中
	BOOL				AddPlayer(Player* pPlayer);
	//将拥有fd句柄的玩家数据从当前系统中清除
	BOOL				DelPlayer(Player* pPlayer);

	//出现异常后将Player信息清除，并将系统中的信息也清除
	//断开玩家的连接
	BOOL				RemovePlayer(Player* pPlayer);
	void				RemoveAllPlayer() ;

	//通过ServerID取得ServerPlayer
	ServerPlayer*		GetServerPlayer(ID_t ServerID);

	//向所有服务器端程序广播消息
	void				BroadCast(CNetMsg* pMsg);
	//只向Server广播
	void				BroadCastServer(CNetMsg* pMsg); 
	//只向Login广播
	void				BroadCastLogin(CNetMsg* pMsg);
	//OnTimer计时器
	CTimeOut&			QueryOnTimerInterval()							{ return m_tOnTimerInterval;	 }
	 

private:
	//用于侦听的服务器Socket
	ServerSocket*                   m_pServerSocket;
        
	//add by huyf 2018.01.24:支持I/O多路复用poll
	//用于侦听的服务器SOCKET句柄值（此数据即m_pServerSocket内拥有的SOCKET句柄值）        
	//SOCKET				m_SocketID;
	//add end:支持I/O多路复用poll
        
#ifdef _SELECT_    	
	//网络相关数据
	enum
	{
		SELECT_BAK = 0, 	//当前系统中拥有的完整句柄数据
		SELECT_USE = 1,		//用于select调用的句柄数据
		SELECT_MAX = 2,		//结构使用数量
		
	};

	fd_set				m_ReadFDs[SELECT_MAX];
	fd_set				m_WriteFDs[SELECT_MAX];
	fd_set				m_ExceptFDs[SELECT_MAX];
	struct timeval		m_Timeout[SELECT_MAX];
#endif
        
#ifdef _POLL_
private:
    //add by huyf 2018.01.24:支持I/O多路复用poll
    struct pollfd                   m_ClientSet[CLIENT_MAX_SIZE + 1];   //1:服务器监听套接字增加一位   
    INT                             m_MaxIndex;  //m_ClientSet[i]最大不空闲位置的下标 
    INT 							m_nEffectivePollFds;         // 有消息通知的fd数量
    //add end:支持I/O多路复用poll   
#endif
        
    //add by huyf 2018.01.24:支持I/O多路复用epoll
    //部分成员留作扩展使用
#ifdef _EPOLL_  
    INT                             m_nEpollFd;             // 创建一个epoll的句柄，size用来告诉内核这个监听的数目一共有多大。这个参数不同于select()中的第一个参数，给出最大监听的fd+1的值。需要注意的是，当创建好epoll句柄后，它就是会占用一个fd值，在linux下如果查看/proc/进程id/fd/，是能够看到这个fd的，所以在使用完epoll后，必须调用close()关闭，否则可能导致fd被耗尽。
    INT 							m_nEffectiveEpollFds;         // 有消息通知的fd数量
    struct epoll_event              m_EffectiveEpollEvents[CLIENT_MAX_SIZE + 1];   //1:服务器监听套接字增加一位(有消息的)
    struct epoll_event              m_EpollEvents[CLIENT_MAX_SIZE + 1];   //1:服务器监听套接字增加一位，//留作后期使用
    INT                             m_MaxEpollIndex;  //m_EpollEvents[i]最大不空闲位置的下标
#endif
    //add end:支持I/O多路复用epoll
    
	SOCKET 				m_MinFD;
	SOCKET 				m_MaxFD;
	INT 				m_iFDSize;
	BOOL 				m_bActive;

private:
	TID                             m_ThreadID;
	//此数组的下标值为ServerID, 数组值为PlayerID
	PlayerID_t                      m_aServerHash[OVER_MAX_SERVER];
	CTimeOut 						m_tOnTimerInterval;					//OnTimer计时器

public:
    //add by huyf 2018.10.09:碰碰车二期
//    CBattleManager* m_pBattleManager;    
    //add end:碰碰车二期
    
};

//全局变量
extern ServerManager* g_pServerManager;

#endif