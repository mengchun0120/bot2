#ifndef INCLUDE_BOT_SKILL
#define INCLUDE_BOT_SKILL

#include <rapidjson/document.h>
#include "skill/bot_skill_type.h"

namespace bot {

class GameScreen;

class Skill {
public:
    static Skill* create(const rapidjson::Value& elem);

    Skill();

    virtual ~Skill()
    {}

    bool init(const rapidjson::Value& elem);

    SkillType getType() const
    {
        return m_type;
    }

    float getConsumedPower() const
    {
        return m_consumedPower;
    }

    float getCooldownMS() const
    {
        return m_cooldownMS;
    }

    virtual bool apply(Robot& robot, GameScreen& screen) = 0;

protected:
    SkillType m_type;
    float m_consumedPower;
    float m_cooldownMS;
};

} // end of namespace bot

#endif

