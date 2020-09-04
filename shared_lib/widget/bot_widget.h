#ifndef INCLUDE_BOT_WIDGET
#define INCLUDE_BOT_WIDGET

#include "misc/bot_constants.h"

namespace bot {

struct KeyEvent;
struct MouseMoveEvent;
struct MouseButtonEvent;
class Rectangle;
class Graphics;

class Widget {
public:
    Widget();

    virtual ~Widget()
    {}

    void init(const Rectangle* rect);

    virtual int processKeyEvent(const KeyEvent& event) = 0;

    virtual int processMouseMoveEvent(const MouseMoveEvent& event) = 0;

    virtual int processMouseButtonEvent(const MouseButtonEvent& event) = 0;

    virtual void setPos(float x, float y);

    virtual void present(Graphics& g) = 0;

    virtual void onLostFocus() = 0;

    virtual void onMouseOut() = 0;

    bool isVisible() const
    {
        return m_visible;
    }

    void setVisible(bool visible)
    {
        m_visible = visible;
    }

    bool containPos(float x, float y) const;

protected:
    float m_pos[Constants::NUM_FLOATS_PER_POSITION];
    const Rectangle* m_rect;
    float m_left, m_right, m_top, m_bottom;
    bool m_visible;
};

} // end of namespace bot

#endif
