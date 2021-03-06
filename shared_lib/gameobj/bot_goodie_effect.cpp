#include "misc/bot_constants.h"
#include "geometry/bot_rectangle.h"
#include "opengl/bot_texture.h"
#include "widget/bot_progress_ring.h"
#include "gameobj/bot_goodie_effect.h"

namespace bot {

void GoodieEffect::start()
{
    m_startTime = Clock::now();
    m_percent = 0.0f;
}

bool GoodieEffect::update(const TimePoint &now)
{
    float duration = getTemplate()->getDuration();
    if (duration == 0.0f)
    {
        return false;
    }

    if (m_percent >= 1.0f)
    {
        return true;
    }

    m_percent = timeDistMs(m_startTime, now) / duration;
    return m_percent >= 1.0f;
}

void GoodieEffect::setPos(float x, float y)
{
    m_pos[0] = x + getRadius();
    m_pos[1] = y + getRadius();
}

void GoodieEffect::draw() const
{
    if (m_template->getProgressRing())
    {
        m_template->getProgressRing()->draw(m_pos, m_percent);
    }

    if (m_template->getEffectRect())
    {
        m_template->getEffectRect()->draw(m_pos, nullptr, nullptr, nullptr,
                                          m_template->getTexture()->textureId(),
                                          nullptr);
    }
}

} // end of namespace bot
