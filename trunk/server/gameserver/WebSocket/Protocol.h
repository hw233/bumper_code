
#ifndef _PROTOCOL_H_
#define _PORTOCOL_H_

typedef uint32_t UInt32;
typedef uint16_t UInt16;

enum _PROTOCOL_PACKET_TYPE{
        MSG_BATTLE = 2001,
        MSG_VAS,
        MSG_ACTION,
        MSG_ITEM,
        MSG_EXCHANGE,
};

struct ProtocolPacket{

        UInt32 pkg_type;
        UInt32 pkg_len;
        char protobufData[0];

        ProtocolPacket():pkg_type(0), pkg_len(0){
        }
        
        
};

#endif   //_PORTOCOL_H_



