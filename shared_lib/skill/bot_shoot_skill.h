#ifndef INCLUDE_BOT_SHOOT_SKILL
#define INCLUDE_BOT_SHOOT_SKILL

#include "skill/bot_shoot_skill_template.h"
#include "skill/bot_skill.h"

namespace bot {

class ShootSkill: public Skill {
public:
    ShootSkill();

    virtual ~ShootSkill()
    {}

    const ShootSkillTemplate* getTemplate() const
    {
        return static_cast<ShootSkillTemplate*>(m_template);
    }

    bool init(const ShootSkillTemplate* t, Robot* robot, unsigned int level);

    virtual void apply(GameScreen& screen, const TimePoint& t);
};

} // end of namespace bot

#endif

