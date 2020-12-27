#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameutil/bot_game_lib.h"
#include "gameobj/bot_robot_template.h"

namespace bot {

RobotTemplate::RobotTemplate()
    : GameObjectTemplate(GAME_OBJ_TYPE_ROBOT)
    , m_baseTemplate(nullptr)
    , m_weaponTemplate(nullptr)
    , m_moverTemplate(nullptr)
{
}

bool RobotTemplate::init(const rapidjson::Value& elem)
{
    if (!GameObjectTemplate::init(elem))
    {
        return false;
    }

    std::string baseName, weaponName, moverName;
    std::vector<std::string> skillTemplateNames;
    std::vector<JsonParamPtr> params = {
        jsonParam(baseName, "base"),
        jsonParam(weaponName, "weapon"),
        jsonParam(moverName, "mover"),
        jsonParam(skillTemplateNames, "skills")
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    const GameLib& lib = GameLib::getInstance();

    m_baseTemplate = lib.getBaseTemplate(baseName);
    if (!m_baseTemplate)
    {
        LOG_ERROR("Failed to find base template %s", baseName.c_str());
        return false;
    }

    m_weaponTemplate = lib.getWeaponTemplate(weaponName);
    if (!m_weaponTemplate)
    {
        LOG_ERROR("Failed to find weapon template %s", weaponName.c_str());
        return false;
    }

    m_moverTemplate = lib.getMoverTemplate(moverName);
    if (!m_moverTemplate)
    {
        LOG_ERROR("Failed to find mover template %s", moverName.c_str());
        return false;
    }

    if (skillTemplateNames.empty())
    {
        LOG_ERROR("Skills must be non-empty");
        return false;
    }

    int skillCount = skillTemplateNames.size();

    m_skillTemplates.resize(skillCount);
    for (int i = 0; i < skillCount; ++i)
    {
        const SkillTemplate* t = lib.getSkillTemplate(skillTemplateNames[i]);
        if (!t)
        {
            LOG_ERROR("Failed to find SkillTemplate %s", skillTemplateNames[i]);
            return false;
        }
    }

    return true;
}

} // end of namespace bot

