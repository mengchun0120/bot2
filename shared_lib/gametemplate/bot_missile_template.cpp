#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"
#include "geometry/bot_rectangle.h"
#include "gametemplate/bot_missile_template.h"
#include "app/bot_app.h"

namespace bot {

MissileTemplate* MissileTemplate::Parser::create(const std::string& name, const rapidjson::Value& elem)
{
    MissileTemplate* t = new MissileTemplate();
    if (!t->init(m_textureLib, m_rectLib, m_particleEffectLib, m_colorLib, elem))
    {
        delete t;
        return nullptr;
    }

    return t;
}

MissileTemplate::MissileTemplate()
    : GameObjectTemplate(GAME_OBJ_TYPE_MISSILE)
    , m_speed(0.0f)
    , m_explosionPower(0)
    , m_explosionBreath(0.0f)
    , m_texture(nullptr)
    , m_rect(nullptr)
    , m_color(nullptr)
    , m_explosionTemplate(nullptr)
{

}

bool MissileTemplate::init(const NamedMap<Texture>& textureLib,
                           const NamedMap<Rectangle>& rectLib,
                           const NamedMap<ParticleEffectTemplate>& particleEffectLib,
                           const NamedMap<Color>& colorLib,
                           const rapidjson::Value& elem)
{
    std::string textureName;
    std::string rectName;
    std::string colorName;
    std::string explosionEffectName;

    std::vector<JsonParseParam> params =
    {
        {&m_coverBreathX,       "coverBreathX",       JSONTYPE_FLOAT},
        {&m_coverBreathY,       "coverBreathY",       JSONTYPE_FLOAT},
        {&m_collideBreathX,     "collideBreathX",     JSONTYPE_FLOAT},
        {&m_collideBreathY,     "collideBreathY",     JSONTYPE_FLOAT},
        {&m_speed,              "speed",              JSONTYPE_FLOAT},
        {&m_explosionPower,     "explosionPower",     JSONTYPE_INT},
        {&m_explosionBreath,    "explosionBreath",    JSONTYPE_FLOAT},
        {&textureName,          "texture",            JSONTYPE_STRING},
        {&rectName,             "rect",               JSONTYPE_STRING},
        {&colorName,            "color",              JSONTYPE_STRING},
        {&explosionEffectName,  "explosionEffect",   JSONTYPE_STRING}
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    m_texture = textureLib.search(textureName);
    if (!m_texture)
    {
        LOG_ERROR("Couldn't find texture %s", textureName.c_str());
        return false;
    }

    m_rect = rectLib.search(rectName);
    if (!m_rect)
    {
        LOG_ERROR("Couldn't find rect %s", rectName.c_str());
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
        LOG_ERROR("Couldn't find particle effect %s", explosionEffectName.c_str());
        return false;
    }

    return true;
}

} // end of namespace bot
