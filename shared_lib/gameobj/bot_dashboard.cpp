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
    if (!player)
    {
        LOG_ERROR("player is null");
        return false;
    }

    m_player = player;

    if (!initProgressBars())
    {
        return false;
    }

    if (!initStatusBars())
    {
        return false;
    }

    return true;
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

    for (int i = 0; i < BAR_COUNT; ++i)
    {
        if (!m_progressBars[i].init(barTemplates[i], x, y))
        {
            LOG_ERROR("Failed to initialize progress-bar %d", i);
            return false;
        }

        x += barTemplates[i]->getWidth() + cfg.getBarSpacing();
    }

    return true;
}

bool Dashboard::initStatusBars()
{
    std::vector<const StatusBarTemplate*> statusTemplates;
    const char* statusNames[] = {"ai_robot_status"};

    if (!getStatusBarTemplates(statusTemplates, statusNames, STATUS_COUNT))
    {
        return false;
    }

    const App& app = App::getInstance();
    const DashboardConfig& cfg = GameLib::getInstance().getDashboardConfig();
    float viewportWidth = app.getViewportWidth();
    float viewportHeight = app.getViewportHeight();
    float totalWidth = 0.0f, maxHeight = 0.0f;

    for (int i = 0; i < STATUS_COUNT; ++i)
    {
        totalWidth += statusTemplates[i]->getWidth();

        float height = statusTemplates[i]->getHeight();
        if (height > maxHeight)
        {
            maxHeight = height;
        }
    }
    totalWidth += (STATUS_COUNT - 1) * cfg.getStatusSpacing();

    float x = (viewportWidth - totalWidth) / 2.0f;
    float y = viewportHeight - cfg.getStatusMargin() - maxHeight;

    m_statusBars.resize(STATUS_COUNT);
    for (int i = 0; i < STATUS_COUNT; ++i)
    {
        if (!m_statusBars[i].init(statusTemplates[i], x, y))
        {
            LOG_ERROR("Failed to initialize status-bar %d", i);
            return false;
        }

        x += statusTemplates[i]->getWidth() + cfg.getStatusSpacing();
    }

    return true;
}

void Dashboard::draw()
{
    drawEffects();
    drawProgressBars();
    drawStatusBars();
}

void Dashboard::setAIRobotCount(int count)
{
    m_statusBars[STATUS_AI_ROBOT].setText(count);
}

void Dashboard::drawEffects()
{
    const GoodieEffect* effect = m_player->getFirstActiveEffect();

    for (; effect; effect = effect->getNext())

    {
        effect->draw();
    }
}

void Dashboard::drawProgressBars()
{
    for (int i = BAR_POWER; i < BAR_COUNT; ++i)
    {
        m_progressBars[i].draw();
    }
}

void Dashboard::drawStatusBars()
{
    for (int i = 0; i < STATUS_COUNT; ++i)
    {
        m_statusBars[i].draw();
    }
}

} // end of namespace bot

