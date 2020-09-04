#include "misc/bot_log.h"
#include "app/bot_app.h"
#include "input/bot_input_manager.h"

namespace bot {

InputManager* k_inputMgr = nullptr;

void handleMouseMove(GLFWwindow *window, double x, double y)
{
    if (k_inputMgr)
    {
        k_inputMgr->addMouseMoveEvent(static_cast<float>(x), static_cast<float>(y));
    }
}

void handleMouseButton(GLFWwindow *window, int button, int action, int mods)
{
    if (k_inputMgr)
    {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        k_inputMgr->addMouseButtonEvent(static_cast<float>(x), static_cast<float>(y), button, action);
    }
}

void handleKey(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (k_inputMgr)
    {
        k_inputMgr->addKeyEvent(key, action);
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

void InputManager::init(GLFWwindow* window, int eventQueueSize, float viewportHeight)
{
    m_window = window;
    m_inputEvents.init(eventQueueSize);
    m_viewportHeight = viewportHeight;
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

void InputManager::addMouseButtonEvent(float x, float y, int button, int action)
{
    if (m_inputEvents.full())
    {
        LOG_WARN("Input events queue is full");
        return;
    }

    InputEvent e;
    e.m_type = InputEvent::ET_MOUSE_BUTTON;
    e.m_mouseButtonEvent.m_x = x;
    e.m_mouseButtonEvent.m_y = m_viewportHeight - y;
    e.m_mouseButtonEvent.m_button = button;
    e.m_mouseButtonEvent.m_action = action;
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

void InputManager::addKeyEvent(int key, int action)
{
    if (m_inputEvents.full())
    {
        LOG_WARN("Input events queue is full");
        return;
    }

    InputEvent e;
    e.m_type = InputEvent::ET_KEY;
    e.m_keyEvent.m_key = key;
    e.m_keyEvent.m_action = action;
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

