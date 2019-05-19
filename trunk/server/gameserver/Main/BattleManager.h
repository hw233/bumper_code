//////////////////////////////////////////////////////////////////////
//文件名称：CBattleManager.h
//功能描述：
//版本说明：CBattleManager API
//			
//编写作者：2018.10.10 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#ifndef __BATTLEMANAGER_H__
#define __BATTLEMANAGER_H__

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include "../Common/Type.h"
#include "../Common/BaseType.h"
#include "../Common/Thread.h"
//!alter by huyf 2018.04.23:添加连接开关可配置 
#include "../Common/Config.h"
//!alter end:添加连接开关可配置 
#include "../NetBase/ClientSocket.h"


//#include "Battle.h"
class CBattle;


//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//class
//////////////////////////////////////////////////////////////////////
class CBattleManager
{
public:
	CBattleManager();
	virtual ~CBattleManager();

public:
	//获取战斗指针
	BOOL SetBattle(CBattle* pBattle) 	{	CHECKF(pBattle);	m_pBattle = pBattle;	return TRUE;	}
	CBattle* GetBattle() 				{	return m_pBattle;	}

	//LOOP 接口
	BOOL	Loop();
private:
	CBattle* m_pBattle;		//大乱斗对象指针

};
//全局变量
//
extern CBattleManager* g_pBattleManager;

//CBattleManager* 	GetBattleManager(){ return g_pBattleManager;  } 

#endif //__BATTLEMANAGER_H__