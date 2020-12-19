#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameutil/bot_game_lib.h"

namespace bot {

MoverTemplate::MoverTemplate()
    : m_speed(0.0f)
    , m_speedPerLevel(0.0f)
{
}

bool MoverTemplate::init(const rapidjson::Value& elem)
{
    if (!SingleUnitTemplate::init(elem))
    {
        return false;
    }

    float speed, speedPerLevel;
    std::vector<JsonParamPtr> params = {
        jsonParam(speed, "speed"),
        jsonParam(speedPerLevel, "speedPerLevel", false)
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

float MoverTemplate::getSpeed(int level) const
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
