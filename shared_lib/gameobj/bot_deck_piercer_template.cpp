#include "misc/bot_json_utils.h"
#include "gameobj/bot_deck_piercer_template.h"

namespace bot {

DeckPiercerTemplate::DeckPiercerTemplate()
    : MissileTemplate()
    , m_explodeOnDeath(false)
    , m_explodeBreath(0.0f)
{
}

bool DeckPiercerTemplate::init(const rapidjson::Value& elem)
{
    if (!MissileTemplate::init(MISSILE_DECK_PIERCER, elem))
    {
        return false;
    }

    std::vector<int> passThroughMaskVec;
    std::vector<JsonParamPtr> params = {
        jsonParam(passThroughMaskVec, "passThroughMask"),
        jsonParam(m_explodeOnDeath, "explodeOnDeath"),
        jsonParam(m_explodeBreath, "explodeBreath", gt(m_explodeBreath, 0.0f))
    };

    if (!parseJson(params, elem))
    {
        return false;
    }

    if (!m_passThroughMask.init(passThroughMaskVec))
    {
        LOG_ERROR("Invalid passThroughMask");
        return false;
    }

    return true;
}

} // end of namespace bot

