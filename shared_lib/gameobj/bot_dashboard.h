#ifndef INCLUDE_BOT_DASHBOARD
#define INCLUDE_BOT_DASHBOARD

#include <utility>
#include <vector>
#include "misc/bot_constants.h"
#include "gameobj/bot_progress_bar.h"

namespace bot {

class Player;

class Dashboard {
public:
    enum {
        BAR_POWER,
        BAR_ARMOR,
        BAR_COUNT
    };

    Dashboard();

    ~Dashboard()
    {}

    bool init(const Player* player);

    void draw();

private:
    void drawEffects();

    void drawStatusBars();

private:
    const Player* m_player;
    std::vector<ProgressBar> m_statusBars;
};

} // end of namespace bot

#endif

