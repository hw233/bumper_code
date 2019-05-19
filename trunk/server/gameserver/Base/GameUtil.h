//////////////////////////////////////////////////////////////////////
//文件名称：GameUtil.h
//功能描述：基本功能性函数定义
//版本说明：socket API
//			
//编写作者：2017.08.21 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#ifndef __GAMEUTIL_H__
#define __GAMEUTIL_H__
////////////////////////////////////////////////////////////////
//定义C++可以读取c文件，下面的代码按照c的语言解析来编译
////////////////////////////////////////////////////////////////
/*#ifndef __cpluscplus  
extern "C"  //C++  
{  
#endif */

//#ifndef __WINDOWS__
//	#define __WINDOWS__
//#endif
#ifndef __LINUX__
	#define __LINUX__
#endif

#include "../Common/Type.h"
#include "../Common/GameDefine.h"
//#include "GameStruct.h"
#include <pthread.h> //新添加

#define __PI			3.1415f
#define __HALF_PI		__PI / 2
#define __QUARTER_PI	__PI / 4

///////////////////////////////////////////////////////////////////////
//基本功能性模块定义
///////////////////////////////////////////////////////////////////////

class MyLock;

//共享锁
#if defined(__WINDOWS__)
	class MyLock
	{
		CRITICAL_SECTION m_Lock ;
	public :
		MyLock( ){ InitializeCriticalSection(&m_Lock); } ;
		~MyLock( ){ DeleteCriticalSection(&m_Lock); } ;
		VOID	Lock( ){ EnterCriticalSection(&m_Lock); } ;
		VOID	Unlock( ){ LeaveCriticalSection(&m_Lock); } ;
	};
#elif defined(__LINUX__)
	class MyLock
	{
		pthread_mutex_t 	m_Mutex; 
	public :
		MyLock( ){ pthread_mutex_init( &m_Mutex , NULL );} ;
		~MyLock( ){ pthread_mutex_destroy( &m_Mutex) ; } ;
		VOID	Lock( ){ pthread_mutex_lock(&m_Mutex); } ;
		VOID	Unlock( ){ pthread_mutex_unlock(&m_Mutex); } ;
	};
#endif

////////////////////////////////////////////////////////////////
//自动加锁解锁器
////////////////////////////////////////////////////////////////
class AutoLock_T
{
public:
	AutoLock_T(MyLock& rLock)
	{
		m_pLock = &rLock;
		m_pLock->Lock();
	}
	~AutoLock_T()
	{
		m_pLock->Unlock();
	}
protected:
private:
	MyLock* m_pLock;
};

////////////////////////////////////////////////////////////////
//定义时间类
////////////////////////////////////////////////////////////////	
class CMyTimer  
{
private:
	UINT m_uTickTerm;
	UINT m_uTickOld;

public:
	BOOL m_bOper;

public:	
	CMyTimer()
	{
		CleanUp() ;
	}

	BOOL IsSetTimer( ){ return m_bOper ; }
	
	VOID SetTermTime( UINT uTerm ){ m_uTickTerm =uTerm; }
	UINT GetTermTime( ){ return m_uTickTerm ; }

	UINT GetTickOldTime( ){ return m_uTickOld; }
	
	VOID CleanUp( )
	{ 
		m_uTickTerm = 0 ;
		m_bOper = FALSE ;
		m_uTickOld = 0 ;
	}

	VOID BeginTimer(UINT uTerm, UINT uNow)
	{
		m_bOper = TRUE;
		m_uTickTerm =uTerm;	
		m_uTickOld =uNow;
	}

	BOOL CountingTimer(UINT uNow)
	{
		if(!m_bOper)
			return FALSE;

		UINT uNew =uNow;

		if(uNew<m_uTickOld+m_uTickTerm )
			return FALSE;

		m_uTickOld =uNew;

		return TRUE;
	}
	UINT GetLeaveTime(UINT uNow)//剩余时间;
	{
		if(!CountingTimer(uNow))
		{
			return m_uTickTerm+m_uTickOld-uNow;
		}
		return 0;
	}
};


///////////////////////////////////////////////////////////////////////
//基本功能性函数定义
///////////////////////////////////////////////////////////////////////

//当前线程挂起一定时间
extern VOID			MySleep(UINT millionseconds=0);

extern TID			MyGetCurrentThreadID();

extern int 			RandGet(int nMax, BOOL bReset=FALSE);
/*
#ifndef __cplusplus
}  
#endif */

#endif //__GAMEUTIL_H__
