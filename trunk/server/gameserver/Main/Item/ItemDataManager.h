//////////////////////////////////////////////////////////////////////
//文件名称：CItemDataManager.h
//功能描述：
//版本说明：CItemDataManager API
//			
//编写作者：2018.12.13 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#ifndef __ITEMDATA_MANAGER_H__
#define __ITEMDATA_MANAGER_H__

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include "../../Common/Type.h"
#include "../../Common/BaseType.h"
//!alter by huyf 2018.04.23:添加连接开关可配置 
#include "../../Common/Config.h"
#include "../../Common/Common.h"
//!alter end:添加连接开关可配置 
#include <vector>
#include <map>
using namespace std;
#include "ItemData.h"
#include "ItemType.h"


//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////
typedef std::map<UINT, CItemData*> MAP_ITEMDATA_SET;
typedef MAP_ITEMDATA_SET::const_iterator MAP_ITEMDATA_CIT;

//////////////////////////////////////////////////////////////////////
//class
//////////////////////////////////////////////////////////////////////
class CItemDataManager
{
public:
	CItemDataManager();
	virtual ~CItemDataManager();
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
	//加载数据信息
	BOOL 	LoadInfo();
	//保存信息
	BOOL 	SaveInfo(BOOL bUpdate = TRUE);

	//获取道具管理容器
	MAP_ITEMDATA_SET& 		QueryItemDataSet() 				{	return m_mapItemDataSet;	}
	//查询道具接口
	CItemData*				QueryItemData(UINT nId);
	//获取道具列表信息
	BOOL 					GetItemDataListInfo();
public:		
	//设置道具的相关属性
	BOOL					SetItemData(UINT nItemType, UINT nItemAttrib, UINT nData, BOOL bUpdate=TRUE);
	//奖励道具
	CItemData*				AwardItem(UINT nItemType, UINT nAmount=1);
	//删除数据库记录
	BOOL 					DeleteRecord(UINT nId);
	//添加数据库记录
	BOOL 					InsertRecord(CItemData* pData);
	//添加道具
	BOOL 					AddItem(CItemData* pData);
	//删除道具
	BOOL 					DelItem(UINT nId, UINT nData=1);

	//查询道具接口
	CItemData* 				QueryItemTypeData(UINT nItemType);

private:
    MAP_ITEMDATA_SET 		m_mapItemDataSet;
    Player*					m_pOwner;		
};

#endif //__ITEMDATA_MANAGER_H__