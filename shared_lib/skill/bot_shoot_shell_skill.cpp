#include "skill/bot_shoot_shell_skill.h"

namespace bot {

ShootShellSkill::ShootShellSkill()
    : ShootMissileSkill()
{
}

bool ShootShellSkill::init(const ShootShellSkillTemplate* t, Robot* robot,
                           unsigned int level)
{
    return ShootMissileSkill::init(t, robot, level);
}

} // end of namespace bot

