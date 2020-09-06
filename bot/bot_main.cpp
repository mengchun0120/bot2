#include <cstdlib>
#include <cstring>
#include <iostream>
#include "misc/bot_log.h"
#include "app/bot_app_config.h"
#include "app/bot_app.h"

void showUsageAndExit()
{
    std::cerr << "Usage: bot [--mode normal|showmap] --log logFile --cfg configFile --app appFolder "
              << "[--map mapFile] [--mapGenerator generatorName] [-v verbosity(0~5)]" << std::endl;
    exit(1);
}

int main(int argc, char* argv[])
{
    std::string mode, logFile, appDir, cfgFile, mapFile, mapGenerator;
    bot::Logger::LogLevel verbosity = bot::Logger::DEFAULT_LOG_LEVEL;

    int i = 1;
    while (i < argc)
    {
        if (strcmp(argv[i], "--mode") == 0)
        {
            if (i + 1 >= argc)
            {
                showUsageAndExit();
            }

            mode = argv[i + 1];
            i += 2;
        }
        else if (strcmp(argv[i], "--log") == 0)
        {
            if (i + 1 >= argc)
            {
                showUsageAndExit();
            }

            logFile = argv[i + 1];
            i += 2;
        }
        else if (strcmp(argv[i], "--cfg") == 0)
        {
            if (i + 1 >= argc)
            {
                showUsageAndExit();
            }

            cfgFile = argv[i + 1];
            i += 2;
        }
        else if (strcmp(argv[i], "--app") == 0)
        {
            if (i + 1 >= argc)
            {
                showUsageAndExit();
            }

            appDir = argv[i + 1];
            i += 2;
        }
        else if (strcmp(argv[i], "--map") == 0)
        {
            if (i + 1 >= argc)
            {
                showUsageAndExit();
            }

            mapFile = argv[i + 1];
            i += 2;
        }
        else if (strcmp(argv[i], "--mapGenerator") == 0)
        {
            if (i + 1 >= argc)
            {
                showUsageAndExit();
            }

            mapGenerator = argv[i + 1];
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

    bot::Screen::Type screen = bot::Screen::SCREEN_NONE;

    if (mode.empty() || mode == "normal")
    {
        screen = bot::Screen::SCREEN_START;
    }
    else if(mode == "showmap")
    {
        screen = bot::Screen::SCREEN_SHOW_MAP;
    }
    else
    {
        LOG_ERROR("Wrong mode %s", mode.c_str());
        exit(1);
    }

    bot::AppConfig cfg;
    if (!cfg.load(appDir, cfgFile))
    {
        LOG_ERROR("Failed to load config file from %s", cfgFile);
        exit(1);
    }

    if (!mapFile.empty())
    {
        cfg.setMapFile(mapFile);
    }

    LOG_INFO("MapGenerator '%s'", mapGenerator.c_str());
    if (!mapGenerator.empty())
    {
        cfg.setMapGenerator(mapGenerator);
    }

    bot::App app;
    if (!app.init(&cfg, screen))
    {
        exit(1);
    }

    bool ret = app.run();

    exit(ret ? 0 : 1);
}
