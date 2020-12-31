#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameutil/bot_game_lib.h"
#include "skill/bot_shoot_missile_skill_template.h"

namespace bot {

ShootMissileSkillTemplate::ShootMissileSkillTemplate()
    : AttackSkillTemplate()
    , m_missileTemplate(nullptr)
    , m_speedMultiplier(1.0f)
{
}

bool ShootMissileSkillTemplate::init(SkillType skillType,
                                     const rapidjson::Value& elem)
{
    if (!AttackSkillTemplate::init(skillType, elem))
    {
        return false;
    }

    std::string missileName;
    std::vector<JsonParamPtr> params = {
        jsonParam(missileName, "missile"),
        jsonParam(m_speedMultiplier, "speedMultiplier",
                  gt(m_speedMultiplier, 0.0f))
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    const GameLib& lib = GameLib::getInstance();

    m_missileTemplate = lib.getMissileTemplate(missileName);
    if (!m_missileTemplate)
    {
        LOG_ERROR("Failed to find MissileTemplate %s", missileName.c_str());
        return false;
    }

    m_flags |= SKILL_FLAG_RANGE;

    return true;
}

} // end of namespace bot

