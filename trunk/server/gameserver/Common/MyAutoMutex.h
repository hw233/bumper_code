////////////////////////////////////////////////////////////////
//文件名称：CMyAutoMutex.h
//功能描述：自动锁互斥体
//版本说明：windows操作系统需要定义宏：__WINDOWS__
//       linux操作系统需要定义宏：__LINUX__
//编写作者：2017.09.15 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>	// 非阻塞的锁定互斥锁:pthread_mutex_trylock
#include "BaseType.h"

#define  return_if_fail(p) if((p) == 0){printf("[%s]:func error!/n",__FUNCTION__);return;} //linux:__FUNCTION__ windows:__func__

/*
typedef struct _MutexInfo    
{    
   pthread_mutex_t mutex;    
   int lock_var;    
   time_t end_time;    
}MutexInfo;
*/

//pthread_mutex_t q_lock = PTHREAD_MUTEX_INITIALIZER;

///////////////////////////////////////////////////////////////
//CAutoMutex
///////////////////////////////////////////////////////////////  
class CMyAutoMutex
{
public:
	explicit CMyAutoMutex(pthread_mutex_t* pMutex, BOOL block=TRUE);
	~CMyAutoMutex();

public:
	BOOL Lock 		();
	BOOL UnLock 	();
	BOOL TryLock 	();   

//禁止赋值和拷贝构造
private:
	void operator = (const CMyAutoMutex& other);  
    CMyAutoMutex(const CMyAutoMutex& other); 

private:
	pthread_mutex_t* m_pMutex;    
	int lock_var;
	time_t end_time;
};


//锁机制
#if defined(__LINUX__ )
	#define TryMyMutexOBJ		;						//尝试加锁（不阻塞），pthread_mutex_trylock()语义与pthread_mutex_lock()类似，不同的是在锁已经被占据时返回EBUSY而不是挂起等
	#define LockMyMutexOBJ		CMyAutoMutex Automtx;		//加锁（阻塞）
	#define UnlockMyMutexOBJ	;						//解锁（阻塞）
#elif defined(__WINDOWS__)
#endif