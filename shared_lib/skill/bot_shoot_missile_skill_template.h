#ifndef INCLUDE_BOT_SHOOT_MISSILE_SKILL_TEMPLATE
#define INCLUDE_BOT_SHOOT_MISSILE_SKILL_TEMPLATE

#include "gameobj/bot_missile_template.h"
#include "skill/bot_attack_skill_template.h"

namespace bot {

class ShootMissileSkillTemplate: public AttackSkillTemplate {
public:
    ShootMissileSkillTemplate();

    virtual ~ShootMissileSkillTemplate()
    {}

    bool init(const rapidjson::Value &elem);

    const MissileTemplate *getMissileTemplate() const
    {
        return m_missileTemplate;
    }

    float getSpeedMultiplier() const
    {
        return m_speedMultiplier;
    }

protected:
    const MissileTemplate *m_missileTemplate;
    float m_speedMultiplier;
};

} // end of namespace bot

#endif

