#ifndef INCLUDE_BOT_SCREEN_MANAGER
#define INCLUDE_BOT_SCREEN_MANAGER

#include <memory>
#include "misc/bot_constants.h"
#include "screen/bot_screen.h"

namespace bot {

class Screen;
struct InputEvent;

class ScreenManager {
public:
    static bool initInstance(Screen::Type startScreenType);

    ScreenManager& getInstance()
    {
        return *k_screenMgr;
    }

    ~ScreenManager();

    int update(float delta);

    void present();

    int processInput(const InputEvent& e);

    bool switchScreen(Screen::Type type);

private:
    ScreenManager();

    bool init(Screen::Type startScreenType);

private:
    static std::shared_ptr<ScreenManager> k_screenMgr;
    Screen::Type m_curScreenType;
    Screen* m_prevScreen;
    Screen* m_curScreen;
};

} // end of namespace bot

#endif

