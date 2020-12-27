#ifndef INCLUDE_BOT_SHOOT_BULLET_SKILL_TEMPLATE
#define INCLUDE_BOT_SHOOT_BULLET_SKILL_TEMPLATE

#include "skill/bot_attack_skill_template.h"

namespace bot {

class BulletTemplate;

class ShootBulletSkillTemplate: public AttackSkillTemplate {
public:
    ShootBulletSkillTemplate();

    virtual ~ShootBulletSkillTemplate()
    {}

    bool init(const rapidjson::Value& elem);

    const BulletTemplate* getBulletTemplate() const
    {
        return m_bulletTemplate;
    }

protected:
    const BulletTemplate* m_bulletTemplate;
};

} // end of namespace bot

#endif

