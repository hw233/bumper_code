//////////////////////////////////////////////////////////////////////
//文件名称：ISingleMap.h
//功能描述：
//版本说明：ISingleMap API
//			
//编写作者：2018.11.5 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#ifndef	ISINGLEMAP_H
#define ISINGLEMAP_H

#include "Common.h"

#include <map>
using namespace std;

template<TYPENAME T>
class ISingleMap
{
protected:
	virtual ~ISingleMap() {}
public:
	typedef	map<OBJID, T*> MAPINDEX;	
	typedef	MAPINDEX::iterator	Iter;

	virtual void		Release()							PURE_VIRTUAL_FUNCTION
	virtual Iter		Begin() 							PURE_VIRTUAL_FUNCTION_0
	virtual Iter		End()								PURE_VIRTUAL_FUNCTION_0
	virtual T*			GetObjByIter(Iter i)				PURE_VIRTUAL_FUNCTION_0
	virtual bool		AddObj(T* pObj)						PURE_VIRTUAL_FUNCTION_0
	virtual bool		DelObj(OBJID key)					PURE_VIRTUAL_FUNCTION_0
	virtual T*			GetObj(OBJID key)					PURE_VIRTUAL_FUNCTION_0
	virtual int			GetAmount()							PURE_VIRTUAL_FUNCTION_0
//	virtual OBJID		Key(iterator Iter)					PURE_VIRTUAL_FUNCTION_0
//	virtual T*			PopObj(OBJID idObj, int idType)		PURE_VIRTUAL_FUNCTION_0
};


#endif // ISINGLEMAP_H