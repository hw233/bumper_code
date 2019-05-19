////////////////////////////////////////////////////////////////
//文件名称：SceneMgr.cpp
//功能描述：地图管理类
//版本说明：
//
//编写作者：2017.09.01 sunbin
//
////////////////////////////////////////////////////////////////

/*
#include "SceneMgr.h"
#include "logger.h"
#include "Common.h"




SceneMgr::SceneMgr()
{
}

SceneMgr::~SceneMgr()
{
    CLEAR_MAP_DATA(m_sceneTable);
}

SceneMgr& SceneMgr::Instance(){
    static SceneMgr g_sceneMgr;    
    return g_sceneMgr;
}

bool SceneMgr::init()
{
    uint32_t mapIds[] = { 1, };

    for(uint8_t i = 0; i < sizeof(mapIds) / sizeof(uint32_t); ++i)
    {
        LOGDEBUG("SceneMgr::init mapId [%d]", mapIds[i]);
        Scene* tmp = new Scene(mapIds[i]);
        CHECKF(tmp);
        if(!tmp->init())
        {
            delete tmp;
            LOG_FATAL("SceneMgr::init map[%u] error", mapIds[i]);
            return false;
        }
        else
        {
            m_sceneTable.insert(std::pair<uint32_t,Scene*>(tmp->id(),tmp));
            LOGDEBUG("SceneMgr::init scene[%u] ok", mapIds[i]);
        }
    }
    return true;
}

*/