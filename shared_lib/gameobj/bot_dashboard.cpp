#include "misc/bot_log.h"
#include "opengl/bot_text_system.h"
#include "opengl/bot_color.h"
#include "geometry/bot_rectangle.h"
#include "gameutil/bot_game_lib.h"
#include "gametemplate/bot_progress_bar_template.h"
#include "gameobj/bot_dashboard_config.h"
#include "gameobj/bot_dashboard.h"
#include "gameobj/bot_player.h"
#include "app/bot_app.h"

namespace bot {

Dashboard::Dashboard()
    : m_player(nullptr)
{
}

bool Dashboard::init(const Player* player)
{
    m_player = player;

    if (!initStatusBars())
    {
        return false;
    }

    initEffectPos();

    return true;
}

bool Dashboard::getStatusBarTemplates(
            std::vector<const ProgressBarTemplate*>& barTemplates)
{
    const NamedMap<ProgressBarTemplate>& lib =
                GameLib::getInstance().getProgressBarTemplateLib();
    const char* barNames[] = {"energy_bar", "armor_bar"};

    barTemplates.resize(BAR_COUNT);
    for (int i = 0; i < BAR_COUNT; ++i)
    {
        ProgressBarTemplate* t = lib.search(barNames[i]);
        if (!t)
        {
            LOG_ERROR("Failed to find progress-bar %s", barNames[i]);
            return false;
        }

        barTemplates[i] = t;
    }

    return true;
}

bool Dashboard::initStatusBars()
{
    std::vector<const ProgressBarTemplate*> barTemplates;
    if (!getStatusBarTemplates(barTemplates))
    {
        return false;
    }

    const DashboardConfig& cfg = GameLib::getInstance().getDashboardConfig();
    float viewportWidth = App::getInstance().getViewportWidth();
    float totalWidth = 0.0f;

    for (int i = 0; i < BAR_COUNT; ++i)
    {
        totalWidth += barTemplates[i]->getRect()->width();
    }
    totalWidth += (BAR_COUNT - 1) * cfg->getBarSpacing();

    float y = cfg.getBarMargin();
    float x = (viewportWidth - totalWidth) / 2.0f;
    float ratios[] = {m_player->getPowerRatio(), m_player->getArmorRatio()};

    m_statusBars.resize(BAR_COUNT);
    for (int i = 0; i < BAR_COUNT; ++i)
    {
        if (!m_statusBars.init(barTemplates[i], x, y, ratios[i]))
        {
            LOG_ERROR("Failed to initialize status-bar %d", i);
            return false;
        }

        x += barTemplates[i]->getRect()->width() + cfg.getBarSpacing();
    }

    return true;
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
    const DashboardConfig& cfg = GameLib::getInstance().getDashboardConfig();
    float totalWidth = 0.0f;
    const GoodieEffect* effect = m_player->getFirstActiveEffect();

    while (effect)
    {
        totalWidth
        effect = effect->getNext();
        ++i;
    }

    m_statusBars[BAR_POWER].draw(m_player->getPowerRatio());
    m_statusBars[BAR_ARMOR].draw(m_player->getArmorRatio());
}

} // end of namespace bot

