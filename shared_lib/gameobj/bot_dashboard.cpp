#include "misc/bot_log.h"
#include "opengl/bot_graphics.h"
#include "opengl/bot_color.h"
#include "gameobj/bot_dashboard_config.h"
#include "gameobj/bot_dashboard.h"
#include "gameobj/bot_player.h"

namespace bot {

Dashboard::Dashboard()
    : m_player(nullptr)
    , m_cfg(nullptr)
{
}

void Dashboard::init(const Player* player, const DashboardConfig* cfg, float viewportHeight,
                     const TextSystem& textSys)
{
    m_player = player;
    m_cfg = cfg;

    float screenHeight = viewportHeight;

    initEffectPos(screenHeight);
    initHeader(textSys, screenHeight);
}

void Dashboard::initEffectPos(float screenHeight)
{
    float y = screenHeight - m_cfg->getHeaderTopMargin() - m_cfg->getEffectRingRadius();
    float x = m_cfg->getEffectStartX() + m_cfg->getEffectRingRadius();
    float deltaX = m_cfg->getEffectSpacingX() + m_cfg->getEffectRingRadius() * 2.0f;

    m_effectPositions.resize(GOODIE_EFFECT_COUNT);
    for (int i = 0; i < GOODIE_EFFECT_COUNT; ++i, x += deltaX)
    {
        m_effectPositions[i].m_pos[0] = x;
        m_effectPositions[i].m_pos[1] = y;
    }
}

void Dashboard::initHeader(const TextSystem& textSys, float screenHeight)
{
    const Rectangle& rect = textSys.getRect(TEXT_SIZE_MEDIUM, ' ');
    float y = screenHeight - m_cfg->getHeaderTopMargin();
    m_hpIconPos[0] = m_cfg->getHPIconX() + m_cfg->getHPRect()->width() / 2.0f;
    m_hpIconPos[1] = y - m_cfg->getHPRect()->height() / 2.0f;
    m_hpTextPos[0] = m_cfg->getHPTextX();
    m_hpTextPos[1] = y - rect.height();
    m_goldIconPos[0] = m_cfg->getGoldIconX() + m_cfg->getGoldRect()->width() / 2.0f;
    m_goldIconPos[1] = y - m_cfg->getGoldRect()->height() / 2.0f;
    m_goldTextPos[0] = m_cfg->getGoldTextX();
    m_goldTextPos[1] = y - rect.height();
}

void Dashboard::draw(Graphics& g)
{
    const TextSystem& textSys = g.getTextSystem();
    const float HP_GOOD_THRESHOLD = 2.0f / 3.0f;
    const float HP_BAD_THRESHOLD = 1.0f / 3.0f;
    int i = 0;
    const GoodieEffect* effect;

    for (effect = m_player->getFirstActiveEffect(); effect; effect = effect->getNext(), ++i)
    {
        effect->draw(g, m_effectPositions[i].m_pos);
    }

    m_cfg->getHPRect()->draw(g, m_hpIconPos, nullptr, nullptr, nullptr,
                             m_cfg->getHPTexture()->textureId(), nullptr);

    const Color* hpColor = nullptr;
    float hpRatio = m_player->getHPRatio();
    if (hpRatio >= HP_GOOD_THRESHOLD)
    {
        hpColor = m_cfg->getHPGoodColor();
    }
    else if (hpRatio >= HP_BAD_THRESHOLD)
    {
        hpColor = m_cfg->getHPBadColor();
    }
    else
    {
        hpColor = m_cfg->getHPCriticalColor();
    }

    textSys.drawString(g.getSimpleShader(), m_player->getHPStr(), TEXT_SIZE_MEDIUM, m_hpTextPos,
                       hpColor->getColor());

    m_cfg->getGoldRect()->draw(g, m_goldIconPos, nullptr, nullptr, nullptr,
                               m_cfg->getGoldTexture()->textureId(), nullptr);

    textSys.drawString(g.getSimpleShader(), m_player->getGoldStr(), TEXT_SIZE_MEDIUM, m_goldTextPos,
                       m_cfg->getGoldTextColor()->getColor());
}

} // end of namespace bot
