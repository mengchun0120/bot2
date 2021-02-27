#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameobj/bot_bullet_template.h"
#include "gameobj/bot_shell_template.h"
#include "gameobj/bot_deck_piercer_template.h"
#include "gameutil/bot_game_lib.h"

namespace bot {

MissileTemplate *MissileTemplate::create(const std::string &name,
                                         const rapidjson::Value &elem)
{
    std::string typeStr;
    std::vector<JsonParamPtr> params = {
        jsonParam(typeStr, "type")
    };

    if (!parseJson(params, elem))
    {
        return nullptr;
    }

    MissileType type = strToMissileType(typeStr);
    if (type == MISSILE_INVALID)
    {
        LOG_ERROR("Invalid type %s", typeStr.c_str());
        return nullptr;
    }

    switch(type)
    {
        case MISSILE_BULLET:
        {
            BulletTemplate *t = new BulletTemplate();
            if (!t->init(elem))
            {
                LOG_ERROR("Failed to initialize BulletTemplate");
                delete t;
                return nullptr;
            }
            return t;
        }
        case MISSILE_SHELL:
        {
            ShellTemplate *t = new ShellTemplate();
            if (!t->init(elem))
            {
                LOG_ERROR("Failed to initialize ShellTemplate");
                delete t;
                return nullptr;
            }
            return t;
        }
        case MISSILE_DECK_PIERCER:
        {
            DeckPiercerTemplate *t = new DeckPiercerTemplate();
            if (!t->init(elem))
            {
                LOG_ERROR("Failed to initialize DeckPiercerTemplate");
                delete t;
                return nullptr;
            }
            return t;
        }
        default:
        {
            LOG_ERROR("Invalid type %d", static_cast<int>(type));
        }
    }

    return nullptr;
}

MissileTemplate::MissileTemplate()
    : GameObjectTemplate(GAME_OBJ_TYPE_MISSILE)
    , SingleUnitTemplate()
    , m_missileType(MISSILE_INVALID)
    , m_impactEffectTemplate(nullptr)
    , m_speed(0.0f)
{
}

bool MissileTemplate::init(MissileType missileType,
                           const rapidjson::Value &elem)
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

    m_missileType = missileType;

    std::string impactEffectName;
    std::vector<JsonParamPtr> params = {
        jsonParam(impactEffectName, "impactEffect"),
        jsonParam(m_speed, "speed", gt(m_speed, 0.0f))
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    if (!impactEffectName.empty())
    {
        const GameLib &lib = GameLib::getInstance();

        m_impactEffectTemplate =
                        lib.getParticleEffectTemplate(impactEffectName);
        if (!m_impactEffectTemplate)
        {
            LOG_ERROR("Failed to find ParticleEffectTemplate %s",
                      impactEffectName.c_str());
            return false;
        }
    }

    return true;
}

} // end of namespace bot

