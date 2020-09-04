#include "gameobj/bot_ai_robot.h"
#include "ai/bot_ai.h"
#include "screen/bot_game_screen.h"

namespace bot {

AIRobot::AIRobot(const AIRobotTemplate* t)
    : Robot(t)
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
