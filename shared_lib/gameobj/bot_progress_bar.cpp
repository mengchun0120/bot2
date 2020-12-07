#include "misc/bot_log.h"
#include "gameobj/bot_progress_bar_template.h"
#include "gameobj/bot_progress_bar.h"

namespace bot {

ProgressBar::ProgressBar()
    : m_template(nullptr)
    , m_visibleVertices(0)
{
}

bool ProgressBar::init(const ProgressBarTemplate* t, float x, float y,
                       float initialRatio)
{
    if (!t)
    {
        LOG_ERROR("ProgressBarTemplate is null");
        return false;
    }

    m_template = t;
    m_pos[0] = x + t->getTexture()->width() / 2.0f;
    m_pos[1] = y + t->getTexture()->height() / 2.0f;


    return true;
}

void ProgressBar::initBar()
{
    std::vector<float> vertices;
    unsigned int numFloats = (m_template->getSlotCount() + 1) * 2 *
                             Constants::NUM_FLOATS_PER_POSITION;

    vertices.resize(numFloats);


}

bool ProgressBar::setRatio(float ratio)
{
}

void ProgressBar::draw()
{
}

} // end of namespace bot

