#ifndef INCLUDE_BOT_WIDGET
#define INCLUDE_BOT_WIDGET

#include "misc/bot_constants.h"

namespace bot {

struct KeyEvent;
struct MouseMoveEvent;
struct MouseButtonEvent;

class Widget {
public:
    Widget();

    virtual ~Widget()
    {}

    bool init(float x, float y, float width, float height,
              bool visible=true, bool acceptInput=false);

    virtual int processKeyEvent(const KeyEvent& event)
    {
        return 0;
    }

    virtual int processMouseMoveEvent(const MouseMoveEvent& event)
    {
        return 0;
    }

    virtual int processMouseButtonEvent(const MouseButtonEvent& event)
    {
        return 0;
    }

    virtual void setPos(float x, float y);

    virtual void shiftPos(float dx, float dy);

    virtual void present() = 0;

    virtual void onLostFocus()
    {}

    virtual void onMouseOut()
    {}

    bool visible() const
    {
        return m_visible;
    }

    void setVisible(bool visible)
    {
        m_visible = visible;
    }

    bool containPos(float x, float y) const;

    bool acceptInput() const
    {
        return m_acceptInput;
    }

    void setAcceptInput(bool accept)
    {
        m_acceptInput = accept;
    }

    float getWidth() const
    {
        return m_width;
    }

    float getHeight() const
    {
        return m_height;
    }

    float getLeft() const
    {
        return m_left;
    }

    float getBottom() const
    {
        return m_bottom;
    }

    float getRight() const
    {
        return m_right;
    }

    float getTop() const
    {
        return m_top;
    }

protected:
    void initPos(float x, float y);

protected:
    float m_width, m_height;
    float m_left, m_right, m_top, m_bottom;
    bool m_visible;
    bool m_acceptInput;
};

} // end of namespace bot

#endif
