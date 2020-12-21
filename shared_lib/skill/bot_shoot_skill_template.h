#ifndef INCLUDE_BOT_SHOOT_SKILL_TEMPLATE
#define INCLUDE_BOT_SHOOT_SKILL_TEMPLATE

#include "skill/bot_skill_template.h"

namespace bot {

template <typename T> class NamedMap;
class MissileTemplate;

class ShootSkillTemplate: public SkillTemplate {
public:
    ShootSkillTemplate();

    virtual ~ShootSkillTemplate()
    {}

    bool init(const rapidjson::Value& elem);

    const MissileTemplate* getMissileTemplate() const
    {
        return m_missile;
    }

    float getDamageMultiplier(unsigned int level) const
    {
        return m_baseDamageMultiplier + m_damageMultiplierPerLevel * level;
    }

    float getSpeedMultiplier() const
    {
        return m_speedMultiplier;
    }

protected:
    const MissileTemplate* m_missile;
    float m_baseDamageMultiplier;
    float m_damageMultiplierPerLevel;
    float m_speedMultiplier;
};

} // end of namespace bot

#endif

