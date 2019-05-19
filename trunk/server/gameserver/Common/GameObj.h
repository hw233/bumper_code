//////////////////////////////////////////////////////////////////////
//文件名称：IObjData.h
//功能描述：
//版本说明：IObjData API
//			
//编写作者：2019.01.10 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0 玩家数据元基础类接口（非抽象类）
//
//////////////////////////////////////////////////////////////////////

#ifndef __GAME_OBJ_H__
#define __GAME_OBJ_H__
#include "Type.h"
#include "BaseType.h"
#include "Config.h"
#include "Common.h"

//////////////////////////////////////////////////////////////////////
///前置声明
extern CHAR	szID[];

//////////////////////////////////////////////////////////////////////
///玩家数据接口类
class CGameObj
{
public:
	CGameObj();
	virtual ~CGameObj();	
	static  BOOL 	SafeCheck(CGameObj* pObj);

public:
	virtual	UINT	GetID()									PURE_VIRTUAL_FUNCTION_0
	virtual INT		GetObjType()							{	return m_nObjType;	}
	virtual VOID	SetObjType(INT nType)					{	m_nObjType = nType;	}

private:
	INT				m_nObjType;
};

#endif//__GAME_OBJ_H__

