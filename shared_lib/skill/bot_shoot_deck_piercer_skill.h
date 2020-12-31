#ifndef INCLUDE_BOT_SHOOT_DECK_PIERCER_SKILL
#define INCLUDE_BOT_SHOOT_DECK_PIERCER_SKILL

#include "skill/bot_shoot_deck_piercer_skill_template.h"
#include "skill/bot_shoot_missile_skill.h"

namespace bot {

class ShootDeckPiercerSkill: public ShootMissileSkill {
public:
    ShootDeckPiercerSkill();

    virtual ~ShootDeckPiercerSkill()
    {}

    bool init(const ShootDeckPiercerSkillTemplate* t, Robot* robot,
              unsigned int level);

    const ShootDeckPiercerSkillTemplate* getTemplate() const
    {
        return static_cast<const ShootDeckPiercerSkillTemplate*>(m_template);
    }
};

} // end of namespace bot

#endif

