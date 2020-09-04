#include "misc/bot_log.h"
#include "screen/bot_screen.h"
#include "screen/bot_screen_manager.h"

namespace bot {

ScreenManager::ScreenManager()
    : m_lib(nullptr)
    , m_cfg(nullptr)
    , m_graphics(nullptr)
    , m_curScreenType(Screen::SCREEN_NONE)
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

bool ScreenManager::init(const AppConfig* cfg, const GameLib* lib, Graphics* g,
                         Screen::Type startScreenType, float viewportWidth, float viewportHeight)
{
    m_lib = lib;
    m_cfg = cfg;
    m_graphics = g;
    m_viewportSize[0] = viewportWidth;
    m_viewportSize[1] = viewportHeight;

    Screen* screen = Screen::create(startScreenType, cfg, lib, g, this, viewportWidth, viewportHeight);
    if (!screen)
    {
        LOG_ERROR("Failed to create start screen");
        return false;
    }

    m_curScreen = screen;
    m_curScreenType = startScreenType;

    return true;
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

bool ScreenManager::switchScreen(Screen::Type type)
{
    LOG_INFO("Switching screen from %d to %d", static_cast<int>(m_curScreenType), static_cast<int>(type));

    if (m_curScreenType == type)
    {
        return true;
    }

    Screen* screen = Screen::create(type, m_cfg, m_lib, m_graphics, this, m_viewportSize[0], m_viewportSize[1]);
    if (!screen)
    {
        LOG_ERROR("Failed to swtich screen");
        return false;
    }

    m_prevScreen = m_curScreen;
    m_curScreen = screen;
    m_curScreenType = type;

    LOG_INFO("Done switching screen");

    return true;
}

} // end of namespace bot

