#ifndef __DBMYSQL_H__
#define __DBMYSQL_H__

#include <stdio.h>
#include <string>
#include <cstring>
#include <iostream>
#include <sstream>   

#include <mysql/mysql.h>

class DBMySql
{
public:
    DBMySql();
	DBMySql(const std::string& host, const std::string& user, const std::string& passwd, const std::string& db, short port = 3306);
	~DBMySql();

    int             Initialize();
    int             Initialize(const std::string& host, const std::string& user, const std::string& passwd, const std::string& db, short port = 3306);
    //Ping
    void            Ping() { mysql_ping(m_mysql); }
public:
    int             Insert(const std::string& str);
    MYSQL_RES*      Query(const std::string& str);
    int             Update(const std::string& str);
    int             Execute(const std::string& str);
    int             Connect();
    void            Destroy();    
    std::string     ErrorMsg(const std::string& func);
    int             GetErrno();
    std::string     GetError();
    int             NumFields(MYSQL_RES* res);
    int             AffectedRows(MYSQL *mysql);
    bool            UpdateRecord(std::string const&);	
    //数据库插入后的id
    int             GetInsertId();
        
private:
    std::string m_host;
    std::string m_user;
    std::string m_passwd;
    std::string m_db;    
    short 		m_port;

private:
	MYSQL* m_mysql;
};

//全局变量
extern DBMySql* g_pDbMySql;
#define Database()          (g_pDbMySql)

#endif // __DBMYSQL_H__
