#include "misc/bot_log.h"
#include "widget/bot_widget.h"

namespace bot {

Widget::Widget()
    : m_width(0.0f)
    , m_height(0.0f)
    , m_left(0.0f)
    , m_right(0.0f)
    , m_top(0.0f)
    , m_bottom(0.0f)
    , m_visible(true)
    , m_acceptInput(false)
{
}

bool Widget::init(float x, float y, float width, float height,
                  bool visible, bool acceptInput)
{
    if (width <= 0.0f)
    {
        LOG_ERROR("Invalid width %f", width);
        return false;
    }

    if (height <= 0.0f)
    {
        LOG_ERROR("Invalid height %f", height);
        return false;
    }

    m_width = width;
    m_height = height;
    initPos(x, y);
    m_visible = visible;
    m_acceptInput = acceptInput;

    return true;
}

void Widget::setPos(float x, float y)
{
    initPos(x, y);
}

void Widget::shiftPos(float dx, float dy)
{
    m_left += dx;
    m_bottom += dy;
    m_right += dx;
    m_top += dy;
}

bool Widget::containPos(float x, float y) const
{
    if (!m_visible)
    {
        return false;
    }
    return m_left <= x && x <= m_right && m_bottom <= y && y <= m_top;
}

void Widget::initPos(float x, float y)
{
    m_left = x;
    m_bottom = y;
    m_right = x + m_width;
    m_top = y + m_height;
}

} // end of namespace bot

