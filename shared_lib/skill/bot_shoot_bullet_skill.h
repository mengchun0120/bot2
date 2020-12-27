#ifndef INCLUDE_BOT_SHOOT_BULLET_SKILL
#define INCLUDE_BOT_SHOOT_BULLET_SKILL

#include "skill/bot_shoot_bullet_skill_template.h"
#include "skill/bot_skill.h"

namespace bot {

class ShootBulletSkill: public Skill {
public:
    ShootBulletSkill();

    virtual ~ShootBulletSkill()
    {}

    bool init(const SkillTemplate* t, Robot* robot, unsigned int level);

    const ShootBulletSkillTemplate* getTemplate() const
    {
        return static_cast<const ShootBulletSkillTemplate*>(m_template);
    }

    virtual void apply(GameScreen& screen, const TimePoint& t);

    virtual bool available(const TimePoint& t) const;
};

} // end of namespace bot

#endif

