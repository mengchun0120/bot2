#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameutil/bot_game_lib.h"

namespace bot {

MissileTemplate::MissileTemplate()
    : GameObjectTemplate(GAME_OBJ_TYPE_MISSILE)
    , m_speed(0.0f)
    , m_damage(0.0f)
    , m_damagePerLevel(0.0f)
    , m_explosionBreath(0.0f)
    , m_color(nullptr)
    , m_explosionTemplate(nullptr)
{

}

bool MissileTemplate::init(const rapidjson::Value& elem)
{
    if (!GameObjectTemplate::init(elem))
    {
        return false;
    }

    if (!SingleUnitTemplate::init(elem))
    {
        return false;
    }

    float speed, damage, damagePerLevel = 0.0f, explosionBreath;
    std::string colorName, explosionEffectName;
    std::vector<JsonParamPtr> params =
    {
        jsonParam(speed, "speed"),
        jsonParam(damage, "damage"),
        jsonParam(damagePerLevel, "damagePerLevel", false),
        jsonParam(explosionBreath, "explosionBreath"),
        jsonParam(colorName, "color"),
        jsonParam(explosionEffectName, "explosionEffect")
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    bool success = setSpeed(speed) &&
                   setDamage(damage) &&
                   setDamagePerLevel(damagePerLevel) &&
                   setExplosionBreath(explosionBreath);
    if (!success)
    {
        return false;
    }

    const GameLib& lib = GameLib::getInstance();

    m_color = lib.getColor(colorName);
    if (!m_color)
    {
        LOG_ERROR("Coundn't find color %s", colorName.c_str());
        return false;
    }

    m_explosionTemplate = lib.getParticleEffectTemplate(explosionEffectName);
    if (!m_explosionTemplate)
    {
        LOG_ERROR("Couldn't find particle effect %s",
                  explosionEffectName.c_str());
        return false;
    }

    return true;
}

bool MissileTemplate::setSpeed(float speed)
{
    if (speed < 0.0f)
    {
        LOG_ERROR("Invalid speed %f", speed);
        return false;
    }

    m_speed = speed;
    return true;
}

bool MissileTemplate::setDamage(int damage)
{
    if (damage < 0.0f)
    {
        LOG_ERROR("Invalid damage %f", damage);
        return false;
    }

    m_damage = damage;
    return true;
}

bool MissileTemplate::setDamagePerLevel(float damagePerLevel)
{
    if (damagePerLevel < 0.0f)
    {
        LOG_ERROR("Invalid damage-per-level %f", damagePerLevel);
        return false;
    }

    m_damagePerLevel = damagePerLevel;
    return true;
}

bool MissileTemplate::setExplosionBreath(float explosionBreath)
{
    if (explosionBreath <= 0.0f)
    {
        LOG_ERROR("Invalid explosion-breath %f", explosionBreath);
        return false;
    }

    m_explosionBreath = explosionBreath;
    return true;
}

} // end of namespace bot
