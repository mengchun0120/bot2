#include "geometry/bot_rectangle.h"
#include "widget/bot_widget.h"

namespace bot {

Widget::Widget()
    : m_rect(nullptr)
    , m_visible(true)
{
    m_pos[0] = 0.0f;
    m_pos[1] = 0.0f;
    m_left = 0.0f;
    m_right = 0.0f;
    m_top = 0.0f;
    m_bottom = 0.0f;
}

void Widget::init(const Rectangle* rect)
{
    m_rect = rect;
}

void Widget::setPos(float x, float y)
{
    m_pos[0] = x + m_rect->width() / 2.0f;
    m_pos[1] = y - m_rect->height() / 2.0f;
    m_left = x;
    m_right = x + m_rect->width();
    m_top = y;
    m_bottom = y - m_rect->height();
}

bool Widget::containPos(float x, float y) const
{
    if (!m_visible)
    {
        return false;
    }
    return m_left <= x && x <= m_right && m_bottom <= y && y <= m_top;
}

} // end of namespace bot
