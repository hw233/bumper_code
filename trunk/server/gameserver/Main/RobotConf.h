#ifndef _ROBOTCONF_H__
#define _ROBOTCONF_H__

#include <string>

#include "BaseType.h"

#pragma pack(1)

struct RobotBaseProp
{
    UInt8 robotId[32];
    UInt8 classId;
    UInt16 nameId;

    UInt32 age;
    UInt8 gender;
    UInt32 intellectual;
    UInt32 beautiful;
    UInt32 lovely;
    UInt32 skinState;
    UInt32 longPhy;
    UInt32 shortPhy;
    UInt32 energy;
    UInt32 thirsty;
    UInt32 hunger;
    UInt32 clean;
    UInt16 sufaceTemperature;
    UInt32 money;
    UInt32 registerTime; // game server ts
    //char   birthday[20];   // pw time 2017-03-29 00:00:00
    UInt64 birthday;
    UInt64 offlinePwTs;
    UInt16 locality;     // ref HouseArea.xlsx
    UInt32 sleep;
};

#pragma pack()

#endif // _ROBOTCONF_H__