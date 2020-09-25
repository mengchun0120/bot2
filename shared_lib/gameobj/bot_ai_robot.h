#ifndef INCLUDE_BOT_AI_ROBOT
#define INCLUDE_BOT_AI_ROBOT

#include "misc/bot_time_utils.h"
#include "gametemplate/bot_ai_robot_template.h"
#include "gameobj/bot_robot.h"

namespace bot {

class AI;
class AIRobotTemplate;

class AIRobot : public Robot {
public:
    AIRobot(const AIRobotTemplate* t, const BaseComponentTemplate* baseTemplate,
            const WeaponComponentTemplate* weaponTemplate, const MoverComponentTemplate* moverTemplate,
            const MissileTemplate* missileTemplate, float x, float y, float directionX, float directionY);

    virtual ~AIRobot()
    {}

    const AIRobotTemplate* getTemplate() const
    {
        return static_cast<const AIRobotTemplate*>(m_template);
    }

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


private:
    TimePoint m_lastChangeActionTime;
    TimePoint m_lastChangeDirectionTime;
    Action m_curAction;
};

} // end of namespace bot

#endif
