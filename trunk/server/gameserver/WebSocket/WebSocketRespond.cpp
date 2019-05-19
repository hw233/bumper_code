#include "WebSocketRespond.h"
#include <stdlib.h>
#include <string.h>
#include "../Common/Common.h"

WebsocketRespond::WebsocketRespond(): m_ucFin(),m_ucOpcode(), m_ucMask()
{

}

WebsocketRespond::~WebsocketRespond() 
{ 
}


int  WebsocketRespond::PackData(const unsigned char* pMessage, size_t unMsgLen, uint8_t ucFin , 
                   uint8_t ucOpcode, uint8_t ucMask, unsigned char** ppOut, size_t* pOutLen)
{
    if(pMessage == NULL || unMsgLen<0 || pOutLen== NULL )
    {
        return -1;
    }

    int nHeadLength=0;                                   //记录消息头部的长度
    size_t          unOutLenTmp = unMsgLen;              //临时变量
    unsigned char*  pOutTmp     = NULL; 

    // 基本一个包可以发送完所有数据
    if(unMsgLen < 126)                                  //如果不需要扩展长度位, 两个字节存放 ucFin(1bit) + rsv[3](1bit) + ucOpcode(4bit); 
                                                        //ucMask(1bit) + payloadLength(7bit);  
        unOutLenTmp += 2;  
    else if (unMsgLen < 0xFFFF)                         //如果数据长度超过126 并且小于两个字节, 我们再用后面的两个字节(16bit) 表示 uint16_t  
        unOutLenTmp += 4;  
    else                                                //如果数据更长的话, 我们使用后面的8个字节(64bit)表示 uint64_t  
        unOutLenTmp += 8;  

    if (ucMask & 0x1)                                   //判断是不是1  
        unOutLenTmp += 4;                               //4byte 掩码位 

    //LOGDEBUG("unOutLenTmp = [%d]\n ", unOutLenTmp);

    pOutTmp = (unsigned char*)realloc((void*)pOutTmp, unOutLenTmp);   //长度已确定，可以分配内存

    //做数据处理
    memset(pOutTmp, 0, unOutLenTmp);                      //申请空间+1做\0

    *pOutTmp = ucFin << 7;                                //开始处理第一个字节的数据                   
    *pOutTmp = *pOutTmp | (0xF & ucOpcode);               //处理ucOpcode, pOutTmp 在 第一个字节开始处

    *(pOutTmp + 1) = ucMask << 7;                         //开始处理第二个字节的数据, pOutTmp在第一个字节的起始位置, 设置ucMask为 1
    if (unMsgLen < 126)                 
    {  
        *(pOutTmp + 1) = *(pOutTmp + 1) | unMsgLen;      //为第二个字节的payload len赋值   
        nHeadLength += 2;                                //nHeadLength标志着已经处理了2个字节的数据
    }  
    else if (unMsgLen < 0xFFFF)  
    {  
        *(pOutTmp + 1) = *(pOutTmp + 1) | 0x7E;          //设置第二个字节后7bit为126  
        uint16_t tmp = htons((uint16_t)unMsgLen);        //转换成网络字节序
        memcpy(pOutTmp + 2, &tmp, sizeof(uint16_t));     //给extended payload length 赋值
        nHeadLength += 4;                                //标志着已经处理了4个字节的数据
    }  
    else  
    {  
        *(pOutTmp + 1) = *(pOutTmp + 1) | 0x7F;          //设置第二个字节后为7bit 127  
        uint16_t tmp = htonl((uint16_t)unMsgLen);        //转换成网络字节序
        memcpy(pOutTmp + 2, &tmp, sizeof(uint16_t));     //给extended payload length 赋值
        nHeadLength += 10;                               //标志着已经处理了10个字节的数据
    }

    //处理掩码
    if (ucMask & 0x1)                                    //因为此时的ucMask为 1 ,条件为假
    {  
        nHeadLength += 4;                                //因协议规定, 从服务器向客户端发送的数据, 一定不能使用掩码处理. 所以这边省略  
    }  

    //LOGDEBUG("nHeadLength = [%d] pMessage=[%s] unMsgLen=[%d]\n ", nHeadLength, pMessage, unMsgLen);
    memcpy(pOutTmp + nHeadLength, pMessage, unMsgLen);    //拷贝真实的数据域    
    
    *pOutLen = unOutLenTmp;                               //间接赋值
    *ppOut   = pOutTmp;

    return 0; 
}
