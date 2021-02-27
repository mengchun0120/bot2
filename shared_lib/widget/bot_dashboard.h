#ifndef INCLUDE_BOT_DASHBOARD
#define INCLUDE_BOT_DASHBOARD

#include <utility>
#include <vector>
#include "misc/bot_constants.h"
#include "widget/bot_progress_bar.h"
#include "widget/bot_status_bar.h"

namespace bot {

class GameScreen;

class Dashboard {
public:
    enum ProgressBarIndex {
        BAR_POWER,
        BAR_ARMOR,
        BAR_COUNT
    };

    enum StatusBarIndex {
        STATUS_AI_ROBOT,
        STATUS_COUNT
    };

    Dashboard();

    ~Dashboard()
    {}

    bool init(const GameScreen *screen);

    void draw();

    bool setProgressBarRatio(ProgressBarIndex idx, float ratio);

    void setAIRobotCount(int count);

private:
    bool initProgressBars();

    bool initStatusBars();

    void drawEffects();

    void drawProgressBars();

    void drawStatusBars();

private:
    const GameScreen *m_screen;
    std::vector<ProgressBar> m_progressBars;
    std::vector<StatusBar> m_statusBars;
};

} // end of namespace bot

#endif

