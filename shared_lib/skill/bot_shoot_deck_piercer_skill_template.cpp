#include "skill/bot_shoot_deck_piercer_skill_template.h"

namespace bot {

ShootDeckPiercerSkillTemplate::ShootDeckPiercerSkillTemplate()
    : ShootMissileSkillTemplate()
{}

bool ShootDeckPiercerSkillTemplate::init(const rapidjson::Value& elem)
{
    return ShootMissileSkillTemplate::init(SKILL_SHOOT_DECK_PIERCER, elem);
}

} // end of namespace bot

