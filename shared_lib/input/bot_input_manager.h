#ifndef INCLUDE_BOT_INPUT_MANAGER
#define INCLUDE_BOT_INPUT_MANAGER

#include "structure/bot_queue.h"
#include "opengl/bot_opengl.h"
#include "input/bot_input_event.h"
#include "input/bot_input_processor.h"

namespace bot {

class InputManager {
public:
    InputManager();

    virtual ~InputManager();

    void init(GLFWwindow* window, int eventQueueSize, float viewPortHeight);

    void start();

    void stop();

    void clear();

    void addMouseButtonEvent(float x, float y, int button, int action);

    void addMouseMoveEvent(float x, float y);

    void addKeyEvent(int key, int action);

    // Returns true if app should continue; false if app should exit
    bool processInput(InputProcessor& processor);

protected:
    GLFWwindow *m_window;
    Queue<InputEvent> m_inputEvents;
    float m_viewportHeight;
};

} // end of namespace bot

#endif
