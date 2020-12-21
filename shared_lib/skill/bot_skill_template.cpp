#include "misc/bot_json_utils.h"
#include "skill/bot_skill_template.h"

namespace bot {

SkillTemplate::SkillTemplate()
    : m_type(SKILL_INVALID)
    , m_powerCost(0.0f)
    , m_cooldownMS(0.0f)
{
}

bool SkillTemplate::init(const rapidjson::Value& elem)
{
    std::vector<JsonParamPtr> params = {
        jsonParam(m_powerCost, "powerCost", ge(m_powerCost, 0.0f)),
        jsonParam(m_cooldownMS, "cooldownMS", ge(m_cooldownMS, 0.0f))
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    return true;
}

} // end of namespace bot

