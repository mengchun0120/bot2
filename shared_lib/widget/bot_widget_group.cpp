#include "misc/bot_log.h"
#include "opengl/bot_opengl.h"
#include "input/bot_input_event.h"
#include "widget/bot_widget_group.h"

namespace bot {

bool WidgetGroup::init(int widgetCount)
{
    if (widgetCount <= 0)
    {
        LOG_ERROR("Invalid widgetCount %d", widgetCount);
        return false;
    }

    m_widgets.resize(widgetCount);

    return true;
}

int WidgetGroup::processInput(const InputEvent& event)
{
    switch (event.m_type)
    {
        case InputEvent::ET_KEY:
        {
            return processKeyEvent(event.m_keyEvent);
        }
        case InputEvent::ET_MOUSE_MOVE:
        {
            return processMouseMoveEvent(event.m_mouseMoveEvent);
        }
        case InputEvent::ET_MOUSE_BUTTON:
        {
            return processMouseButtonEvent(event.m_mouseButtonEvent);
        }
    }

    LOG_ERROR("Unknown event type %d", static_cast<int>(event.m_type));

    return 0;
}

void WidgetGroup::present()
{
    for (auto& widget : m_widgets)
    {
        widget->present();
    }
}

void WidgetGroup::shiftPos(float dx, float dy)
{
    for (auto& widget : m_widgets)
    {
        widget->shiftPos(dx, dy);
    }
}

int WidgetGroup::processKeyEvent(const KeyEvent& event)
{
    if (m_focusWidgetIdx == -1)
    {
        return 0;
    }

    return m_widgets[m_focusWidgetIdx]->processKeyEvent(event);
}

int WidgetGroup::processMouseMoveEvent(const MouseMoveEvent& event)
{
    int idx = findWidget(event.m_x, event.m_y);

    if (idx != m_hoverWidgetIdx && m_hoverWidgetIdx != -1)
    {
        m_widgets[m_hoverWidgetIdx]->onMouseOut();
    }

    if (idx == -1)
    {
        m_hoverWidgetIdx = -1;
        return 0;
    }

    m_hoverWidgetIdx = idx;

    return m_widgets[m_hoverWidgetIdx]->processMouseMoveEvent(event);
}

int WidgetGroup::processMouseButtonEvent(const MouseButtonEvent& event)
{
    int idx = findWidget(event.m_x, event.m_y);
    bool process = event.m_button == GLFW_MOUSE_BUTTON_LEFT &&
                   event.m_action == GLFW_PRESS;

    if (process)
    {
        if (idx != m_focusWidgetIdx)
        {
            if (m_focusWidgetIdx != -1)
            {
                m_widgets[m_focusWidgetIdx]->onLostFocus();
            }

            m_focusWidgetIdx = idx;
        }
    }

    if (idx == -1)
    {
        return 0;
    }

    return m_widgets[idx]->processMouseButtonEvent(event);
}

int WidgetGroup::findWidget(float x, float y)
{
    int i, count = static_cast<int>(m_widgets.size());

    for (i = 0; i < count; ++i)
    {
        bool found = m_widgets[i]->visible() &&
                     m_widgets[i]->acceptInput() &&
                     m_widgets[i]->containPos(x, y);
        if (found)
        {
            return i;
        }
    }

    return -1;
}

} // end of namespace bot
