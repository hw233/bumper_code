//////////////////////////////////////////////////////////////////////
//文件名称：World.h
//功能描述：世界主进程类
//版本说明：World API
//			
//编写作者：2017.08.22 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#ifndef __WORLD_H__
#define __WORLD_H__

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include "Type.h"
//add by huyf 2018.04.03:多线程架构
//#include "../Thread/SocketThread/SocketThread.h"
//add end:多线程架构
//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////
#define ERR_EXIT(m)                  \
        do{                          \
                perror(m);           \
                exit(EXIT_FAILURE);  \
        }while(0)
            
//////////////////////////////////////////////////////////////////////
//class
//////////////////////////////////////////////////////////////////////
//世界服务器
//add by huyf 2018.04.03:多线程架构
//class IMessagePort;
//add end:多线程架构
class World
{
public:
	World();
	~World();

public:
	BOOL		Init();
	BOOL		Loop();
	BOOL		Exit();	

protected:
	BOOL		NewStaticManager();
	BOOL		InitStaticManager();
	BOOL		DelStaticManager();

	//激活
	BOOL 	IsActive()						{  return m_bActive;	}
	VOID    SetActive(BOOL active)			{	m_bActive = active;	}

protected:
	BOOL 				m_bActive;
	
	//add by huyf 2018.04.03:多线程架构
	//CSocketThread* 		m_pSocketThread;
	//IMessagePort* 		m_pMsgPort;
	//add end:多线程架构

};

extern World g_World ;

#endif


//https://github.com/babykylin
