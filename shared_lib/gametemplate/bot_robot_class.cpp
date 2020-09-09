#include "misc/bot_log.h"
#include "misc/bot_constants.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "gametemplate/bot_base_component_template.h"
#include "gametemplate/bot_weapon_component_template.h"
#include "gametemplate/bot_mover_component_template.h"
#include "gametemplate/bot_robot_class.h"

namespace bot {

RobotClass* RobotClass::Parser::create(const std::string& name, const rapidjson::Value& elem)
{
    RobotClass* robotClass = new RobotClass();

    if (!robotClass->init(m_componentLib, elem))
    {
        delete robotClass;
        return nullptr;
    }

    return robotClass;
}

bool RobotClass::init(const NamedMap<ComponentTemplate>& componentLib, const rapidjson::Value& elem)
{
    std::vector<std::string> baseNames, weaponNames, moverNames;
    std::vector<JsonParseParam> params = {
        {&m_coverBreathX,   "coverBreathX",   JSONTYPE_FLOAT},
        {&m_coverBreathY,   "coverBreathY",   JSONTYPE_FLOAT},
        {&m_collideBreathX, "collideBreathX", JSONTYPE_FLOAT},
        {&m_collideBreathY, "collideBreathY", JSONTYPE_FLOAT},
        {&baseNames,        "bases",          JSONTYPE_STRING_ARRAY},
        {&weaponNames,      "weapons",        JSONTYPE_STRING_ARRAY},
        {&moverNames,       "movers",         JSONTYPE_STRING_ARRAY},
        {&m_weaponPos,      "weaponPos",      JSONTYPE_FLOAT_ARRAY},
        {&m_moverPos,       "moverPos",       JSONTYPE_FLOAT_ARRAY}
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    if (m_weaponPos.size() != Constants::NUM_FLOATS_PER_POSITION)
    {
        LOG_ERROR("weaponPos has wrong size");
        return false;
    }

    if (m_moverPos.size() != Constants::NUM_FLOATS_PER_POSITION)
    {
        LOG_ERROR("moverPos has wrong size");
        return false;
    }

    if (!initBases(componentLib, baseNames))
    {
        LOG_ERROR("Failed to initialize bases");
        return false;
    }

    if (!initWeapons(componentLib, weaponNames))
    {
        LOG_ERROR("Failed to initialize weapons");
        return false;
    }

    if (!initMovers(componentLib, moverNames))
    {
        LOG_ERROR("Failed to initialize movers");
        return false;
    }

    return true;
}

bool RobotClass::initBases(const NamedMap<ComponentTemplate>& componentLib, const std::vector<std::string>& baseNames)
{
    int baseCount = static_cast<int>(baseNames.size());
    if (baseCount == 0)
    {
        LOG_ERROR("Base-array is empty");
        return false;
    }

    m_bases.resize(baseCount);
    for (int i = 0; i < baseCount; ++i)
    {
        const ComponentTemplate* t = componentLib.search(baseNames[i]);
        if (!t)
        {
            LOG_ERROR("Failed to find base %s", baseNames[i].c_str());
            return false;
        }

        if (t->getType() != COMPONENT_BASE)
        {
            LOG_ERROR("Wrong type for base: %d", static_cast<int>(t->getType()));
            return false;
        }

        m_bases[i] = static_cast<const BaseComponentTemplate*>(t);
    }

    return true;
}

bool RobotClass::initWeapons(const NamedMap<ComponentTemplate>& componentLib, const std::vector<std::string>& weaponNames)
{
    int weaponCount = static_cast<int>(weaponNames.size());
    if (weaponCount == 0)
    {
        LOG_ERROR("Weapon-array is empty");
        return false;
    }

    m_weapons.resize(weaponCount);
    for (int i = 0; i < weaponCount; ++i)
    {
        const ComponentTemplate* t = componentLib.search(weaponNames[i]);
        if (!t)
        {
            LOG_ERROR("Failed to find weapon %s", weaponNames[i].c_str());
            return false;
        }

        if (t->getType() != COMPONENT_WEAPON)
        {
            LOG_ERROR("Wrong type for weapon: %d", static_cast<int>(t->getType()));
            return false;
        }

        m_weapons[i] = static_cast<const WeaponComponentTemplate*>(t);
    }

    return true;
}

bool RobotClass::initMovers(const NamedMap<ComponentTemplate>& componentLib, const std::vector<std::string>& moverNames)
{
    int moverCount = static_cast<int>(moverNames.size());
    if (moverCount == 0)
    {
        return true;
    }

    m_movers.resize(moverCount);
    for (int i = 0; i < moverCount; ++i)
    {
        const ComponentTemplate* t = componentLib.search(moverNames[i]);
        if (!t)
        {
            LOG_ERROR("Failed to find mover %s", moverNames[i].c_str());
            return false;
        }

        if (t->getType() != COMPONENT_MOVER)
        {
            LOG_ERROR("Wrong type for mover: %d", static_cast<int>(t->getType()));
            return false;
        }

        m_movers[i] = static_cast<const MoverComponentTemplate*>(t);
    }

    return true;
}

} // end of namespace bot

