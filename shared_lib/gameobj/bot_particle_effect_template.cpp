#include "misc/bot_log.h"
#include "misc/bot_constants.h"
#include "misc/bot_json_utils.h"
#include "gameutil/bot_game_lib.h"

namespace bot {

ParticleEffectTemplate::ParticleEffectTemplate()
    : GameObjectTemplate(GAME_OBJ_TYPE_PARTICLE_EFFECT)
    , m_numParticles(0)
    , m_acceleration(0.0f)
    , m_initSpeed(0.0f)
    , m_duration(0.0f)
    , m_texture(nullptr)
    , m_color(nullptr)
{}

bool ParticleEffectTemplate::init(const rapidjson::Value& elem)
{
    if (!GameObjectTemplate::init(elem))
    {
        return false;
    }

    std::vector<float> data;
    std::string textureName, colorName;
    std::vector<JsonParamPtr> params =
    {
        jsonParam(m_acceleration, "acceleration", gt(m_acceleration, 0.0f)),
        jsonParam(m_initSpeed, "initSpeed", gt(m_initSpeed, 0.0f)),
        jsonParam(m_duration, "duration", gt(m_duration, 0.0f)),
        jsonParam(m_particleSize, "particleSize", gt(m_particleSize, 0.0f)),
        jsonParam(textureName, "texture"),
        jsonParam(colorName, "color"),
        jsonParam(data, "particles")
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
        LOG_ERROR("Length of particles must be multiples of %d",
                  NUM_FLOATS_PER_PARTICLE);
        return false;
    }

    m_numParticles = static_cast<int>(data.size()) / NUM_FLOATS_PER_PARTICLE;

    const GameLib& lib = GameLib::getInstance();

    m_texture = lib.getTexture(textureName);
    if (!m_texture)
    {
        LOG_ERROR("Cannot find texture %s", textureName.c_str());
        return false;
    }

    m_color = lib.getColor(colorName);
    if (!m_color)
    {
        LOG_ERROR("Cannot find color %s", colorName.c_str());
        return false;
    }

    const int VERTEX_SIZE = Constants::POSITION_SIZE;
    bool ret = m_vertexArray.load(data.data(), m_numParticles,
                                  VERTEX_SIZE, VERTEX_SIZE);
    if (!ret)
    {
        return false;
    }

    return true;
}

} // end of namespace bot
