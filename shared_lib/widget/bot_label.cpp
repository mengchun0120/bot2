#include "misc/bot_log.h"
#include "opengl/bot_text_system.h"
#include "opengl/bot_color.h"
#include "widget/bot_label.h"

namespace bot {

Label::Label()
    : m_size(TEXT_SIZE_BIG)
    , m_hAlign(ALIGN_HLEFT)
    , m_vAlign(ALIGN_VTOP)
    , m_textColor(nullptr)
{
}

bool Label::init(float x, float y, float width, float height,
                 const std::string& text, const Color* textColor,
                 const Color* backColor, const Color* borderColor,
                 Align hAlign, Align vAlign, TextSize sz, bool acceptInput)
{
    if (!isValidTextSize(sz))
    {
        LOG_ERROR("Invalid text-size %d", static_cast<int>(sz));
        return false;
    }

    bool ret = Widget::init(x, y, width, height, nullptr, borderColor,
                            backColor, acceptInput);
    if (!ret)
    {
        return false;
    }

    if (!isValidHAlign(hAlign))
    {
        LOG_ERROR("Invalid vertical align %d", static_cast<int>(hAlign));
        return false;
    }

    if (!isValidVAlign(vAlign))
    {
        LOG_ERROR("Invalid horizontal align %d", static_cast<int>(vAlign));
        return false;
    }

    m_textColor = textColor;
    m_size = sz;
    m_hAlign = hAlign;
    m_vAlign = vAlign;
    setText(text);

    return true;
}

void Label::setText(const std::string& text)
{
    m_text = text;
    resetTextPos();
}

void Label::setPos(float x, float y)
{
    Widget::setPos(x, y);
    resetTextPos();
}

void Label::shiftPos(float dx, float dy)
{
    Widget::shiftPos(dx, dy);
    m_textPos[0] += dx;
    m_textPos[1] += dy;
}

void Label::present()
{
    Widget::present();

    if (!m_text.empty())
    {
        const TextSystem& textSys = TextSystem::getInstance();
        textSys.drawString(m_text, m_size, m_textPos, m_textColor->getColor());
    }
}

void Label::resetTextPos()
{
    if (m_text.empty())
    {
        return;
    }

    float textWidth, textHeight;
    const TextSystem& textSys = TextSystem::getInstance();

    textSys.getStringSize(textWidth, textHeight, m_size, m_text);

    switch(m_hAlign)
    {
        case ALIGN_HLEFT:
        {
            m_textPos[0] = m_left;
            break;
        }
        case ALIGN_HRIGHT:
        {
            m_textPos[0] = m_right - textWidth;
            break;
        }
        case ALIGN_HMIDDLE:
        {
            m_textPos[0] = m_pos[0] - textWidth / 2.0f;
            break;
        }
        default:
            LOG_ERROR("Wrong value for hAlign");
    }

    switch(m_vAlign)
    {
        case ALIGN_VTOP:
        {
            m_textPos[1] = m_top - textHeight;
            break;
        }
        case ALIGN_VBOTTOM:
        {
            m_textPos[1] = m_bottom;
            break;
        }
        case ALIGN_VMIDDLE:
        {
            m_textPos[1] = m_pos[1] - textHeight / 2.0f;
            break;
        }
        default:
            LOG_ERROR("Wrong value for vAlign");
    }
}

} // end of namespace bot

