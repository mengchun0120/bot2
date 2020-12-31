#include "skill/bot_shoot_bullet_skill.h"

namespace bot {

ShootBulletSkill::ShootBulletSkill()
    : ShootMissileSkill()
{
}

bool ShootBulletSkill::init(const ShootBulletSkillTemplate* t, Robot* robot,
                            unsigned int level)
{
    return ShootBulletSkill::init(t, robot, level);
}

} // end of namespace bot

