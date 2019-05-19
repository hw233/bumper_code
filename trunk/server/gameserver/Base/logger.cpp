#include "logger.h"

#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <cstdarg>
#include <memory>
#include <thread>

Logger& Logger::Instance(){
    static Logger logger;
    return logger;
}

void Logger::TimerFlush(){
    while(true){
        sleep(1);
        Flush();
    }
}

void Logger::Init(std::string logName, LOG_LEVEL level, bool toFile){
    SetLogName(logName);
    SetLevel(level);
    SetToFile(toFile);
    if(!m_flushTimer){
        m_flushTimer = true;
        std::thread thrd_flush(&Logger::TimerFlush, this);
        thrd_flush.detach();
    }
}

void Logger::Log(LOG_LEVEL level, std::string filename, int lineno, const char *fmt, ...){
    if (level < m_level){
        return;
    }
    int old_size = strlen(fmt);
    std::unique_ptr<char[]> buf(new char[old_size]);
    va_list ap;
    va_start(ap, fmt);
    int new_size = vsnprintf(buf.get(), old_size, fmt, ap);
    va_end(ap);
    if (new_size < 0)
        return;
    buf.reset(new char[new_size + 1]);
    va_start(ap, fmt);
    new_size = vsnprintf(buf.get(), new_size + 1, fmt, ap);
    va_end(ap);
    if (new_size < 0)
        return;
    std::string strlog = format_head(level, filename, lineno) + std::string(buf.get());
    write(strlog);
    if (LOG_LEVEL::FATAL == level){
        Flush();
        exit(1);
    }
}

std::string Logger::shortLevel(LOG_LEVEL level){
    std::string shortname;
    switch (level){
        case LOG_LEVEL::DEBUG:
            shortname = "D";
            break;
        case LOG_LEVEL::INFO:
            shortname = "I";
            break;
        case LOG_LEVEL::WARN:
            shortname = "W";
            break;
        case LOG_LEVEL::ERROR:
            shortname = "E";
            break;
        case LOG_LEVEL::CRITICAL:
            shortname = "C";
            break;
        case LOG_LEVEL::FATAL:
            shortname = "F";
            break;
        default:
            shortname = "U";
            break;
    }
    return shortname;
}

void Logger::write(std::string& strlog){
    std::lock_guard<std::mutex> mtx_locker(m_mtx);
    if (m_toConsole){
        std::cout << strlog << std::endl;
    }
    if (m_toFile){
        if (getFileName() == m_fileName && m_file.is_open()){
            m_file << strlog << std::endl;
        }
        else{
            if (m_file.is_open()){
                m_file.close();
            }
            m_fileName = getFileName();
            m_file.open("logs/"+m_fileName, std::ios::app);
            m_file << strlog << std::endl;
        }
    }
}

