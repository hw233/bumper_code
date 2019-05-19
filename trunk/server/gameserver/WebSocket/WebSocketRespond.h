//////////////////////////////////////////////////////////////////////
//文件名称：WebsocketRespond.h
//功能描述：
//版本说明：WebsocketRespond API
//          
//编写作者：2018.9.5 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#ifndef __WEBSOCKET_RESPOND__
#define __WEBSOCKET_RESPOND__

#include <stdint.h>
#include <arpa/inet.h>

#define MSGMAXLEN   2048


class WebsocketRespond 
{
public:
	WebsocketRespond();
	~WebsocketRespond();

public:
    int  PackData(const unsigned char* pMessage,size_t unMsgLen, uint8_t ucFin , 
                   uint8_t ucOpcode, uint8_t ucMask, unsigned char** ppOut, size_t* pOutLen);

private:
	uint8_t m_ucFin;
	uint8_t m_ucOpcode;
	uint8_t m_ucMask;
};

#endif
