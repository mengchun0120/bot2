#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "structure/bot_named_map.h"
#include "opengl/bot_texture.h"
#include "geometry/bot_rectangle.h"
#include "gametemplate/bot_mover_template.h"

namespace bot {

MoverTemplate* MoverTemplate::Parser::create(const std::string& name,
                                             const rapidjson::Value& elem)
{
    MoverTemplate* t = new MoverTemplate();
    if (!t->init(m_textureLib, m_rectLib, elem))
    {
        delete t;
        return nullptr;
    }

    return t;
}

bool MoverTemplate::init(const NamedMap<Texture>& textureLib,
                         const NamedMap<Rectangle>& rectLib,
                         const rapidjson::Value& elem)
{
    if (!SingleUnitTemplate::init(textureLib, rectLib, elem))
    {
        return false;
    }

    float speed, speedPerLevel;
    std::vector<JsonParseParam> params = {
        {&speed,         "speed",         JSONTYPE_FLOAT},
        {&speedPerLevel, "speedPerLevel", JSONTYPE_FLOAT}
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    bool success = setSpeed(speed) && setSpeedPerLevel(speedPerLevel);
    if (!success)
    {
        return false;
    }

    return true;
}

float getSpeed(int level) const;
{
    const float MAX_SPEED = 400.0f;
    float speed = m_speed + m_speedPerLevel * level;

    if (speed > MAX_SPEED)
    {
        speed = MAX_SPEED;
    }

    return speed;
}

bool MoverTemplate::setSpeed(float speed)
{
    if (speed <= 0.0f)
    {
        LOG_ERROR("Invalid speed %f", speed);
        return false;
    }

    m_speed = speed;
    return true;
}

bool MoverTemplate::setSpeedPerLevel(float speedPerLevel)
{
    if (speedPerLevel < 0.0f)
    {
        LOG_ERROR("Invalid speed-per-level %f", speedPerLevel);
        return false;
    }

    m_speedPerLevel = speedPerLevel;
    return true;
}

} // end of namespace bot
