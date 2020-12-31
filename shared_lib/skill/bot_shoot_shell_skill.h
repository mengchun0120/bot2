#ifndef INCLUDE_BOT_SHOOT_SHELL_SKILL
#define INCLUDE_BOT_SHOOT_SHELL_SKILL

#include "skill/bot_shoot_shell_skill_template.h"
#include "skill/bot_shoot_missile_skill.h"

namespace bot {

class ShootShellSkill: public ShootMissileSkill {
public:
    ShootShellSkill();

    virtual ~ShootShellSkill()
    {}

    bool init(const ShootShellSkillTemplate* t, Robot* robot,
              unsigned int level);

    const ShootShellSkillTemplate* getTemplate() const
    {
        return static_cast<const ShootShellSkillTemplate*>(m_template);
    }
};

} // end of namespace bot

#endif

