#include "misc/bot_log.h"
#include "geometry/bot_rectangle.h"
#include "widget/bot_widget.h"

namespace bot {

Widget::Widget()
    : m_texture(nullptr)
    , m_borderColor(nullptr)
    , m_backColor(nullptr)
    , m_left(0.0f)
    , m_right(0.0f)
    , m_top(0.0f)
    , m_bottom(0.0f)
    , m_visible(true)
    , m_acceptInput(false)
{
    m_pos[0] = 0.0f;
    m_pos[1] = 0.0f;
}

bool Widget::init(float x, float y, float width, float height,
                  const Texture* texture, const Color* borderColor,
                  const Color* backColor, bool acceptInput)
{
    if (!m_rect.init(width, height))
    {
        LOG_ERROR("Failed to initialize rect");
        return false;
    }

    setPos(x, y);
    setTexture(texture);
    setBorderColor(borderColor);
    setBackColor(backColor);
    m_acceptInput = acceptInput;

    return true;
}

void Widget::setPos(float x, float y)
{
    m_pos[0] = x + m_rect.width() / 2.0f;
    m_pos[1] = y + m_rect.height() / 2.0f;
    m_left = x;
    m_bottom = y;
    m_right = x + m_rect.width();
    m_top = y + m_rect.height();
}

void Widget::shiftPos(float dx, float dy)
{
    m_pos[0] += dx;
    m_pos[1] += dy;
    m_left += dx;
    m_bottom += dy;
    m_right += dx;
    m_top += dy;
}

void Widget::present()
{
    if (m_texture)
    {
        m_rect.draw(m_pos, nullptr, nullptr, nullptr,
                    m_texture->textureId(), nullptr);
    }
    else if (m_borderColor || m_backColor)
    {
        m_rect.draw(m_pos, nullptr, m_backColor, m_borderColor,
                    0, nullptr);
    }
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

