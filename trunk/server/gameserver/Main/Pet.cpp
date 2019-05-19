//////////////////////////////////////////////////////////////////////
//文件名称 : pet.cpp
//功能描述 : pet
//版本说明 : pet class API
//          
//编写作者：2017.11.20 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//

/*
//////////////////////////////////////////////////////////////////////
#include <string>
#include "Pet.h"
#include "../Common/Common.h"
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//
// func description :   constructor
//
//-----input------
// Parameters
//      无
//------output------
// Return 
//      无
Pet::Pet():m_petID(""),m_OwnerID(""),m_protoTypeID(0)
{

}

//////////////////////////////////////////////////////////////////////
//
// func description :   destructor
//
//-----input------
// Parameters
//      无
//------output------
// Return 
//      无
Pet::~Pet()
{

}

//////////////////////////////////////////////////////////////////////
//
// func description :   destructor
//
//-----input------
// Parameters
//      m_petID      :  const char*
//      protoTypeID  :  UInt32
//      无
//------output------
// Return 
//      无
bool Pet::Init(const char* petID, const char* ownerID, UInt32 protoTypeID)
{
    MY_TRY
    {
        //CHECKF(petID);
        m_petID = petID;
        m_OwnerID = ownerID;
        m_protoTypeID = protoTypeID;
        return true;
    }
    MY_CATCH
    {
        LOGERROR("Init Is Error code:petID=[%s]\townerID=[%s]\tprotoTypeID=[%d]", petID, ownerID, protoTypeID);
        return false;
    }
}
*/