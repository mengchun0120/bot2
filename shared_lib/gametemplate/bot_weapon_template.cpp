#include <algorithm>
#include "misc/bot_log.h"
#include "misc/bot_constants.h"
#include "misc/bot_json_utils.h"
#include "misc/bot_math_utils.h"
#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "geometry/bot_rectangle.h"
#include "gametemplate/bot_missile_template.h"
#include "gametemplate/bot_weapon_template.h"

namespace bot {

bool initFirePoints(std::vector<FirePoint>& firePoints,
                    const rapidjson::Value& elem)
{
    std::vector<float> firePos;
    std::vector<float> fireDirection;
    std::vector<JsonParseParam> params = {
        {
            &firePos,
            "firePos",
            JSONTYPE_FLOAT_ARRAY
        },
        {
            &fireDirection,
            "fireDirection",
            JSONTYPE_FLOAT_ARRAY
        }
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

WeaponTemplate* WeaponTemplate::Parser::create(std::string& name,
                                               const rapidjson::Value& elem)
{
    WeaponTemplate* t = new WeaponTemplate();
    if (!t->init(m_textureLib, m_rectLib, m_missileLib, elem))
    {
        delete t;
        return nullptr;
    }

    return t;
}

WeaponTemplate::WeaponTemplate()
    : m_fireDuration(0.0f)
    , m_fireDurReductionPerLevel(0.0f)
    , m_missileTemplate(nullptr)
{}

bool WeaponTemplate::init(const NamedMap<Texture>& textureLib,
                          const NamedMap<Rectangle>& rectLib,
                          const NamedMap<MissileTemplate>& missileLib,
                          const rapidjson::Value& elem)
{
    if (!SingleUnitTemplate::init(textureLib, rectLib, elem))
    {
        return false;
    }

    float fireDuration, fireDurReductionPerLevel = 0.0f;
    std::string missileName;
    std::vector<JsonParseParam> params = {
        {
            &fireDuration,
            "fireDuration",
            JSONTYPE_FLOAT
        },
        {
            &fireDurReductionPerLevel,
            "fireDurReductionPerLevel",
            JSONTYPE_FLOAT,
            false
        },
        {
            &missileName,
            "missile",
            JSONTYPE_STRING,
            false
        }
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    bool ret = setFireDuration(fireDuration) &&
               setFireDurReductionPerLevel(fireDurReductionPerLevel);
    if (!ret)
    {
        return false;
    }

    if (!missileName.empty())
    {
        m_missileTemplate = missileLib.search(missileName);
        if (!m_missileTemplate)
        {
            LOG_ERROR("Failed to find missile-template %s",
                      missileName.c_str());
            return false;
        }
    }

    if (!initFirePoints(m_firePoints, elem))
    {
        LOG_ERROR("Failed to initialize fire-points");
        return false;
    }

    return true;
}

bool WeaponTemplate::init(const NamedMap<Texture>& textureLib,
                          const NamedMap<Rectangle>& rectLib,
                          const MissileTemplate* missileTemplate,
                          const rapidjson::Value& elem)
{
    if (!SingleUnitTemplate::init(textureLib, rectLib, elem))
    {
        return false;
    }

    float fireDuration, fireDurReductionPerLevel = 0.0f;
    std::vector<JsonParseParam> params = {
        {
            &fireDuration,
            "fireDuration",
            JSONTYPE_FLOAT
        },
        {
            &fireDurReductionPerLevel,
            "fireDurReductionPerLevel",
            JSONTYPE_FLOAT,
            false
        }
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    bool ret = setFireDuration(fireDuration) &&
               setFireDurReductionPerLevel(fireDurReductionPerLevel);
    if (!ret)
    {
        return false;
    }

    if (!initFirePoints(m_firePoints, elem))
    {
        LOG_ERROR("Failed to initialize fire-points");
        return false;
    }

    setMissileTemplate(missileTemplate);

    return true;
}

bool WeaponTemplate::setFireDuration(float duration)
{
    if (duration < 0.0f)
    {
        LOG_ERROR("Invalid duration %f", duration);
        return false;
    }

    m_fireDuration = duration;
    return true;
}

float WeaponTemplate::getFireDuration(int level) const
{
    const float MIN_DURATION = 100.0f;

    float duration = m_fireDuration - m_fireDurReductionPerLevel * level;
    if (duration < MIN_DURATION)
    {
        duration = MIN_DURATION;
    }

    return duration;
}

bool WeaponTemplate::setFireDurReductionPerLevel(float reductionPerLevel)
{
    if (reductionPerLevel < 0.0f)
    {
        LOG_ERROR("Invalid fireDurReductionPerLevel %f", reductionPerLevel);
        return false;
    }

    m_fireDurReductionPerLevel = reductionPerLevel;
    return true;
}

bool WeaponTemplate::setNumFirePoints(int count)
{
    if (count <= 0)
    {
        LOG_ERROR("Invalid num-fire-points %d", count);
        return false;
    }

    m_firePoints.resize(count);
    return true;
}

} // end of namespace bot

