#include "misc/bot_log.h"
#include "screen/bot_start_screen.h"
#include "screen/bot_game_screen.h"
#include "screen/bot_show_map_screen.h"

namespace bot {

Screen* Screen::create(Type type)
{
    Screen* screen = nullptr;
    switch(type)
    {
        case SCREEN_START:
        {
            LOG_INFO("Show start screen");

            StartScreen* startScreen = new StartScreen();
            if (startScreen->init())
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
            if (gameScreen->init())
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
        case SCREEN_SHOW_MAP:
        {
            LOG_INFO("Show map screen");

            ShowMapScreen* showMapScreen = new ShowMapScreen();
            if (showMapScreen->init())
            {
                screen = showMapScreen;
            }
            else
            {
                delete showMapScreen;
            }

            LOG_INFO("Done loading show-map screen");

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

