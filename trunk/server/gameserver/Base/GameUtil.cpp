//////////////////////////////////////////////////////////////////////
//文件名称：GameUtil.cpp
//功能描述：基本功能性函数定义
//版本说明：socket API
//			
//编写作者：2017.08.21 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include "GameUtil.h"
//#include "Type.h"

#ifdef __LINUX__
#include <execinfo.h>
#include <signal.h>
#include <exception>
#include <setjmp.h>
#include <unistd.h>
#endif

//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////

VOID MySleep( UINT millionseconds )
{
#if defined(__WINDOWS__)
	Sleep( millionseconds );
#elif defined(__LINUX__)
	usleep( millionseconds );
#endif
}

TID MyGetCurrentThreadID( )
{
#if defined(__WINDOWS__)
	return GetCurrentThreadId( );
#elif defined(__LINUX__)
	return pthread_self();
#endif
}

int RandGet(int nMax, BOOL bReset/*=FALSE*/)
{
	if( nMax == 0 )
		return 0;
	if( nMax >1000000000 )
	{
		printf("RandGet()nMax值为%d,超出精度,但仍正常返回",nMax);
	}
	//if ( bReset )
		//::srand( (unsigned)TimeGet() );

	int newRand;
	newRand=rand()%1000*10000 + rand()%10000;
	//将精度增加至亿位 /0~9999999,包括边界
	double num = 0;
	num = rand()%100*10000000;
	newRand += num;//0~999999999,包括边界

	return newRand%nMax;

}