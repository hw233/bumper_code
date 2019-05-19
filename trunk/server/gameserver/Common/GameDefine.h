//////////////////////////////////////////////////////////////////////
//文件名称：GameDefine.h
//功能描述：游戏基本信息宏定义
//版本说明：GameDefine API
//			
//编写作者：2017.08.21 yanfeng.hu
//
//修改情况：完成基本功能搭建 V1.0.0
//
//////////////////////////////////////////////////////////////////////

#ifndef __GAMEDEFINE_H__
#define __GAMEDEFINE_H__

//////////////////////////////////////////////////////////////////////
//头文件
//////////////////////////////////////////////////////////////////////
//#include "GameDefine.h"


//////////////////////////////////////////////////////////////////////
//常量预定于
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//游戏基本信息宏定义
//////////////////////////////////////////////////////////////////////

//世界号最大值
#define MAX_WORLD 255

#define MAX_WORLD_USER	3000

//用户角色名字可以输入的最大值
#define MAX_CHARACTER_INPUTNAME 24

//用户角色名字结构 |CharacterName|@|WorldID|00|
//					角色名字      1  3      2个零
//注意：角色名字后面的后缀：“@WorldID”是在用户创建角色时候自动添加进的；
//		服务器端处理时候都是使用完整名字；
//		客户端显示的时候，如果当前角色是本世界创建，则需去掉后面的后缀；
//用户角色名称的最大值
#define MAX_CHARACTER_NAME 30

//用户帐号的最大值
#define MAX_ACCOUNT 16

//用户密码的最大值
#define MAX_PASSWORD 32

//最多能创建角色的数量
#define MAX_CREATE_PLAYER_NUM 3

//CharacterManager中缺省的ObjID缓存大小
#define DEFAULT_OBJ_COUNT 1024

//
#define ZONE_SIZE	10

//
#define MAX_SEND_TIME 1

//搜索Zone格子的范围,0=1格(仅仅包含自己在的那个格子)，1=9格;2=25格;3=49格;
#define MAX_RADIUS 2

#define MAX_REFESH_OBJ_ZONE_RADIUS		2

#define MAX_REFESH_OBJ_ZONE_ARRAY_SIZE	((MAX_REFESH_OBJ_ZONE_RADIUS - 1) * (MAX_REFESH_OBJ_ZONE_RADIUS - 1) + 1 )

//掉落包拾取的最大距离
#define MAX_BOX_OP_DISTANCE				3.0f
//背包的最大容量
#define MAX_BAG_SIZE 60

#define MAX_EXTBAG_SIZE 140

#define MAX_NPC_DISTANCE	5.0

//Container 偏移

//基本包偏移
#define BASE_CONTAINER_OFFSET	0

#define DB_BAG_POS_START			(BASE_CONTAINER_OFFSET)					// 0
#define	DB_BAG_POS_END				(MAX_BAG_SIZE+MAX_EXTBAG_SIZE-1)		// 199

#define DB_EQUIP_POS_START			(MAX_BAG_SIZE+MAX_EXTBAG_SIZE)			// 200
#define DB_EQUIP_POS_END			(DB_EQUIP_POS_START+HEQUIP_NUMBER-1)	// 208

#define DB_BANK_POS_START			(DB_EQUIP_POS_START+HEQUIP_TOTAL)		// 220
#define DB_BANK_POS_END				(MAX_BANK_SIZE+DB_BANK_POS_START-1)		// 319


#define MAX_BANK_MONEY		10000000

//错误包格表示
#define	INVALID_BAG_INDEX (-1)

//一个场景最多可以有多少个玩家商店
#define MAX_SHOP_NUM_PER_SCENE 512

//一个玩家最多可以拥有的玩家商店个数
#define MAX_SHOP_NUM_PER_PLAYER 2


//最大药品效果
#define	MAX_MEDIC_EFFECT_NUM 3

#define MAX_GROW_POINT_RATE_NUM		4
#define MAX_LOGIC_FUNC_LEN			128
#define ITEM_LOGIC_FUNC_LEN			32
#define MAX_SCENE_GROW_POINT_NUM	128

#define MAX_SCENE_GP_OWNER			255

//最大表定义级别属性点分配
#define MAX_TABLE_DEFINE_LEVEL	 150

//断开网络后，服务器继续保留数据的时间（毫秒）
#define TIME_TO_TRUE_QUIT 10000

// 每个角色最多能看见多少种任务物品的掉落
#define MAX_CHAR_CAN_PICK_MISSION_ITEM_NUM	(MAX_CHAR_MISSION_NUM*4)

//技能OBJ最大数量
#define MAX_SKILLOBJ_NUMBER			1024

#define MAX_WEBSHOPINFO_NUMBER		3

//最大可打开ITEM BOX的距离
#define MAX_OPEN_ITEMBOX_DIST		(2.f)

//最大可打开ITEM BOX的距离平方
#define MAX_OPEN_ITEMBOX_DIST_SQ (MAX_OPEN_ITEMBOX_DIST * MAX_OPEN_ITEMBOX_DIST)

//最大可触发脚本事件的距离(客户端)
#define MAX_CLIENT_DEFAULT_EVENT_DIST (3.f)

//最大可触发脚本事件的距离(服务器)
#define MAX_SERVER_DEFAULT_EVENT_DIST	(MAX_CLIENT_DEFAULT_EVENT_DIST * 2.f)

//最大可触发脚本事件的距离平方(客户端)
#define MAX_CLIENT_DEFAULT_EVENT_DIST_SQ (MAX_CLIENT_DEFAULT_EVENT_DIST * MAX_CLIENT_DEFAULT_EVENT_DIST)

//最大可触发脚本事件的距离平方(服务器)
#define MAX_SERVER_DEFAULT_EVENT_DIST_SQ (MAX_SERVER_DEFAULT_EVENT_DIST * MAX_SERVER_DEFAULT_EVENT_DIST)


//数据库名称长度
#define		DATABASE_STR_LEN			128
//数据库用户名长度
#define		DB_USE_STR_LEN				32
//数据库密码长度
#define		DB_PASSWORD_STR_LEN			32
//DB 操作时间间隔
#define		DB_OPERATION_TIME			500
//账号保存角色个数
#define		DB_CHAR_NUMBER				5
//角色中非即时刷新属性同步到ShareMemory的时间间隔
#define		DB_2_SM_TIME				60000 //60秒

//数据库连接池默认连接个数
#define		DB_CONNECTION_COUNT			40

/////////////////////////////////////////////////////////////////////////////////
//游戏基本数据操作宏
/////////////////////////////////////////////////////////////////////////////////

//从GUID_t中取得世界号
#define GETWORLD(u) ((u)/10000000)
//从GUID_t中取得用户部分序列号
#define GETUSER(u) ((u)%10000000)
//判断当前是否是测试世界
#define ISTESTWORLD(w) ((w)>200?1:0)
//判断世界号是否合法
#define ISVALIDWORLD(w) (((w)>0)?(((w)<MAX_WORLD)?1:0):(0))


#define ABS(m) ((m)>0?(m):(m)*(-1))

#define LENGTH(x0,z0,x1,z1)  (ABS((x0)-(x1))+ABS((z0)-(z1)))

#define MAX_FILE_PATH  260

#define MAKE_COLOR(r,g,b,a)		(((r&0xFF)<<24)|((g&0xFF)<<16)|((b&0xFF)<<8)|(a&0xFF))
#define COLOR_R(color)			((color>>24)&0x000000FF)
#define COLOR_G(color)			((color>>16)&0x000000FF)
#define COLOR_B(color)			((color>>8)&0x000000FF)
#define COLOR_A(color)			((color)&0x000000FF)

#define GETHIGH(x) (x>>16)
#define GETLOW(x)  ((x<<16)>>16)


#endif
