#include "DBMySql.h"
#include "../Common/Common.h"


DBMySql* g_pDbMySql = NULL;

DBMySql::DBMySql()
{
    m_mysql = NULL;
}

DBMySql::DBMySql(const std::string& host, const std::string& user, const std::string& passwd, const std::string& db, short port)
{
    m_host = host;
    m_user = user;
    m_passwd = passwd;
    m_db = db;
    m_port = port;

    m_mysql = NULL;

    Initialize();
}

DBMySql::~DBMySql()
{
    if (m_mysql)
    {
        mysql_close(m_mysql);
    }
    m_mysql = NULL;
}

int DBMySql::Initialize()
{
    m_mysql = mysql_init(NULL);

    if(m_mysql == NULL)
    {
        LOGERROR("[DBMySql::Initialize] m_mysql Is NULL");
        return -1;
    }

	if (mysql_set_character_set(m_mysql, "utf8"))
	{
        LOGERROR("[DBMySql::Initialize] mysql_set_character_set Is Error");
	}

    return Connect();
}

int DBMySql::Initialize(const std::string& host, const std::string& user, const std::string& passwd, const std::string& db, short port)
{
    m_host = host;
    m_user = user;
    m_passwd = passwd;
    m_db = db;
    m_port = port;

    m_mysql = mysql_init(NULL);

    if(m_mysql == NULL)
    {
        LOGERROR("[DBMySql::Initialize] m_mysql Is NULL");
        return -1;
    }
    
    if (mysql_set_character_set(m_mysql, "utf8"))
    {
        LOGERROR("[DBMySql::Initialize] mysql_set_character_set Is Error");
    }

    return Connect();
}

int DBMySql::Connect()
{
    if (mysql_real_connect(m_mysql, m_host.c_str(), m_user.c_str(), m_passwd.c_str(), m_db.c_str(), m_port, NULL, 0) == NULL)
    {
        LOGERROR("[DBMySql::Connect] mysql_real_connect Is Error...m_host[%s] m_user=[%s] m_passwd=[%s] m_db=[%s] m_port=[%d]", m_host.c_str(), m_user.c_str(), m_passwd.c_str(), m_db.c_str(), m_port);
        return -1;
	}
    return 0;
}

void DBMySql::Destroy()
{
    if (m_mysql)
    {
        mysql_close(m_mysql);
    }
    m_mysql = NULL;
}

std::string DBMySql::ErrorMsg(const std::string& func)
{
    std::ostringstream os;
    os << func;
    os << " errno = ";
    os << mysql_errno(m_mysql);
    os << ", error = ";
    os << mysql_error(m_mysql);

    std::string str(os.str());

    return str;
}

MYSQL_RES* DBMySql::Query(const std::string& str)
{
    int ret = Execute(str);
    if(ret == -1)
    {
        LOGERROR("[DBMySql::Initialize] execute Is Error ret == -1");
        return NULL;
    }

    return mysql_store_result(m_mysql);
}

int DBMySql::Insert(const std::string& str)
{
    return Execute(str);
}

int DBMySql::Update(const std::string& str)
{   
    return Execute(str);
}

int DBMySql::Execute(const std::string& str)
{
    //alter by huyf 2018.11.24:解决超过8小时候mysql连接自动断开，但是服务器端未察觉问题,增加try catch 模块处理
    //#0  0x00007ffff7bca9ff in __libc_send (fd=3, buf=0x711260, n=53, flags=0) at ../sysdeps/unix/sysv/linux/x86_64/send.c:26
    //#1  0x00007ffff7604575 in ?? () from /usr/lib/x86_64-linux-gnu/libmysqlclient.so.20
    //#2  0x00007ffff75e04e6 in ?? () from /usr/lib/x86_64-linux-gnu/libmysqlclient.so.20
    //#3  0x00007ffff75e06a3 in ?? () from /usr/lib/x86_64-linux-gnu/libmysqlclient.so.20
    //#4  0x00007ffff75e0d47 in ?? () from /usr/lib/x86_64-linux-gnu/libmysqlclient.so.20
    //#5  0x00007ffff75dbe78 in ?? () from /usr/lib/x86_64-linux-gnu/libmysqlclient.so.20
    //#6  0x00007ffff75d83e7 in mysql_send_query () from /usr/lib/x86_64-linux-gnu/libmysqlclient.so.20
    //#7  0x00007ffff75d8461 in mysql_real_query () from /usr/lib/x86_64-linux-gnu/libmysqlclient.so.20
    //#8  0x00000000004c0093 in DBMySql::Execute (this=0x70d290, str="SELECT * FROM g_bumper_info WHERE user_id=199151") at /root/bumper/bumper2/bumper2_code/server/trunk/GameServer/DataBase/DBMySql.cpp:144
    //#9  0x00000000004bff89 in DBMySql::Query (this=0x70d290, str="SELECT * FROM g_bumper_info WHERE user_id=199151") at /root/bumper/bumper2/bumper2_code/server/trunk/GameServer/DataBase/DBMySql.cpp:111
    try 
    {
        if(m_mysql == NULL)
        {       
            Initialize();
            LOGERROR("[DBMySql::Execute] Execute-1 Is Error Execute == NULL");

            if(m_mysql == NULL)
            {
                LOGERROR("[DBMySql::Execute] Execute-2 Is Error Execute == NULL");
                return -1;
            }
        }
        int ret = mysql_query(m_mysql, str.c_str());
        if(ret != 0)
        {
            LOGERROR("[DBMySql::Execute] mysql_query-1 Is Error ret=[%d]", ret);
            int err = GetErrno();
            if(err == 2006 || err == 2013) // MySQL server has gone away
            {
                LOGERROR("[DBMySql::Execute] mysql_query-2 Is Error err=[%d]", err);
                ret = Connect();        
                if(ret != 0)
                {
                    LOGERROR("[DBMySql::Execute] mysql_query-3 Is Error ret=[%d]", ret);
                    return ret;
                }

                ret = mysql_query(m_mysql, str.c_str());
                if(ret != 0)
                {
                    LOGERROR("[DBMySql::Execute] mysql_query-4 Is Error ret=[%d]", ret);
                }
            }
    	}

        return ret;
    }
    catch(...)
    {
        Destroy();
        LOGERROR("[DBMySql::Execute] mysql_query-5 Is catch str=[%s]", str.c_str());
        return -1;
    }
    //alter end:解决超过8小时候mysql连接自动断开，但是服务器端未察觉问题
}

int DBMySql::GetErrno()
{
    return mysql_errno(m_mysql);
}

int DBMySql::NumFields(MYSQL_RES* res)
{
    CHECKF(res);
    int num = mysql_num_fields(res);
    return num; 
}

int DBMySql::AffectedRows(MYSQL *mysql)
{
    CHECKF(mysql);
    int num = mysql_affected_rows(mysql);
    return num;
}

std::string DBMySql::GetError()
{
    return mysql_error(m_mysql);
}

bool DBMySql::UpdateRecord(std::string const& sql)
{
    int result = Execute(sql);
    if(result != 0)
    {
        LOGERROR("[DBMySql::UpdateRecord] Is Error...sql=[%s]", sql.c_str());
        return false;
    }

    return true;
}

//添加数据库插入后的id
int DBMySql::GetInsertId()
{   
    if (m_mysql)
    {
        return (int)mysql_insert_id(m_mysql);
    }
    else
        return 0;
}