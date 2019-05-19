#include "Log.h"
#include "TimeManager.h"
#include "stdarg.h"
#include "../Common/Config.h"
#include "../Common/Type.h"
#include "string.h"

const CHAR* g_pLogFileName[] =
{
	"./Log/login",		//0		LOG_FILE_0
	"./Log/debug",		//1		LOG_FILE_1
	"./Log/error",		//2		LOG_FILE_2
	"./Log/functions",	//3		LOG_FILE_3
	"./Log/world",		//4		LOG_FILE_4
	"./Log/item",		//5		LOG_FILE_5
	"./Log/send",		//6		LOG_FILE_6
	"./Log/money",		//7		LOG_FILE_7
	"./Log/pet",		//8		LOG_FILE_8
	"./Log/skill",		//9		LOG_FILE_9
	"./Log/xinfa",		//10	LOG_FILE_10
	"./Log/ability",	//11	LOG_FILE_11
	"./Log/chat",		//12	LOG_FILE_12
	"./Log/mission",	//13	LOG_FILE_13
	'\0'
};

MyLock g_log_lock ;

Log* g_pLog=NULL ;

Log::Log( )
{


	for( INT i=0; i<LOG_FILE_NUMBER; i++ )
	{
		m_LogCache[i] = NULL ;
		m_LogPos[i] = 0 ;
	}
	m_CacheSize = 0 ;
	m_DayTime= 0 ;


}

Log::~Log( )
{


	for( INT i=0; i<LOG_FILE_NUMBER; i++ )
	{
		SAFE_DELETE_ARRAY(m_LogCache[i])  ;
	}
	m_CacheSize = 0 ;


}

BOOL Log::Init( )
{


	m_CacheSize = DEFAULT_LOG_CACHE_SIZE ;
	//最小化内存占用
	//______________________
	//if( g_Config.m_ConfigInfo.m_SystemModel != 0 )
	{
		for( INT i=0; i<LOG_FILE_NUMBER; i++ )
		{
			m_LogCache[i] = new CHAR[m_CacheSize] ;
			if( m_LogCache[i] == NULL )
			{
				return FALSE ;
			}
			m_LogPos[i] = 0 ;
		}
	}
	//______________________
	//if( g_pTimeManager )
	//	m_DayTime = g_pTimeManager->GetDayTime() ;
	//else
		m_DayTime = 6000 ;

	return TRUE ;



	return FALSE ;
}

VOID Log::FastSaveLog( INT logid, CHAR* msg, ... )
{


	if( logid<0 || logid >=LOG_FILE_NUMBER )
		return ;

	CHAR buffer[2048];
	va_list argptr;

	try
	{
		va_start(argptr, msg);
		vsprintf(buffer,msg,argptr);
		va_end(argptr);

		//if( g_pTimeManager )
		//{
		//	CHAR szTime[64] ;
		//	sLOG_DEBUG( szTime, " (%d)(T=%.4f)\r\n", 
		//		MyGetCurrentThreadID(),
		//		(FLOAT)(g_pTimeManager->RunTime())/1000.0 ) ;
		//	strcat( buffer, szTime ) ;
		//}
	}
	catch(...)
	{
		assert(FALSE) ;
		return ;
	}

	INT iLen = (INT)strlen(buffer) ;
	if( iLen<=0 )
		return ;

	//最小化内存占用
	//______________________
	//if( g_Config.m_ConfigInfo.m_SystemModel == 0 )
	//{
	//	CHAR szName[_MAX_PATH] ;
	//	GetLogName( logid, szName ) ;

	//	SaveLog( szName, buffer );
	//	return;
	//}
	//______________________

	m_LogLock[logid].Lock() ;
	try
	{
		memcpy( m_LogCache[logid]+m_LogPos[logid], buffer, iLen ) ;
		m_LogPos[logid] += iLen ;
		printf("---%s" , buffer ) ;
	}
	catch(...)
	{
	}
	m_LogLock[logid].Unlock() ;

	

	if( m_LogPos[logid] > (DEFAULT_LOG_CACHE_SIZE*2)/3 )
	{
		FlushLog( logid ) ;
	}
	return ;



	return ;
}

VOID Log::FlushLog( INT logid )
{


	CHAR szName[_MAX_PATH] ;
	GetLogName( logid, szName ) ;

	m_LogLock[logid].Lock() ;
	try
	{
		FILE* f = fopen( szName, "ab" ) ;
		fwrite( m_LogCache[logid], 1, m_LogPos[logid], f ) ;
		fclose(f) ;
		m_LogPos[logid] = 0 ;
	}
	catch(...)
	{
	}
	m_LogLock[logid].Unlock() ;
	return ;


}

VOID Log::FlushLog_All( )
{


	for( INT i=0; i<LOG_FILE_NUMBER; i++ )
	{
		FlushLog(i) ;
	}


}


VOID Log::GetLogName( INT logid, CHAR* szName )
{


	sprintf( szName, "%s_%d.log", g_pLogFileName[logid], m_DayTime ) ;

}

VOID Log::SaveLog( const CHAR* filename, const CHAR* msg, ... )
{


	g_log_lock.Lock( ) ;

	CHAR buffer[2048];
	memset( buffer, 0, 2048 ) ;

	/*va_list argptr;

	MY_TRY
	{
		va_start(argptr, msg);
		vsLOG_DEBUG(buffer,msg,argptr);
		va_end(argptr);

		CHAR szTime[64] ;
		//if( g_pTimeManager )
		//{
		//	memset( szTime, 0, 64 ) ;
		//	sLOG_DEBUG( szTime, " (%d)(T=%.4f)\r\n", 
		//		MyGetCurrentThreadID(),
		//		(FLOAT)(g_pTimeManager->RunTime())/1000.0 ) ;
		//	strcat( buffer, szTime ) ;
		//}
		FILE* f = fopen( filename, "ab" ) ;
		fwrite( buffer, 1, strlen(buffer), f ) ;
		fclose(f) ;

		LOG_DEBUG( buffer ) ;
	}
	MY_CATCH
	{
		LOG_DEBUG("a big error here");
	}
*/
	g_log_lock.Unlock( ) ;
	return ;

}

VOID Log::RemoveLog( CHAR* filename )
{


	g_log_lock.Lock( ) ;

	FILE* f = fopen( filename, "w" ) ;
	fclose(f) ;

	g_log_lock.Unlock( ) ;
	return ;


}

