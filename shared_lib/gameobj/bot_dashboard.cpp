#include "misc/bot_log.h"
#include "opengl/bot_text_system.h"
#include "opengl/bot_color.h"
#include "gameobj/bot_dashboard_config.h"
#include "gameobj/bot_dashboard.h"
#include "gameobj/bot_player.h"
#include "app/bot_app.h"

namespace bot {

Dashboard::Dashboard()
    : m_player(nullptr)
    , m_cfg(nullptr)
{
}

void Dashboard::init(const Player* player, const DashboardConfig* cfg)
{
    m_player = player;
    m_cfg = cfg;

    initEffectPos();
}

void Dashboard::initEffectPos()
{
    const App& app = App::getInstance();
    float screenHeight = app.getViewportHeight();
    float y = screenHeight - m_cfg->getEffectMargin() -
              m_cfg->getEffectRingRadius();
    float x = m_cfg->getEffectStartX() + m_cfg->getEffectRingRadius();
    float deltaX = m_cfg->getEffectSpacingX() +
                   m_cfg->getEffectRingRadius() * 2.0f;

    m_effectPositions.resize(GOODIE_EFFECT_COUNT);
    for (int i = 0; i < GOODIE_EFFECT_COUNT; ++i, x += deltaX)
    {
        m_effectPositions[i].m_pos[0] = x;
        m_effectPositions[i].m_pos[1] = y;
    }
}

void Dashboard::draw()
{
    int i = 0;
    const GoodieEffect* effect = m_player->getFirstActiveEffect();

    while (effect)
    {
        effect->draw(m_effectPositions[i].m_pos);
        effect = effect->getNext();
        ++i;
    }
}

} // end of namespace bot
