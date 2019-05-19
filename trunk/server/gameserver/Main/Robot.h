#ifndef _ROBOT_H__
#define _ROBOT_H__

#include <string>

#include "../Common/Type.h"

#include "RobotConf.h"
#include "Player.h"

class Robot
{
public:
    Robot() {_accID = 0; _robotID = ""; _protoTypeID=0; memset(&_data, 0, sizeof(RobotBaseProp));};
    ~Robot() {};

public:
    bool init(const char* robotID, UInt32 accID, UInt32 protoTypeID = 0);
    bool CleanUp();

    bool awaken();
    bool offline();

public:
    inline const char* getRobotId()
    {
        return _robotID.c_str();
    }

    inline UInt32 getProtoTypeId()
    {
        return _protoTypeID;
    }

    inline UInt32 getaccId()
    {
        return _accID;
    }
    inline void setaccId(UInt32 accid)
    {
        _accID = accid;
    }

    inline RobotBaseProp& getRobotData()
    {
        return _data;
    }
    
    inline std::string& getRobotIdStr()
    {
        return _robotID;
    }

private:
    UInt32      _accID;
    std::string _robotID;
    UInt32      _protoTypeID;
    RobotBaseProp _data;
};

#endif // _ROBOT_H__