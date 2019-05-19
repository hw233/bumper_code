////////////////////////////////////////////////////////////////
//文件名称：logger.h
//功能描述：多级别打印，按天自动切割日志
//          可打印文件名，行号信息
//版本说明：适用linux操作系统
//
//编写作者：2017.09.01 sunbin
//
////////////////////////////////////////////////////////////////

#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <mutex>
#include <iostream>
#include <sstream>
#include <fstream>

#define LOG(str) \
    Logger::Instance().Log(INFO, __FILE__, __LINE__, str)

//!alter by huyf 2017.10.17
//#define LOGDEBUG(str) 
//    Logger::Instance().Log(DEBUG, __FILE__, __LINE__, str)
//!alter end

#define LOGINFO(str) \
    Logger::Instance().Log(INFO, __FILE__, __LINE__, str)

//!alter by huyf 2017.10.17
#define LOGWARN(str) \
    Logger::Instance().Log(WARN, __FILE__, __LINE__, str)

//#define LOGERROR(str) 
//    Logger::Instance().Log(ERROR, __FILE__, __LINE__, str)
//!alter end

#define LOGCRITICAL(str) \
    Logger::Instance().Log(CRITICAL, __FILE__, __LINE__, str)

#define LOGFATAL(str) \
    Logger::Instance().Log(FATAL, __FILE__, __LINE__, str)

#define LOG_DEBUG(str, ...) \
    Logger::Instance().Log(LOG_LEVEL::DEBUG, __FILE__, __LINE__, str, __VA_ARGS__)

#define LOG_INFO(str, ...) \
    Logger::Instance().Log(LOG_LEVEL::INFO, __FILE__, __LINE__, str, __VA_ARGS__)

#define LOG_WARN(str, ...) \
    Logger::Instance().Log(LOG_LEVEL::WARN, __FILE__, __LINE__, str, __VA_ARGS__)

#define LOG_ERROR(str, ...) \
    Logger::Instance().Log(LOG_LEVEL::ERROR, __FILE__, __LINE__, str, __VA_ARGS__)

#define LOG_CRITICAL(str, ...) \
    Logger::Instance().Log(LOG_LEVEL::CRITICAL, __FILE__, __LINE__, str, __VA_ARGS__)

#define LOG_FATAL(str, ...) \
    Logger::Instance().Log(LOG_LEVEL::FATAL, __FILE__, __LINE__, str, __VA_ARGS__)



enum LOG_LEVEL
{
    DEBUG=1, //1
    INFO,     //2
    WARN,   //3
    ERROR,   //4
    CRITICAL,
    FATAL,
    //add by huyf 2017.10.16
    ASSERT,
    CHECK,
    CHECKF,
    IF_NOT,
    IF_NOT_,
    IF_OK,
    IF_OK_,
    //add end
    DISABLE = 100
};

class Logger{
    private:
        Logger():
            m_toConsole(true),
            m_toFile(true),
            m_logName("log"),
            m_level(LOG_LEVEL::INFO){
            };
        Logger(Logger &logger);
        Logger& operator=(Logger &logger);
        ~Logger(){
            if (m_file.is_open()){
                m_file.flush();
                m_file.close();
            }
        };

    public:
        static Logger& Instance();

        void TimerFlush();

        void Init(std::string logName, LOG_LEVEL level, bool toFile=true);

        void Log(LOG_LEVEL level, std::string filename, int lineno, const char *fmt, ...);

        void SetLogName(std::string logName){
            m_logName = logName;
        }

        void SetLevel(LOG_LEVEL level){
            m_level = level;
        }

        void SetToFile(bool toFile){
            m_toFile = toFile;
        }

        void Flush(){
            std::lock_guard<std::mutex> mtx_locker(m_mtx);
            if (m_file.is_open()){
                m_file.flush();
            }
        }

    private:
        std::string getTime()
        {
            time_t timep;
            time (&timep);
            char tmp[64];
            strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep) );
            return tmp;
        }

        std::string format_head(LOG_LEVEL level, std::string filename, int lineno){
            int iIndex = filename.rfind("/");
            std::string shortFileName = filename.substr(++iIndex);
            std::ostringstream   ostr;
            ostr << shortLevel(level) <<  getTime() << " " << shortFileName << ":" << lineno << "] ";
            return ostr.str();
        }

        std::string shortLevel(LOG_LEVEL level);

        void write(std::string& strlog);

        std::string getFileName(){
            time_t timep;
            time (&timep);
            char tmp[64];
            strftime(tmp, sizeof(tmp), "%Y%m%d",localtime(&timep) );
            std::ostringstream   ostr;
            ostr << m_logName << "-" << tmp << ".log";
            return ostr.str();
        }

    private:
        bool m_toConsole;
        bool m_toFile;
        std::string m_logName;
        LOG_LEVEL m_level;
        std::ofstream m_file;
        std::string m_fileName;
        std::mutex m_mtx;
        bool m_flushTimer;
};

#endif /* LOGGER_H_ */

