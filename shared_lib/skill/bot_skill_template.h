#ifndef INCLUDE_BOT_SKILL_TEMPLATE
#define INCLUDE_BOT_SKILL_TEMPLATE

#include <rapidjson/document.h>
#include "skill/bot_skill_type.h"

namespace bot {

class GameScreen;
class Robot;

class SkillTemplate {
public:
    static SkillTemplate* create(const rapidjson::Value& elem);

    SkillTemplate();

    virtual ~SkillTemplate()
    {}

    bool init(const rapidjson::Value& elem);

    SkillType getType() const
    {
        return m_type;
    }

    float getPowerCost() const
    {
        return m_powerCost;
    }

    float getCooldownMS() const
    {
        return m_cooldownMS;
    }

protected:
    SkillType m_type;
    float m_powerCost;
    float m_cooldownMS;
};

} // end of namespace bot

#endif

