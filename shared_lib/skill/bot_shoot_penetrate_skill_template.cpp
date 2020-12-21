#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "skill/bot_shoot_penetrate_skill_template.h"

namespace bot {

ShootPenetrateSkillTemplate::ShootPenetrateSkillTemplate()
    : ShootSkillTemplate()
    , m_penetrateLimit(0)
    , m_damageMultiplierPenetrate(0.0f)
    , m_explodeOnDead(false)
{}

bool ShootPenetrateSkillTemplate::init(const rapidjson::Value& elem)
{
    if (!ShootSkillTemplate::init(elem))
    {
        return false;
    }

    std::vector<JsonParamPtr> params = {
        jsonParam(m_penetrateLimit, "penetrateLimit", ge(m_penetrateLimit, 1)),
        jsonParam(m_damageMultiplierPenetrate, "damageMultiplierPenetrate",
                  gt(m_damageMultiplierPenetrate, 0.0f)),
        jsonParam(m_explodeOnDead, "explodeOnDead")
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    return true;
}

} // end of namespace bot

