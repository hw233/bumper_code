////////////////////////////////////////////////////////////////
//文件名称：Event.cpp
//功能描述：在linux下实现事件，主要采用条件锁的方式实现
//版本说明：Event API
//			
//编写作者：2017.09.18 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//头文件
////////////////////////////////////////////////////////////////
#include <time.h>
#include <sys/time.h>
#include "Event.h"
#include "Common.h"


//注意:pthread_cond_init,   pthread_cond_signal, pthread_cond_broadcast,和 pthread_cond_wait从不返回错误代码

////////////////////////////////////////////////////////////////
//construct
//注意：linux系统不同于window 有事件(Event)机制，所以不存在以下说法
// 手动/自动
// TRUE：表示手动，在WaitForSingleObject后必须手动调用ResetEvent清除信号
// FALSE：表示自动，在WaitForSingleObject后，系统自动清除事件信号
CEvent::CEvent(bool bIsManualReset, bool bInitialSignaled):m_bIsManualReset(bIsManualReset),m_bEventStatus(bInitialSignaled),m_bMutexInitialized(false),m_bCondInitialized(false)
{
	m_bEventStatus = false;
}

////////////////////////////////////////////////////////////////
//desconstruct
CEvent::~CEvent()
{
	if (m_bMutexInitialized)
	{
		pthread_mutex_destroy(&m_mutex);
		m_bMutexInitialized = false;
	}

	if (m_bCondInitialized)
	{
		pthread_cond_destroy(&m_cond);
		m_bCondInitialized = false;
	}
}

////////////////////////////////////////////////////////////////
//CreateEvent
bool CEvent::CreateEvent()
{	
	int ret = 0;
	if (!m_bMutexInitialized)
	{
		// 返回值：函数成功返回0；任何其他返回值都表示错误
		//静态:m_mutex=PTHREAD_MUTEX_INITIALIZER
		ret = pthread_mutex_init(&m_mutex, NULL);
		if(ret != 0)//动态创建
		{
			LOGCATCH("cannot create event (m_mutex)");
			return false;
		}
		else
		{
			m_bMutexInitialized = true;
		}
	}

	if (!m_bCondInitialized)
	{
		// 返回值：函数成功返回0；任何其他返回值都表示错误
		//静态:m_cond=PTHREAD_COND_INITIALIZER
		ret = pthread_cond_init(&m_cond, NULL);
		if (ret != 0)//动态
		{
			LOGCATCH("cannot create event (m_cond)");
			return false;
		}
		else
		{
			m_bCondInitialized = true;
		}
	}

	LOGDEBUG("[CEvent::CreateEvent] m_bMutexInitialized=[%d] m_bCondInitialized=[%d]", m_bMutexInitialized, m_bCondInitialized);
	return (m_bMutexInitialized && m_bCondInitialized);
}

////////////////////////////////////////////////////////////////
//EnsureInitialized
bool CEvent::EnsureInitialized()
{
	return (m_bMutexInitialized && m_bCondInitialized);
}


////////////////////////////////////////////////////////////////
//Set
bool CEvent::Set()
{
	if (!EnsureInitialized())
	{
		return false;
	}

	if(pthread_mutex_lock(&m_mutex))
	{
		LOGCATCH("[CEvent::Set] wait for event failed (lock)"); 
		return false;
	}

	m_bEventStatus = true;
	pthread_cond_broadcast(&m_cond);

	pthread_mutex_unlock(&m_mutex);
	return true;
}

////////////////////////////////////////////////////////////////
//Reset
bool CEvent::Reset()
{
	if (!EnsureInitialized())
	{
		return false;
	}

	if(pthread_mutex_lock(&m_mutex))
	{
		LOGCATCH("[CEvent::Reset] wait for event failed (lock)");
		return false;
	}

	m_bEventStatus = false;

	pthread_mutex_unlock(&m_mutex);
	return true;
}

////////////////////////////////////////////////////////////////
//Wait
// cms：纳秒
bool CEvent::Wait(int cms)
{
	if (!EnsureInitialized())
	{
		return false;
	}

	int ret = pthread_mutex_lock(&m_mutex);

	if(ret != 0)
	{
		LOGCATCH("[CEvent::Wait] wait for event failed (lock)");	
		return false;
	}

	if (cms != -1/*INFINITE*/)//立即返回或者等待超时返回
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);

		struct timespec abstime;
		abstime.tv_sec = cms / 1000*1000*1000;//秒
		abstime.tv_nsec = cms % 1000*1000*1000;//纳秒		
		
		while((!m_bEventStatus) && (0 == ret))// 唤醒后再次检查预期条件是否满足	
		{
			ret = pthread_cond_timedwait(&m_cond, &m_mutex, &abstime);
			if(ret != 0)
			{
				if (ret == ETIMEDOUT)
				{
					break;
				}
				pthread_mutex_unlock(&m_mutex);
				LOGCATCH("[CEvent::Wait] cms != INFINITE cannot wait for event");
			}			
		}
	}
	else//cms == INFINITE//永久等待
	{
		while((!m_bEventStatus) && (0 == ret))
		{
			// pthread_cond_wait会先解除之前的pthread_mutex_lock锁定的mtx，然后阻塞在等待对列里休眠，直到再次被唤醒（大多数情况下是等待的条件成立而被唤醒，唤醒后，该进程会先锁定先pthread_mutex_lock(&mtx);，再读取资源  
        	// 用这个流程是比较清楚的/*block-->unlock-->wait() return-->lock*/
        	ret = pthread_cond_wait(&m_cond, &m_mutex);
			if (ret != 0)
			{
				pthread_mutex_unlock(&m_mutex);
				LOGCATCH("[CEvent::Wait] cms == INFINITE wait for event failed");
			}			
		}
	}

	if (0 == ret && !m_bIsManualReset)
	{
		m_bEventStatus = false;
	}

	pthread_mutex_unlock(&m_mutex);

	return ret;
}

////////////////////////////////////////////////////////////////
//对外接口
//CreateEvent
CEvent* CreateEvent(bool bManualReset, bool bInitialState, bool& bRet)
{
	LOGDEBUG("[CreateEvent] bManualReset=[%d] bInitialState=[%d]", bManualReset, bInitialState);
	CEvent* pEvent = new CEvent(bManualReset, bInitialState);
	assert(pEvent);
	LOGDEBUG("[CreateEvent] bManualReset=[%d] bInitialState=[%d] pEvent=[%p]", bManualReset, bInitialState, pEvent);

	bRet = pEvent->CreateEvent();
	assert(bRet);

	return pEvent;
}

////////////////////////////////////////////////////////////////
//对外接口
//WaitForSingleObject
bool WaitForSingleObject(CEvent* pEvent, int cms)
{
	assert(pEvent);
	return pEvent->Wait(cms);
}

////////////////////////////////////////////////////////////////
//对外接口
//CloseHandle
bool CloseHandle(CEvent* pEvent)
{
	if (pEvent)
	{
		delete pEvent;
		pEvent = NULL;		
	}
	return true;
}

////////////////////////////////////////////////////////////////
//对外接口
//SetEvent
bool SetEvent(CEvent* pEvent)
{
	assert(pEvent);
	bool bRet = pEvent->Set();		
	return bRet;
}

////////////////////////////////////////////////////////////////
//对外接口
//ResetEvent
bool ResetEvent(CEvent* pEvent)
{
	assert(pEvent);
	bool bRet = pEvent->Reset();		
	return bRet;
}