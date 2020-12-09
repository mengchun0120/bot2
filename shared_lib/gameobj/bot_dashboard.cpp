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

bool getProgressBarTemplates(
            std::vector<const ProgressBarTemplate*>& barTemplates,
            const char* barNames[], int barCount)
{
    const NamedMap<ProgressBarTemplate>& lib =
                GameLib::getInstance().getProgressBarTemplateLib();

    barTemplates.resize(barCount);
    for (int i = 0; i < barCount; ++i)
    {
        const ProgressBarTemplate* t = lib.search(barNames[i]);
        if (!t)
        {
            LOG_ERROR("Failed to find progress-bar %s", barNames[i]);
            return false;
        }

        barTemplates[i] = t;
    }

    return true;
}

bool getStatusBarTemplates(
            std::vector<const StatusBarTemplate*>& statusTemplates,
            const char* statusNames[], int statusCount)
{
    const NamedMap<StatusBarTemplate>& lib =
                GameLib::getInstance().getStatusBarTemplateLib();

    statusTemplates.resize(statusCount);
    for (int i = 0; i < statusCount; ++i)
    {
        const StatusBarTemplate* t = lib.search(statusNames[i]);
        if (!t)
        {
            LOG_ERROR("Failed to find progress-bar %s", statusNames[i]);
            return false;
        }

        statusTemplates[i] = t;
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
}

bool Dashboard::initProgressBars()
{
    std::vector<const ProgressBarTemplate*> barTemplates;
    const char* barNames[] = {"power_bar", "armor_bar"};

    if (!getProgressBarTemplates(barTemplates, barNames, BAR_COUNT))
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

    m_progressBars.resize(BAR_COUNT);
    for (int i = 0; i < BAR_COUNT; ++i)
    {
        if (!m_progressBars[i].init(barTemplates[i], x, y, ratios[i]))
        {
            LOG_ERROR("Failed to initialize status-bar %d", i);
            return false;
        }

        x += barTemplates[i]->getWidth() + cfg.getBarSpacing();
    }

    return true;
}

bool Dashboard::initStatusBars()
{
    std::vector<const StatusBarTemplate*>
}

void Dashboard::draw()
{
    if (m_player->getActiveEffectCount() > 0)
    {
        drawEffects();
    }

    drawProgressBars();
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

    effect = m_player->getFirstActiveEffect();

    float x = (viewportWidth - totalWidth) / 2.0f + effect->getRadius();
    float pos[] = {x, cfg.getEffectMargin()};
    const GoodieEffect* prevEffect = effect;

    effect->draw(pos);

    for (effect = effect->getNext(); effect; effect = effect->getNext())
    {
        pos[0] += prevEffect->getRadius() + cfg.getEffectSpacing() +
                  effect->getRadius();
        effect->draw(pos);
        prevEffect = effect;
    }
}

void Dashboard::drawProgressBars()
{
    m_progressBars[BAR_POWER].draw(m_player->getBase().getPowerRatio());
    m_progressBars[BAR_ARMOR].draw(m_player->getBase().getArmorRatio());
}

} // end of namespace bot

