////////////////////////////////////////////////////////////////
//文件名称：T_MyQueue.h
//功能描述：队列
//T_MyQueue API
//			
//编写作者：2017.09.15 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
////////////////////////////////////////////////////////////////


#ifndef	MYQUEUE_H
#define MYQUEUE_H

#if defined(__LINUX__ )
#elif defined(__WINDOWS__)
	#pragma warning(disable:4786)
#endif

#include <deque>
using namespace std;

template<typename T>
class CMyQueue
{
// 提供默认的
//public:
	//CMyQueue();
	//~CMyQueue();

public:
	void 	push	(T obj)		{	m_set.push_back(obj);	}
	T 	 	pop		()			{	if(size()) {	T obj = *m_set.begin();	m_set.pop_front();	return obj;		}	return 0;}
	int 	size 	()			{	return m_set.size();	}
	void    clear   ()			{	m_set.clear();			}
	//QUEUE_SET& QueryList()		{ 	return m_set;			}
protected:
	typedef deque<T> QUEUE_SET;
	QUEUE_SET m_set;
	
};

#endif // MYQUEUE_H
