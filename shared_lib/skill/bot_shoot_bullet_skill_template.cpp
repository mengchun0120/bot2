#include "misc/bot_log.h"
#include "skill/bot_shoot_bullet_skill_template.h"

namespace bot {

ShootBulletSkillTemplate::ShootBulletSkillTemplate()
    : ShootMissileSkillTemplate()
{
}

bool ShootBulletSkillTemplate::init(const rapidjson::Value& elem)
{
    return ShootMissileSkillTemplate::init(SKILL_SHOOT_BULLET, elem);
}

} // end of namespace bot

