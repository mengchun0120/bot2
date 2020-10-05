#include <cstdarg>
#include <ctime>
#include <cstdio>
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#endif
#include "misc/bot_log.h"

namespace bot {

Logger Logger::g_logger;

const Logger::LogLevel Logger::DEFAULT_LOG_LEVEL = Logger::LEVEL_INFO;

const char* Logger::levelString(LogLevel level)
{
    switch(level) {
    case LEVEL_DEBUG:
        return "DEBUG";
    case LEVEL_INFO:
        return "INFO";
    case LEVEL_WARN:
        return "WARN";
    case LEVEL_ERROR:
        return "ERROR";
    }
    return "UNKNOWN";
}

Logger::Logger()
    : m_fp(nullptr)
    , m_minLevel(LEVEL_INFO)
{
}

Logger::~Logger()
{
    if(m_fp != stdout)
    {
        fclose(m_fp);
    }
}

bool Logger::init(const char* logFile, LogLevel minLevel)
{
    m_minLevel = minLevel;

    if (logFile)
    {
        m_fp = fopen(logFile, "w");
        if (!m_fp)
        {
            fprintf(stderr, "Failed to open %s", logFile);
            return false;
        }
    }
    else
    {
        m_fp = stdout;
    }

    return true;
}

void Logger::write(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(m_fp, format, args);
    va_end(args);
}

void Logger::logTime()
{
#ifdef _WIN32
    SYSTEMTIME t;
    GetLocalTime(&t);
    write("%04d-%02d-%02d %02d:%02d:%02d.%03d",
          t.wYear, t.wMonth, t.wDay, t.wHour,
          t.wMinute, t.wSecond, t.wMilliseconds);
#elif __linux__
    struct timespec tp;
    if (clock_gettime(CLOCK_REALTIME, &tp) == -1)
    {
        return;
    }

    struct tm t;
    if (localtime_r(&tp.tv_sec, &t) == NULL)
    {
        return;
    }

    int ms = tp.tv_nsec / 1e6;
    write("%04d-%02d-%02d %02d:%02d:%02d.%03d",
          (t.tm_year + 1900), t.tm_mon, t.tm_mday, t.tm_hour,
          t.tm_min, t.tm_sec, ms);
#endif
}

void Logger::flush()
{
    fflush(m_fp);
}

} // end of namespace bump
