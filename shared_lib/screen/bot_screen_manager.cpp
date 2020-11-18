#include "misc/bot_log.h"
#include "screen/bot_screen.h"
#include "screen/bot_screen_manager.h"

namespace bot {

std::shared_ptr<ScreenManager> ScreenManager::k_screenMgr;

bool ScreenManager::initInstance(Screen::Type startScreenType)
{
    ScreenManager* mgr = new ScreenManager();
    k_screenMgr.reset(mgr);

    if (!mgr->init(startScreenType))
    {
        k_screenMgr.reset();
        return false;
    }

    return true;
}

ScreenManager::ScreenManager()
    : m_curScreenType(Screen::SCREEN_NONE)
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

bool ScreenManager::init(Screen::Type startScreenType)
{
    Screen* screen = Screen::create(startScreenType);
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
    LOG_INFO("Switching screen from %d to %d",
             static_cast<int>(m_curScreenType), static_cast<int>(type));

    if (m_curScreenType == type)
    {
        return true;
    }

    Screen* screen = Screen::create(type);
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

