#ifndef INCLUDE_BOT_APP
#define INCLUDE_BOT_APP

#include <string>
#include <memory>
#include "input/bot_input_manager.h"
#include "opengl/bot_graphics.h"
#include "gameutil/bot_time_delta_smoother.h"
#include "gameutil/bot_game_lib.h"
#include "screen/bot_screen_manager.h"

struct GLFWwindow;

namespace bot {

class AppConfig;

class App {
public:
    App();

    virtual ~App();

    bool init(const AppConfig* cfg, Screen::Type startScreenType);

    bool run();

protected:
    bool initWindow();

    bool initInputManager();

    bool initOpenGL();

    void updateViewport();

    bool initGame(Screen::Type startScreenType);

    bool initTimeDeltaSmoother();

    bool initGameLib();

protected:
    GLFWwindow* m_window;
    const AppConfig* m_config;
    float m_viewportSize[Constants::NUM_FLOATS_PER_POSITION];
    InputManager m_inputMgr;
    Graphics m_graphics;
    TimeDeltaSmoother m_timeDeltaSmoother;
    GameLib m_gameLib;
    InputProcessor m_inputProcessor;
    ScreenManager m_screenMgr;
};

} // end of namespace bot

#endif

