#include "skill/bot_shoot_skill.h"

namespace bot {

ShootSkill::ShootSkill()
    : Skill()
{
}

bool ShootSkill::init(const ShootSkillTemplate* t, Robot* robot,
                      unsigned int level)
{
    if (!Skill::init(t, robot, level))
    {
        return false;
    }

    return true;
}

void ShootSkill::apply(GameScreen& screen, const TimePoint& t)
{

}

} // end of namespace bot

