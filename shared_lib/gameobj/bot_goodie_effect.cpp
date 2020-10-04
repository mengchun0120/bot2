#include "misc/bot_constants.h"
#include "geometry/bot_rectangle.h"
#include "opengl/bot_texture.h"
#include "gameobj/bot_progress_ring.h"
#include "gameobj/bot_goodie_effect.h"

namespace bot {

void GoodieEffect::start()
{
    m_startTime = Clock::now();
}

bool GoodieEffect::update(const TimePoint& now)
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

void GoodieEffect::draw(Graphics& g, const float* pos) const
{
    if (m_template->getProgressRing())
    {
        m_template->getProgressRing()->draw(g, pos, m_percent);
    }

    if (m_template->getEffectRect())
    {
        m_template->getEffectRect()->draw(g, pos, nullptr, nullptr, nullptr,
                                          m_template->getTexture()->textureId(),
                                          nullptr);
    }
}

} // end of namespace bot
