#include "misc/bot_json_utils.h"
#include "skill/bot_skill.h"

namespace bot {

Skill::Skill()
    : m_type(SKILL_INVALID)
    , m_consumedPower(0.0f)
    , m_cooldownMS(0.0f)
{
}

bool Skill::init(const rapidjson::Value& elem)
{
    std::vector<JsonParamPtr> params = {
        jsonParam(m_consumedPower, "consumedPower", ge(m_consumedPower, 0.0f)),
        jsonParam(m_cooldownMS, "cooldownMS", ge(m_cooldownMS, 0.0f))
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    return true;
}

} // end of namespace bot

