//////////////////////////////////////////////////////////////////////
//文件名称：IBaseObj.h
//功能描述：
//版本说明：IBaseObj API
//			
//编写作者：2019.01.10 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0 玩家数据接口（抽象类）
//
//////////////////////////////////////////////////////////////////////

#ifndef __I_OBJ_DATA_H__
#define __I_OBJ_DATA_H__
#include "../../Common/Type.h"
#include "../../Common/BaseType.h"
#include "../../Common/Config.h"
#include "../../Common/Common.h"

//////////////////////////////////////////////////////////////////////
///前置声明
class Player;

//////////////////////////////////////////////////////////////////////
///玩家数据接口类
class IObjData
{
public:
	IObjData();
	virtual ~IObjData();
	
public:
	virtual BOOL	Release()											PURE_VIRTUAL_FUNCTION_0
	//类构造函数调用-内部使用
	virtual VOID 	Init()												PURE_VIRTUAL_FUNCTION_0
	//OnTimer函数
	virtual BOOL 	OnTimer()											PURE_VIRTUAL_FUNCTION_0
	//设置归属者指针
	virtual BOOL	SetOwner(Player* pPlayer)							PURE_VIRTUAL_FUNCTION_0
	//设置该条信息是否已经被更新
	virtual BOOL 	SetUpdate(BOOL bUpdate)								PURE_VIRTUAL_FUNCTION_0
	//保存信息
	virtual BOOL 	SaveInfo(BOOL bUpdate = TRUE)						PURE_VIRTUAL_FUNCTION_0
};

#endif//__I_OBJ_DATA_H__
