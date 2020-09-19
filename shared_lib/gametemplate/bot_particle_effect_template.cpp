#include "misc/bot_log.h"
#include "misc/bot_constants.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "opengl/bot_color.h"
#include "gametemplate/bot_particle_effect_template.h"

namespace bot {

ParticleEffectTemplate* ParticleEffectTemplate::Parser::create(const std::string& name, const rapidjson::Value& elem)
{
    ParticleEffectTemplate* t = new ParticleEffectTemplate();
    if (!t->init(m_textureLib, m_colorLib, elem))
    {
        delete t;
        return nullptr;
    }
    return t;
}

ParticleEffectTemplate::ParticleEffectTemplate()
    : GameObjectTemplate(GAME_OBJ_TYPE_PARTICLE_EFFECT)
    , m_numParticles(0)
    , m_acceleration(0.0f)
    , m_initSpeed(0.0f)
    , m_duration(0.0f)
    , m_texture(nullptr)
    , m_color(nullptr)
{}

bool ParticleEffectTemplate::init(const NamedMap<Texture>& textureLib, const NamedMap<Color>& colorLib,
                                  const rapidjson::Value& elem)
{
    if (!GameObjectTemplate::init(elem))
    {
        return false;
    }

    std::vector<float> data;
    std::string textureName, colorName;
    std::vector<JsonParseParam> params =
    {
        {&m_acceleration, "acceleration", JSONTYPE_FLOAT},
        {&m_initSpeed,    "initSpeed",    JSONTYPE_FLOAT},
        {&m_duration,     "duration",     JSONTYPE_FLOAT},
        {&m_particleSize, "particleSize", JSONTYPE_FLOAT},
        {&textureName,    "texture",      JSONTYPE_STRING},
        {&colorName,      "color",        JSONTYPE_STRING},
        {&data,           "particles",    JSONTYPE_FLOAT_ARRAY}
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    if (data.size() == 0)
    {
        LOG_ERROR("Particles must NOT be empty");
        return false;
    }

    const int NUM_FLOATS_PER_PARTICLE = Constants::NUM_FLOATS_PER_POSITION;

    if (data.size() % NUM_FLOATS_PER_PARTICLE != 0)
    {
        LOG_ERROR("Length of particles must be multiples of %d", NUM_FLOATS_PER_PARTICLE);
        return false;
    }

    m_numParticles = static_cast<int>(data.size()) / NUM_FLOATS_PER_PARTICLE;

    m_texture = textureLib.search(textureName);
    if (!m_texture)
    {
        LOG_ERROR("Cannot find texture %s", textureName.c_str());
        return false;
    }

    m_color = colorLib.search(colorName);
    if (!m_color)
    {
        LOG_ERROR("Cannot find color %s", colorName.c_str());
        return false;
    }

    const int VERTEX_SIZE = Constants::POSITION_SIZE;

    if (!m_vertexArray.load(data.data(), m_numParticles, VERTEX_SIZE, VERTEX_SIZE))
    {
        return false;
    }

    return true;
}

} // end of namespace bot
