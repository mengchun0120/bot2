#ifndef INCLUDE_BOT_LOG
#define INCLUDE_BOT_LOG

#include <stdio.h>

namespace bot {

class Logger {
public:
    enum LogLevel {
        LEVEL_DEBUG,
        LEVEL_INFO,
        LEVEL_WARN,
        LEVEL_ERROR
    };

    static const LogLevel DEFAULT_LOG_LEVEL;

    static Logger g_logger;

    static const char* levelString(LogLevel level);

    Logger();

    ~Logger();

    bool init(const char* logFile, LogLevel minLevel=DEFAULT_LOG_LEVEL);

    void write(const char *format, ...);

    void logTime();

    LogLevel minLevel() const
    {
        return m_minLevel;
    }

    void flush();

    void close();

private:
    FILE *m_fp;
    LogLevel m_minLevel;
};

} // end of namespace bot

#ifdef ENABLE_LOG
#define LOG_WRITE(level, ...) \
    { \
        bot::Logger& logger = bot::Logger::g_logger; \
        if(level >= logger.minLevel()) { \
            logger.logTime(); \
            logger.write(" %s:%d %s ", __FILE__, __LINE__, bot::Logger::levelString(level)); \
            logger.write(__VA_ARGS__); \
            logger.write("\n"); \
            logger.flush(); \
        } \
    }
#else
#define LOG_WRITE(level, ...)
#endif

#define LOG_DEBUG(...) \
    LOG_WRITE(bot::Logger::LEVEL_DEBUG, __VA_ARGS__)

#define LOG_INFO(...) \
    LOG_WRITE(bot::Logger::LEVEL_INFO, __VA_ARGS__)

#define LOG_WARN(...) \
    LOG_WRITE(bot::Logger::LEVEL_WARN, __VA_ARGS__)

#define LOG_ERROR(...) \
    LOG_WRITE(bot::Logger::LEVEL_ERROR, __VA_ARGS__)

#endif
