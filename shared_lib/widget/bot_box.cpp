#include "misc/bot_log.h"
#include "opengl/bot_color.h"
#include "opengl/bot_texture.h"
#include "geometry/bot_rectangle.h"
#include "widget/bot_box.h"

namespace bot {

Box::Box()
    : Widget()
    , m_texture(nullptr)
    , m_backColor(nullptr)
    , m_borderColor(nullptr)
{
    m_rectPos[0] = 0.0f;
    m_rectPos[1] = 0.0f;
}

bool Box::init(float x, float y, float width, float height,
               const Rectangle* rect, const Texture* texture,
               const Color* backColor, const Color* borderColor,
               bool visible, bool acceptInput)
{
    if (!Widget::init(x, y, width, height, visible, acceptInput))
    {
        return false;
    }

    if (!rect)
    {
        LOG_ERROR("rect is null");
        return false;
    }

    m_rect = rect;
    m_texture = texture;
    m_backColor = backColor;
    m_borderColor = borderColor;
    resetRectPos();

    return true;
}

void Box::present()
{
    if (!m_visible)
    {
        return;
    }

    if (m_backColor || m_borderColor)
    {
        m_rect->draw(m_rectPos, nullptr, m_backColor, m_borderColor,
                     *m_texture, nullptr);
    }

    if (m_texture)
    {
        m_rect->draw(m_rectPos, nullptr, nullptr, nullptr, *m_texture, nullptr);
    }
}

void Box::setPos(float x, float y)
{
    Widget::setPos(x, y);
    resetRectPos();
}

void Box::shiftPos(float dx, float dy)
{
    Widget::shiftPos(dx, dy);
    m_rectPos[0] += dx;
    m_rectPos[1] += dy;
}

void Box::resetRectPos()
{
    m_rectPos[0] = getLeft() + m_rect->width() / 2.0f;
    m_rectPos[1] = getBottom() + m_rect->height() / 2.0f;
}

} // end of namespace bot

