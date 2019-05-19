// 数据库底层的通用接口
// huyf, 2008.9.27
//////////////////////////////////////////////////////////////////


#ifndef	I_MYDB_H
#define	I_MYDB_H


/////////////////////////////////////////////////////////////////////////////
// 数据库表名定义
//
#define		_TBL_PREHEAD				"g_bumper"

#define		_TBL_ACCOUNT				"account"
#define		_TBL_ITEM					_TBL_PREHEAD "item"							//玩家道具表
#define		_TBL_ITEMTYPE				_TBL_PREHEAD "itemtype"						//道具配置表
#define		_TBL_TASKDETAIL				_TBL_PREHEAD "taskdetail"					//任务配置表
#define		_TBL_CONFIG					_TBL_PREHEAD "config"						//游戏配置表
#define		_TBL_TASK					_TBL_PREHEAD "task"							//任务表
#define		_TBL_RELEASE_SWITCH			_TBL_PREHEAD "release_switch"				//配置开关
#define		_TBL_TYRO_GIFT				_TBL_PREHEAD "tyro_gift"					//新手送礼
#define		_TBL_FRIEND					_TBL_PREHEAD "friend"						//好友表
#define		_TBL_RECENT					_TBL_PREHEAD "recent"						//最近一起玩过游戏的玩家表
#define		_TBL_ITEMAWARD				_TBL_PREHEAD "item_award"					//道具奖励配置表
#define		_TBL_ITEMAWARD_GROUP		_TBL_PREHEAD "item_award_group"				//道具奖励组配置表
#define		_TBL_LIMIT_TIME_AWARD_CFG	_TBL_PREHEAD "limit_time_award_cfg"			//限时奖励组配置表
#define		_TBL_ONLINE_AWARD_CFG		_TBL_PREHEAD "online_award_cfg"				//限时奖励组配置表

/////////////////////////////////////////////////////////////////////////////


#endif // I_MYDB_H
