#ifndef INCLUDE_SHOOT_DECK_PIERCER_SKILL_TEMPLATE
#define INCLUDE_SHOOT_DECK_PIERCER_SKILL_TEMPLATE

#include "skill/bot_shoot_missile_skill_template.h"

namespace bot {

class ShootDeckPiercerSkillTemplate: public ShootMissileSkillTemplate {
public:
    ShootDeckPiercerSkillTemplate();

    virtual ~ShootDeckPiercerSkillTemplate()
    {}

    bool init(const rapidjson::Value& elem);
};


} // end of namespace bo

#endif

