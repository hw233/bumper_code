
#ifndef _MSG_BUMPER_TRANSFER_H_
#define _MSG_BUMPER_TRANSFER_H_

#include <sstream>
#include "../Common/BaseType.h"
#include "../Common/Common.h"

#include "../Main/PacketFactory.h"
#include "../MsgHandler/NetMsg.h"
#include "../NetBase/SocketInputStream.h"
#include "../NetBase/SocketOutputStream.h"

#include "../Main/ClientManager.h"
#include "../Main/PlayerPool.h"
#include "../Main/ServerPlayer.h"
#include "../Main/Player.h"

#include "../Protocol/Command.h"
#include "../Protocol/cJSON.h"

#include <map>
#include "../Main/TopList/TopListManager.h"

enum
{
    //!alter by huyf 2019.01.12:此action 已经不再使用
    // _BUMPER_TRANSFER_GET_USER_INFO                  = 1,    //获取用户数据 
    // _BUMPER_TRANSFER_GET_USER_INFO_SUC              = 2,
    // _BUMPER_TRANSFER_GET_USER_INFO_FAIL             = 3,
    
    // _BUMPER_TRANSFER_UPLOAD_USER_DATA_GAMEOVER      = 4,    //上传游戏结束数据
    // _BUMPER_TRANSFER_UPLOAD_USER_DATA_GAMEOVER_SUC  = 5,
    // _BUMPER_TRANSFER_UPLOAD_USER_DATA_GAMEOVER_FAIL = 6,
  
    // _BUMPER_TRANSFER_UNLOCK_CAR                     = 7,    //解锁车辆
    // _BUMPER_TRANSFER_UNLOCK_CAR_SUC                 = 8,
    // _BUMPER_TRANSFER_UNLOCK_CAR_FAIL                = 9,
    //!alter end:此action 已经不再使用

    _BUMPER_TRANSFER_INVITE_USER                    = 10,   //邀请用户
    _BUMPER_TRANSFER_INVITE_USER_SUC                = 11,
    _BUMPER_TRANSFER_INVITE_USER_FAIL               = 12,

    _BUMPER_TRANSFER_IS_FRINED_JOIN                 = 13,   //是否有好友加入
    _BUMPER_TRANSFER_IS_FRINED_JOIN_SUC             = 14,
    _BUMPER_TRANSFER_IS_FRINED_JOIN_FAIL            = 15,

    _BUMPER_TRANSFER_USE_CAR                        = 16,   //记录使用中的车辆
    _BUMPER_TRANSFER_USE_CAR_SUC                    = 17,
    _BUMPER_TRANSFER_USE_CAR_FAIL                   = 18,

    _BUMPER_TRANSFER_RANK_GET_GRANDING_STARNUM      = 19,   //根据名次获取当前段位和等级
    _BUMPER_TRANSFER_RANK_GET_GRANDING_STARNUM_SUC  = 20,
    _BUMPER_TRANSFER_RANK_GET_GRANDING_STARNUM_FAIL = 21,

    _BUMPER_TRANSFER_UPLOAD_GOLDNUM_KILLNUM         = 22,   //金币和击杀数据上传
    _BUMPER_TRANSFER_UPLOAD_GOLDNUM_KILLNUM_SUC     = 23, 
    _BUMPER_TRANSFER_UPLOAD_GOLDNUM_KILLNUM_FAIL    = 24, 


    _BUMPER_TRANSFER_GET_NOT_DECREASE_STAR_GIFT     = 25,   //领取不减星礼包
    _BUMPER_TRANSFER_GET_NOT_DECREASE_STAR_GIFT_SUC = 26,
    _BUMPER_TRANSFER_GET_NOT_DECREASE_STAR_GIFT_FAIL= 27,

    _BUMPER_TRANSFER_GET_BIG_GIFT                   = 28,   //领取大礼包
    _BUMPER_TRANSFER_GET_BIG_GIFT_SUC               = 29,
    _BUMPER_TRANSFER_GET_BIG_GIFT_FAIL              = 30,

    _BUMPER_TRANSFER_RANKING_LIST_BY_KILLNUM        = 31,   //排行榜 by 击杀数
    _BUMPER_TRANSFER_RANKING_LIST_BY_KILLNUM_SUC    = 32,
    _BUMPER_TRANSFER_RANKING_LIST_BY_KILLNUM_FAIL   = 33,
    
    _BUMPER_TRANSFER_RANKING_LIST_BY_RANK           = 34,   //排行榜 by 段位
    _BUMPER_TRANSFER_RANKING_LIST_BY_RANK_SUC       = 35,
    _BUMPER_TRANSFER_RANKING_LIST_BY_RANK_FAIL      = 36,

    //add by huyf 2019.01.09
    _BUMPER_TRANSFER_TAIL                           = 37,   //设置拖尾
    _BUMPER_TRANSFER_TAIL_SUC                       = 38,
    _BUMPER_TRANSFER_TAIL_FAIL                      = 39,
    //add end
};

class CMsgBumperTransfer : public CNetMsg
{

public:
    CMsgBumperTransfer();
    virtual ~CMsgBumperTransfer();

    virtual BOOL Read(SocketInputStream& iStream, UINT nLen);
    virtual BOOL Write(SocketOutputStream& oStream) const;
    virtual PacketID_t GetPacketID() const { return _MSG_BUMPER_TRANSFER; }
    virtual UINT GetPacketSize() const { return m_nDataLen; }

    BOOL CreateMsg(UINT nAction, VT_RANK_TOP_LIST & vt_tmp, RankTopListStruct* pUserInfo);
    BOOL CreateMsg(UINT nAction, VT_KILLNUM_TOP_LIST & vt_tmp, KillNumTopListStruct* pUserInfo);

    virtual BOOL CreateMsg(UINT nAction, map<string, string> & data, UINT32 nData=0);
    virtual BOOL CreateMsg(UINT nAction, map<string, int> & data);
    virtual BOOL CreateMsg(UINT nAction, vector<map<string, string>*>& data, map<string, string> & user_info);
    
    virtual BOOL SendMsg(CNetMsg* pMsg);
    virtual UINT Process(VOID* pInfo);


};

class CMsgBumperTransferFactory : public PacketFactory
{

public:

        CNetMsg* CreatePacket() { return new CMsgBumperTransfer; }
        PacketID_t GetPacketID() const { return _MSG_BUMPER_TRANSFER; }
        UINT       GetPacketMaxSize() const { return m_nPacketSize; }
        VOID       SetPacketSize(UINT size) { m_nPacketSize = size; }

private:

        UINT m_nPacketSize;
};

#endif


