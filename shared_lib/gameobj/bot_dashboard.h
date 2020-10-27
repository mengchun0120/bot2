#ifndef INCLUDE_BOT_DASHBOARD
#define INCLUDE_BOT_DASHBOARD

#include <utility>
#include <vector>
#include "misc/bot_constants.h"

namespace bot {

class Player;
class DashboardConfig;
class TextSystem;

class Dashboard {
public:
    struct Point {
        float m_pos[Constants::NUM_FLOATS_PER_POSITION];
    };

    Dashboard();

    ~Dashboard()
    {}

    void init(const Player* player, const DashboardConfig* cfg);

    void draw();

private:
    void initEffectPos();

private:
    const Player* m_player;
    const DashboardConfig* m_cfg;
    std::vector<Point> m_effectPositions;
};

} // end of namespace bot

#endif
