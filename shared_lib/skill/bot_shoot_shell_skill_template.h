#ifndef INCLUDE_BOT_SHOOT_SHELL_SKILL_TEMPLATE
#define INCLUDE_BOT_SHOOT_SHELL_SKILL_TEMPLATE

#include "skill/bot_attack_skill_template.h"

namespace bot {

class ShellTemplate;

class ShootShellSkillTemplate: public AttackSkillTemplate {
public:
    ShootShellSkillTemplate();

    virtual ~ShootShellSkillTemplate()
    {}

    bool init(const rapidjson::Value& elem);

    const ShellTemplate* getShellTemplate() const
    {
        return m_shellTemplate;
    }

    float getSpeedMultiplier() const
    {
        return m_speedMultiplier;
    }

protected:
    const ShellTemplate* m_shellTemplate;
    float m_speedMultiplier;
};

} // end of namespace bot

#endif

