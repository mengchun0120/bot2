#ifndef INCLUDE_BOT_DASHBOARD
#define INCLUDE_BOT_DASHBOARD

#include <utility>
#include <vector>
#include "misc/bot_constants.h"

namespace bot {

class Player;
class DashboardConfig;
class Graphics;
class TextSystem;

class Dashboard {
public:
    struct Point {
        float m_pos[Constants::NUM_FLOATS_PER_POSITION];
    };

    Dashboard();

    ~Dashboard()
    {}

    void init(const Player* player, const DashboardConfig* cfg,
              float viewportHeight, const TextSystem& textSys);

    void draw(Graphics& g);

private:
    void initEffectPos(float screenHeight);

    void initHeader(const TextSystem& textSys, float screenHeight);

private:
    const Player* m_player;
    const DashboardConfig* m_cfg;
    std::vector<Point> m_effectPositions;
    float m_hpIconPos[Constants::NUM_FLOATS_PER_POSITION];
    float m_hpTextPos[Constants::NUM_FLOATS_PER_POSITION];
    float m_goldIconPos[Constants::NUM_FLOATS_PER_POSITION];
    float m_goldTextPos[Constants::NUM_FLOATS_PER_POSITION];
};

} // end of namespace bot

#endif
