#ifndef INCLUDE_BOT_WIDGET_GROUP
#define INCLUDE_BOT_WIDGET_GROUP

#include <vector>
#include <memory>
#include "widget/bot_widget.h"

namespace bot {

struct InputEvent;

class WidgetGroup {
public:
    WidgetGroup()
        : m_hoverWidgetIdx(-1)
        , m_focusWidgetIdx(-1)
        , m_visible(true)
    {}

    virtual ~WidgetGroup()
    {}

    bool init(int widgetCount);

    int getWidgetCount() const
    {
        return static_cast<int>(m_widgets.size());
    }

    Widget* getWidget(unsigned int idx);

    bool setWidget(unsigned int idx, Widget* widget);

    int processInput(const InputEvent& event);

    void present();

    void shiftPos(float dx, float dy);

    bool isVisible() const
    {
        return m_visible;
    }

    void setVisible(bool visible)
    {
        m_visible = visible;
    }

protected:
    int processKeyEvent(const KeyEvent& event);

    int processMouseMoveEvent(const MouseMoveEvent& event);

    int processMouseButtonEvent(const MouseButtonEvent& event);

    int findWidget(float x, float y);

protected:
    std::vector<std::shared_ptr<Widget>> m_widgets;
    int m_hoverWidgetIdx;
    int m_focusWidgetIdx;
    bool m_visible;
};

} // end of namespace bot

#endif
