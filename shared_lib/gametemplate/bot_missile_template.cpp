#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"
#include "geometry/bot_rectangle.h"
#include "gametemplate/bot_particle_effect_template.h"
#include "gametemplate/bot_missile_template.h"

namespace bot {

MissileTemplate* MissileTemplate::Parser::create(const std::string& name,
                                                 const rapidjson::Value& elem)
{
    MissileTemplate* t = new MissileTemplate();
    bool ret = t->init(m_textureLib, m_rectLib, m_particleEffectLib,
                       m_colorLib, elem);
    if (!ret)
    {
        delete t;
        return nullptr;
    }

    return t;
}

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

bool MissileTemplate::init(
                    const NamedMap<Texture>& textureLib,
                    const NamedMap<Rectangle>& rectLib,
                    const NamedMap<ParticleEffectTemplate>& particleEffectLib,
                    const NamedMap<Color>& colorLib,
                    const rapidjson::Value& elem)
{
    if (!GameObjectTemplate::init(elem))
    {
        return false;
    }

    if (!SingleUnitTemplate::init(textureLib, rectLib, elem))
    {
        return false;
    }

    float speed, damage, damagePerLevel = 0.0f, explosionBreath;
    std::string colorName, explosionEffectName;
    std::vector<JsonParseParam> params =
    {
        {
            &speed,
            "speed",
            JSONTYPE_FLOAT
        },
        {
            &damage,
            "damage",
            JSONTYPE_FLOAT
        },
        {
            &damagePerLevel,
            "damagePerLevel",
            JSONTYPE_FLOAT,
            false
        },
        {
            &explosionBreath,
            "explosionBreath",
            JSONTYPE_FLOAT
        },
        {
            &colorName,
            "color",
            JSONTYPE_STRING
        },
        {
            &explosionEffectName,
            "explosionEffect",
            JSONTYPE_STRING
        }
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

    m_color = colorLib.search(colorName);
    if (!m_color)
    {
        LOG_ERROR("Coundn't find color %s", colorName.c_str());
        return false;
    }

    m_explosionTemplate = particleEffectLib.search(explosionEffectName);
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
