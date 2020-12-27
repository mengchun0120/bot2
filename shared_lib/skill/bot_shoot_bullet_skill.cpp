#include "misc/bot_log.h"
#include "skill/bot_shoot_bullet_skill.h"
#include "screen/bot_game_screen.h"

namespace bot {

ShootBulletSkill::ShootBulletSkill()
    : Skill()
{
}

bool ShootBulletSkill::init(const SkillTemplate* t, Robot* robot,
                            unsigned int level)
{
    if (t->getType() != SKILL_SHOOT_BULLET)
    {
        LOG_ERROR("Invalid SkillType %d", static_cast<int>(t->getType()));
        return false;
    }

    return Sill::init(t, robot, level);
}

void ShootBulletSkill::apply(GameScreen& screen, const TimePoint& t)
{
}

bool ShootBulletSkill::available(const TimePoint& t) const
{
    return Skill::available(t);
}

} // end of namespace bot

