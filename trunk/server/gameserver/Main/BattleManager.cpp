//////////////////////////////////////////////////////////////////////
//文件名称：CBattleManager.cpp
//功能描述：
//版本说明：CBattleManager API
//			
//编写作者：2018.10.10 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#include "BattleManager.h"
#include "Battle.h"

//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////
 
//全局变量
CBattleManager* g_pBattleManager = NULL;

CBattleManager::CBattleManager()
{
	m_pBattle = new CBattle();
}

CBattleManager::~CBattleManager()
{
	SAFE_DELETE(m_pBattle);  
}

//LOOP 接口
BOOL CBattleManager::Loop()
{
	//循环处理大乱斗数据
	if(m_pBattle)
	{
		m_pBattle->Loop();
		return TRUE;
	}
	return FALSE;
}
