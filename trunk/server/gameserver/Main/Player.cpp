//////////////////////////////////////////////////////////////////////
//文件名称：RobotBase.cpp
//功能描述：机器人接口（基类）
//版本说明：socket API
//			
//编写作者：2017.08.16 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//include头文件
//////////////////////////////////////////////////////////////////////
#include <string>
#include <ctime>
#include <chrono>
#include <unistd.h> 
#include <termios.h> 
#include <stropts.h>
#include <sys/ioctl.h>
#include <stdint.h>

#include "../NetBase/PacketDefine.h"
#include "../Base/TimeManager.h"
#include "../Base/logger.h"
#include "../Protocol/Command.h"
#include "Player.h"
#include "PlayerPool.h"
#include "Pet.h"
#include "PacketFactoryManager.h"
#include "../MsgHandler/AllMsg.h"
#include "./Battle/BattleCfg.h"
#include "./OnLineAward/OnLineAwardCfgManager.h"
#include "./TaskDetail/TaskDetailCfgManager.h"


long long t_start=0;
long long t_end=0;


#if defined(__LINUX__)
extern INT errno;
#endif

//CHAR Error[ERROR_SIZE];

using namespace std;
using namespace chrono;
//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//
// func description	:	constructor
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
Player::Player()
{
	DEBUG_TRY
    //!alter by huyf 2018.02.07:设置初始时间，创建连接后，如果30秒收不到心跳包则断开连接
    system_clock::time_point time = system_clock::now(); 
    m_time = system_clock::to_time_t(time);
    //m_time = 0;
    //!alter end:设置初始时间，创建连接后，如果30秒收不到心跳包则断开连接
    m_robot = new Robot();
//	m_pPet = new Pet();
    m_PID = INVALID_ID;
    m_UID = INVALID_ID;
    m_PlayerManagerID = INVALID_ID;
    //add by huyf 2018.04.18:增加初始化
    m_AccountID = 0;
    //add end:增加初始化
    //创建socket
    m_pSocket = new Socket();
    ASSERT(m_pSocket);

    //创建input-buffer，output-buffer
    m_pSocketInputStream = new SocketInputStream(m_pSocket, g_config.ONLINE_DEFAULT_INPUT_BUFFER_SIZE/*DEFAULTSOCKETINPUTBUFFERSIZE*/, g_config.ONLINE_DISCONNECT_INPUT_BUFFER_SIZE/*DISCONNECTSOCKETINPUTSIZE*/);
    ASSERT(m_pSocketInputStream);		
    m_pSocketOutputStream = new SocketOutputStream(m_pSocket, g_config.ONLINE_DEFAULT_OUTPUT_BUFFER_SIZE/*DEFAULTSOCKETOUTPUTBUFFERSIZE*/, g_config.ONLINE_DISCONNECT_OUTPUT_BUFFER_SIZE/*DISCONNECTSOCKETOUTPUTSIZE*/);
    ASSERT(m_pSocketOutputStream);

    m_PacketIndex = 0;
    m_IsEmpty = TRUE;
    m_IsDisconnect = FALSE;	

    //add by huyf 2018.04.23:添加账号信息
    m_StrAccount = "";
    //add end:添加账号信息
    //
    //m_pBattleManager = NULL;
    m_pBcs = NULL;
    m_pBct = NULL;
    m_pBrbt = NULL;
    m_pBeKiller = NULL;

    //设置玩家游戏进度
    m_nGameProcess = _GAME_PROCESS_NONE;
    m_MapCollideSet.clear();
    m_tDeadTimeInterval.Clear();
    m_nCurCollideFrame = 0;
    //m_FriendManager.Init(this);
    //add by huyf 2018.11.24:添加好友系统
    m_nGameStatus = _FRIEND_STATUS_OFFLINE;
    //add end:添加好友系统
    m_tLimitTimeInterval.Clear();
    m_tOnLineInterval.Clear();
    m_pItemDataManager = new CItemDataManager();
    m_pTaskDetailManager = new CTaskDetailManager();
    m_tMultiKillTimer.Clear();
    m_nMultiKillCount = 0;
	DEBUG_CATCH("Player::Player")
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	destructor
//
//-----input------~Player
// Parameters
//		无
//------output------
// Return 
//		无
Player::~Player()
{
	DEBUG_TRY			
    SAFE_DELETE(m_robot);
//	SAFE_DELETE(m_pPet);
    SAFE_DELETE(m_pSocket);
    SAFE_DELETE(m_pSocketInputStream);
    SAFE_DELETE(m_pSocketOutputStream);
    SAFE_DELETE(m_pItemDataManager);
    SAFE_DELETE(m_pTaskDetailManager);
	DEBUG_CATCH("Player::~Player")
}

//////////////////////////////////////////////////////////////////////
//
// func description :   OnTimer函数
//
//-----input------
// Parameters
//      无
//------output------
// Return 
//      无
//
BOOL Player::OnTimer()
{
    DEBUG_TRY
    BOOL ret = TRUE;

    if (m_nGameProcess == _GAME_PROCESS_LOGIN)
    {
        ////////////////////////////////////////道具系统相关检查//////////////////////////////////////////////////
        //检查玩家道具的倒计时相关
        ret = m_pItemDataManager->OnTimer();  
        ////////////////////////////////////////道具系统相关检查//////////////////////////////////////////////////
        ///
        ////////////////////////////////////////好友系统相关检查//////////////////////////////////////////////////
        //好友系统
        ret = m_FriendManager.OnTimer();  
        ////////////////////////////////////////好友系统相关检查//////////////////////////////////////////////////
        ///
        ////////////////////////////////////////任务系统相关检查//////////////////////////////////////////////////
        //添加任务系统
        ret = m_pTaskDetailManager->OnTimer();  
        ////////////////////////////////////////任务系统相关检查//////////////////////////////////////////////////
        
        //检查玩家其他倒计时相关
        //检查玩家日常任务
        if (this->GetLastResetTaskTime() != (UInt32)DateStamp()) 
        {
            //同步前端星期几
            time_t tInput = time(NULL);
            tm * pTm = localtime(&tInput);
            if (pTm)
            {
                CMsgUserAttrib msg;
                IF_OK(msg.CreateMsg(_ACT_USERATTRIB_CURTMWDAY, pTm->tm_wday))
                {
                    this->SendMsgToClient(&msg);
                }
            }

            //更新掩码位
            this->ResetDayTaskMask(TRUE);
        }

        //检查每周任务
        if (this->GetCurWeekNum() != (UInt32)WeekNum()) 
        {
            //更新掩码位
            this->ResetWeekTaskMask(TRUE);
        }

        //检查限时免费计时器结束---重新随机出一个道具
        if(m_tLimitTimeInterval.IsActive() && m_tLimitTimeInterval.IsTimeOut())
        {
            CItemTypeManager* pItemTypeManager = QueryItemTypeManager();
            if (pItemTypeManager)
            {
                CItemType* pItemType = pItemTypeManager->QueryNoExistItemType(this, ITEM_TYPE_CAR_FACE);
                if (pItemType)
                {
                    //查到没有的类型道具
                    this->SetShowItemType(pItemType->GetItemType());
                    //设置倒计时 2小时
                    UINT nLifeTime = 3600*2;
                    this->SetShowRemainTs(nLifeTime);
                    //启动时钟计时器
                    this->QueryLimitTimeInterval().Startup(nLifeTime);

                    //同步前端倒计时结束
                    CMsgUserAttrib msg;
                    IF_OK(msg.CreateMsg(_ACT_USERATTRIB_SHOW_ITEM_TYPE, pItemType->GetItemType()))
                    {
                        this->SendMsgToClient(&msg);
                    }

                    CMsgUserAttrib msg2;
                    IF_OK(msg2.CreateMsg(_ACT_USERATTRIB_SHOW_REMAIN_Ts, nLifeTime))
                    {
                        this->SendMsgToClient(&msg2);
                    }
                }
                else //未有道具
                {
                    m_tLimitTimeInterval.Clear();
                    //查到没有的类型道具
                    //超时后没有新的道具可以供玩家展现了，则不去处理showItemType,直接把ts设置位0就可以
                    //this->SetShowItemType(0);

                    this->SetShowRemainTs(0);

                    //同步前端
                    // CMsgUserAttrib msg;
                    // IF_OK(msg.CreateMsg(_ACT_USERATTRIB_SHOW_ITEM_TYPE, pItemType->GetItemType()))
                    // {
                    //     this->SendMsgToClient(&msg);
                    // }
                    
                    //同步前端倒计时结束
                    CMsgUserAttrib msg;
                    IF_OK(msg.CreateMsg(_ACT_USERATTRIB_SHOW_REMAIN_Ts, 0))
                    {
                        this->SendMsgToClient(&msg);
                    }
                }
            }
            else
            {
                LOGERROR("[MsgLogin::Process] pItemTypeManager == NULL");
            }
        }

        //检查在线领奖计时器-是否超时
        if(m_tOnLineInterval.IsActive() && m_tOnLineInterval.IsTimeOut())
        {
            m_tOnLineInterval.Clear();
            this->SetOnLineRemainTs(0);

            //同步前端倒计时结束
            CMsgUserAttrib msg;
            IF_OK(msg.CreateMsg(_ACT_USERATTRIB_ONLINE_REMAIN_Ts, 0))
            {
                this->SendMsgToClient(&msg);
            }
        }

        //检查多次击杀
        if (m_tMultiKillTimer.IsActive() && m_tMultiKillTimer.IsTimeOut())
        {
            LOGDEBUG("[CMsgBattle::Process] OnTimer QueryMultiKillTimer Clear");
            m_tMultiKillTimer.Clear();
            m_nMultiKillCount = 0;
        }
    }
    return ret;
    
    DEBUG_CATCHF("OnTimer")
}
//////////////////////////////////////////////////////////////////////
//
// func description :   SaveInfo
//
//-----input------
// Parameters
//      无
//------output------
// Return 
//      无
BOOL Player::SaveInfo()
{
    //保存玩家数据
    m_UserData.SaveInfo();

    //保存好友数据
    m_FriendManager.SaveInfo(TRUE);

    //保存道具数据
    m_pItemDataManager->SaveInfo(TRUE);

    //保存任务数据
    m_pTaskDetailManager->SaveInfo(TRUE);
    
    return TRUE;    
}


//////////////////////////////////////////////////////////////////////
//
// func description	:	CleanUp
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
VOID Player::CleanUp()
{
	DEBUG_TRY
    m_pSocket->close();
    m_pSocketInputStream->CleanUp();
    m_pSocketOutputStream->CleanUp();
    //!alter by huyf 2018.03.01:玩家退出后不再重置m_PID
    //m_PID = INVALID_ID;               
    //!alter end:玩家退出后不再重置m_PID
    m_UID = INVALID_ID;
    m_PlayerManagerID = INVALID_ID;
    m_PacketIndex = 0;
    m_IsEmpty = TRUE;
    m_IsDisconnect = FALSE;	
	//!alter by huyf 2018.04.18:清理robot数据	
    m_robot->CleanUp();
	//!alter end:清理robot数据
    //add by huyf 2018.04.18:增加初始化
    m_AccountID = 0;
    //add end:增加初始化
    //add by huyf 2018.04.23:添加账号信息
    m_StrAccount = "";
    //add end:添加账号信息
    //add by huyf 2018.10.09:碰碰车二期
    m_pBcs = NULL;
    m_pBct = NULL;
    m_pBrbt = NULL;
    m_UserData.CleanUp();
    m_pBeKiller =  NULL;
    //设置玩家游戏进度
    m_nGameProcess = _GAME_PROCESS_NONE;
    m_MapCollideSet.clear();
    m_tDeadTimeInterval.Clear();
    m_nCurCollideFrame = 0;
    m_FriendManager.CleanUp();
    //add by huyf 2018.11.24:添加好友系统
    m_nGameStatus = _FRIEND_STATUS_OFFLINE;
    //add end:添加好友系统
    m_tLimitTimeInterval.Clear();
    m_tOnLineInterval.Clear();
    m_pItemDataManager->CleanUp();
    m_pTaskDetailManager->CleanUp();
    m_tMultiKillTimer.Clear();
    m_nMultiKillCount = 0;

    LOGDEBUG("[Player::CleanUp] Is Call...\n");
    //add end:碰碰车二期
	DEBUG_CATCH("Player::CleanUp")
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	Disconnect
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
VOID Player::Disconnect()
{
    DEBUG_TRY	
    m_pSocket->close();		
    DEBUG_CATCH("Player::Disconnect")
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	IsValid
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL Player::IsValid()
{
    DEBUG_TRY	
    if (m_pSocket  ==  NULL)
    {
        return FALSE;
    }	
    if (m_pSocket->isValid())
    {
        return TRUE;
    }
    DEBUG_CATCH("Player::IsValid")
    return FALSE;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	从socket缓存区中receive数据到用户buffer
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无



BOOL Player::ProcessInput()
{

    t_start =GetMillSecTime();
         
    DEBUG_TRY
    CHECKF(m_pSocket);
    CHECKF(m_pSocketInputStream);

    if (IsDisconnect())
    {
        LOGDEBUG("[Player::ProcessInput] IsDisconnect");
        return TRUE;
    }

    INT ret = m_pSocketInputStream->Fill();
    if (ret <= 0)
    {
        LOGERROR("[Player::ProcessInput] Net Error Ip=[%s] Port=[%d] ret=[%d] errno=[%d] discription=[%s]", m_pSocket->m_Host, m_pSocket->m_Port, ret, errno, strerror(errno));

        //网络问题，关闭连接
        /*this->SetDisconnect(TRUE);			
        g_pPlayerPool->DelPlayer(this->GetPlayerID());
        this->CleanUp();*/
        return FALSE;
    }

    //LOGDEBUG("有数据过来：ret=[%d] m_nFirstHand=[%d] m_handler->buff=[%s]\n", ret, m_pSocket->GetFirstHand(), m_pSocket->GetWebSocketHandler()->GetBuff());
    if(m_pSocket->GetFirstHand() == 1)
	{
       if(m_pSocket->GetWebSocketHandler() && m_pSocket->GetWebSocketHandler()->GetStatus() == WEBSOCKET_UNCONNECT)
       {
            LOGDEBUG("[Player::ProcessInput] Deal With The First HandShark Begin...");
            m_pSocket->GetWebSocketHandler()->HandShark();
            LOGDEBUG("[Player::ProcessInput] Deal With The First HandShark End...\n");
       }
	
        //LOGDEBUG("更新网络连接状态-2，后续消息可以正常通信了...\n\n");
	  	m_pSocket->SetFirstHand(2);
		m_pSocketInputStream->InitSize();
		memset(m_pSocketInputStream->m_Buffer, 0, g_config.ONLINE_DEFAULT_INPUT_BUFFER_SIZE);

        //设置玩家游戏进度
        m_nGameProcess = _GAME_PROCESS_HANDSHARK;
	}
    //LOGERROR("有数据过来：pos=[%d], length=[%d]\n\n", m_pSocket->GetWebSocketHandler()->GetRequest()->pos, m_pSocket->GetWebSocketHandler()->GetRequest()->GetMsgLength());

    DEBUG_CATCHF("Player::ProcessInput")  
    return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	將用户buffer中的数据输出到socket缓存区
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
BOOL Player::ProcessOutput()
{
	DEBUG_TRY	
    CHECKF(m_pSocket);
    CHECKF(m_pSocketOutputStream);

    if (IsDisconnect())
    {
        return TRUE;
    }

    //判断是否有数据需要发送
    UINT size = m_pSocketOutputStream->Length();
    if (size  ==  0)
    {
        return TRUE;
    }

    INT ret = m_pSocketOutputStream->Flush();
    if (ret < 0)
    {
        //网络问题，关闭连接
        /*this->SetDisconnect(TRUE);			
        g_pPlayerPool->DelPlayer(this->GetPlayerID());
        this->CleanUp();*/
        LOGERROR("[Player::ProcessOutput] Net Error ret=[%d] errno[%d] discription[%s]", ret, errno, strerror(errno));
        return FALSE;
    }		
	DEBUG_CATCHF("Player::ProcessOutput")
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	ProcessCommand:处理消息
//
//-----input------
// Parameters
//		Option			：执行部分选项操作
//------output------
// Return 
//		无
BOOL Player::ProcessCommand(bool Option /*= true*/)
{
//	DEBUG_TRY	
    CHECKF(m_pSocketInputStream);
    BOOL ret;

    if(IsDisconnect())
    {
        return TRUE;
    }
        
//每帧可以执行的消息数量上限
#define EXE_COUNT_PER_TICK 1
    UINT uret = 0;
    CNetMsg* pMsg = NULL;
    for(INT i=0; i<EXE_COUNT_PER_TICK; i++)
    {

        if(m_pSocket->m_nFirstHand != 1)
        {
    
            //数据不能填充消息头，出错
            //DEBUG_TRY
            //!alter by huyf 2018.03.09:放到前面判断提高执行效率
            UINT nHaveDataLen = m_pSocketInputStream->Length();
            //LOGERROR("缓存中的数据长度=[%d]\n", nHaveDataLen); 
            if (nHaveDataLen == 0)  //无数据执行执行下一个玩家
            {
                break;
            }
            //!alter end:放到前面判断提高执行效率    
            
            INT nPackageID = 0;
            UINT nNeedReadTotalLengh = 6;
            INT nRet = m_pSocket->ReadNetData(m_pSocketInputStream, nPackageID, nNeedReadTotalLengh);
            if (nRet == -2)
            {
                return FALSE;
            }
            else if (nRet == -2)
            {
                break;
            }
            else if (nRet == 0)
            {
                continue;
            }

            if(nNeedReadTotalLengh > nHaveDataLen)
        	{
        		LOGERROR("缓存中的数据长度不够协议完整包的长度...socketid=[%u] nNeedReadTotalLengh=[%d] nHaveDataLen=[%d]\t数据还不够，需要继续接收-----------", GetSocket()->getSocketID(), nNeedReadTotalLengh, nHaveDataLen);                        
        		continue;
        	}

            //根据消息ID创建对应的消息
            pMsg = g_pPacketFactoryManager->CreatePacket(nPackageID);//2001
            //不能分配到足够的内存
            if(pMsg == NULL)
            {
                LOGERROR("[Player::ProcessCommand] 根据消息ID创建对应的消息 nPackageID=[%d] ", nPackageID);
                return FALSE;
            }
                
            //设置消息报归属角色[m_pPlayer]
            pMsg->SetPlayer(this);
            //DEBUG_TRY
            //设置消息序列号
            //pMsg->SetPacketIndex(packetIndex);
            
            //add by huyf 2018.11.02:添加服务器收到前端消息的时间戳,暂时不做缓存帧
            if(nPackageID == _MSG_WALK)
            {
                long long llCurMillSecTime = GetMillSecTime();
                long long llRecvMsgTimeStamp = m_pSocket->GetRecvMsgTimeStamp();
                m_pSocket->SetRecvMsgTimeStamp(llCurMillSecTime);
                LOGDEBUG("[Player::ProcessCommand] Receive [CMsgWalk] MsgID=[%d] PlayerId=[%d] llCurMillSecTime=[%lld] llRecvMsgTimeStamp=[%lld] TimeStampInterval=[%lld]",
                    _MSG_WALK, this->GetUid(), llCurMillSecTime, llRecvMsgTimeStamp, llCurMillSecTime - llRecvMsgTimeStamp);
            }
            //add end:添加服务器收到前端消息的时间戳,暂时不做缓存帧
            
            //add by huyf 2018.11.16:防止玩家没有发送登录包直接进入游戏，影响其他包处理
            if(nPackageID != _MSG_LOGIN)
            {
                if (m_nGameProcess != _GAME_PROCESS_LOGIN)
                {
                    LOGERROR("[Player::ProcessCommand] Please Send MsgLogin Packet The First Time...reson=[No Receive MsgLogin Before The Dealing Other NetMsgS] PlayerId=[%d] MsgID=[%d] ",this->GetUid(), nPackageID);
                    return FALSE;
                }
            }
            //add end:防止玩家没有发送登录包直接进入游戏，影响其他包处理
            
            //LOGDEBUG("Start ReadPacket\n");
            //去读消息
            ret = m_pSocketInputStream->ReadPacket(pMsg, nNeedReadTotalLengh);
            //读取消息内容错误
            if(ret == FALSE)
            {
                LOGERROR("[Player::ProcessCommand] 读取消息内容错误");
                g_pPacketFactoryManager->RemovePacket(pMsg);
                return FALSE;
            }                        
                				
            //修正m_KickTime信息，m_KickTime信息中的值为判断是否需要踢掉
            //客户端的依据
            ResetKick();
            
            //此处处理玩家数据包-转接到每个消息自己的处理process函数
            uret = pMsg->Process(this);                
           
            if (m_pSocketInputStream->m_pSocket->m_Port != (UINT)g_config.MTS_PORT && m_pSocketInputStream->m_pSocket->m_Port != (UINT)g_config.ACC_PORT && m_pSocketInputStream->m_pSocket->m_Port != (UINT)g_config.PW_PORT && pMsg->GetPacketID()!=COMMON_HEART_BEAT)
            {
               /* code */       
                //LOGDEBUG("[Player::ProcessCommand] Recv Ip=[%s] Port=[%u] AcountId=[%u] PlayerId=[%u] PackageID=[%d] PackageSize=[%d]\n",m_pSocketInputStream->m_pSocket->m_Host, m_pSocketInputStream->m_pSocket->m_Port,this->GetAccountID(),this->GetPlayerID(), pMsg->GetPacketID() ,pMsg->GetPacketSize());
            } 

            BOOL bNeedRemove = TRUE;        
            //出现异常错误，断开此玩家连接
            if(uret  ==  PACKET_EXE_ERROR)
            {
                LOGERROR("[Player::ProcessCommand] uret  ==  PACKET_EXE_ERROR");
                if(pMsg) 
                    g_pPacketFactoryManager->RemovePacket(pMsg);
                LOGERROR("[Player::ProcessCommand] 出现异常错误，断开此玩家连接");
                return FALSE;
            }
            else if(uret == PACKET_EXE_BREAK)
            {
                //当前消息的解析执行将停止
                //直到下个循环时才继续对缓存中的数据进行消息格式化和执行。
                //当需要将客户端的执行从一个场景转移到另外一个场景时：
                //需要在发送转移消息后将执行在本线程中停止。
                if(pMsg) 
                    g_pPacketFactoryManager->RemovePacket(pMsg);
                break;
            }
            //继续解析剩下的消息
            else if(uret == PACKET_EXE_CONTINUE)
            {
                //LOGDEBUG("[Player::ProcessCommand] uret == PACKET_EXE_CONTINUE");
            }
            //继续解析剩下的消息，并且不回收当前消息
            else if(uret == PACKET_EXE_NOTREMOVE)
            {
                bNeedRemove = FALSE;
                LOGERROR("[Player::ProcessCommand] uret == PACKET_EXE_NOTREMOVE");
            }
            else if(uret == PACKET_EXE_NOTREMOVE_ERROR)
            {
                LOGERROR("[Player::ProcessCommand] uret == PACKET_EXE_NOTREMOVE_ERROR");
                return FALSE;
            }
            //未知的返回值
            else
            {
                CHECKF(FALSE);
            }				

            if(pMsg && bNeedRemove) 
                g_pPacketFactoryManager->RemovePacket(pMsg);	      
                
            return TRUE;	
        }
    }
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	SendPacket
//
//-----input------
// Parameters
//		Option			：执行部分选项操作
//------output------
// Return 
//		无
BOOL Player::SendPacket(CNetMsg* pMsg)
{
	DEBUG_TRY	
    CHECKF(pMsg);
    CHECKF(m_pSocketOutputStream);

    if(IsDisconnect())
        return TRUE;

    if(m_pSocketOutputStream != NULL)
    {
        pMsg->SetPacketIndex(m_PacketIndex++);

        //UINT nSizeBefore = m_pSocketOutputStream->Length();

        //写入包头信息
        //包ID
        PacketID_t packetID = pMsg->GetPacketID();
        UINT w = m_pSocketOutputStream->Write((const CHAR*)&packetID, sizeof(PacketID_t));
        if (w  ==  0)
        {
            return FALSE;
        }

        //时间间隔tick
        UINT packetTick = g_pTimeManager->RunTick();
        w = m_pSocketOutputStream->Write((const CHAR*)&packetTick , sizeof(UINT));
        if (w  ==  0)
        {
            return FALSE;
        }

        //包的size 和 index
        UINT packetUINT;
        UINT packetSize = pMsg->GetPacketSize();
        UINT packetIndex = pMsg->GetPacketIndex();
        SET_PACKET_INDEX(packetUINT, packetIndex);
        SET_PACKET_LEN(packetUINT, packetSize);
        w = m_pSocketOutputStream->Write((CHAR*)&packetUINT, sizeof(UINT));
        if (w  ==  0)
        {
            return FALSE;
        }

        //写入包体body信息
        BOOL ret = pMsg->Write(*m_pSocketOutputStream);
        CHECKF(ret);

        //LOGDEBUG( "./Log/World包.txt", "发送包 [pMsg%d, ID=%d，size=%d]", 
        //	m_pSocketOutputStream->m_pSocket->m_Port, pMsg->GetPacketID() ,pMsg->GetPacketSize ());

        //UINT nSizeAfter = m_pSocketOutputStream->Length();

        //if(pMsg->GetPacketSize() != nSizeAfter-nSizeBefore-PACKET_HEADER_SIZE)
        {
            //LOGDEBUG( WORLD_LOGFILE, "!!!!!!!PacketSizeError! ID=%d(Write%d,Should%d)", 
            //	pMsg->GetPacketID(), 
            //	nSizeAfter-nSizeBefore-PACKET_HEADER_SIZE, pMsg->GetPacketSize());
        }
        //LOGDEBUG( WORLD_LOGFILE, "SendPacket! ID=%d", pMsg->GetPacketID() );
    }
	DEBUG_CATCHF("Player::SendPacket")	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	ResetKick
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
VOID Player::ResetKick()
{
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	HeartBeat
//
//-----input------
// Parameters
//		dwTime			：时间戳
//------output------
// Return 
//		无
BOOL Player::HeartBeat(DWORD dwTime)
{
    UInt32 ret = CheckTime();
    if (ret == 0)
        return FALSE;
    return TRUE;
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	UpdateNowTime
//
//-----input------
// Parameters
//------output------
// Return 
//		无
DWORD Player::UpdateNowTime()
{
    system_clock::time_point time = system_clock::now(); 
    m_time = system_clock::to_time_t (time);
    return m_time;	
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	CheckTime//USER
//
//-----input------
// Parameters
//------output------
// Return 
//		无
UInt32 Player::CheckTime()
{
	DEBUG_TRY
    //!alter by huyf 2018.02.07:设置初始时间，创建连接后，如果30秒收不到心跳包则断开连接
    //if (m_time == 0)
    //    return 1;
    //!alter end:设置初始时间，创建连接后，如果30秒收不到心跳包则断开连接

    // get now time 
    system_clock::time_point oldTime = system_clock::from_time_t(m_time); 
    system_clock::time_point time = system_clock::now(); 

    auto duration = duration_cast<seconds>(time - oldTime);
    //if ((this->GetPlayerID() != ACC_SERVER_NUM) && (this->GetPlayerID() != MTS_SERVER_NUM))
    //    LOGDEBUG("[Player::CheckTime] PID[%d], calc sec[%lu]", this->GetPlayerID(), (duration.count() * seconds::period::num / seconds::period::den));

    if ((duration.count() * seconds::period::num / seconds::period::den) > g_config.ONLINE_HEART_BEAT_DETECTION_USER/*HEART_BEAT_DETECTION_USER*/)
    {
        //LOGDEBUG("[Player::CheckTime] timeout sec=[%lu] Ip=[%s] Port=[%d] PlayerID=[%d] AccountID=[%d]", (duration.count() * seconds::period::num / seconds::period::den),GetSocket()->m_Host,GetSocket()->m_Port,GetPlayerID(),GetAccountID());

        return 1; // Remove player
    }
    return 1;
	DEBUG_CATCH0("Player::CheckTime")
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	CheckTimeEx//ACC-MTS
//
//-----input------
// Parameters
//------output------
// Return 
//		无
UInt32 Player::CheckTimeEx()
{
	DEBUG_TRY
    //!alter by huyf 2018.02.07:设置初始时间，创建连接后，如果30秒收不到心跳包则断开连接
    //if (m_time == 0)
    //   return 0;
    //!alter end:设置初始时间，创建连接后，如果30秒收不到心跳包则断开连接

    // get now time 
    system_clock::time_point oldTime = system_clock::from_time_t(m_time); 
    system_clock::time_point time = system_clock::now(); 

    auto duration = duration_cast<seconds>(time - oldTime);

    if ((duration.count() * seconds::period::num / seconds::period::den) >= g_config.ONLINE_HEART_BEAT_DETECTION/*HEART_BEAT_DETECTION*/)
    { 
        //LOGDEBUG("[Player::CheckTimeEX] timeout sec=[%lu] Ip=[%s] Port=[%d] PlayerID=[%d] AccountID=[%d]", (duration.count() * seconds::period::num / seconds::period::den),GetSocket()->m_Host,GetSocket()->m_Port, GetPlayerID(),GetAccountID());

        return 1; // Remove player
    }
    return 0;
	DEBUG_CATCH0("Player::CheckTimeEx")
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	CreateNewRobot
//
//-----input------
// Parameters
//		baseProp		robot info
//------output------
// Return 
//		无
BOOL Player::CreateNewRobot(const char* robotID, UInt32 accID, UInt32 protoTypeID/*=0*/)
{
    DEBUG_TRY
    CHECKF(m_robot);
    //return m_robot->init(robotID, accID, protoTypeID);
    return TRUE;
    DEBUG_CATCHF("Player::CreateNewRobot")	
}

/*
//////////////////////////////////////////////////////////////////////
//
// func description	:	CreateNewRobot
//
//-----input------
// Parameters
//		baseProp		robot info
//------output------
// Return 
//		BOOL
BOOL Player::CreateNewPet(const char* petID, const char* ownerID, UInt32 protoTypeID)
{
	MY_TRY
	{
		CHECKF(m_pPet);
		if (m_pPet->Init(petID, ownerID, protoTypeID))
			return TRUE;
	}
	MY_CATCH
	{
		LOGERROR("CreateNewPet Is Error code:petID=[%s]\townerID=[%s]\tprotoTypeID=[%d]", petID, ownerID, protoTypeID);
	}
	return FALSE;
}
*/

//////////////////////////////////////////////////////////////////////
//
// func description	:	SendMsgToClient
//
//-----input------
// Parameters
//	
//------output------
// Return 
//		无
BOOL Player::SendMsgToClient(CNetMsg* pMsg)
{
    t_end = GetMillSecTime();

//  LOGERROR("time interval: %d\n", t_end - t_start);

    if (!pMsg)
    {
        LOGERROR("[Player::SendMsgToClient] pMsg == NULL!");
    }
    
    //日志
    UINT nPacketID = pMsg->GetPacketID();
    string strMsg(pMsg->m_pData);
    size_t nSize = strMsg.size();    
    //if (nPacketID == 2009)
    {
       LOGDEBUG("[Player::SendMsgToClient] Player=[%p] Id=[%d] nPacketID=[%d] Msg=[%s] nSize=[%d]\n", this, GetUid(), nPacketID, strMsg.c_str(), nSize);
    }

    //发送数据
    return m_pSocket->SendMsgToClient(m_pSocketOutputStream, pMsg); 
}

//////////////////////////////////////////////////////////////////////////
//根据战斗积分获取等级
UINT Player::GetFightCfgLevel(UINT nSorce)
{
    UINT nLevel = 1;    
    if (nSorce > 1000 && nSorce <= 2000)
    {
        nLevel = 1;
    }
    else if (nSorce > 2000 && nSorce <= 4400)
    {
        nLevel = 2;
    }
    else if (nSorce > 4400 && nSorce <= 9750)
    {
        nLevel = 3;
    }
    else if (nSorce > 9750 && nSorce <= 18950)
    {
        nLevel = 4;
    }
    else if (nSorce > 18950 && nSorce <= 33200)
    {
        nLevel = 5;
    }
    else if (nSorce > 33200 && nSorce <= 54000)
    {
        nLevel = 6;
    }
    else if (nSorce > 54000 && nSorce <= 83150)
    {
        nLevel = 7;
    }
    else if (nSorce > 83150 && nSorce <= 122750)
    {
        nLevel = 8;
    }
    else if (nSorce > 122750 && nSorce <= 175200)
    {
        nLevel = 9;
    }
    else if (nSorce > 175200 && nSorce <= 243200)
    {
        nLevel = 10;
    }
    else if (nSorce > 243200 && nSorce <= 329750)
    {
        nLevel = 11;
    }
    else if (nSorce > 329750 && nSorce <= 438150)
    {
        nLevel = 12;
    }
    else if (nSorce > 438150 && nSorce <= 572000)
    {
        nLevel = 13;
    }
    else if (nSorce > 572000 && nSorce <= 735200)
    {
        nLevel = 14;
    }
    return nLevel;
}

////////////////////////////////////////////////////////////////////////
//拾取战斗道具
BOOL Player::PickUpFightItem(INT nItemId)
{
    if (m_pBct)
    {
        return m_pBct->PickUpFightItem(this, nItemId);        
    }
    return FALSE;
}

//发放观看视屏3倍率奖励
VOID Player::AwardThreeMulti()
{
    INT nKillSum = 0;
    UINT nAwardGold = 0;
    INT nAddGold = BattleCfg()->GetBccPerKillAwardGold();
    INT nViewRatio = BattleCfg()->GetBccAwardEndViewRatio();
    INT nKingRatio = BattleCfg()->GetBccAwardKingViewRatio();
    INT nAddTotalKingSum = 0;
    INT nAddTotalGoldSum = 0;

    //霸主额外奖励霸主令牌(一种货币)
    if (GetKingRank() == 100)
    {
        //本次战斗总共奖励的霸主令牌数量：3+1倍
        nAddTotalKingSum = BattleCfg()->GetBccAwardKingAmount() * (nKingRatio + BattleCfg()->GetBccAwardKingBaseRatio());
        //本次三倍奖励的霸主令牌数量
        INT nTotalKingSum = GetKingSum() + BattleCfg()->GetBccAwardKingAmount() * nKingRatio;
        SetKingSum(nTotalKingSum);
        SetKingRank(0);
    }

    //奖励金币
    nKillSum = GetFightKillSum();
    nAwardGold = (GetAddGold() + BattleCfg()->GetBccAwardBaseRatio()) * nKillSum * nAddGold * nViewRatio; 
    SetGoldSum(GetGoldSum() + nAwardGold);

    //前端需要总的金币数量：开始BUFF的奖励倍数+战斗结束看视频的奖励倍数
    nAddTotalGoldSum = nAwardGold + (GetAddGold() + BattleCfg()->GetBccAwardBaseRatio()) * nKillSum * nAddGold;

    //同步前端最终的普通+3倍奖励
    CMsgBattle msg1;
    if (msg1.CreateMsg(_BATTLE_ACTION_VIEW_AWARD_SUC, _BATTLE_MODEL_CONFUSION, nAddTotalKingSum, GetUid(), nAddTotalGoldSum, 0))
    {
        SendMsgToClient(&msg1);                      
    }

    //同步玩家到前端
    CMsgUserInfo msg2;
    if(msg2.CreateMsg(this))
    {
        SendMsgToClient(&msg2);
    }
}
//发放观看视屏3倍率奖励-[红蓝对战]
 VOID Player::AwardThreeMultiFightTeam()
 {
    INT nViewRatio = BattleCfg()->GetBcftAwardViewRatio();
    UINT nAwardGold = 0;
    INT nAddTotalGoldSum = 0;
    
    
    //奖励金币
    LOGDEBUG("[Player::AwardThreeMultiFightTeam] FightTeamResult=%d", GetFightTeamResult());
    if (GetFightTeamResult() == _FIGHT_RESULT_WIN)
    {
        nAwardGold = BattleCfg()->GetBcftWinAwardGold();
    }
    else if(GetFightTeamResult() == _FIGHT_RESULT_DRAW)
    {
        nAwardGold = BattleCfg()->GetBcftDrawAwardGold();
    }
    else if(GetFightTeamResult() == _FIGHT_RESULT_DEFEAT)
    {
        nAwardGold = BattleCfg()->GetBcftDefaultAwardGold();
    }
    else
    {
        LOGERROR("[Player::AwardThreeMultiFightTeam] FightTeamResult=[%d]", GetFightTeamResult());
        return;
    }
    
    //奖励金币 
    SetGoldSum(GetGoldSum() + nAwardGold * nViewRatio);

    //清除玩家的上局战斗结果
    SetFightTeamResult(_FIGHT_RESULT_NONE);

    //前端需要总的金币数量：开始BUFF的奖励倍数+战斗结束看视频的奖励倍数
    nAddTotalGoldSum = nAwardGold * (nViewRatio + BattleCfg()->GetBcftAwardBaseRatio());

    //同步前端最终的普通+3倍奖励
    CMsgBattle msg1;
    if (msg1.CreateMsg(_BATTLE_ACTION_TEAM_VIEW_AWARD_SUC, _BATTLE_MODEL_RB_FIGHT_TEAM, 0, GetUid(), nAddTotalGoldSum, 0))
    {
        SendMsgToClient(&msg1);                      
    }

    //同步玩家到前端
    CMsgUserInfo msg2;
    if(msg2.CreateMsg(this))
    {
        SendMsgToClient(&msg2);
    }
 }


//////////////////////////////////////////////////////////////////////
///永久掩码位
BOOL Player::AddTaskMask(INT idx, BOOL bUpdate)
{
    IF_NOT(idx >= 0 && idx < 32)
        return FALSE;

    UInt32 i32Mask =0;
    if(idx < 32)
    {
        i32Mask = m_UserData.GetTaskMask()|(1<<idx);//(INT64_C(1)<<idx);
        m_UserData.SetTaskMask(i32Mask);
    }

    //掩码修改时给客户端发送消息
    CMsgUserAttrib msg;
    IF_OK(msg.CreateMsg(_ACT_USERATTRIB_TASKMASK, i32Mask))
        SendMsgToClient(&msg);
    
    return TRUE;
}

//////////////////////////////////////////////////////////////////////
///清楚永久任务掩码位
VOID Player::ClrTaskMask(INT idx, BOOL bUpdate)
{
    IF_NOT(idx >= 0 && idx < 32)
        return;

    if(idx < 32)
    {
        UInt32 i32Mask = m_UserData.GetTaskMask();
        i32Mask &= ~(1<<idx);
        m_UserData.SetTaskMask(i32Mask);
    }
}

//////////////////////////////////////////////////////////////////////
///检测永久任务掩码位
BOOL Player::ChkTaskMask(INT idx)
{
    IF_NOT(idx >= 0 && idx < 32)
        return FALSE;

    if(idx < 32)
    {
        return (m_UserData.GetTaskMask() & (1<<idx)) != 0;
    }
    return FALSE;
}

/////////////////////////////////////////////////////////////////////////////////
//每周任务相关
BOOL Player::AddWeekTaskMask(INT idx, BOOL bUpdate)
{
    IF_NOT(idx >= 0 && idx < 32)
        return FALSE;

    UInt32 i32WeekMask = 0;
    if(idx < 32)
    {
        i32WeekMask = m_UserData.GetWeekTaskMask()|(1<<idx);
        m_UserData.SetWeekTaskMask(i32WeekMask, bUpdate);
        //掩码修改时给客户端发送消息
        CMsgUserAttrib msg;
        IF_OK(msg.CreateMsg(_ACT_USERATTRIB_WEEKTASKMASK_FIR, i32WeekMask))
            SendMsgToClient(&msg);
    }
    // else if(idx < 64)
    // {
    //     i32WeekMask = m_UserData.GetDayTaskMask1()|(1<<(idx-32));
    //     m_UserData.SetDayTaskMask1(i32WeekMask, bUpdate);
    //     //掩码修改时给客户端发送消息
    //     //CMsgUserAttrib msg;
    //     //IF_OK(msg.CreateMsg(_ACT_USERATTRIB_DAYTASKMASK_SEC, i32WeekMask))
    //     //    SendMsgToClient(&msg);
    // }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////
VOID Player::ClrWeekTaskMask(INT idx, BOOL bUpdate)
{
    IF_NOT(idx >= 0 && idx < 32)
        return;

    UInt32 i32WeekMask = 0;
    if(idx < 32)
    {
        i32WeekMask = m_UserData.GetWeekTaskMask();
        i32WeekMask &= ~(1<<idx);
        m_UserData.SetWeekTaskMask(i32WeekMask, bUpdate);

        //掩码修改时给客户端发送消息
        CMsgUserAttrib msg;
        IF_OK(msg.CreateMsg(_ACT_USERATTRIB_WEEKTASKMASK_FIR, i32WeekMask))
            SendMsgToClient(&msg);
    }
    // else if(idx < 64)
    // {
    //     i32WeekMask = m_UserData.GetDayTaskMask1();
    //     i32WeekMask   &= ~(1<<(idx-64));
    //     m_UserData.SetDayTaskMask1(i32WeekMask, bUpdate);

    //     //掩码修改时给客户端发送消息
    //     // CMsgTaskDetail msg;
    //     // IF_OK(msg.CreateOnlyForTaskMask(_TASKHINT_SET_DAYTASKMASK_SEC, i32WeekMask))
    //     //     SendMsgToClient(&msg);
    // }
}

/////////////////////////////////////////////////////////////////////////////////
//每周任务相关
VOID Player::ClrWeekTaskMaskAll(BOOL bUpdate, BOOL bSynMsg/*=TRUE*/)
{
    m_UserData.SetWeekTaskMask(0, bUpdate);
    
    {
        if (bSynMsg)
        {
            //掩码修改时给客户端发送消息,当前端收到此消息后，可以充值所有任务未原始数据。
            CMsgUserAttrib msg;
            IF_OK(msg.CreateMsg(_ACT_USERATTRIB_WEEKTASKMASK_FIR, 0))
                SendMsgToClient(&msg);
        }
        
    }

    {
        //掩码修改时给客户端发送消息
        // CMsgTaskDetail msg;
        // IF_OK(msg.CreateOnlyForTaskMask(_TASKHINT_SET_DAYTASKMASK_SEC, 0))
        //     SendMsgToClient(&msg);
    }
}

//////////////////////////////////////////////////////////////////////
BOOL Player::ChkWeekTaskMask(INT idx)
{
    IF_NOT (idx >= 0 && idx < 32)
        return FALSE;

    if(idx < 32)
    {
        return (m_UserData.GetWeekTaskMask() & (1<<idx)) != 0;
    }
    // else if(idx < 64)
    // {
    //     return (m_UserData.GetWeekTaskMask2() & (1<<(idx-32))) != 0;
    // }
    
    return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////
//每周任务相关
VOID Player::ResetWeekTaskMask(BOOL bUpdate, BOOL bSynMsg/*=TRUE*/)///重置任务掩码
{ 
    DEBUG_TRY
    {
        SetCurWeekNum(TRUE);       
        ClrWeekTaskMaskAll(bUpdate, bSynMsg);   

        LOGDEBUG("[Player::ResetDayTaskMask] Is Suc...");   
    }
    DEBUG_CATCH("[Player::ResetDayTaskMask] Is Crash!!!")
  
}

/////////////////////////////////////////////////////////////////////////////////
//每日任务相关
BOOL Player::AddDayTaskMask(INT idx, BOOL bUpdate)
{
    IF_NOT(idx >= 0 && idx < 64)
        return FALSE;

    UInt32 i32DayMask = 0;
    if(idx < 32)
    {
        i32DayMask = m_UserData.GetDayTaskMask()|(1<<idx);
        m_UserData.SetDayTaskMask(i32DayMask, bUpdate);
        //掩码修改时给客户端发送消息
        CMsgUserAttrib msg;
        IF_OK(msg.CreateMsg(_ACT_USERATTRIB_DAYTASKMASK_FIR, i32DayMask))
            SendMsgToClient(&msg);
    }
    else if(idx < 64)
    {
        i32DayMask = m_UserData.GetDayTaskMask1()|(1<<(idx-32));
        m_UserData.SetDayTaskMask1(i32DayMask, bUpdate);
        //掩码修改时给客户端发送消息
        //CMsgUserAttrib msg;
        //IF_OK(msg.CreateMsg(_ACT_USERATTRIB_DAYTASKMASK_SEC, i32DayMask))
        //    SendMsgToClient(&msg);
    }

    return TRUE;
}

//////////////////////////////////////////////////////////////////////
VOID Player::ClrDayTaskMask(INT idx, BOOL bUpdate)
{
    IF_NOT(idx >= 0 && idx < 64)
        return;

    UInt32 i32DayMask = 0;
    if(idx < 32)
    {
        i32DayMask = m_UserData.GetDayTaskMask();
        i32DayMask &= ~(1<<idx);
        m_UserData.SetDayTaskMask(i32DayMask, bUpdate);

        //掩码修改时给客户端发送消息
        CMsgUserAttrib msg;
        IF_OK(msg.CreateMsg(_ACT_USERATTRIB_DAYTASKMASK_FIR, i32DayMask))
            SendMsgToClient(&msg);
    }
    else if(idx < 64)
    {
        i32DayMask = m_UserData.GetDayTaskMask1();
        i32DayMask   &= ~(1<<(idx-64));
        m_UserData.SetDayTaskMask1(i32DayMask, bUpdate);

        //掩码修改时给客户端发送消息
        // CMsgTaskDetail msg;
        // IF_OK(msg.CreateOnlyForTaskMask(_TASKHINT_SET_DAYTASKMASK_SEC, i32DayMask))
        //     SendMsgToClient(&msg);
    }
}

/////////////////////////////////////////////////////////////////////////////////
//每日任务相关
VOID Player::ClrDayTaskMaskAll(BOOL bUpdate, BOOL bSynMsg/*=TRUE*/)
{
    m_UserData.SetDayTaskMask(0, bUpdate);
    m_UserData.SetDayTaskMask1(0, bUpdate);
    
    {
        if (bSynMsg)
        {
           //掩码修改时给客户端发送消息,当前端收到此消息后，可以充值所有任务未原始数据。
            CMsgUserAttrib msg;
            IF_OK(msg.CreateMsg(_ACT_USERATTRIB_DAYTASKMASK_FIR, 0))
                SendMsgToClient(&msg);
        }
    }

    {
        //掩码修改时给客户端发送消息
        // CMsgTaskDetail msg;
        // IF_OK(msg.CreateOnlyForTaskMask(_TASKHINT_SET_DAYTASKMASK_SEC, 0))
        //     SendMsgToClient(&msg);
    }
}

//////////////////////////////////////////////////////////////////////
BOOL Player::ChkDayTaskMask(INT idx)
{
    IF_NOT (idx >= 0 && idx < 64)
        return FALSE;

    if(idx < 32)
    {
        return (m_UserData.GetDayTaskMask() & (1<<idx)) != 0;
    }
    else if(idx < 64)
    {
        return (m_UserData.GetDayTaskMask1() & (1<<(idx-32))) != 0;
    }
    
    return FALSE;
}
/////////////////////////////////////////////////////////////////////////////////
//每日任务相关
VOID Player::ResetDayTaskMask(BOOL bUpdate, BOOL bSynMsg/*=TRUE*/)///重置天任务掩码
{ 
    DEBUG_TRY
    {
        SetLastResetTaskTime();       
        ClrDayTaskMaskAll(bUpdate, bSynMsg);   

        UINT nTaskType = DAY_TASK_DETAIL_TYPE;  //日常任务类型-任务编号 2000 001~999 支持999种日常任务
        ResetDayTaskMaskCompltetNum(nTaskType, bUpdate, bSynMsg);   

        nTaskType = DAY_TASK_DETAIL_TRY_GAME_TYPE;  //试玩任务类型-任务编号 3000 001~999 支持999种试玩任务
        ResetDayTaskMaskCompltetNum(nTaskType, bUpdate, bSynMsg);      

        LOGDEBUG("[Player::ResetDayTaskMask] Is Suc...");   
    }
    DEBUG_CATCH("[Player::ResetDayTaskMask] Is Crash!!!")
  
}

/////////////////////////////////////////////////////////////////////////////////
//清理所有每日任务completenum字段
VOID Player::ResetDayTaskMaskCompltetNum(UINT nTaskType, BOOL bUpdate, BOOL bSynMsg/*=TRUE*/)///重置completenum字段
{ 
    DEBUG_TRY
   
    m_pTaskDetailManager->ResetDayTaskMaskCompltetNum(nTaskType, bUpdate, bSynMsg);  
   
    DEBUG_CATCH("[Player::ResetDayTaskMaskCompltetNum] Is Crash!!!")
}

/////////////////////////////////////////////////////////////////////////////////
//设置在线领奖
BOOL Player::SetOnLineAward(UINT nIdx, BOOL bSynMsg/*=TRUE*/)
{
    //查看awardid奖励组配置                    
    COnLineAwardCfgManager* pOnLineAwardCfgManager = QueryOnLineAwardCfgManager();
    if(!pOnLineAwardCfgManager)
    {
        LOGERROR("[Player::SetOnLineAward] pItemAwardManager == NULL");
        return FALSE;
    }
    COnLineAwardCfg* pOnLineAwardCfg = pOnLineAwardCfgManager->QueryOnLineAwardCfg(nIdx);
    if(!pOnLineAwardCfg)
    {
        LOGERROR("[Player::SetOnLineAward] pOnLineAwardCfg == NULL nIdx=[%d]", nIdx);
        return FALSE;
    }

    //设置本阶段奖励索引
    this->SetOnLineIdx(nIdx);

    //同步前端属性变更包
    //...暂时只做服务器保持，客户端暂时不用
    
    //设置本阶段道具
    UINT nItemType = pOnLineAwardCfg->GetItemType();
    this->SetOnLineItemType(nItemType);

    if (bSynMsg)
    {
        //同步前端属性变更包
        CMsgUserAttrib msg;
        IF_OK(msg.CreateMsg(_ACT_USERATTRIB_ONLINE_ITEM_TYPE, nItemType))
        {
            this->SendMsgToClient(&msg);
        }
    }

    //设置本阶段时间戳
    UINT nIntervalTime = pOnLineAwardCfg->GetIntervalTime();
    this->SetOnLineRemainTs(nIntervalTime);

    //启动时钟计时器
    this->QueryOnLineInterval().Startup(nIntervalTime);

    if (bSynMsg)
    {
        //同步前端属性变更包
        CMsgUserAttrib msg2;
        IF_OK(msg2.CreateMsg(_ACT_USERATTRIB_ONLINE_REMAIN_Ts, nIntervalTime))
        {
            this->SendMsgToClient(&msg2);
        }
    }

    LOGDEBUG("[Player::SetOnLineAward] nIdx=[%d] nItemType=[%d] nIntervalTime=[%d]", nIdx,nItemType,nIntervalTime);
    return TRUE;
}

//触发玩家日常任务接口
BOOL Player::TriggerDayTaskDetail(UINT nTaskId)
{
    if (m_pTaskDetailManager)
    {
        //查找配置信息
        UINT nCompleteNumCfg = 0;
        CTaskDetailCfgManager* pTaskDetailCfgManager = QueryTaskDetailCfgManager();
        if (pTaskDetailCfgManager)
        {
            CTaskDetailCfg* pTaskDetailCfg = pTaskDetailCfgManager->QueryTaskDetailCfgInfo(nTaskId);
            if (pTaskDetailCfg)
            {
                nCompleteNumCfg = pTaskDetailCfg->GetCompleteNum();
            }
            else
            {
                LOGERROR("[Player::TriggerDayTaskDetail] pTaskDetailCfg == NULL TaskId=[%d]", nTaskId);
                return FALSE;
            }
        }
        else
        {
            LOGERROR("[Player::TriggerDayTaskDetail] pTaskDetailCfgManager == NULL");
            return FALSE;
        }

        //找到玩家任务数据
        CTaskDetailData* pTaskDetailData = m_pTaskDetailManager->QueryTaskDetailDataByTaskId(nTaskId);
        if (pTaskDetailData)
        {
            if (pTaskDetailData->GetCompleteNum() < nCompleteNumCfg)
            {
                //找到日常任务更新到数据库
                pTaskDetailData->SetCompleteNum(pTaskDetailData->GetCompleteNum() + 1, TRUE);

                //更新任务数据
                CMsgTaskDetail msg;
                IF_OK(msg.CreateMsg(_TASK_DETAIL_UPDATE, this, pTaskDetailData, pTaskDetailData->GetCompleteNum()))
                {
                    SendMsgToClient(&msg);
                }
            }
        }
        else
        {
            LOGERROR("[Player::TriggerDayTaskDetail] pTaskDetailData == NULL TaskId=[%d]", nTaskId);
            return FALSE;
        }
    }
    else
    {
        LOGERROR("[Player::TriggerDayTaskDetail] pTaskDetailData == NULL TaskId=[%d]", nTaskId);
        return FALSE;
    }

    return TRUE;
}
//解锁车辆改造
BOOL Player::RemakeUnlockCar()
{
    std::string strMidVar("");                    //
    static int nEnOffset = 1;                     //","字节数：英：1
    size_t unPosLeft = std::string::npos + 1;
    size_t unPosRight = std::string::npos + 1;
    size_t unIndex = 0;
    string strUnlockSum = this->GetUnlockSum();
    if(!strUnlockSum.empty())
    {
        while(std::string::npos != (unPosRight = strUnlockSum.find(",", unPosRight)))
        {
            strMidVar = strUnlockSum.substr(unPosLeft, unPosRight - unPosLeft);
            if (!strMidVar.empty())
            {
                UINT nItemType = std::stoi(strMidVar);
                if (nItemType != 0)
                {
                    //插入数据库相关数据
                    if(m_pItemDataManager)
                    {
                        nItemType = 100000 + nItemType*10;
                        CItemData* pItemData = m_pItemDataManager->AwardItem(nItemType);
                        if(pItemData)
                        {
                            LOGDEBUG("[Player::RemakeUnlockCar] AwardItem Is Suc Id=[%d] nItemType=[%d]", pItemData->GetId(), nItemType);
                        }
                        else
                        {
                            LOGERROR("[Player::RemakeUnlockCar] AwardItem Is Error nItemType=[%d]", nItemType);
                        }
                    }
                    else
                    {
                        LOGERROR("[Player::RemakeUnlockCar] m_pItemDataManager==NULL");
                        return FALSE;
                    }
                }
                else
                {
                    LOGERROR("[Player::RemakeUnlockCar] nItemType==0");
                }
                
            }
            else
            {
                LOGERROR("[Player::RemakeUnlockCar] strMidVar Is empty");
            }

            unPosLeft += unPosRight - unPosLeft + nEnOffset;
            unPosRight++;
            unIndex = unPosRight;
        }

        //处理结尾数字
        strMidVar = strUnlockSum.substr(unIndex);
        if (!strMidVar.empty())
        {
            UINT nItemType = std::stoi(strMidVar);
            if (nItemType != 0)
            {
                //插入数据库相关数据
                if(m_pItemDataManager)
                {
                    nItemType = 100000 + nItemType*10;
                    CItemData* pItemData = m_pItemDataManager->AwardItem(nItemType);
                    if(pItemData)
                    {
                        LOGDEBUG("[Player::RemakeUnlockCar] AwardItem Is Suc Id=[%d] nItemType=[%d]", pItemData->GetId(), nItemType);
                    }
                    else
                    {
                        LOGERROR("[Player::RemakeUnlockCar] AwardItem Is Error nItemType=[%d]", nItemType);
                        return FALSE;
                    }
                }
                else
                {
                    LOGERROR("[Player::RemakeUnlockCar] m_pItemDataManager==NULL");
                    return FALSE;
                }
            }
            else
            {
                LOGERROR("[Player::RemakeUnlockCar] nItemType==0");
            }
            
        }
        else
        {
            LOGERROR("[Player::RemakeUnlockCar] strMidVar Is empty");
        }
    }

    return TRUE;
}


//金币修改为道具形式-整改
VOID Player::SetGoldSumTransformItem(UInt32 nGoldSum, BOOL bUpdate/*=TRUE*/)
{
    if (m_pItemDataManager)
    {
        //金币 130010 碎片 140010
        if(m_pItemDataManager->SetItemData(130010, _ITEMATTRIB_AMOUNT, nGoldSum))
        {
        }
        else
        {
            LOGERROR("[Player::SetGoldSumTransformItem] SetItemData Is Error nGoldSum=[%d]", nGoldSum);
        }
    }
    else
    {
        LOGERROR("[Player::SetGoldSumTransformItem] m_pItemDataManager == NULL nGoldSum=[%d]", nGoldSum);
    }

    return;
}

UInt32 Player::GetGoldSumTransformItem()
{
    UINT nAmount = 0;
    CItemData* pItemData = m_pItemDataManager->QueryItemTypeData(130010);
    if (pItemData)
    {
        nAmount = pItemData->GetAmount();
    }

    return nAmount;
}

//奖励道具
CItemData* Player::AwardItem(UINT nItemType, UINT nAmount/*=1*/)
{
    CHECKF(m_pItemDataManager);
    return m_pItemDataManager->AwardItem(nItemType, nAmount);
}