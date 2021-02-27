#include "misc/bot_log.h"
#include "misc/bot_constants.h"
#include "misc/bot_json_utils.h"
#include "misc/bot_math_utils.h"
#include "gameobj/bot_weapon_template.h"

namespace bot {

bool initFirePoints(std::vector<FirePoint> &firePoints,
                    const rapidjson::Value &elem)
{
    std::vector<float> firePos;
    std::vector<float> fireDirection;
    std::vector<JsonParamPtr> params = {
        jsonParam(firePos, "firePos"),
        jsonParam(fireDirection, "fireDirection")
    };

    auto parser = [&](FirePoint &p, const rapidjson::Value &e)->bool
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

        p.m_firePos[0] = firePos[0];
        p.m_firePos[1] = firePos[1];
        p.m_fireDirection[0] = fireDirection[0];
        p.m_fireDirection[1] = fireDirection[1];

        return true;
    };

    if (!parseJsonArray(firePoints, elem, parser, "firePoints"))
    {
        LOG_ERROR("Failed to parse firePoints");
        return false;
    }

    return true;
}

WeaponTemplate::WeaponTemplate()
    : SingleUnitTemplate()
    , m_damage(0.0f)
{}

bool WeaponTemplate::init(const rapidjson::Value &elem)
{
    if (!SingleUnitTemplate::init(elem))
    {
        return false;
    }

    std::vector<JsonParamPtr> params = {
        jsonParam(m_damage, "damage", gt(m_damage, 0.0f))
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    if (!initFirePoints(m_firePoints, elem))
    {
        LOG_ERROR("Failed to initialize fire-points");
        return false;
    }

    return true;
}

} // end of namespace bot

