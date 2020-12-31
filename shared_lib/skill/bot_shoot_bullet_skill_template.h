#ifndef INCLUDE_BOT_SHOOT_BULLET_SKILL_TEMPLATE
#define INCLUDE_BOT_SHOOT_BULLET_SKILL_TEMPLATE

#include "skill/bot_shoot_missile_skill_template.h"

namespace bot {

class ShootBulletSkillTemplate: public ShootMissileSkillTemplate {
public:
    ShootBulletSkillTemplate();

    virtual ~ShootBulletSkillTemplate()
    {}

    bool init(const rapidjson::Value& elem);
};

} // end of namespace bot

#endif

