#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "skill/bot_shoot_missile_skill_template.h"

namespace bot {

SkillTemplate *SkillTemplate::create(const std::string &name,
                                     const rapidjson::Value &elem)
{
    std::string typeStr;
    std::vector<JsonParamPtr> params = {
        jsonParam(typeStr, "type")
    };

    if (!parseJson(params, elem))
    {
        return nullptr;
    }

    SkillType type = strToSkillType(typeStr);

    if (!isValidSkillType(type))
    {
        LOG_ERROR("Invalid SkillType %s", typeStr.c_str());
        return nullptr;
    }

    switch (type)
    {
        case SKILL_SHOOT_MISSILE:
        {
            ShootMissileSkillTemplate *t = new ShootMissileSkillTemplate();
            if (!t->init(elem))
            {
                LOG_ERROR("Failed to initialize ShootMissileSkillTemplate");
                delete t;
                return nullptr;
            }
            LOG_INFO("ShootMissileSkillTemplate %s", name.c_str());
            return t;
        }
        default:
        {
            LOG_ERROR("Invalid skill-type %d", static_cast<int>(type));
        }
    }

    return nullptr;
}

SkillTemplate::SkillTemplate()
    : m_type(SKILL_INVALID)
    , m_flags(0)
    , m_powerCost(0.0f)
    , m_cooldownMS(0.0f)
{
}

bool SkillTemplate::init(SkillType type, const rapidjson::Value &elem)
{
    if (!isValidSkillType(type))
    {
        LOG_ERROR("Invalid SkillType %d", static_cast<int>(type));
        return false;
    }

    m_type = type;
    m_flags = 0;

    std::vector<JsonParamPtr> params = {
        jsonParam(m_viewName, "viewName", false),
        jsonParam(m_description, "description", false),
        jsonParam(m_powerCost, "powerCost", ge(m_powerCost, 0.0f)),
        jsonParam(m_cooldownMS, "cooldownMS", ge(m_cooldownMS, 0.0f)),
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    return true;
}

} // end of namespace bot

