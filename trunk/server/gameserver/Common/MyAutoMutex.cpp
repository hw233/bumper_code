////////////////////////////////////////////////////////////////
//文件名称：CMyAutoMutex.cpp
//功能描述：自动锁互斥体
//版本说明：windows操作系统需要定义宏：__WINDOWS__
//       linux操作系统需要定义宏：__LINUX__
//编写作者：2017.09.15 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
////////////////////////////////////////////////////////////////
#include "MyAutoMutex.h"


//constructor
//block:TRUR 阻塞 FALSE:非阻塞
////////////////////////////////////////////////////////////////
CMyAutoMutex::CMyAutoMutex(pthread_mutex_t* pMutex/*=NULL*/, BOOL block/*=TRUE*/):m_pMutex(pMutex),lock_var(0),end_time(time(NULL))
{

	pthread_mutex_init(m_pMutex, NULL);

	if(block)
	{
		Lock();
	}
	else
	{
		TryLock();
	}
	
}

////////////////////////////////////////////////////////////////
//desconstructor
////////////////////////////////////////////////////////////////
CMyAutoMutex::~CMyAutoMutex()
{
	UnLock();
}

////////////////////////////////////////////////////////////////
//Lock
////////////////////////////////////////////////////////////////
BOOL CMyAutoMutex::Lock()
{
	//m_pMutex = PTHREAD_MUTEX_INITIALIZER;

	pthread_mutex_lock(m_pMutex);

	return TRUE;    
}

////////////////////////////////////////////////////////////////
//Unlock
////////////////////////////////////////////////////////////////
BOOL CMyAutoMutex::UnLock()
{
	pthread_mutex_unlock(m_pMutex);

	return TRUE;    
}

////////////////////////////////////////////////////////////////
//Trylock
////////////////////////////////////////////////////////////////
BOOL CMyAutoMutex::TryLock()
{
	int ret = pthread_mutex_trylock(m_pMutex); 
	if (ret == EBUSY)
	{
		/* mutex已经被锁住的时候无法再获取锁 */
		return FALSE;
	}
	else if(ret == EINVAL)
	{
		/* mutex已经被锁住的时候无法再获取锁 */
		return FALSE;
	}
	else if(ret == EAGAIN)
	{
		/* mutex已经被锁住的时候无法再获取锁 */
		return FALSE;
	}
	return TRUE;    
}

