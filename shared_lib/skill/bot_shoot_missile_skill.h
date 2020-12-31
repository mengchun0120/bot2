#ifndef INCLUDE_BOT_SHOOT_MISSILE_SKILL
#define INCLUDE_BOT_SHOOT_MISSILE_SKILL

#include "skill/bot_skill.h"

namespace bot {

class ShootMissileSkill: public Skill {
public:
    ShootMissileSkill();

    virtual ~ShootMissileSkill()
    {}

    bool init(const SkillTemplate* t, Robot* robot, unsigned int level);

    virtual void apply(GameScreen& screen, const TimePoint& tp);
};

} // end of namespace bot

#endif

