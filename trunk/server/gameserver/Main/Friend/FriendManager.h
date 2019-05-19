//////////////////////////////////////////////////////////////////////
//文件名称：CBattleManager.h
//功能描述：
//版本说明：CBattleManager API
//			
//编写作者：2018.11.22 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#ifndef __FRIEND_MANAGER_H__
#define __FRIEND_MANAGER_H__

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include "../../Common/Type.h"
#include "../../Common/BaseType.h"
#include "../../Common/Thread.h"
//!alter by huyf 2018.04.23:添加连接开关可配置 
#include "../../Common/Config.h"
#include "../../Common/Common.h"
//!alter end:添加连接开关可配置 
#include "../../NetBase/ClientSocket.h"
#include <vector>
#include <map>
using namespace std;
#include "FriendData.h"
#include "../../MsgHandler/NetMsg.h"

class Player;
//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////
typedef std::vector<CFriendData*> VT_FRIEND_SET;
typedef VT_FRIEND_SET::const_iterator VT_FRIEND_CIT;

//////////////////////////////////////////////////////////////////////
//class
//////////////////////////////////////////////////////////////////////
class CFriendManager
{
public:
	CFriendManager();
	virtual ~CFriendManager();
	//类构造函数调用-内部使用
	VOID 	Init();
	//OnTimer函数
	BOOL 	OnTimer();
	//设置归属者指针
	BOOL	SetOwner(Player* pPlayer)						{	m_pOwner = pPlayer;  return TRUE;}
	//用于player类中CleanUp()调用
	VOID 	CleanUp();
	//清空管理器操作，内部调用
	VOID 	Clear();
	//加载数据库好友信息
	BOOL 	LoadInfo();
	//保存好友信息
	BOOL 	SaveInfo(BOOL bUpdate = TRUE);

	//获取好友容器
	VT_FRIEND_SET& GetFriendSet() 				{	return m_vFriendSet;	}

public:	
	//添加好友
	BOOL 			AddFriend(CFriendData* pFriend);
	//删除好友
	BOOL 			DelFriend( UINT nFriendId);
	//查找好友
	CFriendData* 	QueryFriend(UINT nFriendId);
	//是否为好友
	BOOL 			IsFriend(UINT nFriendId);
	//获取好友列表信息
	BOOL 			GetListInfo();
	//广播玩家下线、上线消息给全部好友
	VOID 			BroadcastFriendSMsg(CNetMsg* pMsg, Player* pSender=NULL);
	
	//删除数据库好友记录
	BOOL 			DeleteRecord(UINT nFriendId);
	BOOL 			InsertRecord(UINT nFriendId, UINT nGrandingId, UINT nCurStarSum, std::string const& strNickName, std::string const& strHeadPicurl);

private:
	VT_FRIEND_SET 	m_vFriendSet;		//好友容器
	Player* 		m_pOwner;			//玩家指针

};

#endif //__FRIEND_MANAGER_H__