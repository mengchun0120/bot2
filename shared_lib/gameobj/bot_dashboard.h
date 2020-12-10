#ifndef INCLUDE_BOT_DASHBOARD
#define INCLUDE_BOT_DASHBOARD

#include <utility>
#include <vector>
#include "misc/bot_constants.h"
#include "gameobj/bot_progress_bar.h"
#include "gameobj/bot_status_bar.h"

namespace bot {

class Player;

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

    bool init(const Player* m_player);

    void draw();

    void setProgressBarRatio(ProgressBarIndex idx, float ratio);

    void setAIRobotCount(int count);

private:
    bool initProgressBars();

    bool initStatusBars();

    void drawEffects();

    void drawProgressBars();

    void drawStatusBars();

private:
    const Player* m_player;
    std::vector<ProgressBar> m_progressBars;
    std::vector<StatusBar> m_statusBars;
};

} // end of namespace bot

#endif

