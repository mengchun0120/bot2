#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameobj/bot_bullet_template.h"
#include "gameobj/bot_shell_template.h"
#include "gameobj/bot_deck_piercer_template.h"

namespace bot {

MissileTemplate* MissileTemplaet::create(const std::string& name,
                                         const rapidjson::Value& elem)
{
    std::string typeStr;

    if (!parseJson(typeStr, elem, "type"))
    {
        LOG_ERROR("Cannot find type");
        return nullptr;
    }

    MissileType type = strToMissileType(typeStr);
    if (type == MISSILE_INVALID)
    {
        LOG_ERROR("Invalid type %s", typeStr.c_str());
        return nullptr;
    }

    MissileTemplate* t = nullptr;

    switch(type)
    {
        case MISSILE_BULLET:
        {
            t = new BulletTemplate();
            if (!t->init(elem))
            {
                LOG_ERROR("Failed to initialize BulletTemplate");
                delete t;
                return nullptr;
            }
            break;
        }
        case MISSILE_SHELL:
        {
            t = new ShellTemplate();
            if (!t->init(elem))
            {
                LOG_ERROR("Failed to initialize ShellTemplate");
                delete t;
                return nullptr;
            }
            break;
        }
        case MISSILE_DECK_PIERCER:
        {
            t = new DeckPiercerTemplate();
            if (!t->init(elem))
            {
                LOG_ERROR("Failed to initialize DeckPiercerTemplate");
                delete t;
                return nullptr;
            }
            break;
        }
    }

    return t;
}

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

    std::vector<JsonParamPtr> params = {
        jsonParam(m_speed, "speed", gt(m_speed, 0.0f)
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    return true;
}

} // end of namespace bot

