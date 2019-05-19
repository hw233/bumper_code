#include "../MsgHandler/NetMsg.h"
#include "../Common/Config.h"
#include "../Common/Common.h"

#include "../Base/logger.h"

#include "Robot.h"
#include "ClientManager.h"

#include "../Protocol/Command.h"
#include "../Protocol/cJSON.h"

bool Robot::init(const char* robotID, UInt32 accID, UInt32 protoTypeID)
{
    strncpy((char*)_data.robotId, robotID, 32);
    _robotID = robotID;
    _accID = accID;
    _protoTypeID = protoTypeID;
    _data.classId = g_robotConf_NICO.type;
    _data.nameId = g_robotConf_NICO.name_id;
    _data.age = g_robotConf_NICO.age;
    _data.gender = g_robotConf_NICO.gender;
    _data.intellectual = g_robotConf_NICO.intellectual;
    _data.beautiful = g_robotConf_NICO.beautiful;
    _data.lovely = g_robotConf_NICO.lovely;
    _data.skinState = g_robotConf_NICO.skin_state;
    _data.longPhy = g_robotConf_NICO.long_physical;
    _data.shortPhy = g_robotConf_NICO.short_physical;
    _data.energy = g_robotConf_NICO.energy;
    _data.thirsty = g_robotConf_NICO.thirsty;
    _data.hunger = g_robotConf_NICO.hunger;
    _data.clean = g_robotConf_NICO.clean;
    _data.sufaceTemperature = g_robotConf_NICO.surface_temperature;
    _data.money = g_robotConf_NICO.account;
    if(g_robotConf_NICO.bornArea)
        _data.locality = g_robotConf_NICO.bornArea;
    else
        _data.locality = 1; //todo
    //_data.sleep = ;
    //_data.registerTime = TimeUtil::Now();
    //_data.birthday = gOnline.getPwTime();
	return true;
}

bool Robot::awaken()
{
    cJSON* pRobotInfo = cJSON_CreateObject();
    if (pRobotInfo)
    {
        cJSON_AddStringToObject(pRobotInfo, "uuid", _robotID.c_str());
        cJSON_AddNumberToObject(pRobotInfo, "accid", _accID);
        /*
        cJSON_AddNumberToObject(pRobotInfo, "class_id", _data.classId);
        cJSON_AddNumberToObject(pRobotInfo, "name_id", _data.nameId);
        cJSON_AddNumberToObject(pRobotInfo, "age", _data.age);
        cJSON_AddNumberToObject(pRobotInfo, "gender", _data.gender);
        cJSON_AddNumberToObject(pRobotInfo, "intellectual", _data.intellectual);
        cJSON_AddNumberToObject(pRobotInfo, "beautiful", _data.beautiful);
        cJSON_AddNumberToObject(pRobotInfo, "lovely", _data.lovely);
        cJSON_AddNumberToObject(pRobotInfo, "skinState", _data.skinState);
        cJSON_AddNumberToObject(pRobotInfo, "longPhy", _data.longPhy);
        cJSON_AddNumberToObject(pRobotInfo, "shortPhy", _data.shortPhy);
        cJSON_AddNumberToObject(pRobotInfo, "energy", _data.energy);
        cJSON_AddNumberToObject(pRobotInfo, "thirsty", _data.thirsty);
        cJSON_AddNumberToObject(pRobotInfo, "hunger", _data.hunger);
        cJSON_AddNumberToObject(pRobotInfo, "clean", _data.clean);
        cJSON_AddNumberToObject(pRobotInfo, "suface_temperature", _data.sufaceTemperature);
        cJSON_AddNumberToObject(pRobotInfo, "money", _data.money);
        cJSON_AddNumberToObject(pRobotInfo, "locality", _data.locality);
        cJSON_AddNumberToObject(pRobotInfo, "birthday", _data.registerTime);
        cJSON_AddNumberToObject(pRobotInfo, "pw_birthday", _data.birthday);
        cJSON_AddNumberToObject(pRobotInfo, "offline_pw_time", _data.offlinePwTs);
        cJSON_AddNumberToObject(pRobotInfo, "sleep", _data.sleep);
        */

        LOGDEBUG("[Robot::awaken] SendMsgToServer-MTS=[GS2MTS_ROBOT_ONLINE=%d]", GS2MTS_ROBOT_ONLINE);
        char* pJson = cJSON_Print(pRobotInfo);
        CHECKF(pJson);
        std::string sJson(pJson);
        LOGDEBUG("[Robot::awaken] \njson:\t\n%s\t", sJson.c_str());
        g_pClientManager->SendMsgToServer(MTS_SERVER, sJson, GS2MTS_ROBOT_ONLINE);

        LOGDEBUG("[Robot::awaken] SendMsgToServer-PW=[GS2PW_ROBOT_ONLINE=%d]", GS2PW_ROBOT_ONLINE);
        g_pClientManager->SendMsgToServer(PW_SERVER, sJson, GS2PW_ROBOT_ONLINE);

        //add by huyf 2018.06.15:内存泄漏
        free(pJson);
        //add end:内存泄漏
    }

    cJSON_Delete(pRobotInfo);
	return true;
}

bool Robot::offline()
{
    cJSON* pRobotOffline = cJSON_CreateObject();
    if (pRobotOffline)
    {
        cJSON_AddStringToObject(pRobotOffline, "uuid", _robotID.c_str());
        cJSON_AddNumberToObject(pRobotOffline, "accid", _accID);

        LOGDEBUG("[Robot::awaken] SendMsgToServer-mts=[GS2MTS_ROBOT_OFFLINE=%d]", GS2MTS_ROBOT_OFFLINE);        
        char* pJson = cJSON_Print(pRobotOffline);
        CHECKF(pJson);
        std::string sJson(pJson);
        g_pClientManager->SendMsgToServer(MTS_SERVER, sJson, GS2MTS_ROBOT_OFFLINE);

        LOGDEBUG("[Robot::awaken] SendMsgToServer-PW=[GS2PW_ROBOT_OFFLINE%d]", GS2PW_ROBOT_OFFLINE);
        g_pClientManager->SendMsgToServer(PW_SERVER, sJson, GS2PW_ROBOT_OFFLINE);

        //add by huyf 2018.06.15:内存泄漏
        free(pJson);
        //add end:内存泄漏
    }
    cJSON_Delete(pRobotOffline);
	return true;
}
bool Robot::CleanUp()
{
    _accID = 0; 
    _robotID = "";
    _protoTypeID=0;
    memset(&_data, 0, sizeof(RobotBaseProp));
    return true;
}