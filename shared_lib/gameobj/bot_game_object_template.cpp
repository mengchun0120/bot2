#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameobj/bot_game_object_template.h"

namespace bot {

GameObjectTemplate::GameObjectTemplate(GameObjectType type)
    : m_type(type)
    , m_coverBreath(0.0f)
    , m_collideBreath(0.0f)
    , m_flags(0)
{}

bool GameObjectTemplate::init(const rapidjson::Value& elem)
{
    float coverBreath, collideBreath;
    std::vector<JsonParamPtr> params =
    {
        jsonParam(coverBreath, "coverBreath"),
        jsonParam(collideBreath, "collideBreath")
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    bool success = setCoverBreath(coverBreath) &&
                   setCollideBreath(collideBreath);
    if (!success)
    {
        return false;
    }

    return true;
}

bool GameObjectTemplate::setCoverBreath(float breath)
{
    if (breath <= 0.0f)
    {
        LOG_ERROR("Invalid cover-breath %f", breath);
        return false;
    }

    m_coverBreath = breath;
    return true;
}

bool GameObjectTemplate::setCollideBreath(float breath)
{
    if (breath < 0.0f)
    {
        LOG_ERROR("Invalid collide-breath %f", breath);
        return false;
    }

    m_collideBreath = breath;
    return true;
}

} // end of namespace bot
