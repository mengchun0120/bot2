#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "misc/bot_shoot_bullet_skill_template.h"
#include "misc/bot_shoot_shell_skill_template.h"
#include "misc/bot_shoot_deck_piercer_template.h"

namespace bot {

SkillTemplate* SkillTemplate::create(const std::string& name,
                                     const rapidjson::Value& elem)
{
    std::string typeStr;

    if (!parseJson(typeStr, elem, "type"))
    {
        LOG_ERROR("Cannot find type");
        return nullptr;
    }

    SkillType type = strToSkillType(typeStr);

    if (!isValidSkillType(type))
    {
        LOG_ERROR("Invalid SkillType %s", typeStr.c_str());
        return nullptr;
    }

    SkillTemplate* t = nullptr;

    switch (type)
    {
        case SKILL_SHOOT_BULLET:
        {
            t = new ShootBulletSkillTemplate();
            if (!t->init(elem))
            {
                LOG_ERROR("Failed to initialize ShootBulletSkillTemplate");
            }

            break;
        }
        case SKILL_SHOOT_SHELL:
        {
            t = new ShootShellSkillTemplate();
            if (!t->init(elem))
            {
                LOG_ERROR("Failed to initialize ShootShellSkillTemplate");
            }

            break;
        }
        case SKILL_SHOOT_DECK_PIERCER:
        {
            t = new ShootDeckPiercerSkillTemplate();
            if (!t->init(elem))
            {
                LOG_ERROR("Failed to initialize ShootDeckPiercerSkillTemplate");
            }

            break;
        }
    }

    return t;
}

SkillTemplate::SkillTemplate()
    : m_type(SKILL_INVALID)
    , m_flags(0)
    , m_powerCost(0.0f)
    , m_cooldownMS(0.0f)
{
}

bool SkillTemplate::init(SkillType type, const rapidjson::Value& elem)
{
    if (!isValidSkillType(type))
    {
        LOG_ERROR("Invalid SkillType %d", static_cast<int>(type));
        return false;
    }

    m_type = type;

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

