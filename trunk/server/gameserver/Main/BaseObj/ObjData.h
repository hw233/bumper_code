//////////////////////////////////////////////////////////////////////
//文件名称：CObjData.h
//功能描述：
//版本说明：CObjData API
//			
//编写作者：2019.01.10 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0 玩家数据接口基类----请对它进行继承
//
//////////////////////////////////////////////////////////////////////

#ifndef __OBJ_DATA_H__
#define __OBJ_DATA_H__
#include "IObjData.h"

//////////////////////////////////////////////////////////////////////
///数据库表
///

//道具结构体
//
//////////////////////////////////////////////////////////////////////
///

//////////////////////////////////////////////////////////////////////
///前置声明

//////////////////////////////////////////////////////////////////////
///玩家数据接口基类
class CObjData : public IObjData
{
public:
	CObjData();
	virtual ~CObjData();
	
public:
	//子类释放自己内存
	virtual BOOL	Release();
	//类构造函数调用-内部使用
	virtual VOID 	Init();
	//OnTimer函数
	virtual BOOL 	OnTimer();
	//设置归属者指针
	virtual BOOL	SetOwner(Player* pPlayer);
	//设置该条信息是否已经被更新
	virtual BOOL 	SetUpdate(BOOL bUpdate);
	//保存信息
	virtual BOOL 	SaveInfo(BOOL bUpdate = TRUE);
protected:
	Player* m_pOwner;							// 设置归属者指针
	BOOL    m_bUpdate;							// 数据是否有变更，有则进行下线数据库更新
};

#endif//__OBJ_DATA_H__
