#ifndef INCLUDE_BOT_WIDGET_GROUP
#define INCLUDE_BOT_WIDGET_GROUP

#include <vector>
#include <memory>
#include "widget/bot_widget.h"

namespace bot {

struct InputEvent;
class Graphics;

class WidgetGroup {
public:
    WidgetGroup()
        : m_hoverWidgetIdx(-1)
        , m_focusWidgetIdx(-1)
    {}

    virtual ~WidgetGroup()
    {}

    void init(int widgetCount)
    {
        m_widgets.resize(widgetCount);
    }

    int getWidgetCount() const
    {
        return static_cast<int>(m_widgets.size());
    }

    Widget& getWidget(int idx)
    {
        return *m_widgets[idx];
    }

    void setWidget(int idx, Widget* widget)
    {
        m_widgets[idx].reset(widget);
    }

    int processInput(const InputEvent& event);

    void present(Graphics& g);

private:
    int processKeyEvent(const KeyEvent& event);

    int processMouseMoveEvent(const MouseMoveEvent& event);

    int processMouseButtonEvent(const MouseButtonEvent& event);

    int findWidget(float x, float y);

private:
    std::vector<std::shared_ptr<Widget>> m_widgets;
    int m_hoverWidgetIdx;
    int m_focusWidgetIdx;
};

} // end of namespace bot

#endif
