////////////////////////////////////////////////////////////////
//文件名称：FileAPI.cpp
//功能描述：底层基础系统API接口(based system-call(API) Collection)
//版本说明：文件句柄操作类
//			
//编写作者：2017.08.08 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
//include头文件
////////////////////////////////////////////////////////////////
#include "FileAPI.h"

//#if defined(__WINDOWS__)
//#include <io.h>
//#include <fcntl.h>
//#include <string.h>
//#elif defined(__LINUX__)
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
//#endif

#ifdef __LINUX__
extern INT errno;
#endif

////////////////////////////////////////////////////////////////
//
INT FileAPI::open_ex(const CHAR* filename, INT flags)
{
#if defined(__WINDOWS__)
	INT fd = _open(filename, flags);	
#elif defined(__LINUX__)
	INT fd = open(filename, flags);	
#endif

	if(fd < 0)
	{
#if defined(__WINDOWS__)
		//...
#elif defined(__LINUX__)
		switch(errno)
		{
			case EEXIST:
			case ENOENT:
			case EISDIR:
			case EACCES:
			case ENAMETOOLONG:
			case ENOTDIR:
			case ENXIO:
			case ENODEV:
			case EROFS:
			case ETXTBSY:
			case EFAULT:
			case ELOOP:
			case ENOSPC:
			case ENOMEM:
			case EMFILE:
			case ENFILE:
			default:
			{
				break;
			}
		}//end switch
#endif
	}
	return fd;
}

////////////////////////////////////////////////////////////////
//
INT FileAPI::open_ex(const CHAR* filename, INT flags, INT mode)
{
#if defined(__WINDOWS__)
	INT fd = _open(filename, flags, mode);	
#elif defined(__LINUX__)
	INT fd = open(filename, flags, mode);	
#endif

	if(fd < 0)
	{
#if defined(__WINDOWS__)
		//...
#elif defined(__LINUX__)
		switch(errno)
		{
			case EEXIST:
			case ENOENT:
			case EISDIR:
			case EACCES:
			case ENAMETOOLONG:
			case ENOTDIR:
			case ENXIO:
			case ENODEV:
			case EROFS:
			case ETXTBSY:
			case EFAULT:
			case ELOOP:
			case ENOSPC:
			case ENOMEM:
			case EMFILE:
			case ENFILE:
			default:
			{
				break;
			}
		}//end switch
#endif
	}
	return fd;
}

////////////////////////////////////////////////////////////////
//
UINT FileAPI::read_ex(INT fd, VOID* buf, UINT len)
{
#if defined(__WINDOWS__)
	INT result = _read(fd, buf, len);	
#elif defined(__LINUX__)
	INT result = read(fd, buf, len);	
#endif

	if(result < 0)
	{
#if defined(__WINDOWS__)
		//...
#elif defined(__LINUX__)
		switch(errno)
		{
			case EINTR:
			case EAGAIN:
			case EBADF:
			case EIO:
			case EISDIR:
			case EINVAL:
			case EFAULT:
			case ECONNRESET:
			default:
			{
				break;
			}
		}
#endif
	}
	else if(result == 0)
	{
	//...
	}

	return result;
}

////////////////////////////////////////////////////////////////
//
UINT FileAPI::write_ex(INT fd, const VOID* buf, UINT len)
{
#if defined(__WINDOWS__)
	INT result = _write(fd, buf, len);	
#elif defined(__LINUX__)
	INT result = write(fd, buf, len);	
#endif

	if(result < 0)
	{
#if defined(__WINDOWS__)
		//...
#elif defined(__LINUX__)
		switch(errno)
		{
			case EAGAIN:
			case EINTR:
			case EBADF:
			case EPIPE:
			case EINVAL:
			case EFAULT:
			case ENOSPC:
			case EIO:
			case ECONNRESET:
			default:
			{
				break;
			}
		}
#endif
	}

	return result;
}

////////////////////////////////////////////////////////////////
//
VOID FileAPI::close_ex(INT fd)
{
	if(close(fd) < 0)
	{
#if defined(__WINDOWS__)
		//...
#elif defined(__LINUX__)
		switch(errno)
		{
			case EBADF:
			default:
			{
				break;
			}
		}
#endif
	}
}

//////////////////////////////////////////////////////////////////////
//
INT FileAPI::fcntl_ex(INT fd , INT cmd) 
{
#if defined(__LINUX__)
	INT result = fcntl (fd , cmd);
	if (result < 0) 
	{
		switch (errno) 
		{
			case EINTR : 
			case EBADF : 
			case EACCES : 
			case EAGAIN : 
			case EDEADLK : 
			case EMFILE  : 
			case ENOLCK : 
			default : 
				{
					break;
				}
		}
	}
	return result;
#elif defined(__WINDOWS__)
	return 0;
#endif
}

//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
INT FileAPI::fcntl_ex(INT fd, INT cmd, LONG arg) 
{
#if defined(__LINUX__)
	INT result = fcntl(fd , cmd , arg);
	if(result < 0) 
	{
		switch ( errno ) 
		{
			case EINTR : 
			case EINVAL : 
			case EBADF : 
			case EACCES : 
			case EAGAIN : 
			case EDEADLK : 
			case EMFILE  : 
			case ENOLCK : 
			default : 
				{
					break;
				}
		}
	}
	return result;
#elif defined(__WINDOWS__)
	return 0 ;
#endif	
}


////////////////////////////////////////////////////////////////
//
BOOL FileAPI::getfilenonblocking_ex(INT fd)
{
#if defined(__WINDOWS__)
	return FALSE;
#elif defined(__LINUX__)
		INT flags = fcntl_ex(fd, F_GETFL, 0);
	return flags | O_NONBLOCK;
#endif
}

////////////////////////////////////////////////////////////////
//
VOID FileAPI::setfilenonblocking_ex(INT fd, BOOL no)
{
#if defined(__WINDOWS__)
	//...
#elif defined(__LINUX__)
	INT flags = fcntl_ex(fd, F_GETFL, 0);
	if(no)
		flags |= O_NONBLOCK;
	else
		flags &= ~O_NONBLOCK;

	fcntl_ex(fd, F_SETFL, flags);
#endif	
}

////////////////////////////////////////////////////////////////
//
VOID FileAPI::setfilenonblocking_ex2(INT fd, BOOL no)
{
#if defined(__WINDOWS__)
	//...
#elif defined(__LINUX__)
	ULONG arg = (no == TRUE ? 1 : 0);
	ioctl_ex(fd, FIONBIO, &arg);
#endif	
}

////////////////////////////////////////////////////////////////
//
VOID FileAPI::ioctl_ex(INT fd, INT request, VOID* argp)
{
#if defined(__WINDOWS__)
	//...
#elif defined(__LINUX__)	
	if(ioctl(fd, request, argp) < 0)
	{
		switch(errno)
		{
			case EBADF:
			case ENOTTY:
			case EINVAL:
			default:
			{
				break;
			}
		}
	}
#endif	
}

////////////////////////////////////////////////////////////////
//
INT FileAPI::dup_ex(INT fd)
{
#if defined(__WINDOWS__)
	INT newfd = _dup(fd)
#elif defined(__LINUX__)
	INT newfd = dup(fd);
#endif

	if ( newfd < 0 ) {
#if defined(__WINDOWS__)
		switch (errno) 
		{
			case EBADF: 
			case EMFILE: 
			default:
				{
					break;
				}
		}//end of switch
#elif defined(__LINUX__)
#endif
	}

	return newfd;
}
//////////////////////////////////////////////////////////////////////
//FIONREAD:就是返回緩衝區有多少字節
UINT FileAPI::availablefile_ex (INT fd)
{
#if defined(__LINUX__)
	UINT arg = 0;
	ioctl_ex(fd, FIONREAD, &arg);
	return arg;
#elif defined(__WINDOWS__)
	return 0;
#endif	
}

//////////////////////////////////////////////////////////////////////
//
LONG FileAPI::lseek_ex (INT fd, LONG offset, INT whence)
{
#if defined(__LINUX__)
	LONG result = lseek(fd, offset, whence);
	if (result < 0) 
	{
		switch(errno) 
		{
		case EBADF: 
		case ESPIPE: 
		case EINVAL: 
		default:
			{
				break;
			}
		}
	}
#elif defined(__WINDOWS__)
	LONG result = _lseek(fd, offset, whence);
	if (result < 0) 
	{
		//...
	}
#endif
	return result;
}

LONG FileAPI::tell_ex(INT fd)
{

#if defined(__LINUX__)
		LONG result = 0;
#elif defined(__WINDOWS__)
		LONG result = _tell(fd);		
#endif
	return result;
}