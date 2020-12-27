#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameobj/bot_bullet_template.h"
#include "gameutil/bot_game_lib.h"
#include "skill/bot_shoot_bullet_skill_template.h"

namespace bot {

ShootBulletSkillTemplate::ShootBulletSkillTemplate()
    : AttackSkillTemplate()
    , m_bulletTemplate(nullptr)
{
}

bool ShootBulletSkillTemplate::init(const rapidjson::Value& elem)
{
    if (!AttackSkillTemplate::init(SKILL_SHOOT_BULLET, elem))
    {
        return false;
    }

    m_flags |= SKILL_FLAG_RANGE;

    std::string bulletName;
    std::vector<JsonParamPtr> params = {
        jsonParam(bulletName, "bullet")
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    const GameLib& lib = GameLib::getInstance();

    const MissileTemplate* t = lib.getMissileTemplate(bulletName);
    if (!t)
    {
        LOG_ERROR("Failed to find MissileTemplate %s", bulletName.c_str());
        return false;
    }

    if (t->getMissileType() != MISSILE_BULLET)
    {
        LOG_ERROR("Invalid bullet %s", bulletName.c_str());
        return false;
    }

    m_bulletTemplate = static_cast<const BulletTemplate*>(t);

    return true;
}

} // end of namespace bot

