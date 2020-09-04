#include "misc/bot_log.h"
#include "screen/bot_start_screen.h"
#include "screen/bot_game_screen.h"

namespace bot {

Screen* Screen::create(Type type, const AppConfig* cfg, const GameLib* lib, Graphics* g,
                       ScreenManager* screenMgr, float viewportWidth, float viewportHeight)
{
    Screen* screen = nullptr;
    switch(type)
    {
        case SCREEN_START:
        {
            LOG_INFO("Show start screen");

            StartScreen* startScreen = new StartScreen();
            if (startScreen->init(lib, viewportWidth, viewportHeight, screenMgr, g))
            {
                screen = startScreen;
            }
            else
            {
                delete startScreen;
            }

            LOG_INFO("Done creating start screen");

            break;
        }
        case SCREEN_GAME:
        {
            LOG_INFO("Show game screen");

            GameScreen* gameScreen = new GameScreen();
            if (gameScreen->init(*cfg, lib, g, screenMgr, viewportWidth, viewportHeight))
            {
                screen = gameScreen;
            }
            else
            {
                delete gameScreen;
            }

            LOG_INFO("Done loading game screen");

            break;
        }
        default:
        {
            LOG_ERROR("Invalid screen: %d", static_cast<int>(type));
        }
    }

    return screen;
}

} // end of namespace bot

