#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "skill/bot_shoot_bullet_skill_template.h"
#include "skill/bot_shoot_shell_skill_template.h"
#include "skill/bot_shoot_deck_piercer_skill_template.h"

namespace bot {

SkillTemplate* SkillTemplate::create(const std::string& name,
                                     const rapidjson::Value& elem)
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
        case SKILL_SHOOT_BULLET:
        {
            ShootBulletSkillTemplate *t = new ShootBulletSkillTemplate();
            if (!t->init(elem))
            {
                LOG_ERROR("Failed to initialize ShootBulletSkillTemplate");
                delete t;
                return nullptr;
            }
            return t;
        }
        case SKILL_SHOOT_SHELL:
        {
            ShootShellSkillTemplate* t = new ShootShellSkillTemplate();
            if (!t->init(elem))
            {
                LOG_ERROR("Failed to initialize ShootShellSkillTemplate");
                delete t;
                return nullptr;
            }
            return t;
        }
        case SKILL_SHOOT_DECK_PIERCER:
        {
            ShootDeckPiercerSkillTemplate* t =
                                        new ShootDeckPiercerSkillTemplate();
            if (!t->init(elem))
            {
                LOG_ERROR("Failed to initialize ShootDeckPiercerSkillTemplate");
                delete t;
                return nullptr;
            }
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

bool SkillTemplate::init(SkillType type, const rapidjson::Value& elem)
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

