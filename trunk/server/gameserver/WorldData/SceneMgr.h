////////////////////////////////////////////////////////////////
//文件名称：SceneMgr.h
//功能描述：地图管理类
//版本说明：
//
//编写作者：2017.09.01 sunbin
//
////////////////////////////////////////////////////////////////

#ifndef __SCENEMGR_H__
#define __SCENEMGR_H__

#include "Scene.h"
/*
typedef  std::map<uint32_t,Scene*> SceneTable;

class SceneMgr
{
    private:
        SceneMgr();
        ~SceneMgr();
        SceneMgr(const SceneMgr &sceneMgr);
        SceneMgr& operator=(const SceneMgr &sceneMgr);

    public:
        static SceneMgr& Instance();

        Scene* operator [](uint32_t i)
        {
            if(m_sceneTable.find(i) != m_sceneTable.end())
            {
                return m_sceneTable[i];
            }
            return NULL;
        }

        bool init();

        Scene* DefaultScene(){
            return operator[](1);
        }

    private:
        SceneTable m_sceneTable;
};

//extern static SceneMgr g_sceneMgr;
*/
#endif //__SCENEMGR_H__

