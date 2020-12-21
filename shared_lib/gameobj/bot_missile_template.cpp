#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameobj/bot_missile_template.h"

namespace bot {

MissileTemplate::MissileTemplate()
    : GameObjectTemplate(GAME_OBJ_TYPE_MISSILE)
    , m_missileType(MISSILE_INVALID)
    , m_speed(0.0f)
{
}

bool MissileTemplate::init(MissileType missileType,
                           const rapidjson::Value& elem)
{
    if (!GameObjectTemplate::init(elem))
    {
        return false;
    }

    if (!SingleUnitTemplate::init(elem))
    {
        return false;
    }

    if (!isValidMissileType(missileType))
    {
        LOG_ERROR("Invalid missile type %d", static_cast<int>(missileType));
        return false;
    }

    return true;
}

} // end of namespace bot

