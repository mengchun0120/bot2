#include "misc/bot_log.h"
#include "misc/bot_json_utils.h"
#include "gameobj/bot_deck_piercer_template.h"
#include "gameutil/bot_game_lib.h"

namespace bot {

DeckPiercerTemplate::DeckPiercerTemplate()
    : m_deathEffectTemplate(nullptr)
    , m_passThroughMask(nullptr)
    , m_speed(0.0f)
    , m_maxPierceTimes(0)
    , m_explodeOnDeath(false)
{
}

bool DeckPiercerTemplate::init(const rapidjson::Value& elem)
{
    if (!MissileTemplate::init(MISSILE_DECK_PIERCER, elem))
    {
        return false;
    }

    std::string deathEffectName;
    std::vector<int> passThroughMaskVec;
    std::vector<JsonParamPtr> params = {
        jsonParam(deathEffectName, "deathEffect", false),
        jsonParam(passThroughMaskVec, "passThroughMask"),
        jsonParam(m_speed, "speed", gt(m_speed, 0.0f)),
        jsonParam(m_maxPierceTimes, "maxPierceTimes", gt(m_maxPierceTimes, 0)),
        jsonParam(m_explodeOnDeath, "explodeOnDeath")
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    const GameLib& lib = GameLib::getInstance();

    if (!deathEffectName.empty())
    {
        m_deathEffectTemplate = lib.getParticleEffectTemplate(deathEffectName);
        if (!m_deathEffectTemplate)
        {
            LOG_ERROR("Failed to find death-effect %s",
                      deathEffectName.c_str());
            return false;
        }
    }

    if (!m_passThroughMask.init(passThroughMaskVec))
    {
        LOG_ERROR("Invalid passThroughMask");
        return false;
    }

    return true;
}

} // end of namespace bot

