#include "skill/bot_shoot_shell_skill_template.h"

namespace bot {

ShootShellSkillTemplate::ShootShellSkillTemplate()
    : ShootMissileSkillTemplate()
{
}

bool ShootShellSkillTemplate::init(const rapidjson::Value& elem)
{
    return ShootMissileSkillTemplate::init(SKILL_SHOOT_SHELL, elem);
}

} // end of namespace bot

