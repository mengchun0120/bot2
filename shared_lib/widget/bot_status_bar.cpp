#include <cstdio>
#include <cstring>
#include "misc/bot_log.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_text_system.h"
#include "geometry/bot_rectangle.h"
#include "widget/bot_status_bar.h"

namespace bot {

StatusBar::StatusBar()
    : m_template(nullptr)
    , m_text(nullptr)
{
}

StatusBar::~StatusBar()
{
    delete m_text;
}

bool StatusBar::init(const StatusBarTemplate *t, float x, float y)
{
    if (!t)
    {
        LOG_ERROR("StatusBarTemplate is null");
        return false;
    }

    m_template = t;

    m_text = new char[t->getTextLen() + 1];
    m_text[0] = '\0';

    m_texturePos[0] = x + t->getRect().width() / 2.0f;
    m_texturePos[1] = y + t->getRect().height() / 2.0f;
    m_textPos[0] = x + t->getRect().width() + t->getTextureTextSpacing();
    m_textPos[1] = y;

    return true;
}

void StatusBar::setText(const char *text)
{
    strncpy(m_text, text, m_template->getTextLen());
}

void StatusBar::setText(int i)
{
    snprintf(m_text, m_template->getTextLen() + 1, "%d", i);
}

void StatusBar::setText(float f)
{
    snprintf(m_text, m_template->getTextLen() + 1, "%f", f);
}

void StatusBar::draw()
{
    m_template->getRect().draw(m_texturePos, nullptr, nullptr, nullptr,
                               m_template->getTexture()->textureId(),
                               nullptr);

    const TextSystem &textSys = TextSystem::getInstance();
    textSys.drawString(m_text, m_template->getTextSize(), m_textPos,
                       m_template->getTextColor().getColor());
}

} // end of namespace bot

