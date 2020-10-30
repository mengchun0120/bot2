#ifndef INCLUDE_BOT_WIDGET
#define INCLUDE_BOT_WIDGET

#include "misc/bot_constants.h"
#include "geometry/bot_rectangle.h"

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
              const Texture* texture, const Color* borderColor,
              const Color* backColor);

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

    void setTexture(const Texture* texture)
    {
        m_texture = texture;
    }

    void setBorderColor(const Color* color)
    {
        m_borderColor = color;
    }

    void setBackColor(const Color* color)
    {
        m_backColor = color;
    }

    virtual void present();

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

    void setAccetpInput(bool accept)
    {
        m_acceptInput = accept;
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
    float m_pos[Constants::NUM_FLOATS_PER_POSITION];
    Rectangle m_rect;
    const Texture* m_texture;
    const Color* m_borderColor;
    const Color* m_backColor;
    float m_left, m_right, m_top, m_bottom;
    bool m_visible;
    bool m_acceptInput;
};

} // end of namespace bot

#endif
