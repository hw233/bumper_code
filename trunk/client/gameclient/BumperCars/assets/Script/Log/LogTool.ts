import { MsgID, Msg_Walk_Type, Msg_Common_Type, Msg_Friend_Type, Msg_Friend_Info_Type, Msg_SignIn_Type, Msg_Task_Type, Msg_UserAttrib_Type, Msg_ItemAttrib_Type } from '../network/socket/MsgID';
import { Msg_Bettle_Type } from "../network/socket/MsgID";
import { Msg_Item_Type } from "../network/socket/MsgID";
import { Msg_Syn_Room_Type } from "../network/socket/MsgID";
import { Msg_Vas_Type } from "../network/socket/MsgID";
import { Msg_Exchange_Type } from "../network/socket/MsgID";
import { Msg_Action_Type } from "../network/socket/MsgID";

import GameConfig from "../GameConfig";

/**
 * LOG类型
 * 普通log
 * 错误log
 * 调试log
 * 网络log
 * 同步log
 */


export default class GameLog
{
    private static LogMessage: Object = null;

    public static InitLog()
    {
        if(GameConfig.NetLogSwitch === false)return;

        this.LogMessage = {};
        this.LogMessage[MsgID.MSG_BATTLE] = {};
        this.LogMessage[MsgID.MSG_VAS] = {};
        this.LogMessage[MsgID.MSG_ITEM_ATTR] = {};
        this.LogMessage[MsgID.MSG_EXCHANGE] = {};
        this.LogMessage[MsgID.MSG_SYN_ROOM] = {};
        this.LogMessage[MsgID.MSG_ACTION] = {};
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER] = {};
        this.LogMessage[MsgID.MSG_FRIEND] = {};
        this.LogMessage[MsgID.MSG_FRIEND_INFO] = {};
        this.LogMessage[MsgID.MSG_SIGNIN] = {};
        this.LogMessage[MsgID.MSG_TASKDETAIL] = {};
        this.LogMessage[MsgID.MSG_USERATTRIB] = {};
        this.LogMessage[MsgID.MSG_ITEMTTRIB] = {};
        // this.LogMessage[MsgID.MSG_WALK] = {};

        this.LogMessage[MsgID.MSG_TASKDETAIL][Msg_Task_Type._TASK_DETAIL_LIST]                               = "获取任务列表";
        this.LogMessage[MsgID.MSG_TASKDETAIL][Msg_Task_Type._TASK_DETAIL_ADD]                                = "添加任务";
        this.LogMessage[MsgID.MSG_TASKDETAIL][Msg_Task_Type._TASK_DETAIL_LIST_SUC]                           = "获取任务列表成功";
        this.LogMessage[MsgID.MSG_TASKDETAIL][Msg_Task_Type._TASK_DETAIL_LIST_FAIL]                          = "获取任务列表失败";
        this.LogMessage[MsgID.MSG_TASKDETAIL][Msg_Task_Type._TASK_DETAIL_UPDATE]                             = "更新任务状态";
        this.LogMessage[MsgID.MSG_TASKDETAIL][Msg_Task_Type._TASK_DETAIL_AWARD]                              = "领取奖励";
        this.LogMessage[MsgID.MSG_TASKDETAIL][Msg_Task_Type._TASK_DETAIL_AWARD_SUC]                          = "领取奖励成功";
        this.LogMessage[MsgID.MSG_TASKDETAIL][Msg_Task_Type._TASK_DETAIL_AWARD_FAIL]                         = "领取奖励失败";
        this.LogMessage[MsgID.MSG_TASKDETAIL][Msg_Task_Type._TASK_DETAIL_TRY_GAME]                           = "试玩任务上报";
        this.LogMessage[MsgID.MSG_TASKDETAIL][Msg_Task_Type._TASK_DETAIL_TRY_GAME_FAIL]                      = "试玩任务上报失败";

        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRI_HEAD_PICURL]                    = "【预留不做更新】";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRI_NICK_NAME]                      = "【预留不做更新】";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRI_GOLD_SUM]                       = "更新金币";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRI_KILL_SUM]                       = "更新击杀数";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRI_GRANDING_ID]                    = "更新段位";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRI_CUR_STAR_SUM]                   = "更新星星数";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRI_UNLOCK_CARS]                    = "更新已解锁车辆";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRI_USE_CAR]                        = "更新使用中的车辆";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRI_KING_SUM]                       = "更新霸主令牌";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRI_GIFT_DAY]                       = "更新GIFT_DAY";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRI_GIFT_SUM]                       = "更新领取礼包的次数";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRI_STAR_SUM]                       = "更新保留星星的次数";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRI_FIGHT_KILL_SUM]                 = "更新FIGHT_KILL_SUM";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRI_FIGHT_SCORE]                    = "更新FIGHT_SCORE";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRI_ADD_SCORE]                      = "更新ADD_SCORE";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRI_ADD_GOLD]                       = "更新ADD_GOLD";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRI_RATIO]                          = "更新RATIO";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRI_ENEMY_ID]                       = "更新ENEMY_ID";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRI_GET_GIFT]                       = "更新GET_GIFT";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRIB_DAYTASKMASK_FIR]               = "天任务掩码位1";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRIB_SIGNINMASK_FIR]                = "签到掩码位1";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRIB_SHOW_IDX]                      = "展示索引【预留后期扩展前期不适用】";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRIB_SHOW_ITEM_TYPE]                = "限时皮肤使用奖励道具的ItemType";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRIB_SHOW_REMAIN_TIME]              = "展示的道具的剩余时间";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRIB_ONLINE_IDX]                    = "在线领奖索引";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRIB_ONLINE_ITEM_TYPE]              = "在线领奖索引展示的道具itemtype";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRIB_ONLINE_REMAIN_TIME]            = "在线领奖索引展示的道具的剩余时间";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRIB_TASKMASK]                      = "永久任务掩码位";
        this.LogMessage[MsgID.MSG_USERATTRIB][Msg_UserAttrib_Type._USERATTRIB_WEEKTASKMASK]                  = "每周任务掩码位";

        this.LogMessage[MsgID.MSG_ITEMTTRIB][Msg_ItemAttrib_Type._ITEMATTRIB_ID]                             = "道具唯一ID";
        this.LogMessage[MsgID.MSG_ITEMTTRIB][Msg_ItemAttrib_Type._ITEMATTRIB_USER_ID]                        = "用户ID";
        this.LogMessage[MsgID.MSG_ITEMTTRIB][Msg_ItemAttrib_Type._ITEMATTRIB_ITEMTYPE]                       = "道具类型typeID";
        this.LogMessage[MsgID.MSG_ITEMTTRIB][Msg_ItemAttrib_Type._ITEMATTRIB_TYPE]                           = "道具类型type";
        this.LogMessage[MsgID.MSG_ITEMTTRIB][Msg_ItemAttrib_Type._ITEMATTRIB_LIFE_TIME]                      = "试用时间（剩余）";
        this.LogMessage[MsgID.MSG_ITEMTTRIB][Msg_ItemAttrib_Type._ITEMATTRIB_AMOUNT]                         = "叠加数量(当前)";


        this.LogMessage[MsgID.MSG_LOGIN]                                                                        = "登陆";
        this.LogMessage[MsgID.MSG_USERINFO]                                                                     = "用户信息";
        this.LogMessage[MsgID.MSG_WALK]                                                                         = "同步包";

        this.LogMessage[MsgID.MSG_SIGNIN][Msg_SignIn_Type._SIGNIN_ACTION_SIGNIN]                               = "签到领奖";
        this.LogMessage[MsgID.MSG_SIGNIN][Msg_SignIn_Type._SIGNIN_ACTION_SIGNIN_SUC]                           = "签到领奖成功";
        this.LogMessage[MsgID.MSG_SIGNIN][Msg_SignIn_Type._SIGNIN_ACTION_SIGNIN_FAIL]                          = "签到领奖失败";


        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_CREATE_ROOM]                           = "创建房间";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_CREATE_ROOM_SUC]                       = "创建房间成功";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_CREATE_ROOM_FAIL]                      = "创建房间失败";

        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_QUICK_MATCH]                           = "快速匹配";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_QUICK_MATCH_SUC]                       = "快速匹配成功";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_QUICK_MATCH_FAIL]                      = "快速匹配失败";

        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_QUICK_JOIN]                            = "快速加入";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_QUICK_JOIN_SUC]                        = "快速加入成功";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_QUICK_JOIN_FAIL]                       = "快速加入失败";

        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_INVITE_JOIN]                           = "邀请好友加入";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_INVITE_JOIN_SUC]                       = "邀请好友加入成功";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_INVITE_JOIN_FAIL]                      = "邀请好友加入失败";

        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_BATTLE_START]                          = "战斗开始";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_BATTLE_START_SUC]                      = "战斗开始成功";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_BATTLE_START_FAIL]                     = "战斗开始失败";

        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_BATTLE_COLLIDE]                        = "战斗中碰撞";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_BATTLE_COLLIDE_SUC]                    = "战斗中碰撞成功";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_BATTLE_COLLIDE_FAIL]                   = "战斗中碰撞失败";

        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_BATTLE_KILL]                           = "战斗中击杀";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_BATTLE_KILL_SUC]                       = "战斗中击杀成功";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_BATTLE_KILL_FAIL]                      = "战斗中击杀失败";

        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_BATTLE_SYN_SCORE]                      = "战斗中同步积分";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_BATTLE_SYN_SCORE_SUC]                  = "战斗中同步积分成功";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_BATTLE_SYN_SCORE_FAIL]                 = "战斗中同步积分失败";

        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_BATTLE_RELIVE]                         = "战斗中复活";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_BATTLE_RELIVE_SUC]                     = "战斗中复活成功";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_BATTLE_RELIVE_FAIL]                    = "战斗中复活失败";

        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_BATTLE_END]                            = "战斗结束";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_BATTLE_END_SUC]                        = "战斗结束成功";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_BATTLE_END_FAIL]                       = "战斗结束失败";

        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_VIEW_AWARD]                            = "3倍奖励";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_VIEW_AWARD_SUC]                        = "3倍奖励成功";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_VIEW_AWARD_FAIL]                       = "3倍奖励失败";


        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_TEAM_START]                            = "红蓝对战中战斗开始";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_TEAM_START_SUC]                        = "红蓝对战中战斗开始成功";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_TEAM_START_FAIL]                       = "红蓝对战中战斗开始失败";

        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_TEAM_END]                              = "红蓝对战中战斗结束";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_TEAM_END_SUC]                          = "红蓝对战中战斗结束成功";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_TEAM_END_FAIL]                         = "红蓝对战中战斗结束失败";

        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_TEAM_SYN_SCORE]                        = "红蓝对战中同步队伍积分、击杀方车辆等级";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_TEAM_SYN_SCORE_SUC]                    = "红蓝对战中同步队伍积分、击杀方车辆等级成功";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_TEAM_SYN_SCORE_FAIL]                   = "红蓝对战中同步队伍积分、击杀方车辆等级失败";

        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_LEAVE_TEAM]                            = "离开队伍";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_LEAVE_TEAM_SUC]                        = "离开队伍成功";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_LEAVE_TEAM_FAIL]                       = "离开队伍失败";

        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_END_AWARD]                             = "战斗结束同步前端霸主令牌和金币奖励";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_END_AWARD_SUC]                         = "战斗结束同步前端霸主令牌和金币奖励";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_END_AWARD_FAIL]                        = "战斗结束同步前端霸主令牌和金币奖励";

        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_TEAM_RELIVE]                           = "战斗中复活-[红蓝战队]";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_TEAM_RELIVE_SUC]                       = "战斗中复活-[红蓝战队]成功";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_TEAM_RELIVE_FAIL]                      = "战斗中复活-[红蓝战队]失败";

        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_LOADING_COMPLETE]                      = "进入游戏-[大乱斗]";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_LOADING_COMPLETE_SUC]                  = "进入游戏-[大乱斗]成功";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_LOADING_COMPLETE_FAIL]                 = "进入游戏-[大乱斗]失败";

        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_TEAM_LOADING_COMPLETE]                 = "进入游戏-[红蓝战队]";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_TEAM_LOADING_COMPLETE_SUC]             = "进入游戏-[红蓝战队]成功";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_TEAM_LOADING_COMPLETE_FAIL]            = "进入游戏-[红蓝战队]失败";
        
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_CHANGE_TEAM_LEADER]                    = "队长变更-[红蓝战队]";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_CHANGE_TEAM_LEADER_SUC]                = "队长变更-[红蓝战队]成功";
        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_CHANGE_TEAM_LEADER_FAIL]               = "队长变更-[红蓝战队]失败";

        this.LogMessage[MsgID.MSG_BATTLE][Msg_Bettle_Type._BATTLE_ACTION_INVITE_FRIEND_JOIN]                    = "邀请好友加入队伍";

        // this.LogMessage[MsgID.MSG_WALK][Msg_Walk_Type._WALK_ACTION_USER_CMD]                                 = "用户操作";
        // this.LogMessage[MsgID.MSG_WALK][Msg_Walk_Type._WALK_ACTION_CONTINUE_PREVIOUS_CMD]                    = "空桢";
        // this.LogMessage[MsgID.MSG_WALK][Msg_Walk_Type._WALK_ACTION_USER_COLLIDE_CMD]                         = "碰撞";
        // this.LogMessage[MsgID.MSG_WALK][Msg_Walk_Type._WALK_ACTION_REBOUND_CMD]                              = "反弹";


        this.LogMessage[MsgID.MSG_VAS][Msg_Vas_Type.BUY]                                                        = "购买Buff";
        this.LogMessage[MsgID.MSG_VAS][Msg_Vas_Type.BUY_SUCCESS]                                                = "购买Buff成功";
        this.LogMessage[MsgID.MSG_VAS][Msg_Vas_Type.BUY_FAIL]                                                   = "购买Buff失败";
        this.LogMessage[MsgID.MSG_VAS][Msg_Vas_Type._VAS_SHOP_BUY]                                              = "商城买道具";
        this.LogMessage[MsgID.MSG_VAS][Msg_Vas_Type._VAS_SHOP_BUY_SUC]                                          = "商城买道具成功";
        this.LogMessage[MsgID.MSG_VAS][Msg_Vas_Type._VAS_SHOP_BUY_FAIL]                                         = "商城买道具失败";

        this.LogMessage[MsgID.MSG_ITEM_ATTR][Msg_Item_Type._ITEM_ACTION_CREATE_RANDOM_ITEM_POS]                 = "获取随机点";
        this.LogMessage[MsgID.MSG_ITEM_ATTR][Msg_Item_Type._ITEM_ACTION_CREATE_RANDOM_ITEM]                     = "生成随机道具";
        this.LogMessage[MsgID.MSG_ITEM_ATTR][Msg_Item_Type._ITEM_ACTION_PICKUP_RANDOM_ITEM]                     = "删除道具";
        this.LogMessage[MsgID.MSG_ITEM_ATTR][Msg_Item_Type._ITEM_ACTION__ADD]                                   = "添加道具";
        this.LogMessage[MsgID.MSG_ITEM_ATTR][Msg_Item_Type._ITEM_ACTION__DEL]                                   = "删除道具";
        this.LogMessage[MsgID.MSG_ITEM_ATTR][Msg_Item_Type._ITEM_ACTION__UPDATE]                                = "更新道具(全量)";
        this.LogMessage[MsgID.MSG_ITEM_ATTR][Msg_Item_Type._ITEM_ACTION__USE]                                   = "使用某个道具";

        this.LogMessage[MsgID.MSG_ACTION][Msg_Action_Type._ACTION_GO_BACK_HOME_SCREEN]                          = "大乱斗战斗结束后返回游戏主界面";
        this.LogMessage[MsgID.MSG_ACTION][Msg_Action_Type._ACTION_GO_BACK_HOME_SCREEN_SUC]                      = "大乱斗战斗结束后返回游戏主界面成功";
        this.LogMessage[MsgID.MSG_ACTION][Msg_Action_Type._ACTION_GO_BACK_HOME_SCREEN_FAIL]                     = "大乱斗战斗结束后返回游戏主界面失败";
        this.LogMessage[MsgID.MSG_ACTION][Msg_Action_Type._ACTION_SIGNIN_BOX_RECEIVE]                           = "领取签到宝箱";
        this.LogMessage[MsgID.MSG_ACTION][Msg_Action_Type._ACTION_SIGNIN_BOX_RECEIVE_SUC]                       = "领取签到宝箱成功";
        this.LogMessage[MsgID.MSG_ACTION][Msg_Action_Type._ACTION_SIGNIN_BOX_RECEIVE_FAIL]                      = "领取签到宝箱失败";
        this.LogMessage[MsgID.MSG_ACTION][Msg_Action_Type._ACTION_LIMIT_TIME_AWARD]                             = "限时领奖";
        this.LogMessage[MsgID.MSG_ACTION][Msg_Action_Type._ACTION_LIMIT_TIME_AWARD_SUC]                         = "限时领奖成功";
        this.LogMessage[MsgID.MSG_ACTION][Msg_Action_Type._ACTION_LIMIT_TIME_AWARD_FAIL]                        = "限时领奖失败";
        this.LogMessage[MsgID.MSG_ACTION][Msg_Action_Type._ACTION_ONLINE_AWARD]                                 = "在线领奖";
        this.LogMessage[MsgID.MSG_ACTION][Msg_Action_Type._ACTION_ONLINE_AWARD_SUC]                             = "在线领奖成功";
        this.LogMessage[MsgID.MSG_ACTION][Msg_Action_Type._ACTION_ONLINE_AWARD_FAIL]                            = "在线领奖失败";
        this.LogMessage[MsgID.MSG_ACTION][Msg_Action_Type._ACTION_ONLINE_SPEED_UP]                              = "在线加速";
        this.LogMessage[MsgID.MSG_ACTION][Msg_Action_Type._ACTION_ONLINE_SPEED_UP_SUC]                          = "在线加速成功";
        this.LogMessage[MsgID.MSG_ACTION][Msg_Action_Type._ACTION_ONLINE_SPEED_UP_FAIL]                         = "在线加速失败";
        this.LogMessage[MsgID.MSG_ACTION][Msg_Action_Type._ACTIOIN_LIMIT_TIME_CONTINUE_TRY]                     = "显示免费皮肤继续使用";
        this.LogMessage[MsgID.MSG_ACTION][Msg_Action_Type._ACTIOIN_LIMIT_TIME_CONTINUE_TRY_SUC]                 = "显示免费皮肤继续使用成功";
        this.LogMessage[MsgID.MSG_ACTION][Msg_Action_Type._ACTIOIN_LIMIT_TIME_CONTINUE_TRY_FAIL]                = "显示免费皮肤继续使用失败";
        this.LogMessage[MsgID.MSG_ACTION][Msg_Action_Type._ACTION_ADD_APP_LIST]                                 = "添加到小程序列表";
        this.LogMessage[MsgID.MSG_ACTION][Msg_Action_Type._ACTION_ADD_APP_LIST_SUC]                             = "添加到小程序列表成功";
        this.LogMessage[MsgID.MSG_ACTION][Msg_Action_Type._ACTION_ADD_APP_LIST_FAIL]                            = "添加到小程序列表失败";


        this.LogMessage[MsgID.MSG_EXCHANGE][Msg_Exchange_Type.EXCHANGE_ITEM]                                    = "兑换道具";

        this.LogMessage[MsgID.MSG_SYN_ROOM][Msg_Syn_Room_Type.SYNROOMINFO_ACTION_ADD_MEMBER]                    = "加入一个玩家";
        this.LogMessage[MsgID.MSG_SYN_ROOM][Msg_Syn_Room_Type.SYNROOMINFO_ACTION_DEL_MEMBER]                    = "离开一个玩家";

        this.LogMessage[MsgID.MSG_FRIEND][Msg_Friend_Type._FRIEND_APPLY]                                        = "申请好友";
        this.LogMessage[MsgID.MSG_FRIEND][Msg_Friend_Type._FRIEND_ACCEPT]                                       = "接受请求";
        this.LogMessage[MsgID.MSG_FRIEND][Msg_Friend_Type._FRIEND_GETLIST]                                      = "获取好友列表";

        this.LogMessage[MsgID.MSG_FRIEND_INFO][Msg_Friend_Info_Type._FRIEND_INFO_ADD]                           = "添加好友";
        this.LogMessage[MsgID.MSG_FRIEND_INFO][Msg_Friend_Info_Type._FRIEND_INFO_DEL]                           = "删除好友请求";
        this.LogMessage[MsgID.MSG_FRIEND_INFO][Msg_Friend_Info_Type._FRIEND_INFO_UPDATE]                        = "更新好友数据";
        this.LogMessage[MsgID.MSG_FRIEND_INFO][Msg_Friend_Info_Type._FRIEND_INFO_ONLINE]                        = "上线";
        this.LogMessage[MsgID.MSG_FRIEND_INFO][Msg_Friend_Info_Type._FRIEND_INFO_OFFLINE]                       = "下线";
        this.LogMessage[MsgID.MSG_FRIEND_INFO][Msg_Friend_Info_Type._FRIEND_INFO_GETLIST]                       = "好友列表信息";


        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_GET_USER_INFO]                       = "获取用户数据";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_GET_USER_INFO_SUC]                   = "获取用户数据成功";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_GET_USER_INFO_FAIL]                  = "获取用户数据失败";

        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_UPLOAD_USER_DATA_GAMEOVER]           = "上传游戏结束数据";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_UPLOAD_USER_DATA_GAMEOVER_SUC]       = "上传游戏结束数据成功";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_UPLOAD_USER_DATA_GAMEOVER_FAIL]      = "上传游戏结束数据失败";

        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_UNLOCK_CAR]                          = "解锁车辆";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_UNLOCK_CAR_SUC]                      = "解锁车辆成功";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_UNLOCK_CAR_FAIL]                     = "解锁车辆失败";

        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_INVITE_USER]                         = "邀请用户";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_INVITE_USER_SUC]                     = "邀请用户成功";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_INVITE_USER_FAIL]                    = "邀请用户失败";

        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_IS_FRINED_JOIN]                      = "是否有好友加入";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_IS_FRINED_JOIN_SUC]                  = "是否有好友加入成功";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_IS_FRINED_JOIN_FAIL]                 = "是否有好友加入失败";

        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_USE_CAR]                             = "记录使用中的车辆";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_USE_CAR_SUC]                         = "记录使用中的车辆成功";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_USE_CAR_FAIL]                        = "记录使用中的车辆失败";

        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_RANK_GET_GRANDING_STARNUM]           = "根据名次获取当前段位和等级";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_RANK_GET_GRANDING_STARNUM_SUC]       = "根据名次获取当前段位和等级成功";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_RANK_GET_GRANDING_STARNUM_FAIL]      = "根据名次获取当前段位和等级失败";

        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_UPLOAD_GOLDNUM_KILLNUM]              = "金币和击杀数据上传";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_UPLOAD_GOLDNUM_KILLNUM_SUC]          = "金币和击杀数据上传成功";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_UPLOAD_GOLDNUM_KILLNUM_FAIL]         = "金币和击杀数据上传失败";

        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_GET_NOT_DECREASE_STAR_GIFT]          = "领取不减星礼包";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_GET_NOT_DECREASE_STAR_GIFT_SUC]      = "领取不减星礼包成功";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_GET_NOT_DECREASE_STAR_GIFT_FAIL]     = "领取不减星礼包失败";

        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_GET_BIG_GIFT]                        = "领取大礼包";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_GET_BIG_GIFT_SUC]                    = "领取大礼包成功";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_GET_BIG_GIFT_FAIL]                   = "领取大礼包失败";

        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_RANKING_LIST_BY_KILLNUM]             = "获取击杀排行榜";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_RANKING_LIST_BY_KILLNUM_SUC]         = "获取击杀排行榜成功";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_RANKING_LIST_BY_KILLNUM_FAIL]        = "获取击杀排行榜失败";

        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_RANKING_LIST_BY_RANK]                = "获取段位排行榜";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_RANKING_LIST_BY_RANK_SUC]            = "获取段位排行榜成功";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_RANKING_LIST_BY_RANK_FAIL]           = "获取段位排行榜失败";
        
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_TAIL]                               = "使用拖尾";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_TAIL_SUC]                           = "使用拖尾成功";
        this.LogMessage[MsgID.MSG_BUMPER_TRANSFER][Msg_Common_Type._BUMPER_TRANSFER_TAIL_FAIL]                          = "使用拖尾失败";
    }

    public static PrintSendNetLog(msgID, msgType, strData)
    {
        if(GameConfig.NetLogSwitch === false)return;

        let strInfo = "";
        if(msgID)
        {
            if(this.LogMessage[msgID])
            {
                if(msgType != undefined && msgID != MsgID.MSG_WALK)
                {
                    if(this.LogMessage[msgID][msgType])
                    {
                        strInfo = this.LogMessage[msgID][msgType];
                    }
                }else
                {
                    strInfo = this.LogMessage[msgID];
                }
            }
        }

        if(!strInfo)
        {
            strInfo = "NULL";
        }
        
        let time:any = "";
        if(GameConfig.Timestemp)
        {
            time = Date.now();
        }

        let logInfo: string = "";

        logInfo += "\n";
        logInfo += "\n";
        logInfo += "<<<<<<<<<<<<<<<发送消息:" + strInfo + "Start--->>>>>>>>>>>>>>>>>>";
        logInfo += "\n";
        logInfo += "\n";
        logInfo += "<<<<<<<<<<<<<<<---" + time + "-" + strData + "--->>>>>>>>>>>>>>>>>>";
        logInfo += "\n";
        logInfo += "\n";
        logInfo += "<<<<<<<<<<<<<<<发送消息:" + strInfo + "End--->>>>>>>>>>>>>>>>>>";
        logInfo += "\n";
        logInfo += "\n";

        // if(GameConfig.SaveLogSwitch)
        // {
        //     FileUtil.GetInstance().SaveLog(logInfo);
        // }

        console.log(logInfo);
    }

    public static PrintRecvNetLog(msgID, msgType, strData){
        if(GameConfig.NetLogSwitch === false)return;

        let strInfo = "";
        if(msgID)
        {
            if(this.LogMessage[msgID])
            {
                if(msgType != undefined)
                {
                    if(this.LogMessage[msgID][msgType])
                    {
                        strInfo = this.LogMessage[msgID][msgType];
                    }
                }else
                {
                    strInfo = this.LogMessage[msgID];
                }
            }
        }

        if(!strInfo)
        {
            strInfo = "NULL";
        }

        let time:any = "";
        if(GameConfig.Timestemp)
        {
            time = Date.now();
        }

        let logInfo: string = "";

        logInfo += ("\n");
        logInfo += ("\n");
        logInfo += ("<<<<<<<<<<<<<<<接受消息:" + strInfo + "Start--->>>>>>>>>>>>>>>>>>");
        logInfo += ("\n");
        logInfo += ("<<<<<<<<<<<<<<<---" + time + "-" + strData + "--->>>>>>>>>>>>>>>>>>");
        logInfo += ("\n");
        logInfo += ("<<<<<<<<<<<<<<<接受消息:" + strInfo + "End--->>>>>>>>>>>>>>>>>>");
        logInfo += ("\n");
        logInfo += ("\n");

        // if(GameConfig.SaveLogSwitch)
        // {
        //     FileUtil.GetInstance().SaveLog(logInfo);
        // }

        console.log(logInfo);
    }

    public static PrintSendWalkLog(...ops){
        if(GameConfig.WalkLogSwitch === false)return;

        let strLog = "";
        for(let i = 0;i < ops.length;i ++){
            let item = ops[i];
            if(typeof(item) == "object"){
                item = JSON.stringify(item);
            }
            strLog += item;
        }

        let logInfo: string = "";

        logInfo += ("\n");
        logInfo += ("发送同步消息Start\n");
        logInfo += ("<<<<<<<<<<<<<<<---" + strLog + "--->>>>>>>>>>>>>>>>>>");
        logInfo += ("发送同步消息End\n");
        logInfo += ("\n");

        console.log(logInfo);
    }

    public static PrintRecvWalkLog(...ops){
        if(GameConfig.WalkLogSwitch === false)return;

        let strLog = "";
        for(let i = 0;i < ops.length;i ++){
            let item = ops[i];
            if(typeof(item) == "object"){
                item = JSON.stringify(item);
            }
            strLog += item;
        }

        let logInfo: string = "";

        logInfo += ("\n");
        logInfo += ("接受同步消息Start\n");
        logInfo += ("<<<<<<<<<<<<<<<---" + strLog + "--->>>>>>>>>>>>>>>>>>");
        logInfo += ("接受同步消息End");
        logInfo += ("\n");

        console.log(logInfo);
    }

    public static PrintLog(...ops){
        if(GameConfig.LogSwitch === false)return;

        let strLog = "";
        for(let i = 0;i < ops.length;i ++){
            let item = ops[i];
            if(typeof(item) == "object"){
                item = JSON.stringify(item);
            }
            strLog += item;
        }

        let logInfo: string = "";

        logInfo += ("\n");
        logInfo += ("\n");
        logInfo += ("<<<<<<<<<<<<<<<---" + strLog + "--->>>>>>>>>>>>>>>>>>");
        logInfo += ("\n");
        logInfo += ("\n");

        // if(GameConfig.SaveLogSwitch)
        // {
        //     FileUtil.GetInstance().SaveLog(logInfo);
        // }

        console.log(logInfo);
    }

    public static PrintError(...ops)
    {
        if(GameConfig.ErrorLogSwitch === false)return;

        let strLog = "";
        for(let i = 0;i < ops.length;i ++){
            let item = ops[i];
            if(typeof(item) == "object"){
                item = JSON.stringify(item);
            }
            strLog += item;
        }

        let logInfo: string = "";

        logInfo += ("\n");
        logInfo += ("\n");
        logInfo += ("<<<<<<<<<<<<<<<---" + strLog + "--->>>>>>>>>>>>>>>>>>");
        logInfo += ("\n");
        logInfo += ("\n");

        // if(GameConfig.SaveLogSwitch)
        // {
        //     FileUtil.GetInstance().SaveLog(logInfo);
        // }

        console.error(logInfo);
    }

    public static PrintDebug(...ops)
    {
        if(GameConfig.DebugLogSwitch === false)return;

        let strLog = "";
        for(let i = 0;i < ops.length;i ++){
            let item = ops[i];
            if(typeof(item) == "object"){
                item = JSON.stringify(item);
            }
            strLog += item;
        }

        let logInfo: string = "";

        logInfo += ("\n");
        logInfo += ("\n");
        logInfo += ("<<<<<<<<<<<<<<<---" + strLog + "--->>>>>>>>>>>>>>>>>>");
        logInfo += ("\n");
        logInfo += ("\n");

        // if(GameConfig.SaveLogSwitch)
        // {
        //     FileUtil.GetInstance().SaveLog(logInfo);
        // }

        console.debug(logInfo);
    }
}
