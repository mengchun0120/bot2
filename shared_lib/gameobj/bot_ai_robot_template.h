#ifndef INCLUDE_BOT_AI_ROBOT_TEMPLATE
#define INCLUDE_BOT_AI_ROBOT_TEMPLATE

#include <string>
#include "gameobj/bot_robot_template.h"

namespace bot {

class AI;

class AIRobotTemplate : public RobotTemplate {
public:
    AIRobotTemplate();

    virtual ~AIRobotTemplate()
    {}

    bool init(const rapidjson::Value& elem);

    AI* getAI() const
    {
        return m_ai;
    }

    void setAI(AI* ai);

    float getGoodieSpawnProb() const
    {
        return m_goodieSpawnProb;
    }

    bool setGoodieSpawnProb(float prob);

protected:
    AI* m_ai;
    float m_goodieSpawnProb;
};

} // end of namespace bot

#endif
