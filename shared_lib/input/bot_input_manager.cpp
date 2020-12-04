#include "misc/bot_log.h"
#include "input/bot_input_manager.h"
#include "app/bot_app.h"
#include "app/bot_app_config.h"

namespace bot {

InputManager* k_inputMgr = nullptr;

void handleMouseMove(GLFWwindow *window, double x, double y)
{
    if (k_inputMgr)
    {
        k_inputMgr->addMouseMoveEvent(static_cast<float>(x),
                                      static_cast<float>(y));
    }
}

void handleMouseButton(GLFWwindow *window, int button, int action, int mods)
{
    if (k_inputMgr)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        k_inputMgr->addMouseButtonEvent(static_cast<float>(x),
                                        static_cast<float>(y),
                                        button, action, mods);
    }
}

void handleKey(GLFWwindow *window, int key, int scanCode, int action, int mods)
{
    if (k_inputMgr)
    {
        k_inputMgr->addKeyEvent(key, action, scanCode, mods);
    }
}

InputManager::InputManager()
    : m_window(nullptr)
    , m_viewportHeight(0.0f)
{
}

InputManager::~InputManager()
{
}

void InputManager::init()
{
    App& app = App::getInstance();
    const AppConfig& cfg = AppConfig::getInstance();

    m_window = app.getWindow();
    m_inputEvents.init(cfg.getEventQueueSize());
    m_viewportHeight = app.getViewportHeight();
    k_inputMgr = this;
}

void InputManager::start()
{
    glfwSetCursorPosCallback(m_window, handleMouseMove);
    glfwSetMouseButtonCallback(m_window, handleMouseButton);
    glfwSetKeyCallback(m_window, handleKey);
}

void InputManager::stop()
{
    glfwSetCursorPosCallback(m_window, nullptr);
    glfwSetMouseButtonCallback(m_window, nullptr);
    glfwSetKeyCallback(m_window, nullptr);
}

void InputManager::clear()
{
    m_inputEvents.clear();
}

void InputManager::addMouseButtonEvent(float x, float y, int button,
                                       int action, int mods)
{
    if (m_inputEvents.full())
    {
        LOG_WARN("Input events queue is full");
        return;
    }

    InputEvent e;
    MouseButtonEvent& btnEvent = e.m_mouseButtonEvent;

    e.m_type = InputEvent::ET_MOUSE_BUTTON;
    btnEvent.m_x = x;
    btnEvent.m_y = m_viewportHeight - y;
    btnEvent.m_button = button;
    btnEvent.m_action = action;
    btnEvent.m_mods = mods;

    m_inputEvents.enqueue(e);
}

void InputManager::addMouseMoveEvent(float x, float y)
{
    if (m_inputEvents.full())
    {
        LOG_WARN("Input events queue is full");
        return;
    }

    InputEvent e;

    e.m_type = InputEvent::ET_MOUSE_MOVE;
    e.m_mouseMoveEvent.m_x = x;
    e.m_mouseMoveEvent.m_y = m_viewportHeight - y;

    m_inputEvents.enqueue(e);
}

void InputManager::addKeyEvent(int key, int action, int scanCode, int mods)
{
    if (m_inputEvents.full())
    {
        LOG_WARN("Input events queue is full");
        return;
    }

    InputEvent e;
    KeyEvent& kevt = e.m_keyEvent;

    e.m_type = InputEvent::ET_KEY;
    kevt.m_key = key;
    kevt.m_action = action;
    kevt.m_scanCode = scanCode;
    kevt.m_mods = mods;

    m_inputEvents.enqueue(e);
}

bool InputManager::processInput(InputProcessor &processor)
{
    InputEvent e;
    while (!m_inputEvents.empty())
    {
        m_inputEvents.dequeue(e);
        int ret = processor(e);
        if (ret != 0)
        {
            m_inputEvents.clear();
            return ret == 2 ? false : true;
        }
    }

    return true;
}

} // end of namespace bot

