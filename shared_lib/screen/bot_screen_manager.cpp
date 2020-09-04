#include "misc/bot_log.h"
#include "screen/bot_start_screen.h"
#include "screen/bot_game_screen.h"
#include "screen/bot_screen_manager.h"

namespace bot {

ScreenManager::ScreenManager()
    : m_lib(nullptr)
    , m_cfg(nullptr)
    , m_graphics(nullptr)
    , m_curScreenType(SCREEN_NONE)
    , m_prevScreen(nullptr)
    , m_curScreen(nullptr)
{
}

ScreenManager::~ScreenManager()
{
    if (m_prevScreen)
    {
        delete m_prevScreen;

    }

    if (m_curScreen)
    {
        delete m_curScreen;
    }
}

void ScreenManager::init(const AppConfig* cfg, const GameLib* lib, Graphics* g,
                         float viewportWidth, float viewportHeight)
{
    m_lib = lib;
    m_cfg = cfg;
    m_graphics = g;
    m_viewportSize[0] = viewportWidth;
    m_viewportSize[1] = viewportHeight;

    StartScreen* s = new StartScreen();
    s->init(m_lib, viewportWidth, viewportHeight, this, g);
    m_curScreen = s;
    m_curScreenType = SCREEN_START;
}

int ScreenManager::update(float delta)
{
    int ret = m_curScreen->update(delta);
    if (ret == 1)
    {
        // switched to another screen
        delete m_prevScreen;
        m_prevScreen = nullptr;
    }
    return ret;
}

void ScreenManager::present()
{
    m_curScreen->present();
}

int ScreenManager::processInput(const InputEvent& e)
{
    int ret = m_curScreen->processInput(e);
    if (ret == 1)
    {
        // switched to another screen
        delete m_prevScreen;
        m_prevScreen = nullptr;
    }
    return ret;
}

void ScreenManager::switchScreen(ScreenType type)
{
    LOG_INFO("Switching screen from %d to %d", static_cast<int>(m_curScreenType), static_cast<int>(type));

    if (m_curScreenType == type)
    {
        return;
    }

    Screen *screen = nullptr;
    switch (type)
    {
        case SCREEN_START:
        {
            StartScreen* s = new StartScreen();
            s->init(m_lib, m_viewportSize[0], m_viewportSize[1], this, m_graphics);
            screen = s;
            break;
        }
        case SCREEN_GAME:
        {
            LOG_INFO("Show game screen")
            GameScreen* s = new GameScreen();
            s->init(*m_cfg, m_lib, m_graphics, this, m_viewportSize[0], m_viewportSize[1]);
            LOG_INFO("Done loading game screen");
            screen = s;
            break;
        }
        default:
        {
            LOG_ERROR("Invalid screen: %d", static_cast<int>(type));
        }
    }

    m_prevScreen = m_curScreen;
    m_curScreen = screen;
    m_curScreenType = type;

    LOG_INFO("Done switching screen");
}

} // end of namespace bot

