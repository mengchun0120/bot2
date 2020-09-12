#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "gametemplate/bot_base_component_template.h"
#include "gametemplate/bot_weapon_component_template.h"
#include "gametemplate/bot_mover_component_template.h"
#include "gametemplate/bot_missile_template.h"
#include "gameutil/bot_player_data.h"

namespace bot {

bool loadBaseTemplates(std::vector<const BaseComponentTemplate*>& baseTemplates,
                       const std::vector<std::string>& baseNames,
                       const NamedMap<ComponentTemplate>& componentLib)
{
    int count = static_cast<int>(baseNames.size());

    baseTemplates.resize(count);
    for (int i = 0; i < count; ++i)
    {
        const BaseComponentTemplate* t = getBaseTemplate(componentLib, baseNames[i]);
        if (!t)
        {
            LOG_ERROR("Failed to find base %s", baseNames[i].c_str());
            return false;
        }

        baseTemplates[i] = t;
    }

    return true;
}

bool loadWeaponTemplates(std::vector<const WeaponComponentTemplate*>& weaponTemplates,
                         const std::vector<std::string>& weaponNames,
                         const NamedMap<ComponentTemplate>& componentLib)
{
    int count = static_cast<int>(weaponNames.size());

    weaponTemplates.resize(count);
    for (int i = 0; i < count; ++i)
    {
        const WeaponComponentTemplate* t = getWeaponTemplate(componentLib, weaponNames[i]);
        if (!t)
        {
            LOG_ERROR("Failed to find weapon %s", weaponNames[i].c_str());
            return false;
        }

        weaponTemplates[i] = t;
    }

    return true;
}

bool loadMoverTemplates(std::vector<const MoverComponentTemplate*>& moverTemplates,
                        const std::vector<std::string>& moverNames,
                        const NamedMap<ComponentTemplate>& componentLib)
{
    int count = static_cast<int>(moverNames.size());

    moverTemplates.resize(count);
    for (int i = 0; i < count; ++i)
    {
        const MoverComponentTemplate* t = getMoverTemplate(componentLib, moverNames[i]);
        if (!t)
        {
            LOG_ERROR("Failed to find mover %s", moverNames[i].c_str());
            return false;
        }

        moverTemplates[i] = t;
    }

    return true;
}

PlayerData::PlayerData()
    : m_goldCount(0)
    , m_experience(0)
    , m_extraHP(0.0f)
    , m_extraArmor(0.0f)
    , m_extraPower(0.0f)
    , m_extraCapacity(0)
    , m_lastBaseTemplate(nullptr)
    , m_lastWeaponTemplate(nullptr)
    , m_lastMoverTemplate(nullptr)
    , m_lastMissileTemplate(nullptr)
{}

bool PlayerData::load(const std::string& fileName, const NamedMap<ComponentTemplate>& componentLib,
                      const NamedMap<MissileTemplate>& missileLib)
{
    rapidjson::Document doc;

    if (!readJson(doc, fileName.c_str()))
    {
        LOG_ERROR("Failed to parse json from %s", fileName.c_str());
        return false;
    }

    if (!doc.IsObject())
    {
        LOG_ERROR("Json format is wrong");
        return false;
    }

    rapidjson::Value elem = doc.GetObject();
    std::string lastBaseName, lastWeaponName, lastMoverName, lastMissileName;
    std::vector<JsonParseParam> params = {
        {&m_goldCount,        "goldCount",        JSONTYPE_INT},
        {&m_experience,       "experience",       JSONTYPE_FLOAT},
        {&m_extraHP,          "extraHP",          JSONTYPE_FLOAT},
        {&m_extraArmor,       "extraArmor",       JSONTYPE_FLOAT},
        {&m_extraPower,       "extraPower",       JSONTYPE_FLOAT},
        {&m_extraCapacity,    "extraCapacity",    JSONTYPE_INT},
        {&lastBaseName,       "lastBase",         JSONTYPE_STRING},
        {&lastWeaponName,     "lastWeapon",       JSONTYPE_STRING},
        {&lastMoverName,      "lastMover",        JSONTYPE_STRING},
        {&lastMissileName,    "lastMissile",      JSONTYPE_STRING},
        {&m_availBaseNames,   "availBaseNames",   JSONTYPE_STRING_ARRAY},
        {&m_availWeaponNames, "availWeaponNames", JSONTYPE_STRING_ARRAY},
        {&m_availMoverNames,  "availMoverNames",  JSONTYPE_STRING_ARRAY}
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    m_lastBaseTemplate = getBaseTemplate(componentLib, lastBaseName);
    if (!m_lastBaseTemplate)
    {
        LOG_ERROR("Failed to find base %s", lastBaseName.c_str());
        return false;
    }

    m_lastWeaponTemplate = getWeaponTemplate(componentLib, lastWeaponName);
    if (!m_lastWeaponTemplate)
    {
        LOG_ERROR("Failed to find weapon %s", lastWeaponName.c_str());
        return false;
    }

    m_lastMoverTemplate = getMoverTemplate(componentLib, lastMoverName);
    if (!m_lastMoverTemplate)
    {
        LOG_ERROR("Failed to find mover %s", lastMoverName.c_str());
        return false;
    }

    m_lastMissileTemplate = missileLib.search(lastMissileName);
    if (!m_lastMissileTemplate)
    {
        LOG_ERROR("Failed to find missile %s", lastMissileName.c_str());
        return false;
    }

    if (!loadBaseTemplates(m_availBaseTemplates, m_availBaseNames, componentLib))
    {
        return false;
    }

    if (!loadWeaponTemplates(m_availWeaponTemplates, m_availWeaponNames, componentLib))
    {
        return false;
    }

    if (!loadMoverTemplates(m_availMoverTemplates, m_availMoverNames, componentLib))
    {
        return false;
    }

    return true;
}

} // end of namespace bot

