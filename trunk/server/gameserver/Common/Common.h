////////////////////////////////////////////////////////////////
//文件名称：common.h
//功能描述：系统底层常用宏定义。
//版本说明：windows操作系统需要定义宏：__WINDOWS__
//       linux操作系统需要定义宏：__LINUX__
//编写作者：2017.08.14 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
////////////////////////////////////////////////////////////////

#ifndef	__COMMON_H__
#define	__COMMON_H__

#include <assert.h>
//#include "logfile.h"
#include <time.h>		//time、time_t
#include <string.h>
#include <stdio.h>
#include <stdarg.h>		//vsLOG_DEBUG
#include <algorithm>
//#include "Type.h"
#include "BaseType.h"

//add by huyf 2018.10.17
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <functional>
//add end

//日志开关标志位
//extern BOOL g_bLogSave;

extern BOOL g_bLogSave;


//获取毫秒时间
long long GetMillSecTime();  
////////////////////////////////////////////////////////////////////////////////////////////////////////
//String	DumpBuffer(const char* buf, int nSize);
void	DateTime(char * buf20, time_t tInput = 0);		// 0 : curr time
int		DateTimeSign(time_t tInput = 0);	// 将时间的年和月转换为整数，月两位
int		DateTimeUpMonthSign(time_t tInput = 0); // 将时间的年和上月转换为整数，月两位
bool	FormatDateTime(char* buf, const char* szFormat, time_t tInput = 0);		// szFormat: "%04d-%02d-%02d %02d:%02d:%02d"
void	DateTimeStamp(char * buf15, time_t tInput = 0);		// 0 : curr time
int		DateStamp(time_t tTime=0);
int		DateStampPass(int nDate, int nDays);	// return -1: error//return: 时间标签nDate加上指定天数nDays
bool	IsActiveTime(time_t tCurr, unsigned long nFormat);
bool	IsTimeRange(unsigned long nBegin, unsigned long nEnd, time_t tCurr=0);
//获取当前时间是本年中第几周
int 	WeekNum();
//bool    IsSameWeek(time_t time1, time_t time2);//判断两个时间是否在同一周内  
//bool    IsSameDay(time_t time1, time_t time2);//判断两个时间是否同一天 
//bool    IsSameMonth(time_t time1, time_t time2); //判断两个时间是否在同一月  
//bool    IsLastDayOfMon(time_t tInput = 0); //判断是否为某月的最后一天
inline long	GetCurrTime()				{ return time(NULL); }
char*	StrStrCh(const char* string, const char* strCharSet);
//bool    IsOver_7_Day(time_t time1, time_t time2);//判断两个时间是否超过7天

////////////////////////////////////////////////////////////////////////////////////////////////////////
void InitLog(const char * pTitle, const char * pFilename, time_t tStart);
void log_Save(const char * pFilename, const char * pBuf, int nFlag=0);
void LOSEDATA(const char * sFormat, ...);

/////////////////////////////////////////////////////////////////////////////////////////////////////////
#define log_InlineLog(x,y)\
inline void	log_InlineLog##y(/*const char* pFilename, int nLine, */const char * sFormat, ...)\
{\
	try{\
	va_list argptr;\
	va_start( argptr, sFormat);     /* Initialize variable arguments. */\
\
	time_t ltime;\
	time( &ltime );\
	tm *	pTm = localtime(&ltime);\
	long long llCurTime = GetMillSecTime();\
	char	buf[4096] = {0};					/* ?????�ռ���ܲ��� */\
	sprintf(buf, "%02d:%02d:%02d %d " #y ": ", pTm->tm_hour, pTm->tm_min, pTm->tm_sec, (int)(llCurTime%100000000));\
	int		ret = vsnprintf(buf + strlen(buf), 2048, sFormat, argptr);\
	strcpy(buf + strlen(buf), "\n");\
	if(!(ret < 4096-2)) log_Save(x, "ASSERT(ret < 4096-2)");	/* ?????�ռ���ܲ��� */\
\
	va_end( argptr );              /* Reset variable arguments.      */\
\
	log_Save(x,buf);\
	}\
	catch(...)\
	{\
		log_Save(x, "☆☆☆☆☆catch...☆☆☆☆☆");	/* ?????�ռ���ܲ��� */\
	}\
}
//add by huyf 2018.08.22:从客户端发送日志信息到服务器GameServer
/////////////////////////////////////////////////////////////////////////////////////////////////////////
inline void	log_InlineLogCLIENT(const char* pFilename, const char * sFormat, ...)
{
	try{
	va_list argptr;
	va_start( argptr, sFormat);     /* Initialize variable arguments. */

	time_t ltime;
	time( &ltime );
	tm *	pTm = localtime(&ltime);
	char	buf[4096] = "";					/* ?????�ռ���ܲ��� */
	sprintf(buf, "%02d:%02d:%02d " ": ", pTm->tm_hour, pTm->tm_min, pTm->tm_sec);
	int		ret = vsprintf(buf + strlen(buf), sFormat, argptr);
	strcpy(buf + strlen(buf), "\n");
	if(!(ret < 4096-2)) log_Save(pFilename, "ASSERT(ret < 4096-2)", 1);	/* ?????�ռ���ܲ��� */

	va_end( argptr );              /* Reset variable arguments.      */

	log_Save(pFilename,buf, 0);
	}
	catch(...)
	{
		log_Save(pFilename, "☆☆☆☆☆catch...☆☆☆☆☆");	/* ?????�ռ���ܲ��� */
	}
}
//end:从客户端发送日志信息到服务器GameServer

extern char		log_szFilename[];		// 

log_InlineLog(log_szFilename, DEBUG)
log_InlineLog(log_szFilename, WARNING)
log_InlineLog(log_szFilename, Message)
log_InlineLog(log_szFilename, ERROR)
log_InlineLog(log_szFilename, CATCH_ERROR)


//inline log_InlineLogNULL(const char * sFormat, ...){}

//#define LOGDEBUG(str,...) 		log_InlineLogDEBUG(__FILE__, __LINE__, str, __VA_ARGS__)
#define LOGDEBUG        log_InlineLogDEBUG
#define LOGWARNING		log_InlineLogWARNING
#define LOGMSG			log_InlineLogMessage
#define LOGERROR		log_InlineLogERROR
#define LOGCATCH		log_InlineLogCATCH_ERROR
#define LOGCLIENT		log_InlineLogCLIENT


inline void   log_assert(const char* type, const char* str, const char* file, int line) { LOGERROR("★%s(%s)★ in %s, %d", type, str, file, line); }

#ifdef	LOCAL_DEBUG
	#include <assert.h>
	#undef	ASSERT
		#define		ASSERT(x)	(void)( (x) || (assert(!"(#x)"),log_assert("ASSERT", #x, __FILE__, __LINE__), 0) )
	#define		CHECK(x)	do{ if(!(x)) { assert(!(#x)),log_assert("CHECK", #x, __FILE__, __LINE__); return; } }while(0)
	#define		CHECKF(x)	do{ if(!(x)) { assert(!(#x)),log_assert("CHECKF", #x, __FILE__, __LINE__); return 0; } }while(0)
	#define		IF_NOT(x)	if( !(x) ? ( assert(!(#x)),log_assert("IF_NOT", #x, __FILE__, __LINE__),1 ) : 0 )
	#define		IF_NOT_(x)	if( !(x) ? ( assert(!"(#x)"),log_assert("IF_NOT_", #x, __FILE__, __LINE__),1 ) : 0 )
	#define		IF_OK(x)	if( (x) ? 1 : ( assert(!(#x)),log_assert("IF_OK", #x, __FILE__, __LINE__),0 ) )
	#define		IF_OK_(x)	if( (x) ? 1 : ( assert(!"(#x)"),log_assert("IF_OK_", #x, __FILE__, __LINE__),0 ) )
	#define		PURE_VIRTUAL_FUNCTION_0		{ ASSERT(!"PURE_VIRTUAL_FUNCTION_0"); return 0; }
	#define		PURE_VIRTUAL_FUNCTION		{ ASSERT(!"PURE_VIRTUAL_FUNCTION"); }
	#define		PURE_VIRTUAL_FUNCTION_(x)	{ ASSERT(!"PURE_VIRTUAL_FUNCTION"); return x(); }
	
#else // LOCAL_DEBUG
	#undef	ASSERT
		#define		ASSERT(x)	(void)( (x) || (log_assert("ASSERT", #x, __FILE__, __LINE__), 0) )
	#define		CHECK(x)	do{ if(!(x)) { log_assert("CHECK", #x, __FILE__, __LINE__); return; } }while(0)
	#define		CHECKF(x)	do{ if(!(x)) { log_assert("CHECKF", #x, __FILE__, __LINE__); return 0; } }while(0)
	#define		IF_NOT(x)	if( !(x) ? ( log_assert("IF_NOT", #x, __FILE__, __LINE__),1 ) : 0 )
	#define		IF_NOT_(x)	if( !(x) ? ( log_assert("IF_NOT_", #x, __FILE__, __LINE__),1 ) : 0 )
	#define		IF_OK(x)	if( (x) ? 1 : ( log_assert("IF_OK", #x, __FILE__, __LINE__),0 ) )
	#define		IF_OK_(x)	if( (x) ? 1 : ( log_assert("IF_OK_", #x, __FILE__, __LINE__),0 ) )
	#define		PURE_VIRTUAL_FUNCTION_0		= 0;
	#define		PURE_VIRTUAL_FUNCTION		= 0;
	#define		PURE_VIRTUAL_FUNCTION_(x)	= 0;

#endif // LOCAL_DEBUG

void	LogSave		(const char* fmt, ...);
#define		DEBUG_TRY		try{
#define		DEBUG_CATCH(x)	}catch(...){ ::LogSave("CATCH: *** " x " crash()! ***\n"); }
#define		DEBUG_CATCH2(x,y)	}catch(...){ ::LogSave((x),(y)); ::LogSave("CATCH: *** " x " crash()! ***\n"); }
#define		DEBUG_CATCH3(x,y,z)	}catch(...){ ::LogSave((x),(y),(z)); ::LogSave("CATCH: *** " x " crash()! ***\n"); }
#define		DEBUG_CATCH4(x,y,z,u)	}catch(...){ ::LogSave((x),(y),(z),(u)); ::LogSave("CATCH: *** " x " crash()! ***\n"); }

#define		DEBUG_CATCHF(x)	}catch(...){ ::LogSave("CATCH: *** " x " crash()! ***\n"); return false; }
#define		DEBUG_CATCH0(x)	}catch(...){ ::LogSave("CATCH: *** " x " crash()! ***\n"); return 0; }
#define		DEBUG_CATCHF2(x,y)	}catch(...){ ::LogSave((x),(y)); ::LogSave("CATCH: *** " x " crash()! ***\n"); return false; }
#define		DEBUG_CATCHF3(x,y,z)	}catch(...){ ::LogSave((x),(y),(z)); ::LogSave("CATCH: *** " x " crash()! ***\n"); return false; }
#define		DEBUG_CATCHF4(x,y,z,u)	}catch(...){ ::LogSave((x),(y),(z),(u)); ::LogSave("CATCH: *** " x " crash()! ***\n"); return false; }

/////////////////////////////////////////////////////////////////////////////
// function
/////////////////////////////////////////////////////////////////////////////
bool SafeCopy(char * pTarget, const char * pSource, int nBufLen = 0);
////////////////////////////////////////////////////////////////
//内存释放宏定义
////////////////////////////////////////////////////////////////
//定义断言
//#ifndef ASSERT
//	#define ASSERT(x) assert(x){}
//#endif//ASSERT

//根据指针值释放内存
#ifndef SAFE_DELETE
	#define SAFE_DELETE(x) if((x)!=NULL) { delete (x); (x)=NULL; }
#endif//SAFE_DELETE

//根据指针值释放数组元素内存
#ifndef SAFE_DELETE_ARRAY
	#define SAFE_DELETE_ARRAY(x) if((x)!=NULL) { delete[] (x); (x)=NULL; }
#endif//SAFE_DELETE_ARRAY

//根据指针调用free接口
#ifndef SAFE_FREE
	#define SAFE_FREE(x) if((x)!=NULL) { free(x); (x)=NULL; }
#endif//SAFE_FREEn

//根据指针调用Release接口
#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(x) if((x)!=NULL) { (x)->Release(); (x)=NULL; }
#endif//SAFE_RELEASE

#ifndef SAFE_RELEASEBYOWNER
	#define SAFE_RELEASEBYOWNER(ptr) { if(ptr){ try{ ptr->ReleaseByOwner(); }catch(...){ LOGERROR("CATCH: *** SAFE_RELEASE() crash! ***\n"); } ptr = 0; } }
#endif//SAFE_RELEASEBYOWNER


struct DeletePtrs
{
    template<typename T>
        inline void operator()(const T *ptr) const
        {
            if(ptr)
                delete ptr;
        }
};


struct DeletePair
{
    template<typename Ty1, typename Ty2>
        inline void operator() (const std::pair<Ty1, Ty2> &ptr) const
        {
            if (ptr.second)
                delete ptr.second;
        }
};


#define CLEAR_MAP_DATA(m) \
std::for_each(m.begin(),m.end(),DeletePair()); \
m.clear();

//------------------------------------------
// eg:
// 在使用的时候先进行std::set_new_handle(outOfMem);
// int* pt = new int;
/*using namespace std;
void outOfMem()
{
	std::cerr<<"Unable to satisfy request for memory\n";
	std::abort();
}
printf("%.*s\n", 3, "abc");        // 输出abc >3是一样的效果 因为输出类型type = s，遇到'\0'会结束
*/
//////////////////////////////////////////////////////////////////////
//template<TYPENAME T>
//	inline void		safe_release(T*& pT)			{ pT->Release(); pT=NULL; }
//#define		SafeRelease	safe_release

//锁机制
#if defined(__LINUX__ )
	#define TRYLOCKOBJ_EX(mtx)	pthread_mutex_trylock(&mtx)		//尝试加锁（不阻塞），pthread_mutex_trylock()语义与pthread_mutex_lock()类似，不同的是在锁已经被占据时返回EBUSY而不是挂起等
	#define LOCKOBJ_EX(mtx)		pthread_mutex_lock(&mtx)			//加锁（阻塞）
	#define UNLOCKOBJ_EX(mtx)	pthread_mutex_unlock(&mtx)			//解锁（阻塞）
	#define LOCKOBJ(mtx)		CMyAutoMutex xLock(&mtx, TRUE)		//自动锁（阻塞）
	#define UNLOCKOBJ(mtx)		xLock.UnLock()
	#define INTERLOCKEDEXCHANGE(x,y) xchg(x,y)
	#define INTERLOCKEDEXCHANGEADD(v,p) __sync_fetch_and_add(v, p)	//v的值原子添加P的大小
	#define INTERLOCKEDINCREMENT(t) __sync_fetch_and_add(t, 1)		//原子+1
	#define INTERLOCKEDDECREMENT(t) __sync_fetch_and_sub(t, 1)		//原子-1

#elif defined(__WINDOWS__)
	#define	CRITSECT	CCriticalSection
	#define LOCKOBJ		CSingleLock xLock(&m_xCtrl, true)
	#define UNLOCKOBJ	xLock.Unlock()
	#define INTERLOCKEDEXCHANGE(x,y) InterlockedExchange(x,y)
	#define INTERLOCKEDEXCHANGEADD(v,p) InterlockedExchangeAdd(v,p)
	#define INTERLOCKEDINCREMENT(t) InterlockedIncrement(t)
	#define INTERLOCKEDDECREMENT(t) InterlockedDecrement(t)
#endif


#define	PURE_VIRTUAL_DECLARE		PURE_VIRTUAL_FUNCTION		
#define	PURE_VIRTUAL_DECLARE_0		PURE_VIRTUAL_FUNCTION_0

//修改密钥
//#define LOGIN_KEY1				0x4524a441	// A = 0x3F, B = 0x97, C = 0xE4, first = 0x24, key = 0x4524a441
//#define LOGIN_KEY2				0x14349414	// A = 0x6A, B = 0xA7, C = 0xB5, first = 0x34, key = 0x14349414
//const int		RECV_BUFFER_SIZE		= 4096;				// 接收缓冲区尺寸
//const int		SEND_BUFFER_SIZE		= 256 * 1024;		// 发送缓冲区尺寸

/////////////////////////////////////////////////////////////////////////////
template<typename T>
inline T	CutOverflow(T x, T y) { return (x <= y) ? x : y; }
template<typename T>
inline T	CutTrail(T x, T y) { return (x >= y) ? x : y; }
template<typename T>
inline T	CutRange(T n, T min, T max) { return (n<min) ? min : ((n>max) ? max : n); }

//TCP MGET宏
/*
在展开宏MGET之前，m_retry被定义为一个空指针。这可以防止当存储器仍然不可用时的无休止的循环：这个MGET展开会把m设置为空指针而不是调用m_retry函数。
在MGET展开以后，这个m_retry的临时定义就被取消了，以防在此之后第2章 mbuf：存储器缓存计计33下载
有对MGET的其他引用。
*/
// 需要吧\都删掉，屏蔽后编译报警告
// #define MGET(m, how, type) 
// {
// 	MALLOC((m), struct mbuf*, MSIZE, mbtypes[type], (how));
// 	if (m)
// 	{
// 		(m)->m_type = (type);
// 		MBUFLOCK(mbstat.m_mtypes[type]++;)
// 		(m)->m_next = (struct mbuf*)NULL;
// 		(m)->m_nextpkt = (struct mbuf*)NULL;
// 		(m)->m_data = (m)->m_dat;
// 		(m)->m_flags = 0;
// 	}
// 	else
// 	{
// 		(m) = m_retry((how), (type));
// 	}
// }

// struct mbuf* m_retry(int i, int t)
// {
// 	struct mbuf *m;
// 	m_reclaim();

// #define m_retry(i, t) (struct mbuf *)0
// 	MGET(m, i, t);
// #undef m_retry

// 	return (m);
// }


//ifaddr
// struct ifaddr
// {
// 	struct 	ifaddr* 		ifa_next;		/*next address fro interface*/
// 	struct 	ifnet* 			ifa_ifp;		/*back-point to interface*/
// 	struct 	sockaddr* 		ifa_addr;		/*address of interface*/
// 	struct 	sockaddr* 		ifa_dstaddr;	/*other end of p-to-p link*/
// #define    	ifa_broadaddr 	ifa_dstaddr;	/*broadcase address interface*/
// 	struct 	sockaddr* 		ifa_netmask;	/*used to determine subnet*/
// 	void 	(*ifa_rtrequest)();				/*check or clean routes*/
// 	u_short ifa_flags;						/*mostly rt_flags for cloning*/
// 	short   ifa_refcnt;						/*references to this structure*/
// 	int     ifa_metric;						/*cost for this interface*/
// #define 	ROUNDUP(a) 		(1+(((a)-1|sizeof(long)-1)))
// 	socksize = ROUNDUP(socksize);
// };
// 

//生成随机数
INT 	Random(INT nRange);
INT SafeAtoi(LPCTSTR str);

void splitString(string str, const char* sep, vector<int>& vec);


#endif//__COMMON_H__
