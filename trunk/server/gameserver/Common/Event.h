////////////////////////////////////////////////////////////////
//文件名称：Event.h
//功能描述：在linux下实现事件，主要采用条件锁的方式实现
//版本说明：Event API
//			
//编写作者：2017.09.18 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
////////////////////////////////////////////////////////////////
#ifndef	EVENT_H
#define EVENT_H

////////////////////////////////////////////////////////////////
//头文件
////////////////////////////////////////////////////////////////

#if defined(__LINUX__ )
	#include <pthread.h>  
	#include <sys/time.h> 
#elif defined(__WINDOWS__)
#endif

#include <sys/time.h>
#include <pthread.h>

////////////////////////////////////////////////////////////////
//常量
////////////////////////////////////////////////////////////////

class CEvent 
{
public:
	CEvent(bool bIsManualReset, bool bInitialSignaled);
	~CEvent();

public:
	bool CreateEvent();
	bool Set();
	bool Reset();
	bool Wait(int cms);
	bool EnsureInitialized();

private:
	bool m_bIsManualReset;
	bool m_bEventStatus;
	bool m_bMutexInitialized;
	bool m_bCondInitialized;
	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;
	
};


//CreateEvent
CEvent* CreateEvent(bool bManualReset, bool bInitialState, bool& bRet);
//SetEvent
bool SetEvent(CEvent* pEvent);
//ResetEvent
bool ResetEvent(CEvent* pEvent);
//WaitForSingleObject
bool WaitForSingleObject(CEvent* pEvent, int cms);
//CloseHandle
bool CloseHandle(CEvent* pEvent);


#endif//EVENT_H
