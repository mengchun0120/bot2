#ifndef INCLUDE_BOT_APP
#define INCLUDE_BOT_APP

#include <string>
#include <memory>
#include "misc/bot_constants.h"
#include "input/bot_input_manager.h"
#include "gameutil/bot_time_delta_smoother.h"
#include "screen/bot_screen.h"

struct GLFWwindow;

namespace bot {

class App {
public:
    static bool initInstance(Screen::Type startScreenType);

    static App &getInstance()
    {
        return *k_app;
    }

    virtual ~App();

    bool init(Screen::Type startScreenType);

    bool run();

    GLFWwindow *getWindow()
    {
        return m_window;
    }

    float getViewportWidth() const
    {
        return m_viewportSize[0];
    }

    float getViewportHeight() const
    {
        return m_viewportSize[1];
    }

    const float *getViewportSize() const
    {
        return m_viewportSize;
    }

private:
    App();

    bool initWindow();

    bool initInputManager();

    bool initOpenGL();

    void updateViewport();

    bool initGame(Screen::Type startScreenType);

    bool initTimeDeltaSmoother();

    bool initGameLib();

private:
    static std::shared_ptr<App> k_app;
    GLFWwindow *m_window;
    float m_viewportSize[Constants::NUM_FLOATS_PER_POSITION];
    InputManager m_inputMgr;
    TimeDeltaSmoother m_timeDeltaSmoother;
    InputProcessor m_inputProcessor;
};

} // end of namespace bot

#endif

