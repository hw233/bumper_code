//////////////////////////////////////////////////////////////////////
//文件名称：type.h
//功能描述：一些常量的定义
//版本说明：const data
//			
//编写作者：2017.08.17 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#ifndef __TYPE_H__
#define __TYPE_H__

//#ifndef __WINDOWS__
//	#define __WINDOWS__
//#endif
//#ifndef __LINUX__
//	#define __LINUX__
//#endif


#include <vector>
#include "BaseType.h"
using namespace std;  

#ifndef _TIME_T_DEFINED
typedef long time_t;         /* 时间值 */
#define _TIME_T_DEFINED      /* 避免重复定义 time_t */
#endif

///////////////////////////////////////////////////////////////////////
//const data
///////////////////////////////////////////////////////////////////////


typedef SHORT	ID_t;
//typedef UINT	ID_t;
typedef USHORT	SMUID_t;
typedef	ULONG	SM_KEY;
typedef UINT	ObjID_t;			

typedef INT				PrescriptionID_t;	
typedef ID_t			AbilityID_t;		// 
typedef ID_t			SceneID_t;			//
typedef ID_t			PlayerID_t;			//
typedef ID_t			ClientID_t;			//			


typedef INT				Time_t; 

typedef struct _Int64
{
	LONG				a;
	ULONG				b;
}Int64;





typedef UINT			GUID_t;	

#define INVALID_GUID			((GUID_t)(-1))
#define INVALID_INDEX			(-1)
#define INVALID_ID_EX			-2

#if defined(__LINUX__) || defined(__WIN_CONSOLE__)
	typedef UCHAR  BYTE;
	typedef USHORT WORD;
	typedef	ULONG  DWORD;
#endif


#if defined(__WINDOWS__)
	typedef DWORD		TID ;
	typedef unsigned __int64 MODEL_PART;
#elif defined(__LINUX__)
	typedef pthread_t	TID ;
	typedef unsigned long long MODEL_PART;
	typedef const char* LPCSTR;
#endif

#if defined(__LINUX__)
	typedef		INT		SMHandle;
#elif defined(__WINDOWS__)
	typedef		VOID*	SMHandle;
#endif

typedef		VOID*	DBHandle;




//ÅÐ¶ÏÄ³Î»ÊÇ·ñ±»ÖÃ
//15.14....3.2.1.0 
#define ISSET0(x) ((x)&0x1)
#define ISSET1(x) ((x)&0x2)
#define ISSET2(x) ((x)&0x4)
#define ISSET3(x) ((x)&0x8)
#define ISSET4(x) ((x)&0x10)
#define ISSET5(x) ((x)&0x20)
#define ISSET6(x) ((x)&0x40)
#define ISSET7(x) ((x)&0x80)
#define ISSET8(x) ((x)&0x100)
#define ISSET9(x) ((x)&0x200)
#define ISSET10(x) ((x)&0x400)
#define ISSET11(x) ((x)&0x800)
#define ISSET12(x) ((x)&0x1000)
#define ISSET13(x) ((x)&0x2000)
#define ISSET14(x) ((x)&0x4000)
#define ISSET15(x) ((x)&0x8000)


///////////////////////////////////////////////////////////////////////
//»ù±¾Í·ÎÄ¼þÒýÓÃ
///////////////////////////////////////////////////////////////////////

#include "assert.h"
#include "PlayerStatus.h"
#include "GameDefine.h"
#include "GameDefine2.h"
//#include "GameStruct.h"
//#include "GameUtil.h"
//#include "PacketDefine.h"
//#include "IniFile.h"
//#include "Config.h"
//#include "Common.h"


#if defined(GAME_CLIENT)
	#define __ENTER_FUNCTION 
	#define __LEAVE_FUNCTION
#elif 1

#if defined(__WINDOWS__)
	#if defined(NDEBUG)
		#define __ENTER_FUNCTION {try{
		#define __LEAVE_FUNCTION }catch(...){AssertSpecial(FALSE,__FUNCTION__);}}
	#else
		#define __ENTER_FUNCTION {try{
		#define __LEAVE_FUNCTION }catch(...){AssertSpecial(FALSE,__FUNCTION__);}}
	#endif
#else	//linux
	#define __ENTER_FUNCTION {try{
	#define __LEAVE_FUNCTION }catch(...){}}
#endif 
#endif


//private:禁止拷贝和赋值操作
#undef DISALLOW_EVIL_CONSTRUCTORS
#define DISALLOW_EVIL_CONSTRUCTORS(TypeName)    \
  TypeName(const TypeName&);                           \
  void operator=(const TypeName&)


#endif
