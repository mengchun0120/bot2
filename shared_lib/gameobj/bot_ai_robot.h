#ifndef INCLUDE_BOT_AI_ROBOT
#define INCLUDE_BOT_AI_ROBOT

#include "misc/bot_time_utils.h"
#include "gameobj/bot_action.h"
#include "gameobj/bot_robot.h"

namespace bot {

class AIRobotTemplate;

class AIRobot : public Robot {
public:
    AIRobot();

    virtual ~AIRobot()
    {}

    bool init(const AIRobotTemplate* t, Side side, int hpLevel, int hpRestoreLevel,
              int armorLevel, int armorRepairLevel, int powerLevel, int powerRestoreLevel,
              int weaponLevel, int missileLevel, int moverLevel, float x, float y,
              float directionX, float directionY);

    virtual void present(Graphics& g);

    virtual void update(float delta, GameScreen& screen);

    float getGoodieSpawnProb() const
    {
        return getTemplate()->getGoodieSpawnProb();
    }

    const TimePoint& getLastChangeActionTime() const
    {
        return m_lastChangeActionTime;
    }

    const TimePoint& getLastChangeDirectionTime() const
    {
        return m_lastChangeDirectionTime;
    }

    Action getCurAction() const
    {
        return m_curAction;
    }

    void setCurAction(Action action);

protected:
    TimePoint m_lastChangeActionTime;
    TimePoint m_lastChangeDirectionTime;
    Action m_curAction;
};

} // end of namespace bot

#endif
