#include "misc/bot_log.h"
#include "opengl/bot_graphics.h"
#include "gametemplate/bot_ai_robot_template.h"
#include "gameobj/bot_ai_robot.h"
#include "ai/bot_ai.h"
#include "screen/bot_game_screen.h"

namespace bot {

AIRobot::AIRobot()
    : m_curAction(ACTION_NONE)
{
}

bool AIRobot::init(const AIRobotTemplate* t, Side side,
                   int hpLevel, int hpRestoreLevel,
                   int armorLevel, int armorRepairLevel,
                   int powerLevel, int powerRestoreLevel,
                   int weaponLevel, int missileLevel,
                   int moverLevel, float x, float y,
                   float directionX, float directionY)
{
    bool ret = Robot::init(t, side, hpLevel, hpRestoreLevel, armorLevel,
                           armorRepairLevel, powerLevel, powerRestoreLevel,
                           weaponLevel, missileLevel, moverLevel,
                           x, y, directionX, directionY);

    if (!ret)
    {
        return false;
    }

    m_lastChangeActionTime = Clock::now();
    m_lastChangeDirectionTime = Clock::now();

    return true;
}

void AIRobot::present(Graphics& g)
{
    Robot::present(g);
}

void AIRobot::update(float delta, GameScreen& screen)
{
    const AIRobotTemplate* t = static_cast<const AIRobotTemplate*>(m_template);
    t->getAI()->apply(*this, delta, screen);
}

} // end of namespace bot
