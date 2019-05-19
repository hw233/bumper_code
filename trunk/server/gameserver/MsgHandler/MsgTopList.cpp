#include "MsgTopList.h"


//////////////////////////////////////////////////// CMsgTopList ////////////////////////////////////////////////////
//构造函数
CMsgTopList::CMsgTopList() 
{ 
}

//析构函数
CMsgTopList::~CMsgTopList() 
{ 	
}

//读取数据
BOOL CMsgTopList::Read(SocketInputStream& iStream, UINT nLen)
{
	CNetMsg::Read(iStream, nLen);
	return TRUE;
}

BOOL CMsgTopList::Write(SocketOutputStream& oStream) const
{
	return TRUE;
}
BOOL CMsgTopList::CreateMsg(UINT nRetStatus, UINT nAction)
{
	return TRUE;
}
//创建消息
BOOL CMsgTopList::CreateMsg(UINT nAction, Player* pPlayer, UINT nStatus)
{

	return TRUE;
}

//创建消息
BOOL CMsgTopList::CreateMsg(UINT nAction, CFriendData* pFriendData, UINT nStatus, UINT nIndex)
{

	return TRUE;
}

//追加消息
BOOL CMsgTopList::AppendMsg(UINT nAction, CFriendData* pFriendData, UINT nStatus, UINT nIndex)
{

	return TRUE;
}

//发送json数据
BOOL CMsgTopList::SendMsg(CNetMsg* pMsg)
{
	m_pPlayer->SendMsgToClient(pMsg);
	return TRUE;	
}

//处理前端发送过来的消息
UINT CMsgTopList::Process(VOID *pInfo)
{	
	LOGDEBUG("[CMsgTopList::Process] begin");	
	return PACKET_EXE_CONTINUE;
}