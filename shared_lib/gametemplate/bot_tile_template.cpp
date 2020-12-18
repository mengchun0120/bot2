#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gametemplate/bot_tile_template.h"
#include "gameobj/bot_game_object_flag.h"

namespace bot {

TileTemplate::TileTemplate()
    : GameObjectTemplate(GAME_OBJ_TYPE_TILE)
    , m_hp(0.0f)
    , m_hpPerLevel(0.0f)
{
}

bool TileTemplate::init(const rapidjson::Value& elem)
{
    if (!GameObjectTemplate::init(elem))
    {
        return false;
    }

    if (!SingleUnitTemplate::init(elem))
    {
        return false;
    }

    float hp, hpPerLevel;
    bool indestructable = false;
    std::vector<JsonParamPtr> params =
    {
        jsonParam(hp, "hp"),
        jsonParam(hpPerLevel, "hpPerLevel"),
        jsonParam(indestructable, "indestructable"),
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    bool success = setHP(hp) && setHPPerLevel(hpPerLevel);
    if (!success)
    {
        return false;
    }

    if (indestructable)
    {
        setFlags(GAME_OBJ_FLAG_INDESTRUCTABLE);
    }
    else
    {
        clearFlags(GAME_OBJ_FLAG_INDESTRUCTABLE);
    }

    return true;
}

bool TileTemplate::setHP(float hp)
{
    if (hp < 0.0f)
    {
        LOG_ERROR("Invalid hp %f", m_hp);
        return false;
    }

    m_hp = hp;
    return true;
}

bool TileTemplate::setHPPerLevel(float hpPerLevel)
{
    if (hpPerLevel < 0.0f)
    {
        LOG_ERROR("Invalid hp-per-level %f", m_hpPerLevel);
        return false;
    }

    m_hpPerLevel = hpPerLevel;
    return true;
}

} // end of namespace bot
