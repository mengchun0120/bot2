#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameutil/bot_game_lib.h"
#include "skill/bot_shoot_skill_template.h"

namespace bot {

ShootSkillTemplate::ShootSkillTemplate()
    : SkillTemplate()
    , m_missile(nullptr)
{
}

bool ShootSkillTemplate::init(const rapidjson::Value& elem)
{
    m_type = SKILL_SHOOT;

    if (!SkillTemplate::init(elem))
    {
        return false;
    }

    std::string missileName;
    std::vector<JsonParamPtr> params = {
        jsonParam(missileName, "missile"),
        jsonParam(m_baseDamageMultiplier, "baseDamageMultiplier",
                  gt(m_baseDamageMultiplier, 0.0f)),
        jsonParam(m_damageMultiplierPerLevel, "damageMultiplierPerLevel",
                  ge(m_damageMultiplierPerLevel, 0.0f)),
        jsonParam(m_speedMultiplier, "speedMultiplier",
                  gt(m_speedMultiplier, 0.0f))
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    const GameLib& lib = GameLib::getInstance();

    m_missile = lib.getMissileTemplate(missileName);
    if (!m_missile)
    {
        LOG_ERROR("Failed to find missile-template %s", missileName.c_str());
        return false;
    }

    return true;
}

} // end of namespace bot

