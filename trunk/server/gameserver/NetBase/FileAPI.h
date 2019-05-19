////////////////////////////////////////////////////////////////
//文件名称：FileAPI.h
//功能描述：文件句柄操作类(based system-call(API) Collection)
//版本说明：文件句柄操作类
//			
//编写作者：2017.08.08 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
////////////////////////////////////////////////////////////////

#ifndef __FILE_API_H__
#define __FILE_API_H__

//#ifndef __cpluscplus  
//	extern "C"  //C++  
//	{  
//	#endif
////////////////////////////////////////////////////////////////
//include头文件
////////////////////////////////////////////////////////////////
#include "../Common/BaseType.h"
#include "../Common/Common.h"

////////////////////////////////////////////////////////////////
//namespace
////////////////////////////////////////////////////////////////

namespace FileAPI
{

	

	//
	//exception version of open()
	//
	INT open_ex(const CHAR* filename, INT flags);
	INT open_ex(const CHAR* filename, INT flags, INT mode);

	//
	//exception version of close()
	//
	VOID close_ex(INT fd);

	//
	//exception version of read()
	//
	UINT read_ex(INT fd, VOID* buf, UINT len);

	//
	//exception version of write()
	//
	UINT write_ex(INT fd, const VOID* buf, UINT len);

	//
	//exception version of fcntl()
	//
	INT fcntl_ex(INT fd, INT cmd);

	//
	//exception version of fcntl()
	//
	INT fcntl_ex(INT fd, INT cmd, LONG arg);

	//
	//is this stream if nonblocking?
	//
	//using getfilenonblocking_ex()
	//
	BOOL getfilenonblocking_ex(INT fd);

	//
	//make this stream if nonblocking?
	//
	//using setfilenonblocking_ex()
	//
	VOID setfilenonblocking_ex(INT fd, BOOL on);

	//
	//make this stream if nonblocking?
	//
	//using setfilenonblocking_ex2()
	//
	VOID setfilenonblocking_ex2(INT fd, BOOL on);

	//
	//exception version of ioctl()
	//	
	//
	VOID ioctl_ex(INT fd, INT request, VOID* argp);

	//
	//how much bytes available in this steam?
	//
	//using availablefile_ex()
	//
	UINT availablefile_ex(INT fd);

	//
	//exception version of dup()
	//
	INT dup_ex(INT fd);	

	//
	//exception version of lseek()
	//
	LONG lseek_ex(INT fd, LONG offset, INT whence);	

	//
	//exception version of tell_ex()
	//
	LONG tell_ex(INT fd);	

	 

}//namespace FileAPI

//#ifndef __cplusplus  
//	}  
//	#endif

#endif//__FILE_API_H__