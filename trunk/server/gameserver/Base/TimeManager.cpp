//////////////////////////////////////////////////////////////////////
//文件名称：TimeManager.cpp
//功能描述：时间管理器
//版本说明：World API
//			
//编写作者：2017.08.22 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//include头文件
//////////////////////////////////////////////////////////////////////

#include "TimeManager.h"
#include <string.h>

//////////////////////////////////////////////////////////////////////
//全局变量
//////////////////////////////////////////////////////////////////////
TimeManager*	g_pTimeManager=NULL;

//////////////////////////////////////////////////////////////////////
//class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//
// func description	:	constructor
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
TimeManager::TimeManager()
{

	m_CurrentTime = 0;

}

//////////////////////////////////////////////////////////////////////
//
// func description	:	destructor
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		无
TimeManager::~TimeManager()
{
}

//////////////////////////////////////////////////////////////////////
//
// func description	:	Init
//
//-----input------
// Parameters
//		无
//------output------
// Return 
//		BOOL : TRUE \ FALSE
BOOL TimeManager::Init()
{
#if defined(__WINDOWS__)
	m_StartTime = GetTickCount();
	m_CurrentTime = GetTickCount();
#elif defined(__LINUX__)
	m_StartTime		= 0;
	m_CurrentTime	= 0;
	gettimeofday(&_tstart, &tz);
#endif
	SetTime();
	return TRUE;
}

UINT TimeManager::CurrentTime()
{
	m_CurrentTime = 0;
#if defined(__WINDOWS__)
	m_CurrentTime = GetTickCount();
#elif defined(__LINUX__)
	gettimeofday(&_tend,&tz);
	double t1, t2;
	t1 =  (double)_tstart.tv_sec*1000 + (double)_tstart.tv_usec/1000;
	t2 =  (double)_tend.tv_sec*1000 + (double)_tend.tv_usec/1000;
	m_CurrentTime = (UINT)(t2-t1);
#endif

	return m_CurrentTime;
}

UINT TimeManager::CurrentDate()
{
	SetTime();
	UINT Date;
	ConvertTU(&m_TM,Date);
	return Date;
}


VOID TimeManager::SetTime()
{
	time( &m_SetTime );
	tm* ptm = localtime( &m_SetTime );
	m_TM = *ptm;
}

// 得到标准时间
time_t TimeManager::GetANSITime()
{
	SetTime();
	return m_SetTime;
}

UINT TimeManager::Time2DWORD()
{
	SetTime();

	UINT uRet=0;
	uRet += GetYear();
	uRet -= 2000;
	uRet =uRet*100;

	uRet += GetMonth()+1;
	uRet =uRet*100;

	uRet += GetDay();
	uRet =uRet*100;

	uRet += GetHour();
	uRet =uRet*100;

	uRet += GetMinute();

	return uRet;
}

UINT TimeManager::DiffTime( UINT Date1, UINT Date2 )
{
	tm S_D1, S_D2;
	ConvertUT( Date1, &S_D1 );
	ConvertUT( Date2, &S_D2 );
	time_t t1,t2;
	t1 = mktime(&S_D1);
	t2 = mktime(&S_D2);
	UINT dif = (UINT)(difftime(t2,t1)*1000);
	return dif;
}

VOID TimeManager::ConvertUT( UINT Date, tm* TM )
{
	assert(TM);
	memset( TM, 0, sizeof(tm) );
	TM->tm_year = (Date>>26)&0xf;
	TM->tm_mon  = (Date>>22)&0xf;
	TM->tm_mday = (Date>>17)&0x1f;
	TM->tm_hour = (Date>>12)&0x1f;
	TM->tm_min  = (Date>>6) &0x3f;
	TM->tm_sec  = (Date)    &0x3f;
}

VOID TimeManager::ConvertTU( tm* TM, UINT& Date )
{
	assert( TM );
	Date = 0;
	Date += (TM->tm_yday%10) & 0xf;
	Date = (Date<<4);
	Date += TM->tm_mon & 0xf;
	Date = (Date<<4);
	Date += TM->tm_mday & 0x1f;
	Date = (Date<<5);
	Date += TM->tm_hour & 0x1f;
	Date = (Date<<5);
	Date += TM->tm_min & 0x3f;
	Date = (Date<<6);
	Date += TM->tm_sec & 0x3f;
}

UINT TimeManager::GetDayTime()
{
	time_t st;
	time( &st );
	tm* ptm = localtime( &m_SetTime );

	UINT uRet=0;

	uRet  = (ptm->tm_year-100)*1000;
	uRet += ptm->tm_yday;

	return uRet;
}

WORD TimeManager::GetTodayTime()
{
	time_t st;
	time( &st );
	tm* ptm = localtime( &m_SetTime );

	WORD uRet=0;

	uRet  = ptm->tm_hour*100;
	uRet += ptm->tm_min;

	return uRet;
}

BOOL TimeManager::FormatTodayTime(WORD& nTime)
{
	BOOL ret = FALSE;

	WORD wHour = nTime / 100;
	WORD wMin = nTime % 100;
	WORD wAddHour = 0;
	if( wMin > 59 )
	{
		wAddHour = wMin / 60;
		wMin = wMin % 60;
	}
	wHour += wAddHour;
	if( wHour > 23 )
	{
		ret = TRUE;
		wHour = wHour % 60;
	}

	return ret;
}
