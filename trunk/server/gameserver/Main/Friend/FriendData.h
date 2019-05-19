//////////////////////////////////////////////////////////////////////
//文件名称：CFriendData.h
//功能描述：
//版本说明：CFriendData API
//			
//编写作者：2018.11.22 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#ifndef __FRIEND_DATA_H__
#define __FRIEND_DATA_H__
#include "../../Common/Type.h"
#include "../../Common/BaseType.h"
#include "../../Common/Config.h"
#include "../../Common/Common.h"

#include "../../Common/GameObj.h"
#include "../BaseObj/ObjData.h"

//////////////////////////////////////////////////////////////////////
enum _FRIEND_DATA 
{
	_FRIEND_DATA_ID = 0,					// 自增量ID
	_FRIEND_DATA_USERID = 1,				// 玩家ID
	_FRIEND_DATA_FRIENDID = 2,				// 好友id
	_FRIEND_DATA_GRANDINGID = 3,			// 段位id
	_FRIEND_DATA_CURSTARSUM = 4,			// 当前星星数量
	_FRIEND_DATA_NICKNAME = 5,				// 昵称
	_FRIEND_DATA_HEADPICURL = 6, 			// 头像	
};
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
class CFriendData : public CGameObj, public CObjData
{
public:
	CFriendData();
	virtual ~CFriendData();
	virtual	UINT	GetID()	{return 0;}

public:
	static CFriendData* 	CreateNew()	{	return new CFriendData();	}
	BOOL					Release()					{ 	delete this; return TRUE; 		}
	//类构造函数调用-内部使用
	VOID 					Init();
	//OnTimer函数
	BOOL 					OnTimer();
	//设置归属者指针
	//BOOL					SetOwner(Player* pPlayer)						{	CHECKF(pPlayer);  m_pOwner = pPlayer;  return TRUE;}
	BOOL					SetOwner(Player* pPlayer)						{	return CObjData::SetOwner(pPlayer);	}
	//保存信息
	BOOL 					SaveInfo(BOOL bUpdate = TRUE);
	
public:

	UINT	GetId()															{	return m_unId;			}
	BOOL	SetId(UINT nId)													{	m_unId = nId; return TRUE;			}
	
	UINT	GetUid()														{	return m_unUid;			}
	BOOL 	SetUid(UINT nUid, BOOL bUpdate = TRUE);

	UINT	GetFriendId()													{	return m_unFriendId;	}
	BOOL	SetFriendId(UINT nFriendId, BOOL bUpdate = TRUE);

	UINT	GetGrandingId()													{	return m_nGrandingId;	}
	BOOL	SetGrandingId(UINT nGrandingId, BOOL bUpdate = TRUE);

	UINT	GetCurStarSum()													{	return m_nCurStarSum;	}
	BOOL	SetCurStarSum(UINT nCurStarSum, BOOL bUpdate = TRUE);

	string&	GetNickName()													{ 	return m_strNickName; 	}
	BOOL	SetNickName(string strNickName, BOOL bUpdate = TRUE);
	
	string&	GetHeadPicurl()													{ 	return m_strHeadPicurl; }
	BOOL	SetHeadPicurl(string strHeadPicurl, BOOL bUpdate = TRUE);	

protected:
	UINT    m_unId;								// 自增量ID
	UINT    m_unUid;							// 玩家ID
	UINT    m_unFriendId;						// 好友id
	UINT    m_nGrandingId;						// 段位id
	UINT    m_nCurStarSum;						// 当前星星数量
	string  m_strNickName;						// 昵称
	string  m_strHeadPicurl;					// 头像

	Player* m_pOwner;
	BOOL    m_bUpdate;							// 数据是否有变更，有则进行下线数据库更新
};

#endif//__FRIEND_DATA_H__
