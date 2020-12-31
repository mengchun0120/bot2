#include "skill/bot_shoot_deck_piercer_skill.h"

namespace bot {

ShootDeckPiercerSkill::ShootDeckPiercerSkill()
    : ShootMissileSkill()
{
}

bool ShootDeckPiercerSkill::init(const ShootDeckPiercerSkillTemplate* t,
                                 Robot* robot,
                                 unsigned int level)
{
    return ShootMissileSkill::init(t, robot, level);
}

}

