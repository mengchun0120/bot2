#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "skill/bot_attack_skill_template.h"

namespace bot {

AttackSkillTemplate::AttackSkillTemplate()
    : SkillTemplate()
    , m_baseDamageMultiplier(1.0f)
    , m_damageMultiplierPerLevel(0.0f)
{
}

bool AttackSkillTemplate::init(SkillType type, const rapidjson::Value &elem)
{
    if (!SkillTemplate::init(type, elem))
    {
        return false;
    }

    std::vector<JsonParamPtr> params = {
        jsonParam(m_baseDamageMultiplier, "baseDamageMultiplier",
                  gt(m_baseDamageMultiplier, 0.0f)),
        jsonParam(m_damageMultiplierPerLevel, "damageMultiplierPerLevel",
                  ge(m_damageMultiplierPerLevel, 0.0f))
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    m_flags |= SKILL_FLAG_OFFENSIVE;

    return true;
}

} // end of namespace bot

