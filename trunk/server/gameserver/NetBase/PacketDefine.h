////////////////////////////////////////////////////////////////
//文件名称：PacketDefine.h
//功能描述：消息包编号
//文件名称：PacketDefine API
//			
//编写作者：2017.08.17 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
////////////////////////////////////////////////////////////////
#ifndef __PACKET_DEFINE_H__
#define __PACKET_DEFINE_H__


namespace Packets
{
	//消息类型值描述格式：PACKET_XX_YYYYYY
	//XX可以描述为：C2PW、PW2C、C2MTS、MTS2C、PW2MTS、MTS2PW、C2ACC、ACC2C
	//PW游戏服务器端、C客户端、L登陆服务器端、MTS思维服务器
	//YYYYYY表示消息内容
	//例如：PACKET_C2ACC_REGISTER 表示客户端发给账号服务器端注册的消息
	enum PACKET_DEFINE
	{
		PACKET_NONE = 0 ,						//0，空
/*------------------------C2ACC-------------------------------------------------------------------------------------------*/
	/* ID:1 */PACKET_C2ACC_TEST ,
		PACKET_C2ACC_REGISTER ,										//前端发送注册请求到账号服务器
		PACKET_C2ACC_CHECK_ACCOUNT ,								//前端发送检查账号信息到账号服务器
/*------------------------C2ACC-------------------------------------------------------------------------------------------*/
		PACKET_MAX=50999													//消息类型的最大值
	};
};


#endif

