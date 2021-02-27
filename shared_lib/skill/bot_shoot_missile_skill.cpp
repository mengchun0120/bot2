#include "misc/bot_log.h"
#include "gameobj/bot_robot.h"
#include "skill/bot_shoot_missile_skill_template.h"
#include "skill/bot_shoot_missile_skill.h"
#include "screen/bot_game_screen.h"

namespace bot {

ShootMissileSkill::ShootMissileSkill()
    : Skill()
{
}

bool ShootMissileSkill::init(const SkillTemplate *t, Robot *robot,
                             unsigned int level)
{
    if (t->getType() != SKILL_SHOOT_MISSILE)
    {
        LOG_ERROR("Skill type is invalid: %d", static_cast<int>(t->getType()));
        return false;
    }

    return Skill::init(t, robot, level);
}

void ShootMissileSkill::apply(GameScreen &screen, const TimePoint &tp)
{
    const ShootMissileSkillTemplate *t =
                    static_cast<const ShootMissileSkillTemplate*>(m_template);
    float damageMultiplier = t->getDamageMultiplier(m_level);

    m_robot->getWeapon().fireMissile(screen, t->getMissileTemplate(),
                                     damageMultiplier, t->getSpeedMultiplier());

    Skill::apply(screen, tp);

}

} // end of namespace bot

