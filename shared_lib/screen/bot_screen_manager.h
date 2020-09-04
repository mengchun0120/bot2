#ifndef INCLUDE_BOT_SCREEN_MANAGER
#define INCLUDE_BOT_SCREEN_MANAGER

#include "misc/bot_constants.h"

namespace bot {

class Screen;
struct InputEvent;
class GameLib;
class Graphics;
class AppConfig;
class ScreenManager;

class ScreenManager {
public:
    enum ScreenType {
        SCREEN_START,
        SCREEN_GAME,
        SCREEN_NONE
    };

    ScreenManager();

    ~ScreenManager();

    void init(const AppConfig* cfg, const GameLib* lib, Graphics* g,
              float viewportWidth, float viewportHeight);

    int update(float delta);

    void present();

    int processInput(const InputEvent& e);

    void switchScreen(ScreenType type);

private:
    const GameLib* m_lib;
    const AppConfig* m_cfg;
    Graphics* m_graphics;
    float m_viewportSize[Constants::NUM_FLOATS_PER_POSITION];
    ScreenType m_curScreenType;
    Screen* m_prevScreen;
    Screen* m_curScreen;
};

} // end of namespace bot

#endif

