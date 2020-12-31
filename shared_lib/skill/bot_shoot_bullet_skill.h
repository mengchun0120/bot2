#ifndef INCLUDE_BOT_SHOOT_BULLET_SKILL
#define INCLUDE_BOT_SHOOT_BULLET_SKILL

#include "skill/bot_shoot_bullet_skill_template.h"
#include "skill/bot_shoot_missile_skill.h"

namespace bot {

class ShootBulletSkill: public ShootMissileSkill {
public:
    ShootBulletSkill();

    virtual ~ShootBulletSkill()
    {}

    bool init(const ShootBulletSkillTemplate* t, Robot* robot,
              unsigned int level);

    const ShootBulletSkillTemplate* getTemplate() const
    {
        return static_cast<const ShootBulletSkillTemplate*>(m_template);
    }
};

} // end of namespace bot

#endif

