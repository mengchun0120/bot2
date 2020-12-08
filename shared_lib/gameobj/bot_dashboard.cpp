#include "misc/bot_log.h"
#include "opengl/bot_text_system.h"
#include "opengl/bot_color.h"
#include "geometry/bot_rectangle.h"
#include "gameutil/bot_game_lib.h"
#include "gametemplate/bot_goodie_template.h"
#include "gametemplate/bot_progress_bar_template.h"
#include "gameobj/bot_dashboard_config.h"
#include "gameobj/bot_dashboard.h"
#include "gameobj/bot_player.h"
#include "app/bot_app.h"

namespace bot {

bool getStatusBarTemplates(
            std::vector<const ProgressBarTemplate*>& barTemplates)
{
    const NamedMap<ProgressBarTemplate>& lib =
                GameLib::getInstance().getProgressBarTemplateLib();
    const char* barNames[] = {"power_bar", "armor_bar"};

    barTemplates.resize(Dashboard::BAR_COUNT);
    for (int i = 0; i < Dashboard::BAR_COUNT; ++i)
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

Dashboard::Dashboard()
    : m_player(nullptr)
{
}

bool Dashboard::init(const Player* player)
{
    m_player = player;

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
        totalWidth += barTemplates[i]->getWidth();
    }
    totalWidth += (BAR_COUNT - 1) * cfg.getBarSpacing();

    float y = cfg.getBarMargin();
    float x = (viewportWidth - totalWidth) / 2.0f;
    float ratios[] = {
        m_player->getBase().getPowerRatio(),
        m_player->getBase().getArmorRatio()
    };

    m_statusBars.resize(BAR_COUNT);
    for (int i = 0; i < BAR_COUNT; ++i)
    {
        if (!m_statusBars[i].init(barTemplates[i], x, y, ratios[i]))
        {
            LOG_ERROR("Failed to initialize status-bar %d", i);
            return false;
        }

        x += barTemplates[i]->getWidth() + cfg.getBarSpacing();
    }

    return true;
}

void Dashboard::draw()
{
    if (m_player->getActiveEffectCount() > 0)
    {
        drawEffects();
    }

    drawStatusBars();
}

void Dashboard::drawEffects()
{
    const DashboardConfig& cfg = GameLib::getInstance().getDashboardConfig();
    float viewportWidth = App::getInstance().getViewportWidth();
    float totalWidth = 0.0f;
    const GoodieEffect* effect = m_player->getFirstActiveEffect();

    while (effect)
    {
        totalWidth += effect->getWidth();
        effect = effect->getNext();
    }
    totalWidth += (m_player->getActiveEffectCount() - 1) *
                  cfg.getEffectSpacing();

    float x = (viewportWidth - totalWidth) / 2.0f + effect->getRadius();
    float pos[] = {x, cfg.getEffectMargin()};

    effect = m_player->getFirstActiveEffect();
    effect->draw(pos);

    const GoodieEffect* prevEffect = effect;

    for (effect = effect->getNext(); effect; effect = effect->getNext())
    {
        pos[0] += prevEffect->getRadius() + cfg.getEffectSpacing() +
                  effect->getRadius();
        effect->draw(pos);
        prevEffect = effect;
    }
}

void Dashboard::drawStatusBars()
{
    m_statusBars[BAR_POWER].draw(m_player->getBase().getPowerRatio());
    m_statusBars[BAR_ARMOR].draw(m_player->getBase().getArmorRatio());
}

} // end of namespace bot

