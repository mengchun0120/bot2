#ifndef INCLUDE_BOT_DASHBOARD
#define INCLUDE_BOT_DASHBOARD

#include <utility>
#include <vector>
#include "misc/bot_constants.h"
#include "gameobj/bot_progress_bar.h"
#include "gameobj/bot_status_bar.h"

namespace bot {

class GameObjectManager;

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

    bool init(const GameObjectManager* gameObjMgr);

    void draw();

private:
    bool initProgressBars();

    bool initStatusBars();

    void drawEffects();

    void drawProgressBars();

    void drawStatusBars();

private:
    const GameObjectManager* m_gameObjMgr;
    std::vector<ProgressBar> m_progressBars;
    std::vector<StatusBar> m_statusBars;
};

} // end of namespace bot

#endif

