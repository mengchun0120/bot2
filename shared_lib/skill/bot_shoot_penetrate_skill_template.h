#ifndef INCLUDE_SHOOT_PENETRATE_SKILL_TEMPLATE
#define INCLUDE_SHOOT_PENETRATE_SKILL_TEMPLATE

#include "skill/bot_shoot_skill_template.h"

namespace bot {

class ShootPenetrateSkillTemplate: public ShootSkillTemplate {
public:
    ShootPenetrateSkillTemplate();

    virtual ~ShootPenetrateSkillTemplate()
    {}

    bool init(const rapidjson::Value& elem);

    int getPenetrateLimit() const
    {
        return m_penetrateLimit;
    }

    float getDamageMultiplierPenetrate() const
    {
        return m_damageMultiplierPenetrate;
    }

    bool getExplodeOnDead() const
    {
        return m_explodeOnDead;
    }

protected:
    int m_penetrateLimit;
    float m_damageMultiplierPenetrate;
    bool m_explodeOnDead;
};


} // end of namespace bo

#endif

