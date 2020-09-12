#include "misc/bot_json_utils.h"
#include "gametemplate/bot_game_object_template.h"

namespace bot {

GameObjectTemplate::GameObjectTemplate(GameObjectType type)
    : m_type(type)
    , m_coverBreathX(0.0f)
    , m_coverBreathY(0.0f)
    , m_collideBreathX(0.0f)
    , m_collideBreathY(0.0f)
    , m_flags(0)
{}

bool GameObjectTemplate::init(const rapidjson::Value& elem)
{
    std::vector<JsonParseParam> params =
    {
        {&m_coverBreathX,    "coverBreathX",    JSONTYPE_FLOAT},
        {&m_coverBreathY,    "coverBreathY",    JSONTYPE_FLOAT},
        {&m_collideBreathX,  "collideBreathX",  JSONTYPE_FLOAT},
        {&m_collideBreathY,  "collideBreathY",  JSONTYPE_FLOAT}
    };

    return parseJson(params, elem);
}

} // end of namespace bot
