#include "misc/bot_log.h"
#include "misc/bot_constants.h"
#include "misc/bot_json_utils.h"
#include "misc/bot_math_utils.h"
#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "geometry/bot_rectangle.h"
#include "gametemplate/bot_missile_template.h"
#include "gametemplate/bot_weapon_component_template.h"

namespace bot {

bool WeaponComponentTemplate::init(const NamedMap<Texture>& textureLib, const NamedMap<Rectangle>& rectLib,
                                   const NamedMap<MissileTemplate>& missileLib, const rapidjson::Value& elem)
{
    if (!ComponentTemplate::init(textureLib, rectLib, elem))
    {
        return false;
    }

    std::vector<JsonParseParam> params = {
        {&m_fireDuration, "fireDuration", JSONTYPE_FLOAT},
        {&m_firePower,    "firePower",    JSONTYPE_FLOAT}
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    if (!initMissiles(missileLib, elem))
    {
        LOG_ERROR("Failed to initialize missiles");
        return false;
    }

    if (!initFirePoints(elem))
    {
        LOG_ERROR("Failed to initialize fire-points");
        return false;
    }

    return true;
}

bool WeaponComponentTemplate::initMissiles(const NamedMap<MissileTemplate>& missileLib, const rapidjson::Value& elem)
{
    std::vector<std::string> missileNames;

    if (!parseJson(missileNames, elem, "missiles"))
    {
        return false;
    }

    int missileCount = static_cast<int>(missileNames.size());
    if (missileCount == 0)
    {
        LOG_ERROR("Missile count is zero!");
        return false;
    }

    m_missiles.resize(missileCount);
    for (int i = 0; i < missileCount; ++i)
    {
        const MissileTemplate* t = missileLib.search(missileNames[i]);

        if (!t)
        {
            LOG_ERROR("Failed to find missile %s", missileNames[i].c_str());
            return false;
        }

        m_missiles[i] = t;
    }

    return true;
}

bool WeaponComponentTemplate::initFirePoints(const rapidjson::Value& elem)
{
    std::vector<float> firePos;
    std::vector<float> fireDirection;
    std::vector<JsonParseParam> params = {
        {&firePos,       "firePos",       JSONTYPE_FLOAT_ARRAY},
        {&fireDirection, "fireDirection", JSONTYPE_FLOAT_ARRAY}
    };

    auto parser = [&](FirePoint& p, const rapidjson::Value& e)->bool
    {
        if (!parseJson(params, e))
        {
            return false;
        }

        if (firePos.size() != Constants::NUM_FLOATS_PER_POSITION)
        {
            LOG_ERROR("firePos is of wrong size");
            return false;
        }

        if (fireDirection.size() != Constants::NUM_FLOATS_PER_POSITION)
        {
            LOG_ERROR("fireDirection is of wrong size");
            return false;
        }

        if (!validateDirection(fireDirection[0], fireDirection[1]))
        {
            LOG_ERROR("Invalid fire direction");
            return false;
        }

        p.m_firePos.swap(firePos);
        p.m_fireDirection.swap(fireDirection);

        return true;
    };

    if (!parseJsonArray(m_firePoints, elem, parser, "firePoints"))
    {
        LOG_ERROR("Failed to parse firePoints");
        return false;
    }

    return true;
}

} // end of namespace bot

