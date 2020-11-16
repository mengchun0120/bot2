#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "gametemplate/bot_base_template.h"
#include "gametemplate/bot_weapon_template.h"
#include "gametemplate/bot_mover_template.h"
#include "gametemplate/bot_robot_template.h"

namespace bot {

RobotTemplate::RobotTemplate()
    : GameObjectTemplate(GAME_OBJ_TYPE_ROBOT)
    , m_baseTemplate(nullptr)
    , m_weaponTemplate(nullptr)
    , m_moverTemplate(nullptr)
{
}

bool RobotTemplate::init(const NamedMap<BaseTemplate>& baseLib,
                         const NamedMap<WeaponTemplate>& weaponLib,
                         const NamedMap<MoverTemplate>& moverLib,
                         const rapidjson::Value& elem)
{
    if (!GameObjectTemplate::init(elem))
    {
        return false;
    }

    std::string baseName, weaponName, moverName;
    std::vector<JsonParamPtr> params = {
        jsonParam(baseName, "base"),
        jsonParam(weaponName, "weapon"),
        jsonParam(moverName, "mover"),
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    m_baseTemplate = baseLib.search(baseName);
    if (!m_baseTemplate)
    {
        LOG_ERROR("Failed to find base template %s", baseName.c_str());
        return false;
    }

    m_weaponTemplate = weaponLib.search(weaponName);
    if (!m_weaponTemplate)
    {
        LOG_ERROR("Failed to find weapon template %s", weaponName.c_str());
        return false;
    }

    m_moverTemplate = moverLib.search(moverName);
    if (!m_moverTemplate)
    {
        LOG_ERROR("Failed to find mover template %s", moverName.c_str());
        return false;
    }

    return true;
}

} // end of namespace bot
