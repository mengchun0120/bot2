#ifndef INCLUDE_BOT_DASHBOARD
#define INCLUDE_BOT_DASHBOARD

#include <utility>
#include <vector>
#include "misc/bot_constants.h"
#include "gameobj/bot_progress_bar.h"

namespace bot {

class Player;
class DashboardConfig;
class TextSystem;

class Dashboard {
    enum {
        BAR_POWER,
        BAR_ARMOR,
        BAR_COUNT
    };

public:
    struct Point {
        float m_pos[Constants::NUM_FLOATS_PER_POSITION];
    };

    Dashboard();

    ~Dashboard()
    {}

    void init(const Player* player);

    void draw();

private:
    bool getStatusBarTemplates(
            std::vector<const ProgressBarTemplate*>& barTemplates);

    bool initStatusBars();

    void initEffectPos();

private:
    const Player* m_player;
    std::vector<ProgressBar> m_statusBars;
};

} // end of namespace bot

#endif

