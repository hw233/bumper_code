////////////////////////////////////////////////////////////////
//文件名称：Scene.cpp
//功能描述：地图信息类
//版本说明：
//
//编写作者：2017.09.01 sunbin
//
////////////////////////////////////////////////////////////////

/*
#include <fstream>
#include "Scene.h"
#include "logger.h"
#include "Common.h"

#include <google/protobuf/util/json_util.h>
using namespace google::protobuf::util;


Scene::Scene(uint32_t mapId):m_id(mapId)
{
}

Scene::~Scene()
{
}

bool Scene::init()
{
    static char fileName[32];
    snprintf(fileName, 32, "map/%u.map", m_id);
    fstream input(fileName, ios::in | ios::binary);

    if(!m_sceneMap.ParseFromIstream(&input))
    {
        LOG_ERROR("Scene::init init map from file[%s]  error! please check it",fileName);
        return false;
    }
    this->Show();
    return true;
}

void Scene::Show(){
    LOGDEBUG("....................SceneMap Info: id=%d............................", m_id);
    int obj_info_size = m_sceneMap.obj_info_size();
    int status_vector_size = m_sceneMap.status_vector_size();
    LOGDEBUG("owner_uuid=%s, map_id=%d, scene_id=%s, obj_info_size=%d, status_vector_size=%d",
            m_sceneMap.owner_uuid().c_str(), m_sceneMap.map_id(), m_sceneMap.scene_id().c_str(),
            obj_info_size, status_vector_size);

    for (int i=0; i<obj_info_size; ++i){
        SceneMap_ObjInfo objInfo = m_sceneMap.obj_info(i);
        LOGDEBUG("objInfo: uuid=%s, class_id=%d, item_id=%d, PosRot:Quaternion[w=%f, x=%f, y=%f, z=%f], PosRot:Vector3[x=%f, y=%f, z=%f]",
                objInfo.uuid().c_str(), objInfo.class_id(), objInfo.item_id(),
                objInfo.pos_rot().rotation().w(), objInfo.pos_rot().rotation().x(), objInfo.pos_rot().rotation().y(), objInfo.pos_rot().rotation().z(),
                objInfo.pos_rot().position().x(), objInfo.pos_rot().position().y(), objInfo.pos_rot().position().z());
    }

    for (int i=0; i<status_vector_size; ++i){
        ::NetProto::Action::Status::NaturalActionStatus status = m_sceneMap.status_vector(i);
        LOGDEBUG("status: flag=%d, type=%d, data=%s", status.flag(), status.type(), status.data().ShortDebugString().c_str());
    }
    LOGDEBUG("....................SceneMap Info............................", m_id);
}

Scene Scene::Clone(){
    Scene tmp(m_id);
    tmp.m_sceneMap.CopyFrom(this->m_sceneMap);
    return tmp;
}


GS2CEnterScene Scene::ToGS2CEnterScene(){
    GS2CEnterScene gs2cEnterScene;
    gs2cEnterScene.set_owner_uuid(m_sceneMap.owner_uuid());
    gs2cEnterScene.set_map_id(m_sceneMap.map_id());
    gs2cEnterScene.set_scene_id(m_sceneMap.scene_id());

    int obj_info_size = m_sceneMap.obj_info_size();
    for (int i=0; i<obj_info_size; ++i){
        GS2CEnterScene_ObjInfo* enterscene_objInfo = gs2cEnterScene.add_obj_info();
        //add by huyf 2017.10.27
        if(!enterscene_objInfo)
            continue;
        //add end
        SceneMap_ObjInfo scenemap_objInfo = m_sceneMap.obj_info(i);
        enterscene_objInfo->set_uuid(scenemap_objInfo.uuid());
        enterscene_objInfo->set_class_id(scenemap_objInfo.class_id());
        enterscene_objInfo->set_item_id(scenemap_objInfo.item_id());
        enterscene_objInfo->mutable_pos_rot()->CopyFrom(scenemap_objInfo.pos_rot());
    }

    int status_vector_size = m_sceneMap.status_vector_size();
    for (int i=0; i<status_vector_size; ++i){
        //add by huyf 2017.10.27
        if(!gs2cEnterScene.add_status_vector())
            continue;
        //add end
        gs2cEnterScene.add_status_vector()->CopyFrom(m_sceneMap.status_vector(i));
    }
    return gs2cEnterScene;
}
*/