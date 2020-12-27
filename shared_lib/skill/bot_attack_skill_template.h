#ifndef INCLUDE_BOT_ATTACK_SKILL_TEMPLATE
#define INCLUDE_BOT_ATTACK_SKILL_TEMPLATE

#include "skill/bot_skill_template.h"

namespace bot {

class AttackSkillTemplate: public SkillTemplate {
public:
    AttackSkillTemplate();

    virtual ~AttackSkillTemplate()
    {}

    bool init(SkillType type, const rapidjson::Value& elem);

    float getDamageMultiplier(unsigned int level) const
    {
        return m_baseDamageMultiplier + m_damageMultiplierPerLevel * level;
    }

protected:
    float m_baseDamageMultiplier;
    float m_damageMultiplierPerLevel;
};

} // end of namespace bot

#endif

