#include "opengl/bot_opengl.h"
#include "opengl/bot_color.h"
#include "opengl/bot_graphics.h"
#include "input/bot_input_event.h"
#include "widget/bot_button_config.h"
#include "widget/bot_button.h"

namespace bot {

Button::Button()
    : m_cfg(nullptr)
    , m_textColor(nullptr)
{
    m_textPos[0] = 0.0f;
    m_textPos[1] = 0.0f;
}

bool Button::init(const ButtonConfig* cfg, const Rectangle* rect, const std::string& text)
{
    Widget::init(rect);
    m_cfg = cfg;
    m_text = text;

    return true;
}

void Button::setPos(const TextSystem& textSys, float x, float y)
{
    Widget::setPos(x, y);

    float textWidth, textHeight;
    textSys.getStringSize(textWidth, textHeight, TEXT_SIZE_BIG, m_text);

    m_textPos[0] = m_pos[0] - textWidth / 2.0f;
    m_textPos[1] = m_pos[1] - textHeight / 2.0f;
}

int Button::processMouseMoveEvent(const MouseMoveEvent& event)
{
    m_textColor = m_cfg->getHoverTextColor();
    return 0;
}

int Button::processMouseButtonEvent(const MouseButtonEvent& event)
{
    if (event.m_button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (event.m_action == GLFW_PRESS)
        {
            m_textColor = m_cfg->getPressTextColor();
        }
        else if (event.m_action == GLFW_RELEASE && m_actionFunc)
        {
            return m_actionFunc();
        }
    }

    return 0;
}

void Button::onMouseOut()
{
    m_textColor = m_cfg->getNormalTextColor();
}

void Button::present(Graphics& g)
{
    if (!m_visible)
    {
        return;
    }

    m_rect->draw(g, m_pos, nullptr, nullptr, nullptr, m_cfg->getTexture()->textureId(), nullptr);
    g.getTextSystem().drawString(g.getSimpleShader(), m_text, TEXT_SIZE_BIG, m_textPos, m_textColor->getColor());
}

} // end of namespace bot
