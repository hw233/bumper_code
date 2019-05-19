////////////////////////////////////////////////////////////////
//文件名称：common.cpp
//功能描述：系统底层常用宏定义。
//版本说明：windows操作系统需要定义宏：__WINDOWS__
//       linux操作系统需要定义宏：__LINUX__
//编写作者：2017.09.06 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include "Common.h"
#include "Config.h"
//add by huyf 2018.01.24:支持I/O多路复用poll
#include "IniFile.h"
//add end:支持I/O多路复用poll

#include <random>
#include <functional>
#include <sys/timeb.h> 

#include <string.h>
#include <vector>
#include <time.h>

//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////
#define		BUFSIZE		1024

/////////////////////////////////////////////////////////////////////////////
// function
/////////////////////////////////////////////////////////////////////////////
char		log_szTitle[BUFSIZE]	= "未命名程序 V1.0";			//? 可能空间不足
char		log_szFilename[BUFSIZE]	= "Logfile";
time_t		log_tStartServerTime	= 0;


//日志开关标志位

//BOOL g_bLogSave = TRUE;
//BOOL g_bLogSave = false;


ST_CONFIG	g_config;
RobotConf 	g_robotConf_NICO;
//add by huyf 2018.01.24:支持I/O多路复用poll
int IOModelOpen(const char* IoModelName);
//static const int g_SetSize = IOModelOpen("SETSIZE");
int g_IoModelSelect = IOModelOpen("SELECT");
int g_IoModelPoll = IOModelOpen("POLL");
int g_IoModelEPoll = IOModelOpen("EPOLL");
//add end:支持I/O多路复用poll

/////////////////////////////////////////////////////////////////////////////
/*String	DumpBuffer(const char* buf, int nSize)
{
	IF_NOT(buf && nSize > 0 && nSize <= 256)
		return String();

	String str;
	for(int i = 0; i < nSize; i++)
	{
		char bufChar[255];
		sprintf(bufChar, "%02x", (*(buf++) & 0xFF));
		str	+= bufChar;
		if((i % 16) == 15 && i != nSize-1)
			str += ' ';
		else if((i % 4) == 3 && i != nSize-1)
			str	+= '-';
	}
	return str;
}*/

/////////////////////////////////////////////////////////////////////////////
// szFormat: "%04d-%02d-%02d %02d:%02d:%02d"
// 格式化日期时间串
void	DateTime(char * buf20, time_t tInput /*= 0*/)	// 填入buf20中
{
	if(!buf20)
		return;

	if(tInput == 0)
		tInput = time(NULL);

	tm * pTm = localtime(&tInput);
	if(buf20)
		sprintf(buf20, "%04d-%02d-%02d %02d:%02d:%02d", 
					pTm->tm_year+1900, pTm->tm_mon+1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
}
/////////////////////////////////////////////////////////////////////////////
// 将时间的年和月转换为整数，月两位
int	DateTimeSign(time_t tInput /*= 0*/)	
{
	if(tInput == 0)
		tInput = time(NULL);
	
	tm * pTm = localtime(&tInput);

	return (pTm->tm_year+1900)*100 + pTm->tm_mon+1;
}

/////////////////////////////////////////////////////////////////////////////
// 将时间的年和上月转换为整数，月两位
int	DateTimeUpMonthSign(time_t tInput /*= 0*/)	
{
	if(tInput == 0)
		tInput = time(NULL);
	
	tm * pTm = localtime(&tInput);
	
	int nMonth = pTm->tm_mon+1;
	int nDateSign = 0;
	if(nMonth == 1)
	{
		nDateSign = (pTm->tm_year+1900-1)*100 + 12;
	}
	else
	{
		nDateSign = (pTm->tm_year+1900)*100 + (nMonth -1);
	}

	return nDateSign;
}
////////////////////////////////////////////////////////////////////////////
// szFormat: "%04d-%02d-%02d %02d:%02d:%02d"
bool FormatDateTime(char* buf, const char* szFormat, time_t tInput /*= 0*/)
{
	if(!buf || !szFormat)
		return false;

	if(tInput == 0)
		tInput = time(NULL);

	tm * pTm = localtime(&tInput);
	sprintf(buf, szFormat, 
			pTm->tm_year+1900, pTm->tm_mon+1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);

	return true;
}

////////////////////////////////////////////////////////////////////////////
// 0 : curr time
void DateTimeStamp(char * buf15, time_t tInput /*= 0*/)	// 填入buf15中
{
	if(!buf15)
		return;

	if(tInput == 0)
		tInput = time(NULL);

	tm * pTm = localtime(&tInput);
	if(buf15)
		sprintf(buf15, "%04d%02d%02d%02d%02d%02d", 
					pTm->tm_year+1900, pTm->tm_mon+1, pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
}

////////////////////////////////////////////////////////////////////////////
int DateStamp(time_t tTime/*=0*/)
{
	time_t	tInput;
	if(tTime)
		tInput	= tTime;
	else
		tInput = time(NULL);

	tm * pTm = localtime(&tInput);
	return (pTm->tm_year+1900)	* 10000
			+ (pTm->tm_mon+1) * 100
			+ pTm->tm_mday;
}

////////////////////////////////////////////////////////////////////////////
// return: 时间标签nDate加上指定天数nDays
// return -1: error
int DateStampPass(int nDate, int nDays)
{
	//time_t	tCurr = time(NULL);
	tm	tmComp;
	memset(&tmComp, 0, sizeof(tm));
	tmComp.tm_year	= nDate/10000 - 1900;
	tmComp.tm_mon	= (nDate/100)%100 - 1;
	tmComp.tm_mday	= nDate%100 + nDays;

	time_t	tComp	= mktime(&tmComp);
	if(tComp == -1)
		return -1;

	return DateStamp(tComp);
}

//////////////////////////////////////////////////////////////////////
bool IsActiveTime(time_t tCurr, unsigned long nFormat)		// DDWWHHMMSS
{
	tm* pTm = localtime(&tCurr);
	CHECKF(pTm);

	if(nFormat / 100000000)		// day per month
	{
		if(pTm->tm_mday == int(nFormat/100000000)
			&& pTm->tm_hour == int(nFormat/10000) % 100
			&& pTm->tm_min	== int(nFormat/100) % 100
			&& pTm->tm_sec	== int(nFormat % 100)
			)
			return true;
	}
	else if(nFormat / 1000000)		// day per weak
	{
		if(((pTm->tm_wday+6)%7)+1 == int(nFormat/1000000)
			&& pTm->tm_hour == int(nFormat/10000) % 100
			&& pTm->tm_min	== int(nFormat/100) % 100
			&& pTm->tm_sec	== int(nFormat % 100)
			)
			return true;
	}
	else			// time per day
	{
		if(pTm->tm_hour == int(nFormat/10000) % 100
			&& pTm->tm_min	== int(nFormat/100) % 100
			&& pTm->tm_sec	== int(nFormat % 100)
			)
			return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
bool IsBetween(int nCurr, int nBegin, int nEnd)
{
	if(nBegin <= nEnd)
	{
		if(nBegin <= nCurr && nCurr < nEnd)
			return true;
	}
	else // if(nEnd < nBegin)
	{
		if(nCurr < nEnd || nBegin <= nCurr)
			return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
bool IsTimeRange(unsigned long nBegin, unsigned long nEnd, time_t tCurr/*=0*/)		// DDWWHHMMSS
{
	if(tCurr == 0)
		tCurr = time(NULL);

	tm* pTm = localtime(&tCurr);
	CHECKF(pTm);

	if(nBegin / 100000000)		// per month
	{
		if(IsBetween(pTm->tm_mday*100*1000000 + pTm->tm_hour*10000 + pTm->tm_min*100 + pTm->tm_sec, nBegin, nEnd))
			return true;
	}
	else		// per weak or day ...
	{
		if(IsBetween(pTm->tm_wday*1000000 + pTm->tm_hour*10000 + pTm->tm_min*100 + pTm->tm_sec, nBegin, nEnd))
			return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////
///获取当前时间是本年中第几周
int WeekNum()
{
    int nweek = 0;
    int wd = 0;
    int yd = 0;
    time_t t; /*声明time_t结构体 t 。*/
    struct tm *ptr; /*声明tm结构体指针。*/

    time(&t); /*初始化time_t结构体t 。*/
    ptr = gmtime(&t); /*获得已初始化的tm结构体。*/

    wd = ptr->tm_wday; /*今天是星期几。*/
    yd = ptr->tm_yday; //今天是今年的第几天 范围0-365 所以yd也表示今天距1月1日的总天数

    //计算本年1月1日是周几
    //int base = 7 - (yd + 1 - (wd + 1)) % 7;//yd+1表示到今天的总天数 wd+1表示到今天本周的总天数 
    int base = 7 - (yd + 1 - (wd + 0)) % 7;//yd+1表示到今天的总天数 wd+0表示到今天本周的总天数 1~7
    if (base == 7)
        base = 0; //0代表周日 一周的开始

    //计算本周是一年的第几周
    nweek = (base + yd) / 7 + 1;
    //printf("\n从1月1日算起今周是第 %d 周。\n", nweek);

    return nweek;
}

//////////////////////////////////////////////////////////////////////////
//判断两个时间是否在同一周内
/*
bool    IsSameWeek(time_t time1, time_t time2)
{
	//////////////////////////////////////////////////////////////////////////
	//确保time1或time2不能为0，否则下面的CTime构造函数会出错
	if(time1 == 0 && time2 != 0)
		return false;

	if(time2 == 0 && time1 != 0)
		return false;

	if(time1 == 0 && time2 == 0)
		return true;
	//////////////////////////////////////////////////////////////////////////
	struct tm *ptm1;
	struct tm *ptm2;
	
    ptm1 = localtime(&time1);
    ptm2 = localtime(&time2);

	CTime ct1(time1);
	CTime ct2(time2);
	
	int iWeekDay = ptm1->tm_wday*3600;
	
	CTimeSpan ts1(1-iWeekDay, 0, 0, 0);
	CTimeSpan ts2(7-iWeekDay, 0, 0, 0);
	
	CTime tSunday = ct1 + ts1;
	CTime tSaturday = ct1 + ts2;

	CTime tStart(tSunday.GetYear(),tSunday.GetMonth(),tSunday.GetDay(),0,0,1);
	CTime tEnd(tSaturday.GetYear(),tSaturday.GetMonth(),tSaturday.GetDay(),23,59,59);
	
	if(ct2 >= tStart && ct2 <= tEnd)
		return true;
	
	return false;
}

//////////////////////////////////////////////////////////////////////////
//判断两个时间是否同一天
bool   IsSameDay(time_t time1, time_t time2)
{
	//////////////////////////////////////////////////////////////////////////
	//确保time1或time2不能为0，否则下面的CTime构造函数会出错
	if(time1 == 0 && time2 != 0)
		return false;

	if(time2 == 0 && time1 != 0)
		return false;

	if(time1 == 0 && time2 == 0)
		return true;
	//////////////////////////////////////////////////////////////////////////

	CTime ct1(time1);
	CTime ct2(time2);

	if( (ct1.GetDay() != ct2.GetDay()) || (ct1.GetMonth() != ct2.GetMonth()) || (ct1.GetYear() != ct2.GetYear()) )
	{
		return false;
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
//判断两个时间是否在同一月
bool   IsSameMonth(time_t time1, time_t time2)
{
	//////////////////////////////////////////////////////////////////////////
	//确保time1或time2不能为0，否则下面的CTime构造函数会出错 
	if(time1 == 0 && time2 != 0)
		return false;

	if(time2 == 0 && time1 != 0)
		return false;

	if(time1 == 0 && time2 == 0)
		return true;
	//////////////////////////////////////////////////////////////////////////

	CTime ct1(time1);
	CTime ct2(time2);

	if( (ct1.GetMonth() != ct2.GetMonth()) || (ct1.GetYear() != ct2.GetYear()) )
	{
		return false;
	}

	return true;
}
//////////////////////////////////////////////////////////////////////////
//判断两个时间是否超过7天  
bool    IsOver_7_Day(time_t time1, time_t time2)
{
	//////////////////////////////////////////////////////////////////////////
	//这里只要time1时间为0 表示超过了7天
	if(time1 == 0 && time2 != 0)
		return true;
	//只要time1不为0，time2为0了 表示没有超过7天
	if(time2 == 0 && time1 != 0)
		return false;
	//只要time1和time2同时为0 表示没有超过7天
	if(time1 == 0 && time2 == 0)
		return false;
	//////////////////////////////////////////////////////////////////////////

	CTime ct1(time1);
	CTime ct2(time2);
	
	//向后扩延7天
	CTimeSpan ts1(7,0,0,0);
		
	ct1 += ts1;
	
	if(ct2 > ct1)//比扩延后7天还大的那就超过7天了
		return true;
	else	
		return false;
}


//////////////////////////////////////////////////////////////////////////
//判断是否为某月的最后一天 
bool    IsLastDayOfMon(time_t tInput) 
{
	if(tInput == 0)
		tInput = time(NULL);
	
	CTime ct1(tInput);
	CTimeSpan ts1(1,0,0,0);
	CTime ct2 = ct1 + ts1;
	if(ct2.GetMonth() != ct1.GetMonth())
		return true;

	return false;
}
*/
////////////////////////////////////////////////////////////////////////////
// 初始化LOG文件
void InitLog(const char * pTitle, const char * pFilename, time_t tStart)
{
	if(pTitle)
	{
		int nLen = strlen(pTitle);
		if(nLen >= BUFSIZE-1)
			nLen = BUFSIZE-1;
		strncpy(log_szTitle, pTitle, nLen+1);
		log_szTitle[BUFSIZE-1]	= 0;
	}

	if(pFilename)
	{
		int nLen = strlen(pFilename);
		if(nLen >= BUFSIZE-1)
			nLen = BUFSIZE-1;
		strncpy(log_szFilename, pFilename, nLen+1);
		log_szFilename[BUFSIZE-1]	= 0;
	}

	log_tStartServerTime = tStart;
}

void LOSEDATA(const char * sFormat, ...)
{
	va_list argptr;
	va_start( argptr, sFormat);     /* Initialize variable arguments. */

	char	buf[4096] = "";					/* ?????空间可能不足 */
	vsprintf(buf, sFormat, argptr);
	strcat(buf, "\n");
//	if(!(ret < 4096-2)) log_Save(x, "ASSERT(ret < 4096-2)");	/* ?????空间可能不足 */

	va_end( argptr );              /* Reset variable arguments.      */

	log_Save("LOSE_DATA", buf);
}

void log_Save(const char * pFilename, const char * pBuf, int nFlag/*=0*/)
{
	//写日志标志位---------
	if(!g_bLogSave)
		return;
	//printf("22222222222222222222222");
	// 写文件
	time_t tCurrTime;
	time( &tCurrTime );
	tm *	pTmFile = localtime(&tCurrTime);
	char	szLogFile[1024] = "";				//? 可能空间不足
	//!alter by huyf 2018.08.23:帮忙记录前端日志，暂时修改
	if (nFlag)
	{
		sprintf(szLogFile, "%s\/%s %04d-%02d-%02d.LOG",  LOGFILE_DIR, pFilename, pTmFile->tm_year+1900, pTmFile->tm_mon+1, pTmFile->tm_mday);
	}
	else
	{
		sprintf(szLogFile, "%s %04d-%02d-%02d.LOG",  pFilename, pTmFile->tm_year+1900, pTmFile->tm_mon+1, pTmFile->tm_mday);
	}

	FILE	* pFile;
	pFile = fopen(szLogFile, "a");
	if(pFile)
	{
		fseek(pFile, 0, SEEK_END);
		{
			if(ftell(pFile) == 0)
			{
				char buf[4096];			
				char bufStart[20];
				char bufCurr[20];
			
				DateTime(bufStart, log_tStartServerTime);
				DateTime(bufCurr, tCurrTime);
				int	nSec = (int)difftime(tCurrTime, log_tStartServerTime);
				//!alter by huyf 2018.08.23:帮忙记录前端日志，暂时修改
				if (nFlag)
				{
					sprintf(buf,	"================================================================================\n"								
								"=== Start server time is %s\n"
								"=== Current time is %s\n"
								"=== Server takes %d days  %02d:%02d:%02d\n"
								"================================================================================\n"								
								, bufStart
								, bufCurr
								, nSec / 86400, (nSec/3600)%24, (nSec/60)%60, nSec%60 );
				}
				else
				{
					sprintf(buf,	"================================================================================\n"
								"=== %s\n"
								"=== Start server time is %s\n"
								"================================================================================\n"
								"=== Current time is %s\n"
								"=== Server takes %d days  %02d:%02d:%02d\n"
								"================================================================================\n"
								, log_szTitle
								, bufStart
								, bufCurr
								, nSec / 86400, (nSec/3600)%24, (nSec/60)%60, nSec%60 );
				}
				//!alter end:帮忙记录前端日志，暂时修改
				
				fputs(buf, pFile);
			}
		}

		// 输出消息
		if(ftell(pFile) < 500*1000*1000)
			fputs(pBuf, pFile);
		fclose(pFile);
	}
}
/////////////////////////////////////////////////////////////////////////////////////
//
bool	SafeCopy(char * pTarget, const char * pSource, int nBufLen /*= 0*/)
{
	//try{
	DEBUG_TRY
		if(pTarget)
		{
			pTarget[0] = 0;
			if(pSource)
			{
				if(nBufLen && (int)strlen(pSource) >= nBufLen)
				{
					strncpy(pTarget, pSource, nBufLen-1);
					pTarget[nBufLen-1] = 0;
					return false;
				}
				strcpy(pTarget, pSource);
				return true;
			}
		}
	//}catch(...){ ::OutputDebugString("CATCH: *** SafeCopy() crash! ***\n"); }
	DEBUG_CATCH("CATCH: *** SafeCopy() crash! ***\n");

	assert(!"SafeCopy()");
	return false;
}

/////////////////////////////////////////////////////////////////////////////
void LogSave (const char* fmt, ...)
{	
	//写日志标志位---------
	if(!g_bLogSave)
		return;
	//----------------------------------


	const UINT _MAX_LOGLINE	=1000000;
	static UINT dwLineCount	=0;

	time_t ltime;
	time( &ltime );

	struct tm *pTime;
	pTime = localtime( &ltime ); /* Convert to local time. */

    char buffer[4096];

    //需要特别注意window和linux不同之处
	va_list argptr;
	va_start(argptr, fmt);
	vsprintf(buffer, fmt, argptr);
	va_end(argptr);
	//vsprintf( buffer, fmt, (char*) ((&fmt)+1) );

	char szLogName[256];
	sprintf(szLogName, "syslog/sys %d-%d %u.log", (pTime->tm_mon+1)%13, pTime->tm_mday, ++dwLineCount/_MAX_LOGLINE);

	FILE* fp	=fopen(szLogName, "a+");
	if(!fp)
		return;

	fprintf(fp, "%s -- %s", buffer, ctime(&ltime));
	fclose(fp);	
}

int IOModelOpen(const char* IoModelName)
{
	return 1;

    if(!strlen(IoModelName))
        return 0;
    //从配置文件读取I/O模型配置信息
    CIniFile ini("../Config/OnLineConfig.ini", "IO_MODEL");   
    int nOpen = ini.GetInt(IoModelName);
    //LOGDEBUG("[IOModelOpen] Read OnLineConfig.ini->IO_MODEL【%s=%d】",IoModelName,nOpen);
    if(nOpen)
    {
        if(g_IoModelSelect)
        {
            LOGERROR("[IOModelOpen] ★★★Read OnLineConfig.ini->IO_MODEL【%s = %d】Is Error 【SELECT】 I/O_Model Is Open", IoModelName, nOpen);
            return 0;
        }
        if(g_IoModelPoll)
        {
            LOGERROR("[IOModelOpen] ★★★Read OnLineConfig.ini->IO_MODEL【%s = %d】Is Error 【POLL】 I/O_Model Is Open", IoModelName, nOpen);
            return 0;
        }
        
        LOGDEBUG("[IOModelOpen] ☆☆☆Read OnLineConfig.ini->IO_MODEL【%s = %d】Is Open", IoModelName, nOpen);
    }
    else
    {
        LOGDEBUG("[IOModelOpen] ★★★Read OnLineConfig.ini->IO_MODEL【%s = %d】Is Close", IoModelName, nOpen);
    }
    //LOGDEBUG("[IOModelOpen] Read OnLineConfig.ini->IO_MODEL g_IoModelSelect=【%d】g_IoModelPoll=【%d】 g_IoModelPoll=【%d】", g_IoModelSelect, g_IoModelPoll, g_IoModelEPoll);
    return  nOpen;
}


//生成随机数
INT Random(int nRange)
{
	//srand((unsigned)time(NULL)); //用于保证是随机数
	//return rand()%nRange;  //用rand产生随机数并设定范围
	static std::default_random_engine generator(time(NULL));
	std::uniform_int_distribution<int> dis(0,nRange-1); 
	return dis(generator);
}

//获取毫秒时间
long long GetMillSecTime(){  
    timeb t;  
    ftime(&t);  
    return t.time * 1000 + t.millitm;  
}  
  
///////////////////////////////////////////////////////////////
INT SafeAtoi(LPCTSTR str)
{
    if(str)
    {
        return atoi(str);
    }
    else
    {
        return 0;
    }
}

void splitString(string str, const char* sep, vector<int>& vec){

    char* p = strtok((char*)str.c_str(), sep);

    while(p) {
        
        vec.push_back(atoi(p));
        p = strtok(NULL, sep);

    }
}