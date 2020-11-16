#include "opengl/bot_opengl.h"
#include "opengl/bot_color.h"
#include "opengl/bot_text_system.h"
#include "input/bot_input_event.h"
#include "widget/bot_button.h"
#include "gameutil/bot_game_lib.h"

namespace bot {

Button::Button()
    : m_textColor(nullptr)
{
    m_textPos[0] = 0.0f;
    m_textPos[1] = 0.0f;
}

bool Button::init(float x, float y, float width, float height,
                  const std::string& text, bool acceptInput)
{
    const ButtonConfig& cfg = GameLib::getInstance().getButtonConfig();

    bool ret = Widget::init(x, y, width, height, cfg.getTexture(),
                            nullptr, nullptr, acceptInput);
    if (!ret)
    {
        return false;
    }

    m_textColor = cfg.getNormalTextColor();
    setText(text);

    return true;
}

void Button::setText(const std::string& text)
{
    m_text = text;

    float w, h;
    const TextSystem& textSys = TextSystem::getInstance();

    textSys.getStringSize(w, h, TEXT_SIZE_BIG, m_text);
    m_textPos[0] = m_pos[0] - w / 2.0f;
    m_textPos[1] = m_pos[1] - h / 2.0f;
}

void Button::setPos(float x, float y)
{
    float oldX = m_pos[0], oldY = m_pos[1];

    Widget::setPos(x, y);

    m_textPos[0] += m_pos[0] - oldX;
    m_textPos[1] += m_pos[1] - oldY;
}

void Button::shiftPos(float dx, float dy)
{
    Widget::shiftPos(dx, dy);
    m_textPos[0] += dx;
    m_textPos[1] += dy;
}

int Button::processMouseMoveEvent(const MouseMoveEvent& event)
{
    if (!m_acceptInput)
    {
        return 0;
    }

    const ButtonConfig& cfg = GameLib::getInstance().getButtonConfig();

    m_textColor = cfg.getHoverTextColor();

    return 0;
}

int Button::processMouseButtonEvent(const MouseButtonEvent& event)
{
    if (!m_acceptInput)
    {
        return 0;
    }

    if (event.m_button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (event.m_action == GLFW_PRESS)
        {
            const ButtonConfig& cfg = GameLib::getInstance().getButtonConfig();
            m_textColor = cfg.getPressTextColor();
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
    const ButtonConfig& cfg = GameLib::getInstance().getButtonConfig();
    m_textColor = cfg.getNormalTextColor();
}

void Button::present()
{
    if (!m_visible)
    {
        return;
    }

    const TextSystem& textSys = TextSystem::getInstance();

    Widget::present();
    textSys.drawString(m_text, TEXT_SIZE_BIG,
                       m_textPos, m_textColor->getColor());
}

} // end of namespace bot

