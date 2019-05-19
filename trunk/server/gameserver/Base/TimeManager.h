//////////////////////////////////////////////////////////////////////
//文件名称：TimeManager.h
//功能描述：时间管理器
//版本说明：TimeManager API
//			
//编写作者：2017.08.22 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//include头文件
//////////////////////////////////////////////////////////////////////
#ifndef __TIMEMANAGER_H__
#define __TIMEMANAGER_H__

#include "../Common/Type.h"
#ifdef __LINUX__
#include <sys/utsname.h>
#include <sys/time.h>
#endif

class TimeManager
{
public :
	TimeManager();
	~TimeManager();

	BOOL			Init();

	//当前时间计数值，起始值根据系统不同有区别
	//返回的值为：微妙单位的时间值
	UINT			CurrentTime();
	//直接返回，不调用系统接口
	UINT			CurrentSavedTime(){ return m_CurrentTime; };
	//取得服务器端程序启动时的时间计数值
	UINT			StartTime(){ return m_StartTime; };

	//将当前的系统时间格式化到时间管理器里
	VOID			SetTime();

	// 得到标准时间
	time_t			GetANSITime();

public :
	//***注意：
	//以下接口调用没有系统调用，只针对时间管理器内的数据
	//

	//取得设置时间时候的“年、月、日、小时、分、秒、星期的值”
	INT				GetYear(){ return m_TM.tm_year+1900; };	//[1900,????]
	INT				GetMonth(){ return m_TM.tm_mon; };		//[0,11]
	INT				GetDay(){ return m_TM.tm_mday; };		//[1,31]
	INT				GetHour(){ return m_TM.tm_hour; };		//[0,23]
	INT				GetMinute(){ return m_TM.tm_min; };		//[0,59]
	INT				GetSecond(){ return m_TM.tm_sec; };		//[0,59]
	//取得当前是星期几；0表示：星期天，1～6表示：星期一～星期六
	UINT			GetWeek(){ return m_TM.tm_wday; };
	//将当前的时间（年、月、日、小时、分）转换成一个UINT来表示
	//例如：0,507,211,233 表示 "2005.07.21 12:33"
	UINT			Time2DWORD();
	//取得当前的日期[4bit 0-9][4bit 0-11][5bit 0-30][5bit 0-23][6bit 0-59][6bit 0-59]
	UINT			CurrentDate();
	//取得服务器启动后的运行时间（毫秒）
	UINT			RunTime(){ 
		CurrentTime();
		return (m_CurrentTime-m_StartTime);  
	};
	UINT			RunTick()
	{
		CurrentTime();
		return m_CurrentTime-m_StartTime;  
	};
	//取得两个日期时间的时间差（单位：毫秒）, Ret = Date2-Data1
	UINT			DiffTime( UINT Date1, UINT Date2 );
	//将一个UINT的日期转换成一个tm结构
	VOID			ConvertUT( UINT Date, tm* TM );
	//将一个tm结构转换成一个UINT的日期
	VOID			ConvertTU( tm* TM, UINT& Date );
	//取得已天为单位的时间值, 千位数代表年份，其他三位代表时间（天数）
	UINT			GetDayTime();
	//得到当前是今天的什么时间2310表示23点10分
	WORD			GetTodayTime();
	BOOL			FormatTodayTime(WORD& nTime);


public :
	UINT			m_StartTime;
	UINT			m_CurrentTime;
	time_t			m_SetTime;
	tm				m_TM;
#ifdef __LINUX__
	struct timeval _tstart, _tend;
	struct timezone tz;
#endif

};

extern TimeManager* g_pTimeManager;

#endif
