#include <cstdlib>
#include <cstring>
#include <iostream>
#include "misc/bot_log.h"
#include "misc/bot_config.h"
#include "app/bot_app.h"

void showUsageAndExit() 
{
    std::cerr << "Usage: bot -l logFile -c configFile -r appFolder [-v verbosity(0~5)]" << std::endl;
    exit(1);
}

int main(int argc, char* argv[])
{
    std::string logFile;
    std::string appDir;
    std::string cfgFile;
    bot::Logger::LogLevel verbosity = bot::Logger::DEFAULT_LOG_LEVEL;

    int i = 1;
    while (i < argc)
    {
        if (strcmp(argv[i], "-l") == 0) 
        {
            if (i + 1 >= argc)
            {
                showUsageAndExit();
            }

            logFile = argv[i + 1];
            i += 2;
        } 
        else if (strcmp(argv[i], "-c") == 0) 
        {
            if (i + 1 >= argc)
            {
                showUsageAndExit();
            }

            cfgFile = argv[i + 1];
            i += 2;
        }
        else if (strcmp(argv[i], "-r") == 0) 
        {
            if (i + 1 >= argc)
            {
                showUsageAndExit();
            }

            appDir = argv[i + 1];
            i += 2;
        }
        else if (strcmp(argv[i], "-v") == 0)
        {
            if (i + 1 >= argc)
            {
                showUsageAndExit();
            }

            int level = atoi(argv[i + 1]);
            if (level < bot::Logger::LEVEL_DEBUG || level > bot::Logger::LEVEL_ERROR)
            {
                showUsageAndExit();
            }

            verbosity = static_cast<bot::Logger::LogLevel>(level);
            i += 2;
        }
        else
        {
            showUsageAndExit();
        }
    }

    if (appDir.empty() || cfgFile.empty()) 
    {
        showUsageAndExit();
    }

#ifdef ENABLE_LOG
    const char* log = logFile.empty() ? nullptr : logFile.c_str();
    if (!bot::Logger::g_logger.init(log, verbosity)) 
    {
        exit(1);
    }
#endif

    bot::App app;
    if (!app.init(appDir, cfgFile)) 
    {
        exit(1);
    }

    bool ret = app.run();

    exit(ret ? 0 : 1);
}