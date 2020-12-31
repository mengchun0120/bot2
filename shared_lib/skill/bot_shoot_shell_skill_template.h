#ifndef INCLUDE_BOT_SHOOT_SHELL_SKILL_TEMPLATE
#define INCLUDE_BOT_SHOOT_SHELL_SKILL_TEMPLATE

#include "skill/bot_shoot_missile_skill_template.h"

namespace bot {

class ShootShellSkillTemplate: public ShootMissileSkillTemplate {
public:
    ShootShellSkillTemplate();

    virtual ~ShootShellSkillTemplate()
    {}

    bool init(const rapidjson::Value& elem);
};

} // end of namespace bot

#endif

