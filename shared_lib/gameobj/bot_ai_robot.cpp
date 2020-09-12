#include "gameobj/bot_ai_robot.h"
#include "ai/bot_ai.h"
#include "screen/bot_game_screen.h"

namespace bot {

AIRobot::AIRobot(const AIRobotTemplate* t, const BaseComponentTemplate* baseTemplate,
                 const WeaponComponentTemplate* weaponTemplate, const MoverComponentTemplate* moverTemplate,
                 const MissileTemplate* missileTemplate, float x, float y, float directionX, float directionY)
    : Robot(t, baseTemplate, weaponTemplate, moverTemplate, missileTemplate, x, y, directionX, directionY, SIDE_AI)
{
}

void AIRobot::present(Graphics& g)
{
    Robot::present(g);
}

void AIRobot::update(float delta, GameScreen& screen)
{
    const AIRobotTemplate* t = getTemplate();
    t->getAI()->apply(*this, delta, screen);
}

} // end of namespace bot
