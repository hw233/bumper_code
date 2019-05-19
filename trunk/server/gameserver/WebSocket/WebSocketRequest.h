//////////////////////////////////////////////////////////////////////
//文件名称：WebsocketRequest.h
//功能描述：
//版本说明：WebsocketRequest API
//          
//编写作者：2018.9.5 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#ifndef __WEBSOCKET_REQUEST__
#define __WEBSOCKET_REQUEST__

#include <stdint.h>
#include <arpa/inet.h>
#include <string>
#include "../Common/BaseType.h"

/*https://blog.csdn.net/Windgs_YF/article/details/86479648
* 一次特殊的HTTP请求
* 紧接着是一次Http请求（第四个包），说明Http的确是使用Tcp建立连接的。
* 先来看传输层（Tcp）: PSH（推送比特）置1，ACK置1，PSH置1说明开始发送数据，同时发送数据ACK要置1，因为需要接收到这个数据包的端给予确认。
* PSH为1的情况，一般只出现在 DATA内容不为0的包中，也就是说PSH为1表示的是有真正的TCP数据包内容被传递。
* 
* 再来看应用层（Http）：这是一次特殊的Http请求，为什么是一次特殊的Http请求呢？
* Http请求头中Connection:Upgrade Upgrade:websocket,Upgrade代表升级到较新的Http协议或者切换到不同的协议。
* 很明显WebSocket使用此机制以兼容的方式与HTTP服务器建立连接。
* WebSocket协议有两个部分：握手建立升级后的连接，然后进行实际的数据传输。
* 首先，客户端通过使用Upgrade: WebSocket和Connection: Upgrade头部以及一些特定于协议的头来请求WebSocket连接，以建立正在使用的版本并设置握手。
* 服务器，如果它支持协议，回复与相同Upgrade: WebSocket和Connection: Upgrade标题，并完成握手。
* 握手完成后，数据传输开始。这些信息在前面的Chrome控制台中也可以看到。
*/

using namespace std;

class WebsocketRequest 
{
public:
	WebsocketRequest();
	~WebsocketRequest();

public:
	int 		FetchWebsocketInfo(char* pMsg);
	void 		PrintWebProtocol();
	void 		Reset();
    std::string GetMsg();
    uint64_t    GetMsgLength();
    uint8_t     GetMsgOpcode();
    int     	GetPos() {	return m_nPos;	}

private:
	int FetchFin(char *pMsg, int &nPos);
	int FetchOpcode(char *pMsg, int &nPos);
	int FetchMask(char *pMsg, int &nPos);
	int FetchMaskingKey(char *pMsg, int &nPos);
	int FetchPayloadLength(char *pMsg, int &nPos);
	int FetchPayload(char *pMsg, int &nPos);
public:
	uint8_t m_ucFin;
	uint8_t m_ucOpcode;
	uint8_t m_ucMask;
	uint8_t m_ucMaskingKeySet[4];
	uint64_t m_u64PayloadLength;
	char m_szPayload[MSG_MAX_SIZE];
    int m_nPos;
};

#endif
