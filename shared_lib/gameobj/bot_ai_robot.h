#ifndef INCLUDE_BOT_AI_ROBOT
#define INCLUDE_BOT_AI_ROBOT

#include "misc/bot_time_utils.h"
#include "gametemplate/bot_ai_robot_template.h"
#include "gameobj/bot_action.h"
#include "gameobj/bot_robot.h"

namespace bot {

class AIRobot : public Robot {
public:
    AIRobot();

    virtual ~AIRobot()
    {}

    bool init(const AIRobotTemplate* t, Side side,
              int hpLevel, int hpRestoreLevel,
              int armorLevel, int armorRepairLevel,
              int powerLevel, int powerRestoreLevel,
              int weaponLevel, int missileLevel,
              int moverLevel, float x, float y,
              float directionX, float directionY);

    const AIRobotTemplate* getTemplate() const
    {
        return static_cast<const AIRobotTemplate*>(m_template);
    }

    virtual void present();

    virtual void update(float delta, GameScreen& screen);

    virtual void setDirection(float directionX, float directionY);

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

    bool setCurAction(Action action);

protected:
    TimePoint m_lastChangeActionTime;
    TimePoint m_lastChangeDirectionTime;
    Action m_curAction;
};

} // end of namespace bot

#endif
