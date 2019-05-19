//////////////////////////////////////////////////////////////////////
//文件名称：CSingleMap.h
//功能描述：
//版本说明：CSingleMap API
//			
//编写作者：2018.11.5 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#include "ISingleMap.h"
#include <map>
using namespace std;

//////////////////////////////////////////////////////////////////////
///
template<TYPENAME T>
class CSingleMap : public ISingleMap<T>
{
protected:
	typedef	map<OBJID, T*>		MAP_SET;
	typedef	MAP_SET::iterator	MAP_ITER;

public:
	class	Iterator
	{
		private: // create by parent class
			Iterator(MAP_SET* pSet) : m_pSet(pSet),m_iter(pSet->begin()),m_pObj(0) {}
			friend class CSingleMap<T>;
		public:
			//Iterator(const Iterator& iter);		// need not define this ^_^
		public: // application
			bool	Next()		{ if(m_iter==m_pSet->end()) return m_pObj=NULL,false; ASSERT(m_pObj=m_iter->second); m_iter++; return true; }
			void	Reset()		{ m_iter = m_pSet->begin(); m_pObj=NULL; }
			//类型转换运算符(T*)
			operator T*()							{ return m_pObj; }
			//运算符重载(->)
			T * operator->()						{ return m_pObj; }
		protected:
			MAP_SET*	m_pSet;
			MAP_ITER	m_iter;
			T*			m_pObj;
	};
public: // traverse
	Iterator	NewEnum		() 						{ return Iterator(&m_map); }
	int			Size		() const				{ return m_map.size(); }

//////////////////////////////////////////////////////
protected:
	CSingleMap(bool bOwner) { m_bOwner = bOwner; }
	virtual ~CSingleMap();
public:
	static CSingleMap<T>*	CreateNew(bool bOwner) { return new CSingleMap<T>(bOwner); }
public:
//	typedef	map<OBJID, T*> MAPINDEX;	typedef	MAPINDEX::iterator	Iter;
	void	Release() { delete this; }
	Iter	Begin() 										{ return m_map.begin(); }
	Iter	End()											{ return m_map.end(); }
	T*		GetObjByIter(Iter i)							{ return i->second; }
	bool	AddObj(T* pObj);
	bool	DelObj(OBJID key);
	bool	DelObj(Iterator& iter);
	T*		PopObj(Iterator& iter);
	T*		GetObj(OBJID key);
	int		GetAmount()										{ return m_map.size(); }
///	OBJID		Key(iterator Iter)								{ return Iter->first; }
//	bool	IsIn(OBJID key);
protected:
	void		Clear			();
protected:
	bool		m_bOwner;
	MAPINDEX	m_map;
};

///////////////////////////////////////////////////////////////////////
///析构函数
template<TYPENAME T>
CSingleMap<T>::~CSingleMap()
{
	Clear();
}

///////////////////////////////////////////////////////////////////////
///清空m_map中的所有对象并且释放
template<TYPENAME T>
void CSingleMap<T>::Clear()
{
	if(m_bOwner && m_map.size())
	{
		for(Iter i = Begin(); i != End(); i++)
			GetObjByIter(i)->ReleaseByOwner();
	}
	m_map.clear();
}

///////////////////////////////////////////////////////////////////////
///添加
template<TYPENAME T>
bool CSingleMap<T>::AddObj(T* pObj)
{
	CHECKF(pObj && pObj->GetID());

	if(m_bOwner)
	{
		ASSERT(!DelObj(pObj->GetID()));
	}
	m_map[pObj->GetID()] = pObj;
	return true;
}

///////////////////////////////////////////////////////////////////////
///删除
template<TYPENAME T>
bool CSingleMap<T>::DelObj(Iterator& iter)
{
	if(iter.m_pObj)
	{
		if(m_bOwner)
			iter.m_pObj->ReleaseByOwner();
		return PopObj(iter) != NULL;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////
///pop
template<TYPENAME T>
T* CSingleMap<T>::PopObj(Iterator& iter)
{
	if(iter.m_pObj)
	{
		T* pObj = iter.m_pObj;
		iter.m_pObj	= NULL;

		IF_OK(iter.m_iter != m_map.begin())
		{
			MAP_ITER	temp = iter.m_iter;
			temp--;
			m_map.erase(temp);
			return pObj;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////
///删除
template<TYPENAME T>
bool CSingleMap<T>::DelObj(OBJID key)
{
	if(m_bOwner)
	{
		Iter i = m_map.find(key);
		if(i != m_map.end())
		{
			GetObjByIter(i)->ReleaseByOwner();
			m_map.erase(i);
			return true;
		}
		return false;
	}
	else
		return m_map.erase(key) != 0;
}

///////////////////////////////////////////////////////////////////////
///获取
template<TYPENAME T>
T* CSingleMap<T>::GetObj(OBJID key)
{
	Iter i;
	if((i=m_map.find(key)) != m_map.end())
		return i->second;

	return NULL;
}

/*template	< TYPENAME T >
bool	CSingleMap<T>::IsIn(OBJID key)
{
	return (m_map.find(key) != m_map.end());
}*/
