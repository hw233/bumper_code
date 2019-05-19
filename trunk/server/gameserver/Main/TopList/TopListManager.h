//////////////////////////////////////////////////////////////////////
//文件名称：CTopListManager.h
//功能描述：
//版本说明：CTopListManager API
//			
//编写作者：2018.12, fliu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////
#ifndef __TOPLISTMANAGER_H__
#define __TOPLISTMANAGER_H__ 

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
#include "../Common/Type.h"
#include "../Common/BaseType.h"
#include "../Common/Thread.h"
//!alter by huyf 2018.04.23:添加连接开关可配置 
#include "../Common/Config.h"
//!alter end:添加连接开关可配置 

#include <vector>



//排行榜结构体-Rank
struct  RankTopListStruct
{
    std::string m_HeadPicurl;           //string 头像
    std::string m_NickName;             //string 昵称
    UInt32      m_nStarNum;             //int 当前星星数  
    UInt32      m_nGrandingId;          // int 段位
    UInt32      m_nRank;                //int 等级
    BOOL        m_nIsRank;              //bool 是否在排行榜
    UInt32      m_nUserId;              //用户user_id

    RankTopListStruct(BOOL isRank = 0):m_HeadPicurl(""), m_NickName(""), m_nStarNum(0), m_nGrandingId(0), m_nRank(0), m_nIsRank(isRank)
    {
    }
};


//排行榜结构体-KillNum
struct  KillNumTopListStruct
{
    std::string m_HeadPicurl;           //string 头像
    std::string m_NickName;             //string 昵称
    UInt32      m_nKillNum;            //int 击杀数量  
    UInt32      m_nStarNum;             //int 当前星星数  
    UInt32      m_nRank;                //int 等级
    BOOL        m_nIsRank;              //bool 是否在排行榜
    UInt32      m_nUserId;              //用户user_id

    /*
    使用GCC编译代码时遇到以下警告：
    warning "will be initialized after [-Wreorder]
    构造函数时，初始化成员变量的顺序要与类声明中的变量顺序相对应，若不对应，则出现如题错误。解决方法就是按照顺序进行初始化。
    */
    KillNumTopListStruct(BOOL isRank = 0):m_HeadPicurl(""), m_NickName(""), m_nKillNum(0), m_nStarNum(0), m_nRank(0), m_nIsRank(isRank)
    {
    }
    
};

enum _Rank_TOP_LIST
{
       _Rank_TOP_LIST_HEAD_PICURL       = 0, 
       _Rank_TOP_LIST_NICKNAME          = 1,
       _Rank_TOP_LIST_STAR_NUM          = 2,
       _Rank_TOP_LIST_GRANDING_ID       = 3,
       _Rank_TOP_LIST_USER_ID           = 4,
};

enum _KILLNUM_TOP_LIST
{
    _KILLNUM_TOP_LIST_HEAD_PICURL     =  0, 
    _KILLNUM_TOP_LIST_NICKNAME        =  1,
    _KILLNUM_TOP_LIST_KILL_NUM        =  2,
    _KILLNUM_TOP_LIST_STAR_NUM        =  3,
     _KILLNUM_TOP_LIST_USER_ID        = 4,
        
};


typedef std::vector<RankTopListStruct*> VT_RANK_TOP_LIST;
typedef std::vector<KillNumTopListStruct*> VT_KILLNUM_TOP_LIST;

//////////////////////////////////////////////////////////////////////
class CTopListManager
{
public:
	CTopListManager();
	virtual ~CTopListManager();
	BOOL Init();

public:

	//加载数据库文件
	BOOL 	LoadRankTopList();
	BOOL 	LoadKillNumTopList();
	BOOL    Loop();

public:	

    //Rank排行榜数据
    VT_RANK_TOP_LIST&      GetVtRankTopList(){      	return m_vRankTopList;	}

    //KILLNUM排行榜数据
    VT_KILLNUM_TOP_LIST&      GetVtKillNumTopList(){    		return m_vKillNumTopList;	}

private:
	tm *					m_UpdateTime;		//上次更新排行榜的数据

	VT_RANK_TOP_LIST        m_vRankTopList;
	VT_KILLNUM_TOP_LIST     m_vKillNumTopList;      	
};
//全局变量
extern CTopListManager* g_pTopListManager;
#define 	GetTopListManager() (g_pTopListManager)


 #endif//__TOPLISTMANAGER_H__