#include "opengl/bot_opengl.h"
#include "opengl/bot_color.h"
#include "opengl/bot_text_system.h"
#include "input/bot_input_event.h"
#include "widget/bot_button.h"
#include "gameutil/bot_game_lib.h"

namespace bot {

Button::Button()
    : Box()
    , m_textColor(nullptr)
    , m_textSize(TEXT_SIZE_MEDIUM)
{
    m_textPos[0] = 0.0f;
    m_textPos[1] = 0.0f;
}

bool Button::init(float x, float y,
                  float width, float height,
                  const Rectangle* rect,
                  const std::string& text,
                  TextSize textSize,
                  bool visible, bool acceptInput)
{
    const ButtonConfig& cfg = GameLib::getInstance().getButtonConfig();

    bool ret = Box::init(x, y, width, height, rect, cfg.getTexture(),
                         nullptr, nullptr, visible, acceptInput);
    if (!ret)
    {
        return false;
    }

    if (!isValidTextSize(textSize))
    {
        LOG_ERROR("Invalid text-size %d", static_cast<int>(textSize));
        return false;
    }

    m_textSize = textSize;
    setText(text);
    m_textColor = cfg.getNormalTextColor();

    return true;
}

void Button::present()
{
    if (!m_visible)
    {
        return;
    }

    Box::present();

    const TextSystem& textSys = TextSystem::getInstance();
    textSys.drawString(m_text, m_textSize,
                       m_textPos, m_textColor->getColor());
}

void Button::setText(const std::string& text)
{
    m_text = text;
    resetTextPos();
}

void Button::setPos(float x, float y)
{
    Box::setPos(x, y);
    resetTextPos();
}

void Button::shiftPos(float dx, float dy)
{
    Box::shiftPos(dx, dy);
    m_textPos[0] += dx;
    m_textPos[1] += dy;
}

int Button::processKeyEvent(const KeyEvent& event)
{
    if (!m_acceptInput)
    {
        return 0;
    }

    switch(event.m_key)
    {
        case GLFW_KEY_ENTER:
        {
            if (event.m_action == GLFW_PRESS && m_actionFunc)
            {
                return m_actionFunc();
            }
            return 0;
        }
        default:
        {
            break;
        }
    }

    return 0;
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

void Button::resetTextPos()
{
    float w, h;
    const TextSystem& textSys = TextSystem::getInstance();

    textSys.getStringSize(w, h, m_textSize, m_text);
    m_textPos[0] = m_left + (m_width - w) / 2.0f;
    m_textPos[1] = m_bottom + (m_height - h) / 2.0f;
}

} // end of namespace bot

