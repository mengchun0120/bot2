#ifndef INCLUDE_BOT_APP
#define INCLUDE_BOT_APP

#include <string>
#include <memory>
#include "input/bot_input_manager.h"
#include "opengl/bot_graphics.h"
#include "gameutil/bot_time_delta_smoother.h"
#include "gameutil/bot_game_lib.h"
#include "screen/bot_screen_manager.h"
#include "app/bot_app_config.h"

struct GLFWwindow;

namespace bot {

class App {
public:
    App();

    ~App();

    bool init(const std::string& appDir, const std::string& cfgFile);

    bool run();

private:
    bool initWindow();

    bool initInputManager();

    bool initOpenGL();

    void updateViewport();

    bool initGame();

    bool initTimeDeltaSmoother();

    bool initGameLib();

private:
    static std::shared_ptr<App> k_app;
    GLFWwindow* m_window;
    AppConfig m_config;
    float m_viewportSize[Constants::NUM_FLOATS_PER_POSITION];
    InputManager m_inputMgr;
    Graphics m_graphics;
    TimeDeltaSmoother m_timeDeltaSmoother;
    GameLib m_gameLib;
    ScreenManager m_screenMgr;
};

} // end of namespace bot

#endif

