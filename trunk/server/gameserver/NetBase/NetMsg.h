////////////////////////////////////////////////////////////////
//文件名称：Packet.h
//功能描述：相关包基类
//文件名称：Packet API
//			
//编写作者：2017.08.15 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
////////////////////////////////////////////////////////////////

#ifndef __PACKET_H__
#define __PACKET_H__

#include "../Common/BaseType.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

class Socket;
class Player;
class SocketInputStream;
class SocketOutputStream;

#define GET_PACKET_INDEX(a) ((a) >> 24)
#define SET_PACKET_INDEX(a, index) ((a)=(((a) & 0xffffff) + ((index) << 24)))
#define GET_PACKET_LEN(a) ((a) & 0xffffff)
#define SET_PACKET_LEN(a, len) ((a)=((a) & 0xff000000) + (len))

/*
*-------------------header-info-----------------------------|
* 					   f    f    f    f    f    f    f    f
* |1 ~ 1| |1 ~ 1|	|1111-1111|1111-1111|1111-1111|1111-1111|   
* 	 ID    Tick		|             a                         |  
* 					   index          	    len 
*   32      32		32       24                  			1
* 					11*******11*****************************1
*/


enum PACKET_EXE
{
	PACKET_EXE_ERROR = 0,			//出现异常错误，断开此玩家连接
	PACKET_EXE_BREAK,
	PACKET_EXE_CONTINUE,
	PACKET_EXE_NOTREMOVE,
	PACKET_EXE_NOTREMOVE_ERROR,
};

class CNetMsg
{
public :
	BYTE			m_Index;						//消息索引号
	BYTE			m_Status;						//消息状态

public:
	CHAR* 	m_pData;
	UINT 	m_nDataLen;
	cJSON*  m_pMsg;
	Player* m_pPlayer;

public:
	CNetMsg();
	virtual ~CNetMsg();
	virtual VOID		CleanUp(){};
	//virtual BOOL		Read(SocketInputStream& iStream) = 0;	
    virtual BOOL        Read(SocketInputStream& iStream, INT nLen) = 0;
	virtual BOOL		Write(SocketOutputStream& oStream) const = 0;	
	virtual UINT		Execute(Player* pPlayer) = 0;	
	virtual	PacketID_t	GetPacketID() const = 0;	
	virtual	UINT		GetPacketSize() const = 0;
	virtual BOOL		CheckPacket()								{ 	return TRUE; 		}
	virtual	BOOL 		CreateMsg(UINT nRetStatus, UINT nAction);
	virtual	BOOL 		SendMsg(CNetMsg* pMsg);
	virtual UINT 		Process(VOID *pInfo);

	BYTE				GetPacketIndex() const 						{ 	return m_Index; 	}
	VOID				SetPacketIndex(BYTE Index )					{ 	m_Index = Index; 	}

	BYTE				GetPacketStatus() const 					{ 	return m_Status; 	}
	VOID				SetPacketStatus(BYTE Status )				{ 	m_Status = Status; 	}
};

#pragma pack(1)

struct ProtocolHead
{
	UInt32  pkgLen;            // pkg length
	UInt32  checkSum;          // adler32 check, adler32 of command, retCode and protobufData
	UInt16  cmd;           // Message type     --- Client Send Msg - NULL
	UInt16  target;            // Target Module    --- MessageTransferTarget
	UInt16  ret;           // return           --- 0 - success, != 0 - error. ERROR_CODE

    ProtocolHead(UInt32 c= 0,UInt16 rtn = 0, UInt16 cs=0):pkgLen(0),checkSum(cs),
    cmd(c),target(0),ret(rtn)
    {
    }

    UInt32 dataLen()
    {
        return pkgLen + sizeof(UInt32) - sizeof(ProtocolHead);
    }

    UInt32 getAllLen()
    {
        return pkgLen + sizeof(UInt32);
    }
};

struct ProtocolCmd 
{
	UInt32  pkgLen;            // pkg length
	UInt32  checkSum;          // adler32 check, adler32 of command, retCode and protobufData
	UInt16  cmd;           // Message type     --- Client Send Msg - NULL
	UInt16  target;            // Target Module    --- MessageTransferTarget
	UInt16  ret;           // return           --- 0 - success, != 0 - error. ERROR_CODE
	char    protobufData[0];   // Message Data     --- Protobuf Data

	ProtocolCmd():pkgLen(0),checkSum(0),cmd(0),ret(0)
	{}

	UInt32 dataLen()
	{
	    return pkgLen + sizeof(UInt32) - sizeof(ProtocolHead);
	}


	UInt32 getAllLen()
	{
	    return pkgLen + sizeof(UInt32);
	}
};
#pragma pack()




#define PACKET_HEADER_SIZE (sizeof(ProtocolHead))

#endif
